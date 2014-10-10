<?php
//This is class for Reports functions.
class ClsReports
{
    /**
     * Function return FMRs those have video attached with it.
     *
     * @param int $intCompanyOgId : Company OG Id
     *
     * @return array
     */
    public function fnGetFMRWithVideo($intCompanyOgId)
    {
        //Collect all FMRs With Videos
        $strFMRWithVideos = "SELECT DISTINCT(ga.entity_id) AS fmrIds
                    FROM field_data_group_audience AS ga
                    JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = ga.entity_id
                    JOIN field_data_field_video AS fmrv ON ( fmrv.bundle = 'fmr' AND fmrv.entity_id = ga.entity_id)
                    WHERE ga.group_audience_gid  =  $intCompanyOgId AND ga.bundle = 'fmr'
                    AND pubdt.field_fmr_date_time_value >= DATE_SUB('" . $this->strCurrentDate . "', INTERVAL 180 DAY )";

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
     * Function return all FMRs that get published from last 6 months
     *
     * @param int    $intCompanyOgId : Company OG Id
     * @param string $strMediaType   : FMR Media Type
     * @param string $intCampaignId  : Campaign Id
     * @param string $strIsArchiveFlag  : Flag to check condition for archive field
     * @param string $strFileType  : Flag to check file type (excel)
     *
     * @return array
     */
    public function fnGetCompanyFMRs($intCompanyOgId, $strMediaType = "", $intCampaignId = "", $strIsArchiveFlag = "", $strFileType = "")
    {
        $strSelect = "SELECT n.nid, t.field_fmr_type_of_content_value AS fmr_type ";

        $strCondition = '';
        $strCampaignFrom = ''; //change
        if ((int)$intCampaignId > 0) {
            $strSelect .= ", n.title as title, pubdt.field_fmr_date_time_value ";
            $strCampaignFrom = "  JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = n.nid AND COM.field_fmr_campaign_nid = '" . $intCampaignId . "' ";
        } else {
            if ($strMediaType != '') {
                if ($strMediaType != "all") {
                    $strCondition .= " AND t.field_fmr_type_of_content_value = '$strMediaType' ";
                } else {
                    $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter') ";
                }
            } else {
                $strCondition .= " AND t.field_fmr_type_of_content_value IN ('press_release','blog','multimedia','article','newsletter')  ";
            }
        }
        $strFrom = " FROM node AS n
                    JOIN field_data_group_audience AS ga ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                    JOIN field_data_field_fmr_date_time AS pubdt ON pubdt.entity_id = n.nid
                    JOIN field_data_field_fmr_type_of_content AS t ON n.nid = t.entity_id ";

        $strWhere = " WHERE n.status = 1 AND ga.group_audience_gid = $intCompanyOgId
                    AND pubdt.field_fmr_date_time_value >= DATE_SUB('" . $this->strCurrentDate . "', INTERVAL 180 DAY ) ";

        $strOrderBy = ($strMediaType == "") ? " ORDER BY t.field_fmr_type_of_content_value " : " ORDER BY pubdt.field_fmr_date_time_value DESC ";

        if ($strMediaType != "" && $intCampaignId == 0) {
            $strSelect .= ", n.title as title, pubdt.field_fmr_date_time_value, CN.title as campaign ";
            $strCampaignFrom .= " LEFT JOIN field_data_field_fmr_campaign as COM ON COM.entity_id = n.nid
                                  LEFT JOIN node AS CN ON CN.nid = COM.field_fmr_campaign_nid ";
        }

        //Get Primary Categories Assigned to FMRs
        if ($strMediaType != "" || (int)$intCampaignId >0) {
            $strSelect .= ", td.name AS primarycayegory, av.field_fmr_average_views_value as benchmark_view, ac.field_fmr_average_clicks_value as benchmark_click";
            $strCampaignFrom .= " LEFT JOIN field_data_field_fmr_primary_category AS prim ON n.nid = prim.entity_id
                                  LEFT JOIN taxonomy_term_data AS td ON td.tid = prim.field_fmr_primary_category_target_id
                                  LEFT JOIN field_data_field_fmr_average_views AS av ON n.nid = av.entity_id
                                  LEFT JOIN field_data_field_fmr_average_clicks AS ac ON n.nid = ac.entity_id ";
        }

        // select archive field
        $strArchiveFlag = "JOIN field_data_field_dist_archive AS ar ON n.nid = ar.entity_id";
        if ($strIsArchiveFlag == "non_archive") {
            // Get Non archive FMRs
            $strWhere .= " AND ar.field_dist_archive_value = 0";
        } elseif ($strIsArchiveFlag == "archive") {
            $strSelect .= ", ar.field_dist_archive_value AS is_archive";
        }

        #Final SQL
        $strQry = $strSelect.$strFrom.$strCampaignFrom.$strArchiveFlag.$strWhere.$strCondition.$strOrderBy;
        $objFMRNidInfo = db_query($strQry)->fetchAll();

        $arrFMRIds = array();
        $arrFMRInfo = array();
        $strFMRIds = '';

        if (!empty($objFMRNidInfo)) {

            if ((int)$intCampaignId == 0 && $strMediaType == "") {
                foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                    $arrFMRInfo[$arrFMRNidInfo->fmr_type][] = $arrFMRNidInfo->nid;
                    $arrFMRIds[] = $arrFMRNidInfo->nid;
                }
            } elseif ((int)$intCampaignId > 0 || $strMediaType != "all") {
                foreach ($objFMRNidInfo as $arrFMRNidInfo) {
                    $arrFMRInfo[$arrFMRNidInfo->nid]['nid'] = $arrFMRNidInfo->nid;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['publishdate'] = $arrFMRNidInfo->field_fmr_date_time_value;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['campaign'] = $arrFMRNidInfo->campaign;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['media'] = $arrFMRNidInfo->fmr_type;
                    $arrFMRInfo[$arrFMRNidInfo->nid]['primary_category'] = $arrFMRNidInfo->primarycayegory;

                    $arrFMRInfo[$arrFMRNidInfo->nid]['title'] = $arrFMRNidInfo->title;
                    if ($strFileType == "excel") {
                        $arrFMRInfo[$arrFMRNidInfo->nid]['title'] = $this->fnRemoveWordFormatting($arrFMRNidInfo->title);
                    }

                    //getting Benchmark views and clicks
                    $strFMRPublishedDate = date("Y-m-d", strtotime($arrFMRNidInfo->field_fmr_date_time_value));
                    if (strtotime($strFMRPublishedDate) <= strtotime($this->strBenchmarkNAMonth)) {
                        // Set N/A value for Benchmark views and clicks, if FMR published before Apr 2014
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_view'] = "N/A";
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_click'] = "N/A";
                    } else {
                        // Set value for Benchmark views and clicks
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_view'] = ((int) $arrFMRNidInfo->benchmark_view > 0) ? number_format((int) $arrFMRNidInfo->benchmark_view) : "-";
                        $arrFMRInfo[$arrFMRNidInfo->nid]['benchmark_click'] = ((int) $arrFMRNidInfo->benchmark_click > 0) ? number_format((int) $arrFMRNidInfo->benchmark_click) : "-";
                    }

                    $arrFMRInfo[$arrFMRNidInfo->nid]['is_archive'] = $arrFMRNidInfo->is_archive;
                }
                $arrFMRIds = array_keys($arrFMRInfo);
            }

            //Implode FMR ids
            $strFMRIds = implode(', ', $arrFMRIds);
        }
        return array($arrFMRInfo, $arrFMRIds, $strFMRIds);
    }

