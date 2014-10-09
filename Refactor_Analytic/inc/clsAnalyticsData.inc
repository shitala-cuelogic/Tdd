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
     * Function return FMRs those have video attached with it for  Benchmark.
     *
     * @return array
     */
    public function fnGetBenchmarkFMRWithVideo()
    {
        //Get start and End date of Previous month
        $arrStartEndDate = $this->fnGetStartEndDate(2);
        $strGetStartDate = $arrStartEndDate["start_date"];
        $strGetEndDate = $arrStartEndDate["end_date"];

        //Collect all FMRs With Videos
        $strFMRWithVideos = "SELECT DISTINCT(fmrv.entity_id) AS fmrIds
                             FROM field_data_field_video AS fmrv
                             JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = fmrv.entity_id
                             WHERE fmrv.bundle = 'fmr'
                             AND (DATE_FORMAT(pubdt.field_fmr_date_time_value,'%Y-%m-%d') >='".$strGetStartDate."'
                             AND DATE_FORMAT(pubdt.field_fmr_date_time_value,'%Y-%m-%d') <= '".$strGetEndDate."')";

        $arrFMRRecords = db_query($strFMRWithVideos)->fetchAll();
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