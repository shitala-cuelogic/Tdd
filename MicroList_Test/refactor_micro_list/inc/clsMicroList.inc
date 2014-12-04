<?php

module_load_include('inc', 'threebl_micro_list', 'inc/micro_listControllerInterface');

module_load_include('inc', 'threebl_micro_list', 'inc/clsMicroEmailList');

module_load_include('inc', 'refactor_micro_list', 'inc/clsMicroListController');

module_load_include('inc', 'refactor_micro_list', 'inc/clsMicroListDatabase');

class clsMicroList extends micro_listController implements micro_listControllerInterface
{
    public $arrEmailName;
    private $strBannerImagePath;
    private $intMaxNumOfClientUser;
    private $intMaxNumOfAdminUser;
    private $objMicroListController;
    private $objMicroListDB;

    public function clsMicroList()
    {
        global $conf;
        $this->arrEmailName = array('Email');
        $this->intMaxNumOfClientUser = 500;
        $this->intMaxNumOfAdminUser = 500;
        $this->strBannerImagePath = '/'. $conf['IMAGES_PATH_3BL'];
        $this->objMicroListController = new clsMicroListController();
        $this->objMicroListDB = new clsMicroListDatabase();
    }

    /**
     * Get the Max uploading user limit
     *
     * @param string $strListType : Micro List Type
     *
     * @return int
     */
    public function fnGetMaxUploadUserLimit($strListType = "")
    {
        if ($strListType == "3bl_micro_list") {
            return $this->intMaxNumOfAdminUser;
        } elseif ($strListType == "client_micro_list") {
            return $this->intMaxNumOfClientUser;
        }
    }

    /**
     * Delete from synchronize table
     *
     * @param int    $intListId  : List Id
     * @param string $arrUserIds : Array of User Ids
     *
     * @return bool
     */
    public function fnDeleteEmailList($intListId = 0, $arrUserIds = '')
    {
        if (count($arrUserIds) > 0) {
            foreach ($arrUserIds as $row) {
                $this->fnDeleteMicroListFieldItem($row, $intListId);
                $this->fnDeleteMicroListRevItem($row, $intListId);
            }
        }
        return true;
    }

    
    /**
     * Add User using email address
     *
     * @param Array $arrEmailDetails : Email Details
     */
    public function AddSubscriberUser($arrEmailDetails)
    {
        //Getting all the emails addresses
        $strFirstName = '';
        $strLastName = '';
        $strEmailAddress = '';

        if (!empty($arrEmailDetails)) {
            $strFirstName = utf8_decode($arrEmailDetails['fname']);
            $strLastName = utf8_decode($arrEmailDetails['lname']);
            $strEmailAddress = $arrEmailDetails['email'];

        }
        //if

        //Getting username For Add User
        $strUserName = trim(strstr($strEmailAddress, '@', true));

        //Build new user array.
        $arrNewsLetterUser = array(
            'name' => $strUserName . time(),
            'pass' => $strUserName, // unique password
            'mail' => trim($strEmailAddress), //  user email
            'init' => trim($strEmailAddress),
            'status' => 1,
            'roles' => array(12 => true),
            'field_real_name' => array(LANGUAGE_NONE => array(array('value' => trim($strFirstName . ' ' . $strLastName)))),
            'field_daily_news_alert' => array(LANGUAGE_NONE => array(array('value' => 0)))
        );

        //save the new user record
        $arrUser = $this->objMicroListDB->fnGetNewsUserRecord($arrNewsLetterUser);    
        
        return $arrUser->uid;
    }