    /**
     * Function fetch FMR views information
     *
     * @param string $strFMRIds : Comma separated FMR Ids
     *
     * @return array
     */
    public function fnGetViews($strFMRIds)
    {
        $strSQL = " SELECT SUM(v.totalcount) AS viewcount, v.nid
                    FROM " . $this->strViewTableName . " AS v WHERE v.nid IN ($strFMRIds) GROUP BY v.nid";
        $objFMRViewInfo = db_query($strSQL)->fetchAll();

        $arrFMRInfo = array();

        //Checking is empty
        if (!empty($objFMRViewInfo)) {
            # Get FMR view count
            foreach ($objFMRViewInfo as $objArrRow) {
                $arrFMRInfo[$objArrRow->nid] = $objArrRow->viewcount;
            }
        }
        return $arrFMRInfo;
    }

    /**
     * Function fetch FMR Clicks information
     *
     * @param string $strFMRIds : Comma separated FMR Ids
     *
     * @return array
     */
    public function fnGetClicks($strFMRIds)
    {
        $strSQL = " SELECT COUNT(C.nid) AS clickcount, C.nid
                    FROM " . $this->strClickTableName . " AS C WHERE C.nid IN ($strFMRIds) GROUP BY C.nid";
        $objFMRViewInfo = db_query($strSQL)->fetchAll();

        $arrFMRInfo = array();

        //Checking is empty
        if (!empty($objFMRViewInfo)) {
            # Get FMR Click count
            foreach ($objFMRViewInfo as $objArrRow) {
                $arrFMRInfo[$objArrRow->nid] = $objArrRow->clickcount;
            }
        }
        return $arrFMRInfo;
    }

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
     * Get all Vertical List
     *
     * @param string $strNid : Node Id
     *
     * @return array
     */
    public function fnFMRVerticals($strNid)
    {
        $arrVerticals = array();
        if ($strNid != "") {
            $strSQL = "SELECT vert.entity_id AS fmrId, ver.name AS Verticals
             FROM field_data_field_dist_vertical AS vert
             LEFT JOIN taxonomy_term_data AS ver ON ver.tid = vert.field_dist_vertical_target_id
             WHERE vert.entity_id IN (" . $strNid . ")";
            $objVerticals = db_query($strSQL)->fetchAll();
            if (count($objVerticals) > 0) {
                foreach ($objVerticals as $arrRow) {
                    $arrVerticals[$arrRow->fmrId][] = $arrRow->Verticals;
                }
            }
        }
        return $arrVerticals;
    }

