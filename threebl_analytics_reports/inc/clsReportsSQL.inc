<?php
//This is class for Reports functions.
class ClsReportsSQL extends clsBaseClass
{
    
    /**
     * Find emails for monthly report functionality
     *
     * @param $strCompanyNid : List of company ids
     *
     * @return mixed
     */
    function getMonthlyAutoReportEmail($strCompanyNid) {

        $strSql = "SELECT field_email_for_automated_report_email AS email, entity_id AS nid, delta
                   FROM field_data_field_email_for_automated_report
                   WHERE entity_id IN (" . $strCompanyNid . ") ORDER BY nid";

        return db_query($strSql)->fetchAll();
    }

    /**
     * Find emails for weekly report functionality
     *
     * @param $strCompanyNid : List of company ids
     *
     * @return mixed
     */
    function getWeeklyAutoReportEmail($strCompanyNid) {

        $strSql = "SELECT field_email_for_automated_month_email AS email, entity_id AS nid, delta
                   FROM field_data_field_email_for_automated_month
                   WHERE entity_id IN (".$strCompanyNid.") ORDER BY nid";

        return db_query($strSql)->fetchAll();
    }

    /**
     * Get Latest FMR
     *
     * @param int $intCompanyId : Company Id
     *
     * @return array
     */
    public function fnGetCronLatestFMR($intCompanyId = 0)
    {
        //Include all required files
        module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

        //Object of class
        $objAnalytics = new clsAnalytic();
        $arrLatestFMR = array();
        if ((int)$intCompanyId > 0) {
            $strSql = "SELECT DISTINCT n.nid as fmrIds
             FROM  og
             JOIN og_membership ogm ON (og.gid = ogm.gid AND ogm.entity_type = 'node')
             JOIN node AS n ON n.nid = ogm.etid
             JOIN field_data_field_fmr_date_time AS d ON d.entity_id = n.nid
             WHERE n.status = 1 AND n.type ='fmr' AND og.etid = $intCompanyId
             AND (d.field_fmr_date_time_value < DATE_SUB( '" . $this->strCurrentDate . "', INTERVAL 8 DAY) AND d.field_fmr_date_time_value >= DATE_SUB( '" . $this->strCurrentDate . "', INTERVAL 15 DAY))
             ORDER BY d.field_fmr_date_time_value DESC";

            $objLatestFMR = db_query($strSql)->fetchAll();
            
            //To get FMR Ids
            $arrLatestFMR = $objAnalytics->fnGetFMRIds($objLatestFMR);
        }
        return $arrLatestFMR;
    }

    /**
     * Getting active company records.
     *
     * @return array
     */
    public function fnGetActiveCompanies()
    {
        $strSql = " SELECT N.nid, og.gid
					FROM node AS N
					JOIN og ON og.etid= N.nid
					JOIN field_data_field_active_3bl_client_account AS ac ON N.nid = ac.entity_id
					JOIN field_data_field_email_for_automated_report AS ae ON ae.entity_id = N.nid
					WHERE N.status = 1 AND ac.field_active_3bl_client_account_value = 1 AND N.type = 'client'
					GROUP BY og.gid
					ORDER BY og.gid";
        $objCompany = db_query($strSql)->fetchAll();

        if (!empty($objCompany)) {
            foreach ($objCompany as $arrRow) {
                $arrCompany[$arrRow->gid] = $arrRow->nid;
            }
        }
        return $arrCompany;
    }

    /**
     * Function fnGetFMRAnalyticView to get analytic views
     *
     * @param int    $intFMRIds    : FMR id
     * @param string $strReportType: Report type (Weekly or Individual).
     *
     * @return array
     */
    function fnGetFMRAnalyticView($intFMRIds, $strReportType = "")
    {
        //Include all required files
        module_load_include('inc', 'refactor_reports', 'inc/clsReports');

        //Object of class
        $objReports = new clsReports();
        
        //Group By condition
        $strGroupBy = "GROUP BY v.channel";
        if ($strReportType == "weekly") {
            $strGroupBy .= ", v.nid";
        }
        $strSQL = " SELECT SUM(v.totalcount) AS viewcount, v.nid, v.channel
                    FROM " . $this->strViewTableName . " AS v WHERE v.nid IN(" . $intFMRIds . ") $strGroupBy";
        $objFMRViewInfo = db_query($strSQL)->fetchAll();
        
        $arrAnalyticData = $objReports->fnGetAnalyticViewData($objFMRViewInfo);
        
        return $arrAnalyticData;
    }

