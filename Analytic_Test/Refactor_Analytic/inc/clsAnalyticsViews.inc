<?php

module_load_include('inc', 'refactor_Analytic', 'inc/clsBaseClass');
//This is class for Analytics functions.

class clsAnalyticsViews extends clsBaseClass
{
    public function __construct()
    {
        parent::__construct();
    }
    
    /**
     * Function fetch FMR views information
     *
     * @param string $strFMRIds : Comma separated FMR Ids
     *
     * @return array
     */
    public function fnGetViews($strFMRIds)
    {
        //Include all required files
        module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

        //Object of class
        $objAnalytics = new clsAnalytic();

        $strSQL = " SELECT SUM(v.totalcount) AS viewcount, v.nid
                    FROM  threebl_analytic_views  AS v WHERE v.nid IN ($strFMRIds) GROUP BY v.nid";
        $objFMRViewInfo = db_query($strSQL)->fetchAll();

        //To get view count information
        $arrFMRInfo = $objAnalytics->fnViewClickCountInfo($objFMRViewInfo, "view");
        return $arrFMRInfo;
    }

    /**
     * Query for getting View Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId          : Company OG Id
     * @param string $strFMRIds               : Comma separated FMR Ids
     * @param int    $intCampaignId           : Campaign Id
     *
     * @return array
     */
    public function fnGetViewChartInfo($intCompanyOgId, $strFMRIds = "", $intCampaignId = 0)
    {
        //Include all required files
        module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

        //Object of class
        $objAnalytics = new clsAnalytic();

        $strCampaignFrom = ''; //change
        if ((int)$intCampaignId != 0) {
            $strCampaignFrom = "  JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = v.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        }

        $strSQL = "SELECT SUM(v.totalcount) as totalcount, v.datevisited AS datevisit
                  FROM  threebl_analytic_views  AS v
                  $strCampaignFrom
                  WHERE v.company_ogid = $intCompanyOgId
                  AND (v.datevisited BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "')
                  AND v.nid IN ($strFMRIds)
                  GROUP BY datevisited
                  ORDER by datevisited ASC";

        $arrViewChartData = db_query($strSQL)->fetchAll();
        $arrViewChart = array();

        //To get Clicks chart information
        $arrViewChart = $objAnalytics->fnViewClickChartInfo($arrViewChartData, "view");
        return $arrViewChart;
    }
}