<?php
//This is class for Analytics functions.

class clsAnalyticsViews
{
    /**
     * Query for getting View Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId          : Company OG id
     * @param string $strFMRIds               : FMR node id's with comma separated
     *
     * @return mixed
     */
    public function fnGetAnalyticViewChartInfo($intCompanyOgId, $strFMRIds = "")
    {
        $strSQL = "SELECT SUM(v.totalcount) as totalcount, v.datevisited AS datevisit
                  FROM " . $this->strViewTableName . " AS v
                  $strCampaignFrom
                  WHERE v.company_ogid = $intCompanyOgId
                  AND (v.datevisited BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "')
                  AND v.nid IN ($strFMRIds)
                  GROUP BY datevisited
                  ORDER by datevisited ASC";

        $arrViewChartData = db_query($strSQL)->fetchAll();
        $arrViewChart = array();
        //Checking empty array condition
        if (!empty($arrViewChartData)) {
            foreach ($arrViewChartData as $arrVal) {
                $arrViewChart[$arrVal->datevisit] = $arrVal->totalcount;
            }
        }
        return $arrViewChart;
    }

    /**
     *  Getting View-Count By Media-Id
     *
     * @param int  $intCompanyOgId :  Company OG id
     * @param int  $intMediaId     : FMR node id
     *
     * @return mixed
     */
    public function fnGetFMRViewCountByMediaId($intCompanyOgId, $intMediaId)
    {
        $strSQL = "SELECT  SUM(v.totalcount) as totalcount
                  FROM " . $this->strViewTableName . " as v
                  JOIN field_data_field_fmr_type_of_content t ON  v.nid = t.entity_id
                  Where v.company_ogid = $intCompanyOgId AND v.nid = $intMediaId
                  GROUP BY  v.nid";
        return $arrViewCount = db_query($strSQL)->fetchAssoc();
    }

    /**
     * Process view data for cron functionality and save analytics in to the view table.
     *
     * @param array        $arrFmrViews : Array of FMRs node ids
     * @param string       $otherInfo   : Other information such as date
     * @param string       $strChannel  : default channel value for save record into the database
     */
    public function fnViewsProcessing($arrFmrViews, $otherInfo, $strChannel = "3blmedia")
    {
        // Get out the IP addr and date
        $strJustDate = $otherInfo['view_date'];

        // fmrViews is a 2-D array with the first dimension being associative with FMR types and the second
        // dimension being an array of FMR IDs (nids) for each FMR type.
        // There can be multiple media's for inbound and we will process each of them.
        if (count($arrFmrViews) && is_array($arrFmrViews)) {
            foreach ($arrFmrViews as $strFMRType => $FMRids) {
                if (count($FMRids) && is_array($FMRids)) {
                    $this->fnAddViewsProcess($FMRids, $strFMRType, $strChannel, $strJustDate);

                }
            }
        }
    }

    public function fnAddViewsProcess($FMRids, $strFMRType, $strChannel, $strJustDate) {

        foreach ($FMRids as $FMRid) {

            if ((int)$FMRid > 0) {
                // for each FMR, we need to get the master ids, from the sync tables, for
                // both the FMR and Company
                $arrMedia = $this->fnGetCompanyOgIds($FMRid);


                if (!empty($arrMedia['etid'])) {
                    $intFMRNid = $arrMedia['etid'];
                }
                if (!empty($arrMedia['companyOgId'])) {
                    $intCompanyOgId = $arrMedia['companyOgId'];
                }


                // Set $strMediaType  to the appropriate jm_mediawise_views column name, to be used in
                // the update/insert queries, below.

                list($strMediaType, $intFMRNid, $intCompanyOgId) = $this->fnSetMediaType($strFMRType, $intFMRNid, $intCompanyOgId);

                // This is to insert count in views table as well as in the dummy views table, we took dummy table because it will have data for a month which will automatically gets deleted after that this will keep fast processing of data. We can grab all the historic data from main views table, also count   gets incremented as media is viewed.
                if ($strMediaType && (int)$intFMRNid > 0 && (int)$intCompanyOgId > 0) {
                    //Creating the object for ClickView Class
                    $ObjClickView = new clsClickView();

                    //Create array for view and click tables
                    $arrAllView = array();
                    $arrAllView['intCompanyId'] = $intCompanyOgId;
                    $arrAllView['intMediaId'] = $intFMRNid;
                    $arrAllView['strChannel'] = $strChannel;
                    $arrAllView['strFMRType'] = $strMediaType;
                    $arrAllView['strViewDate'] = $strJustDate;
                    //Add View Table  Function
                    $ObjClickView->fnAddAllViews($arrAllView);
                }
            }
        }
    }

    public function fnSetMediaType($strFMRType, $intFMRNid, $intCompanyOgId) {

        switch ($strFMRType) {
                    case "pressrelease":
                    case "press_release":
                    case "press":
                        $strMediaType = 'press_release'; // this is how it is stored in jm_..._views tables
                        break;
                    case "article":
                    case "3bl_article":
                        $strMediaType = 'article';
                        break;
                    case "newsletter":
                    case "3bl_newsletter":
                        $strMediaType = 'newsletter';
                        break;
                    case "multimedia":
                        $strMediaType = 'multimedia';
                        break;
                    case "blog":
                    case "blogs":
                        $strMediaType = 'blog';
                        break;
                    default:
                        $intFMRNid = 0;
                        $strMediaType = "";
                        $intCompanyOgId = 0;
                        break;
        }

        return array($strMediaType, $intFMRNid, $intCompanyOgId);
    }
}