   /**
     * Add Email List functionality for Admin and Client Side
     *
     * @param Array $arrPostValue  : Post Values
     * @param Array $arrFileData   : Upload File Data
     * @param int   $intCompanyNid : Company Node Id
     * @param int   $intOgId       : Company OG Id
     * @param int   $intEditId     : List Id
     *
     * @return int
     */
    public function fnAddAdminClientList($arrPostValue, $arrFileData, $intCompanyNid = 0, $intOgId = 0, $intEditId = 0)
    {
        extract($arrPostValue); //extract the post data

        //Checking client id if yee then add bundle type client
        if ($intCompanyNid > 0) {
            $strBundleType = 'client_micro_list';
            $intSubscriberLimit = $this->intMaxNumOfClientUser;
        } else {
            $strBundleType = '3bl_micro_list';
            $intSubscriberLimit = $this->intMaxNumOfAdminUser;
        }

        //Create an entity object to insert the affiliate into the db
        $objEntity = new stdClass();
        $objEntity->type = $strBundleType;

        ##Edit case
        if ($intEditId > 0) {
            $intMessage = 3; //edit message

            #Collect all information array for the list
            $objEntity->id = $intEditId;
            $objEntity->title = trim($title);
            $objEntity->field_email_list_details['und'][0]['value'] = trim($details);

            #Update data for the list
            $objEntity = $this->objMicroListDB->fnInsertList($objEntity);

            unset($objEntity);

            return $intMessage;

        } else { ## Add case.
            //checking file exist or not
            if ($arrFileData["emailfile"]['name'] != '') {

                $strFileName = $arrFileData["emailfile"]["name"]; // The file name
                $strFileTmpLoc = $arrFileData['emailfile']['tmp_name'];
                $arrFileName = explode(".", $strFileName); // Split file name into an array using the dot
                $strFileExt = end($arrFileName);

                $arrUserCsv = array();
                $arrUserXls = array();
                $intXlsCount = 0;
                $intNumSuccess = 0;

                #Checking file is xls or csv file validation
                if ($strFileExt == 'xls') {
                    list($intXlsCount, $intNumSuccess, $arrUserXls) = $this->fnXlsProcess($strFileTmpLoc);
                } else {
                    list($intNumSuccess, $arrUserCsv) = $this->fnCsvProcess($strFileTmpLoc);
                }

                if ((int) $intNumSuccess > 0) {
                    # get the List Title
                    $objEntity->title = $title;

                    #get the List details.
                    $objEntity->field_email_list_details['und'][0]['value'] = $details;

                    #get the list owner gid
                    $objEntity->field_client_micro_list_owner['und'][0]['value'] = $intOgId;
                    $objEntity->group_audience['und'][0]['gid'] = $intOgId;

                    #Insert email title into the eck_listmail
                    //Insert entity
                    $intListId = $this->objMicroListDB->fnInsertList($objEntity);

                    #Get List Name
                    $strListTitle = $objEntity->title;

                    unset($objEntity);
                }

                //Checking file is xls or csv file.
                if ($strFileExt == 'xls') {
                    //if excel is not empty.
                    if ($intXlsCount >= 1 && !empty($arrUserXls)) {
                        $intMessage = $this->fnGetXlsInfo($intListId, $arrUserXls);
                    } //if excel value exist or not end of excel code
                } else {

                    //if excel is not empty.
                    if (!empty($arrUserCsv)) {
                        $intMessage = $this->fnGetCsvInfo($intListId, $arrUserCsv);
                        
                    } //if csv value exist or not end of Csv code
                }
            }

            if ($intNumSuccess == 0) {
                $intNumSuccess = 0;
                $intMessage = 4;
            }
            #Create a session variable.
            $_SESSION['microList'][$strListTitle] = "Number of entries added: $intNumSuccess. If this number doesn't match the number in your list, either some were missing information (all fields are required) or email addresses were invalid.  If you have concerns, please contact your media consultant.";

            #Assign the message to the session variable which denotes number of subscribers add or failed.
            return $intMessage;
        }
    }
    
    function fnGetXlsInfo($intListId, $arrUserXls) {

        #for all the excel value start
        foreach ($arrUserXls as $arrUserXlsInfo) {

            #Assign Values to variables
            $strFirstName = trim($arrUserXlsInfo['strFirstName']);
            $strLastName = trim($arrUserXlsInfo['strLastName']);
            $strEmail = trim($arrUserXlsInfo['strEmail']);

            # Common function to check email already exits and add Subscriber users
            $intMessageNum = $this->fnCheckAddSubscriberUser($intListId, $strEmail, $strFirstName, $strLastName);

            # Check Message number
            if ($intMessageNum == 2) {
                $intMessage = $intMessageNum;
            }
        }

        return $intMessage;
    }

    function fnGetCsvInfo($intListId, $arrUserCsv) {

        //Getting existing emails with email id
        //Read the csv file at end of file
        foreach ($arrUserCsv as $strUsers) {
            $strFirstName = trim($strUsers['strFirstName']);
            $strLastName = trim($strUsers['strLastName']);
            $strEmail = trim($strUsers['strEmail']);

            # Common function to check email already exits and add Subscriber users
            $intMessageNum = $this->fnCheckAddSubscriberUser($intListId, $strEmail, $strFirstName, $strLastName);

            # Check Message number
            if ($intMessageNum == 2) {
                $intMessage = $intMessageNum;
            }
        }
        return $intMessage;
    }
    
    function fnXlsProcess($strFileTmpLoc) {

        $arrUserXls = array();
        $intXlsCount = 0;
        $intNumSuccess = 0;
        $intXlsLineCount = 0;

        //Include all required files
        module_load_include('inc', 'threebl_micro_list', 'inc/excel_reader');
        $objDataXls = new Spreadsheet_Excel_Reader($strFileTmpLoc);

        //Getting records array from excel file
        $arrXlsCount = $objDataXls->sheets[0]["cells"];

        #decrease by 1 as first row is a row of title and check the rest count for meet the target 1000.
        $intExistingUsers = count($arrXlsCount) - 1;

        ## Check existing number of user and number of user from the list > 1000
        if ($intExistingUsers > $intSubscriberLimit) {
            return 5;
        }

        $intXlsCount = $intExistingUsers;

        if ($intXlsCount >= 1) {

            #Remove first record from the array
            array_shift($arrXlsCount);

            #for all the excel value start
            foreach ($arrXlsCount as $key => $arrRecord) {
                #Assign Values to variables
                $strFirstName = trim($arrRecord[1]);
                $strLastName = trim($arrRecord[2]);
                $strEmail = trim($arrRecord[3]);

                $arrSubscribers = array('fname' => $strFirstName, 'lname' => $strLastName, 'email' => $strEmail);

                # Check validation for First name, last name and email address.
                $strIsValidate = $this->fnValidateSubscriber($arrSubscribers);
                // If all validate then proceed to further.
                if ($strIsValidate == "") {
                    #Add validate record into the array
                    $arrUserXls[$intXlsLineCount]['strFirstName'] = $strFirstName;
                    $arrUserXls[$intXlsLineCount]['strLastName'] = $strLastName;
                    $arrUserXls[$intXlsLineCount]['strEmail'] = $strEmail;

                    #increase the number of line number as read new line
                    $intXlsLineCount++;
                }
            }
            #assign the count of array as number of success.
            $intNumSuccess = count($arrUserXls);
        }

        return array($intXlsCount, $intNumSuccess, $arrUserXls);
    }

