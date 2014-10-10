<?php
//This is class for Analytics functions.

class clsAnalyticsClicks
{
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