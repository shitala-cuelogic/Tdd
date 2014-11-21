<?php
/**
 * Class contain all functionality regarding Justmeans Views and Clicks
 * Date: 10/10/14
 * Time: 2:12 PM
 */

class JmClicksViews {
    /**
     * Function to get the Justmeans newsletter total clicks for selected FMR
     *
     * @param int    $intNodeId     : Node Id
     * @param string $strChannelType: Channel Type (Jm-Newsletter, Justmeans)
     * @param string $strReportType : Report type (Weekly or Individual).
     *
     * @return mixed
     */
    public function fnGetJMClicks($intNodeId, $strChannelType, $strReportType = "")
    {
        // Group By Condition
        $strGroupBy = "";
        if ($strReportType == "weekly") {
            $strGroupBy = "GROUP BY N.nid_3bl";
        }
        $strPrimarySQL = "SELECT count(jc.mediaid) as click, N.nid_3bl
                          FROM  nid_to_nid as N
                          JOIN jm_analytic_clicks as jc on N.nid_jm = jc.mediaid
                          WHERE N.nid_3bl IN (" . $intNodeId . ")
                          AND jc.channel = '$strChannelType' $strGroupBy";
        $objJMClicks = db_query($strPrimarySQL)->fetchAll();

        $arrJMClicksInfo = array();

        //Weekly Justmeans FMRs clicks
        if ($strReportType == "weekly") {
            if (!empty($objJMClicks)) {
                foreach ($objJMClicks as $arrJMClicks) {
                    $arrJMClicksInfo[$arrJMClicks->nid_3bl]["JMClicks"] = $arrJMClicks->click;
                }
            }
            return $arrJMClicksInfo;
        } else {
            //Individual Justmeans FMR clicks
            return $objJMClicks[0]->click;
        }
    }

    /**
     * Function to get the Justmeans newsletter total views for selected FMR
     *
     * @param int    $intNodeId     : Node Id
     * @param string $strChannelType: Channel Type (Jm-Newsletter, Justmeans)
     * @param string $strReportType : Report type (Weekly or Individual).
     *
     * @return mixed
     */
    public function fnGetJMViews($intNodeId, $strChannelType, $strReportType = "")
    {
        // Group By Condition
        $strGroupBy = "";
        if ($strReportType == "weekly") {
            $strGroupBy = "GROUP BY N.nid_3bl";
        }
        $strPrimarySQL = "SELECT sum(jv.totalcount) as views, N.nid_3bl
                          FROM  nid_to_nid as N
                          JOIN jm_analytic_views as jv on N.nid_jm = jv.mediaid
                          WHERE N.nid_3bl IN(" . $intNodeId . ")
                          AND jv.channel = '$strChannelType' $strGroupBy";
        $objJMViews = db_query($strPrimarySQL)->fetchAll();

        $arrJMViewsInfo = array();
        //Weekly Justmeans FMRs views
        if ($strReportType == "weekly") {
            if (!empty($objJMViews)) {
                foreach ($objJMViews as $arrJMViews) {
                    $arrJMViewsInfo[$arrJMViews->nid_3bl]["JMViews"] = $arrJMViews->views;
                }
            }
            return $arrJMViewsInfo;
        } else {
            //Individual Justmeans FMR views
            return $objJMViews[0]->views;
        }
    }
} 