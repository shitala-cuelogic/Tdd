<?php
//This is class for Reports functions.
class ClsReportsData extend BaseClass
{
    /**
     * Function gather data of 7 days and create array for the Graph.
     *
     * @param array $arrChart  : FMR Views Info
     * @param array $arrChart1 : FMR Clicks Info
     *
     * @return array
     */
    public function fnSumWeekViewsClicks($arrChart, $arrChart1 = array())
    {
        $intMaxChart = max($arrChart);
        $intMaxChart1 = (count($arrChart1) > 0) ? max($arrChart) : 0;

        if ($intMaxChart <= 0 && $intMaxChart1 <= 0) {
            return (count($arrChart1) > 0) ? array("", "", "") : array("", "");
        }

        # getting previous date from today's date means we need the dates array having 7 days difference.(ex 1-11-2012, 7-11-2012)
        $strTodayDate = $this->strCurrentDate;
        $arrDts = $this->fnGetPreviousDates($strTodayDate, 59, 1);
        $arrDts = array_reverse($arrDts);

        //Process views and clicks and prepare 7 days interval data for graph
        $arrDates = array();
        $arrChartSum = $arrChartSum1 = array();
        $strDate = "";
        $intCount = $intGraphSum = $intGraphSum1 = 0;
        foreach ($arrDts as $key => $val) {

            $intGraphSum += (int)$arrChart[$val];
            $intGraphSum1 += (int)$arrChart1[$val];
            $intCount++;
            if ($intCount == 7) {
                $strDate = date("M d", strtotime($val));
                $arrDates[] = $strDate;
                $arrChartSum[$strDate] = (int)$intGraphSum;
                $arrChartSum1[$strDate] = (int)$intGraphSum1;

                $strDate = "";
                $intCount = 0;
                $intGraphSum = 0;
                $intGraphSum1 = 0;
            }
        }
        //Update Last date range data.
        if ($intCount > 0) {
            $strDate = date("M d", strtotime($val));
            $arrDates[] = $strDate;
            $arrChartSum[$strDate] = (int)$intGraphSum;
            $arrChartSum1[$strDate] = (int)$intGraphSum1;
        }
        if (count($arrChart1) > 0) {
            return array($arrChartSum, $arrChartSum1, $arrDates);
        } else {
            return array($arrChartSum, $arrDates);
        }
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
        $strMessage = $this->fnGenerateCompanyReportMessage($intMonth, $strHeaderHTML, $strFotterHTML, $strHeaderHTML, $strFotterHTML);

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
        $strCategoryHTML = $this->fnGenerateCategoryHTML($strPrimaryCategory, $strSecondaryCategory);

        //Build the header section such as information about the FMR.
        $strHtml = $this->fnGenerateHeaderHTML($arrFinalResult['FMRID'], $arrFinalResult['title'], $arrFinalResult['FMRTYPE'], $arrFinalResult['CompUrl'], $arrFinalResult['CompName'], $arrFinalResult['published_date'], $strCategoryHTML);

        
        //Build the html related to analytic data.
        $int3blmediaView = ($arrAnalyticData['3blmedia']['view'] > 0) ? $arrAnalyticData['3blmedia']['view'] : 0;
        $int3blmediaClick = ($arrAnalyticData['3blmedia']['click']) ? $arrAnalyticData['3blmedia']['click'] : 0;
        $intJMView = ($arrAnalyticData['jm']['view'] > 0) ? $arrAnalyticData['jm']['view'] : 0;
        $intJmClick = ($arrAnalyticData['jm']['click']) ? $arrAnalyticData['jm']['click'] : 0;
        $intJMNewsletterView = ($arrAnalyticData['jm-newsletter']['view'] > 0) ? $arrAnalyticData['jm-newsletter']['view'] : 0;
        $intJmNewsletterClick = ($arrAnalyticData['jm-newsletter']['click']) ? $arrAnalyticData['jm-newsletter']['click'] : 0;
        $intWidgetView = ($arrAnalyticData['3bl_widgets']['view'] > 0) ? $arrAnalyticData['3bl_widgets']['view'] : 0;
        $intWidgetClick = ($arrAnalyticData['3bl_widgets']['click']) ? $arrAnalyticData['3bl_widgets']['click'] : 0;

        //Generating HTML for analytic data
        $strHtml = $this->fnGenerateAnalyticHTML($int3blmediaView, $int3blmediaClick, $intWidgetView, $intWidgetClick, $arrAnalyticData['newsletter']['click'], $intJMView, $intJmClick, $intJMNewsletterView, $intJmNewsletterClick);




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
        $strHtml .= $this->function fnGenerateFinalHTML($str3blListHtml, $strClientListHtml, $strTotalViews, $strTotalClicks, $arrFinalResult['totalviews'], $arrFinalResult['totalClicks']);

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

            //Generating FMRMonthlyReport Message
            $strHTML = $this->fnGenerateMonthlyReportMessage($arrFMRInfo);

        }

