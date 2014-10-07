<?php

//This is class for add click and view count functions.

class clsClickView
{
    private $strDate;
    private $strIpAddress;
    private $strRefererLink;
    private $strCDate;
    public $strViewTableName;
    public $strClickTableName;

    public function __construct()
    {
        $this->strDate = date("Y-m-d"); // date value for view table
        $this->strCDate = date("Y-m-d H:i:s"); // date value for click table.

        $this->strIpAddress = $_SERVER['REMOTE_ADDR']; // ; user ip address
        $this->strRefererLink = $_SERVER['HTTP_REFERER'];

        $this->strViewTableName = 'threebl_analytic_views';
        $this->strClickTableName = 'threebl_analytic_clicks';
    }

    /**
     * Insert value in threebl_analytic_views table.
     *
     * @param array  $arrMediaIdWise : array of condition
     * @param string $strFMRType     : FMR media type
     * @param string $strViewDate    : view date information
     * @param int    $intMediaId     : FMR node id
     *
     * @return bool
     */
    public function fnInsertAnalyticsView($arrMediaIdWise = array(), $strFMRType = '', $strViewDate = '', $intMediaId = 0)
    {
        if ($intMediaId == 0) {
            db_insert('threebl_analytic_views')->fields($arrMediaIdWise)->execute();
        } else {
            $strViewIdQry = "UPDATE " . $this->strViewTableName . " SET totalcount = `totalcount`+1  WHERE nid ='" . $intMediaId . "' AND datevisited ='" . $strViewDate . "'";
            db_query($strViewIdQry);
        }
        return true;
    }

    /**
     * Insert the Value in threebl_analytic_clicks
     *
     * @param array $arr3blClicksNew : array of click record data
     *
     * @return bool
     */
    public function fnInsertAnalyticsClick($arr3blClicksNew = array())
    {
        db_insert('threebl_analytic_clicks')->fields($arr3blClicksNew)->execute();
        return true;
    }

    /**
     * Checking Duplicate Record from JM_Media_Id_Wise
     *
     * @param int    $intMediaId   : FMR node id
     * @param int    $intCompanyId : company OG id
     * @param string $strFMRType   : FMR media type
     * @param string $strChannel   : channel type for record
     * @param string $strViewDate  : view date information
     *
     * @return mixed
     */
    public function fnDuplicateAnalyticsView($intMediaId = 0, $intCompanyId = 0, $strFMRType = '', $strChannel = '', $strViewDate = '')
    {
        $strFmrQuery = ($strFMRType == 'Client') ? " AND mediatype='client' " : " AND  mediatype != 'client' ";

        $strDuplicateMediaIdWiseQry = " SELECT count(*) as rec FROM " . $this->strViewTableName . "
         WHERE nid ='" . $intMediaId . "'
        AND mediatype='$strFMRType'
        AND company_ogid ='" . $intCompanyId . "'
        AND channel = '$strChannel'
        AND datevisited ='" . $strViewDate . "'" . $strFmrQuery;

        return db_query($strDuplicateMediaIdWiseQry)->fetchField();
    }


    /**
     * Checking Duplicate Record from threebl_analytic_clicks
     *
     * @param int    $intMediaId   : FMR node id
     * @param int    $intCompanyId : company og id
     * @param string $strFMRType   : FMR media type
     * @param string $strDate      : Click date information
     *
     * @return mixed
     */
    public function fnDuplicateAnalyticsClick($intMediaId = 0, $intCompanyId = 0, $strFMRType = '', $strDate = '')
    {

        $strDuplicate3BLClicksNewQry = "SELECT COUNT(nid) FROM  " . $this->strClickTableName . "
							WHERE nid ='$intMediaId'
							AND company_ogid = '$intCompanyId'
							AND fmr_type = '$strFMRType'
						    AND click_date = '$strDate' ";

        return db_query($strDuplicate3BLClicksNewQry)->fetchField();
    }

    /**
     * Add View Count Function
     *
     * @param array $arrAllView : array contain information about the all views
     */
    public function fnAddAllViews($arrAllView)
    {
        $strViewDate = '';
        extract($arrAllView); // getting the array value by extracting

        $strVDate = ($strViewDate == '') ? $this->strDate : $strViewDate; //getting the date value for view cron.
        #CHECK if record already exist or not
        //Change Master_companyId to CompanyId and master_MediaId to nid in Following Query
        $intExistRecords = $this->fnDuplicateAnalyticsView($intMediaId, $intCompanyId, $strFMRType, $strChannel, $strVDate);

        #Update record if already exist for threebl_analytic_views
        if ($intExistRecords > 0) {
            #UPDATE OR INSERT INFO
            $arrMediaIdWise = array('mediatype' => $strFMRType,
                'company_ogid' => $intCompanyId, //Change Master_companyId to CompanyId and master_MediaId to 3bl NodeId
                'nid' => $intMediaId,
                'channel' => $strChannel
            );

            $this->fnInsertAnalyticsView($arrMediaIdWise, $strFMRType, $strVDate, $intMediaId);

        } else {
            //array of Media_Id_Wise
            $arrMediaIdWise = array(
                "nid" => $intMediaId, //Change master_MediaId to 3bl NodeId
                "mediatype" => $strFMRType,
                "company_ogid" => $intCompanyId, // Change Master_companyId to CompanyId
                "channel" => $strChannel,
                "totalcount" => '1',
                "uniquecount" => '1',
                "datevisited" => $strVDate
            );

            #Insert record if not exist in Jm_Media_Id_Wise
            $this->fnInsertAnalyticsView($arrMediaIdWise, $strFMRType, $strVDate);
        }
    }