    function fnCsvProcess($strFileTmpLoc) {

        #open csv file.
        $objCsvFile = fopen($strFileTmpLoc, 'r');
        $intLineCount = 0;
        $intTotalCount = 0;

        while (!feof($objCsvFile) && $intSubscriberLimit >= $intTotalCount) {
            //Checking file contents
            $arrFile = fgetcsv($objCsvFile, 1024);

            $strFName = trim($arrFile[0]);
            $strLName = trim($arrFile[1]);
            $strEmail = trim($arrFile[2]);

            $arrSubscribers = array('fname' => trim($strFName), 'lname' => trim($strLName), 'email' => trim($strEmail));

            # Validate User Array.
            $strIsValid = $this->fnValidateSubscriber($arrSubscribers);

            if ($strIsValid == '' && !in_array($strEmail, $this->arrEmailName)) {
                #Add validate record into the array
                $arrUserCsv[$intLineCount]['strFirstName'] = $strFName;
                $arrUserCsv[$intLineCount]['strLastName'] = $strLName;
                $arrUserCsv[$intLineCount]['strEmail'] = $strEmail;

                #increase the number of line number as read new line
                $intLineCount++;
            }
            $intTotalCount++;
        }
        //While
        #assign the count of array as number of success.
        $intNumSuccess = count($arrUserCsv);

        fclose($objCsvFile);

        if ($intTotalCount > $intSubscriberLimit) {
            return 5;
        }
        return array($intNumSuccess, $arrUserCsv);
    }

    /**
     * Checking Session For Client Side
     *
     * @param int $intAdmin : Flag to check 3BL and client list
     *
     * @return bool
     */
    public function fnCheckSession($intAdmin = 0)
    {
        #Check if that admin
        if ($intAdmin == 1 && (int)$_SESSION['ADMIN_BLOCK'] != 1) {
            drupal_goto('');
        } elseif ((int)$_SESSION['client_og_nid'] <= 0 && $intAdmin == 0) { #Check for client
            drupal_goto('');
        }
        return true;
    }

   /**
     * Add Admin and Client email to database
     *
     * @param Array $arrPostData  : Post Data
     * @param int   $intListId    : List Id
     * @param int   $intCheckFlag : Checking type of list using flag.
     *
     * @return int
     */
    