        // Download report in excel file
        header("Content-Type: application/vnd.ms-excel;");
        header("Content-type: application/x-msexcel;");
        header("Content-disposition: attachment; filename=FMRReport" . time() . ".xls");

        print $strHTML;
        exit;
    }


    /**
     * This function Removes special character's added through Msword.
     *
     * @param sting $strMsWordText : MS word text
     * @param string $strSection
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


    /* Added for micro functioning */

    public function fnGenerateCategoryHTML($strPrimaryCategory, $strSecondaryCategory)
    {
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

        return $strCategoryHTML;
    }

    public function fnGenerateHeaderHTML($intFMRID, $strtitle, $strFMRTYPE, $strCompUrl, $strCompName, $strpublished_date, $strCategoryHTML)
    {
                $strHtml .= '
        <div id="fmr-header" style="background: none repeat scroll 0 0 #E9EBED;margin: 0 0 0.5em;padding: 1em 20px;">
            <div class="client-name" style="padding: 5px">
                <a href="http://test2.3blmedia.com/node/' . $intFMRID . '"><b><span style="font-size: 18px;">' . $strtitle . '</span></b></a>
            </div>
            <div class="client-link" style="padding: 5px">
                Media Type: <b>' . ucwords($strFMRTYPE) . '</b> | <a
                href="http://test2.3blmedia.com/node/' . $strCompUrl . '" target="blank">' .
                    $strCompName . '</a><br>

                <div style="margin-top: 10px">Date: ' . date('l, F d, Y', strtotime($strpublished_date)) . '
                </div>
            </div>
        </div>' . $strCategoryHTML;

        return $strHtml;
    }

    public function fnGenerateAnalyticHTML($int3blmediaView, $int3blmediaClick, $intWidgetView, $intWidgetClick, $intNewsletterClick, $intJMView, $intJmClick, $intJMNewsletterView, $intJmNewsletterClick)
    {
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
                    $intNewsletterClick . '
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

        return $strHtml;
    }

    public function fnGenerateFinalHTML($str3blListHtml, $strClientListHtml, $strTotalViews, $strTotalClicks, $strFinalResultTotalViews, $strFinalResultTotalClicks)
    {

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
                $intFinalTotalViews = $strTotalViews + $strFinalResultTotalViews;
                $intFinalTotalClicks = $strTotalClicks + $strFinalResultTotalClicks;

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

    public function fnGenerateMonthlyReportMessage($arrFMRInfo)
    {

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

            return $strHTML;


    }

    public function fnGenerateCompanyReportMessage($intMonth, $strHeaderHTML, $strFotterHTML, $strHeaderHTML, $strFotterHTML)
    {

        $strMessage = ((int)$intMonth == 1) ? $strHeaderHTML."Attached you will find your up to date Analytics Summary Report from 3BL Media. This report is sent automatically once a month. You can review all of your 3BL Media analytics at any time at http://3blmedia.com/Dashboard/Analytics/Views".$strFotterHTML : $strHeaderHTML.'Attached you should find Flexible Media Release (FMR) Detail Reports for any content distributed between ' . date("F d, Y", mktime(0, 0, 0, date("m"), date("d") - 15, date("Y"))) . ' and ' . date("F d, Y", mktime(0, 0, 0, date("m"), date("d") - 9, date("Y"))) . '.  If you have any questions, please contact your media consultant at 866.508.0993 or reply to this message.<br/>Of course, you can review all of your 3BL Media Analytics and Tracking in your portal at any time at http://3blmedia.com/Dashboard/Analytics/Views<br/><br/>Thank you,<br/>The 3BL Media Team'.$strFotterHTML;

        return $strMessage;


    }

    /* Added for micro functioning */

    /**
     * Class destructor
     */
    public function __destruct()
    {

    }
}
// Object of class
$objReports = new clsReports();

