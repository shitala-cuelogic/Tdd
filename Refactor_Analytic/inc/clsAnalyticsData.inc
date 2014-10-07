<?php
//This is class for Analytics functions.

class clsAnalyticsData
{
    /**
     * Query for getting View Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId          : company OG id
     * @param int    $strMediaType            : FMR media type
     * @param int    $intChartDateBetweenFlag : boolean flag which indicate functionality for chart data.
     *
     * @return mixed
     */
    public function fnGetFMRViewsByCompanyChart($intCompanyOgId, $strMediaType, $intChartDateBetweenFlag = 0)
    {
        if ((int)$intChartDateBetweenFlag == 1) {
            $strChartDateBetween = " AND (datevisited BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "')";
        }
        $strCondition = '';
        if ($strMediaType != '') {
            if ($strMediaType != "all") {
                $strCondition .= " AND  t.field_fmr_type_of_content_value = '$strMediaType' ";
            } else {
                $strCondition .= " AND  t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')";
            }
        }
        $strSql = "SELECT SUM(v.totalcount) AS totalcount,v.datevisited AS datevisit
                  FROM " . $this->strViewTableName . " AS v
                      JOIN node AS n ON n.nid = v.nid
                      JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                      JOIN field_data_field_fmr_type_of_content t ON n.nid = t.entity_id
                      WHERE v.company_ogid  =  $intCompanyOgId AND  n.status = 1
                      AND pubdt.field_fmr_date_time_value >= DATE_SUB( '" . $this->strCurrentDate . "' , INTERVAL 180 DAY )
                  $strChartDateBetween
                  $strCondition
                  GROUP BY datevisited
                  ORDER by datevisited asc";
        return $arrayViewChartData = db_query($strSql)->fetchAll();
    }