    public function fnAddEmail($arrPostData, $intListId, $intNumOfExistingUsers, $intCheckFlag = 0)
    {
        #check whether new user exceed the limit.
        //$intNumOfExistingUsers = $this->fnGetCountOfUser($intListId);

        #Get Max limit for upload user.
        $intSubscriberLimit = ($intCheckFlag == 1) ? $this->intMaxNumOfClientUser : $this->intMaxNumOfAdminUser;

        ##Check whether user exceed the limit
        if ($intNumOfExistingUsers >= $intSubscriberLimit) {
            return 5;
        }

        extract($arrPostData); //extract the post data

        # Common function to check email already exits and add Subscriber users
        $intMessage = $this->fnCheckAddSubscriberUser($intListId, $email, $fname, $lname);
        return $intMessage;
    }

 
    /**
     * Sending the fmr details via email to the users which are in the 3bl micro list
     */
    public function fnSendFMRToCustomList()
    {
        #function for getting All fmr with their respective custom list.
        $objFMRResult = $this->objMicroListDB->fnGetAllPublishedFMR();

        global $base_url, $conf;

        #sender details name
        $strSendName = "3BL Media";
        $strSendEmail = $conf['no_reply_from'];

        

        // Current Date
        $strTodayDate = date('Y-m-d');

        // Checking FMR List is empty or not.
        if (!empty($objFMRResult)) {
            
            list($arrFMRHtml, $arrFMRIds, $arrFMRProcessingContact) = $this->fnGetFMRDetails($objFMRResult);
            
            #Get All fmrs and turn into the , separated string
            $strFMRIds = implode(',', $arrFMRIds);

            #get list ids and user information assigned for FMR
            $objGetRelListFMRIds = $this->objMicroListDB->fnGetFMRAssignListUserDetails($strFMRIds);
            $arrFMRAssignListUserInfo = $this->fnGetFMRAssignUserListInfo($objGetRelListFMRIds);

            $arrFMRAssignListId = $arrFMRAssignListUserInfo['arrListIds'];
            $arrFMRAssignListName = $arrFMRAssignListUserInfo['arrListName'];

            if (is_array($arrFMRAssignListId) && count($arrFMRAssignListId) > 0) {
                #Include module to use the send mail function
                module_load_include('inc', 'threebl_analytics_reports', 'inc/sendEmail');

                //Loop to get FMR List id and FMR array
                foreach ($arrFMRAssignListId as $intFMRListId => $arrListFMR) {
                    
                    list($strFMRBody, $arrProcessFMRId, $arrSendFMRContactInfo, $strProcessContactFMRBody) = $this->fnGetFMRBodyHtml($arrListFMR, $arrFMRProcessingContact, $arrFMRHtml);
                    
                    #Function to get user information assigned to list
                    $arrUserArrResult = $this->objMicroListDB->fnGetListUserInfo($intFMRListId);
                    $strListName = $arrFMRAssignListName[$intFMRListId];

                    #3BL Email Message
                    $str3BLMessage = 'You are receiving this email via 3BL Media because you are a thought leader with regard to ' . $strListName . ". If you don't wish to receive such influencer alerts in the future, please use the unsubscribe link in this message.";

                    # Processing contact Footer message
                    $str3BLContactMessage = 'This is a copy of the message sent to thought leaders with regard to "' . $strListName . '". You are receiving this copy because you are the Processing Contact. Please contact your Media Consultant at 3BL Media if you have any questions.';

                    #To build the final html body to fmr distribution.
                    $strFinalEmailBody = $this->objMicroListController->fnBuildFinalHtml($str3BLMessage, $strFMRBody, $strListName);

                    #To build the final html body to FMR Processing Contact.
                    $strProcessContactEmailBody = $this->objMicroListController->fnBuildFinalHtml($str3BLContactMessage, $strProcessContactFMRBody, $strListName, '', "processing_contact");

                    //Loop to get User information
                    foreach ($arrUserArrResult as $arrUserInfo) {

                        // Set 'View this issue on the web' url for user email template
                        $strUrlForIssue = $base_url.'/influencer-alert/'. $this->fnReplaceSpecialCharWithHypen($strListName).'-'.$strTodayDate.'.html?id='.base64_encode(json_encode($arrUserInfo->userid));
                        $strEmailBody = str_replace("[WEB_ISSUE_URL]", $strUrlForIssue, $strFinalEmailBody);

                        #Create Click FMR tracking URL and replace that into the email template
                        $strClickMicroListUrl = base64_encode(http_build_query(array('id' => $arrUserInfo->userid, 'listid' => $intFMRListId)));
                        $strClickTrackURL = $base_url."/Tracking-FMR-clicks?token=".$strClickMicroListUrl;
                        $strMicroClickMessage = str_replace("[strClickTrackingURL]", $strClickTrackURL, $strEmailBody);

                        #Create Open Email tracking URL and replace that into the email template
                        $strViewMicroListUrl = base64_encode(http_build_query(array('id' => $arrUserInfo->userid, 'date' => date('Y-m-d H:i:s'), 'mediaid' => $arrListFMR, 'listid' => $intFMRListId)));
                        $strOpenTrackURL = $base_url . "/Tracking-FMR-Updates?token=" . $strViewMicroListUrl;
                        $strEmailMessage = str_replace("[strOpenTrackURL]", $strOpenTrackURL, $strMicroClickMessage);

                        #Create Un-subscribed Code and replace that into the email template
                        $strCode = md5($arrUserInfo->userid . 'I Want Out'); //encode code.
                        $strUserCode = $arrUserInfo->userid . '/' . $strCode;
                        $strFinalEmailMessage = str_replace("[strUserCode]", $strUserCode, $strEmailMessage);

                        #sending email of FMR HTML
                        $strEmailAddress = $arrUserInfo->mail;

                        fnSendEmail($strSendName, $strSendEmail, $strEmailAddress, $strSubject = "Today's " . $strListName . " News", $strFinalEmailMessage, "", false);
                    }
                    watchdog('micro_list_debug','Sent to list');

                    # Send email to FMR Processing Contacts
                    if (is_array($arrSendFMRContactInfo) && count($arrSendFMRContactInfo) > 0) {
                        foreach ($arrSendFMRContactInfo as $arrProcessingContact) {
							if (!empty($arrProcessingContact[0]["email"])){
                            fnSendEmail($strSendName, $conf['consult_email_from'], $arrProcessingContact[0]["email"], $strSubject = "Today's " . $strListName . " News", $strProcessContactEmailBody, "", false);
                                watchdog('micro_list_debug','Sent to processing contact'.$arrProcessingContact[0]["email"]);
							}
                        }
                    }

                    #To build the final html body to View Html on the web.
                    $strEmailBodyOnWeb = $this->objMicroListController->fnBuildFinalHtml($str3BLContactMessage, $strFMRBody, $strListName, '', "on_web");

                    // Get Server Name
                    $strServerName = $_SERVER['HTTP_HOST'];

                    //Created HTML file Path
                    $strMicroListHtmlTemplate = "/var/www/vhosts/".$strServerName."/httpdocs/influencer-alert/".$this->fnReplaceSpecialCharWithHypen($strListName)."-".$strTodayDate.".html";

                    //Create Click FMR tracking URL and replace that into the email template
                    $strViewWebMicroListUrl = base64_encode(http_build_query(array('listid' => $intFMRListId)));
                    $strClickTrackURL = $base_url . "/Tracking-FMR-clicks?token=". $strViewWebMicroListUrl;
                    $strFinalEmailBodyOnWeb = str_replace("[strClickTrackingURL]", $strClickTrackURL, $strEmailBodyOnWeb);

                    // Open and write HTML content to HTML file
                    $fileHandler = fopen($strMicroListHtmlTemplate, 'w+');
                    fwrite($fileHandler, $strFinalEmailBodyOnWeb);
                    watchdog('micro_list_debug','Done with list: '.$intFMRListId);

                    // to track Micro Lists Processing
                    $arruniqueProcessFMRId = array_unique($arrProcessFMRId);
                    foreach($arruniqueProcessFMRId as $intGetFMRId) {
                        $this->objMicroListDB->fnTrackMicroListProcessing($intGetFMRId, $intFMRListId);
                    }
                }

                //foreach of users
                ## Loop to update flag 0 to 1 after sending email complete.
                foreach ($arrFMRIds as $intFMRIdValue) {
                    //updating fmr flag 0 to 1 after sending email complete.
                    $arrUpdate = node_load($intFMRIdValue);
                    $arrUpdate->field_3bl_micro_list_distributio['und'][0]['value'] = '1';

                    #Update field_custom_email_flag field to 1
                    node_save($arrUpdate);
                    unset($arrUpdate); //unset the node value
                }

            }
            //end of array of users
        } else {
            echo "No FMR exist.";
        }
        return true;
    }