    /**
     * Google Map for Click Count for Second Level and Third Level
     *
     * @param int $intCompanyOgId : Company Og Id
     * @param int $intMediaId     : Media Id
     *
     * @return mixed
     */
    public function fnGetFMRClicksByCountryGoogleMap($intCompanyOgId, $intMediaId)
    {
        $strMapDataSql = "SELECT nid,click_ip,count(nid) as ipcount,country
									  FROM " . $this->strClickTableName . " as C
									  JOIN field_data_field_fmr_type_of_content t ON C.nid = t.entity_id
									  WHERE C.company_ogid = $intCompanyOgId
									  AND C.nid = $intMediaId
									  GROUP BY UPPER(country) order by ipcount desc ";
        return $arrayClickMapData = db_query($strMapDataSql)->fetchAll();
    }

    /**
     * Get Referrer website for Second Level and Third Level
     *
     * @param int $intCompanyOgId : Company Og Id
     * @param int $intMediaId     : Media Id
     *
     * @return mixed
     */
    public function fnGetFMRReferrerClicksByMediaType($intCompanyOgId, $intMediaId)
    {
        // remove extra website.
        $arrExtraLink = array('bestgenericviagra.biz', 'blog.pnarp.com', 'diffbot.com', 'timberland.justmeans.com', 'www.mysmutsearch.com', 'lala123.com', '_bit=528e713d-', '_bit=52af68a5-00111-07550', 'piwik.3blmedia.com', 'stats.3blmedia.com');

        $strExtraLink = implode("|", $arrExtraLink);
        $strCondition = " AND C.nid = $intMediaId
                          AND click_referrer NOT RLIKE '$strExtraLink'
                          GROUP BY click_referrer Order by click_date desc ";
        $strReferSql = "SELECT distinct click_referrer as rlink
								   FROM " . $this->strClickTableName . " as C
								   JOIN field_data_field_fmr_type_of_content t ON C.nid = t.entity_id
								   WHERE C.company_ogid = $intCompanyOgId
								   AND C.click_referrer <> '' $strCondition";
        return $arrayClickReferData = db_query($strReferSql)->fetchAll();
    }

    /**
     * Getting View-Count By Media-Id
     *
     * @param int  $intCompanyOgId : Company OG Id
     * @param int  $intMediaId     : Media Id
     * @param bool $boolReport     : Boolean Flag
     *
     * @return mixed
     */
    public function fnGetFMRViewCountByMediaId($intCompanyOgId, $intMediaId, $boolReport)
    {
        $strSql = "SELECT  SUM(v.totalcount) as totalcount
              FROM " . $this->strViewTableName . " as v
              Where  v.company_ogid = $intCompanyOgId AND v.nid  IN ($intMediaId)
              GROUP BY v.nid";
        return $arrViewCount = ($boolReport == 0) ? db_query($strSql)->fetchAssoc() : db_query($strSql)->fetchAll();
    }

