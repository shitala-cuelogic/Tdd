<?php
/**
 * Function for widget details
 *
 * @param int        $strTokenId  : Widget Id
 * @param string     $strPage     : calling page
 * @param int        $intNewsId   : FMR nid
 * @param bool       $boolAddView : Flag which indicate views added into the database or not.
 * @param int        $intPageNo   : page number for pagination
 *
 * @return array|string
 */
function fnWidgetNews($strTokenId, $strPage, $intNewsId = 0, $boolAddView = true, $intPageNo = 1)
{
// Report simple running errors
error_reporting(E_ERROR | E_WARNING | E_PARSE);

    global $base_url;
    $arrData = array();
    $arrData['path'] = $base_url;

    //Include all required files
    module_load_include('inc', 'threebl_affiliate', 'inc/clsTracking');

    //Creating the Object of Class
    $objClsTracking = new clsTracking();

    //Get all information about
    if ($strTokenId != '') {
        $strWidgetIdSql = "select entity_id
                           from field_data_field_token_id where field_token_id_value = '" . $strTokenId . "' ";
        $intWidgetId = db_query($strWidgetIdSql)->fetchField();

        $arrEntity = @entity_load('affiliates', array($intWidgetId));
        $arrResult = @json_decode(json_encode($arrEntity[$intWidgetId]), 1);

        //Check widget status
        if ($boolAddView == true && $arrResult['field_affiliate_status']['und'][0]['tid'] != $objClsTracking->AffiliateActive) {
            return "This widget is inactivated from admin.";
            die;
        }

        $arrData['widget_name'] = $arrResult['field_affiliate_title']['und'][0]['value'];
        $arrData['token'] = $strTokenId;
        $arrData['widget_id'] = base64_encode($intWidgetId);

        $limit = ('details' === $strPage) ? $arrResult['field_widget_no_of_posts_on_list']['und'][0]['value'] : $arrResult['field_widget_no_of_posts_on_font']['und'][0]['value'];

        # Open FMR link in iew tab flag
        $arrResult['field_open_fmr_link_in_new_tab']['und'][0]['value'];

        $strFMRInNewTab = "";
        if ($arrResult['field_open_fmr_link_in_new_tab']['und'][0]['value'] == 1) {
            $strFMRInNewTab = 'target="_blank"';
        }

        #Get the Limit for the front page description
        $intDescriptionLimit = $arrResult['field_front_page_fmr_description']['und'][0]['value'];

        #Get the Description of the widget
        $strFrontPageDesc = $arrResult['field_affiliate_description']['und'][0]['value'];

        $arrData['widget_viewmore_link'] = $arrResult['field_widget_view_more_label']['und'][0]['value'];

        // All Clients flag
        $boolAllClients = $arrResult['field_all_clients']['und'][0]['value'];

        // All Vertical Category
        $boolAllVerticalCategory = $arrResult['field_all_vertcal_category']['und'][0]['value'];

        // Get flag to check whether selected clients want to exclude or include
        $strCompanyExcludeInclude = $arrResult['field_clients_to_exclude_include']['und'][0]['value'];

        // Get Widget FMR language flag
        $strWidgetFMRLanguage = $arrResult['field_widget_fmr_language']['und'][0]['value'];

        if ($boolAllClients == 0) {
            // Get selected client Og ids in array
            $arrClientOgId = array();
            $strClientOgIds = "";
            $arrWidgetClientOgIds = $arrResult['group_audience']['und'];
            if (is_array($arrWidgetClientOgIds) && count($arrWidgetClientOgIds) > 0) {
                foreach ($arrWidgetClientOgIds as $arrGetClientOgId) {
                    $arrClientOgId[] = $arrGetClientOgId["gid"];
                }
                // make a comma separated string of client ids
                if (is_array($arrClientOgId) && count($arrClientOgId) > 0) {
                    $strClientOgIds = implode(",", $arrClientOgId);
                }
            }
        }

        //Collect all settings into the array

        $arrSettings = $this->fnSetSettingsArray($arrResult, $intDescriptionLimit, $strFrontPageDesc, $strFMRInNewTab);

        $arrData['settings'] = $arrSettings;

        //CSS management
        if ($strPage == 'more') {
            //Front Page Css Function.
            $strCssFrontPage = fnGetWidgetCss($arrResult, 'FrontPage');
            $arrData['widget_css'] = html_entity_decode($strCssFrontPage); // Front Page Css.

        } elseif ((int)$intNewsId > 0 && $strPage == 'details') {
            // Details Page Css Function.
            // Checking old hidden field exits or not
            $strCssDetailPage = fnGetWidgetCss($arrResult, 'DetailPage');
            $arrData['widget_css'] = html_entity_decode($strCssDetailPage); //Detail Page Css Variable.

        } elseif ($strPage == 'details') {
            // Checking old hidden field exits or not
            // List page Css Function.
            $strCssListPage = fnGetWidgetCss($arrResult, 'ListPage');
            $arrData['widget_css'] = html_entity_decode($strCssListPage); // List Page Css Variable.
        }

        // array of primary category and secondary  category
        $arrMediaType = array();
        $arrVertical = array();
        $arrPrimaryCategories = array();
        $arrSecondaryCategories = array();

        //checking is media type exits or not
        if (is_array($arrResult['field_widget_media_type']['und'])) {
            foreach ($arrResult['field_widget_media_type']['und'] as $key => $value) {
                $arrMediaType[] = $value['value'];
            }
        }

        //checking vertical values array is exist
        if ($boolAllVerticalCategory == 0) {
            if (is_array($arrResult['field_vertical_fmr_taxonomy']['und'])) {
                foreach ($arrResult['field_vertical_fmr_taxonomy']['und'] as $key => $value) {
                    $arrVertical[] = $value['tid'];
                    $arrPrimaryCategories[] = $value['tid'];
                    $arrSecondaryCategories[] = $value['tid'];
                }
            }
        }

        //combine media and vertical type array.
        $arrMediaFilter = array('media_type' => $arrMediaType,
            'secondaryCategories' => $arrSecondaryCategories,
            'primaryCategories' => $arrPrimaryCategories,
            "boolallclients" => $boolAllClients,
            "client_ogid" => $strClientOgIds,
            "allverticalcategory" => $boolAllVerticalCategory,
            "company_exclude_include" => $strCompanyExcludeInclude,
            "widget_fMR_language" => $strWidgetFMRLanguage,
            "primaryFlag" => 0,
            "secondaryFlag" => 0);

        $arrData['intPageNo'] = $intPageNo;
        $arrData['intLimit'] = $limit;
        $arrData['boolFrontPageThumb'] = @$arrResult['field_front_page_thumbnail']['und'][0]['value'];
        $arrData['intFrontPageThumbWidth'] = @$arrResult['field_front_page_thumbnail_width']['und'][0]['value'];

        //Get FMRs array for the widget page.
        list($arrData['arrContentList'], $arrData['arrCompany'], $arrData['arrCompanyName'], $arrData['intCnt'], $arrData['arrFMRContentList']) = fnGetNewsArray($boolAddView, $strPage, $limit, $arrMediaFilter, $intNewsId, $intWidgetId, $intPageNo);
    }
    // Report All running errors
    error_reporting(E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
    return $arrData;
}

/**
 * Function for getting news array
 *
 * @param bool      $boolAddView : Flag which indicate views added into the database or not.
 * @param string    $strPage     : widget requested page.
 * @param int       $limit       : number of news on the requested page
 * @param array     $arrWhere    : multiple where conditions as per the page request.
 * @param int       $intNewsId   : FMR nid (if the requested page is detail page)
 * @param int       $intWidgetId : widget eck id
 * @param int       $intPageNo   : page number [pagination]
 *
 * @return array
 */

function fnGetNewsArray($boolAddView, $strPage, $limit = 5, $arrWhere = array(), $intNewsId = 0, $intWidgetId = 0, $intPageNo = 1)
{
    $arrNodeIds = array();
    $arrContentList = array();

    //Include all required files
    module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReports');

    //Object of class
    $objReports = new ClsReports();

    if ($intNewsId > 0) { //This block contain information for the widget detail page
        // $token = (isset($_GET['t']))?base64_decode($_GET['t']):0;
        $strPage = (isset($_GET['p'])) ? base64_decode($_GET['p']) : 'more';
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
        #Collect all node ids
        $arrNodeIds[] = $intNewsId;
    } else {
        $strSecondaryIds = '';
        $strPrimaryIds = '';
        $intStart = abs($limit * ($intPageNo - 1));
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
        $sqlLimit = " LIMIT " . $intStart . ", " . $limit;

        ## ADD FILTER AS PER SELECTION
        #VERTICAL
        #PRIMARY CATEGORIES
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

        // Condition to fetch FMR of selected clients
        if ($arrWhere['client_ogid'] !="" && $arrWhere["boolallclients"] == 0) {

            // Get flag to check whether selected clients want to exclude or include
            $strCompanyExcludeInclude = $arrWhere['company_exclude_include'];

            if ($strCompanyExcludeInclude == "include" || $strCompanyExcludeInclude == "") {

                $strCompanyCondition = " AND group_audience_gid IN (".$arrWhere['client_ogid'].")";

            } else if ($strCompanyExcludeInclude == "exclude") {
                $strCompanyCondition = " AND group_audience_gid NOT IN (".$arrWhere['client_ogid'].")";
            }

            $sqlFrom .= " JOIN field_data_group_audience comp ON comp.entity_id = n1.nid $strCompanyCondition";
        }

        # MEDIA TYPE
        if (isset($arrWhere['media_type']) && is_array($arrWhere['media_type'])) {
            $strMedia = (is_array($arrWhere['media_type'])) ? "'" . implode("','", $arrWhere['media_type']) . "'" : '';
            $sqlWhere .= " AND t.field_fmr_type_of_content_value IN (" . $strMedia . ") ";
        } else {
            $sqlWhere .= " AND t.field_fmr_type_of_content_value IN (" . $arrMediaType . ")";
        }

        //Filter FMRs by Language option (all, english only and non-english)
        if ($arrWhere['widget_fMR_language'] == "english_only") {

            $sqlFrom .= " JOIN  field_data_field_fmr_non_english ne ON ne.entity_id = n1.nid";
            $sqlWhere .= " AND (ne.field_fmr_non_english_value <> 1 OR ne.field_fmr_non_english_value IS NULL)";
            
        } else if ($arrWhere['widget_fMR_language'] == "non_english") {

            $sqlFrom .= " JOIN  field_data_field_fmr_non_english ne ON ne.entity_id = n1.nid";
            $sqlWhere .= " AND ne.field_fmr_non_english_value = 1";
        }

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

/**
 * Get widget css details
 *
 * @param array  $arrResult   : array of widget page and other setting information
 * @param string $strPageType : widget page type [front, list or detail]
 *
 * @return string
 */
function fnGetWidgetCss($arrResult, $strPageType)
{
// Report simple running errors
error_reporting(E_ERROR | E_WARNING | E_PARSE);
    global $base_url;

    // General Page Css
    $strDefaultFont = $arrResult['field_default_font_family']['und'][0]['value'];
    $strTitleFont = $arrResult['field_titles_font_family']['und'][0]['value'];
    $strBodyFont = $arrResult['field_body_font_family']['und'][0]['value'];

    // Power By Text Color
    $strFrontPgPowerSize = $arrResult['field_powered_by_link_size']['und'][0]['value'];
    $strFrontPgPowerColor = $arrResult['field_powered_by_color']['und'][0]['value'];

    # FMR link <a> tag text-decoration
    $strFMRLinkUnderline = "underline";

    if ($arrResult['field_remove_underline_frm_link']['und'][0]['value'] == 1) {
        $strFMRLinkUnderline = "none";
    }
    // Flag for old css
    $intOldFlag = $arrResult['field_select_css']['und'][0]['value'];

    if (!empty($arrResult['field_widget_hidden_old_css']['und'])) {
        // getting Old widget css record.
        $arrCssResult = $arrResult['field_widget_hidden_old_css']['und'][0]['value'];
        $arrCss = unserialize(base64_decode(html_entity_decode($arrCssResult)));
    }
    $strCssValue = '';

    switch ($strPageType) {
        case 'FrontPage':

            //Front Page Css
            // Additional
            $strFrontPgAdditional = $arrResult['field_front_page_additional']['und'][0]['value'];

            # Front Thumbnail Width
            $strFrontPageThumbnailWidth = $arrResult['field_front_page_thumbnail_width']['und'][0]['value'];
            if ($strFrontPageThumbnailWidth == '') {
                $strFrontPageThumbnailWidth = 40;
            }

            #Get the Front page description font size and color
            $intFrontPageDescSize = $arrResult['field_front_page_description_fon']['und'][0]['value'];
            $intFrontPageDescColor = $arrResult['field_front_description_font_col']['und'][0]['value'];

            #get the value of check box for thumbnail.
            $strFrontPageThumbnail = $arrResult['field_front_page_thumbnail']['und'][0]['value'];
            $intRightDivWidth = ($strFrontPageThumbnail) ? $strFrontPageThumbnailWidth + 10 : 0;

            $strThumbnailCSS = '#threeblmediawidget ul.threebl_sidebar div.thumbnailWidth{width:' . $strFrontPageThumbnailWidth . 'px !important;padding-right:10px;float:left}
                                #threeblmediawidget ul.threebl_sidebar div.thumbnailWidth img{width:100% !important}
                                #threeblmediawidget ul.threebl_sidebar div.rightdivwidth{padding-left:' . $intRightDivWidth . 'px}';

            # Front page description condition and css class
            $strFrontPageDescCss = "";
            if ($arrResult['field_front_page_fmr_description']['und'][0]['value'] > 0) {
                $strFrontPageDescCss = '#threeblmediawidget ul.threebl_sidebar div.threebl_frontdesc{color:#'.$intFrontPageDescColor.';line-height:16px;margin:10px 0; font-size :'.$intFrontPageDescSize.'px}';
            }

            // Old widget css
            if ($intOldFlag == 1 && !empty($arrCss)) {
                return addslashes($arrCss['frontpage'] . $strThumbnailCSS . $strFrontPageDescCss. $strFrontPgAdditional);
            }

            //Front Title
            $strFrontPgTitleSize = $arrResult['field_front_page_title_size']['und'][0]['value'];
            $strFrontPgTitleColor = $arrResult['field_front_title_color']['und'][0]['value'];


            //Front Size
            $strFrontPgHeadSize = $arrResult['field_front_page_headlines_size']['und'][0]['value'];
            $strFrontPgHeadColor = $arrResult['field_front_headlines_color']['und'][0]['value'];


            $strFrontPgHeadHcolor = $arrResult['field_front_headlines_hover']['und'][0]['value'];

            $strFrontPgHeadLHeight = $arrResult['field_front_page_headlines_line_']['und'][0]['value'];

            //Front Time
            $strFrontPgTimeDisplay = $arrResult['field_front_page_date_time_displ']['und'][0]['value'];
            $strFrontPgTimeSize = $arrResult['field_front_page_date_time_size']['und'][0]['value'];

            $strFrontPgTimeColor = $arrResult['field_front_date_time_color']['und'][0]['value'];

            //Front More
            if (!empty($arrResult['field_see_more_link_size']['und'])){
            $strFrontPgMoreSize = $arrResult['field_see_more_link_size']['und'][0]['value'];
            }
            
            if (!empty($arrResult['field_view_more_link_color']['und'])){
            $strFrontPgMoreColor = $arrResult['field_view_more_link_color']['und'][0]['value'];
            }

            if (!empty($arrResult['field_view_more_link_hover_color']['und'])){
            $strFrontPgMoreHcolor = $arrResult['field_view_more_link_hover_color']['und'][0]['value'];
            }

            $strCssValue = '/* CSS for Frontpage Title*/
		   #threeblmediawidget h2.threebl_title {color:#' . $strFrontPgTitleColor . ';font-size:' . $strFrontPgTitleSize . 'px; margin-top: 10px;text-transform: uppercase;}

			/* CSS for Frontpage Headline */
			#threeblmediawidget ul li h2 a.threebl_fmr_headlines{color:#' . $strFrontPgHeadColor . ';font-size:' . $strFrontPgHeadSize . 'px;margin: 0;padding: 0;
			vertical-align: middle;line-height:' . $strFrontPgHeadLHeight . ';text-decoration:' . $strFMRLinkUnderline . '}
			#threeblmediawidget ul li h2 a.threebl_fmr_headlines:hover{color:#' . $strFrontPgHeadHcolor . ';font-size:' . $strFrontPgHeadSize . 'px;margin: 0;padding: 0;vertical-align: middle; line-height:' . $strFrontPgHeadLHeight . '}

			/* CSS for Date and Time */
			#threeblmediawidget .threebl_datetime {color:#' . $strFrontPgTimeColor . ' !important; font-size:' . $strFrontPgTimeSize . 'px !important; padding: 5px 0; display:' . $strFrontPgTimeDisplay . ';}

			/* CSS for View more link */
			#threeblmediawidget ul li a.threebl_vew_more{color:#' . $strFrontPgMoreColor . ';font-size:' . $strFrontPgMoreSize . 'px;}
			#threeblmediawidget ul li a.threebl_vew_more:hover {color:#' . $strFrontPgMoreHcolor . ';font-size:' . $strFrontPgMoreSize . 'px ;}

			/* CSS for Headline blocks*/
			#threeblmediawidget ul {list-style:none;}
			#threeblmediawidget ul.threebl_sidebar{ list-style:none; padding:0 10px; font-family:' . $strBodyFont . ';}
			#threeblmediawidget ul.threebl_sidebar li{ padding:10px 0; border-bottom:1px dotted #ccc}
			#threeblmediawidget ul.threebl_sidebar li.threebl_border-none{ border:none}{ padding:10px 0; border-bottom:1px dotted #fff;}

			/* CSS for anchor tag*/
			#threeblmediawidget ul.threebl_sidebar a{text-decoration:none; display:block; color:#00AACC;text-decoration:underline;}
			#threeblmediawidget ul.threebl_sidebar a:hover{ color:#00AACC; text-decoration:none;}

			/* CSS for Read more link */
			#threeblmediawidget ul.threebl_sidebar li .threebl_view-more{ text-align:right; font-size:11px;}

			/* CSS for Powered By 3BLMedia.com */
			#threeblmediawidget .threebl_link a {color:#' . $strFrontPgPowerColor . ' !important;font-size:' . $strFrontPgPowerSize . 'px;}


            /* css for front page thumbnail width*/
            ' . $strThumbnailCSS . '

             /* css for front page teaser*/
            ' . $strFrontPageDescCss . '

			/* Common classes */
			#threeblmediawidget {color:#333; font-family:' . $strDefaultFont . ';font-size:12px;padding: 0 20px; }
			#threeblmediawidget h1, #threeblmediawidget h2, #threeblmediawidget h3{ padding:0; margin:0; font-family:' . $strTitleFont . ' }
			#threeblmediawidget a:link, #threeblmediawidget a:visited, #threeblmediawidget a:hover, #threeblmediawidget a:active {color:#00AACC;}' . $strFrontPgAdditional;

            break;

        case 'ListPage':

            // Additional
            $strListPgAdditional = $arrResult['field_list_page_additional_']['und'][0]['value'];

            # Read More link color
            $strReadMoreLinkColor = $arrResult['field_read_more_link_color']['und'][0]['value'];

            # Read More link font size
            $strReadMoreLinkSize = $arrResult['field_read_more_link_size']['und'][0]['value'];

            if ($strReadMoreLinkColor == "") {
                $strReadMoreLinkColor = "00aacc";
            }
            if ($strReadMoreLinkSize == "") {
                $strReadMoreLinkSize = 12;
            }

            // Old widget css list page css
            if ($intOldFlag == 1 && !empty($arrCss)) {
                return addslashes($arrCss['list'] . $strListPgAdditional);
            }

            //List  Title
            $strListPgTitleSize = $arrResult['field_list_page_title_size']['und'][0]['value'];
            $strListPgTitleColor = $arrResult['field_list_title_color']['und'][0]['value'];

            //List Headlines
            $strListPgHeadSize = $arrResult['field_list_page_headlines_size']['und'][0]['value'];

            $strListPgHeadColor = $arrResult['field_list_headlines_color']['und'][0]['value'];

            $strListPgHeadHcolor = $arrResult['field_list_headlines_hover_color']['und'][0]['value'];

            $strListPgHeadLHeight = $arrResult['field_list_page_headlines_line_h']['und'][0]['value'];

            //List Time
            $strListPgTimeSize = $arrResult['field_list_page_date_time_size']['und'][0]['value'];
            $strListPgTimeColor = $arrResult['field_list_date_time_color']['und'][0]['value'];

            //List Fmr
            $strListFmrSize = $arrResult['field_fmr_type_size']['und'][0]['value'];
            $strListFmrColor = $arrResult['field_fmr_type_colors']['und'][0]['value'];

            //List Company
            $strListPgCompanySize = $arrResult['field_list_page_company_name_siz']['und'][0]['value'];
            $strListPgCompanyColor = $arrResult['field_list_company_name_color']['und'][0]['value'];

            $strCssValue = '/* CSS for List Page Title */
		#threeblmediadetaillist h2.threebl_title {color:#' . $strListPgTitleColor . ';font-size:' . $strListPgTitleSize . 'px;margin-top: 10px;text-transform: uppercase;}

		/* CSS for List Page Headline */
		#threeblmediadetaillist ul li h2 a.threebl_fmr_headlines{color:#' . $strListPgHeadColor . ';font-size:' . $strListPgHeadSize . 'px;margin: 0;
		padding: 0;vertical-align: middle;line-height:' . $strListPgHeadLHeight . ';text-decoration:' . $strFMRLinkUnderline . '}

		#threeblmediadetaillist ul li h2 a.threebl_fmr_headlines:hover{color:#' . $strListPgHeadHcolor . ';font-size:' . $strListPgHeadSize . 'px;margin: 0;padding: 0;vertical-align: middle; text-decoration: none;line-height:' . $strListPgHeadLHeight . '}

		/* CSS for Teaser block*/
		#threeblmediadetaillist ul.threebl_sidebar .threebl_teaser{margin-top:5px;}

		/* CSS for Headline blocks*/
		#threeblmediadetaillist ul {list-style:none;}

		/* CSS for category from */
		#threeblmediadetaillist ul.threebl_sidebar .threebl_teaser span{color:#' . $strListFmrColor . '; font-size:' . $strListFmrSize . 'px; float:left;}

		/* CSS for Company Name */
		#threeblmediadetaillist ul.threebl_sidebar .threebl_teaser a.threebl_company_name{color:#' . $strListPgCompanyColor . ';display:inline;
		text-decoration:none; font-size:' . $strListPgCompanySize . 'px;}

		#threeblmediadetaillist ul.threebl_sidebar .threebl_teaser a.threebl_company_name:hover{color:#' . $strListPgCompanyColor .
                ';display:inline;text-decoration:none; font-size:' . $strListPgCompanySize . 'px;}

		/* CSS for Date and Time */
		#threeblmediadetaillist .threebl_datetime {color:#' . $strListPgTimeColor . '!important;font-size:' . $strListPgTimeSize . 'px !important;padding:5px 0;}

		/* CSS for Teaser Description*/
		#threeblmediadetaillist .threebl_desc16{clear: both;color:#2C2C2B;line-height:16px;margin:10px 0;}

		/* CSS for View more link */
		#threeblmediadetaillist .threebl_desc16 a.threebl_read_more{color:#' . $strReadMoreLinkColor . ';font-size:' . $strReadMoreLinkSize . 'px;}
		#threeblmediadetaillist .threebl_desc16 a.threebl_read_more:hover{color:#' . $strReadMoreLinkColor . ';font-size:' . $strReadMoreLinkSize . 'px;}

		/* CSS for Powered By threeblMedia.com */
		#threeblmediadetaillist .threebl_link a{color:#' . $strFrontPgPowerColor . ' !important;font-size:' . $strFrontPgPowerSize . 'px;}


		/* CSS for News block*/
		#threeblmediadetaillist ul.threebl_sidebar{ list-style:none; padding:0 10px; font-family:' . $strBodyFont . ';}
		#threeblmediadetaillist ul.threebl_sidebar li{ padding:10px 0; border-bottom:1px dotted #ccc}
		#threeblmediadetaillist ul.threebl_sidebar li.threebl_border-none{ border:none}{ padding:10px 0; border-bottom:1px dotted #fff;}

		/* Common classes */
		.threebl_fl{float:left}
		#threeblmediadetaillist{font-family:' . $strDefaultFont . '; color:#2C2C2B; font-size:12px; padding: 0 20px; }
		#threeblmediadetaillist h1, #threeblmediadetaillist h2, #threeblmediadetaillist h3{ padding:0; margin:0; font-family:' . $strTitleFont . '}
		#threeblmediadetaillist a:link, #threeblmediadetaillist a:visited, #threeblmediadetaillist a:hover, #threeblmediadetaillist a:active {color:#00AACC;}
		.pglink{border: 1px solid #00AACC; margin: 5px; padding: 1px 3px; text-decoration: none}

		/* CSS For Pagination*/
		#threeblmediadetaillist .pagination {float: right;}' . $strListPgAdditional;
            break;

        case 'DetailPage':

            // Additional
            $strDetailsPgAdditional = $arrResult['field_detail_page_additional']['und'][0]['value'];

            $strTweetMeFontFamily = " #threeblmediadetaillist #fmr-tweet {font-family:".$strDefaultFont."}";

            // if old widget record for details page.
            if ($intOldFlag == 1 && !empty($arrCss)) {
                return addslashes($arrCss['details'].$strDetailsPgAdditional.$strTweetMeFontFamily);
            }

            // Details headlines
            $strDetailsPgHeadSize = $arrResult['field_detail_page_headline_size']['und'][0]['value'];
            $strDetailsPgHeadColor = $arrResult['field_detail_headline_color']['und'][0]['value'];

            //Details Time
            $strDetailsPgTimeSize = $arrResult['field_detail_page_date_time_size']['und'][0]['value'];
            $strDetailsPgTimeColor = $arrResult['field_detail_date_time_color']['und'][0]['value'];

            //Details Tweet
            $strDetailsPgTweetSize = $arrResult['field_tweetme_size']['und'][0]['value'];
            $strDetailsPgTweetColor = $arrResult['field_tweetme_colors']['und'][0]['value'];

            //Details Company
            $strDetailsPgBackToSize = $arrResult['field_back_to_headline_size']['und'][0]['value'];
            $strDetailsPgBackToColor = $arrResult['field_back_to_headline_link_colo']['und'][0]['value'];

            //Back To Headline CSS
            $strDetailsPgCompanySize = $arrResult['field_detail_page_company_name_s']['und'][0]['value'];
            $strDetailsPgCompanyColor = $arrResult['field_detail_company_name_color']['und'][0]['value'];

            //Details Body
            $strDetailsPgBodySize = $arrResult['field_detail_page_body_size']['und'][0]['value'];
            $strDetailsPgBodyColor = $arrResult['field_detail_body_color']['und'][0]['value'];

            $strCssValue = "/* New Details page Title */

			#threeblmediadetaillist .threebl_title-box h1{ color:#" . $strDetailsPgHeadColor . "!important;font-family:" . $strBodyFont . "; font-size:" . $strDetailsPgHeadSize . "px !important;
			margin: 0;padding: 0;}

			/*CSS For Date and Time*/
			#threeblmediadetaillist .threebl_datetime{ color:#" . $strDetailsPgTimeColor . "!important;font-size:" . $strDetailsPgTimeSize . "px !important;padding:5px 0; margin: 0 0 20px;}

			/*CSS For Twitter*/
			#threeblmediadetaillist #fmr-tweet {background:url('" . $base_url . "/sites/all/themes/threebl/images/tweetbg.gif')
			no-repeat 0 0; height:35px; margin-bottom:20px; padding:15px;}
			#threeblmediadetaillist #fmr-tweet-tweetme-text {color:#" . $strDetailsPgTweetColor . "; float:left; margin-right:10px; text-transform:uppercase;
			font-size:" . $strDetailsPgTweetSize . ";}
			#threeblmediadetaillist #fmr-tweet-icon {float:left; margin-right:10px;}
			#threeblmediadetaillist .threebl_field-name-field-fmr-tweet {max-width:485px;}
			#threeblmediadetaillist #fmr-tweet-overlay {height:55px; margin-left:-15px; margin-top:-30px; position:absolute; width:500px;}

			/* CSS for Company Name */
			#threeblmediadetaillist .back-to-headlines a.threebl_company{color:#" . $strDetailsPgCompanyColor . ";display:inline;text-decoration:none;
			font-size:" . $strDetailsPgCompanySize . "px;}

			#threeblmediadetaillist .back-to-headlines a.threebl_company:hover{color:#" . $strDetailsPgCompanyColor . "; display:inline;text-decoration:none;
			font-size:" . $strDetailsPgCompanySize . "px;}

			/* CSS for Line */
			#threeblmediadetaillist hr{color:#fff;margin:10px 0}

			/* CSS for FMR description*/
			#threeblmediadetaillist .threebl_description{color:#" . $strDetailsPgBodyColor . "; font-family:" . $strBodyFont . ";
			font-size:" . $strDetailsPgBodySize . "px;line-height:16px;}

			/* CSS FOR Image data */
			#threeblmediadetaillist .fmrImg{font-size:12px;font-weight: bold;}

			/* CSS FOR Video data */
			#threeblmediadetaillist .frmvideo{font-size:12px;font-weight: bold;}

			/* CSS FOR Audio data */
			#threeblmediadetaillist .fmrAudio{font-size:12px;font-weight: bold;}

			/* CSS for clr Div*/
			#threeblmediadetaillist .threebl_clr{margin-bottom:5px;clear:both;}

			/* CSS for Links*/
			#threeblmediadetaillist ul.threebl_sidebar a{text-decoration:none; display:block; color:#00AACC; text-decoration:underline;}
			#threeblmediadetaillist ul.threebl_sidebar a:hover{ color:#00AACC; text-decoration:none;}

			/*Padding from top*/
			#threeblmediadetaillist .threebl_title-box{padding-top: 34px;}

			/* CSS for back to headlines */
			#threeblmediadetaillist a.threebl_back_to_HeadLines{color: #" . $strDetailsPgBackToColor . " !important;font-size:" . $strDetailsPgBackToSize . "px;float:right;}


			/* CSS for Powered By 3BLMedia.com */
			#threeblmediadetaillist a.threebl_link{color:#" . $strFrontPgPowerColor . " !important;font-size:" . $strFrontPgPowerSize . "px;float:left;}

			/* Common classes */
			#threeblmediadetaillist h1, #threeblmediadetaillist h2, #threeblmediadetaillist h3{ padding:0; margin:0; font-family:" . $strTitleFont . "}
			#threeblmediadetaillist{font-family:" . $strDefaultFont . "; color:#2C2C2B; font-size:12px; padding: 0 20px;}
			#threeblmediadetaillist a:link, #threeblmediadetaillist a:visited, #threeblmediadetaillist a:hover, #threeblmediadetaillist a:active {color:#00AACC;}
			#threeblmediadetaillist .threebl_margin20{clear:both;margin:20px 0;}".$strTweetMeFontFamily.$strDetailsPgAdditional;
            break;

        default:
            break;
    }
    //end -switch case
    
    // Report simple running errors
    error_reporting(E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
    return $strCssValue;
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

/* Added for refactoring */

public function fnSetSettingsArray($arrResult, $intDescriptionLimit, $strFrontPageDesc, $strFMRInNewTab)
{
	//Collect all settings into the array
        
        $arrSettings = array(
            'widget_height' => $arrResult['field_widget_height']['und'][0]['value'],
            'widget_width' => $arrResult['field_widget_width']['und'][0]['value'],
            'widget_no_post' => $arrResult['field_widget_no_of_posts_on_font']['und'][0]['value'],
            'widget_front_title' => $arrResult['field_widget_front_page_title']['und'][0]['value'],
            'widget_list_title' => $arrResult['field_widget_list_page_title']['und'][0]['value'],
            'widget_viewmore_text' => $arrResult['field_widget_view_more_label']['und'][0]['value'],
            'widget_no_more_posts' => $arrResult['field_widget_no_of_posts_on_list']['und'][0]['value'],
            'widget_viewmore_link' => $arrResult['field_affiliate_news_url']['und'][0]['value'],
            'widget_video_width' => $arrResult['field_widget_width_of_video']['und'][0]['value'],
            'widget_poweredBy' => (isset($arrResult['field_widget_power_by']['und'][0]['value'])) ? 1 : 0,
            'widget_listpageDesc_limit' => $arrResult['field_list_page_description_limi']['und'][0]['value'],
            'widget_frontpageTitle_limit' => $arrResult['field_front_page_title_limit']['und'][0]['value'],
            'widget_frontpage_desc_limit' => $intDescriptionLimit,
            'widget_frontpage_desc' => $strFrontPageDesc,
            'widget_fmr_in_tab' => $strFMRInNewTab
        );

        return $arrSettings;

}

public function fnGetPhotoData($node)
{

	foreach ($node->field_fmr_photo['und'] as $k => $photo) {
        $file = file_load($photo['fid']);

        #Assign 3BL path
        $strPhotoLink = urldecode(image_style_url('medium', "images/" . basename(file_create_url($file->uri))));
        $strFilePath = @file_get_contents($strPhotoLink);
        if ($strFilePath == "") {
            continue;
        }

        //Add available Photo Information
        $arrContentList[$k]['photolink'] = $strPhotoLink;
        $arrPhotoData = explode("/", $strPhotoLink);
        $arrContentList[$k]['name'] = $arrPhotoData[5];
        $arrContentList[$k]['description'] = $photo['title'];
    }    
    return $arrContentList;
}

public function fnGetAudioData($node)
{
	foreach ($node->field_fmr_audio['und'] as $k => $audio) {

        $file = file_load($audio['fid']);
        $arrContentList[$k]['audioLink'] = file_create_url($file->uri);
        $strAudioLink = $arrContentList['audio'][$k]['audioLink'];
        $arrAudioData = explode("/", $strAudioLink);
        $arrContentList[$k]['name'] = $arrAudioData[5];

    }
    return $arrContentList;
}

public function fnGetVideoData($node)
{
	$arrURLs = $arrFiles = array();
    foreach ($node->field_video['und'] as $k => $video) {
    	//$video = $node->field_video['und'][0];
	    
	    $arrURLs[$video['fid']]['thumbnail'] = file_create_url(file_load(array($video['thumbnail']))->uri);
	    $arrURLs[$video['fid']]['original'] = file_create_url($video['uri']);

	    $query = db_query('SELECT output_fid FROM video_output WHERE original_fid=:fid', array(':fid' => $video['fid']))->fetchAll();
	    foreach ($query as $video_output) {
	        $file = file_load($video_output->output_fid);
	        $arrFiles[$video['fid']][$file->filemime] = $file;
	        $arrURLs[$video['fid']][$file->filemime] = file_create_url($file->uri);
	     }

	    $arrContentList[$k]['videourl'] = addslashes($arrURLs[$video['fid']]['video/mp4']);
	    $arrContentList[$k]['videotype'] = '.flv';
	    $arrContentList[$k]['videothumb'] = $arrURLs[$video['fid']]['thumbnail'];

	    // we are separating thumbnail image with and without file extention
	    $strVideoThumbImage = $arrContentList[$k]['videothumb'];
	    $arrVideoTumbData = explode("/", $strVideoThumbImage);
	                
	    $arrContentList[$k]['videothumbimg'] = $arrVideoTumbData[7];
	                
	    $strVideoUrl = $arrContentList[$k]['videourl'];
	    $arrVideoData = explode("/", $strVideoUrl);
	                
	    $arrContentList[$k]['link'] = $arrVideoData[6];
    }
            //foreach end
}

/* Added for refactoring */


?>