    function fnGetFMRBodyHtml($arrListFMR, $arrFMRProcessingContact, $arrFMRHtml) {

        $strFMRBody = "";
        watchdog('micro_list_debug','List to send to: '.$intFMRListId);
        $arrProcessFMRId = array();
        $arrSendFMRContactInfo = array();
        $strProcessContactFMRBody = "";
        // Loop to create FMR HTML
        foreach ($arrListFMR as $intArrFMR) {
            watchdog('micro_list_debug','FMR to include in list: '.$intArrFMR);
            $strFMRBody = $strFMRBody . $arrFMRHtml[$intArrFMR];
            $arrProcessFMRId[] = $intArrFMR;

            $arrSendFMRContactInfo[] = $arrFMRProcessingContact[$intArrFMR];

            # Replace click tracking url with blank for processing contact
            $strProcessContactFMRBody = $strProcessContactFMRBody.str_replace('[strClickTrackingURL]&action='.base64_encode($intArrFMR).'&url=', '', $arrFMRHtml[$intArrFMR]);
        }
        return array($strFMRBody, $arrProcessFMRId, $arrSendFMRContactInfo, $strProcessContactFMRBody);
    }

    function fnGetFMRDetails($objFMRResult) {
        
        $arrFMRHtml = array();
        $arrFMRIds = array();
        $arrFMRProcessingContact = array();

        //foreach
        foreach ($objFMRResult as $arrFMRDetails) {

            watchdog('micro_list_debug','FMR to send to: '.$arrFMRDetails->nid);
            #Get the fmr Id
            $intFMRId = $arrFMRDetails->nid;
            $arrFMRIds[] = $intFMRId;

            //Sending array for FMR Value
            $arrFmrDetails['publishdate'] = $arrFMRDetails->publishdate;
            $arrFmrDetails['alias'] = $arrFMRDetails->alias;

            $objNode = node_load($intFMRId);

            #Get FMR all information related to create HTML
            $arrCompanyFMRDetails = $this->fnCompanyFMRDetails($objNode, $arrFmrDetails);

            # Processing Contact Information
            if (is_array($arrCompanyFMRDetails["strContactInfo"]["ProcessingContact"]) && count($arrCompanyFMRDetails["strContactInfo"]["ProcessingContact"]) > 0) {
                # Gather processing contact email information
                foreach ($arrCompanyFMRDetails["strContactInfo"]["ProcessingContact"] as $strFMRProcessingContactInfo) {
                    $arrFMRProcessingContact[$intFMRId][] = $strFMRProcessingContactInfo;
                }
            }

            #Get Respective FMR HTML
            $arrFMRHtml[$intFMRId] = $this->objMicroListController->fnBuildFMRDetailsHTML($arrCompanyFMRDetails);
        }

        return array($arrFMRHtml, $arrFMRIds, $arrFMRProcessingContact);
    }
    
    /**
     * Function to validate the post data in add and edit list.
     *
     * @param array $arrPostData : Post Data
     * @param array $arrFileData : Upload File Data
     *
     * @return int|string
     */
    public function fnValidateAddListFormData($arrPostData, $arrFileData = array())
    {
        ## Get values from the post data
        $strListTitle = trim($arrPostData['title']);
        $strListDetails = trim($arrPostData['details']);

        $strMessage = "";

        ##Check possible errors for the post array values
        if ($strListTitle == "" || $strListDetails == "") {
            $strMessage = "Please add title and description";
            return $strMessage;

        } elseif (count($arrFileData) > 0) {
            ## Get values from the file
            $strFileName = $arrFileData['emailfile']['name'];
            $strFileType = $arrFileData['emailfile']['type'];
            $intFileSize = $arrFileData['emailfile']['size'];

            $strFileExtesion = pathinfo($strFileName, PATHINFO_EXTENSION);
            if ($intFileSize == 0) {
                $strMessage = "Please upload the list";
                return $strMessage;
            } elseif ($strFileExtesion != 'xls' && $strFileExtesion != 'csv') {
                $strMessage = "Please upload the file of .xls and .csv format only";
                return $strMessage;
            } else {
                return $strMessage;
            }
        }
        return 0;
    }

