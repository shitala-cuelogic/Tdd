<?php
class ClsWidgets
{

    public function __construct()
    {

    }
	
	/**
	 * Function to get all widget affiliates
	 *
	 * @return mixed
	 */
	function fnGetWidgetList()
	{
	    $arrWidgetList = db_query("SELECT id FROM eck_affiliates WHERE type = 'widget'")->fetchAll();
	    return $arrWidgetList;
	}

    public function fnFilterByPrimarySecondaryCategories($arrWhere)
    {
        $sqlFrom = '';
        $sqlWhere = '';
        if ($arrWhere['allverticalcategory'] == 0) {
            $strPrimaryWhere = '';
            if ($arrWhere['primaryFlag'] != 1) {
                $sqlFrom .= " JOIN field_data_field_fmr_primary_category prmCat on prmCat.entity_id = n1.nid ";
                $strPrimaryIds = (@is_array($arrWhere['primaryCategories'])) ? implode(',', $arrWhere['primaryCategories']) : '';
            }

            #SECONDARY CATEGORIES
            $strSecondaryWhere = '';
            if ($arrWhere['secondaryFlag'] != 1) {
                $sqlFrom .= " LEFT JOIN field_data_field_fmr_taxonomy secCat on secCat.entity_id = n1.nid ";
                $strSecondaryIds = (@is_array($arrWhere['secondaryCategories'])) ? implode(',', $arrWhere['secondaryCategories']) : '';
            }

            if ($strSecondaryIds != '' && $strPrimaryIds != '') {
                $strPrimaryWhere .= " prmCat.field_fmr_primary_category_target_id IN (" . $strPrimaryIds . "," . $strSecondaryIds . ") ";
                $strSecondaryWhere .= " secCat.field_fmr_taxonomy_target_id IN (" . $strPrimaryIds . "," . $strSecondaryIds . ") ";
            } elseif ($strPrimaryIds != '') {
                $strPrimaryWhere .= " prmCat.field_fmr_primary_category_target_id IN (" . $strPrimaryIds . ") ";
            } elseif ($strSecondaryIds != '') {
                $strSecondaryWhere .= " secCat.field_fmr_taxonomy_target_id IN (" . $strSecondaryIds . ") ";
            }

            if ($strPrimaryWhere != "" || $strSecondaryWhere != "") {
                $sqlWhere .= " AND ( ";
                if ($strPrimaryWhere != "" && $strSecondaryWhere != "") {
                    $sqlWhere .= " (" . $strPrimaryWhere . ") OR (" . $strSecondaryWhere . ") ";
                } else {
                    $sqlWhere .= ($strPrimaryWhere != "") ? $strPrimaryWhere : $strSecondaryWhere;
                }
                $sqlWhere .= " )";
            }
        }

        return array($sqlFrom, $sqlWhere);

    }

    public function fnFetchFmr($arrWhere)
    {
        $sqlFrom = '';
        if ($arrWhere['client_ogid'] != "" && $arrWhere["boolallclients"] == 0) {

            // Get flag to check whether selected clients want to exclude or include
            $strCompanyExcludeInclude = $arrWhere['company_exclude_include'];

            if ($strCompanyExcludeInclude == "include" || $strCompanyExcludeInclude == "") {

                $strCompanyCondition = " AND group_audience_gid IN (" . $arrWhere['client_ogid'] . ")";

            } else if ($strCompanyExcludeInclude == "exclude") {
                $strCompanyCondition = " AND group_audience_gid NOT IN (" . $arrWhere['client_ogid'] . ")";
            }

            $sqlFrom .= " JOIN field_data_group_audience comp ON comp.entity_id = n1.nid $strCompanyCondition";
        }

        return $sqlFrom;
    }

    public function fnFetchMediaType($arrWhere, $arrMediaType)
    {
        $sqlWhere = '';
        if (isset($arrWhere['media_type']) && is_array($arrWhere['media_type'])) {
            $strMedia = (is_array($arrWhere['media_type'])) ? "'" . implode("','", $arrWhere['media_type']) . "'" : '';
            $sqlWhere .= " AND t.field_fmr_type_of_content_value IN (" . $strMedia . ") ";
        } else {
            $sqlWhere .= " AND t.field_fmr_type_of_content_value IN (" . $arrMediaType . ")";
        }

        return $sqlWhere;
    }

