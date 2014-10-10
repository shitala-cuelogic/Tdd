<?php

interface affiliatesControllerInterface extends DrupalEntityControllerInterface
{
    public function create();
    public function save($entity);
    public function delete($entity);
}


//This is class for Tracking Function.

class clsTracking extends affiliatesController implements affiliatesControllerInterface
{
    private $intCompanyId;
    private $strDate;
    public $AffiliateActive;
    public $AffiliateInactive;
    public $TrackingInclude;
    public $TrackingExclude;
    public $TrackingReport;
    public $NewsStatusFullText;
    public $NewsStatusHeadline;

    /**
     * class constructor
     */
    public function clsTracking($intCompany = 0)
    {
        $this->intCompanyId = $intCompany;
        $this->strDate = date("Y-m-d");

        //Affiliate Status
        $this->AffiliateActive = 113040;
        $this->AffiliateInactive =113041;

        //Tracking Status
        $this->TrackingInclude = 113036;
        $this->TrackingExclude = 113037;
        $this->TrackingReport =  113038;

        //3bl News Status
        $this->NewsStatusFullText = 113030;
        $this->NewsStatusHeadline = 113031;

        $this->strCurrentDate = date("Y-m-d H:i:s", (time() - 86400)); //(time()-86400) date("Y-m-d", strtotime("2013-12-16"))//
    }

    /**
     * getting all title and link form 3bl_affiliate
     * @return array
     */
    public function fnGetAllTitleOfAffiliate() //  old name -getAllTitleOfAffiliate
    {
        $arrAffiliate = array();
        //getting the entity details
        $arrEntity = entity_load('affiliates', false, array("type"=>"prconnect"));
        $arrAffEntity = json_decode(json_encode($arrEntity), 1); //object to array

        if (!empty($arrAffEntity)) {
            foreach ($arrAffEntity as $arrTitle) {
                if ($arrTitle['field_affiliate_title']['und'][0]['value'] != '') {
                    $arrAffiliate[$arrTitle['id']] =  $arrTitle['field_affiliate_title']['und'][0]['value'];
                }
            }
        }
        return $arrAffiliate;
    }

