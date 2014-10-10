<?php
//This is class for Analytics functions.

class clsAnalyticsData
{
    /**
     * Function collect all verticals category attached with requested FMRs
     *
     * @param string $strNid : FMRs node ids with comma separated
     *
     * @return array
     */
    public function fnFMRVerticals($strNid)
    {
        $arrVerticals = array();
        if ($strNid != "") {
            $strSQL = " SELECT vert.entity_id AS fmrId, ver.name AS Verticals
             FROM field_data_field_dist_vertical AS vert
             LEFT JOIN taxonomy_term_data AS ver ON ver.tid = vert.field_dist_vertical_target_id
             WHERE vert.entity_id IN (" . $strNid . ")";
            $objVerticals = db_query($strSQL)->fetchAll();
            if (count($objVerticals) > 0) {
                foreach ($objVerticals as $row) {
                    $arrVerticals[$row->fmrId][] = $row->Verticals;
                }
            }
        }
        return $arrVerticals;
    }
    
    /**
     * Function to get Primary Category Name
     *
     * @param int $intPrimaryCategoryId : Primary category id
     *
     * @return array
     */
    public function fnPrimaryCategoryName($intPrimaryCategoryId)
    {
        $strPrimaryCategoryName = "";
        if ($intPrimaryCategoryId >0) {
            $strSQL = "SELECT name
                       FROM taxonomy_term_data
                       WHERE tid = $intPrimaryCategoryId";
            $arrPrimaryCategory = db_query($strSQL)->fetchAssoc();
            $strPrimaryCategoryName = stripslashes($arrPrimaryCategory["name"]);
        }
        return $strPrimaryCategoryName;
    }
}