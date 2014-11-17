<?php
    //This is class for Benchmark functions.

    class clsBenchmark
    {
        /**
         * Fetch FMR views information for Benchmark
         *
         * @param string $strFMRIds : Comma separated FMR Ids
         *
         * @return mixed
         */
        function fnGetBenchmarkViews($strFMRIds)
        {
            //Include all required files
            module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

            //Object of class
            $objAnalytics = new clsAnalytic();

            $strSQL = "SELECT SUM(v.totalcount) AS viewcount, v.nid
                         FROM  threebl_analytic_views  AS v
                         JOIN field_data_field_fmr_date_time AS PD ON PD.entity_id = v.nid
                         WHERE v.nid IN ($strFMRIds)
                         AND v.datevisited >= PD.field_fmr_date_time_value AND v.datevisited <= DATE_ADD(PD.field_fmr_date_time_value, INTERVAL 7 DAY)
                         GROUP BY v.nid";
            $objFMRViewInfo = db_query($strSQL)->fetchAll();

            
            //To get view count information
            $arrFMRViewInfo = $objAnalytics->fnViewClickCountInfo($objFMRViewInfo, "view");
            return $arrFMRViewInfo;
        }

        /**
         * Get Benchmarks views information
         *
         * @return array
         */
        public function fnGetBenchmarkViewsByMediaType()
        {
            //Include all required files
            module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

            //Object of class
            $objAnalytics = new clsAnalytic();

            $arrStartEndDate = $this->fnGetDateForBenchmarkByMedia();
            $intCurrentMonthDate = $arrStartEndDate["Current_month_date"];
            $intNextMonthDate = $arrStartEndDate["next_month_date"];

            $strBenchmarkViewsByMediaTypeQuery = "SELECT benchmark_views, benchmark_views_withvideo, benchmark_views_withoutvideo, media_type
                                              FROM threebl_benchmark_views_avg_by_mediatype
                                              WHERE DATE_FORMAT(benchmark_views_month, '%Y-%m') >= '$intCurrentMonthDate'
                                              AND DATE_FORMAT(benchmark_views_month, '%Y-%m') <= '$intNextMonthDate'";
            $arrBenchmarkViewsByMediaType = db_query($strBenchmarkViewsByMediaTypeQuery)->fetchAll();

            //To get Benchmark view count information
            $arrBenchmarkViewsFMRInfo = $objAnalytics->fnGetBenchMarkViewInfo($arrBenchmarkViewsByMediaType);
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
            //Include all required files
            module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

            //Object of class
            $objAnalytics = new clsAnalytic();

            $strSQL = "SELECT COUNT(C.nid) AS clickcount, C.nid
                   FROM  threebl_analytic_clicks  AS C
                   JOIN field_data_field_fmr_date_time AS PD ON PD.entity_id = C.nid
                   WHERE C.nid IN ($strFMRIds)
                   AND C.click_date >= PD.field_fmr_date_time_value AND C.click_date <= DATE_ADD(PD.field_fmr_date_time_value, INTERVAL 168 HOUR)
                   GROUP BY C.nid";
            $objFMRClickInfo = db_query($strSQL)->fetchAll();

            //To get view count information
            $arrFMRClickInfo = $objAnalytics->fnViewClickCountInfo($objFMRClickInfo, "view");
            return $arrFMRClickInfo;
        }

        /**
         * Get Benchmarks views information
         *
         * @return array
         */
        public function fnGetBenchmarkClicksByMediaType()
        {
            //Include all required files
            module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

            //Object of class
            $objAnalytics = new clsAnalytic();

            $arrStartEndDate = $this->fnGetDateForBenchmarkByMedia();
            $intCurrentMonthDate = $arrStartEndDate["Current_month_date"];
            $intNextMonthDate = $arrStartEndDate["next_month_date"];

            $strBenchmarkClicksByMediaTypeQuery = "SELECT benchmark_clicks, benchmark_clicks_withvideo, benchmark_clicks_withoutvideo, media_type
                                               FROM threebl_benchmark_clicks_avg_by_mediatype
                                               WHERE DATE_FORMAT(benchmark_clicks_month, '%Y-%m') >= '$intCurrentMonthDate'
                                               AND DATE_FORMAT(benchmark_clicks_month, '%Y-%m') <= '$intNextMonthDate'";
            $arrBenchmarkClicksByMediaType = db_query($strBenchmarkClicksByMediaTypeQuery)->fetchAll();


            //To get Benchmark view count information
            $arrBenchmarkClicksFMRInfo = $objAnalytics->fnGetBenchMarkViewInfo($arrBenchmarkClicksByMediaType);
            return $arrBenchmarkClicksFMRInfo;
        }

        /**
         * Function return FMRs those have video attached with it for  Benchmark.
         *
         * @return array
         */
        public function fnGetBenchmarkFMRWithVideo()
        {
            //Include all required files
            module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

            //Object of class
            $objAnalytics = new clsAnalytic();

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

            // To get FMR Ids
            $arrFMRIds = $objAnalytics->fnGetFMRIds($arrFMRRecords);
            return $arrFMRIds;
        }

        /**
         * Insert Benchmark Views
         *
         * @param array $arrInsertViewsData: Benchmark Views Information
         *
        */
        function fnInsertBenchmarkViews($arrInsertViewsData)
        {
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
        }

        /**
         * Insert Benchmark Clicks
         *
         * @param array $arrInsertClicksData: Benchmark Clicks Information
         *
        */
        function fnInsertBenchmarkClicks($arrInsertClicksData)
        {
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