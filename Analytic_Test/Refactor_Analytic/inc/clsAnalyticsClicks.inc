<?php
//This is class for Analytics functions.
    module_load_include('inc', 'refactor_Analytic', 'inc/clsBaseClass');

class clsAnalyticsClicks extends clsBaseClass
{
    public function __construct()
    {
        parent::__construct();
    }
        
    /**
     * Get Top five clicks count and their FMR information
     * @param int $intCompanyOgId : Company OG Id
     *
     * @return array
     */
    public function fnGetTopFiveClicksFMRs($intCompanyOgId)
    {
        //Include all required files
        module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

        //Object of class
        $objAnalytics = new clsAnalytic();

        $strQry = "SELECT n.nid, t.field_fmr_type_of_content_value AS fmr_type, td.name AS primarycategory, COUNT(C.nid) AS clickcount, n.title, pubdt.field_fmr_date_time_value
                   FROM node AS n
                   JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                   JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                   JOIN field_data_field_fmr_type_of_content AS t ON n.nid = t.entity_id
                   LEFT JOIN field_data_field_fmr_primary_category AS prim ON n.nid = prim.entity_id
                   LEFT JOIN taxonomy_term_data AS td ON td.tid = prim.field_fmr_primary_category_target_id
                   JOIN field_data_field_dist_archive AS ar ON n.nid = ar.entity_id
                   JOIN  threebl_analytic_clicks  AS C ON C.nid = n.nid
                   WHERE n.status = 1 AND ga.group_audience_gid = $intCompanyOgId
                   AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')
                   AND pubdt.field_fmr_date_time_value >= '$this->strChartPrevDate' AND pubdt.field_fmr_date_time_value <= '$this->strCurrentDate'
                   AND ar.field_dist_archive_value = 0
                   GROUP BY C.nid
                   ORDER BY clickcount DESC
                   LIMIT 0,5";
        #Final SQL
        $objFMRNidInfo = db_query($strQry)->fetchAll();

        //Get Top Clicks FMR information
        $arrTopClicksFMRInfo = $objAnalytics->fnGetTopClicksInformation($objFMRNidInfo);

        return $arrTopClicksFMRInfo;
    }

    /**
     * Function fetch FMR Clicks information
     *
     * @param string $strFMRIds : Comma separated FMR Ids
     *
     * @return array
     */
    public function fnGetClicks($strFMRIds)
    {
        //Include all required files
        module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

        //Object of class
        $objAnalytics = new clsAnalytic();

        $strSQL = " SELECT COUNT(C.nid) AS clickcount, C.nid
                    FROM  threebl_analytic_clicks  AS C WHERE C.nid IN ($strFMRIds) GROUP BY C.nid";
        $objFMRClickInfo = db_query($strSQL)->fetchAll();

        //To get Clicks count information
        $arrFMRInfo = $objAnalytics->fnViewClickCountInfo($objFMRClickInfo, "click");
        return $arrFMRInfo;
    }

    /**
     * Query for getting Click Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId               : Company OG Id
     * @param string $strFMRIds                    : Comma separated FMR Ids
     * @param int    $intCampaignId                : Campaign Id
     *
     * @return array
     */
    public function fnGetClickChartInfo($intCompanyOgId, $strFMRIds = "", $intCampaignId = 0)
    {
        //Include all required files
        module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');

        //Object of class
        $objAnalytics = new clsAnalytic();

        $strCampaignFrom = '';
        if ((int)$intCampaignId > 0) {
            $strCampaignFrom = " JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = C.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        }

        $strSQL = "SELECT DATE_FORMAT(C.click_date, '%Y-%m-%d') as clickdate, count(C.nid) as totalcount
                  FROM  threebl_analytic_clicks  as C
                  $strCampaignFrom
                  WHERE C.company_ogid = $intCompanyOgId
                  AND (DATE_FORMAT(C.click_date, '%Y-%m-%d') BETWEEN '".$this->strChartPrevDate."' AND '".$this->strCurrentDate."')
                  AND C.nid IN ($strFMRIds)
                  GROUP BY  DATE_FORMAT(C.click_date, '%Y/%m/%d')
                  ORDER by C.click_date asc";

        $arrayClickChartData = db_query($strSQL)->fetchAll();

        //To get Clicks chart information
        $arrClickChart = $objAnalytics->fnViewClickChartInfo($arrayClickChartData, "click");
        
        return $arrClickChart;
    }
}