    /**
     * Function which will fetch all affiliate those are from 3bl
     * and get all FMR information from their sites and insert it into affiliate_tracking table.
     */
    public function fnTrackingAffiliates()
    {
        # Get all the affiliates
        $arrEntity = entity_load('affiliates', false, array("type"=>"3bl"));
        $arrAffEntity = json_decode(json_encode($arrEntity), 1); //object to array
        if (!empty($arrAffEntity)) {
            $intNoOfPages = 3; // number of HTML pages to parse
            foreach ($arrAffEntity as $arrAffData) {
                //field_affiliate_status = 108419 = Active
                //field_3bl_news_status = 108417 = Full Text
                if ($arrAffData['field_affiliate_status']['und'][0]['tid']== $this->AffiliateActive && $arrAffData['field_3bl_news_status']['und'][0]['tid']== $this->NewsStatusFullText && $arrAffData['field_affiliate_news_url']['und'][0]['value'] != '') {
                    $strLink = $arrAffData['field_affiliate_news_url']['und'][0]['value'];
                    $strQS  = $arrAffData['field_affiliate_query_string']['und'][0]['value'];
                    $strUrlPrefix = $arrAffData['field_affiliate_prefix']['und'][0]['value'];
                    $intAffId = $arrAffData['id'];
                    $strPaging = $arrAffData['field_affiliate_paging']['und'][0]['value'];

                    $arrElements = array(
                        'title' => $strQS,
                        'url'   => $strQS.'/@href'
                    );
                    # Start to get html and filter html
                    $arrData = $this->fnParseHTML($strLink, $arrElements, $strPaging, $intNoOfPages);
                    $intCnt = count($arrData);
                    for ($k = 0; $k < $intCnt; $k++) {
                        //CHECK if already exist
                        $intCount = $this->fnDuplicateTrackingUrl($strUrlPrefix.$arrData[$k]['url']);

                        if ($intCount == 0) {
                            // Fetch FMR node_id
                            //Getting the node id by node title.
                            $intNodeId = $this->fnGetNodeIdByTitle($arrData[$k]['title']);
                            if ($intNodeId > 0) {
                                //Tracking data array
                                $arrTrackingDate = array('nid'=>$intNodeId,
                                    'aff_id'=>$intAffId,
                                    'tracking_url'=> $strUrlPrefix.$arrData[$k]['url'],
                                    'added_date'=> $this->strDate
                                );
                                $this->fnInsertAffTracking($arrTrackingDate); // Add value in affiliate_tracking
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Function for prconnect cron
     * @param int $intId : FMR id
     * @return bool|string
     */
    public function fnPrconnectCron($intId = 0)
    {
        global $conf;
        //$intId is the FMR id
        if ((int)$intId == 0) {
            return;
        }
        global $base_url;

        // get all affiliate from 3bl_affiliate tables by
        $arrAffiliate = $this->fnGetAllTitleOfAffiliate();

        //prconnect tracking link
        $strURL = "http://tracking.prconnect.com/3blmedia?Module=clipping-all&SourceID=".$intId;
        $fileContents = file_get_contents($strURL);
        $fileContents = str_replace(array("\n", "\r", "\t"), '', $fileContents);
        $fileContents = trim(str_replace('"', "'", $fileContents));
        $simpleXml    = simplexml_load_string($fileContents, null, LIBXML_NOCDATA);

        $i=0;
        $arrComp = array();
        $arrContent = array();
        // To process the links from the parsed html
        if (count($simpleXml->channel->item) > 0) {
            foreach ($simpleXml->channel->item as $objChannelItem) {
                $strSource = (string) $objChannelItem->source[0];
                $arrContent[$i]['title'] = (string) addslashes($objChannelItem->title[0]);
                $arrContent[$i]['affiliate_news_url'] = (string) addslashes($objChannelItem->link[0]);
                $arrContent[$i]['affiliate_home_url'] = (string) $objChannelItem->source[0];

                //store all the affiliate title in particular array
                $arrComp[$i] =  (string)($objChannelItem->title[0]);
                $i++;
            }
        } else {
            return "2";
        }
        #Check the FMR distribute on the PRConnect or not.
        if (count($arrComp) < 1) {
            return "2";
        }
        //Get array diff from above getting array and existing database affiliate title array for key mapping
        $arrDiffer = array_diff($arrComp, $arrAffiliate);

        #If there is any new affiliate present in the list then email to admin about that.
        if (!empty($arrDiffer)) {
            // All system mails need to specify the module and template key (mirrored from
            // hook_mail()) that the message they want to send comes from.
            $strModule = 'email';
            $strMailKey = 'new_prconnect_message';

            // Specify 'to' and 'from' addresses.
            $strTo = $conf['prconnect_cron_distribution_to'];
            $strFrom = $conf['no_reply_from'];
            $strMailContent = "Hi,<br><br>We got new PRconnect affiliate(s) in the affiliate admin section. Please check the following details:<br><br>";
            $i = 1;
            foreach ($arrDiffer as $key => $value) {
                // insert unique record in affiliate table
                $strLink = (string) $arrContent[$key]['affiliate_news_url'];
                $strLnk = strstr($strLink, "news/", true);
                if ($strLnk != '') {
                    $strLnk.="news/channelinfo?ChannelID=6906";
                } else {
                    $strLnk = $strLink;
                }
                $arrContent[$key]['affiliate_news_url'] = (string) $strLnk;

                //Create an entity object to insert the affiliate into the db
                $entity = new stdClass();
                $entity->type = 'prconnect';
                $entity->id = 0;
                $entity->field_affiliate_title = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' =>$arrContent[$key]['title']
                        )
                    )
                );
                $entity->field_affiliate_news_url = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' =>$arrContent[$key]['affiliate_news_url']
                        )
                    )
                );
                $entity->field_affiliate_source = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' =>$arrContent[$key]['affiliate_home_url']
                        )
                    )
                );
                $entity->field_affiliate_status = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'tid' => $this->AffiliateInactive // inactive status (108420)
                        )
                    )
                );
                $entity->field_tracking_status = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'tid' => $this->TrackingInclude       //Include Status (108421)
                        )
                    )
                );

                $entity->field_affiliate_description = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' => ""
                        )
                    )
                );
                $entity->field_affiliate_monthly_traffic = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' => "0"
                        )
                    )
                );

                $aff_id = $this->fnInsertAffiliate($entity);

                unset($entity);
                $arrAffiliate[$aff_id] = $arrContent[$key]['title'];
                $strEditLink = $base_url."/admin/affiliate/edit?aid=".$aff_id;
                $strMailContent .= $i.". ".stripslashes($arrContent[$key]['title'])." <a href='".$strEditLink."'>(Edit)</a><br>
									Source link: ".stripslashes((string) $arrContent[$key]['affiliate_home_url'])."<br>
									News link: ".stripslashes($strLnk)."<br><br>";
                $strLnk = '';
                $i++;
            }//foreach
            $strMailContent .="Thanks,<br>3BL Media.<br>";
            $arrParams['message'] = $strMailContent;
            $arrParams['cc'] = $conf['tech_email'];
            $strLanguage = language_default();
            // Whether or not to automatically send the mail when drupal_mail() is
            // called. This defaults to TRUE, and is normally what you want unless you
            // need to do additional processing before drupal_mail_send() is called.
            $boolSend = true;
            // Send the mail, and check for success. Note that this does not guarantee
            // message delivery; only that there were no PHP-related issues encountered
            // while sending.

            drupal_mail($strModule, $strMailKey, $strTo, $strLanguage, $arrParams, $strFrom, $boolSend);
        }//Differ-array

        #Insert PRConnect Tracking for the FMR.
        foreach ($arrContent as $arrValue) {
            $intAffID = array_search($arrValue['title'], $arrAffiliate);
            $arrEntityAff = entity_load('affiliates', array($intAffID));
            $arrResult = json_decode(json_encode($arrEntityAff[$intAffID]), 1); //object to array
            $strStatus = $arrResult['field_affiliate_status']['und'][0]['tid'];

            if ((int)$strStatus == $this->AffiliateActive) {
                //To check if the tracking url already exists
                $intCount =$this->fnDuplicateTrackingUrl(trim($arrValue['affiliate_news_url']));

                if ((int)$intCount == 0) {
                    //Tracking data array
                    $arrTrackingDate = array('nid'=>$intId,
                        'aff_id'=>$intAffID,
                        'tracking_url'=>$arrValue['affiliate_news_url'],
                        'added_date'=> $this->strDate
                    );
                    $this->fnInsertAffTracking($arrTrackingDate); // Add value in  affiliate_tracking
                }
            }
        }

        //Checking Count of FMR if Yes Then Update Flag.
        if (count($arrComp) > 0) {

          //update the prconnect tracking flag in field_data_field_prconnect_tracking table
          $this->fnUpdateTrackingFlag($intId);
          
          
        }
        return true;
    }

    
    
    /**
     * Getting  old affiliate  logo
     * @param string $strLogo : name of Logo
     * @return array
     */
    public function fnLogo($strLogo)
    {
        $_FILES['image']['name'] = $strLogo;
        $_FILES['image']['tmp_name'] = '/var/www/vhosts/3blmedia.com/httpdocs/sites/default/files/affiliate_logo_live/scale/'.$strLogo;
        $_FILES['image']['type'] = 'image/jpeg';
        $_FILES['image']['error'] = 0;
        //Get the uploaded file from the temp directory.
        $image = file_get_contents($_FILES['image']['tmp_name']);

        //Saves a file to the specified destination and creates a database entry.
        $objFile = file_save_data($image, 'public://affiliates/'.$_FILES['image']['name'], FILE_EXISTS_RENAME);

        //Set the file status to permanent so it is not deleted in next cron run
        $objFile->status = FILE_STATUS_PERMANENT;
        file_save($objFile);
        return (array) $objFile;
    }

    

    /**
     *    Function to insert the tracking data from the given links.
     *    $strLink 		: url of the html page to be parsed.
     *    $arrElements 	: array of parameters as keys and DOM query string as value.
     *    Ex.: array(
     *    'title' => '//h2/a',
     *    'url'  => '//h2/a/@href'
     *    );
     *    $strPaging 		: pageing parameters for the link specified by $strLink
     *	   $intNoOfPages 	: number of pages to parse
     */
    public function fnParseHTML($strLink = '', $arrElements = '', $strPaging = '', $intNoOfPages = 0)
    {
        $arrData = array();
        $dom = new DOMDocument();
        $fileContents = '';
        if ($strPaging == '') {
            $fileContents = file_get_contents($strLink);
        } else {
            // Loop to get the HTML contents for the number of pages specified by $intNoOfPages
            for ($intCount = 1; $intCount <= $intNoOfPages; $$intCount++) {
                if ($intCount > 1) {
                    // #pgno is used to replace the string with the page number to get the contents of that page
                    $strTmp = str_replace("#pgno", $intCount, $strPaging);
                }
                // Concat HTML of all the pages
                $fileContents .= @file_get_contents($strLink.$strTmp);
            }
        }

        @$dom->loadHTML($fileContents);
        $xpath = new DOMXPath($dom);

        foreach ($arrElements as $strKey => $strVal) {
            $arrParams = @$xpath->query($strVal);
            $intCounter = 0;
            if (is_array($arrParams)) {
                foreach ($arrParams as $arrParam) {
                    $arrData[$intCounter++][$strKey] = $arrParam->childNodes->item(0)->nodeValue;
                }
            }
        }
        return $arrData;
    }

    

   
    /**
     * Function Create PDF and send email to respective company
     *
     * @param int    $intCompanyNid  : Company Node Id
     * @param string $strCompanyName : Company Name
     * @param array  $arrRecipient   : Recipients email array
     * @param array  $arrFMRInfo     : FMR Info
     *
     * @return bool|DatabaseStatementInterface|int|null
     */
    public function fnAffiliateReportsByCompany($intCompanyNid, $strCompanyName, $arrRecipient, $arrFMRInfo)
    {
        // Base Url
        global $base_url, $conf;
        $strTime = time();
        $strServerName = $_SERVER['HTTP_HOST'];

        $strSender = "info@3blmedia.com";
        $strSenderName = "3blmedia Team";       

        if (is_array($arrFMRInfo) && count($arrFMRInfo) >0) {
            $intFMRId = $arrFMRInfo['nid'];

            ## FMR Detail Report Attachment
            if ((int)$intFMRId >0) {
            
                // Build query URL
                $strUrl = http_build_query(array('fmr' => $intFMRId));

                $strMediaTitle = stripslashes($arrFMRInfo['title']);
                $strMediaUrl = $base_url."/".$arrFMRInfo['alias'];

                // Destination URL
                $strSrc1 = $base_url."/Dashboard/Analytics/Affiliate/PDF/".base64_encode($strUrl);
                $strApiKey = '31q8AjO7u0Ej';
                $strResultPDF = file_get_contents("http://api.htm2pdf.co.uk/urltopdf?apikey=$strApiKey&url=$strSrc1");
                
                 // PDF file path
                $strFilePath = "/var/www/vhosts/".$strServerName."/httpdocs/sites/default/files/pdf/TrackingResults_".$intFMRId."_".$intCompanyNid."_$strTime.pdf";
                $strAffiliatesDetailReport = $strFilePath;
                file_put_contents($strFilePath, $strResultPDF);
            
                $strHeaderImagePath = $base_url."/".$conf['IMAGES_PATH_3BL']."/ConfirmHeader.png";
                $strFooterImagePath = $base_url."/".$conf['IMAGES_PATH_3BL']."/ConfirmFooter.png";

                //Set Subject for report cron email on the basis of report type
                $strSubject = "3BL Media: Tracking Report";

                //Set the message for the email on the basis of report type
                $strMessage = "<div><img src='$strHeaderImagePath'/></div><br/>Attached to this message is the Tracking Report for your most recent* Flexible Media Release (FMR) distributed through the 3BL Media platform (<a href='$strMediaUrl'>".$strMediaTitle."</a>).<br/><br/>You can view and download additional Tracking Reports, as well as our full suite of Analytics, at any time via<br/> 
                  ".$base_url."/Dashboard/Analytics/Views  (you must be logged in)<br/><br/>If you have any questions, please contact your media consultant at 866.508.0993 or reply to this message.<br/><br/>Thank you,<br/>The 3BL Media Team<br/><br/>* FMRs must have been distributed at least 48 hours ago to be included.<br/><br/><div><img src='$strFooterImagePath'/></div>";

                //User list those who got report Email
                $arrReceivedReport = array();

                //Send email with attachments
                if (is_array($arrRecipient) && count($arrRecipient) >0) {
                    foreach ($arrRecipient as $strRecipientAddress) {
                        $boolSent = fnSendEmail($strSenderName, $strSender, $strRecipientAddress, $strSubject, $strMessage, $strAffiliatesDetailReport);

                        if ($boolSent) {
                            $arrReceivedReport[] = $strRecipientAddress;
                        }
                    }  
                }

                //Delete file after email those attachment.
                @unlink($strAffiliatesDetailReport);
                
                if (count($arrReceivedReport) > 0) {
                    //If mail sent successfuly then insert these details in the table
                    $strRecipients = implode(", ", $arrReceivedReport);

                    // Send Recipients information to Media Consultant
                    $strRecipientsInfoHTML = $this->fnGetRecipientsInfoHTML($strCompanyName, $strRecipients, $strMediaTitle, $strMediaUrl);

                    return $strRecipientsInfoHTML;
                }
            }
        }
    }

   

    /**
     * To Build Recipients information HTML (Company name and Email)
     *
     * @param string $strCompanyName : Company Name
     * @param string $strRecipients  : Comma separated email address
     * @param string $strMediaTitle  : FMR Tile
     * @param string $strMediaUrl    : FMR Url
     *
     * @return string
     */
    public function fnGetRecipientsInfoHTML($strCompanyName, $strRecipients, $strMediaTitle, $strMediaUrl)
    {
        //Include all required files
        module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReports');

        //Object of class
        $objReports = new ClsReports();

        $strGetMediaTitle = $objReports->fnRemoveWordFormatting($strMediaTitle);
        return $strHTML = "<tr><td>$strCompanyName</td><td>$strRecipients</td><td><a href='$strMediaUrl'>$strGetMediaTitle</a></td></tr>";
    }

    /**
     * To Send Recipients Information to Media Consultant
     *
     * @param string $strRecipientsHTML : Recipients Info HTML
     *
     * @return string
     */
    public function fnSendRecipientInfoToMCs($strRecipientsHTML)
    {
        $strCurrentDate = date('F j, Y');

        $intCurMonth = date("m", strtotime($this->strCurrentDate));
        $intCurYear = date("Y", strtotime($this->strCurrentDate));
        $intCurDay = date("d", strtotime($this->strCurrentDate));

        $strGetStartDate = date('F j, Y', mktime(0, 0, 0, $intCurMonth, $intCurDay - 9, $intCurYear));
        $strGetEndDate   = date('F j, Y', mktime(0, 0, 0, $intCurMonth, $intCurDay - 2, $intCurYear));
        
        $strHTML = "<table width='100%' cellpadding='0' cellspacing='0' border='1' >
                    <tr><td colspan='3' style='font-size:14px;''>Weekly Tracking Reports were sent on $strCurrentDate for the period from $strGetStartDate to $strGetEndDate.</td></tr>
                    <tr><td colspan='3'>&nbsp;</td></tr>
                    <tr><td><strong>Company Name</strong></td><td><strong>Recipients Email</strong></td><td><strong>FMR Title</strong></td></tr>".$strRecipientsHTML."</table>";

        // Get Server Name
        $strServerName = $_SERVER['HTTP_HOST'];
        //Created xls file Path
        $strRecipientsReport = "/var/www/vhosts/".$strServerName."/httpdocs/sites/default/files/excel/Tracking_Report_Recipients".time().".xls";

        // Open and write HTML content to xls file
        file_put_contents($strRecipientsReport, $strHTML);

        //Set Subject for report cron email on the basis of report type
        $strSubject = "Notice to MCs: Tracking Reports sent";

        //Set the message for the email on the basis of report type
        $strMessage =  "Dear MCs:<br/><br/>The weekly process to send Tracking Reports to clients was just completed, for the period from $strGetStartDate to $strGetEndDate. If a client published any FMRs in this period, they received a PDF of the Tracking Report for the most recent FMR in the period.  If a client did NOT have activity in the period, then no message was sent to them.<br /><br />We have attached a log of recipients - Company and email address(es) along with the relevant FMR - to this message.<br/><br/>Thank you,<br/>The 3BL Media Tech Team<br/>";

        $strRecipientMail = "dfulton@3blmedia.com, consult@3blmedia.com";

        // Send Monthly report
        fnSendEmail("3BL Media Admin", "no-reply@3blmedia.com", $strRecipientMail, $strSubject, $strMessage, $strRecipientsReport);

        //Unlink Micro List report excel file after mail sent
        if (is_file($strRecipientsReport)) {
            @unlink($strRecipientsReport);
        }
    }

    

    /**
     * Class destructor
     */
    public function __destruct()
    {

    }
}