    /**
     * Function fnGetFMRAnalyticClicks to get the Analytic clicks.
     *
     * @param int    $intFMRIds    : FMR id.
     * @param string $strReportType: Report type (Weekly or Individual).
     *
     * @return array
     */
    function fnGetFMRAnalyticClicks($intFMRIds, $strReportType = "")
    {
        //Group By condition
        $strGroupBy = "GROUP BY C.channel";
        if ($strReportType == "weekly") {
            $strGroupBy .= ", C.nid";
        }
        $strSQL = " SELECT COUNT(C.nid) AS clickcount, C.nid, C.channel
                    FROM " . $this->strClickTableName . " AS C WHERE C.nid IN(" . $intFMRIds . ") $strGroupBy";
        $objFMRClickInfo = db_query($strSQL)->fetchAll();

        //Include all required files
        module_load_include('inc', 'refactor_reports', 'inc/clsReports');

        //Object of class
        $objReports = new clsReports();

        $arrAnalyticData = $objReports->fnGetAnalyticClickData($objFMRClickInfo);

        return $arrAnalyticData;
    }

    /**
     * Function for Getting click count For 3bl NewsLetter
     *
     * @param int    $intFMRId     : Fmr Id
     * @param string $strReportType: Report type (Weekly or Individual).
     *
     * @return int
     */
    public function fnGetFMRNewsletterClick($intFMRId, $strReportType = "")
    {
        //Group By condition
        $strGroupBy = "";
        if ($strReportType == "weekly") {
            $strGroupBy = "GROUP BY C.contentID";
        }

        $intClickCount = 0;
        $strSQL = " SELECT COUNT(C.contentID) AS clickcount, C.contentID
                    FROM " . $this->strNewsletterClick . " AS C WHERE C.contentID IN(" . $intFMRId . ") AND contentType = 'fmr' $strGroupBy";

        $arrClickCount = array();
        //Weekly FMR 3bl newsletter clicks
        if ($strReportType == "weekly") {
            $objFMRClickInfo = db_query($strSQL)->fetchAll();
            if (!empty($objFMRClickInfo)) {
                foreach ($objFMRClickInfo as $arrFMRClickInfo)
                    $arrClickCount[$arrFMRClickInfo->contentID]["3bl_newsletter_click"] = $arrFMRClickInfo->clickcount;
            }
            return $arrClickCount;

        } else {
            //Individual FMR 3bl newsletter clicks
            $objFMRClickInfo = db_query($strSQL)->fetchAssoc();
            if (!empty($objFMRClickInfo)) {
                $intClickCount = $objFMRClickInfo['clickcount'];
            }
            return $intClickCount;
        }
    }

    /**
     * Function fnGetMicroListClickCount
     *
     * @param int $intFMRId : FMR id
     *
     * @return array
     */
    function fnGetMicroListClickCount($intFMRId)
    {
        $strSql = " (
                    SELECT count( tr.fmr_nid ) AS totalview, eck.type AS typ, eck.title, eck.id, tr.tracking_flag
                    FROM threebl_microlist_tracking AS tr
                    JOIN eck_micro_list AS eck ON eck.id = tr.list_id
                    WHERE tr.fmr_nid =" . $intFMRId . "
                    AND tr.tracking_flag = 'open'
                    GROUP BY tr.list_id
                    )
                    UNION
                    (
                    SELECT count( tr.fmr_nid ) AS totalClick, eck.type AS typ, eck.title, eck.id, tr.tracking_flag
                    FROM threebl_microlist_tracking AS tr
                    JOIN eck_micro_list AS eck ON eck.id = tr.list_id
                    WHERE tr.fmr_nid =" . $intFMRId . "
                    AND tr.tracking_flag = 'click'
                    GROUP BY tr.list_id
                    ) ORDER BY typ";
        $arrViewCount = db_query($strSql)->fetchAll();
        //Include all required files
        module_load_include('inc', 'refactor_reports', 'inc/clsReports');

        //Object of class
        $objReports = new clsReports();

