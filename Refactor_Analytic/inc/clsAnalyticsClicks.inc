<?php
//This is class for Analytics functions.

class clsAnalyticsClicks
{
    /**
     * Query for getting Click Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId               : Company OG id
     * @param string $strFMRIds                    : FMR node id's with comma separated
     * @param int    $intClickChartDateBetweenFlag : boolean flag which indicate functionality for chart data.
     * @param int    $intCampaignId                : Campaign node id
     *
     * @return mixed
     */
    public function fnGetAnalyticClickChartInfo($intCompanyOgId, $strFMRIds = "", $intClickChartDateBetweenFlag = 0, $intCampaignId = 0)
    {
        $strClickChartDateBetween = '';
        if ((int)$intClickChartDateBetweenFlag == 1) {
            $strClickChartDateBetween = " AND (C.click_date BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "') ";
        }

        $strCampaignFrom = '';
        if ((int)$intCampaignId > 0) {
            $strCampaignFrom = " JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = C.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        }

        $strCondition = "";
        if ($strFMRIds != "") {
            $strCondition = " AND C.nid IN ($strFMRIds)";
        }

        $strSQL = "SELECT DATE_FORMAT(C.click_date, '%Y-%m-%d') as clickdate, count(C.nid) as totalcount
                  FROM " . $this->strClickTableName . " as C
                  $strCampaignFrom
                  WHERE C.company_ogid = $intCompanyOgId
                  $strClickChartDateBetween
                  $strCondition
                  GROUP BY  DATE_FORMAT(C.click_date, '%Y/%m/%d')
                  ORDER by C.click_date asc";

        $arrayClickChartData = db_query($strSQL)->fetchAll();
        $arrClickChart = array();

        //Checking empty array condition
        if (!empty($arrayClickChartData)) {
            foreach ($arrayClickChartData as $arrVal) {
                $arrClickChart[$arrVal->clickdate] = $arrVal->totalcount;
            }
        }
        return $arrClickChart;
        ## For test put 2013-01-18 .prev '".$this->strCurrentDate."'
    }

    /**
     * Query for Google Map for Click Count for Second Level and Third Level
     *
     * @param int   $intCompanyOgId   :  Company OG id
     * @param mixed $strMediaTypeORId : value will be either media type or campaign id
     *
     * @return mixed
     */
    public function fnGetFMRClicksByCountryGoogleMap($intCompanyOgId, $strMediaTypeORId)
    {
        $strCondition = '';
        $strCndClk = '';
        if ((int)$strMediaTypeORId == 0 && $strMediaTypeORId != '') {
            //getting particular media-type condition for clicks.
            if ($strMediaTypeORId == "all") {
                $strCndClk .= " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter') ";
            } else {
                $strCndClk .= " AND C.fmr_type ='" . $strMediaTypeORId . "'";
            }
        } elseif ((int)$strMediaTypeORId > 0) {
            $strCndClk .= " AND C.nid = $strMediaTypeORId ";
        }
        $strCondition .= " $strCndClk  GROUP BY UPPER(country) order by ipcount desc ";

        $strFinalSql = "SELECT nid,click_ip,count(nid) as ipcount,country
									  FROM " . $this->strClickTableName . " as C
									  JOIN field_data_field_fmr_type_of_content t ON  C.nid = t.entity_id
									  WHERE C.company_ogid = $intCompanyOgId
									  $strCondition";

        return $arrayClickMapData = db_query($strFinalSql)->fetchAll();
    }

    /**
     * Query for Referral website for Second Level and Third Level
     *
     * @param int   $intCompanyOgId    :  Company OG id
     * @param mixed $strMediaTypeORId  : value will be either media type or campaign id
     *
     * @return mixed
     */
    public function fnGetFMRReferrerClicksByMediaType($intCompanyOgId, $strMediaTypeORId)
    {
        $strCondition = '';
        $strCndClk = '';
        if ((int)$strMediaTypeORId == 0 && $strMediaTypeORId != '') {
            //getting particular media-type condition for clicks.
            if ($strMediaTypeORId == "all") {
                $strCndClk .= " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter') ";
            } else {
                $strCndClk .= " AND C.fmr_type ='" . $strMediaTypeORId . "' ";
            }
        } elseif ((int)$strMediaTypeORId > 0) {
            $arrExtraLink = array('bestgenericviagra.biz', 'blog.pnarp.com', 'diffbot.com', 'timberland.justmeans.com', 'www.mysmutsearch.com', 'lala123.com', '_bit=528e713d-', '_bit=52af68a5-00111-07550', 'piwik.3blmedia.com', 'stats.3blmedia.com');
            $strExtraLink = implode("|", $arrExtraLink);
            $strCndClk .= " AND C.nid = $strMediaTypeORId AND click_referrer NOT RLIKE '$strExtraLink' ";
        }

        $strCondition .= " $strCndClk  GROUP BY click_referrer Order by click_date desc ";

        $strFinalQuery = "SELECT distinct click_referrer as rlink
								   FROM " . $this->strClickTableName . " as C
								   JOIN field_data_field_fmr_type_of_content t ON  C.nid = t.entity_id
								   WHERE C.company_ogid = $intCompanyOgId
								   AND C.click_referrer <> '' $strCondition";
        return $arrayClickReferData = db_query($strFinalQuery)->fetchAll();
    }

    /**
     * Getting Click-Count By Media-Id
     *
     * @param int  $intCompanyOgId :  Company gid
     * @param int  $intMediaId     : FMR node id
     *
     * @return mixed
     */
    public function fnGetFMRClickCountByMediaId($intCompanyOgId, $intMediaId)
    {
        $strClickCount = "SELECT C.click_date, count(C.nid) as totalcount,fmr_type as type
								  FROM " . $this->strClickTableName . " as C
								  Where C.company_ogid = $intCompanyOgId AND C.nid = $intMediaId
								  GROUP BY nid";
        return $arrClickCount = db_query($strClickCount)->fetchAssoc();
    }

    /**
     * Function Update View table for Company id
     *
     * @param int  $intCompOgId : Company OG id
     * @param int  $intSynId    : Company JM id
     */
    public function fnUpdateClicksCompany($intCompOgId = 0, $intSynId = 0)
    {
        $strUpdateQuery = "Update " . $this->strClickTableName . " SET company_ogid =" . $intCompOgId . " Where company_ogid =" . $intSynId . " And click_date >= DATE_SUB( '2013-07-10' , INTERVAL 180 DAY ) ";
        db_query($strUpdateQuery)->execute();
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

    /**
     * Get Top five clicks count and their FMR information
     * @param int $intCompanyOgId : Company OG Id
     *
     * @return array
     */
    public function fnGetTopFiveClicksFMRs($intCompanyOgId)
    {
        $strQry = "SELECT n.nid, t.field_fmr_type_of_content_value AS fmr_type, td.name AS primarycategory, COUNT(C.nid) AS clickcount, n.title, pubdt.field_fmr_date_time_value
                   FROM node AS n
                   JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                   JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                   JOIN field_data_field_fmr_type_of_content AS t ON n.nid = t.entity_id
                   LEFT JOIN field_data_field_fmr_primary_category AS prim ON n.nid = prim.entity_id
                   LEFT JOIN taxonomy_term_data AS td ON td.tid = prim.field_fmr_primary_category_target_id
                   JOIN field_data_field_dist_archive AS ar ON n.nid = ar.entity_id
                   JOIN $this->strClickTableName AS C ON C.nid = n.nid
                   WHERE n.status = 1 AND ga.group_audience_gid = $intCompanyOgId
                   AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')
                   AND pubdt.field_fmr_date_time_value >= '$this->strChartPrevDate' AND pubdt.field_fmr_date_time_value <= '$this->strCurrentDate'
                   AND ar.field_dist_archive_value = 0
                   GROUP BY C.nid
                   ORDER BY clickcount DESC
                   LIMIT 0,5";
        #Final SQL
        $objFMRNidInfo = db_query($strQry)->fetchAll();

        $arrTopClicksFMRInfo = array();

        if (!empty($objFMRNidInfo)) {
            foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['nid'] = $arrFMRNidInfo->nid;
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['title'] = $arrFMRNidInfo->title;
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['publishdate'] = $arrFMRNidInfo->field_fmr_date_time_value;
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['media'] = $this->arrShowMediaType[$arrFMRNidInfo->fmr_type];
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['primary_category'] = $arrFMRNidInfo->primarycategory;
                $arrTopClicksFMRInfo[$arrFMRNidInfo->nid]['clickcount'] = $arrFMRNidInfo->clickcount;
            }
        }
        //Implode FMR ids
        return $arrTopClicksFMRInfo;
    }
}