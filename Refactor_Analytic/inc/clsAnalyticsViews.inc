<?php
//This is class for Analytics functions.

class clsAnalyticsViews
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
}