    /**
     * Function for checking email validation.
     *
     * @param Array $arrPostData : Post Data
     *
     * @return string
     */
    public function fnValidateSubscriber($arrPostData)
    {
        $strUserFirstName = trim($arrPostData['fname']);
        $strUserLastName = trim($arrPostData['lname']);
        $strUserEmailId = trim($arrPostData['email']);
        $strMessage = "";

        ##Check possible validations.
        if ($strUserFirstName == "" || $strUserLastName == "") {
            $strMessage = "Please add first name and last name ";
            return $strMessage;
        } elseif ($strUserEmailId == "") {
            $strMessage = "Please add email address ";
            return $strMessage;
        } elseif (!filter_var($strUserEmailId, FILTER_VALIDATE_EMAIL)) {
            $strMessage = "Please add valid email address ";
            return $strMessage;
        } else {
            return $strMessage;
        }
    }


    /**
     * Function for getting all user from single or multipal FMR
     *
     * @param int    $intFMRIds   : FMR Ids
     * @param string $strListType : List Type
     *
     * @return array
     */
    public function fnGetFMRListUserDetails($intFMRIds, $strListType)
    {
        #Call function to get the lists related to the FMR
        $objEmailArrResult = $this->fnGetUserFromFmrId($intFMRIds, $strListType);

        #Declare the array to hold the user id and list id.
        $arrUserEmails = array();
        $intCount = 0;

        //if object array is empty
        if (!empty($objEmailArrResult)) {
            ##Loop to get the user emails
            foreach ($objEmailArrResult as $arrUsers) {
                $arrUserEmails[$arrUsers->userid] = $arrUsers->mail;
                $arrUserListId[$arrUsers->userid]['listid'] = $arrUsers->list_id;
                $intCount++;
            }
        }
        return array('arrUserEmails' => $arrUserEmails, 'arrUserListId' => $arrUserListId);
    }

