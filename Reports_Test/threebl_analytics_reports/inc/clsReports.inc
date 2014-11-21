<?php

module_load_include('inc', 'refactor_Analytic', 'inc/clsBaseClass');
//This is class for Analytics functions.

//This is class for Reports functions.
class ClsReports extends clsBaseClass
{
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

        /* $intInsertSuccess = false;
        if (count($arrReceivedReport) > 0) {
            //If mail sent successfully then insert these details in the table
            $strRecipients = implode(",", $arrReceivedReport);
            $intInsertSuccess = $this->fnInsertCompanyDetails((int)$intMonth,$intCompanyNid,$strRecipients,'email');
        } */
        //return $arrReceivedReport;
        //return $intInsertSuccess;
        return array($arrReceivedReport, $intMonth);
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

    public function fnGenerateCompanyReportMessage($intMonth, $strHeaderHTML, $strFooterHTML)
    {
        $strMessage = ((int)$intMonth == 1) ? $strHeaderHTML."Attached you will find your up to date Analytics Summary Report from 3BL Media. This report is sent automatically once a month. You can review all of your 3BL Media analytics at any time at http://3blmedia.com/Dashboard/Analytics/Views".$strFooterHTML : $strHeaderHTML.'Attached you should find Flexible Media Release (FMR) Detail Reports for any content distributed between ' . date("F d, Y", mktime(0, 0, 0, date("m"), date("d") - 15, date("Y"))) . ' and ' . date("F d, Y", mktime(0, 0, 0, date("m"), date("d") - 9, date("Y"))) . '.  If you have any questions, please contact your media consultant at 866.508.0993 or reply to this message.<br/>Of course, you can review all of your 3BL Media Analytics and Tracking in your portal at any time at http://3blmedia.com/Dashboard/Analytics/Views<br/><br/>Thank you,<br/>The 3BL Media Team'.$strFooterHTML;

        return $strMessage;
    }

    /**
     * Function gather data of 7 days and create array for the Graph.
     *
     * @param array $arrViewChart  : FMR Views Info
     * @param array $arrClickChart : FMR Clicks Info
     *
     * @return array
     */
    public function fnSumWeekViewsClicks($arrViewChart, $arrClickChart = array())
    {
        module_load_include('inc', 'refactor_Analytic', 'inc/commonFunction');

        $intMaxChart = max($arrViewChart);
        $intMaxChart1 = (count($arrClickChart) > 0) ? max($arrViewChart) : 0;

        if ($intMaxChart <= 0 && $intMaxChart1 <= 0) {
            return (count($arrClickChart) > 0) ? array("", "", "") : array("", "");
        }

        # getting previous date from today's date means we need the dates array having 7 days difference.(ex 1-11-2012, 7-11-2012)
        $strTodayDate = $this->strCurrentDate;
        $arrDts = fnGetPreviousDates($strTodayDate, 89, 1);
        $arrDts = array_reverse($arrDts);

        //Process views and clicks and prepare 7 days interval data for graph
        $arrDates = array();
        $arrViewChartSum = $arrClickChartSum = array();
        $strDate = "";
        $intCount = $intClickSum = $intViewSum = 0;
        foreach ($arrDts as $key => $val) {

            $intClickSum += (int)$arrViewChart[$val];
            $intViewSum += (int)$arrClickChart[$val];
            $intCount++;
            if ($intCount == 7) {
                $strDate = date("M d", strtotime($val));
                $arrDates[] = $strDate;
                $arrViewChartSum[$strDate] = (int)$intClickSum;
                $arrClickChartSum[$strDate] = (int)$intViewSum;

                $strDate = "";
                $intCount = 0;
                $intClickSum = 0;
                $intViewSum = 0;
            }
        }
        //Update Last date range data.
        if ($intCount > 0) {
            $strDate = date("M d", strtotime($val));
            $arrDates[] = $strDate;
            $arrViewChartSum[$strDate] = (int)$intClickSum;
            $arrClickChartSum[$strDate] = (int)$intViewSum;
        }

        if (count($arrClickChart) > 0) {
            return array($arrViewChartSum, $arrClickChartSum, $arrDates);
        } else {
            return array($arrViewChartSum, $arrDates);
        }
       
        
    }

    


    public function fnGetAnalyticViewData($objFMRViewInfo, $strReportType="") {

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

    public function fnGetAnalyticClickData($objFMRClickInfo, $strReportType = "") {

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


    public function fnGetMicrolistDetails($arrViewCount) {

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

    public function fnGetLatestFMRInfo($arrFMRDetails) {

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
        
        return array($arrFMRInfo, $strFMRIds);
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

