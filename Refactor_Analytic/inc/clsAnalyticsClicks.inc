<?php
//This is class for Analytics functions.

class clsAnalyticsClicks
{
    /**
     * Query for getting Click Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId               : Company OG id
     * @param string $strFMRIds                    : FMR node id's with comma separated
     *
     * @return mixed
     */
    public function fnGetAnalyticClickChartInfo($intCompanyOgId, $strFMRIds = "")
    {
        $strSQL = "SELECT DATE_FORMAT(C.click_date, '%Y-%m-%d') as clickdate, count(C.nid) as totalcount
                  FROM " . $this->strClickTableName . " as C
                  WHERE C.company_ogid = $intCompanyOgId
                  AND (DATE_FORMAT(C.click_date BETWEEN, '%Y-%m-%d') '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "')
                  AND C.nid IN ($strFMRIds)
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
    }

    /**
     * Query for Google Map for Click Count for Second Level and Third Level
     *
     * @param int   $intCompanyOgId   :  Company OG id
     * @param mixed $strMediaTypeORId : value will be either media type or id
     *
     * @return mixed
     */
    public function fnGetFMRClicksByCountryGoogleMap($intCompanyOgId, $strMediaTypeORId)
    {
        // To get Media type condition
        $strMediaCondition = $this->fnMediaTypeCondition($strMediaTypeORId);

        $strCondition = " $strMediaCondition  GROUP BY UPPER(country) order by ipcount desc ";

        $strFinalSql = "SELECT nid,click_ip,count(nid) as ipcount,country
									  FROM " . $this->strClickTableName . " as C
									  JOIN field_data_field_fmr_type_of_content t ON  C.nid = t.entity_id
									  WHERE C.company_ogid = $intCompanyOgId
									  $strCondition";

        return $arrayClickMapData = db_query($strFinalSql)->fetchAll();
    }

    /**
    * To return Media type condition
    * @param mixed $strMediaTypeORId:  value will be either media type or id
    * 
    * * @return string
    */
    public function fnMediaTypeCondition($strMediaTypeORId) 
    {
        $strMediaCondition = '';
        if ((int)$strMediaTypeORId == 0 && $strMediaTypeORId != '') {
            //getting particular media-type condition for clicks.
            if ($strMediaTypeORId == "all") {
                $strMediaCondition .= " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter') ";
            } else {
                $strMediaCondition .= " AND C.fmr_type ='" . $strMediaTypeORId . "'";
            }
        } elseif ((int)$strMediaTypeORId > 0) {
            $strMediaCondition .= " AND C.nid = $strMediaTypeORId ";
        }
        return $strMediaCondition;
    }

    /**
     * Query for Referral website for Second Level and Third Level
     *
     * @param int   $intCompanyOgId    :  Company OG id
     * @param mixed $strMediaTypeORId  : value will be either media type or id
     *
     * @return mixed
     */
    public function fnGetFMRReferrerClicksByMediaType($intCompanyOgId, $strMediaTypeORId)
    {
        // To get Media type condition
        $strMediaCondition = $this->fnMediaTypeCondition($strMediaTypeORId);
        
        if ((int)$strMediaTypeORId > 0) {
            $arrExtraLink = array('bestgenericviagra.biz', 'blog.pnarp.com', 'diffbot.com', 'timberland.justmeans.com', 'www.mysmutsearch.com', 'lala123.com', '_bit=528e713d-', '_bit=52af68a5-00111-07550', 'piwik.3blmedia.com', 'stats.3blmedia.com');
            $strExtraLink = implode("|", $arrExtraLink);
            $strMediaCondition = " $strMediaCondition AND click_referrer NOT RLIKE '$strExtraLink' ";
        }

        $strCondition = " $strMediaCondition  GROUP BY click_referrer Order by click_date desc ";

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