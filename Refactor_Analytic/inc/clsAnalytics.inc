<?php
//This is class for Analytics functions.

    class clsAnalytic
    {
        private $arrShowMediaType = array();
        public $strBenchmarkNAMonth;

        public function __construct()
        {
            $this->arrShowMediaType = array(
                'press_release' => 'Press Release',
                'blog' => 'Blog',
                'multimedia' => 'Multimedia',
                'article' => 'Article',
                'newsletter' => 'Newsletter',
                'all' => 'All'
            );

            $this->strBenchmarkNAMonth = '2014-03-31';
        }

        /**
         * Function manage current selected tab css.
         *
         * @param string $strAction : Current selected tab.
         *
         * @return string
         */
        public function fnAnalyticsMaintab($strAction)
        {
            global $base_url;
            global $strView;
            global $strTrack;
            global $strSearch;
            global $strReports;
            switch ($strAction) {
                case 'View':
                    $strView = ' class="active"';
                    break;
                case 'Search':
                    $strSearch = ' class="active"';
                    break;
                case 'Track':
                    $strTrack = ' class="active"';
                    break;
                case 'Reports':
                    $strReports = ' class="active"';
                    break;
            }
            //Analytics/
            $strTabHtml = '<ul class="anlyticsTab">
            <li' . $strView . '><span><a href="' . $base_url . '/Dashboard/Analytics/Views">Analytics</a></span></li>
            <li' . $strTrack . '><span><a href="' . $base_url . '/Dashboard/Analytics/Tracking">Tracking</a></span></li>
            <li' . $strSearch . '><span><a href="' . $base_url . '/Dashboard/Analytics/Searching">Search Engines</a></span></li>
            <li' . $strReports . '><span><a href="' . $base_url . '/Dashboard/Analytics/Reports">Reports</a></span></li>
        </ul>';
            return $strTabHtml;
        }

        /**
         * Primary Categories array to replace long title with short title
         * @return array
         */
        function fnGetPrimaryCategoryShortName()
        {
            $arrLongPrimaryCategoryName = array(
                "Environment and Climate Change"            => "Enviro & Climate Change",
                "Diversity and Human Resources"             => "Diversity & HR",
                "Healthcare Institutional News"             => "Healthcare News",
                "Markets, Regulation & Policy"              => "Markets",
                "Pharma News, Research & Solutions"         => "Pharma News",
                "Ethical Production and Consumption"        => "Ethical Production",
                "Events, Conferences & Webinars"            => "Events",
                "Finance & Socially Responsible Investment" => "Finance & SRI",
                "Social Innovation and Entrepreneurship"    => "Social Innovation",
                "Volunteerism & Community Engagement"       => "Volunteerism & Community",
                "Climate and the environment"               => "Climate and the enviro"
            );

            return $arrLongPrimaryCategoryName;
        }

        /**
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
         * Get fmr media type
         */
        public function getMediaType($strKey)
        {
            return $this->arrShowMediaType[$strKey];
        }

        /**
         * Get fmr media type array
         */
        public function getMediaTypeArray()
        {
            return $this->arrShowMediaType;
        }

        /**
         * To return FMR information
         *
         * @param $objFMRNidInfo: Object of FMR result
         * @param $strMediaType:  FMR Media TYpe
         *
         * @return array
         */
        public function fnGetFMRInfo($objFMRNidInfo, $strMediaType)
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
         * To calculate average of views/clicks and stored it against each FMR.
         *
         * @param array $arrTotalViewClickCount  : Total Views and Clicks count
         * @param int   $intTotalFMRCount   : Total FMR count
         * @param array $arrFMRIds          : array of FMR ids
         * @param string $strBenchmarkGroup : Benchmark group (Media and Media-Category)
         * @param string $strMediaType      : Media Type
         * @param array $arrBenchmarkFMRIds : Benchmarks FMR Ids by Media type and Primary Category
         */
        public function fnCalculateViewsClicksAverage($arrTotalViewClickCount, $intTotalFMRCount, $arrFMRIds, $strBenchmarkGroup, $strMediaType="", $arrBenchmarkFMRIds = array())
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

                    //Include all required files
                    module_load_include('inc', 'refactor_Analytic', 'inc/clsBenchmark');

                    //Object of class
                    $objBenchmark = new clsBenchmark();

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

                    //Insert Benchmark Views details in the table.
                    $objBenchmark->fnInsertBenchmarkViews($arrInsertViewsData);
                    
                    //Insert Benchmark Clicks details in the table.
                    $objBenchmark->fnInsertBenchmarkClicks($arrInsertClicksData);    
                }
            }
        }

        public function fnSaveBenchMark($arrBenchmarkFMRIds, $strMediaType, $intAverageViewCount, $intAverageClickCount) 
        {

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
        public function fnGetDateForBenchmarkByMedia()
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
         * To return Benchmark information
         * @param object $objFMRNidInfo: Object of FMR result
         * @return array
         */
        public function fnGetBenchmarkInfo($objFMRNidInfo)
        {
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

            return array($arrBenchmarkFMRInfo, $arrFMRIds, $arrBenchMarkByMediaType, $arrBenchmarkFMRIds);
        }

        /**
         * Process view data for cron functionality and save analytics in to the view table.
         *
         * @param array        $arrFmrViews : Array of FMRs node ids
         * @param string       $otherInfo   : Other information such as date
         * @param string       $strChannel  : default channel value for save record into the database
         */
        public function fnViewsProcessing($arrFmrViews, $otherInfo, $strChannel = "3blmedia")
        {
            // Get out the IP addr and date
            $strJustDate = $otherInfo['view_date'];

            // fmrViews is a 2-D array with the first dimension being associative with FMR types and the second
            // dimension being an array of FMR IDs (nids) for each FMR type.
            // There can be multiple media's for inbound and we will process each of them.
            if (count($arrFmrViews) && is_array($arrFmrViews)) {
                foreach ($arrFmrViews as $strFMRType => $FMRids) {
                    if (count($FMRids) && is_array($FMRids)) {
                        $this->fnAddViewsProcess($FMRids, $strFMRType, $strChannel, $strJustDate);

                    }
                }
            }
        }

        public function fnAddViewsProcess($FMRids, $strFMRType, $strChannel, $strJustDate) 
        {

            foreach ($FMRids as $FMRid) {

                if ((int)$FMRid > 0) {
                    // for each FMR, we need to get the master ids, from the sync tables, for
                    // both the FMR and Company
                    $arrMedia = $this->fnGetCompanyOgIds($FMRid);


                    if (!empty($arrMedia['etid'])) {
                        $intFMRNid = $arrMedia['etid'];
                    }
                    if (!empty($arrMedia['companyOgId'])) {
                        $intCompanyOgId = $arrMedia['companyOgId'];
                    }


                    // Set $strMediaType  to the appropriate jm_mediawise_views column name, to be used in
                    // the update/insert queries, below.

                    list($strMediaType, $intFMRNid, $intCompanyOgId) = $this->fnSetMediaType($strFMRType, $intFMRNid, $intCompanyOgId);

                    // This is to insert count in views table as well as in the dummy views table, we took dummy table because it will have data for a month which will automatically gets deleted after that this will keep fast processing of data. We can grab all the historic data from main views table, also count   gets incremented as media is viewed.
                    if ($strMediaType && (int)$intFMRNid > 0 && (int)$intCompanyOgId > 0) {
                        //Creating the object for ClickView Class
                        $ObjClickView = new clsClickView();

                        //Create array for view and click tables
                        $arrAllView = array();
                        $arrAllView['intCompanyId'] = $intCompanyOgId;
                        $arrAllView['intMediaId'] = $intFMRNid;
                        $arrAllView['strChannel'] = $strChannel;
                        $arrAllView['strFMRType'] = $strMediaType;
                        $arrAllView['strViewDate'] = $strJustDate;
                        //Add View Table  Function
                        $ObjClickView->fnAddAllViews($arrAllView);
                    }
                }
            }
        }

        public function fnSetMediaType($strFMRType, $intFMRNid, $intCompanyOgId) 
        {
            switch ($strFMRType) {
                        case "pressrelease":
                        case "press_release":
                        case "press":
                            $strMediaType = 'press_release'; // this is how it is stored in jm_..._views tables
                            break;
                        case "article":
                        case "3bl_article":
                            $strMediaType = 'article';
                            break;
                        case "newsletter":
                        case "3bl_newsletter":
                            $strMediaType = 'newsletter';
                            break;
                        case "multimedia":
                            $strMediaType = 'multimedia';
                            break;
                        case "blog":
                        case "blogs":
                            $strMediaType = 'blog';
                            break;
                        default:
                            $intFMRNid = 0;
                            $strMediaType = "";
                            $intCompanyOgId = 0;
                            break;
            }
            return array($strMediaType, $intFMRNid, $intCompanyOgId);
        }

        /**
        * To get Top Clicks FMR information
        * @param object $objFMRNidInfo: Object of FMR result
        * @return array
        */
        public function fnGetTopClicksInformation($objFMRNidInfo)
        {
            $arrTopClicksFMRInfo = array();

            if (!empty($objFMRNidInfo)) {
            
                $this->fnGetTopClicksInformation($objFMRNidInfo);

                foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                    $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['nid'] = $arrFMRNidInfo->nid;
                    $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['title'] = $arrFMRNidInfo->title;
                    $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['publishdate'] = $arrFMRNidInfo->field_fmr_date_time_value;
                    $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['media'] = $this->arrShowMediaType[$arrFMRNidInfo->fmr_type];
                    $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['primary_category'] = $arrFMRNidInfo->primarycategory;
                    $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['clickcount'] = $arrFMRNidInfo->clickcount;
                }
            }
            return $arrTopClicksFMRInfo;
        }

        
        /**
        * To get Clicks and Views count information
        * @param object $objFMRViewClickInfo: Object of views/clicks information
        * @param string $strAction: Click or View action
        * @return array
        */
        public function fnViewClickCountInfo($objFMRViewClickInfo, $strAction)
        {
            $arrFMRInfo = array();

            //Checking is empty
            if (!empty($objFMRViewClickInfo)) {
                # Get FMR Click count
                foreach ($objFMRViewClickInfo as $objArrRow) {
                    $arrFMRInfo[$objArrRow->nid] = ($strAction == "click") ? $objArrRow->clickcount : $objArrRow->viewcount;
                }
            }
            return $arrFMRInfo;
        }

        /**
        * To get Clicks and Views chart information
        * @param object $arrayClickChartData: Object of FMR clicks/views
        * @param string $strAction: Click or View action
        * @return array
        */
        public function fnViewClickChartInfo($arrayViewClickChartData, $strAction)
        {
            $arrViewClickChart = array();

            //Checking empty condition
            if (!empty($arrayViewClickChartData)) {

                foreach ($arrayViewClickChartData as $arrVal) {
                    if ($strAction == "click") {
                        $arrViewClickChart[$arrVal->clickdate] = $arrVal->totalcount;
                    } else {
                        $arrViewClickChart[$arrVal->datevisit] = $arrVal->totalcount;
                    }
                    
                }
            }
            return $arrViewClickChart;
        }

        /**
        * To get Benchmark Views count information
        * @param object $arrBenchmarkViewsByMediaType: Object of views information
        * @return array
        */
        public function fnGetBenchMarkViewInfo($arrBenchmarkViewsByMediaType)
        {
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
        * To get Benchmark Click count information
        * @param object $arrBenchmarkViewsByMediaType: Object of click information
        * @return array
        */
        public function fnGetBenchMarkClickInfo($arrBenchmarkClicksByMediaType)
        {
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
        * To get FMR Ids
        * @param object $arrFMRRecords: Object FMR Information
        * @return array
        */
        public function fnGetFMRIds($arrFMRRecords)
        {
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
         * To return FMR information
         *
         * @param $objFMRNidInfo: Object of FMR result
         * @param $strMediaType : FMR Media TYpe
         * @param $intCampaignId: Campaign Id
         *
         * @return array
         */
        public function fnAnalyticFMRInfo($objFMRNidInfo, $strMediaType, $intCampaignId)
        {
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

            return array($arrFMRInfo, $arrFMRIds);
        }

        /**
         * Class destructor
         */
        public function __destruct()
        {

        }
    }