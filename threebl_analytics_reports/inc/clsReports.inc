<?php
//This is class for Reports functions.
class ClsReports
{




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

                $objAutoReportEmail = $this->getMonthlyAutoReportEmail($strCompanyNid);

            } else {

                $objAutoReportEmail = $this->getWeeklyAutoReportEmail($strCompanyNid);
            }

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
     * Class destructor
     */
    public function __destruct()
    {

    }
}
// Object of class
$objReports = new clsReports();

