<?php
    module_load_include('inc', 'refactor_Analytic', 'inc/clsBaseClass');
    //This is class for Analytics functions.

    class clsCompanyData extends clsBaseClass
    {
        public function __construct()
        {
            parent::__construct();
        }
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
            //Include all required files
            module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

            //Object of class
            $objAnalytic = new clsAnalytic();

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
                list($arrFMRInfo, $arrFMRIds) = $objAnalytic->fnGetFMRInfo($objFMRNidInfo, $strMediaType);

                //Implode FMR ids
                $strFMRIds = implode(', ', $arrFMRIds);
            }
            return array($arrFMRInfo, $arrFMRIds, $strFMRIds);
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
        public function fnGetFMRViewClickByCompanyInfo($intCompanyOgId, $strMediaType = '')
        {
            $strSelect = " SELECT N.nid";

            $strFrom = " FROM node AS N
                      JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = N.nid
                      JOIN field_data_field_fmr_type_of_content t ON N.nid = t.entity_id
                      JOIN field_data_group_audience AS ga ON (N.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node') ";

            $strWhere = " WHERE ga.group_audience_gid = $intCompanyOgId and N.status = 1 AND pubdt.field_fmr_date_time_value >= DATE_SUB( '" . $this->strCurrentDate . "' , INTERVAL 240 DAY ) ";

            $strCondition = ($strMediaType != 'all') ? " AND t.field_fmr_type_of_content_value ='" . $strMediaType . "'" : " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') ";
            $strSQL = $strSelect . $strFrom . $strWhere . $strCondition;
            $objFMRNidInfo = db_query($strSQL)->fetchAll();

            return $objFMRNidInfo;
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
        public function fnGetCompanyPublishedFMRsInfo($intCompanyOgId, $strMediaType = "", $intStart = 0, $intLimit = 0)
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

            #Final SQL
            $strQry = $strSelect.$strFrom.$strWhere.$strCondition.$strOrderBy.$strLimitSql;
            $objFMRNidInfo = db_query($strQry)->fetchAll();

            //Get count of all Records
            $intViewClickCntResult = $this->fnGetRecordsCount();
            
            return array($objFMRNidInfo, $intViewClickCntResult);

<<<<<<< HEAD
            //Checking if array is not empty
           /* if (!empty($objFMRNidInfo)) {

                // To get FMR information
                list($arrFMRInfo, $arrFMRIds) = $objAnalytic->fnGetFMRInfo($objFMRNidInfo, $strMediaType);

                //Implode FMR ids
                $strFMRIds = implode(', ', $arrFMRIds);
            }
            return array($arrFMRInfo, $arrFMRIds, $strFMRIds, $intViewClickCntResult);*/
=======
            return array($objFMRNidInfo, $intViewClickCntResult);
>>>>>>> 84e5e163b158b53cca80a4a4c5c446d997091655
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
         * Function return name of requested company id
         *
         * @param int $intCompanyNid : Company Node Id
         *
         * @return string
         */
        function fnGetCompanyName($intCompanyNid = 0)
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
        function fnInsert3blReportDetails($intCompanyId, $strEmailIds, $strReportType, $strReportFileType)
        {
            //--------------------Details of parameters --------------------//
            //  $intCompanyId =   Company id                                //
            //  $strEmailIds = Email  id is available only for email option //
            //  $strReportType =                                            //
            //  1. BADF                                                     //
            //  2. MCSR                                                     //
            //  3. CSMT                                                     //
            //  4. CSC                                                      //
            //  5. FMR                                                      //
            //  $intEmailSendByOption =                                     //
            //   1. pdf                                                     //
            //   2. Excel                                                   //
            //   3. Email                                                   //
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

        function fnGetCompanyOgIds($FMRid) 
        {

            $strMedia = "select ogm.etid, ogm.gid companyOgId
                         from og_membership ogm
                         where ogm.etid = $FMRid";

            $arrMedia = db_query($strMedia)->fetchAssoc();

            return $arrMedia;
        }
    }