<?php
//This is class for Analytics functions.

class clsBenchmark
{
    /**
     * To calculate average of views/clicks and stored it against each FMR.
     *
     * @param array $arrTotalViewClickCount  : Total Views and Clicks count
     * @param int   $intTotalFMRCount   : Total FMR count
     * @param array $arrFMRIds          : array of FMR ids
     * @param string $strBenchmarkGroup : Benchmark group (Media and Media-Category)
     * @param string $strMediaType      : Media Type
     * @param array $arrBenchmarkFMRIds : Benchmarks FMR Ids by Media type and Primary Category
     */
    function fnCalculateViewsClicksAverage($arrTotalViewClickCount, $intTotalFMRCount, $arrFMRIds, $strBenchmarkGroup, $strMediaType="", $arrBenchmarkFMRIds = array())
    {
        // Check array count and is_array condition
        if (is_array($arrFMRIds) && count($arrFMRIds) > 0) {

            $intCurrentDate = date("Y-m-d");

            $intTotalViewCount = $arrTotalViewClickCount[$strMediaType]["views"];
            $intTotalClickCount = $arrTotalViewClickCount[$strMediaType]["clicks"];

            // Calculate average views and clicks
            $intAverageViewCount = ($intTotalViewCount != 0 && $intTotalFMRCount != 0) ? round($intTotalViewCount / $intTotalFMRCount) : 0;
            $intAverageClickCount = ($intTotalClickCount != 0 && $intTotalFMRCount != 0) ? round($intTotalClickCount / $intTotalFMRCount) : 0;

            //save FMR average views and clicks count.
            if ($strBenchmarkGroup == "media_category") {
                $this->fnSaveBenchMark($arrBenchmarkFMRIds, $strMediaType, $intAverageViewCount, $intAverageClickCount);
            } else {

                // Get Views and Clicks with video
                $intTotalFMRCountWithVideo = $arrTotalViewClickCount[$strMediaType]["withvideo"]["fmrcount"];
                $intTotalViewCountWithVideo = $arrTotalViewClickCount[$strMediaType]["withvideo"]["views"];
                $intTotalClickCountWithVideo = $arrTotalViewClickCount[$strMediaType]["withvideo"]["clicks"];

                // Calculate average views and clicks with video
                $intAverageViewCountWithVideo = ($intTotalViewCountWithVideo != 0 && $intTotalFMRCountWithVideo != 0) ? round($intTotalViewCountWithVideo / $intTotalFMRCountWithVideo) : 0;
                $intAverageClickCountWithVideo = ($intTotalClickCountWithVideo != 0 && $intTotalFMRCountWithVideo != 0) ? round($intTotalClickCountWithVideo / $intTotalFMRCountWithVideo) : 0;

                // Get Views and Clicks without video
                $intTotalFMRCountWithoutVideo = $arrTotalViewClickCount[$strMediaType]["withoutvideo"]["fmrcount"];
                $intTotalViewCountWithoutVideo = $arrTotalViewClickCount[$strMediaType]["withoutvideo"]["views"];
                $intTotalClickCountWithoutVideo = $arrTotalViewClickCount[$strMediaType]["withoutvideo"]["clicks"];

                // Calculate average views and clicks without video
                $intAverageViewCountWithoutVideo = ($intTotalViewCountWithoutVideo != 0 && $intTotalFMRCountWithoutVideo != 0) ? round($intTotalViewCountWithoutVideo / $intTotalFMRCountWithoutVideo) : 0;
                $intAverageClickCountWithoutVideo = ($intTotalClickCountWithoutVideo != 0 && $intTotalFMRCountWithoutVideo != 0) ? round($intTotalClickCountWithoutVideo / $intTotalFMRCountWithoutVideo) : 0;

                //Insert all details in the table.
                $arrInsertViewsData = array(
                    'benchmark_views'              => $intAverageViewCount,
                    'benchmark_views_withvideo'    => $intAverageViewCountWithVideo,
                    'benchmark_views_withoutvideo' => $intAverageViewCountWithoutVideo,
                    'media_type'                   => $strMediaType,
                    'benchmark_views_month'        => $intCurrentDate,
                );
                db_insert('threebl_benchmark_views_avg_by_mediatype')
                    ->fields($arrInsertViewsData)
                    ->execute();

                //Insert all details in the table.
                $arrInsertClicksData = array(
                    'benchmark_clicks'              => $intAverageClickCount,
                    'benchmark_clicks_withvideo'    => $intAverageClickCountWithVideo,
                    'benchmark_clicks_withoutvideo' => $intAverageClickCountWithoutVideo,
                    'media_type'                    => $strMediaType,
                    'benchmark_clicks_month'        => $intCurrentDate,
                );
                db_insert('threebl_benchmark_clicks_avg_by_mediatype')
                    ->fields($arrInsertClicksData)
                    ->execute();
            }
        }
    }

    public function fnSaveBenchMark($arrBenchmarkFMRIds, $strMediaType, $intAverageViewCount, $intAverageClickCount) {

        if (is_array($arrBenchmarkFMRIds[$strMediaType]) && count($arrBenchmarkFMRIds[$strMediaType]) >0) {

            foreach ($arrBenchmarkFMRIds[$strMediaType] as $intFMRId) {

                //Insert fmr average views and clicks count.
                $arrUpdate = node_load($intFMRId);

                $arrUpdate->field_fmr_average_views['und'][0]['value'] = $intAverageViewCount;
                $arrUpdate->field_fmr_average_clicks['und'][0]['value'] = $intAverageClickCount;

                #Update field_fmr_average_clicks and field_fmr_average_views field to average views and clicks count
                node_save($arrUpdate);
                unset($arrUpdate); //unset the node value
            }
        }
    }

    /**
     * FUnction to get Date duration for Benchmarks data
     * @return array
     */
    function fnGetDateForBenchmarkByMedia()
    {
        $intCurrentDate = date("j");
        $intCurrentMonth = date("m");
        $intCurrentYear = date("Y");

        if ($intCurrentDate >= 1 && $intCurrentDate < 8) {
            $intCurrentMonth = date('m', mktime(0, 0, 0, $intCurrentMonth - 1, 8, $intCurrentYear));
        }

        $intNextMonthDate = date('Y-m', mktime(0, 0, 0, $intCurrentMonth + 1, 8, $intCurrentYear));
        $intCurrentMonthDate = "$intCurrentYear-$intCurrentMonth";

        return $arrStartEndDate = array("Current_month_date" => $intCurrentMonthDate, "next_month_date" => $intNextMonthDate);
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
}