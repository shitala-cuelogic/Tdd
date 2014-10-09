<?php
//This is class for Analytics functions.

class clsAnalyticsData
{
    /**
     * Function fetch all campaign related to company
     *
     * @param int   $intCompOgId : Company OG id
     *
     * @return mixed
     */
    public function fnGetAllCampaign($intCompOgId)
    {
        $strSelect = " SELECT SUM(N.nid), CN.title as campaign, COM.field_fmr_campaign_nid AS campaignId
                    FROM node AS N
					JOIN og_membership AS ogm ON (N.nid = ogm.etid AND ogm.entity_type = 'node')
                    JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = N.nid
                    LEFT JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = N.nid
                    LEFT JOIN node AS CN ON CN.nid = COM.field_fmr_campaign_nid
                    Where ogm.gid = $intCompOgId AND N.status = 1 AND COM.field_fmr_campaign_nid > 0
                    AND pubdt.field_fmr_date_time_value >= DATE_SUB( NOW( ) , INTERVAL 180 DAY )
                    GROUP BY campaignId ORDER BY campaign ASC";
        return $objCampaignList = db_query($strSelect)->fetchAll();
    }

    /**
     * Function return all FMRs that get published for excel sheet download from report
     *
     * @param int    $intCompanyOgId : Company OG id
     * @param string $strMediaType   : FMR media type
     *
     * @return array
     */
    public function fnGetCompanyFMRsExcel($intCompanyOgId, $strMediaType = "")
    {
        $strSelect = "SELECT n.nid, t.field_fmr_type_of_content_value AS fmr_type, td.name AS primarycategory, av.field_fmr_average_views_value as benchmark_view, ac.field_fmr_average_clicks_value as benchmark_click, ar.field_dist_archive_value AS is_archive";

        $strFrom = " FROM node AS n
                     JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                     JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                     JOIN field_data_field_fmr_type_of_content AS t ON n.nid = t.entity_id
                     LEFT JOIN field_data_field_fmr_average_views AS av ON n.nid = av.entity_id
                     LEFT JOIN field_data_field_fmr_average_clicks AS ac ON n.nid = ac.entity_id
                     LEFT JOIN field_data_field_fmr_primary_category AS prim ON n.nid = prim.entity_id
                     LEFT JOIN taxonomy_term_data AS td ON td.tid = prim.field_fmr_primary_category_target_id
                     JOIN field_data_field_dist_archive AS ar ON n.nid = ar.entity_id ";

        $strWhere = " WHERE n.status = 1 AND ga.group_audience_gid = $intCompanyOgId";

        //To get Media type condition
        list($strCondition, $strOrderBy, $strMediaWiseSelect, $strCampaignFrom) = $this->fnMediaCondition($strMediaType);

        $arrFMRIds = array();
        $arrFMRInfo = array();

        #Final SQL
        $strQry = $strSelect.$strMediaWiseSelect.$strFrom.$strCampaignFrom.$strWhere.$strCondition.$strOrderBy;
        $objFMRNidInfo = db_query($strQry)->fetchAll();

        if (!empty($objFMRNidInfo)) {
            // To get FMR information
            list($arrFMRInfo, $arrFMRIds) = $this->fnGetFMRInfo($objFMRNidInfo);

            //Implode FMR ids
            $strFMRIds = implode(', ', $arrFMRIds);
        }
        return array($arrFMRInfo, $arrFMRIds, $strFMRIds);
    }