    /**
     * Function for getting array for FMR HTML
     *
     * @param Object $objNode                  : Node Object
     * @param Array  $arrAdditionalInformation : Additional Information
     *
     * @return array
     */
    public function fnCompanyFMRDetails($objNode, $arrAdditionalInformation)
    {
        global $base_url;

        //--------Getting the Mail content---------------//
        $strCompanyName = '';
        $strCompanyUrl = '';
        $strCompanyImagePath = '';

        #Getting the fmr title.
        $strFMRTitle = $objNode->title;
        #Get the fmr published date
        $dateFMRPublishedDate = $arrAdditionalInformation['publishdate'];

        #Get the company id with respect to the fmr.
        $intCompanyId = $objNode->client_nid;

        #The fmr url.
        $strFMRUrl = $base_url . '/' . $arrAdditionalInformation['alias'];

        #Get the fmr Id
        $intFMRId = $objNode->nid;

        #Getting fmr typr of content.
        $strFMRTypeOfContent = $objNode->field_fmr_type_of_content['und'][0]['value'];

        #Getting fmr Contact Information .
        $strFMRContactName = $objNode->field_fmr_contact_name['und'][0]['value'];
        $strFMRContactOrganization = $objNode->field_fmr_contact_organization['und'][0]['value'];
        $strFMRContactPhoneNo = $objNode->field_fmr_contact_phone['und'][0]['number'];
        $strFMRPhoneNoExt = $objNode->field_fmr_contact_phone['und'][0]['extension'];
        $strFMRContactEmail = $objNode->field_fmr_contact_mail['und'][0]['email'];
        $strFMRContactUrl = $objNode->field_other_contact_link_1['und'][0]['url'];
        $arrFMROtherContact = $objNode->field_fmr_contact_other['und'];
        $arrFMRProcessingContact = $objNode->field_processing_mail['und'];

        $strFMRTypeOfContent = ($strFMRTypeOfContent == 'press_release') ? 'Press Release' : $strFMRTypeOfContent;

        #Get the fmr photo
        $strFMRPhotoUrl = '';

        if (!empty($objNode->field_fmr_photo)) {
            if ($objNode->field_fmr_photo['und'][0]['uri'] != '') {
                $strPhotoUri = trim(str_replace("public://images/", '', $objNode->field_fmr_photo['und'][0]['uri']));
                $strFMRPhotoUrl = "/media/styles/medium/public/images/" . $strPhotoUri;
            }
        }

        #Getting the fmr desc.
        $strFMRShortDesc = ''; $strFMRDetailsWithoutStrip='';
        if (trim($objNode->field_fmr_body['und'][0]['value']) != '') {
            $strFMRShortDesc = strip_tags($objNode->field_fmr_body['und'][0]['value'], '<p><br>');
            //Without stripDetails
            $strFMRDetailsWithoutStrip = $objNode->field_fmr_body['und'][0]['value'];
        }

        $strTweet = '';

        //FMR Other Details
        if ($objNode->field_fmr_tweet['und'][0]['value'] != '') {
            $strTweet = $objNode->field_fmr_tweet['und'][0]['value'];
        }

        // Video Images
        if ($objNode->field_video['und'][0]['thumbnailfile']->uri != '') {
            #Video Multimedia attached to the FMR
            $strPhotoName = str_replace("public://", '', $objNode->field_video['und'][0]['thumbnailfile']->uri);

            $strVideoImagePath = ($strPhotoName != '') ? "/media/styles/medium/public/" . $strPhotoName : '';
        }

        #Use the node load function for company to get the company details such as company url and company logo
        $objCompany = node_load($intCompanyId);

        #Getting the company name with respect to company.
        $strCompanyName = $objCompany->title;

        #Get the Company url
        $strCompanyUrl = $objCompany->field_client_website['und'][0]['url'];

        $strCompanyImagePath = '';

        #Get the company logo
        if ($objCompany->field_client_logo['und'][0]['uri'] != '') {
            $strCompanyPhotoUri = trim(str_replace("public://Clients/", '', $objCompany->field_client_logo['und'][0]['uri']));
            $strCompanyImagePath = "/media/styles/medium/public/Clients/" . $strCompanyPhotoUri;
        }

        # If fmr photo available use it otherwise use the company logo.
        $strFinalPhotoUrl = ($strFMRPhotoUrl == '') ? $strCompanyImagePath : $strFMRPhotoUrl;
        $strFinalPhotoUrl = $base_url . $strFinalPhotoUrl;

        //Summary of FMR
        $strFMRSummary = strip_tags($objNode->field_fmr_body['und'][0]['summary']);

        # Get Company Url
        $arrCompanyProfileUrl = $this->fnGetCompanyUrl($objNode->client_nid);
        $strCompanyProfileUrl = $arrCompanyProfileUrl[0]['alias'];

        #Creating the array of fmr and associate company details.
        $arrCompanyFMRDetails = array();
        $arrCompanyFMRDetails['intFmrId'] = $intFMRId;
        $arrCompanyFMRDetails['strFmrTitle'] = $strFMRTitle;
        $arrCompanyFMRDetails['strFmrUrl'] = $strFMRUrl;
        $arrCompanyFMRDetails['strFmrShortDesc'] = $strFMRShortDesc;
        $arrCompanyFMRDetails['strPublishDate'] = $dateFMRPublishedDate;
        $arrCompanyFMRDetails['strFinalPhotoUrl'] = $strFinalPhotoUrl;
        $arrCompanyFMRDetails['strCompanyName'] = $strCompanyName;
        $arrCompanyFMRDetails['strFmrTypeOfContent'] = $strFMRTypeOfContent;
        $arrCompanyFMRDetails['strCompanyUrl'] = $strCompanyProfileUrl;
        $arrCompanyFMRDetails['strSummary'] = $strFMRSummary;
        $arrCompanyFMRDetails['strFMRDetailsWithoutStrip']= $strFMRDetailsWithoutStrip;

        //image for socail Media Account and FMR details
        $arrCompanyFMRDetails['strFMRImage'] = $strFMRPhotoUrl;
        $arrCompanyFMRDetails['strCompanyImage'] = $strCompanyImagePath;
        $arrCompanyFMRDetails['strVideoImage'] = $strVideoImagePath;
        $arrCompanyFMRDetails['intCompanyNid'] = $intCompanyId;
        $arrCompanyFMRDetails['strTweet'] = $strTweet;

        $arrCompanyFMRDetails['strContactInfo'] = array(
            "ContactName" => $strFMRContactName,
            "ContactOrganization" => $strFMRContactOrganization,
            "ContactPhoneNo" => $strFMRContactPhoneNo,
            "ContactPhoneExt" => $strFMRPhoneNoExt,
            "ContactEmail" => $strFMRContactEmail,
            "ContactUrl" => $strFMRContactUrl,
            "OtherContact" => $arrFMROtherContact,
            "ProcessingContact" => $arrFMRProcessingContact
        );
        return $arrCompanyFMRDetails;
    }

