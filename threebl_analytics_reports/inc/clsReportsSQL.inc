<?php
//This is class for Reports functions.
class ClsReportsSQL extends clsBaseClass
{

    /**
     * Query for getting View Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId          : Company OG Id
     * @param string $strFMRIds               : Comma separated FMR Ids
     * @param string $strChartDateBetweenFlag : Chart date between flag
     * @param int    $intCampaignId           : Campaign Id
     *
     * @return array
     */
    public function fnGetViewChartInfo($intCompanyOgId, $strFMRIds = "", $strChartDateBetweenFlag = "0", $intCampaignId = 0)
    {
        # Check date between date visited and current date
        if ($strChartDateBetweenFlag == "1") {
            $strChartDateBetween = " AND (v.datevisited BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "') ";
        }

        $strCampaignFrom = ''; //change
        if ((int)$intCampaignId != 0) {
            $strCampaignFrom = "  JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = v.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        }

        $strCondition = "";
        if ($strFMRIds != "") {
            $strCondition = " AND v.nid IN ($strFMRIds)";
        }
        $strSQL = "SELECT SUM(v.totalcount) as totalcount, v.datevisited AS datevisit
                  FROM " . $this->strViewTableName . " AS v
                  $strCampaignFrom
                  WHERE v.company_ogid = $intCompanyOgId
                  $strChartDateBetween
                  $strCondition
                  GROUP BY datevisited
                  ORDER by datevisited ASC";

        $arrViewChartData = db_query($strSQL)->fetchAll();
        $arrViewChart = array();

        //Checking empty condition
        if (!empty($arrViewChartData)) {

            foreach ($arrViewChartData as $arrVal) {
                $arrViewChart[$arrVal->datevisit] = $arrVal->totalcount;
            }
        }
        return $arrViewChart;
    }

    /**
     * Query for getting Click Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId               : Company OG Id
     * @param string $strFMRIds                    : Comma separated FMR Ids
     * @param string $strClickChartDateBetweenFlag : Chart date between flag
     * @param int    $intCampaignId                : Campaign Id
     *
     * @return array
     */
    public function fnGetClickChartInfo($intCompanyOgId, $strFMRIds = "", $strClickChartDateBetweenFlag = "0", $intCampaignId = 0)
    {
        $strClickChartDateBetween = '';
        if ($strClickChartDateBetweenFlag == "1") {
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

        $arrayViewChartData = db_query($strSQL)->fetchAll();
        $arrClickChart = array();

        //Checking empty condition
        if (!empty($arrayViewChartData)) {

            foreach ($arrayViewChartData as $arrVal) {
                $arrClickChart[$arrVal->clickdate] = $arrVal->totalcount;
            }
        }
        return $arrClickChart;
    }

    /**
     * Query for getting all the views and FMR count by company id and by media-type we get count of particular media
     *
     * @param int    $intCompanyOgId : Company OG Id
     * @param string $strMediaType   : Media Type
     * @param int    $intCampaignId  : Campaign Id
     * @param string $strVideo       : Flag to check video or all type
     *
     * @return mixed
     */
    public function fnGetFMRViewsByCompany($intCompanyOgId, $strMediaType = '', $intCampaignId = 0, $strVideo = "all")
    {

        $strSelect = " SELECT SUM(v.totalcount) AS viewcount, COUNT(DISTINCT(n.nid)) as totalFMR, t.field_fmr_type_of_content_value as mediatype ";

        $strFrom = " FROM " . $this->strViewTableName . " AS v
                     JOIN node AS n ON n.nid = v.nid
                     JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                     JOIN field_data_field_fmr_type_of_content t ON n.nid = t.entity_id ";

        $strWhere = " WHERE v.company_ogid = $intCompanyOgId AND n.status = 1
                      AND pubdt.field_fmr_date_time_value >= DATE_SUB('" . $this->strCurrentDate . "' , INTERVAL 180 DAY ) ";

        $strCondition = '';
        $strCampaignFrom = ''; //change
        if ((int)$intCampaignId > 0) {
            $strCampaignFrom = "  JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = n.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        } else {
            if ($strMediaType != '') {
                if ($strMediaType != "all") {
                    $strCondition .= " AND t.field_fmr_type_of_content_value = '$strMediaType' ";
                } else {
                    $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')";
                }
            } else {
                $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') GROUP BY t.field_fmr_type_of_content_value ";
            }
        }

        if ($strVideo == "all") {

            $strQry = $strSelect . $strFrom . $strCampaignFrom . $strWhere . $strCondition;
            $arrViews = db_query($strQry)->fetchAll();

        } elseif ($strVideo == "NO") {

            //Get FMRs with Video attached with them.
            $strFMRIds = $this->fnGetFMRWithVideo($intCompanyOgId);

            //Get the Non Videos FMRs
            $strQry = $strSelect . $strFrom . $strCampaignFrom . $strWhere . " AND n.nid NOT IN ($strFMRIds)" . $strCondition;
            $arrViews = db_query($strQry)->fetchAll();
        }
        return $arrViews;
    }

    /**
     * Query for getting View Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId          : Company OG Id
     * @param string $strMediaType            : Media Type
     * @param string $strChartDateBetweenFlag : Chart date between flag
     * @param int    $intCampaignId           : Campaign Id
     *
     * @return mixed
     */
    public function fnGetFMRViewsByCompanyChart($intCompanyOgId, $strMediaType, $strChartDateBetweenFlag = "0", $intCampaignId = 0)
    {
        if ($strChartDateBetweenFlag == "1") {
            $strChartDateBetween = " AND (datevisited BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "') ";
        }
        $strCampaignFrom = ''; //change
        if ((int)$intCampaignId != 0) {
            $strCampaignFrom = "  JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = n.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        } else {
            $strCondition = '';
            if ($strMediaType != '') {
                if ($strMediaType != "all") {
                    $strCondition .= " AND t.field_fmr_type_of_content_value = '$strMediaType' ";
                } else {
                    $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')";
                }
            }
        }
        $strViewChatSql = "SELECT SUM(v.totalcount) as totalcount, v.datevisited as datevisit
									  FROM " . $this->strViewTableName . " AS v
									  JOIN node AS n ON n.nid = v.nid
									  JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
									  JOIN field_data_field_fmr_type_of_content t ON n.nid = t.entity_id
									  $strCampaignFrom
									  WHERE v.company_ogid = $intCompanyOgId AND n.status = 1
									  AND pubdt.field_fmr_date_time_value >= DATE_SUB( '" . $this->strCurrentDate . "' , INTERVAL 180 DAY )
									  $strChartDateBetween
									  $strCondition
									  GROUP BY datevisited
									  ORDER by datevisited asc";
        return $arrayViewChartData = db_query($strViewChatSql)->fetchAll();
    }

    /**
     * Query for getting all the clicks amd clicks count by company id  and by media-type we get count of particular media
     *
     * @param int    $intCompanyOgId : Company OG Id
     * @param string $strMediaType   : Media Type
     * @param int    $intCampaignId  : CampaignId
     * @param string $strVideo       : Flag to check video or all type
     *
     * @return mixed
     */
    public function fnGetFMRClicksByCompany($intCompanyOgId, $strMediaType = '', $intCampaignId = 0, $strVideo = "all")
    {
        $strSelect = " SELECT COUNT(C.nid) AS clickcount, fmr_type, COUNT(DISTINCT(C.nid)) AS fmrcount ";

        $strFrom = " FROM " . $this->strClickTableName . " AS C
                     JOIN node AS N ON C.nid = N.nid
                     JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = N.nid
                     JOIN field_data_field_fmr_type_of_content t ON N.nid = t.entity_id ";

        $strWhere = " WHERE C.company_ogid = $intCompanyOgId and
                      N.status = 1 AND pubdt.field_fmr_date_time_value >= DATE_SUB( '" . $this->strCurrentDate . "' , INTERVAL 180 DAY ) ";

        $strCampaignFrom = ''; //change
        if ((int)$intCampaignId > 0) {
            $strCampaignFrom = "  JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = N.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        } else {
            $strCondition = '';
            if ($strMediaType != '') {
                //getting particular media-type condition for clicks.
                if ($strMediaType == "all") {
                    $strCondition .= " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter')";
                } else {
                    $strCondition .= " AND C.fmr_type ='" . $strMediaType . "' GROUP BY C.fmr_type ";
                }
            } else {
                $strCondition .= " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter') GROUP BY C.fmr_type ";
            }

        }
        if ($strVideo == "all") {
            $strSQL = $strSelect . $strFrom . $strCampaignFrom . $strWhere . $strCondition;
            $arrayClickData = db_query($strSQL)->fetchAll();
            ## For test put 2013-01-18 .prev '".$this->strCurrentDate."'
        } elseif ($strVideo == "NO") {
            //Collect all FMRs With Videos
            $strFMRIds = $this->fnGetFMRWithVideo($intCompanyOgId);

            //Get the Non Videos FMRs
            $strQry = $strSelect . $strFrom . $strCampaignFrom . $strWhere . " AND N.nid NOT IN ($strFMRIds) " . $strCondition;
            $arrayClickData = db_query($strQry)->fetchAll();
        }
        return $arrayClickData;
    }

    /**
     * Query for getting Click Chart for First Level and Second Level
     *
     * @param int    $intCompanyOgId               : Company OG Id
     * @param string $strMediaType                 : Media Type
     * @param string $strClickChartDateBetweenFlag : Click Chart Date Between Flag
     * @param int    $intCampaignId                : Campaign Id
     *
     * @return mixed
     */
    public function fnGetFMRClicksByCompanyChart($intCompanyOgId, $strMediaType, $strClickChartDateBetweenFlag = "0", $intCampaignId = 0)
    {
        $strClickChartDateBetween = '';
        if ($strClickChartDateBetweenFlag == "1") {
            $strClickChartDateBetween = " AND (click_date BETWEEN '" . $this->strChartPrevDate . "' AND '" . $this->strCurrentDate . "') ";
        }
        $strCampaignFrom = '';
        if ((int)$intCampaignId > 0) {
            $strCampaignFrom = " JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = N.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        } else {
            $strCondition = '';
            if ($strMediaType != '') {
                //getting particular media-type condition for clicks.
                if ($strMediaType == "all") {
                    $strCondition .= " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter') ";
                } else {
                    $strCondition .= " AND C.fmr_type ='" . $strMediaType . "'";
                }
            }
        }
        $strViewChartDataSql = "SELECT DATE_FORMAT(C.click_date, '%Y-%m-%d') as clickdate, count(C.nid) as totalcount
								          FROM " . $this->strClickTableName . " as C
									  	  JOIN node as N ON C.nid = N.nid
									  	  JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = N.nid
									  	  JOIN field_data_field_fmr_type_of_content t ON N.nid = t.entity_id
										  $strCampaignFrom
								          WHERE C.company_ogid = $intCompanyOgId
								          AND N.status = 1
										  AND pubdt.field_fmr_date_time_value >= DATE_SUB( '" . $this->strCurrentDate . "' , INTERVAL 180 DAY )
								          $strClickChartDateBetween
										  $strCondition
								          GROUP BY  DATE_FORMAT(click_date, '%Y/%m/%d')
								          ORDER by click_date asc";
        return $arrayViewChartData = db_query($strViewChartDataSql)->fetchAll();
    }

    /**
     * Getting Particular Media Type for Views
     *
     * @param int    $intCompanyOgId : Company OG Id
     * @param string $strMediaType   : Media Type
     * @param int    $boolReport     : Boolean Flag
     * @param int    $intCampaignId  : CampaignId
     *
     * @return mixed
     */
    public function fnGetFMRViewCountByMediaType($intCompanyOgId, $strMediaType = "", $boolReport = 0, $intCampaignId = 0)
    {
        if ((int)$intCampaignId != 0) {
            $strCnd = " AND CN.nid='" . $intCampaignId . "' ";
        } elseif ($strMediaType != "all") {
            $strCnd = " AND t.field_fmr_type_of_content_value = '$strMediaType'";
        } else {
            $strCnd = " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')";
        }
        $strSelect = " SELECT N.nid as nid, N.title as title, SUM(v.totalcount) as viewcount, v.datevisited as datevisit,
	                 pubdt.field_fmr_date_time_value as publishdate ";

        $strFrom = " FROM " . $this->strViewTableName . " AS v
						JOIN node AS N ON N.nid = v.nid
						JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = N.nid
						JOIN field_data_field_fmr_type_of_content t ON N.nid = t.entity_id";

        $strWhere = " Where v.company_ogid  = $intCompanyOgId AND N.status = 1 AND
                        pubdt.field_fmr_date_time_value >= DATE_SUB( '" . $this->strCurrentDate . "' , INTERVAL 180 DAY )
                        " . $strCnd;

        if ($boolReport == 1) {
            $strSelect .= " , t.field_fmr_type_of_content_value as type, CN.title as campaign ";
            $strFrom .= " LEFT JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = N.nid
                            LEFT JOIN node AS CN ON CN.nid = COM.field_fmr_campaign_nid ";

            $strReportGroupBy = " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')
			                      GROUP BY N.nid ORDER BY pubdt.field_fmr_date_time_value DESC ";
        }

        $strSql = $strSelect . $strFrom . $strWhere . $strReportGroupBy;

        if ($boolReport == 0) {
            $strSql .= " GROUP BY N.nid ORDER BY pubdt.field_fmr_date_time_value DESC ";
        }
        $ViewTypeResult = db_query($strSql)->fetchAll();

        return $ViewTypeResult;
    }

    /**
     * Getting Particular Media Type for Clicks.
     *
     * @param int    $intCompanyOgId : Company OG Id
     * @param string $strMediaType   : Media Tpe
     * @param string $intCampaignId  : Campaign Id
     * @param int    $intGroupBy     : Flag to Get group by result
     * @param string $strViewNid     : Node Id
     *
     * @return mixed
     */
    public function fnGetFMRClickCountByMediaType($intCompanyOgId, $strMediaType = '', $intCampaignId = '', $intGroupBy = 0, $strViewNid = '')
    {
        if ($intCampaignId != '') {
            $strCndClk = " ";
        } elseif ($strMediaType != "all") {
            $strCndClk = " AND C.fmr_type ='" . $strMediaType . "'";
        } elseif ($strMediaType == "all") {
            $strCndClk = " AND C.fmr_type IN ('press_release','blog','multimedia','article','newsletter') ";
        }

        $strSql = "SELECT count(C.nid) as clickcount,C.nid as nid,C.click_date as datevisit,N.title as title
					FROM " . $this->strClickTableName . " as C
					JOIN node as N ON N.nid = C.nid
					JOIN field_data_field_fmr_type_of_content t ON N.nid = t.entity_id
					WHERE C.company_ogid = $intCompanyOgId ";

        $strSql .= $strCndClk;

        if ($intGroupBy == 1) {
            $strSql .= "  AND N.nid IN($strViewNid)
                           Group by N.nid ORDER by nid DESC ";
        }
        return $ClickTypeResult = db_query($strSql)->fetchAll();
    }


    /**
     * Getting  Email address
     *
     * @param string $strCompanyNid : Company Node Id
     * @param int    $intMonth      : Flag to get result from particular table
     *
     * @return array
     */
    public function fnGetAutoReportEmail($strCompanyNid = "", $intMonth = 0)
    {
        $arrAutoReportEmail = array();
        if ($strCompanyNid != "") {
            if ($intMonth == 0) {
                $strSql = "SELECT field_email_for_automated_report_email AS email, entity_id AS nid, delta
                   FROM field_data_field_email_for_automated_report
                   WHERE entity_id IN (" . $strCompanyNid . ") ORDER BY nid";
            } else {
                $strSql = "SELECT field_email_for_automated_month_email AS email, entity_id AS nid, delta
                   FROM field_data_field_email_for_automated_month
                   WHERE entity_id IN (" . $strCompanyNid . ") ORDER BY nid";
            }
            $objAutoReportEmail = db_query($strSql)->fetchAll();
            if (is_array($objAutoReportEmail) && count($objAutoReportEmail) > 0) {
                foreach ($objAutoReportEmail as $arrRow) {
                    $arrAutoReportEmail[$arrRow->nid][$arrRow->delta] = $arrRow->email;
                }
            }
        }
        return $arrAutoReportEmail;
    }

    /**
     * Get Latest FMR
     *
     * @param int $intCompanyId : Company Id
     *
     * @return array
     */
    public function fnGetCronLatestFMR($intCompanyId = 0)
    {
        $arrLatestFMR = array();
        if ((int)$intCompanyId > 0) {
            $strSql = "SELECT DISTINCT n.nid
             FROM  og
             JOIN og_membership ogm ON (og.gid = ogm.gid AND ogm.entity_type = 'node')
             JOIN node AS n ON n.nid = ogm.etid
             JOIN field_data_field_fmr_date_time AS d ON d.entity_id = n.nid
             WHERE n.status = 1 AND n.type ='fmr' AND og.etid = $intCompanyId
             AND (d.field_fmr_date_time_value < DATE_SUB( '" . $this->strCurrentDate . "', INTERVAL 8 DAY) AND d.field_fmr_date_time_value >= DATE_SUB( '" . $this->strCurrentDate . "', INTERVAL 15 DAY))
             ORDER BY d.field_fmr_date_time_value DESC";

            $objLatestFMR = db_query($strSql)->fetchAll();
            //fetch all FMRs
            if (!empty($objLatestFMR)) {
                foreach ($objLatestFMR as $arrRow) {
                    $arrLatestFMR[] = $arrRow->nid;
                }
            }
        }
        return $arrLatestFMR;
    }

    /**
     * Getting active company records.
     *
     * @return array
     */
    public function fnGetActiveCompanies()
    {
        $strSql = " SELECT N.nid, og.gid
					FROM node AS N
					JOIN og ON og.etid= N.nid
					JOIN field_data_field_active_3bl_client_account AS ac ON N.nid = ac.entity_id
					JOIN field_data_field_email_for_automated_report AS ae ON ae.entity_id = N.nid
					WHERE N.status = 1 AND ac.field_active_3bl_client_account_value = 1 AND N.type = 'client'
					GROUP BY og.gid
					ORDER BY og.gid";
        $objCompany = db_query($strSql)->fetchAll();

        if (!empty($objCompany)) {
            foreach ($objCompany as $arrRow) {
                $arrCompany[$arrRow->gid] = $arrRow->nid;
            }
        }
        return $arrCompany;
    }

    /**
     * Function to get the Justmeans newsletter total clicks for selected FMR
     *
     * @param int    $intNodeId     : Node Id
     * @param string $strChannelType: Channel Type (Jm-Newsletter, Justmeans)
     * @param string $strReportType : Report type (Weekly or Individual).
     *
     * @return mixed
     */
    public function fnGetJMClicks($intNodeId, $strChannelType, $strReportType = "")
    {
        // Group By Condition
        $strGroupBy = "";
        if ($strReportType == "weekly") {
            $strGroupBy = "GROUP BY N.nid_3bl";
        }
        $strPrimarySQL = "SELECT count(jc.mediaid) as click, N.nid_3bl
                          FROM  nid_to_nid as N
                          JOIN jm_analytic_clicks as jc on N.nid_jm = jc.mediaid
                          WHERE N.nid_3bl IN (" . $intNodeId . ")
                          AND jc.channel = '$strChannelType' $strGroupBy";
        $objJMClicks = db_query($strPrimarySQL)->fetchAll();

        $arrJMClicksInfo = array();
        //Weekly Justmeans FMRs clicks
        if ($strReportType == "weekly") {
            if (!empty($objJMClicks)) {
                foreach ($objJMClicks as $arrJMClicks) {
                    $arrJMClicksInfo[$arrJMClicks->nid_3bl]["JMClicks"] = $arrJMClicks->click;
                }
            }
            return $arrJMClicksInfo;
        } else {
            //Individual Justmeans FMR clicks
            return $objJMClicks[0]->click;
        }
    }

    /**
     * Function to get the Justmeans newsletter total views for selected FMR
     *
     * @param int    $intNodeId     : Node Id
     * @param string $strChannelType: Channel Type (Jm-Newsletter, Justmeans)
     * @param string $strReportType : Report type (Weekly or Individual).
     *
     * @return mixed
     */
    public function fnGetJMViews($intNodeId, $strChannelType, $strReportType = "")
    {
        // Group By Condition
        $strGroupBy = "";
        if ($strReportType == "weekly") {
            $strGroupBy = "GROUP BY N.nid_3bl";
        }
        $strPrimarySQL = "SELECT sum(jv.totalcount) as views, N.nid_3bl
                          FROM  nid_to_nid as N
                          JOIN jm_analytic_views as jv on N.nid_jm = jv.mediaid
                          WHERE N.nid_3bl IN(" . $intNodeId . ")
                          AND jv.channel = '$strChannelType' $strGroupBy";
        $objJMViews = db_query($strPrimarySQL)->fetchAll();

        $arrJMViewsInfo = array();
        //Weekly Justmeans FMRs views
        if ($strReportType == "weekly") {
            if (!empty($objJMViews)) {
                foreach ($objJMViews as $arrJMViews) {
                    $arrJMViewsInfo[$arrJMViews->nid_3bl]["JMViews"] = $arrJMViews->views;
                }
            }
            return $arrJMViewsInfo;
        } else {
            //Individual Justmeans FMR views
            return $objJMViews[0]->views;
        }
    }

    /**
     * Function for Getting Campaign Name
     *
     * @param int $intCampaignId : Campaign Id
     *
     * @return string
     */
    function fnGetCampaignName($intCampaignId = 0)
    {
        //Get Campaign Name from campaign id
        $strCampaignName = '';
        if ((int)$intCampaignId > 0) {
            $strCampaignName = db_query("SELECT title FROM node WHERE nid = '" . $intCampaignId . "'")->fetchField();
        }
        return $strCampaignName;
    }

    /**
     * Function fnGetFMRAnalyticView to get analytic views
     *
     * @param int    $intFMRIds    : FMR id
     * @param string $strReportType: Report type (Weekly or Individual).
     *
     * @return array
     */
    function fnGetFMRAnalyticView($intFMRIds, $strReportType = "")
    {
        //Group By condition
        $strGroupBy = "GROUP BY v.channel";
        if ($strReportType == "weekly") {
            $strGroupBy .= ", v.nid";
        }
        $strSQL = " SELECT SUM(v.totalcount) AS viewcount, v.nid, v.channel
                    FROM " . $this->strViewTableName . " AS v WHERE v.nid IN(" . $intFMRIds . ") $strGroupBy";
        $objFMRViewInfo = db_query($strSQL)->fetchAll();

        $arrAnalyticData = array();
        $arrAnalyticData['3blmedia']['view'] = '';
        $arrAnalyticData['3bl_widgets']['view'] = '';
        $arrAnalyticData['click_cron']['view'] = '';
        if (count($objFMRViewInfo) > 0) {
            //Weekly FMRs views
            if ($strReportType == "weekly") {
                foreach ($objFMRViewInfo as $arrFMRData) {
                    if ($arrFMRData->channel == '3blmedia') {
                        $arrAnalyticData[$arrFMRData->nid]['3blmedia']['view'] = $arrFMRData->viewcount;
                    } elseif ($arrFMRData->channel == '3bl_widgets') {
                        $arrAnalyticData[$arrFMRData->nid]['3bl_widgets']['view'] = $arrFMRData->viewcount;
                    } elseif ($arrFMRData->channel == 'click_cron') {
                        $arrAnalyticData[$arrFMRData->nid]['click_cron']['view'] = $arrFMRData->viewcount;
                    }
                }
            } else {
                //Individual FMR views
                foreach ($objFMRViewInfo as $arrFMRData) {
                    if ($arrFMRData->channel == '3blmedia') {
                        $arrAnalyticData['3blmedia']['view'] = $arrFMRData->viewcount;
                    } elseif ($arrFMRData->channel == '3bl_widgets') {
                        $arrAnalyticData['3bl_widgets']['view'] = $arrFMRData->viewcount;
                    } elseif ($arrFMRData->channel == 'click_cron') {
                        $arrAnalyticData['click_cron']['view'] = $arrFMRData->viewcount;
                    }
                }

                if ($arrAnalyticData['3blmedia']['view'] == '') {
                    $arrAnalyticData['3blmedia']['view'] = 0;
                } elseif ($arrAnalyticData['3bl_widgets']['view'] == '') {
                    $arrAnalyticData['3bl_widgets']['view'] = 0;
                } elseif ($arrAnalyticData['click_cron']['view'] == '') {
                    $arrAnalyticData['click_cron']['view'] = 0;
                }
            }
        }
        return $arrAnalyticData;
    }

    /**
     * Function fnGetFMRAnalyticClicks to get the Analytic clicks.
     *
     * @param int    $intFMRIds    : FMR id.
     * @param string $strReportType: Report type (Weekly or Individual).
     *
     * @return array
     */
    function fnGetFMRAnalyticClicks($intFMRIds, $strReportType = "")
    {
        //Group By condition
        $strGroupBy = "GROUP BY C.channel";
        if ($strReportType == "weekly") {
            $strGroupBy .= ", C.nid";
        }
        $strSQL = " SELECT COUNT(C.nid) AS clickcount, C.nid, C.channel
                    FROM " . $this->strClickTableName . " AS C WHERE C.nid IN(" . $intFMRIds . ") $strGroupBy";
        $objFMRClickInfo = db_query($strSQL)->fetchAll();

        $arrAnalyticData = array();
        $arrAnalyticData['3blmedia']['click'] = '';
        $arrAnalyticData['3bl_widgets']['click'] = '';
        $arrAnalyticData['click_cron']['click'] = '';
        if (count($objFMRClickInfo) > 0) {
            //Weekly FMRs clicks
            if ($strReportType == "weekly") {
                foreach ($objFMRClickInfo as $arrFMRData) {
                    if ($arrFMRData->channel == '3bl') {
                        $arrAnalyticData[$arrFMRData->nid]['3blmedia']['click'] = $arrFMRData->clickcount;
                    } elseif ($arrFMRData->channel == '3bl_widgets') {
                        $arrAnalyticData[$arrFMRData->nid]['3bl_widgets']['click'] = $arrFMRData->clickcount;
                    } elseif ($arrFMRData->channel == 'click_cron') {
                        $arrAnalyticData[$arrFMRData->nid]['click_cron']['click'] = $arrFMRData->clickcount;
                    }
                }

            } else {
                //Individual FMR clicks
                foreach ($objFMRClickInfo as $arrFMRData) {
                    if ($arrFMRData->channel == '3bl') {
                        $arrAnalyticData['3blmedia']['click'] = $arrFMRData->clickcount;
                    } elseif ($arrFMRData->channel == '3bl_widgets') {
                        $arrAnalyticData['3bl_widgets']['click'] = $arrFMRData->clickcount;
                    } elseif ($arrFMRData->channel == 'click_cron') {
                        $arrAnalyticData['click_cron']['click'] = $arrFMRData->clickcount;
                    }
                }

                if ($arrAnalyticData['3blmedia']['click'] == '') {
                    $arrAnalyticData['3blmedia']['click'] = 0;
                } elseif ($arrAnalyticData['3bl_widgets']['click'] == '') {
                    $arrAnalyticData['3bl_widgets']['click'] = 0;
                } elseif ($arrAnalyticData['click_cron']['click'] == '') {
                    $arrAnalyticData['click_cron']['click'] = 0;
                }
            }
        }
        return $arrAnalyticData;
    }

    /**
     * Function for Getting click count For 3bl NewsLetter
     *
     * @param int    $intFMRId     : Fmr Id
     * @param string $strReportType: Report type (Weekly or Individual).
     *
     * @return int
     */
    public function fnGetFMRNewsletterClick($intFMRId, $strReportType = "")
    {
        //Group By condition
        $strGroupBy = "";
        if ($strReportType == "weekly") {
            $strGroupBy = "GROUP BY C.contentID";
        }

        $intClickCount = 0;
        $strSQL = " SELECT COUNT(C.contentID) AS clickcount, C.contentID
                    FROM " . $this->strNewsletterClick . " AS C WHERE C.contentID IN(" . $intFMRId . ") AND contentType = 'fmr' $strGroupBy";

        $arrClickCount = array();
        //Weekly FMR 3bl newsletter clicks
        if ($strReportType == "weekly") {
            $objFMRClickInfo = db_query($strSQL)->fetchAll();
            if (!empty($objFMRClickInfo)) {
                foreach ($objFMRClickInfo as $arrFMRClickInfo)
                    $arrClickCount[$arrFMRClickInfo->contentID]["3bl_newsletter_click"] = $arrFMRClickInfo->clickcount;
            }
            return $arrClickCount;

        } else {
            //Individual FMR 3bl newsletter clicks
            $objFMRClickInfo = db_query($strSQL)->fetchAssoc();
            if (!empty($objFMRClickInfo)) {
                $intClickCount = $objFMRClickInfo['clickcount'];
            }
            return $intClickCount;
        }
    }

    /**
     * Function fnGetMicroListClickCount
     *
     * @param int $intFMRId : FMR id
     *
     * @return array
     */
    function fnGetMicroListClickCount($intFMRId)
    {
        $strSql = " (
                    SELECT count( tr.fmr_nid ) AS totalview, eck.type AS typ, eck.title, eck.id, tr.tracking_flag
                    FROM threebl_microlist_tracking AS tr
                    JOIN eck_micro_list AS eck ON eck.id = tr.list_id
                    WHERE tr.fmr_nid =" . $intFMRId . "
                    AND tr.tracking_flag = 'open'
                    GROUP BY tr.list_id
                    )
                    UNION
                    (
                    SELECT count( tr.fmr_nid ) AS totalClick, eck.type AS typ, eck.title, eck.id, tr.tracking_flag
                    FROM threebl_microlist_tracking AS tr
                    JOIN eck_micro_list AS eck ON eck.id = tr.list_id
                    WHERE tr.fmr_nid =" . $intFMRId . "
                    AND tr.tracking_flag = 'click'
                    GROUP BY tr.list_id
                    ) ORDER BY typ";
        $arrViewCount = db_query($strSql)->fetchAll();
        $arrMicroListDetails = array();
        if (count($arrViewCount) > 0) {
            foreach ($arrViewCount as $arrListInfo) {
                if ($arrListInfo->typ == '3bl_micro_list') {
                    $arrMicroListDetails['3bl_micro_list'][$arrListInfo->id]['title'] = $arrListInfo->title;
                    $arrMicroListDetails['3bl_micro_list'][$arrListInfo->id][$arrListInfo->tracking_flag] = $arrListInfo->totalview;
                } elseif ($arrListInfo->typ == 'client_micro_list') {
                    $arrMicroListDetails['client_micro_list'][$arrListInfo->id]['title'] = $arrListInfo->title;
                    $arrMicroListDetails['client_micro_list'][$arrListInfo->id][$arrListInfo->tracking_flag] = $arrListInfo->totalview;
                }
            }
        }
        return $arrMicroListDetails;
    }

    /**
     * * Function for Secondary Category of FMRs
     *
     * @param $strPrimaryCategory
     * @param $strSecondCategory
     *
     * @return mixed
     */
    public function fnGetSecondaryCategory($strPrimaryCategory, $strSecondCategory)
    {
        $objFMRPrimaryCategory = '';
        $objFMRSecondaryCategory = '';

        if ($strPrimaryCategory != '') {
            $strPrimarySQL = "SELECT distinct name
                                   From taxonomy_term_data WHERE tid
                                   IN ($strPrimaryCategory)";
            $objFMRPrimaryCategory = db_query($strPrimarySQL)->fetchAll();
        }


        if ($strSecondCategory != '') {
            $strSecondarySQL = "SELECT distinct name
                                     From taxonomy_term_data
                                     WHERE tid IN ($strSecondCategory)";
            $objFMRSecondaryCategory = db_query($strSecondarySQL)->fetchAll();
        }
        return array('arrPrimaryCat' => $objFMRPrimaryCategory, 'arrSecondCat' => $objFMRSecondaryCategory);
    }




    /**
     * Function to get FMRs information distributed in the previous 30 or 7 days
     *
     * @param int    $intCalDays   : Number of days
     * @param string $strReportType: Report type (Weekly or Individual).
     *
     * @return array
     */
    function fnGetLastPublishedFMRInfo($intCalDays = 30, $strReportType = "")
    {
        // Current Date
        if ($strReportType == "weekly") {
            $strCurrentDate = date("Y-m-d", strtotime($this->strCurrentDate . "- 1 days"));
        } else {
            $strCurrentDate = $this->strCurrentDate;
        }

        //Get date of Previous month
        $intCurrentYear = date("Y", strtotime($strCurrentDate));
        $intCurrentMonth = date("m", strtotime($strCurrentDate));
        $intCurrentDate = date("d", strtotime($strCurrentDate));
        $intPrevMonthDate = date('Y-m-d', mktime(0, 0, 0, $intCurrentMonth, $intCurrentDate - $intCalDays, $intCurrentYear));

        $strFMRDetails = "SELECT n.nid as fmrid, n.title, pubdt.field_fmr_date_time_value as publishdate, og.label as companyname, og.etid
                          FROM node n
                          JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                          JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                          JOIN og as og on og.gid = ga.group_audience_gid
                          WHERE n.status = 1
                          AND pubdt.field_fmr_date_time_value >=  '$intPrevMonthDate' AND pubdt.field_fmr_date_time_value <= '$strCurrentDate'
                          ORDER BY pubdt.field_fmr_date_time_value DESC";

        $arrFMRDetails = db_query($strFMRDetails)->fetchAll();

        $arrFMRInfo = array();
        if (is_array($arrFMRDetails) && count($arrFMRDetails) > 0) {

            foreach ($arrFMRDetails as $arrFMRValue) {
                $arrFMRInfo[$arrFMRValue->fmrid]['fmrid'] = (int) $arrFMRValue->fmrid;
                $arrFMRInfo[$arrFMRValue->fmrid]['title'] = stripslashes($arrFMRValue->title);
                $arrFMRInfo[$arrFMRValue->fmrid]['publishdate'] = $arrFMRValue->publishdate;
                $arrFMRInfo[$arrFMRValue->fmrid]['companyname'] = stripslashes($arrFMRValue->companyname);
                $arrFMRInfo[$arrFMRValue->fmrid]['companyid'] = (int) $arrFMRValue->etid;
                $arrFMRIds[] = (int) $arrFMRValue->fmrid;
            }
            //Implode FMR ids
            $strFMRIds = implode(', ', $arrFMRIds);
        }
        return array($arrFMRInfo, $strFMRIds, $strCurrentDate, $intPrevMonthDate);
    }

}