        $arrMicroListDetails  = $objReports->fnGetMicrolistDetails($arrViewCount);
        return $arrMicroListDetails;
    }

    /**
     * * Function for Secondary Category of FMRs
     *
     * @param $strPrimaryCategory
     * @param $strSecondCategory
     *
     * @return mixed
     */
    public function fnGetSecondaryCategory($strPrimaryCategory, $strSecondCategory)
    {
        $objFMRPrimaryCategory = '';
        $objFMRSecondaryCategory = '';

        if ($strPrimaryCategory != '') {
            $strPrimarySQL = "SELECT distinct name
                                   From taxonomy_term_data WHERE tid
                                   IN ($strPrimaryCategory)";
            $objFMRPrimaryCategory = db_query($strPrimarySQL)->fetchAll();
        }


        if ($strSecondCategory != '') {
            $strSecondarySQL = "SELECT distinct name
                                     From taxonomy_term_data
                                     WHERE tid IN ($strSecondCategory)";
            $objFMRSecondaryCategory = db_query($strSecondarySQL)->fetchAll();
        }
        return array('arrPrimaryCat' => $objFMRPrimaryCategory, 'arrSecondCat' => $objFMRSecondaryCategory);
    }




    /**
     * Function to get FMRs information distributed in the previous 30 or 7 days
     *
     * @param int    $intCalDays   : Number of days
     * @param string $strReportType: Report type (Weekly or Individual).
     *
     * @return array
     */
    function fnGetLastPublishedFMRInfo($intCalDays = 30, $strReportType = "")
    {
        // Current Date
        if ($strReportType == "weekly") {
            $strCurrentDate = date("Y-m-d", strtotime($this->strCurrentDate . "- 1 days"));
        } else {
            $strCurrentDate = $this->strCurrentDate;
        }

        //Get date of Previous month
        $intCurrentYear = date("Y", strtotime($strCurrentDate));
        $intCurrentMonth = date("m", strtotime($strCurrentDate));
        $intCurrentDate = date("d", strtotime($strCurrentDate));
        $intPrevMonthDate = date('Y-m-d', mktime(0, 0, 0, $intCurrentMonth, $intCurrentDate - $intCalDays, $intCurrentYear));

        $strFMRDetails = "SELECT n.nid as fmrid, n.title, pubdt.field_fmr_date_time_value as publishdate, og.label as companyname, og.etid
                          FROM node n
                          JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                          JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                          JOIN og as og on og.gid = ga.group_audience_gid
                          WHERE n.status = 1
                          AND pubdt.field_fmr_date_time_value >=  '$intPrevMonthDate' AND pubdt.field_fmr_date_time_value <= '$strCurrentDate'
                          ORDER BY pubdt.field_fmr_date_time_value DESC";

        $arrFMRDetails = db_query($strFMRDetails)->fetchAll();

        $arrFMRInfo = array();
        if (is_array($arrFMRDetails) && count($arrFMRDetails) > 0) {

            //Include all required files
            module_load_include('inc', 'refactor_reports', 'inc/clsReports');

            //Object of class
            $objReports = new clsReports();

            list($arrFMRInfo, $strFMRIds) = $objReports->fnGetLatestFMRInfo($arrFMRDetails);

        }

        return array($arrFMRInfo, $strFMRIds, $strCurrentDate, $intPrevMonthDate);
    }

    /**
     * Get all Vertical List
     *
     * @param string $strNid : Node Id
     *
     * @return array
     */
    public function fnFMRVerticals($strNid)
    {
        $arrVerticals = array();
        if ($strNid != "") {
            $strSQL = "SELECT vert.entity_id AS fmrId, ver.name AS Verticals
             FROM field_data_field_dist_vertical AS vert
             LEFT JOIN taxonomy_term_data AS ver ON ver.tid = vert.field_dist_vertical_target_id
             WHERE vert.entity_id IN (" . $strNid . ")";
            $objVerticals = db_query($strSQL)->fetchAll();
            if (count($objVerticals) > 0) {
                foreach ($objVerticals as $arrRow) {
                    $arrVerticals[$arrRow->fmrId][] = $arrRow->Verticals;
                }
            }
        }
        return $arrVerticals;
    }

    /**
     * Function insertCompanyDetails
     *
     * @param int    $intMonth                  : Flag to check report type
     * @param int    $intCompanyNid             : Company Node Id
     * @param string $strRecipient              : Recipient email
     * @param string $strDownloadReportFileType : Download Report File Type
     *
     * @return mixed
     */
    public function fnInsertCompanyDetails($intMonth, $intCompanyNid, $strRecipient, $strDownloadReportFileType = 'email')
    {
        //set the current date
        $dateCurrentDate = date("Y-m-d");
        $arrUpdate = array(
            'companyid' => $intCompanyNid,
            'email' => $strRecipient,
            'channel' => 'cron',
            'report_type' => ($intMonth ? 7 : 6),
            'download_file_type' => $strDownloadReportFileType,
            'send_on' => $dateCurrentDate
        );

        //Insert all details in the table.
        $intInsertId = db_insert('threebl_company_auto_report_tracking')
            ->fields($arrUpdate)
            ->execute();
        return $intInsertId;
    }
}