    /**
     * Query for getting View Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId          : Company OG id
     * @param string $strFMRIds               : FMR node id's with comma separated
     * @param int    $intChartDateBetweenFlag : boolean flag which indicate functionality for chart data.
     * @param int    $intCampaignId           : Campaign node id
     *
     * @return mixed
     */
    public function fnGetAnalyticViewChartInfo($intCompanyOgId, $strFMRIds = "", $intChartDateBetweenFlag = 0, $intCampaignId = 0)
    {
        if ((int)$intChartDateBetweenFlag == 1) {
            $strChartDateBetween = " AND (v.datevisited BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "') ";
        }

        $strCampaignFrom = ''; //change
        if ((int)$intCampaignId != 0) {
            $strCampaignFrom = "  JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = v.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        }

        $strCondition = "";
        if ($strFMRIds != "") {
            $strCondition = " AND v.nid IN ($strFMRIds)";
        }
        $strSQL = "SELECT SUM(v.totalcount) as totalcount, v.datevisited AS datevisit
                  FROM " . $this->strViewTableName . " AS v
                  $strCampaignFrom
                  WHERE v.company_ogid = $intCompanyOgId
                  $strChartDateBetween
                  $strCondition
                  GROUP BY datevisited
                  ORDER by datevisited ASC";

        $arrViewChartData = db_query($strSQL)->fetchAll();
        $arrViewChart = array();
        //Checking empty array condition
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
     * @param int    $intCompanyOgId               : Company OG id
     * @param string $strFMRIds                    : FMR node id's with comma separated
     * @param int    $intClickChartDateBetweenFlag : boolean flag which indicate functionality for chart data.
     * @param int    $intCampaignId                : Campaign node id
     *
     * @return mixed
     */
    public function fnGetAnalyticClickChartInfo($intCompanyOgId, $strFMRIds = "", $intClickChartDateBetweenFlag = 0, $intCampaignId = 0)
    {
        $strClickChartDateBetween = '';
        if ((int)$intClickChartDateBetweenFlag == 1) {
            $strClickChartDateBetween = " AND (C.click_date BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "') ";
        }

        $strCampaignFrom = '';
        if ((int)$intCampaignId > 0) {
            $strCampaignFrom = " JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = C.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        }

        $strCondition = "";
        if ($strFMRIds != "") {
            $strCondition = " AND C.nid IN ($strFMRIds)";
        }

        $strSQL = "SELECT DATE_FORMAT(C.click_date, '%Y-%m-%d') as clickdate, count(C.nid) as totalcount
                  FROM " . $this->strClickTableName . " as C
                  $strCampaignFrom
                  WHERE C.company_ogid = $intCompanyOgId
                  $strClickChartDateBetween
                  $strCondition
                  GROUP BY  DATE_FORMAT(C.click_date, '%Y/%m/%d')
                  ORDER by C.click_date asc";

        $arrayClickChartData = db_query($strSQL)->fetchAll();
        $arrClickChart = array();

        //Checking empty array condition
        if (!empty($arrayClickChartData)) {
            foreach ($arrayClickChartData as $arrVal) {
                $arrClickChart[$arrVal->clickdate] = $arrVal->totalcount;
            }
        }
        return $arrClickChart;
        ## For test put 2013-01-18 .prev '".$this->strCurrentDate."'
    }

    /**
     * Query for Google Map for Click Count for Second Level and Third Level
     *
     * @param int   $intCompanyOgId   :  Company OG id
     * @param mixed $strMediaTypeORId : value will be either media type or campaign id
     *
     * @return mixed
     */
    public function fnGetFMRClicksByCountryGoogleMap($intCompanyOgId, $strMediaTypeORId)
    {
        $strCondition = '';
        $strCndClk = '';
        if ((int)$strMediaTypeORId == 0 && $strMediaTypeORId != '') {
            //getting particular media-type condition for clicks.
            if ($strMediaTypeORId == "all") {
                $strCndClk .= " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter') ";
            } else {
                $strCndClk .= " AND C.fmr_type ='" . $strMediaTypeORId . "'";
            }
        } elseif ((int)$strMediaTypeORId > 0) {
            $strCndClk .= " AND C.nid = $strMediaTypeORId ";
        }
        $strCondition .= " $strCndClk  GROUP BY UPPER(country) order by ipcount desc ";

        $strFinalSql = "SELECT nid,click_ip,count(nid) as ipcount,country
									  FROM " . $this->strClickTableName . " as C
									  JOIN field_data_field_fmr_type_of_content t ON  C.nid = t.entity_id
									  WHERE C.company_ogid = $intCompanyOgId
									  $strCondition";

        return $arrayClickMapData = db_query($strFinalSql)->fetchAll();
    }

    /**
     * Query for Referral website for Second Level and Third Level
     *
     * @param int   $intCompanyOgId    :  Company OG id
     * @param mixed $strMediaTypeORId  : value will be either media type or campaign id
     *
     * @return mixed
     */
    public function fnGetFMRReferrerClicksByMediaType($intCompanyOgId, $strMediaTypeORId)
    {
        $strCondition = '';
        $strCndClk = '';
        if ((int)$strMediaTypeORId == 0 && $strMediaTypeORId != '') {
            //getting particular media-type condition for clicks.
            if ($strMediaTypeORId == "all") {
                $strCndClk .= " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter') ";
            } else {
                $strCndClk .= " AND C.fmr_type ='" . $strMediaTypeORId . "' ";
            }
        } elseif ((int)$strMediaTypeORId > 0) {
            $arrExtraLink = array('bestgenericviagra.biz', 'blog.pnarp.com', 'diffbot.com', 'timberland.justmeans.com', 'www.mysmutsearch.com', 'lala123.com', '_bit=528e713d-', '_bit=52af68a5-00111-07550', 'piwik.3blmedia.com', 'stats.3blmedia.com');
            $strExtraLink = implode("|", $arrExtraLink);
            $strCndClk .= " AND C.nid = $strMediaTypeORId AND click_referrer NOT RLIKE '$strExtraLink' ";
        }

        $strCondition .= " $strCndClk  GROUP BY click_referrer Order by click_date desc ";

        $strFinalQuery = "SELECT distinct click_referrer as rlink
								   FROM " . $this->strClickTableName . " as C
								   JOIN field_data_field_fmr_type_of_content t ON  C.nid = t.entity_id
								   WHERE C.company_ogid = $intCompanyOgId
								   AND C.click_referrer <> '' $strCondition";
        return $arrayClickReferData = db_query($strFinalQuery)->fetchAll();
    }

    /**
     *  Getting View-Count By Media-Id
     *
     * @param int  $intCompanyOgId :  Company OG id
     * @param int  $intMediaId     : FMR node id
     *
     * @return mixed
     */
    public function fnGetFMRViewCountByMediaId($intCompanyOgId, $intMediaId)
    {
        $strSQL = "SELECT  SUM(v.totalcount) as totalcount
                  FROM " . $this->strViewTableName . " as v
                  JOIN field_data_field_fmr_type_of_content t ON  v.nid = t.entity_id
                  Where v.company_ogid = $intCompanyOgId AND v.nid = $intMediaId
                  GROUP BY  v.nid";
        return $arrViewCount = db_query($strSQL)->fetchAssoc();
    }

    /**
     * Getting Click-Count By Media-Id
     *
     * @param int  $intCompanyOgId :  Company gid
     * @param int  $intMediaId     : FMR node id
     *
     * @return mixed
     */
    public function fnGetFMRClickCountByMediaId($intCompanyOgId, $intMediaId)
    {
        $strClickCount = "SELECT C.click_date, count(C.nid) as totalcount,fmr_type as type
								  FROM " . $this->strClickTableName . " as C
								  Where C.company_ogid = $intCompanyOgId AND C.nid = $intMediaId
								  GROUP BY nid";
        return $arrClickCount = db_query($strClickCount)->fetchAssoc();
    }

    /**
     * To get Particular Media Type Views
     *
     * @param int       $intCompOgId : Company OG id
     * @param string    $strType     : FMR media type
     * @param int       $intGroupBy  : flag for group by condition
     * @param int       $intStart    : starting number for pagination
     * @param int       $intLimit    : number of records per page
     * @param int       $boolReport  : flag indicate data process for report of analytic section.
     *
     * @return mixed
     */
    public function fnGetFMRViewCountByMediaType($intCompOgId, $strType = "", $intGroupBy = 0, $intStart = 0, $intLimit = 10, $boolReport = 0)
    {
        $strCnd = '';
        if ($strType != '') {
            if ($strType != "all") {
                $strCnd = " AND  t.field_fmr_type_of_content_value = '$strType' ";
            } else {
                $strCnd = " AND  t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') ";
            }
        }
        $strSelect = " SELECT N.nid as nid, N.title as title, SUM(v.totalcount) as viewcount, v.datevisited as datevisit,
	                 pubdt.field_fmr_date_time_value as publishdate ";

        if ($intLimit == 0 && $intGroupBy == 0) {
            $strSelect = " SELECT count(DISTINCT(N.nid)) as rowCount ";
        }

        $strFrom = " FROM " . $this->strViewTableName . " as v
                        JOIN node AS N ON N.nid = v.nid
                        JOIN field_data_field_fmr_type_of_content t ON  N.nid = t.entity_id
						JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = N.nid	";

        $strWhere = " WHERE v.company_ogid = $intCompOgId AND N.status = 1 ";
        $strReportGroupBy = "";
        if ($boolReport == 1) {
            $strSelect .= " ,  t.field_fmr_type_of_content_value as type, CN.title as campaign, fmrv.field_video_thumbnail as video ";
            $strFrom .= " LEFT JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = N.nid
                        LEFT JOIN node AS CN ON CN.nid = COM.field_fmr_campaign_nid
                        LEFT JOIN field_data_field_video AS fmrv ON ( fmrv.bundle = 'fmr' AND fmrv.entity_id = N.nid)";
            $strReportGroupBy = " AND  t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')
			                      GROUP BY N.nid ORDER BY pubdt.field_fmr_date_time_value DESC ";
        }
        $strSql = $strSelect . $strFrom . $strWhere;
        $strSql .= $strCnd . $strReportGroupBy;

        if ($intGroupBy == 1) {
            $strSql .= " GROUP BY N.nid ORDER BY pubdt.field_fmr_date_time_value DESC";
            if ($intLimit > 0) {
                $strSql .= " LIMIT $intStart, $intLimit ";
            }
        }

        if ($intLimit == 0 && $intGroupBy == 0) {
            $ViewTypeResult = db_query($strSql)->fetchAssoc();
        } else {
            $ViewTypeResult = db_query($strSql)->fetchAll();
        }
        return $ViewTypeResult;
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
     * Function Update View table for Company id
     *
     * @param int  $intCompOgId : Company OG id
     * @param int  $intSynId    : Company JM id
     */
    public function fnUpdateClicksCompany($intCompOgId = 0, $intSynId = 0)
    {
        $strUpdateQuery = "Update " . $this->strClickTableName . " SET company_ogid =" . $intCompOgId . " Where company_ogid =" . $intSynId . " And click_date >= DATE_SUB( '2013-07-10' , INTERVAL 180 DAY ) ";
        db_query($strUpdateQuery)->execute();
    }

    /**
     * Function Update clicks table for Company id
     *
     * @param  int   $intCompOgId : Company OG id
     * @param int    $intSynId    : Company JM id
     */
    public function fnUpdateViewCompany($intCompOgId = 0, $intSynId = 0)
    {
        $strUpdateQuery = "Update " . $this->strViewTableName . " SET company_ogid	 =" . $intCompOgId . " Where company_ogid	 =" . $intSynId . " And datevisited >= DATE_SUB( '2013-07-10' , INTERVAL 180 DAY ) ";
        db_query($strUpdateQuery)->execute();
    }

    /**
     * Function return all FMRs that get published for excel sheet download from report
     *
     * @param int    $intCompanyOgId : Company OG id
     * @param string $strMediaType   : FMR media type
     * @param string $intCampaignId  : Campaign node id
     *
     * @return array
     */
    public function fnGetCompanyFMRsExcel($intCompanyOgId, $strMediaType = "", $intCampaignId = "")
    {
        $strSelect = "SELECT n.nid, t.field_fmr_type_of_content_value AS fmr_type, td.name AS primarycategory, av.field_fmr_average_views_value as benchmark_view, ac.field_fmr_average_clicks_value as benchmark_click, ar.field_dist_archive_value AS is_archive";

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
                $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') ";
            }
        }
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

        $strOrderBy = ($strMediaType == "") ? " ORDER BY t.field_fmr_type_of_content_value " : " ORDER BY pubdt.field_fmr_date_time_value DESC ";

        if ($strMediaType != "") {
            $strSelect .= ", n.title as title, pubdt.field_fmr_date_time_value, CN.title as campaign, fmrv.field_video_thumbnail as video ";
            $strCampaignFrom = " LEFT JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = n.nid LEFT JOIN node AS CN ON CN.nid = COM.field_fmr_campaign_nid
                                 LEFT JOIN field_data_field_video AS fmrv ON (fmrv.bundle = 'fmr' AND fmrv.entity_id = n.nid) ";
        }

        $arrFMRIds = array();
        $arrFMRInfo = array();

        #Final SQL
        $strQry = $strSelect . $strFrom . $strCampaignFrom . $strWhere . $strCondition . $strOrderBy;
        $objFMRNidInfo = db_query($strQry)->fetchAll();

        if ((int)$intCampaignId == 0 && $strMediaType == "") {
            foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                $arrFMRInfo[$arrFMRNidInfo->fmr_type][] = $arrFMRNidInfo->nid;
                $arrFMRIds[] = $arrFMRNidInfo->nid;
            }
        } elseif ((int)$intCampaignId > 0 || $strMediaType != "") {
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
        //Implode FMR ids
        $strFMRIds = implode(', ', $arrFMRIds);
        return array($arrFMRInfo, $arrFMRIds, $strFMRIds);
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
        $strSelect = "SELECT sql_calc_found_rows n.nid, t.field_fmr_type_of_content_value AS fmr_type, av.field_fmr_average_views_value as benchmark_view, ac.field_fmr_average_clicks_value as benchmark_click, td.name AS primary_category, field_dist_archive_value as is_archive ";

        $strCondition = '';
        $strCampaignFrom = ''; //change

        if ($strMediaType != '') {
            if ($strMediaType != "all") {
                $strCondition .= " AND t.field_fmr_type_of_content_value = '$strMediaType' ";
            } else {
                $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') ";
            }
        } else {
            $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')  ";
        }

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

        $strOrderBy = ($strMediaType == "") ? " ORDER BY t.field_fmr_type_of_content_value " : " ORDER BY pubdt.field_fmr_date_time_value DESC ";

        if ($strMediaType != "") {
            $strSelect .= ", n.title as title, pubdt.field_fmr_date_time_value ";
        }

        $strLimitSql = "";
        if ($intLimit > 0) {
            $strLimitSql .= " LIMIT $intStart, $intLimit ";
        }

        $arrFMRIds = array();
        $arrFMRInfo = array();

        #Final SQL
        $strQry = $strSelect . $strFrom . $strCampaignFrom . $strWhere . $strCondition . $strOrderBy . $strLimitSql;

        $objFMRNidInfo = db_query($strQry)->fetchAll();
        $strFMRIds = '';

        //Get count of all Records
        list($arrRecordsCount) = db_query('SELECT FOUND_ROWS()')->fetchAll();
        $arrRecordsCount = (array) ($arrRecordsCount);
        $intViewClickCntResult = $arrRecordsCount['FOUND_ROWS()'];

        //Checking if array is not empty
        if (!empty($objFMRNidInfo)) {

            if ($strMediaType == "") {
                foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                    $arrFMRInfo[$arrFMRNidInfo->fmr_type][] = $arrFMRNidInfo->nid;
                    $arrFMRIds[] = $arrFMRNidInfo->nid;
                }
            } elseif ($strMediaType != "") {
                foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                    $arrFMRInfo[$arrFMRNidInfo->nid]['nid'] = $arrFMRNidInfo->nid;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['title'] = $arrFMRNidInfo->title;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['publishdate'] = $arrFMRNidInfo->field_fmr_date_time_value;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['campaign'] = $arrFMRNidInfo->campaign;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['media'] = $arrFMRNidInfo->fmr_type;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['video'] = $arrFMRNidInfo->video;

                    //getting Benchmark views and clicks
                    $strFMRPublishedDate = date("Y-m-d", strtotime($arrFMRNidInfo->field_fmr_date_time_value));
                    if(strtotime($strFMRPublishedDate) <= strtotime($this->strBenchmarkNAMonth)) {
                        // Set N/A value for Benchmark views and clicks, if FMR published before Apr 2014
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_view'] = "N/A";
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_click'] = "N/A";
                    } else {
                        // Set value for Benchmark views and clicks
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_view'] = ((int) $arrFMRNidInfo->benchmark_view > 0) ? number_format((int) $arrFMRNidInfo->benchmark_view) : "-";
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_click'] = ((int) $arrFMRNidInfo->benchmark_click > 0) ? number_format((int) $arrFMRNidInfo->benchmark_click) : "-";
                    }

                    $arrFMRInfo[$arrFMRNidInfo->nid]['primary_category'] = $arrFMRNidInfo->primary_category;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['is_archive'] = $arrFMRNidInfo->is_archive;
                    $arrFMRIds[] = $arrFMRNidInfo->nid;
                }
            }
            //Implode FMR ids
            $strFMRIds = implode(', ', $arrFMRIds);
        }
        return array($arrFMRInfo, $arrFMRIds, $strFMRIds, $intViewClickCntResult);
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
            foreach ($objFMRNidInfo as $arrFMRData) {

                $strPublishedDate = date("Y-m-d", strtotime($arrFMRData->field_fmr_date_time_value));
                $strFMRPublishedDate = strtotime($strPublishedDate);

                //Get FMR Ids for the current Benchmarks Month
                if ((int)$strFMRPublishedDate >= (int)$strBenchmarkMonthDate) {
                    $arrBenchmarkFMRIds[$arrFMRData->fmr_type."-".$arrFMRData->category_id][] = $arrFMRData->fmrId;
                }
                $arrFMRIds[] = $arrFMRData->fmrId;
                $arrBenchmarkFMRInfo[$arrFMRData->fmr_type."-".$arrFMRData->category_id][] = $arrFMRData->fmrId;
                $arrBenchMarkByMediaType[$arrFMRData->fmr_type][] = $arrFMRData->fmrId;
            }
            //Implode FMR ids
            $strFMRIds = implode(', ', $arrFMRIds);
        }
        return array($arrBenchmarkFMRInfo, $arrFMRIds, $strFMRIds, $arrBenchMarkByMediaType, $arrBenchmarkFMRIds);
    }

    /**
     * Fetch FMR views information for Benchmark
     *
     * @param string $strFMRIds : Comma separated FMR Ids
     *
     * @return mixed
     */
    function fnGetBenchmarkViews($strFMRIds)
    {
        $strSQL = "SELECT SUM(v.totalcount) AS view_count, v.nid
                         FROM " . $this->strViewTableName . " AS v
                         JOIN field_data_field_fmr_date_time AS PD ON PD.entity_id = v.nid
                         WHERE v.nid IN ($strFMRIds)
                         AND v.datevisited >= PD.field_fmr_date_time_value AND v.datevisited <= DATE_ADD(PD.field_fmr_date_time_value, INTERVAL 7 DAY)
                         GROUP BY v.nid";
        $objFMRViewInfo = db_query($strSQL)->fetchAll();

        $arrFMRViewInfo = array();

        //Checking is empty
        if (!empty($objFMRViewInfo)) {

            # Get FMR view count
            foreach ($objFMRViewInfo as $objFMRView) {
                $arrFMRViewInfo[$objFMRView->nid] = $objFMRView->view_count;
            }
        }
        return $arrFMRViewInfo;
    }

    /**
     * Fetch FMR Click information for Benchmark
     *
     * @param string $strFMRIds : Comma separated FMR Ids
     *
     * @return mixed
     */
    function fnGetBenchmarkClick($strFMRIds)
    {
        $strSQL = "SELECT COUNT(C.nid) AS click_count, C.nid
                   FROM " . $this->strClickTableName . " AS C
                   JOIN field_data_field_fmr_date_time AS PD ON PD.entity_id = C.nid
                   WHERE C.nid IN ($strFMRIds)
                   AND C.click_date >= PD.field_fmr_date_time_value AND C.click_date <= DATE_ADD(PD.field_fmr_date_time_value, INTERVAL 168 HOUR)
                   GROUP BY C.nid";
        $objFMRClickInfo = db_query($strSQL)->fetchAll();

        $arrFMRClickInfo = array();

        //Checking is empty
        if (!empty($objFMRClickInfo)) {

            # Get FMR Click count
            foreach ($objFMRClickInfo as $objFMRClick) {
                $arrFMRClickInfo[$objFMRClick->nid] = $objFMRClick->click_count;
            }
        }
        return $arrFMRClickInfo;
    }

    /**
     * Function return FMRs those have video attached with it for  Benchmark.
     *
     * @return array
     */
    public function fnGetBenchmarkFMRWithVideo()
    {
        //Get start and End date of Previous month
        $arrStartEndDate = $this->fnGetStartEndDate(2);
        $strGetStartDate = $arrStartEndDate["start_date"];
        $strGetEndDate = $arrStartEndDate["end_date"];

        //Collect all FMRs With Videos
        $strFMRWithVideos = "SELECT DISTINCT(fmrv.entity_id) AS fmrIds
                             FROM field_data_field_video AS fmrv
                             JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = fmrv.entity_id
                             WHERE fmrv.bundle = 'fmr'
                             AND (DATE_FORMAT(pubdt.field_fmr_date_time_value,'%Y-%m-%d') >='".$strGetStartDate."'
                             AND DATE_FORMAT(pubdt.field_fmr_date_time_value,'%Y-%m-%d') <= '".$strGetEndDate."')";

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
     * Get Benchmarks views information
     *
     * @return array
     */
    public function fnGetBenchmarkViewsByMediaType()
    {
        $arrStartEndDate = $this->fnGetDateForBenchmarkByMedia();
        $intCurrentMonthDate = $arrStartEndDate["Current_month_date"];
        $intNextMonthDate = $arrStartEndDate["next_month_date"];

        $strBenchmarkViewsByMediaTypeQuery = "SELECT benchmark_views, benchmark_views_withvideo, benchmark_views_withoutvideo, media_type
                                              FROM threebl_benchmark_views_avg_by_mediatype
                                              WHERE DATE_FORMAT(benchmark_views_month, '%Y-%m') >= '$intCurrentMonthDate'
                                              AND DATE_FORMAT(benchmark_views_month, '%Y-%m') <= '$intNextMonthDate'";
        $arrBenchmarkViewsByMediaType = db_query($strBenchmarkViewsByMediaTypeQuery)->fetchAll();

        $arrBenchmarkViewsFMRInfo = array();

        //Checking is empty
        if (!empty($arrBenchmarkViewsByMediaType)) {
            # Get Benchmark view count
            foreach ($arrBenchmarkViewsByMediaType as $objBenchmarkViews) {
                $arrBenchmarkViewsFMRInfo[$objBenchmarkViews->media_type]["benchmark_views"] = $objBenchmarkViews->benchmark_views;
                $arrBenchmarkViewsFMRInfo[$objBenchmarkViews->media_type]["benchmark_views_withvideo"] = $objBenchmarkViews->benchmark_views_withvideo;
                $arrBenchmarkViewsFMRInfo[$objBenchmarkViews->media_type]["benchmark_views_withoutvideo"] = $objBenchmarkViews->benchmark_views_withoutvideo;
            }
        }
        return $arrBenchmarkViewsFMRInfo;
    }

    /**
     * Get Benchmarks views information
     *
     * @return array
     */
    public function fnGetBenchmarkClicksByMediaType()
    {
        $arrStartEndDate = $this->fnGetDateForBenchmarkByMedia();
        $intCurrentMonthDate = $arrStartEndDate["Current_month_date"];
        $intNextMonthDate = $arrStartEndDate["next_month_date"];

        $strBenchmarkClicksByMediaTypeQuery = "SELECT benchmark_clicks, benchmark_clicks_withvideo, benchmark_clicks_withoutvideo, media_type
                                               FROM threebl_benchmark_clicks_avg_by_mediatype
                                               WHERE DATE_FORMAT(benchmark_clicks_month, '%Y-%m') >= '$intCurrentMonthDate'
                                               AND DATE_FORMAT(benchmark_clicks_month, '%Y-%m') <= '$intNextMonthDate'";
        $arrBenchmarkClicksByMediaType = db_query($strBenchmarkClicksByMediaTypeQuery)->fetchAll();

        $arrBenchmarkClicksFMRInfo = array();

        //Checking is empty
        if (!empty($arrBenchmarkClicksByMediaType)) {
            # Get Benchmark view count
            foreach ($arrBenchmarkClicksByMediaType as $objBenchmarkClicks) {
                $arrBenchmarkClicksFMRInfo[$objBenchmarkClicks->media_type]["benchmark_clicks"] = $objBenchmarkClicks->benchmark_clicks;
                $arrBenchmarkClicksFMRInfo[$objBenchmarkClicks->media_type]["benchmark_clicks_withvideo"] = $objBenchmarkClicks->benchmark_clicks_withvideo;
                $arrBenchmarkClicksFMRInfo[$objBenchmarkClicks->media_type]["benchmark_clicks_withoutvideo"] = $objBenchmarkClicks->benchmark_clicks_withoutvideo;
            }
        }
        return $arrBenchmarkClicksFMRInfo;
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

    /**
     * Get Top five clicks count and their FMR information
     * @param int $intCompanyOgId : Company OG Id
     *
     * @return array
     */
    public function fnGetTopFiveClicksFMRs($intCompanyOgId)
    {
        $strQry = "SELECT n.nid, t.field_fmr_type_of_content_value AS fmr_type, td.name AS primarycategory, COUNT(C.nid) AS clickcount, n.title, pubdt.field_fmr_date_time_value
                   FROM node AS n
                   JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                   JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                   JOIN field_data_field_fmr_type_of_content AS t ON n.nid = t.entity_id
                   LEFT JOIN field_data_field_fmr_primary_category AS prim ON n.nid = prim.entity_id
                   LEFT JOIN taxonomy_term_data AS td ON td.tid = prim.field_fmr_primary_category_target_id
                   JOIN field_data_field_dist_archive AS ar ON n.nid = ar.entity_id
                   JOIN $this->strClickTableName AS C ON C.nid = n.nid
                   WHERE n.status = 1 AND ga.group_audience_gid = $intCompanyOgId
                   AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')
                   AND pubdt.field_fmr_date_time_value >= '$this->strChartPrevDate' AND pubdt.field_fmr_date_time_value <= '$this->strCurrentDate'
                   AND ar.field_dist_archive_value = 0
                   GROUP BY C.nid
                   ORDER BY clickcount DESC
                   LIMIT 0,5";
        #Final SQL
        $objFMRNidInfo = db_query($strQry)->fetchAll();

        $arrTopClicksFMRInfo = array();

        if (!empty($objFMRNidInfo)) {
            foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['nid'] = $arrFMRNidInfo->nid;
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['title'] = $arrFMRNidInfo->title;
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['publishdate'] = $arrFMRNidInfo->field_fmr_date_time_value;
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['media'] = $this->arrShowMediaType[$arrFMRNidInfo->fmr_type];
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['primary_category'] = $arrFMRNidInfo->primarycategory;
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['clickcount'] = $arrFMRNidInfo->clickcount;
            }
        }
        //Implode FMR ids
        return $arrTopClicksFMRInfo;
    }
}