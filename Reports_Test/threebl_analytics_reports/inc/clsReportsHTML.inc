<?php
//This is class for Reports functions.
class ClsReportHTML
{

    public function __construct()
    {

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

    public function fnGenerateHeaderHTML($intFMRId, $strTitle, $strFMRType, $strCompUrl, $strCompName, $strPublishedDate, $strCategoryHTML)
    {
                $strHtml = '
        <div id="fmr-header" style="background: none repeat scroll 0 0 #E9EBED;margin: 0 0 0.5em;padding: 1em 20px;">
            <div class="client-name" style="padding: 5px">
                <a href="http://test2.3blmedia.com/node/' . $intFMRId . '"><b><span style="font-size: 18px;">' . $strTitle . '</span></b></a>
            </div>
            <div class="client-link" style="padding: 5px">
                Media Type: <b>' . ucwords($strFMRType) . '</b> | <a
                href="http://test2.3blmedia.com/node/' . $strCompUrl . '" target="blank">' .
                    $strCompName . '</a><br>

                <div style="margin-top: 10px">Date: ' . date('l, F d, Y', strtotime($strPublishedDate)) . '
                </div>
            </div>
        </div>' . $strCategoryHTML;

        return $strHtml;
    }

    public function fnGenerateAnalyticHTML($int3blmediaView, $int3blmediaClick, $intWidgetView, $intWidgetClick, $intNewsletterClick, $intJMView, $intJmClick, $intJMNewsletterView, $intJmNewsletterClick)
    {
                $strHtml = '</div>
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
                $strHtml = '
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

    public function fnGenerateMonthlyReportMessage($arrFMRInfo, $intViewsFMRid, $intClicksFMRid)
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
                                 <td>' . (int) $intViewsFMRid . '</td>
                                 <td>' . (int) $intClicksFMRid . '</td>
                                 <td>' . date("M d, Y", strtotime($arrFMRInfoValue["publishdate"])) . '</td>
                             </tr>';
            }

            $strHTML .= '</table>';

            return $strHTML;


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
     * Build the FMR report in HTML format and download it in the excel file.
     *
     * @param array  $arrFMRInfo : array of published FMR information
     * @param string $strFMRIds  : Comma separated FMR ids
     *
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

        return $strHTML;
        //exit;
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