    public function fnFilterFmrByLanguage($arrWhere)
    {
        $sqlFrom = '';
        $sqlWhere = '';
        if ($arrWhere['widget_fMR_language'] == "english_only") {

            $sqlFrom .= " JOIN  field_data_field_fmr_non_english ne ON ne.entity_id = n1.nid";
            $sqlWhere .= " AND (ne.field_fmr_non_english_value <> 1 OR ne.field_fmr_non_english_value IS NULL)";

        } else if ($arrWhere['widget_fMR_language'] == "non_english") {

            $sqlFrom .= " JOIN  field_data_field_fmr_non_english ne ON ne.entity_id = n1.nid";
            $sqlWhere .= " AND ne.field_fmr_non_english_value = 1";
        }

        return array($sqlFrom, $sqlWhere);
    }

    function fetchWidgetListPageInformation($arrWhere, $intLimit, $intPageNo) {

        //Include all required files
        module_load_include('inc', 'threebl_analytics_reports', 'inc/clsWidgets');

        //Object of class
        $objWidget = new ClsWidgets();

        $intStart = abs($intLimit * ($intPageNo - 1));
        $arrMediaType = "'press_release','blog','newsletter','article','multimedia'";
        $sqlSelect = " SELECT sql_calc_found_rows distinct n1.nid,
	                               n1.title, t.field_fmr_type_of_content_value,
	                               d.field_fmr_date_time_value as date,
	                               SUBSTR(bdy.field_fmr_body_value, 1, 1000) as description";
        $sqlFrom = " FROM node n1
	                             JOIN field_data_field_fmr_type_of_content t ON t.entity_id = n1.nid
	                             JOIN field_data_field_fmr_date_time d ON d.entity_id = n1.nid
								 JOIN field_data_field_fmr_body bdy ON bdy.entity_id = n1.nid ";
        $sqlWhere = " WHERE n1.type = 'fmr' AND n1.status = 1 AND n1.nid NOT IN (44620, 44603)";
        $sqlOrderBy = " ORDER BY d.field_fmr_date_time_value DESC ";
        $sqlLimit = " LIMIT " . $intStart . ", " . $intLimit;

        ## ADD FILTER AS PER SELECTION
        #VERTICAL
        #PRIMARY CATEGORIES

        list($sqlCatFrom, $sqlCatWhere) = $this->fnFilterByPrimarySecondaryCategories($arrWhere);

        $sqlFrom .= $sqlCatFrom;
        $sqlWhere .= $sqlCatWhere;

        // Condition to fetch FMR of selected clients
        $sqlFrom .= $this->fnFetchFmr($arrWhere);

        # MEDIA TYPE
        $sqlWhere .= $this->fnFetchMediaType($arrWhere, $arrMediaType);

        //Filter FMRs by Language option (all, english only and non-english)
        list($sqlFilterFmrLangFrom, $sqlFilterFmrLangWhere) = $this->fnFilterFmrByLanguage($arrWhere);

        $sqlFrom .= $sqlFilterFmrLangFrom;
        $sqlWhere .= $sqlFilterFmrLangWhere;

        #Build final SQL query
        $sqlQuery = $sqlSelect . " " . $sqlFrom . " " . $sqlWhere . " " . $sqlOrderBy . " " . $sqlLimit;
        $arrContentList = db_query($sqlQuery)->fetchAll();
        //TOTAL RECORDS
        list($arrTotalRecords) = db_query('SELECT FOUND_ROWS() AS intTotalRecords')->fetchAll();
        $intTotalRecords = $arrTotalRecords->intTotalRecords;

        ## To get FMR Information
        $arrFMRContentList = $objWidget->fnGetFMRInformation($arrContentList);
        
        return array($arrContentList, $intTotalRecords, $arrFMRContentList);
    }

    /**
	 * To get widget Id
	 *
	 * @param int $strTokenId  : Widget Token Id
	 *
	 * @return int
	 */
    function fnGetWidgetId($strTokenId)
    {
    	$strWidgetIdSql = "select entity_id
                           from field_data_field_token_id where field_token_id_value = '" . $strTokenId . "' ";
        $intWidgetId = db_query($strWidgetIdSql)->fetchField();

        return $intWidgetId;
    }

    /**
	 *  Function return url for related node
	 *
	 * @param $intNodeId : is the node id from 3BL
	 *
	 * @return string
	 */
	function getCompanyURL($intNodeId)
	{
	    $result = db_select('url_alias', 'n')
	        ->fields('n')
	        ->condition('source', 'node/' . $intNodeId, '=')
	        ->execute()
	        ->fetchAssoc();
	    return trim($result['alias']);
	}
}

?>