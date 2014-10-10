<?php
//This is class for Reports functions.
class ClsCommonSQL extends clsBaseClass
{
    /**
     * Function return FMRs those have video attached with it.
     *
     * @param int $intCompanyOgId : Company OG Id
     *
     * @return array
     */
    public function fnGetFMRWithVideo($intCompanyOgId)
    {
        //Collect all FMRs With Videos
        $strFMRWithVideos = "SELECT DISTINCT(ga.entity_id) AS fmrIds
                    FROM field_data_group_audience AS ga
                    JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = ga.entity_id
                    JOIN field_data_field_video AS fmrv ON ( fmrv.bundle = 'fmr' AND fmrv.entity_id = ga.entity_id)
                    WHERE ga.group_audience_gid  =  $intCompanyOgId AND ga.bundle = 'fmr'
                    AND pubdt.field_fmr_date_time_value >= DATE_SUB('" . $this->strCurrentDate . "', INTERVAL 180 DAY )";

        $arrFMRRecords = db_query($strFMRWithVideos)->fetchAll();
        $arrFMRIds = array();

        //Checking blank code
        if (!empty($arrFMRRecords)) {
            foreach ($arrFMRRecords as $arrRow) {
                $arrFMRIds[] = $arrRow->fmrIds;
            }
        }
        return $arrFMRIds;
    }

    /**
     * Function return all FMRs that get published from last 6 months
     *
     * @param int    $intCompanyOgId : Company OG Id
     * @param string $strMediaType   : FMR Media Type
     * @param string $intCampaignId  : Campaign Id
     * @param string $strIsArchiveFlag  : Flag to check condition for archive field
     * @param string $strFileType  : Flag to check file type (excel)
     *
     * @return array
     */
    public function fnGetCompanyFMRs($intCompanyOgId, $strMediaType = "", $intCampaignId = "", $strIsArchiveFlag = "", $strFileType = "")
    {
        $strSelect = "SELECT n.nid, t.field_fmr_type_of_content_value AS fmr_type ";

        $strCondition = '';
        $strCampaignFrom = ''; //change
        if ((int)$intCampaignId > 0) {
            $strSelect .= ", n.title as title, pubdt.field_fmr_date_time_value ";
            $strCampaignFrom = "  JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = n.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        } else {
            if ($strMediaType != '') {
                if ($strMediaType != "all") {
                    $strCondition .= " AND t.field_fmr_type_of_content_value = '$strMediaType' ";
                } else {
                    $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') ";
                }
            } else {
                $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')  ";
            }
        }
        $strFrom = " FROM node AS n
                    JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                    JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                    JOIN field_data_field_fmr_type_of_content AS t ON n.nid = t.entity_id ";

        $strWhere = " WHERE n.status = 1 AND ga.group_audience_gid = $intCompanyOgId
                    AND pubdt.field_fmr_date_time_value >= DATE_SUB('" . $this->strCurrentDate . "', INTERVAL 180 DAY ) ";

        $strOrderBy = ($strMediaType == "") ? " ORDER BY t.field_fmr_type_of_content_value " : " ORDER BY pubdt.field_fmr_date_time_value DESC ";

        if ($strMediaType != "" && $intCampaignId == 0) {
            $strSelect .= ", n.title as title, pubdt.field_fmr_date_time_value, CN.title as campaign ";
            $strCampaignFrom .= " LEFT JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = n.nid
                                  LEFT JOIN node AS CN ON CN.nid = COM.field_fmr_campaign_nid ";
        }

        //Get Primary Categories Assigned to FMRs
        if ($strMediaType != "" || (int)$intCampaignId >0) {
            $strSelect .= ", td.name AS primarycayegory, av.field_fmr_average_views_value as benchmark_view, ac.field_fmr_average_clicks_value as benchmark_click";
            $strCampaignFrom .= " LEFT JOIN field_data_field_fmr_primary_category AS prim ON n.nid = prim.entity_id
                                  LEFT JOIN taxonomy_term_data AS td ON td.tid = prim.field_fmr_primary_category_target_id
                                  LEFT JOIN field_data_field_fmr_average_views AS av ON n.nid = av.entity_id
                                  LEFT JOIN field_data_field_fmr_average_clicks AS ac ON n.nid = ac.entity_id ";
        }

        // select archive field
        $strArchiveFlag = "JOIN field_data_field_dist_archive AS ar ON n.nid = ar.entity_id";
        if ($strIsArchiveFlag == "non_archive") {
            // Get Non archive FMRs
            $strWhere .= " AND ar.field_dist_archive_value = 0";
        } elseif ($strIsArchiveFlag == "archive") {
            $strSelect .= ", ar.field_dist_archive_value AS is_archive";
        }

        #Final SQL
        $strQry = $strSelect.$strFrom.$strCampaignFrom.$strArchiveFlag.$strWhere.$strCondition.$strOrderBy;
        $objFMRNidInfo = db_query($strQry)->fetchAll();

        $arrFMRIds = array();
        $arrFMRInfo = array();
        $strFMRIds = '';

        if (!empty($objFMRNidInfo)) {

            if ((int)$intCampaignId == 0 && $strMediaType == "") {
                foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                    $arrFMRInfo[$arrFMRNidInfo->fmr_type][] = $arrFMRNidInfo->nid;
                    $arrFMRIds[] = $arrFMRNidInfo->nid;
                }
            } elseif ((int)$intCampaignId > 0 || $strMediaType != "all") {
                foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                    $arrFMRInfo[$arrFMRNidInfo->nid]['nid'] = $arrFMRNidInfo->nid;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['publishdate'] = $arrFMRNidInfo->field_fmr_date_time_value;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['campaign'] = $arrFMRNidInfo->campaign;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['media'] = $arrFMRNidInfo->fmr_type;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['primary_category'] = $arrFMRNidInfo->primarycayegory;

                    $arrFMRInfo[$arrFMRNidInfo->nid]['title'] = $arrFMRNidInfo->title;
                    if ($strFileType == "excel") {
                        $arrFMRInfo[$arrFMRNidInfo->nid]['title'] = $this->fnRemoveWordFormatting($arrFMRNidInfo->title);
                    }

                    //getting Benchmark views and clicks
                    $strFMRPublishedDate = date("Y-m-d", strtotime($arrFMRNidInfo->field_fmr_date_time_value));
                    if (strtotime($strFMRPublishedDate) <= strtotime($this->strBenchmarkNAMonth)) {
                        // Set N/A value for Benchmark views and clicks, if FMR published before Apr 2014
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_view'] = "N/A";
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_click'] = "N/A";
                    } else {
                        // Set value for Benchmark views and clicks
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_view'] = ((int) $arrFMRNidInfo->benchmark_view > 0) ? number_format((int) $arrFMRNidInfo->benchmark_view) : "-";
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_click'] = ((int) $arrFMRNidInfo->benchmark_click > 0) ? number_format((int) $arrFMRNidInfo->benchmark_click) : "-";
                    }

                    $arrFMRInfo[$arrFMRNidInfo->nid]['is_archive'] = $arrFMRNidInfo->is_archive;
                }
                $arrFMRIds = array_keys($arrFMRInfo);
            }

