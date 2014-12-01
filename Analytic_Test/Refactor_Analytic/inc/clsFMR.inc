<?php
    /**
     * Class containing Sql operations
     * Date: 29/10/14
     */

    module_load_include('inc', 'refactor_Analytic', 'inc/clsBaseClass');

    class clsFMR extends clsBaseClass
    {
        public function __construct()
        {
            parent::__construct();
        }
        /**
         * Function return FMRs those have video attached with it.
         *
         * @param int $intCompanyOgId : Company OG Id
         *
         * @return array
         */
        public function fnGetFMRWithVideoInfo($intCompanyOgId)
        {
            //Collect all FMRs With Videos
            $strFMRWithVideos = "SELECT DISTINCT(ga.entity_id) AS fmrIds
                    FROM field_data_group_audience AS ga
                    JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = ga.entity_id
                    JOIN field_data_field_video AS fmrv ON ( fmrv.bundle = 'fmr' AND fmrv.entity_id = ga.entity_id)
                    WHERE ga.group_audience_gid  =  $intCompanyOgId AND ga.bundle = 'fmr'
                    AND pubdt.field_fmr_date_time_value >= DATE_SUB('" . $this->strCurrentDate . "', INTERVAL 180 DAY )";

            $ObjFMRRecords = db_query($strFMRWithVideos)->fetchAll();

            return $ObjFMRRecords;
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
        public function fnGetCompanyFMRsInfo($intCompanyOgId, $strMediaType = "", $intCampaignId = "", $strIsArchiveFlag = "", $strFileType = "")
        {
            //Include all required files
            module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

            //Object of class
            $objAnalytics = new clsAnalytic();

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
            $objCompanyFMRInformation = db_query($strQry)->fetchAll();

            return $objCompanyFMRInformation;
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
                                      FROM threebl_analytic_clicks as C
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
                                   FROM threebl_analytic_clicks as C
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
              FROM threebl_analytic_views as v
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
                   FROM threebl_analytic_clicks as C
                   Where C.company_ogid = $intCompanyOgId
                   AND C.nid IN ($intMediaId)
                   GROUP BY nid";
            return $arrClickCount = ($boolReport == 0) ? db_query($strSql)->fetchAssoc() : db_query($strSql)->fetchAll();
        }

        /**
         * Function collect all verticals category attached with requested FMRs
         *
         * @param string $strNid : FMRs node ids with comma separated
         *
         * @return array
         */
        public function fnFMRVerticals($strNid)
        {
            $arrVerticals = array();
            if ($strNid != "") {
                $strSQL = " SELECT vert.entity_id AS fmrId, ver.name AS Verticals
                 FROM field_data_field_dist_vertical AS vert
                 LEFT JOIN taxonomy_term_data AS ver ON ver.tid = vert.field_dist_vertical_target_id
                 WHERE vert.entity_id IN (" . $strNid . ")";
                $objVerticals = db_query($strSQL)->fetchAll();
                if (count($objVerticals) > 0) {
                    foreach ($objVerticals as $row) {
                        $arrVerticals[$row->fmrId][] = $row->Verticals;
                    }
                }
            }
            return $arrVerticals;
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
            return $objFMRNidInfo;            
        }

        /**
         * Function to get Primary Category Name
         *
         * @param int $intPrimaryCategoryId : Primary category id
         *
         * @return array
         */
        public function fnPrimaryCategoryName($intPrimaryCategoryId)
        {
            $strPrimaryCategoryName = "";
            if ($intPrimaryCategoryId >0) {
                $strSQL = "SELECT name
                       FROM taxonomy_term_data
                       WHERE tid = $intPrimaryCategoryId";
                $arrPrimaryCategory = db_query($strSQL)->fetchAssoc();
                $strPrimaryCategoryName = stripslashes($arrPrimaryCategory["name"]);
            }
            return $strPrimaryCategoryName;
        }
    }