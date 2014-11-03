<?php


class clsMicroListHtml {

	/**
     * To Build FMR Details HTML
     *
     * @param Array  $arrCompanyFMRDetails : Company FMR Details
     * @param string $strListType          : List Type
     *
     * @return string
     */
    public function fnBuildFMRDetailsHTML($arrCompanyFMRDetails, $strListType = '')
    {
        global $base_url, $conf;

        #Include required files
        module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReports');

        //Create the object of class
        $objReport = new ClsReports();

        # Get FMR Contact Information
        $arrFMRContactInfo = $arrCompanyFMRDetails["strContactInfo"];
        $strContactName = $arrFMRContactInfo["ContactName"];
        $strContactOrganization = $arrFMRContactInfo["ContactOrganization"];
        $strContactPhoneNo = $arrFMRContactInfo["ContactPhoneNo"];
        $strContactPhoneExt = $arrFMRContactInfo["ContactPhoneExt"];
        $strContactEmail = $arrFMRContactInfo["ContactEmail"];
        $strContactHtml = "";

        extract($arrCompanyFMRDetails); //getting the all fmr variable.

        // Social Media Icon image url
        $strSocialMediaUrl = $conf['SOCIAL_MEDIA_ICON'];

        //Building the Email Messages
        if ($strSummary != "") {
            $strSummaryDesc = $strSummary;
            $strSummary = substr($strSummaryDesc, 0, 150);

            if (strlen($strSummaryDesc) > 150) {
                $strSummary .= '...';
            }
        }

        if ($strFmrShortDesc != "") {
            $strDetails = $strFmrShortDesc;

            if ($strListType == "client_micro_list") {
                $strFmrShortDesc = $this->fnWordWrapString($strDetails, 1200);
            } else {
                $strFmrShortDesc = $this->fnWordWrapString($strDetails, 600);
            }
        }

        if ($strContactName != "") {
            $strExtension = ($strContactPhoneExt != "") ? " ext. " . $strContactPhoneExt : "";
            $strContactHtml = '<tr>
                        	<td>&nbsp;</td>
                        <td>
                        <table style="font-size:11px; background:#eee; padding:5px; font-family:verdana">
                        	<tr>
                            	<td valign="top" style="width: 60px"><b>Contact:</b></td>
                                <td>
                                	<div style="padding-bottom:4px"><span>' . $strContactName . ', ';
            $strContactHtml .= ($strContactOrganization != "") ? $strContactOrganization : "";
            $strContactHtml .= '</span></div>';

            $strContactHtml .= ($strContactPhoneNo != "") ? '<div style="padding-bottom:4px"><span>' . $strContactPhoneNo . ' ' . $strExtension . '</span></div>' : "";
            $strContactHtml .= ($strContactEmail != "") ? '<div style="padding-bottom:10px"><span><a href="mailto:' . $strContactEmail . '" style="color:#00A5C5">' . $strContactEmail . '</a></span></div>' : "";
            $strContactHtml .= ' </td>
                            </tr>
                        </table>
                        </td></tr>';
        }

        $strEncodeFMRTitle = urlencode($strFmrTitle);
        $strSocialMediaIcons = '<tr>
                                    <td align="right" colspan="2">
                                         <table cellpadding="1">
                                         <tr>
                                             <td valign="middle"><span style="background-color:#FFFFFF; font-size:9px; font-family:verdana;color:#999999">SHARE THIS: </span></td>
                                             <td><a target="_blank" href="http://www.facebook.com/share.php?u='.$strFmrUrl.'" title="Facebook" share="link"><img border="0" src="'.$strSocialMediaUrl.'facebook_button.jpg"></a></td>
                                             <td><a target="_blank" href="http://twitter.com/share?text='.$strEncodeFMRTitle.'%3A'.$strFmrUrl.'" title="Twitter" share="link"><img border="0" src="'.$strSocialMediaUrl.'twitter_button.jpg"></a></td>
                                             <td><a href="http://www.linkedin.com/shareArticle?mini=true&url=' . $strFmrUrl . '&title=' . $strEncodeFMRTitle . '&source=3BL Media" target="_blank" title="LinkedIn" share="link"><img border="0" src="'.$strSocialMediaUrl.'/linkedin_button.jpg" name="linkedn"></a></td>
                                             <td><a href="http://tumblr.com/share?s=&v=3&t='. $strEncodeFMRTitle.'&u='.urlencode($strFmrUrl).'" target="_blank" title="Tumblr" share="link"><img border="0" src="'.$base_url."/".$conf['IMAGES_PATH_3BL'].'/tumblr.png" name="Tumblr"></a></td>
                                         <tr>
                                         </table>
                                     </td>
                                </tr>';

        if ($strListType == "client_micro_list") {
            $strClickTrackUrl = '[strClickTrackingURL]&url='.$strFmrUrl;
            $strEmailBody = '<tr>
            	<td style="border-bottom:1px solid #afb9bb; padding-bottom:10px;">
                    <span style="font-family:verdana; color:#202020; display:block;"><h2 style="font-weight: bold; margin: 10px 0px 10px 10px ! important; font-size:16px !important; color:#999">Source: <a href="' . $base_url . '/' . $strCompanyUrl . '" target="_blank" style="color:#009EBC;text-decoration:none">' . $strCompanyName . '</a></h2></span>
                    <div style="margin:5px 10px 5px 10px;">
                        <a style="font-size:14px; font-family:verdana; font-weight:bold; color:#616161 !important;text-decoration:none;display:block;padding-bottom:6px;" href="'.$strClickTrackUrl.'"><span style="color:#000;">'.$objReport->fnRemoveWordFormatting($strFmrTitle).'</span></a>
                    </div>
                    <table width="100%" border="0" cellspacing="0" cellpadding="0" style="padding:5px 0 5px 10px">
                      <tbody>
                        <tr>
                          <td width="200" align="left" valign="top">
                          		<div style="padding-right:20px"><a href="'.$strClickTrackUrl.'" style="text-decoration:none!important" target="_blank"><img src="' . $strFinalPhotoUrl . '" alt="image" style="border: 1px solid #ECECEC;"></a></div>
                          </td>
                          <td width="560" align="left" valign="top" style="padding-right:10px;">

                                <div style="margin-bottom:8px; clear:both">
                                    <div style="font-size:11px; font-family:verdana; color:#202020; display:block; margin-bottom:4px; float:left"><strong style="color:#626262;">' . ucfirst($strFmrTypeOfContent) . '</strong></div><div style="font-size:10px; font-family:verdana; color:#626262; display:block; float:left; ">&nbsp;|&nbsp;' . date('M d, Y g:i A', strtotime($strPublishDate)) . ' ET</div>&nbsp;
                                </div>

                                <div style="font-size:11px; font-family:verdana; color:#8f8f8f; margin:0px;">'.$objReport->fnRemoveWordFormatting($strSummary).'</div>

                                <p style="font-size:11px; font-family:verdana; color:#202020; line-height:16px;">'. $objReport->fnRemoveWordFormatting($strFmrShortDesc).'
                                <span><a href="'.$strClickTrackUrl.'"  style="font-size:11px; font-family:verdana; color:#00a5c5;text-decoration:none">Read more</a></span></p>
                            </td>
                        </tr>'.$strSocialMediaIcons;

            #Add Contact information tr
            $strEmailBody .= ($strContactHtml != "") ? $strContactHtml : " ";

            #Add remaining table tags
            $strEmailBody .= '</tbody>
                    </table>
                </td>
            </tr>';

        } else {
            $strClickTrackUrl = '[strClickTrackingURL]&action='.base64_encode($intFmrId).'&url=';

            $strEmailBody = '<tr>
            	<td style=" padding-bottom:10px; ">
                    <table width="100%" border="0" cellspacing="0" cellpadding="0" style="border-bottom:1px solid #afb9bb; padding-bottom:5px;">
                      <tbody>
                        <tr>
                          <td width="200" align="left" valign="top">
                          		<div style="padding-right:20px"><a href="'.$strClickTrackUrl.$strFmrUrl .'" target="_blank"><img src="' . $strFinalPhotoUrl . '" alt="image" style="max-width:100%"></a></div>
                          </td>
                          <td width="560" align="left" valign="top">
                                <a style="font-size:16px; font-family:verdana; font-weight:bold; color:#616161 !important;text-decoration:none;display:block;padding-bottom:6px;" href="'.$strClickTrackUrl.$strFmrUrl.'"><span style="color:#009ebc;">'.$objReport->fnRemoveWordFormatting($strFmrTitle).'</span></a>
                                <div style="font-size:11px; font-family:verdana; color:#8f8f8f; margin:0px;">'.$objReport->fnRemoveWordFormatting($strSummary).'</div>
                                <p style="margin-top:8px; margin-bottom:8px;"><span style="font-size:11px; font-family:verdana; color:#202020; display:block; margin-bottom:4px;"><strong style="color:#626262;">' . ucfirst($strFmrTypeOfContent) . '</strong> from <strong><a href="' . $base_url . '/' . $strCompanyUrl . '" target="_blank" style="color:#009EBC;text-decoration:none" name="company">' . $strCompanyName . '</a></strong></span>
                                <span style="font-size:10px; font-family:verdana; color:#626262; display:block;">' . date('M d, Y g:i A', strtotime($strPublishDate)) . ' ET</span></p>
                                <p style="font-size:11px; font-family:verdana; color:#202020; line-height:16px;">'.$objReport->fnRemoveWordFormatting($strFmrShortDesc).'
                                <span><a href="'.$strClickTrackUrl.$strFmrUrl.'"  style="font-size:11px; font-family:verdana; color:#00a5c5;text-decoration:none">Read more</a></span></p>
                            </td>
                        </tr>'. $strSocialMediaIcons.'
                        </tbody>
                    </table>
                </td>
            </tr>';
        }
        return $strEmailBody;
    }

