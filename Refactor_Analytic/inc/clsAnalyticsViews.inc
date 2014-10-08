<?php
//This is class for Analytics functions.

class clsAnalyticsViews
{
    /**
     * Query for getting View Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId          : Company OG id
     * @param string $strFMRIds               : FMR node id's with comma separated
     *
     * @return mixed
     */
    public function fnGetAnalyticViewChartInfo($intCompanyOgId, $strFMRIds = "")
    {
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