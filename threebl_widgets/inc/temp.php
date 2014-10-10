<?php

/**
 * Created by PhpStorm.
 * User: shubhodeep
 * Date: 10/10/14
 * Time: 11:32 AM
 */

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


//--------------------------------------------------------------------------------------------------------

function fnGetNewsArray($boolAddView, $strPage, $limit = 5, $arrWhere = array(), $intNewsId = 0, $intWidgetId = 0, $intPageNo = 1)
{
    $arrNodeIds = array();
    $arrContentList = array();

    if ($intNewsId > 0) { //This block contain information for the widget detail page

        $strPage = (isset($_GET['p'])) ? base64_decode($_GET['p']) : 'more';

        $arrContentList = fetchFMRDetailInformation();

        #Collect all node ids
        $arrNodeIds[] = $intNewsId;
    }

    else {

        list ($arrContentList, $intTotalRecords, $arrFMRContentList) = fetchWidgetListPageInformation($arrWhere, $limit, $intPageNo);
        //For store only views not clicks
        $intWidgetId = 0;

    }

    ## Add Views & Clicks for FMR
    $arrCompany = array();
    $arrCompany = fnAddFMRAnalyticsViews($arrNodeIds, $boolAddView, $intWidgetId, $strPage);

    if (is_array($arrCompany) && count($arrCompany) > 0) {
        $strIds = implode(",", $arrCompany);
        $arrCompanies = db_query("SELECT * FROM node WHERE nid IN ( " . $strIds . ")")->fetchAllKeyed(0, 4);
        foreach ($arrCompany as $key => $val) {
            $arrCompanyName[$key]['cname'] = $arrCompanies[$val];
            $arrCompanyName[$key]['cnid'] = array_search($arrCompanies[$val], $arrCompanies);
        }
    }

    return array($arrContentList, $arrCompany, $arrCompanyName, $intTotalRecords, $arrFMRContentList);
}

function fetchFMRDetailInformation(){

    //Include all required files
    module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReports');

    //Object of class
    $objReports = new ClsReports();

    // $token = (isset($_GET['t']))?base64_decode($_GET['t']):0;
    $intNewsId = (isset($_GET['md'])) ? ($_GET['md']) : '0';
    $node = node_load($intNewsId);
    $strMediaType = $node->field_fmr_type_of_content['und'][0]['value'];

    $objArrResource = $node->field_fmr_resource_links[$node->language]; // Resource link array
    $strResourceText = $node->field_fmr_news_facts_text[$node->language][0]['value']; // Resource Text
    $strContactName = $node->field_fmr_contact_name[$node->language][0]['value']; // Resource Contact Name
    $objArrContactPhone = $node->field_fmr_contact_phone[$node->language][0]; // Resource Contact Phone array
    $strContactEmail = $node->field_fmr_contact_mail[$node->language][0]['email']; // Resource Contact email

    if (!empty($node->field_fmr_contact_organization[LANGUAGE_NONE])) {
        $strContactOrganization = $node->field_fmr_contact_organization[$node->language][0]['value']; // Resource Contact organization
    }

    $objArrContactOther = $node->field_fmr_contact_other[$node->language]; // multiple other details array
    $arrBlogLink = $node->field_fmr_blog_newsletter_url[$node->language][0]; //Blog Url

    $arrContentList = array(
        'title' => $objReports->fnRemoveWordFormatting($node->title, "widget"),
        'mediatype' => $strMediaType,
        'date' => date("F d, Y", strtotime($node->field_fmr_date_time[$node->language][0]['value'])),
        'description' => $objReports->fnRemoveWordFormatting($node->field_fmr_body[$node->language][0]['value'], "widget"),
        'resourceLink' => $objArrResource,
        'resourceText' => $strResourceText,
        'contactName' => $strContactName,
        'contactPhone' => $objArrContactPhone,
        'contactEmail' => $strContactEmail,
        'contactOrganization' => $strContactOrganization,
        'contactOther' => $objArrContactOther,
        'blogUrl' => $arrBlogLink
    );
    $arrContentList['playVideo'] = 'none';
    $arrContentList['playAudio'] = 'none';
    $arrContentList['playImage'] = 'none';
    $arrContentList['displayTwitter'] = 'none';

    //for Twitter
    if (is_array($node->field_fmr_tweet['und']) && count($node->field_fmr_tweet['und'])) {
        foreach ($node->field_fmr_tweet['und'] as $k => $twitter) {
            $arrContentList['twitter'][$k]['value'] = addslashes($twitter['value']);
            $arrContentList['twitter'][$k]['format'] = $twitter['format'];
            $arrContentList['twitter'][$k]['safe_value'] = $twitter['safe_value'];
            $arrContentList['displayTwitter'] = 'block';
        }
    }

    // for photos
    if (is_array($node->field_fmr_photo['und']) && count($node->field_fmr_photo['und'])) {

        $arrContentList['photo'] = $this->fnGetPhotoData($node);
        $arrContentList['playImage'] = 'block';

    }

    //for Audio
    if (is_array($node->field_fmr_audio['und']) && count($node->field_fmr_audio['und'])) {

        $arrContentList['audio'] = $this->fnGetAudioData($node);
        $arrContentList['playAudio'] = 'block';
    }

    // If media has multiple or singal videos then we have to process all of them
    if (is_array($node->field_video['und']) && count($node->field_video['und'])) {

        $arrContentList['video'] = $this->fnGetVideoData($node);
        $arrContentList['playVideo'] = 'block';

    }
    return $arrContentList;
}

function fetchWidgetListPageInformation($arrWhere, $intLimit, $intPageNo){

    //Include all required files
    module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReports');

    //Object of class
    $objReports = new ClsReports();

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
    $sqlWhere .= $this->fnFetchMediaType($arrWhere);

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

    ## GET ALL NODE IDS
    if (is_array($arrContentList) && count($arrContentList) > 0) {
        foreach ($arrContentList as $row) {
            $arrNodeIds[] = $row->nid;

            $arrFMRContentList[$row->nid]['title'] = $objReports->fnRemoveWordFormatting($row->title, "widget");
            $arrFMRContentList[$row->nid]['description'] = $objReports->fnRemoveWordFormatting($row->description, "widget");
        }
    }

    return array($arrContentList, $intTotalRecords, $arrFMRContentList);
}