    /**
     * Add Click Count Function
     *
     * @param array $arrAllClick : array contain click related data
     * @param int   $int3blCron  : flag indicate function called by cron functionality or not
     *
     * @return bool
     */
    public function fnAddAllClicks($arrAllClick, $int3blCron = 0)
    {
        extract($arrAllClick); // getting the array value by extracting

        $intRecordExist = 0; //set value for 3bl_cron
        $strRefUrl = $this->strRefererLink;
        $strClickIp = $this->strIpAddress;
        $strDate = $this->strCDate;

        if ($int3blCron == 1) {
            // checking  duplicate record exits or not
            $intRecordExist = $this->fnDuplicateAnalyticsClick($intMediaId, $intCompanyId, $strFMRType, $strClickDate); //getting the array value by extracting

            $strRefUrl = $strRefererUrl; // getting the array value by extracting
            $strClickIp = $strHostName; // getting the array value by extracting
            $strDate = $strClickDate;
        }

        //Checking data exits or not  if yes not add.
        if ($intRecordExist == 0) {
            $arrAllClick = array(
                "nid" => $intMediaId, // getting the array value by extracting
                "company_ogid" => $intCompanyId, // Change Master_companyId to CompanyId
                "fmr_type" => $strFMRType, // getting the array value by extracting
                "click_date" => $strDate,
                "click_ip" => $strClickIp,
                "click_referrer" => $strRefUrl,
                "country" => $this->fnGetCountryFromIp($strClickIp), // getting the array value by extracting Of Country
                "channel" => $strChannel, // getting the array value by extracting Of Channel
                "channelid" => $intChannelId, // Widget ID
            );
            #INSERT CLICK IN  threebl_analytic_clicks TABLE
            $this->fnInsertAnalyticsClick($arrAllClick);
            return true;
        } else {
            return false;
        }
    }

    /**
     * Getting the value Country from IP Address
     *
     * @param string $strIpAddress : Ip address of machine
     *
     * @return string
     */
    public function fnGetCountryFromIp($strIpAddress)
    {
        #CONVERT CURRENT USERS IPV4
        $strUserV4Ip = ip2long($strIpAddress);

        if ($strUserV4Ip < 0) {
            $strUserV4Ip = ip2long($strUserV4Ip) + pow(2, 32);
        }

        #Get Country Code from ip address
        $arrCCResult = db_query("SELECT countryLONG FROM ipcountry WHERE ipFROM <= '$strUserV4Ip' AND ipTO >= '$strUserV4Ip'")->fetchAll();
        $strCountryShort = $arrCCResult[0]->countryLONG;
        return addslashes(trim($strCountryShort));
    }

    /**
     * Getting difference media Type
     *
     * @param int $intTable  : flag which indicate table
     * @param int $intOffset : starting value for pagination
     * @param int $intLimit  : number of records per page
     *
     * @return array
     */
    public function fnChangeMediaType($intTable = 0, $intOffset = 0, $intLimit = 0)
    {
        $objArrMedia = array();

        // 1 for click table
        if ($intTable > 0) {

            $strQueryClick = "SELECT 3cn.nid as MediaId, t.field_fmr_type_of_content_value as MediaType
                              FROM " . $this->strClickTableName . " AS 3cn
                              JOIN field_data_field_fmr_type_of_content t ON ( t.entity_id = 3cn.nid AND t.field_fmr_type_of_content_value != 3cn.fmr_type )
                              GROUP BY 3cn.nid
                              ORDER BY 3cn.nid ASC
                              LIMIT $intOffset, $intLimit";

            $objArrMedia = db_query($strQueryClick)->fetchAll();

        } else {

            $strQueryView = "SELECT nid as nid, t.field_fmr_type_of_content_value as MediaType
                          FROM " . $this->strClickTableName . " AS jmv
                          JOIN field_data_field_fmr_type_of_content t ON (t.entity_id = jmv.nid AND  t.field_fmr_type_of_content_value != jm t.field_fmr_type_of_content_value)
                          GROUP BY jmv.nid ASC ORDER BY jmv.nid ASC LIMIT $intOffset, $intLimit";

            $objArrMedia = db_query($strQueryView)->fetchAll();
        }
        return $objArrMedia;
    }


    /**
     * Function for update accessLog table flag to 1
     *
     * @param int $intAid : accesslog record id
     *
     * @return bool
     */
    public function fnUpdateAccessLog($intAid = 0)
    {
        $strUpdateAccessLogQry = "UPDATE accesslog  SET addflag = 1 WHERE aid =" . $intAid;
        db_query($strUpdateAccessLogQry);
        return true;
    }

    /**
     * Function for update 'threebl_tmp_fmr_headline_views' view table flag to 1
     *
     * @param int $intResultId : fetch record from temporary view table
     *
     * @return bool
     */
    public function fnUpdateHeadLineView($intResultId = 0)
    {
        $strUpdateViewUpdateQry = "UPDATE threebl_tmp_fmr_headline_views  SET addflag = 1  WHERE results_id =" . $intResultId;
        db_query($strUpdateViewUpdateQry);
        return true;
    }

    /**
     * Class destructor
     */
    public function __destruct()
    {

    }
}//class
