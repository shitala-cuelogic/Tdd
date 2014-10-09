<?php
//This is class for Analytics functions.

class clsAnalytics extends clsBaseClass
{
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

    public function fnAddViewsProcess($FMRids, $strFMRType, $strChannel, $strJustDate) {

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

    public function fnGetCompanyOgIds($FMRid) {

        $strMedia = "select ogm.etid, ogm.gid companyOgId
                     from og_membership ogm
                     where ogm.etid = $FMRid";

        $arrMedia = db_query($strMedia)->fetchAssoc();

        return $arrMedia;
    }

    public function fnSetMediaType($strFMRType, $intFMRNid, $intCompanyOgId) {

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
     * Class destructor
     */
    public function __destruct()
    {

    }
}
// Object of class
$objAnalytics = new clsAnalytics();