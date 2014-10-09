<?php
//This is class for Analytics functions.

class clsAnalytics extends clsBaseClass
{
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
     * Class destructor
     */
    public function __destruct()
    {

    }
}
// Object of class
$objAnalytics = new clsAnalytics();