            //Implode FMR ids
            $strFMRIds = implode(', ', $arrFMRIds);
        }
        return array($arrFMRInfo, $arrFMRIds, $strFMRIds);
    }

    /**
     * Function fetch FMR views information
     *
     * @param string $strFMRIds : Comma separated FMR Ids
     *
     * @return array
     */
    public function fnGetViews($strFMRIds)
    {
        $strSQL = " SELECT SUM(v.totalcount) AS viewcount, v.nid
                    FROM " . $this->strViewTableName . " AS v WHERE v.nid IN ($strFMRIds) GROUP BY v.nid";
        $objFMRViewInfo = db_query($strSQL)->fetchAll();

        $arrFMRInfo = array();

        //Checking is empty
        if (!empty($objFMRViewInfo)) {
            # Get FMR view count
            foreach ($objFMRViewInfo as $objArrRow) {
                $arrFMRInfo[$objArrRow->nid] = $objArrRow->viewcount;
            }
        }
        return $arrFMRInfo;
    }

    /**
     * Function fetch FMR Clicks information
     *
     * @param string $strFMRIds : Comma separated FMR Ids
     *
     * @return array
     */
    public function fnGetClicks($strFMRIds)
    {
        $strSQL = " SELECT COUNT(C.nid) AS clickcount, C.nid
                    FROM " . $this->strClickTableName . " AS C WHERE C.nid IN ($strFMRIds) GROUP BY C.nid";
        $objFMRViewInfo = db_query($strSQL)->fetchAll();

        $arrFMRInfo = array();

        //Checking is empty
        if (!empty($objFMRViewInfo)) {
            # Get FMR Click count
            foreach ($objFMRViewInfo as $objArrRow) {
                $arrFMRInfo[$objArrRow->nid] = $objArrRow->clickcount;
            }
        }
        return $arrFMRInfo;
    }

    /**
     * Query for getting View Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId          : Company OG Id
     * @param string $strFMRIds               : Comma separated FMR Ids
     * @param int    $intCampaignId           : Campaign Id
     *
     * @return array
     */
    public function fnGetViewChartInfo($intCompanyOgId, $strFMRIds = "", $intCampaignId = 0)
    {
        $strCampaignFrom = ''; //change
        if ((int)$intCampaignId != 0) {
            $strCampaignFrom = "  JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = v.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        }

        $strSQL = "SELECT SUM(v.totalcount) as totalcount, v.datevisited AS datevisit
                  FROM " . $this->strViewTableName . " AS v
                  $strCampaignFrom
                  WHERE v.company_ogid = $intCompanyOgId
                  AND (v.datevisited BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "')
                  AND v.nid IN ($strFMRIds)
                  GROUP BY datevisited
                  ORDER by datevisited ASC";

        $arrViewChartData = db_query($strSQL)->fetchAll();
        $arrViewChart = array();

        //Checking empty condition
        if (!empty($arrViewChartData)) {

            foreach ($arrViewChartData as $arrVal) {
                $arrViewChart[$arrVal->datevisit] = $arrVal->totalcount;
            }
        }
        return $arrViewChart;
    }

    /**
     * Query for getting Click Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId               : Company OG Id
     * @param string $strFMRIds                    : Comma separated FMR Ids
     * @param int    $intCampaignId                : Campaign Id
     *
     * @return array
     */
    public function fnGetClickChartInfo($intCompanyOgId, $strFMRIds = "", $intCampaignId = 0)
    {
        $strCampaignFrom = '';
        if ((int)$intCampaignId > 0) {
            $strCampaignFrom = " JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = C.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        }

        $strSQL = "SELECT DATE_FORMAT(C.click_date, '%Y-%m-%d') as clickdate, count(C.nid) as totalcount
                  FROM " . $this->strClickTableName . " as C
                  $strCampaignFrom
                  WHERE C.company_ogid = $intCompanyOgId
                  AND (DATE_FORMAT(C.click_date, '%Y-%m-%d') BETWEEN '".$this->strChartPrevDate."' AND '".$this->strCurrentDate."')
                  AND C.nid IN ($strFMRIds)
                  GROUP BY  DATE_FORMAT(C.click_date, '%Y/%m/%d')
                  ORDER by C.click_date asc";

        $arrayClickChartData = db_query($strSQL)->fetchAll();
        $arrClickChart = array();

        //Checking empty condition
        if (!empty($arrayClickChartData)) {

            foreach ($arrayClickChartData as $arrVal) {
                $arrClickChart[$arrVal->clickdate] = $arrVal->totalcount;
            }
        }
        return $arrClickChart;
    }

    /**
     * Query for Google Map for Click Count for Second Level and Third Level
     *
     * @param int   $intCompanyOgId   :  Company OG id
     * @param mixed $strMediaTypeORId : value will be either media type or id
     *
     * @return mixed
     */
    public function fnGetFMRClicksByCountryGoogleMap($intCompanyOgId, $strMediaTypeORId)
    {
        // To get Media type condition
        $strMediaCondition = $this->fnMediaTypeCondition($strMediaTypeORId);

        $strCondition = " $strMediaCondition  GROUP BY UPPER(country) order by ipcount desc ";

        $strFinalSql = "SELECT nid,click_ip,count(nid) as ipcount,country
                                      FROM " . $this->strClickTableName . " as C
                                      JOIN field_data_field_fmr_type_of_content t ON  C.nid = t.entity_id
                                      WHERE C.company_ogid = $intCompanyOgId
                                      $strCondition";

        return $arrayClickMapData = db_query($strFinalSql)->fetchAll();
    }

    /**
    * To return Media type condition
    * @param mixed $strMediaTypeORId:  value will be either media type or id
    * 
    * * @return string
    */
    public function fnMediaTypeCondition($strMediaTypeORId) 
    {
        $strMediaCondition = '';
        if ((int)$strMediaTypeORId == 0 && $strMediaTypeORId != '') {
            //getting particular media-type condition for clicks.
            if ($strMediaTypeORId == "all") {
                $strMediaCondition .= " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter') ";
            } else {
                $strMediaCondition .= " AND C.fmr_type ='" . $strMediaTypeORId . "'";
            }
        } elseif ((int)$strMediaTypeORId > 0) {
            $strMediaCondition .= " AND C.nid = $strMediaTypeORId ";
        }
        return $strMediaCondition;
    }

    /**
    * Query for Referral website for Second Level and Third Level
    *
    * @param int   $intCompanyOgId    :  Company OG id
    * @param mixed $strMediaTypeORId  : value will be either media type or id
    *
    * @return mixed
    */
    public function fnGetFMRReferrerClicksByMediaType($intCompanyOgId, $strMediaTypeORId)
    {
        // To get Media type condition
        $strMediaCondition = $this->fnMediaTypeCondition($strMediaTypeORId);
        
        if ((int)$strMediaTypeORId > 0) {
            $arrExtraLink = array('bestgenericviagra.biz', 'blog.pnarp.com', 'diffbot.com', 'timberland.justmeans.com', 'www.mysmutsearch.com', 'lala123.com', '_bit=528e713d-', '_bit=52af68a5-00111-07550', 'piwik.3blmedia.com', 'stats.3blmedia.com');
            $strExtraLink = implode("|", $arrExtraLink);
            $strMediaCondition = " $strMediaCondition AND click_referrer NOT RLIKE '$strExtraLink' ";
        }

        $strCondition = " $strMediaCondition  GROUP BY click_referrer Order by click_date desc ";

        $strFinalQuery = "SELECT distinct click_referrer as rlink
                                   FROM " . $this->strClickTableName . " as C
                                   JOIN field_data_field_fmr_type_of_content t ON  C.nid = t.entity_id
                                   WHERE C.company_ogid = $intCompanyOgId
                                   AND C.click_referrer <> '' $strCondition";
        return $arrayClickReferData = db_query($strFinalQuery)->fetchAll();
    }

    /**
     * Getting View-Count By Media-Id
     *
     * @param int  $intCompanyOgId : Company OG Id
     * @param int  $intMediaId     : Media Id
     * @param bool $boolReport     : Boolean Flag
     *
     * @return mixed
     */
    public function fnGetFMRViewCountByMediaId($intCompanyOgId, $intMediaId, $boolReport = 0)
    {
        $strSql = "SELECT  SUM(v.totalcount) as totalcount
              FROM " . $this->strViewTableName . " as v
              Where  v.company_ogid = $intCompanyOgId AND v.nid  IN ($intMediaId)
              GROUP BY v.nid";
        return $arrViewCount = ($boolReport == 0) ? db_query($strSql)->fetchAssoc() : db_query($strSql)->fetchAll();
    }

    /**
     * Getting Click-Count By Media-Id
     *
     * @param int  $intCompanyOgId : Company OG Id
     * @param int  $intMediaId     : Media Id
     * @param bool $boolReport     : Boolean Flag
     *
     * @return mixed
     */
    public function fnGetFMRClickCountByMediaId($intCompanyOgId, $intMediaId, $boolReport = 0)
    {
        $strSql = "SELECT C.nid, C.click_date, count(C.nid) as totalcount, C.fmr_type AS type
                   FROM " . $this->strClickTableName . " as C
                   Where C.company_ogid = $intCompanyOgId
                   AND C.nid IN ($intMediaId)
                   GROUP BY nid";
        return $arrClickCount = ($boolReport == 0) ? db_query($strSql)->fetchAssoc() : db_query($strSql)->fetchAll();
    }

    **
     * Function gather data of 7 days and create array for the Graph.
     *
     * @param array $arrViewChart  : FMR Views Info
     * @param array $arrClickChart : FMR Clicks Info
     *
     * @return array
     */
    public function fnSumWeekViewsClicks($arrViewChart, $arrClickChart = array())
    {
        $intMaxChart = max($arrViewChart);
        $intMaxChart1 = (count($arrClickChart) > 0) ? max($arrViewChart) : 0;

        if ($intMaxChart <= 0 && $intMaxChart1 <= 0) {
            return (count($arrClickChart) > 0) ? array("", "", "") : array("", "");
        }

        # getting previous date from today's date means we need the dates array having 7 days difference.(ex 1-11-2012, 7-11-2012)
        $strTodayDate = $this->strCurrentDate;
        $arrDts = $this->fnGetPreviousDates($strTodayDate, 59, 1);
        $arrDts = array_reverse($arrDts);

        //Process views and clicks and prepare 7 days interval data for graph
        $arrDates = array();
        $arrChartSum = $arrChartSum1 = array();
        $strDate = "";
        $intCount = $intClickSum = $intViewSum = 0;
        foreach ($arrDts as $key => $val) {

            $intClickSum += (int)$arrViewChart[$val];
            $intViewSum += (int)$arrClickChart[$val];
            $intCount++;
            if ($intCount == 7) {
                $strDate = date("M d", strtotime($val));
                $arrDates[] = $strDate;
                $arrChartSum[$strDate] = (int)$intClickSum;
                $arrChartSum1[$strDate] = (int)$intViewSum;

                $strDate = "";
                $intCount = 0;
                $intClickSum = 0;
                $intViewSum = 0;
            }
        }
        //Update Last date range data.
        if ($intCount > 0) {
            $strDate = date("M d", strtotime($val));
            $arrDates[] = $strDate;
            $arrViewChartSum[$strDate] = (int)$intClickSum;
            $arrClickChartSum[$strDate] = (int)$intViewSum;
        }
        if (count($arrClickChart) > 0) {
            return array($arrViewChartSum, $arrClickChartSum, $arrDates);
        } else {
            return array($arrViewChartSum, $arrDates);
        }
    }

    /**
     * Function return name of requested company id
     *
     * @param int $intCompanyNid : Company Node Id
     *
     * @return string
     */
    public function fnGetCompanyName($intCompanyNid = 0)
    {
        $strCompanyName = "";
        if ((int)$intCompanyNid > 0) {
            $strCompanyName = db_query("SELECT title FROM node WHERE nid = '" . $intCompanyNid . "'")->fetchField();
        }
        return $strCompanyName;
    }

    /**
     * Function for Getting Campaign Name
     *
     * @param int $intCampaignId : Campaign Id
     *
     * @return string
     */
    function fnGetCampaignName($intCampaignId = 0)
    {
        //Get Campaign Name from campaign id
        $strCampaignName = '';
        if ((int)$intCampaignId > 0) {
            $strCampaignName = db_query("SELECT title FROM node WHERE nid = '" . $intCampaignId . "'")->fetchField();
        }
        return $strCampaignName;
    }

    /**
     * Function to insert 3BL report details
     *
     * @param int    $intCompanyId      : Company Id
     * @param string $strEmailIds       : Email Ids
     * @param string $strReportType     : Report type
     * @param string $strReportFileType : Report File type
     *
     * @return mixed
     */
    public function fnInsert3blReportDetails($intCompanyId, $strEmailIds, $strReportType, $strReportFileType)
    {
        //--------------------Details of parameters --------------------//
        //	$intCompanyId =   Company id                                //
        //	$strEmailIds = Email  id is available only for email option //
        //	$strReportType =                                            //
        //  1. BADF														//
        //  2. MCSR 													//
        //  3. CSMT  													//
        //  4. CSC														//
        //  5. FMR                                 						//
        //	$intEmailSendByOption = 									//
        //   1. pdf														//
        //   2. Excel													//
        //   3. Email 		                             				//
        //--------------------------- End ------------------------------//

        //set the current date
        $dateCurrentDate = date("Y-m-d");

        #Set the report Source to 3bl
        $strChannel = "3bl";

        //Insert all details in the table.
        $intInsertId = db_insert('threebl_company_auto_report_tracking')
            ->fields(array(
                'companyid' => $intCompanyId,
                'email' => $strEmailIds,
                'channel' => $strChannel,
                'report_type' => $strReportType,
                'download_file_type' => $strReportFileType,
                'send_on' => $dateCurrentDate
            ))->execute();
        return $intInsertId;
    }

    /**
     * This function Removes special character's added through Msword.
     *
     * @param sting $strMsWordText : MS word text
     * @param string $strSection
     * @return string
     */
    function fnRemoveWordFormatting($strMsWordText, $strSection = "")
    {
        $search = array(
            '/(%E2%80%93|%u2013|%u2026|%u2014|%96|%E2%80%94)/i',
            '/(%E2%80%9C|%E2%80%9D|%u201C|%u201D|%93|%94)/i',
            '/(%E2%80%98|%E2%80%99|%u2018|%u2019|%91|%92|%E2%80%9B)/i',
            '/(%95%09)/',
            '/(%E2%80%A6)/',
            '/(%E2%80%9A)/',
            '/(%E2%80%8B)/',
            '/(%E2%80%A8)/'
        );

        $replace = array(
            '-',
            '"',
            "'",
            "- ",
            "...",
            ",",
            " ",
            " "
        );

        $strMsWordText = preg_replace($search, $replace, $strMsWordText);
        $strMsWordUrlDecodeText = urldecode(preg_replace($search, $replace, urlencode($strMsWordText)));

        if ($strSection == "widget") {
            $strMsWordFinalText = html_entity_decode(htmlentities($strMsWordUrlDecodeText));
        } else {
            $strMsWordFinalText = utf8_decode(html_entity_decode(htmlentities($strMsWordUrlDecodeText)));
        }


        return $strMsWordFinalText;
    }

}