    /**
     *  Function to build the final html body to fmr distribution.
     *
     * @param string $strClientMessage     : Client Message
     * @param string $strMainHtmlBody      : HTML Body
     * @param string $strListName          : List Name
     * @param string $strListType          : List Type
     * @param string $strProcessingContact : Flag to FMR Processing Contacts
     *
     * @return string
     */
    public function fnBuildFinalHtml($strClientMessage, $strMainHtmlBody, $strListName = '', $strListType = '', $strProcessingContact = '')
    {
        global $base_url;
        // $strEmailNote = "Testing footer Message";
        if ($strListName != '') {
            $strListName = trim(strtoupper($strListName)) . " NEWS ALERT";
        }

        // Final FMR Body HTML
        $strFinalFMRBody = $strMainHtmlBody;

        // Banner Image Path
        $strImagePath = $base_url . $this->strBannerImagePath;
        $strFMTHtmlBottom = "";

        // Set View this issue on web url html
        $strIssueOnWebLink = '<tr>
                                    <td width = "800" height = "28" align ="center" >
                                    <a href = "[WEB_ISSUE_URL]" > View this issue on the web </a >
                                    </td >
                                </tr>';

        // Set Javascript code
        $strJavaScriptCode = "";

        // skipp 'view issue on web' for processing contact and web html
        if ($strProcessingContact == "on_web" || $strProcessingContact == "processing_contact") {
            $strIssueOnWebLink = "";
        }

        if ($strListType == "client_micro_list") {
            $strFMRHeaderHtml = '';
            $strFMRTableHtml = '<table width="100%" border="0" cellspacing="0" cellpadding="0">';
        } else {

            $strFMRHeaderHtml = $strIssueOnWebLink.'<tr><td><table width="800" height="112" border="0" cellspacing="0" cellpadding="0"  style="border: solid 2px #fff" background="' . $strImagePath . '/3bl_banner_news.png">
                                  <tbody>
                                    <tr>
                                      <td style="font-family:arial; font-size:16px; color:#ffffff; padding:10px; font-weight:bold;">' . $strListName . '</td>
                                    </tr>
                                  </tbody>
                                </table></tr></td>';
            $strFMRTableHtml = '<table width="100%" border="0" cellspacing="0" cellpadding="10">';
            $strFMTHtmlBottom = '<tr>
                                    <td>
                                        <img src="' . $strImagePath . '/bottom_line_news.png" width="800" height="25" alt="" />
                                    </td>
                                </tr>';
        }

        # To skipp for processing contacts.
        $strUnSubscribeLinkHTML = "";
        $strProcessContactMsg = "";
        $strUnsubscribeLinkstyle = "style='padding-right:5px'";
        $strOpenTrackUrl = "";
        if ($strProcessingContact == "" && $strProcessingContact != "processing_contact") {

            $strUnsubscribeLinkstyle = "";
            $strUnsubscribeLink = '<td align="right"><span style="font-family:arial; font-size:11px; color:#626262; padding-right:20px; padding-bottom:10px; display:block"><a target="_blank" style="color:#bcbcbc!important;text-decoration:none" href="' . $base_url . '/OptOut/[strUserCode]/confirm/fmr">UNSUBSCRIBE</a></span></td >';

            $strOpenTrackUrl = '<tr><td><img src="[strOpenTrackURL]" height="1" width="1" align=""/></td></tr>';
            $strUnSubscribeLinkHTML = '<tr>
                        <td style="padding-top:5px;">
                            <table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tbody>
                                    <tr>
                                        <td align="left" width="75%" ' . $strUnsubscribeLinkstyle . '>
                                        <span style="font-family:verdana; font-size:11px; color:#626262; padding-left:20px; padding-bottom:10px; display:block">' . $strClientMessage . '</span>
                                        </td>
                                        ' . $strUnsubscribeLink . '
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>';

        } else if ($strProcessingContact == "processing_contact") {

            $strProcessContactMsg = '<table width="802" border="0" cellspacing="0" cellpadding="0" style="border-top-style:solid;border-right-style:solid;border-bottom-style:solid;border-left-style:solid;border-top-width:1px;border-right-width:1px;border-bottom-width:1px;border-left-width:1px; border-color: #E3E3E3;margin-bottom: 10px;">
                        <tr>
                        <td style="padding-top:5px">
                            <table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tbody>
                                    <tr>
                                        <td align="left" width="75%" style="padding-right:5px">
                                        <span style="font-size:12px;color:#181818 ;padding-left:20px;padding-bottom:10px;display:block;">' . $strClientMessage . '</span>
                                        </td>

                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    </table>';
        }

        if ($strProcessingContact == "on_web") {

            $strProcessContactMsg = "";
            $strOpenTrackUrl = "";
            $strUnSubscribeLinkHTML = "";
            // Set Javascript code to attache user id to tracking url
            $strJavaScriptCode = '<script type="text/jscript">
                                      jQuery(document).ready(function () {

                                      arrFMRAnchor = jQuery("#Id_FMR_List a");
                                      var strUserId = getUrlVars()["id"];

                                      for (intTab = 0; intTab < arrFMRAnchor.length; intTab++) {
                                          var strCompany = jQuery(arrFMRAnchor[intTab]).attr("name")
                                          var strShare = jQuery(arrFMRAnchor[intTab]).attr("share")
                                          if (strCompany != "company" && strShare != "link") {
                                              arrFMRAnchor[intTab].href = arrFMRAnchor[intTab].href+"&id="+strUserId;
                                          }
                                      }
                                      });

                                      /**
                                       * Function parse url and get the parameters for widget
                                       * @return {Object}
                                     */
                                     function getUrlVars() {
                                        var vars = {};
                                        var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function (m, key, value) {
                                            vars[key] = value;
                                        });
                                        return vars;
                                     }

                                   </script>';
        }

        //Building the Email Messages
        $strEmailBody = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
                        <html xmlns="http://www.w3.org/1999/xhtml">
                        <head>
                        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                        <title>3BL Media LLC</title>
                        <script src="/sites/all/themes/threebl/js/jquery-latest.js"></script>
                        <style type="text/css">
                            img{max-width:100%;}
                        </style>
                        '. $strJavaScriptCode.'
                        </head>
                        <body>
                        '. $strProcessContactMsg.'
                        <table width="800" border="0" cellspacing="0" cellpadding="0"  style="border: solid 1px #e3e3e3">
                          <tbody>
                            ' . $strFMRHeaderHtml . '
                            <tr>
                              <td id="Id_FMR_List">
                                ' . $strFMRTableHtml . '
                                  <tbody>' .
            $strFinalFMRBody . '
                             </tbody>
                        </table>
                      </td>
                    </tr>'. $strUnSubscribeLinkHTML . $strFMTHtmlBottom . $strOpenTrackUrl . '

                  </tbody>
                </table>
                <br/><br/>
                </body>
                </html>';
        return $strEmailBody;
    }


}

?>