    /**
    * To return FMR information
    * @param object $objFMRNidInfo: Object of FMR result
    */
    public function fnGetFMRInfo($objFMRNidInfo)
    {
        if ($strMediaType == "") {
            foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                $arrFMRInfo[$arrFMRNidInfo->fmr_type][] = $arrFMRNidInfo->nid;
                $arrFMRIds[] = $arrFMRNidInfo->nid;
            }
        } elseif ($strMediaType != "") {

            foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                $arrFMRInfo[$arrFMRNidInfo->nid]['nid'] = $arrFMRNidInfo->nid;
                $arrFMRInfo[$arrFMRNidInfo->nid]['title'] = stripslashes($arrFMRNidInfo->title);
                $arrFMRInfo[$arrFMRNidInfo->nid]['publishdate'] = $arrFMRNidInfo->field_fmr_date_time_value;
                $arrFMRInfo[$arrFMRNidInfo->nid]['campaign'] = $arrFMRNidInfo->campaign;
                $arrFMRInfo[$arrFMRNidInfo->nid]['media'] = $arrFMRNidInfo->fmr_type;
                $arrFMRInfo[$arrFMRNidInfo->nid]['video'] = $arrFMRNidInfo->video;
                $arrFMRInfo[$arrFMRNidInfo->nid]['primary_category'] = $arrFMRNidInfo->primarycategory;
                $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_view'] = $arrFMRNidInfo->benchmark_view;
                $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_click'] = $arrFMRNidInfo->benchmark_click;

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
                $arrFMRIds[] = $arrFMRNidInfo->nid;
             }
        }
        return array($arrFMRInfo, $arrFMRIds);
    }

     /**
    * To return Media type condition
    * @param string $strMediaType:  Media type
    * 
    * * @return string
    */
    public function fnMediaCondition($strMediaType)
    {
        $strCondition = '';
        $strSelect = "";
        $strCampaignFrom = "";
        if ($strMediaType != "") {

            if ($strMediaType != "all") {
                $strCondition .= " AND t.field_fmr_type_of_content_value = '$strMediaType' ";
            } else {
                $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') ";
            }

            $strSelect .= ", n.title as title, pubdt.field_fmr_date_time_value, CN.title as campaign, fmrv.field_video_thumbnail as video ";

            $strCampaignFrom = " LEFT JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = n.nid LEFT JOIN node AS CN ON CN.nid = COM.field_fmr_campaign_nid
                                 LEFT JOIN field_data_field_video AS fmrv ON (fmrv.bundle = 'fmr' AND fmrv.entity_id = n.nid) ";

            $strOrderBy = " ORDER BY pubdt.field_fmr_date_time_value DESC ";
        } else {
            $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') ";
            $strOrderBy = " ORDER BY t.field_fmr_type_of_content_value ";
        }
        return array($strCondition, $strOrderBy, $strSelect, $strCampaignFrom);
   }

    /**
     * Function return all FMRs that get published
     *
     * @param int    $intCompanyOgId  : Company OG id
     * @param string $strMediaType    : FMR media type
     *
     * @return array
     */
    public function fnGetFMRViewClickByCompany($intCompanyOgId, $strMediaType = '')
    {
        $strSelect = " SELECT N.nid";

        $strFrom = " FROM node AS N
                      JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = N.nid
                      JOIN field_data_field_fmr_type_of_content t ON N.nid = t.entity_id
                      JOIN field_data_group_audience AS ga ON (N.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node') ";

        $strWhere = " WHERE ga.group_audience_gid = $intCompanyOgId and N.status = 1 AND pubdt.field_fmr_date_time_value >= DATE_SUB( '" . $this->strCurrentDate . "' , INTERVAL 180 DAY ) ";

        $strCondition = ($strMediaType != 'all') ? " AND t.field_fmr_type_of_content_value ='" . $strMediaType . "'" : " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') ";
        $strSQL = $strSelect . $strFrom . $strWhere . $strCondition;
        $objFMRNidInfo = db_query($strSQL)->fetchAll();

        if (!empty($objFMRNidInfo)) {
            foreach ($objFMRNidInfo as $row) {
                $arrFMRInfo[$row->nid]['nid'] = $row->nid;
                $arrFMRIds[] = $row->nid;
            }
            $strFMRIds = implode(', ', $arrFMRIds);
        }
        return array($arrFMRInfo, $arrFMRIds, $strFMRIds);
    }

    /**
     * Function return all FMRs that get published for analytic second level ajax
     *
     * @param int       $intCompanyOgId  : Company OG id
     * @param string    $strMediaType    : FMR media type
     * @param int       $intStart        : Starting record for pagination
     * @param int       $intLimit        : number of records per page
     *
     * @return array
     */
    public function fnGetCompanyPublishedFMRs($intCompanyOgId, $strMediaType = "", $intStart = 0, $intLimit = 0)
    {
        $strSelect = "SELECT sql_calc_found_rows n.nid, t.field_fmr_type_of_content_value AS fmr_type, av.field_fmr_average_views_value as benchmark_view, ac.field_fmr_average_clicks_value as benchmark_click, td.name AS primarycategory, field_dist_archive_value as is_archive ";

        $strSelect .= ($strMediaType != "") ? ", n.title as title, pubdt.field_fmr_date_time_value " : "";

        $strFrom = " FROM node AS n
                     JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                     JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                     JOIN field_data_field_fmr_type_of_content AS t ON n.nid = t.entity_id
                     JOIN field_data_field_dist_archive AS ar ON n.nid = ar.entity_id
                     LEFT JOIN field_data_field_fmr_average_views AS av ON n.nid = av.entity_id
                     LEFT JOIN field_data_field_fmr_average_clicks AS ac ON n.nid = ac.entity_id
                     LEFT JOIN field_data_field_fmr_primary_category AS prim ON n.nid = prim.entity_id
                     LEFT JOIN taxonomy_term_data AS td ON td.tid = prim.field_fmr_primary_category_target_id ";

        $strWhere = " WHERE n.status = 1 AND ga.group_audience_gid = $intCompanyOgId";

        //To get Media type condition
        list($strCondition, $strOrderBy) = $this->fnMediaCondition($strMediaType);

        $strLimitSql = "";
        if ($intLimit > 0) {
            $strLimitSql .= " LIMIT $intStart, $intLimit ";
        }

        $arrFMRIds = array();
        $arrFMRInfo = array();

        #Final SQL
        $strQry = $strSelect.$strFrom.$strWhere.$strCondition.$strOrderBy.$strLimitSql;
        $objFMRNidInfo = db_query($strQry)->fetchAll();
        
        $strFMRIds = '';

        //Get count of all Records
        $intViewClickCntResult = $this->fnGetRecordsCount();

        //Checking if array is not empty
        if (!empty($objFMRNidInfo)) {

            // To get FMR information
            list($arrFMRInfo, $arrFMRIds) = $this->fnGetFMRInfo($objFMRNidInfo);

            //Implode FMR ids
            $strFMRIds = implode(', ', $arrFMRIds);
        }
        return array($arrFMRInfo, $arrFMRIds, $strFMRIds, $intViewClickCntResult);
    }

    /**
    * Get count of all Records
    *@return int
    */
    public function fnGetRecordsCount()
    {
        list($arrRecordsCount) = db_query('SELECT FOUND_ROWS()')->fetchAll();
        $arrRecordsCount = (array) ($arrRecordsCount);
        $intViewClickCntResult = $arrRecordsCount['FOUND_ROWS()'];

        return $intViewClickCntResult;
    }

    /**
     * To get FMR Ids published in last month
     *
     * @return array
     */
    public function fnGetLastMonthPublishedFMRIds()
    {
        //Get start and End date of Previous month
        $arrStartEndDate = $this->fnGetStartEndDate(2);
        $strGetStartDate = $arrStartEndDate["start_date"];
        $strGetEndDate = $arrStartEndDate["end_date"];

        $strGetBenchmarkYear = date("Y", strtotime($strGetEndDate));
        $strGetBenchmarkMonth = date("m", strtotime($strGetEndDate));

        $strBenchmarkMonthDate = strtotime(date('Y-m-d', mktime(0, 0, 0, $strGetBenchmarkMonth, 1, $strGetBenchmarkYear)));

        // Query to get FMR Information
        $strQry = "SELECT n.nid AS fmrId, prim.field_fmr_primary_category_target_id AS category_id, t.field_fmr_type_of_content_value as fmr_type, PD.field_fmr_date_time_value, ga.group_audience_gid
                   FROM node n
                   JOIN field_data_field_fmr_date_time AS PD ON PD.entity_id = n.nid
                   JOIN field_data_field_fmr_type_of_content AS t ON n.nid = t.entity_id
                   JOIN field_data_field_fmr_primary_category AS prim ON n.nid = prim.entity_id
                   JOIN field_data_field_dist_archive AS ar ON n.nid = ar.entity_id
                   JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                   WHERE n.status = 1
                   AND (DATE_FORMAT(PD.field_fmr_date_time_value,'%Y-%m-%d') >='".$strGetStartDate."'
                   AND DATE_FORMAT(PD.field_fmr_date_time_value,'%Y-%m-%d') <= '".$strGetEndDate. "')
                   AND field_dist_archive_value = 0
                   AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')";
        $objFMRNidInfo = db_query($strQry)->fetchAll();

        // Initialize variables
        $arrFMRIds = array();
        $arrBenchmarkFMRInfo = array();
        $arrBenchMarkByMediaType = array();
        $arrBenchmarkFMRIds = array();
        $strFMRIds = '';

        // Group FMR Ids by Media type and Primary category
        if (!empty($objFMRNidInfo)) {
            
            // To get Benchmark information
            list($arrBenchmarkFMRInfo, $arrFMRIds, $arrBenchMarkByMediaType, $arrBenchmarkFMRIds) = $this->fnGetBenchmarkInfo($objFMRNidInfo);

            //Implode FMR ids
            $strFMRIds = implode(', ', $arrFMRIds);
        }
        return array($arrBenchmarkFMRInfo, $arrFMRIds, $strFMRIds, $arrBenchMarkByMediaType, $arrBenchmarkFMRIds);
    }  

   

    public function fnGetCompanyOgIds($FMRid) {

        $strMedia = "select ogm.etid, ogm.gid companyOgId
                     from og_membership ogm
                     where ogm.etid = $FMRid";

        $arrMedia = db_query($strMedia)->fetchAssoc();

        return $arrMedia;
    }

     /**
     * //Get start and End date of Previous month
     * @param int $strMonthDuration : Month duration for start and end date
     *
     * @return array
     */
    public function fnGetStartEndDate($strMonthDuration = 1)
    {
        $intCurMonth = date("m");

        $intCurYear = date("Y");
        // Get days in previous month
        $intDaysInPrevMonth = date('t', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));

        //Get previous month for start date
        $intStartPrevMonth = date('m', mktime(0, 0, 0, $intCurMonth - $strMonthDuration, 1, $intCurYear));

        //Get previous month for end date
        $intEndPrevMonth = date('m', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));

        //Get previous month year for start date
        $intStartPrevMonthYear = date('Y', mktime(0, 0, 0, $intCurMonth - $strMonthDuration, 1, $intCurYear));

        //Get previous month year for end date
        $intEndPrevMonthYear = date('Y', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));

        //Get previous month date
        $intPrevMonthDate = date('d', mktime(0, 0, 0, $intEndPrevMonth, 0 + $intDaysInPrevMonth, $intCurYear));

        // Start date
        $strGetStartDate = "$intStartPrevMonthYear-$intStartPrevMonth-01";
        // End date
        $strGetEndDate = "$intEndPrevMonthYear-$intEndPrevMonth-$intPrevMonthDate";

        return $arrStartEndDate = array("start_date" => $strGetStartDate, "end_date" => $strGetEndDate);
    } 
}