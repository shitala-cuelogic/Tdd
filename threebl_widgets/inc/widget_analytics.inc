<?php
/**
 * Add Views and Clicks Count for FMR
 *
 * @param array  $arrNodeIds  : Array of FMR node ids
 * @param bool   $boolAddView : Flag which indicate add views and clicks for FMRs
 * @param int    $intWidgetId : Widget entity id
 * @param string $strPage     : widget page type
 *
 * @return array
 */

function fnAddFMRAnalyticsViews($arrNodeIds, $boolAddView = false, $intWidgetId = 0, $strPage = 'more')
{
    $strChannel = '3bl_widgets';
    module_load_include('inc', 'threebl_analytics', 'inc/clsClickView'); // include file

    $arrCompany = array();
    if (count($arrNodeIds) <= 0) {
        return;
    }
    //if

    $strNodeId = implode(',', $arrNodeIds);

    //Here we removed join of jm_3bl_media(master_MediaId)
    //Add company join again master table
    $strCompMediaIdsSql = "SELECT ogm.etid, ogm.gid AS CompanyOgId, og.etid AS CompanyNid
                            FROM og_membership ogm
                            JOIN og ON og.gid = ogm.gid
                            WHERE ogm.etid IN (" . $strNodeId . ") and ogm.entity_type = 'node'";
    $arrCompMediaIds = db_query($strCompMediaIdsSql)->fetchAll();

    if (@count($arrCompMediaIds) > 0) {
        foreach ($arrCompMediaIds as $arrMediaId) {
            # GET FMR INFO
            $objNode = node_load($arrMediaId->etid);
            $strFMRType = $objNode->field_fmr_type_of_content['und'][0]['value'];

            //Return Specific FMR CompanyId
            $arrCompany[$arrMediaId->etid] = $arrMediaId->CompanyNid;

            if ($boolAddView == true) {

                // To increment the views of companies.
                if ($strPage != 'more') {
                    fnAddCompanyAnalyticsViews($arrMediaId->CompanyNid, $intWidgetId, 1);
                }

                //Create Object Of ClickView Class
                $ObjClickView = new clsClickView();

                //Create array for view and click tables
                $arrAllView = array();
                $arrAllView['intCompanyId'] = $arrMediaId->CompanyOgId;
                $arrAllView['intMediaId'] = $arrMediaId->etid;
                $arrAllView['strChannel'] = $strChannel;
                $arrAllView['strFMRType'] = $strFMRType;

                //Add View Table  Function
                $ObjClickView->fnAddAllViews($arrAllView);

                //Add click array if widget id is grater than 0
                if (@(int)$intWidgetId > 0) {
                    //Add the value in array AllView for Clicks
                    $arrAllView['intChannelId'] = $intWidgetId;
                    // Add Clicks  Table  Function
                    $ObjClickView->fnAddAllClicks($arrAllView);
                }
                //if
            }
            //boolAddValue end
        }
        //fore each end
    }
    return $arrCompany;
}// end Function

/**
 * Tracking Company Click and Views
 *
 * @param int $intCompanyNodeId : Company nid
 * @param int $intWidgetId      : Widget entity Id
 * @param int $intFlag          : indicate the about insertion of click count for company.
 */
function fnAddCompanyAnalyticsViews($intCompanyNodeId = 0, $intWidgetId = 0, $intFlag = 0)
{
    $strChannel = '3bl_widgets';
    $strFMRType = "Client";

    module_load_include('inc', 'threebl_analytics', 'inc/clsClickView'); // include file

    if ($intCompanyNodeId <= 0) {
        return;
    }

    $strCompMediaIdsSql = "SELECT og.gid AS CompanyOgId
                                 FROM og
                                 WHERE og.etid ='" . $intCompanyNodeId . "'";
    $arrCompMediaIds = db_query($strCompMediaIdsSql)->fetchAll();
    if (count($arrCompMediaIds) > 0) {

        $objArray = $arrCompMediaIds[0];

        //Create Object Of ClickView Class
        $ObjClickView = new clsClickView();

        //Create array for view and click tables
        $arrAllView = array();
        $arrAllView['intCompanyId'] = $objArray->CompanyOgId;
        $arrAllView['intMediaId'] = $intCompanyNodeId;
        $arrAllView['strChannel'] = $strChannel;
        $arrAllView['strFMRType'] = $strFMRType;
        //Add View Table  Function
        $ObjClickView->fnAddAllViews($arrAllView);

        //Add click array if widget id is grater than 0
        if (@(int)$intWidgetId > 0 && $intFlag != 1) {
            //Add the value in array AllView for Clicks
            $arrAllView['intChannelId'] = $intWidgetId;
            // Add Clicks  Table  Function
            $ObjClickView->fnAddAllClicks($arrAllView);
        }
        //if
    }
    return;
}//end fnTrackClicksViews