    /**
     * Function For Sending FMR To Client List for Client Cron
     *
     * @param int    $intNodeId            : Node Id
     * @param Array  $arrCompanyFMRDetails : Company FMR Deatils
     * @param string $strListType          : List Type
     *
     * @return string
     */
    public function fnSendFmrToClientList($intNodeId, $arrCompanyFMRDetails, $strListType = 'client_micro_list')
    {
        global $base_url, $conf;

        #Include module to use the sendmail function
        module_load_include('inc', 'threebl_analytics_reports', 'inc/sendEmail');

        # To get all user from single or multipal FMR
        $arrListUserDetails = $this->fnGetFMRListUserDetails($intNodeId, $strListType);

        $arrEmails = $arrListUserDetails['arrUserEmails'];
        $arrListIds = $arrListUserDetails['arrUserListId'];

        $arrListId = array();

        // list is empty
        if (!empty($arrEmails)) {
            #sender details name
            $strSendName = $arrCompanyFMRDetails['strCompanyName'] . " via 3BL Media ";
            $strSendEmail = $conf['no_reply_from'];

            #Get Respective FMR HTML
            $strEmailBody = $this->fnBuildFMRDetailsHTML($arrCompanyFMRDetails, 'client_micro_list');

            #Client Email Message
            $strClientMessage = $arrCompanyFMRDetails['strCompanyName'] . ' has sent you this announcement via 3BL Media. If you don’t wish to receive such alerts in the future, please use the unsubscribe link in this message.';
            #To build the final html body to fmr distribution.
            $strEmailHTML = $this->fnBuildFinalHtml($strClientMessage, $strEmailBody, '', 'client_micro_list');
            if (!empty($arrEmails)) {
                foreach ($arrEmails as $intUserId => $strEmailAddress) {
                    #Assign UserId
                    $intListId = $arrListIds[$intUserId]['listid'];

                    $arrListId[] = $intListId;

                    #Create Click FMR tracking URL and replave that into the email template
                    $strClickMicroListUrl = base64_encode(http_build_query(array('id' => $intUserId, 'listid' => $intListId, 'mediaid' => array($intNodeId))));
                    $strClickTrackURL = $base_url . "/Tracking-FMR-clicks?token=".$strClickMicroListUrl."&action=";
                    $strMicroClickMessage = str_replace("[strClickTrackingURL]", $strClickTrackURL, $strEmailHTML);

                    #Create Open Email tracking URL and replace that into the email template
                    $strViewMicroListUrl = base64_encode(http_build_query(array('id' => $intUserId, 'date' => date('Y-m-d H:i:s'), 'mediaid' => array($intNodeId), 'listid' => $intListId)));
                    $strOpenTrackURL = $base_url . "/Tracking-FMR-Updates?token=" . $strViewMicroListUrl;
                    $strEmailUrlMessage = str_replace("[strOpenTrackURL]", $strOpenTrackURL, $strMicroClickMessage);

                    #Create Un-subscribed Code and replace that into the email template
                    $strCode = md5($intUserId . 'I Want Out'); //encode code.
                    $strUserCode = $intUserId . '/' . $strCode;
                    $strEmailAddress;
                    $strFinalEmailMessage = str_replace("[strUserCode]", $strUserCode, $strEmailUrlMessage);

                    #sending email of FMR HTML
                    fnSendEmail($strSendName, $strSendEmail, $strEmailAddress, $strSubject = $arrCompanyFMRDetails['strFmrTitle'], $strFinalEmailMessage);
                }
            }

            //to track Micro Lists Processing
            $arruniqueListId = array_unique($arrListId);
            foreach($arruniqueListId as $intGetListId) {
                $this->fnTrackMicroListProcessing($intNodeId, $intGetListId);
            }

            return $intNodeId;
        } else {
            return "No list assign";
        }
    }


    /**
     * Function to check email already exits and add Subscriber users
     *
     * @param int    $intListId    : List Id
     * @param string $strEmail     : Email
     * @param string $strFirstName : First Name
     * @param string $strLastName  : Last Name
     *
     * @return int
     */
    public function fnCheckAddSubscriberUser($intListId, $strEmail, $strFirstName, $strLastName)
    {
        //Function for checking email already exits or not.
        list($intUserId, $boolList) = $this->objMicroListDB->fnGetUserIdByEmail($intListId, $strEmail);

        //if email exists then
        if ($intUserId < 1) {
            //if not existing then add in email list table
            $arrSubscriber = array(
                'email' => trim($strEmail),
                'fname' => trim($strFirstName),
                'lname' => trim($strLastName)
            );
            #Insert Subscriber users.
            $intUserId = $this->AddSubscriberUser($arrSubscriber);
        }
        //else

        //if email id then add
        if ((int) $intUserId > 0 && $boolList == 0) {
            //insert value in threebl_synchronize_custom_email
            $this->objMicroListDB->fnAddSyncMicroList($intListId, $intUserId);
            $intMessage = 2; // Message for
        } else { //if email exists then
            $intMessage = 3;
        }
        //end else

        return $intMessage;
    }
   
    
    /**
     * Function to check date is in valid format or not
     *
     * @param $strDate
     *
     * @return bool
     */
    public function fnValidateDateFormat($strDate)
    {
        $arrDate = explode("-", $strDate);

        $strError = "";
        if (is_array($arrDate) && count($arrDate) >0) {
            $intYear   = $arrDate["0"];
            $intMonth = $arrDate["1"];
            $intDay   = $arrDate["2"];

            if (!checkdate($intMonth, $intDay, $intYear)) {

                $strError = "error";
            }
        } else {

            $strError = "error";
        }
        return $strError;
    }

    /**
     * Function fnReplaceSpecialCharWithHypen to replace special character with Hypen
     *
     * @param string $strInput : Input string
     *
     * @return string
     */
    public function fnReplaceSpecialCharWithHypen($strInput)
    {
        $strInput = str_replace("?", "", preg_replace("/[???]/", "-", utf8_decode($strInput)));
        return str_replace("?", "", utf8_decode(preg_replace("/[`_~!@#$%&,.:;?'<>\[\]{}()=\/\"\/\*\/\-\/\^\/\+\/\|\/\/ ]/", "-", $strInput)));
    }

    public function fnGetFMRAssignUserListInfo($objGetRelListFMRIds) {

        #Declare the array for the holds the values.
        $arrListIds = array();
        $arrListName = array();
        if (!empty($objGetRelListFMRIds)) {
            ##Loop to get the array of list Id where kye as list id and value as fmr id.
            foreach ($objGetRelListFMRIds as $lists) {
                # Create the array as index as the list id and value as fmr ids.
                $arrListIds[$lists->field_3bl_micro_list_target_id][] = $lists->entity_id;
                $arrListName[$lists->field_3bl_micro_list_target_id] = $lists->title;
            }
            return array('arrListIds' => $arrListIds, "arrListName" => $arrListName);

        }

        return "";


    }
}