    /**
     * Getting Click-Count By Media-Id
     *
     * @param int  $intCompanyOgId : Company OG Id
     * @param int  $intMediaId     : Media Id
     * @param bool $boolReport     : Boolean Flag
     *
     * @return mixed
     */
    public function fnGetFMRClickCountByMediaId($intCompanyOgId, $intMediaId, $boolReport)
    {
        $strSql = "SELECT C.nid, C.click_date, count(C.nid) as totalcount, t.field_fmr_type_of_content_value AS `type`
              FROM " . $this->strClickTableName . " as C
              JOIN field_data_field_fmr_type_of_content t ON C.nid = t.entity_id
              Where C.company_ogid = $intCompanyOgId
              AND C.nid IN ($intMediaId)
              GROUP BY nid";
        return $arrClickCount = ($boolReport == 0) ? db_query($strSql)->fetchAssoc() : db_query($strSql)->fetchAll();
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
     * Function Create PDF and send email to respective company
     *
     * @param int    $intCompanyNid  : Company Node Id
     * @param int    $intCompanyOgId : Company Og Id
     * @param array  $arrRecipient   : Recipients email array
     * @param string $strMediaIds    : Comma separated Media Id
     * @param int    $intMonth
     *
     * @return bool|DatabaseStatementInterface|int|null
     */
    public function fnReportsByCompany($intCompanyNid, $intCompanyOgId, $arrRecipient, $strMediaIds, $intMonth = 0)
    {
        //Define all required variables
        global $base_url, $conf;
        $strTime = time();
        $strServerName = $_SERVER['HTTP_HOST'];
        $strSender = "info@3blmedia.com";
        $strSenderName = "3blmedia Team";
        $strHeader = $base_url . "/Dashboard/Report/Views/pdfhtml/header";
        $strFooter = $base_url . "/Dashboard/Report/Views/pdfhtml/footer";

        //if month report then
        if ((int)$intMonth == 1) {
            ## Analytics Summary Report Attachment
            $strUrl = http_build_query(array('client_og' => $intCompanyOgId, 'client_og_nid' => $intCompanyNid));
            #HTML file path
            $strSrcMCSR = $base_url . "/Dashboard/Report/Views/" . base64_encode($strUrl);

            //Create PDF file
            exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrcMCSR . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Analytics_Summary_Report_" . $intCompanyNid . $strTime . ".pdf");

            //Created PDF file Path
            $mixFMRDetailReport = "/var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Analytics_Summary_Report_" . $intCompanyNid . $strTime . ".pdf";

        } else {
            $mixFMRDetailReport = array();

            ## FMR Detail Report Attachment
            if ($strMediaIds != '') {
                //getting multiple media-id
                $arrMediaIds = explode(",", $strMediaIds);
                //Foreach
                foreach ($arrMediaIds as $val) {
                    $strUrl1 = http_build_query(array('client_og' => $intCompanyOgId, 'client_og_nid' => $intCompanyNid, 'intMediaId' => $val, 'cronRpt' => true));
                    $strSrcFMR = $base_url . "/Dashboard/Report/Views/CronMediaId/" . base64_encode($strUrl1);

                    //Create PDF file
                    exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet  --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrcFMR . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/FMR_Detail_Report_" . $val . "_" . $intCompanyNid . $strTime . ".pdf");
                    //Created PDF file Path array
                    $mixFMRDetailReport[] = "/var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/FMR_Detail_Report_" . $val . "_" . $intCompanyNid . $strTime . ".pdf";

                }
            }
        }

        $strHeaderImagePath = $base_url."/".$conf['IMAGES_PATH_3BL']."/ConfirmHeader.png";
        $strFooterImagePath = $base_url."/".$conf['IMAGES_PATH_3BL']."/ConfirmFooter.png"; 

        $strHeaderHTML = "<div><img src='$strHeaderImagePath'/></div><br/>";
        $strFotterHTML = "<br/><br/><div><img src='$strFooterImagePath'/></div>";

        //Set Subject for report cron email on the basis of report type
        $strSubject = ((int)$intMonth == 1) ? "3BL Media: Monthly Analytics Summary Report" : "3BL Media: Weekly Activity Report";

        //Set the message for the email on the basis of report type
        $strMessage = ((int)$intMonth == 1) ? $strHeaderHTML."Attached you will find your up to date Analytics Summary Report from 3BL Media. This report is sent automatically once a month. You can review all of your 3BL Media analytics at any time at http://3blmedia.com/Dashboard/Analytics/Views".$strFotterHTML : $strHeaderHTML.'Attached you should find Flexible Media Release (FMR) Detail Reports for any content distributed between ' . date("F d, Y", mktime(0, 0, 0, date("m"), date("d") - 15, date("Y"))) . ' and ' . date("F d, Y", mktime(0, 0, 0, date("m"), date("d") - 9, date("Y"))) . '.  If you have any questions, please contact your media consultant at 866.508.0993 or reply to this message.<br/>Of course, you can review all of your 3BL Media Analytics and Tracking in your portal at any time at http://3blmedia.com/Dashboard/Analytics/Views<br/><br/>Thank you,<br/>The 3BL Media Team'.$strFotterHTML;

        //Set the test parameter true for test purpose
        $boolTest = false;
        //User list those who got report Email
        $arrReceivedReport = array();

        //Send email with attachments
        foreach ($arrRecipient as $strRecipientAddress) {
            $boolSent = fnSendEmail($strSenderName, $strSender, $strRecipientAddress, $strSubject, $strMessage, $mixFMRDetailReport, $boolTest);
            if ($boolSent) {
                $arrReceivedReport[] = $strRecipientAddress;
            }
        }
        //Delete file after email those attachment.
        if (is_array($mixFMRDetailReport)) {

            foreach ($mixFMRDetailReport as $key => $strFile) {
                @unlink($strFile);
            }

        } else {
            @unlink($mixFMRDetailReport);
        }

        $intInsertSuccess = false;
        if (count($arrReceivedReport) > 0) {
            //If mail sent successfuly then insert these details in the table
            $strRecipients = implode(",", $arrReceivedReport);
            $intInsertSuccess = $this->fnInsertCompanyDetails((int)$intMonth,$intCompanyNid,$strRecipients,'email');
        }
        return $intInsertSuccess;
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
     * Function insertCompanyDetails
     *
     * @param int    $intMonth                  : Flag to check report type
     * @param int    $intCompanyNid             : Company Node Id
     * @param string $strRecipient              : Recipient email
     * @param string $strDownloadReportFileType : Download Report File Type
     *
     * @return mixed
     */
    public function fnInsertCompanyDetails($intMonth, $intCompanyNid, $strRecipient, $strDownloadReportFileType = 'email')
    {
        //set the current date
        $dateCurrentDate = date("Y-m-d");
        $arrUpdate = array(
            'companyid' => $intCompanyNid,
            'email' => $strRecipient,
            'channel' => 'cron',
            'report_type' => ($intMonth ? 7 : 6),
            'download_file_type' => $strDownloadReportFileType,
            'send_on' => $dateCurrentDate
        );

        //Insert all details in the table.
        $intInsertId = db_insert('threebl_company_auto_report_tracking')
            ->fields($arrUpdate)
            ->execute();
        return $intInsertId;
    }

    /**
     * Function to insert 3BL report details
     *
     * @param int    $intCompanyId      : Company Id
     * @param string $strEmailIds       : Email Ids
     * @param string $strReportType     : Report type
     * @param string $strReportFileType : Report File type
     *
     * @return mixed
     */
    public function fnInsert3blReportDetails($intCompanyId, $strEmailIds, $strReportType, $strReportFileType)
    {
        //--------------------Details of parameters --------------------//
        //	$intCompanyId =   Company id                                //
        //	$strEmailIds = Email  id is available only for email option //
        //	$strReportType =                                            //
        //  1. BADF														//
        //  2. MCSR 													//
        //  3. CSMT  													//
        //  4. CSC														//
        //  5. FMR                                 						//
        //	$intEmailSendByOption = 									//
        //   1. pdf														//
        //   2. Excel													//
        //   3. Email 		                             				//
        //--------------------------- End ------------------------------//

        //set the current date
        $dateCurrentDate = date("Y-m-d");

        #Set the report Source to 3bl
        $strChannel = "3bl";

        //Insert all details in the table.
        $intInsertId = db_insert('threebl_company_auto_report_tracking')
            ->fields(array(
            'companyid' => $intCompanyId,
            'email' => $strEmailIds,
            'channel' => $strChannel,
            'report_type' => $strReportType,
            'download_file_type' => $strReportFileType,
            'send_on' => $dateCurrentDate
        ))->execute();
        return $intInsertId;
    }

    /**
     * Function return name of requested company id
     *
     * @param int $intCompanyNid : Company Node Id
     *
     * @return string
     */
    public function fnGetCompanyName($intCompanyNid = 0)
    {
        $strCompanyName = "";
        if ((int)$intCompanyNid > 0) {
            $strCompanyName = db_query("SELECT title FROM node WHERE nid = '" . $intCompanyNid . "'")->fetchField();
        }
        return $strCompanyName;
    }

    /**
     * Function sending email to all Company users those email id sink with report functionality.
     *
     * @param array $arrCompany                : Array of company Info
     * @param array $arrClientWeeklyEmailList  : Array of Client Weekly Email List
     * @param array $arrClientMonthlyEmailList : Array of Client Monthly Email List
     *
     * @return bool
     */
    public function fnSendMonthlyReport($arrCompany = array(), $arrClientWeeklyEmailList = array(), $arrClientMonthlyEmailList = array())
    {
        if (count($arrCompany) > 0) {
            $intCount = 0;
            //Fetch all email from company to send Monthly report.
            foreach ($arrCompany as $intCompanySynId => $intCompanyNid) {
                if ($intCount > 5) {
                    return true;
                }
                $arrEmails = array();
                //Get all weekly & Monthly emails for company
                $arrEmails = array_unique(array_merge_recursive($arrClientWeeklyEmailList[$intCompanyNid], $arrClientMonthlyEmailList[$intCompanyNid]));
                $arrEmails = array("himanshuc.cuelogic@gmail.com", "sankets.cuelogic@gmail.com", "prasanaa.cuelogic@gmail.com", "dfulton@3blmedia.com"); //Send email to all subscribers
                if (count($arrEmails) > 0) {
                    $this->fnReportsByCompany($intCompanyNid, $intCompanySynId, $arrEmails, '', 1); //
                    $intCount++;
                }
            }
        }
        return true;
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
     * @param array $arrFinalResult      : All information regarding channel like 3bl media, widget
     * @param array $arrAnalyticData     : Analytic data
     * @param array $arrMicroListDetails : Data of micro list details
     *
     * @return mixed
     */
    public function fnBuildHtmlFMRReport($arrFinalResult, $arrAnalyticData, $arrMicroListDetails)
    {
        $strHtml = "";
        $strCategoryHTML = '';
        $strSecondaryCategory = '-';
        $strPrimaryCategory = '-';

        //Checking array empty or not
        if (!empty($arrFinalResult['arrFMRCategory'])) {

            //Code for getting all Primary FMR Category
            if (!empty($arrFinalResult['arrFMRCategory']['arrPrimaryCat'])) {

                foreach ($arrFinalResult['arrFMRCategory']['arrPrimaryCat'] as $arrPCategory) {
                    //Category Name
                    $arrPrimaryCategory[] = $arrPCategory->name;
                }
            }

            //Code for getting all FMR Secondary Category
            if (!empty($arrFinalResult['arrFMRCategory']['arrSecondCat'])) {
                foreach ($arrFinalResult['arrFMRCategory']['arrSecondCat'] as $arrSCategory) {
                    ///Category Name
                    $arrSecondaryCategory[] = $arrSCategory->name;
                }
            }

            //Create Sting seprated by comma
            if (!empty($arrPrimaryCategory)) {
                $strPrimaryCategory = implode(",", $arrPrimaryCategory);
            }
            if (!empty($arrSecondaryCategory)) {
                $strSecondaryCategory = implode(",", $arrSecondaryCategory);
            }
        }

        //HTML of Category
        $strCategoryHTML = '
<div id="fmr-header" style="margin: 0 0 0.5em;padding: 1em 20px;border:1px solid">
    <div class="client-link" style="padding: 5px">
        <strong style="display: block;float: left;width: 150px;"> Primary Category:</strong> <span
        style="display: block;float: left;width:1037px;">' . $strPrimaryCategory . '
         </span>

        <div style="clear:both"></div>
    </div>
    <div class="client-link" style="padding: 5px">
        <strong style="display: block;float: left;width: 150px;"> Secondary Category:</strong> <span
        style="display: block;float: left;width:1037px;">' . $strSecondaryCategory . '
         </span>

        <div style="clear:both"></div>
    </div>
</div>';

        //Build the header section such as information about the FMR.
        $strHtml .= '
<div id="fmr-header" style="background: none repeat scroll 0 0 #E9EBED;margin: 0 0 0.5em;padding: 1em 20px;">
    <div class="client-name" style="padding: 5px">
        <a href="http://test2.3blmedia.com/node/' . $arrFinalResult['FMRID'] . '"><b><span style="font-size: 18px;">' . $arrFinalResult['title'] . '</span></b></a>
    </div>
    <div class="client-link" style="padding: 5px">
        Media Type: <b>' . ucwords($arrFinalResult['FMRTYPE']) . '</b> | <a
        href="http://test2.3blmedia.com/node/' . $arrFinalResult['CompUrl'] . '" target="blank">' .
            $arrFinalResult['CompName'] . '</a><br>

        <div style="margin-top: 10px">Date: ' . date('l, F d, Y', strtotime($arrFinalResult['published_date'])) . '
        </div>
    </div>
</div>' . $strCategoryHTML;

        //Build the html related to analytic data.
        $int3blmediaView = ($arrAnalyticData['3blmedia']['view'] > 0) ? $arrAnalyticData['3blmedia']['view'] : 0;
        $int3blmediaClick = ($arrAnalyticData['3blmedia']['click']) ? $arrAnalyticData['3blmedia']['click'] : 0;
        $intJMView = ($arrAnalyticData['jm']['view'] > 0) ? $arrAnalyticData['jm']['view'] : 0;
        $intJmClick = ($arrAnalyticData['jm']['click']) ? $arrAnalyticData['jm']['click'] : 0;
        $intJMNewsletterView = ($arrAnalyticData['jm-newsletter']['view'] > 0) ? $arrAnalyticData['jm-newsletter']['view'] : 0;
        $intJmNewsletterClick = ($arrAnalyticData['jm-newsletter']['click']) ? $arrAnalyticData['jm-newsletter']['click'] : 0;
        $intWidgetView = ($arrAnalyticData['3bl_widgets']['view'] > 0) ? $arrAnalyticData['3bl_widgets']['view'] : 0;
        $intWidgetClick = ($arrAnalyticData['3bl_widgets']['click']) ? $arrAnalyticData['3bl_widgets']['click'] : 0;
        $strHtml .= '</div>
<table width="100%" cellpadding="0" cellspacing="0" border="0" style="border:1px solid #e1e1e1">
    <tr>
        <td style="background:#000000; border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">
            <b><span style="color:white">Channels</span></b></td>
        <td style="background:#000000;padding:5px;border-right:1px solid #eee; text-align:left; border-bottom:1px solid #e1e1e1;">
            <b><span style="color:white">Views</span></b></td>
        <td style="background:#000000;padding:5px; text-align:left; border-bottom:1px solid #e1e1e1;"><b><span
            style="color:white">Clicks</span></b></td>
    </tr>
    <tr>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">3bl Media</td>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">' .
            $int3blmediaView . '
        </td>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">' .
            $int3blmediaClick . '
        </td>
    </tr>

    <tr>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">3bl Widgets</td>
        <td style="border-right:1px solid #eee;padding:5px; text-align:left; border-bottom:1px solid #e1e1e1;">' .
            $intWidgetView . '
        </td>
        <td style="padding:5px; text-align:left; border-bottom:1px solid #e1e1e1;">' .
            $intWidgetClick . '
        </td>
    </tr>

    <tr>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">3bl Newsletter</td>
        <td style="border-right:1px solid #eee;padding:5px; text-align:left; border-bottom:1px solid #e1e1e1;"> -</td>
        <td style="padding:5px; text-align:left; border-bottom:1px solid #e1e1e1;">' .
            $arrAnalyticData['newsletter']['click'] . '
        </td>
    </tr>
    <tr>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">Justmeans</td>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">' .
            $intJMView . '
        </td>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">' .
            $intJmClick . '
        </td>
    </tr>
    <tr>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">Justmeans Newsletter</td>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">' .
            $intJMNewsletterView . '
        </td>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">' .
            $intJmNewsletterClick . '
        </td>
    </tr>
    ';
        $str3blListHtml = "";
        $strClientListHtml = "";
        $strTotalClicks = 0;
        $strTotalViews = 0;

        $int3blCount = 0;
        $intClientCount = 0;

        //Checking list exist or not
        if (!empty($arrMicroListDetails)) {

            foreach ($arrMicroListDetails as $strKey => $arrMicroList) {

                if ($strKey == "3bl_micro_list") {

                    foreach ($arrMicroList AS $listDetails) {
                        $str3blListHtml .= $this->fnBuildMicroListDataHtml($listDetails['title'], $listDetails['open'],
                            $listDetails['click']);
                        $strTotalClicks = $strTotalClicks + $listDetails['click'];
                        $strTotalViews = $strTotalViews + $listDetails['open'];
                        $int3blCount++;
                    }

                } elseif ($strKey == "client_micro_list") {
                    foreach ($arrMicroList AS $listNewDetails) {
                        $strClientListHtml .= $this->fnBuildMicroListDataHtml($listNewDetails['title'], $listNewDetails['open'],
                            $listNewDetails['click']);
                        $strTotalClicks = $strTotalClicks + $listNewDetails['click'];
                        $strTotalViews = $strTotalViews + $listNewDetails['open'];
                        $intClientCount++;
                    }

                }
            }
        }

        //Checking 3bl micro list
        if ($int3blCount < 1) {
            $str3blListHtml = '
    <tr>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;text-align:center"
            colspan="3">No any list attach.
        </td>
    </tr>
    ';
        }

        //Checking client micro list
        if ($intClientCount < 1) {

            $strClientListHtml = '
    <tr>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;text-align:center"
            colspan="3">No any list attach.
        </td>
    </tr>
    ';
        }

        //Build the html for client and 3bl micro list
        $strHtml .= '
    <tr>
        <td colspan="3"
            style="background:#ccc;border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;"> 3bl
            Micro List
        </td>
    </tr>
    ';
        $strHtml .= $str3blListHtml;
        $strHtml .= '
    <tr>
        <td colspan="3"
            style="background:#ccc;border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;"> Client
            Micro List
        </td>
    </tr>
    ';
        $strHtml .= $strClientListHtml;


        //Aggregate all clicks and views.
        $intFinalTotalViews = $strTotalViews + $arrFinalResult['totalviews'];
        $intFinalTotalClicks = $strTotalClicks + $arrFinalResult['totalClicks'];

        //Build the total section.
        $strHtml .= '
    <tr>
        <td style="background:#ccc;border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">
            <b>Total</b></td>
        <td style="background:#ccc;border-right:1px solid #eee;padding:5px; text-align:left; border-bottom:1px solid #e1e1e1;">
            <b>' . $intFinalTotalViews . ' </b></td>
        <td style="background:#ccc;padding:5px; text-align:left; border-bottom:1px solid #e1e1e1;"><b>' .
            $intFinalTotalClicks . '</b></td>
    </tr>
</table>';
        return $strHtml;
    }

    /**
     * Function fnBuildMicroListDataHtml
     *
     * @param string $strTitle  : Title of micro list
     * @param string $strOpen   : views
     * @param string $listClick : clicks
     *
     * @return string
     */
    function fnBuildMicroListDataHtml($strTitle, $strOpen, $listClick)
    {

        $strMicroListHtml = '<tr>
                             <td style="border-right:1px solid #eee;padding: 5px 20px; border-bottom:1px solid #e1e1e1;" align="right">' . $strTitle . '</td>
                             <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">' . $strOpen . '</td>
                             <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;">' . $listClick . '</td>
                             </tr>';

        return $strMicroListHtml;
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

    /**
     * Build the FMR report in HTML format and download it in the excel file.
     *
     * @param array  $arrFMRInfo : array of published FMR information
     * @param string $strFMRIds  : Comma separated FMR ids

     */
    function fnBuildHtmlFMRMonthlyReport($arrFMRInfo, $strFMRIds)
    {
        // for getting the Clicks Count
        $arrFMRClicks = $this->fnGetClicks($strFMRIds);

        // for getting the Views Count
        $arrFMRViews = $this->fnGetViews($strFMRIds);

        if (is_array($arrFMRInfo) && count($arrFMRInfo)) {

            $strHTML = '<table width="100%" cellpadding="0" cellspacing="0" border="1">
                        <tr>
                            <th align="center">FMR Id</th>
                            <th align="center">FMR Title</th>
                            <th align="center">Client Name</th>
                            <th align="center">Views</th>
                            <th align="center">Clicks</th>
                            <th align="center">Published Date</th>
                        </tr>';
            foreach ($arrFMRInfo as $intFMRId => $arrFMRInfoValue) {

                $strHTML .= '<tr>
                                 <td>' . $arrFMRInfoValue["fmrid"] . '</td>
                                 <td>' . $arrFMRInfoValue["title"] . '</td>
                                 <td>' . $arrFMRInfoValue["companyname"] . '</td>
                                 <td>' . (int) $arrFMRViews[$intFMRId] . '</td>
                                 <td>' . (int) $arrFMRClicks[$intFMRId] . '</td>
                                 <td>' . date("M d, Y", strtotime($arrFMRInfoValue["publishdate"])) . '</td>
                             </tr>';
            }

            $strHTML .= '</table>';
        }

        // Download report in excel file
        header("Content-Type: application/vnd.ms-excel;");
        header("Content-type: application/x-msexcel;");
        header("Content-disposition: attachment; filename=FMRReport" . time() . ".xls");

        print $strHTML;
        exit;
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
     * This function Removes special character's added through Msword.
     *
     * @param sting $strMsWordText : MS word text
     *
     * @return string

     */
    function fnRemoveWordFormatting($strMsWordText, $strSection = "")
    {
        $search = array(
            '/(%E2%80%93|%u2013|%u2026|%u2014|%96|%E2%80%94)/i',
            '/(%E2%80%9C|%E2%80%9D|%u201C|%u201D|%93|%94)/i',
            '/(%E2%80%98|%E2%80%99|%u2018|%u2019|%91|%92|%E2%80%9B)/i',
            '/(%95%09)/',
            '/(%E2%80%A6)/',
            '/(%E2%80%9A)/',
            '/(%E2%80%8B)/',
            '/(%E2%80%A8)/'
        );

        $replace = array(
            '-',
            '"',
            "'",
            "- ",
            "...",
            ",",
            " ",
            " "
        );

        $strMsWordText = preg_replace($search, $replace, $strMsWordText);
        $strMsWordUrlDecodeText = urldecode(preg_replace($search, $replace, urlencode($strMsWordText)));

        if ($strSection == "widget") {
            $strMsWordFinalText = html_entity_decode(htmlentities($strMsWordUrlDecodeText));
        } else {
            $strMsWordFinalText = utf8_decode(html_entity_decode(htmlentities($strMsWordUrlDecodeText)));
        }
        

        return $strMsWordFinalText;
    }

    /**
     * Class destructor
     */
    public function __destruct()
    {

    }
}
// Object of class
$objReports = new clsReports();

