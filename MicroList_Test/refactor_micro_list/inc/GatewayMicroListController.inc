<?php
module_load_include('inc', 'refactor_micro_list', 'inc/clsMicroListController');
module_load_include('inc', 'refactor_micro_list', 'inc/clsMicroListDatabase');
module_load_include('inc', 'refactor_micro_list', 'inc/pagination');

class GatewayMicroListController
{
	private $objMicroListController;
	private $objMicroListDB;

	public function __construct()
	{
		$this->objMicroListController = new clsMicroListController();
		$this->objMicroListDB = new clsMicroListDatabase();
	}
	
	public function fnProcessManageListForm()
	{
		
		#Check Admin Session
		$this->objMicroListController->fnCheckSession(1);

		#Get the Max uploading user limit
		$intMaxUserLimit = $this->objMicroListController->fnGetMaxUploadUserLimit("3bl_micro_list");

		//Checking the post data
		if (isset($_POST['submitList'])) {
			$arrListValue = $_POST;
			$arrFileValue = $_FILES;

			# Set POST values in session
			$_SESSION['microList']['title'] = trim($_POST['title']);
			$_SESSION['microList']['details'] = trim($_POST['details']);


			$intOgId = 0;
			//Add functionality of form
			$intStatusMessage = $this->objMicroListController->fnAddAdminClientList($arrListValue, $arrFileValue, 0, $intOgId);
			if ($intStatusMessage == 5) {

				drupal_set_message("The maximum number of contacts per list is " . $intMaxUserLimit . ". If you have a need for more than " . $intMaxUserLimit . " in a list, please contact your media consultant. Thank you.", "error");
			} elseif ($intStatusMessage == 2) {
				drupal_set_message("Your list, " . trim($_POST['title']) . ", has been added successfully");
			} elseif ($intStatusMessage == 4) {
				drupal_set_message("Records in the uploaded file are not in proper format . Please check sample CSV or Xls files", "error");
			}
			drupal_goto('admin/3bl-micro-list');
		}
		//end of post

		#File not readable redirect to same page with error message.
		if (isset($_SESSION['microList']['read']) && $_SESSION['microList']['read'] != "") {
			$strMessage = $_SESSION['microList']['read'];

			if (isset($_SESSION['microList']['title']) && $_SESSION['microList']['title'] != "") {
				$strListTitle = $_SESSION['microList']['title'];
			}

			if (isset($_SESSION['microList']['details']) && $_SESSION['microList']['details'] != "") {
				$strListDetails = $_SESSION['microList']['details'];
			}

			unset($_SESSION['microList']['title']);
			unset($_SESSION['microList']['details']);

			drupal_set_message($strMessage, "error");
		}
		unset($_SESSION['microList']['read']);

		$arrListData = array('strListTitle'  => $strListTitle, 'strListDetails'=> $strListDetails, "intMaxUserLimit"=> $intMaxUserLimit);
		
		return $arrListData;

	}
	
	/**
	 * Delete Email from particular list
	 *
	 * @param int $intListId  : List Id
	 * @param int $intEmailId : Micro List entity Id
	 */
	function fnProcessDeleteEmail($intListId, $intEmailId)
	{
		#Check Admin Session
		$this->objMicroListController->fnCheckSession(1);
		
		if ($intListId < 1) {
			drupal_goto('/');
		}
		
		#Delete Email information
		$this->objMicroListDB->fnDeleteEmailList($intListId, $intEmailId);
		
		return true;
	}
	
	/**
	 * Add user email details using form
	 *
	 * @param int $intListId : List Id
	 *
	 * @return string
	 */
	function fnProcessAddEmailByAdmin($intListId)
	{
		
		#Check Admin Session
		$this->objMicroListController->fnCheckSession(1);
		
		if ($intListId < 1) {
			drupal_goto('/');
		}
		
		//Checking the post data
		if (isset($_POST['submitEmail'])) {
			$arrPostData = $_POST;
			#Add Admin and Client email to database
			
			#check whether new user exceed the limit.
			$intNumOfExistingUsers = $this->objMicroListDB->fnGetCountOfUser($intListId);
			
			$intMessage = $this->objMicroListController->fnAddEmail($arrPostData, $intListId,$intNumOfExistingUsers);
		
			// set falg in  session 1 for success message.
			$_SESSION['microList']['flag'] = $intMessage;
			drupal_goto('admin/3bl_micro_list/' . $intListId . '/');
		
		}
		//end of post
		
		// To get the id name of the list
		$arrListDetails = entity_load("micro_list", array($intListId));
		$strListName = stripslashes($arrListDetails[$intListId]->title);
		$arrListData = array('listName'=> $strListName);
		
		return $arrListData;
		
	}
	
	/**
	 * Function for getting Client side list
	 *
	 * @return string
	 */
	public function fnProcessClientMicroList()
	{
		//Check Session
		$this->objMicroListController->fnCheckSession();
		
		global $base_url;
		
		#Get the company related information.
		$intCompanyOgId = $_SESSION['client_og']; //Company og
		$intCompanyNid = $_SESSION['client_og_nid']; // Company node id
		
		# For display success or error message.
		$intFlag = (int) $_SESSION['microList']['flag'];
		unset($_SESSION['microList']['flag']);
		
		//check massage if flag 1
		switch ($intFlag) {
			case 1:
				$intFlag = 1;
				$strMessage = 'Your email list has been removed successfully.';
				break;
		
			case 2:
				$intFlag = 2;
				# Get information about the total records as well as successfully added and skipped records from uploaded list.
				$strSubscriberStatus = $_SESSION['microList'][$intCompanyOgId];
				unset($_SESSION['microList'][$intCompanyOgId]);
		
				$strMessage = 'Your list has been added and should be in the list, below.' . $strSubscriberStatus;
				break;
		
			case 3:
				$intFlag = 3;
				$strMessage = 'Your email list has been successfully updated.';
				break;
		
			case 4:
				$intFlag = 4;
				$strMessage = 'Records in the uploaded file are not in the proper format. Please check sample CSV or Xls files for the correct format.  And/or contact your media consultant for assistance.  Thank you!';
				break;
		
			case 5:
				$intFlag = 5;
				#Get the Max uploading user limit
				$intMaxUserLimit = $this->objMicroListController->fnGetMaxUploadUserLimit("client_micro_list");
				$strMessage = "The maximum number of contacts per list is " . $intMaxUserLimit . ". If you have a need for more than " . $intMaxUserLimit . " in a list, please contact your media consultant. Thank you.";
				break;
		
			default:
				$intFlag = 0;
				$strMessage = '';
		
		}
		
		#Manage all pagination related parameters
		$intLimit = 25;
		$intNext = 0;
		$intPage = 1;
		$intPrev = 0;
		$intStart = 0;
		$strType = 'client';
		
		//Get all client micro list of Particular Company
		list($objArrList, $intTotalRecords) = $this->objMicroListDB->fnGetListByCompanyId($intCompanyOgId, 2, $intStart, $intLimit, $strType, "");
		
		#Calculate total number of pages.
		$intNumPages = ceil($intTotalRecords / $intLimit);
		if ($intNumPages == $intPage) {
			$intNext = 1;
		}
		
		//check for 'previous' option in pagination
		if ($intPage == 1) {
			$intPrev = 1;
		}
		
		#Assign Values to pass HTML template
		$arrListData = array('arrList'=> $objArrList, 'strBaseUrl'=> $base_url, 'intFlag'=> $intFlag, 'strMessage'=> $strMessage, 'intCompanyNid'=> $intCompanyNid, 'intNext'=> $intNext, 'intPrev'=> $intPrev, 'intPage'=> $intPage, 'strType'=> $strType, 'intLastPage'=> $intNumPages);
		
		return $arrListData;
		
	}
	
	
	public function fnProcessManageClientListForm($strListId = "")
	{
		//Checking session exist or not
		$this->objMicroListController->fnCheckSession();
		
		global $base_url;
		
		#Get Company Ids from session
		$intCompanyNid = $_SESSION['client_og_nid'];
		$intCompanyOgId = $_SESSION['client_og']; // Company node id
		
		//Get List id
		$intListId = (int) ($strListId != "") ? base64_decode($strListId) : 0;
		
		$strListTitle = '';
		$strListDetails = '';
		
		//if edit form details
		if ($intListId > 0) {
		
			//Checking is it client list or not.
			$this->objMicroListDB->fnCheckCountListId($intListId);
		
			//Getting list title and details by edit id
			$arrListDetails = entity_load("micro_list", array($intListId));
			$strListTitle = stripslashes($arrListDetails[$intListId]->title);
			$strListDetails = stripslashes($arrListDetails[$intListId]->field_email_list_details["und"][0]["value"]);
		}
		
		if (isset($_POST['submitList'])) {
			$arrListValue = $_POST;	
			$arrFileValue = $_FILES;
		
			//Add functionality of form
			$strValidateFormFields = ($intListId > 0) ? $this->objMicroListController->fnValidateAddListFormData($_POST) : $this->objMicroListController->fnValidateAddListFormData($_POST, $_FILES);
		
			if ($strValidateFormFields != "") {
				$strListTitle = $_POST['title'];
				$strListDetails = $_POST['details'];
				//check massage if flag 1
				$intFlag = 5;
				$strMessage = $strValidateFormFields;
		
			} else {
				$strListTitle = $_POST['title'];
		
				# Set POST values in session
				$_SESSION['microList']['title'] = $_POST['title'];
				$_SESSION['microList']['details'] = $_POST['details'];
		
				$intListId = $this->objMicroListController->fnAddAdminClientList($arrListValue, $arrFileValue, $intCompanyNid, $intCompanyOgId, $intListId);
				if ($intListId == 0) {
					$_SESSION['microList']['flag'] =  "2";
				} elseif ($intListId == 2) {
					$_SESSION['microList']['flag'] = 2;
					$_SESSION['microList'][$intCompanyOgId] = $_SESSION['microList'][$strListTitle];
					unset($_SESSION['microList'][$strListTitle]);
		
					#Include module to use the sendmail function
					module_load_include('inc', 'threebl_analytics_reports', 'inc/sendEmail');
		
					$objArrCompanyDetails = node_load($intCompanyNid);
					$strCompanyName = $objArrCompanyDetails->title;
		
					#sender details name
					$strTo = "microlist@3blmedia.com";
					$strSendName = "3blmedia Team";
					$strSendEmail = "noreply@3blmedia.com";
		
					#Email Message
					$strFinalEmailMessage = "Micro List Name: " . $strListTitle;
					$strFinalEmailMessage .= "<br />Company Name: " . $strCompanyName;
					$strFinalEmailMessage .= "<br />Added Date: " . date("d F Y");
		
					//Send Email to Consult Team
					fnSendEmail($strSendName, $strSendEmail, $strTo, $strSubject = $strListTitle . " micro list added", $strFinalEmailMessage);
		
				} elseif ($intListId == 5) {
					$_SESSION['microList']['flag'] = 5;
				} elseif ($intListId == 4) {
					$_SESSION['microList']['flag'] = 4;
				} else {
					$_SESSION['microList']['flag'] = 3;
				}
		
				drupal_goto('Dashboard/ClientMicroList/');
			}
		
		}
		//end of post
		
		#File not readable redirect to same page with error message.
		if (isset($_SESSION['microList']['read']) && $_SESSION['microList']['read'] != "") {
			$strMessage = $_SESSION['microList']['read'];
		
			if (isset($_SESSION['microList']['title']) && $_SESSION['microList']['title'] != "") {
				$strListTitle = $_SESSION['microList']['title'];
			}
		
			if (isset($_SESSION['microList']['details']) && $_SESSION['microList']['details'] != "") {
				$strListDetails = $_SESSION['microList']['details'];
			}
		
			unset($_SESSION['microList']['title']);
			unset($_SESSION['microList']['details']);
		
			$intFlag = 6;
		}
		unset($_SESSION['microList']['read']);
		
		#Get the Max uploading user limit
		$intMaxUserLimit = $this->objMicroListController->fnGetMaxUploadUserLimit("client_micro_list");
		
		$arrListData = array('intListId'   => $intListId, 'strBaseUrl'=> $base_url, 'strListTitle'=> $strListTitle, 'strListDetails'=> $strListDetails, 'intFlag'=> $intFlag, 'strMessage'  => $strMessage, "intMaxUserLimit"=> $intMaxUserLimit);
		
		return $arrListData;
		
	}
	
	public function fnProcessViewEmailList($intListId = 0)
	{
		
		//Check Session
		$this->objMicroListController->fnCheckSession(1);
		
		if ($intListId < 1) {
			drupal_goto('admin/3bl-micro-list');
		}
		
		$strURLPath = "/admin/3bl_micro_list/";
		$intFlag = $_SESSION['microList']['flag'];
		unset($_SESSION['microList']['flag']);
		//check massage if flag 1
		switch ($intFlag) {
			case 1:
				$intFlag = 1;
				$strMessage = 'The entry was successfully removed.';
				break;
		
			case 2:
				$intFlag = 2;
				$strMessage = 'The entry was successfully added.';
				break;
		
			case 3:
				$intFlag = 3;
				$strMessage = 'This email address already exists in this list.';
				break;
		
			case 5:
				$intFlag = 3;
		
				#Get the Max uploading user limit
				$intMaxUserLimit = $this->objMicroListController->fnGetMaxUploadUserLimit("3bl_micro_list");
				$strMessage = "The maximum number of contacts per list is " . $intMaxUserLimit . ". If you have a need for more than " . $intMaxUserLimit . " in a list, please contact your media consultant. Thank you.";
				break;
		
			default:
				$intFlag = 0;
				$strMessage = '';
		
		}//end switch case
		
		global $base_url;
		
		$strSQL = '';
		$intLimit = 100;
		$intStart = 0;
		
		//array for order
		$arrSort = array(
				'name'   => 'asc',
				'title'  => 'asc',
				'mail'   => 'asc',
		);
		
		//Checking Get method value for order of query.
		if (isset($_GET['order']) && $arrSort[$_GET['order']] != "") {
			$strSQL .= " ORDER BY " . $_GET['order'] . " " . $_GET['sort'];
			$arrSort[$_GET['order']] = ($_GET['sort'] == 'asc') ? 'desc' : 'asc';
		} else {
		
			$strSQL .= " ORDER BY us.mail ASC ";
		}
		
		//Checking Get method value for page value.
		if (isset($_GET['page']) && (int) ($_GET['page']) > 0) {
			$intStart = $intLimit * ($_GET['page'] - 1);
		}
		
		$strSQL .= " LIMIT " . $intStart . "," . $intLimit;
		
		#Include required files
		module_load_include('inc', 'threebl_micro_list', 'inc/pagination');
		
		// post value for delete form.
		if (isset($_POST['delete'])) {
			$arrPostData = $_POST;
			$arrEmailId = $arrPostData['emailid'];
			// email-id
			if (!empty($arrEmailId)) {
				$this->objMicroListDB->fnDeleteEmailList($intListId, $arrEmailId);
			}
			//if email id
			$_SESSION['microList']['flag'] = 1;
			drupal_goto('admin/3bl_micro_list/' . $intListId . '/');
		
		}
		//end of post
		
		//Function for Count all the list email number
		$intCountEmail = $this->objMicroListDB->fnEmailList($intListId, 1, '', 1);
		
		//Pagination function.
		$strPage = fnGetPagerData($intCountEmail, $intLimit, $strTempAction = $base_url . $strURLPath . $intListId . "?");
		
		//function for getting email list
		$arrObjEmailList = $this->objMicroListDB->fnEmailList($intListId, 0, $strSQL, 1);
		
		$arrListDetails = entity_load("micro_list", array($intListId));
		$strListTitle = stripslashes($arrListDetails[$intListId]->title);
		$strListType = stripslashes($arrListDetails[$intListId]->type);
		
		$arrEmailList = array('strBaseUrl'  => $base_url, 'strSortUrl'=> $base_url . $strURLPath, 'arrEmailList'=> $arrObjEmailList, 'intListId'=> $intListId, 'arrSort'=> $arrSort, 'page' => $strPage, 'intFlag'=> $intFlag, 'strMessage'=> $strMessage, 'strListTitle'=> $strListTitle, 'strListType'=> $strListType);
		
		return $arrEmailList;
		
	}
	
	/**
	 *  Delete client list
	 *
	 * @param string $strListId :  Base64 encoded List Id
	 */
	public function fnProcessDeleteClientList($strListId = "")
	{
		//Check Session
		$this->objMicroListController->fnCheckSession();
		
		//Get List id
		$intListId = (int) ($strListId != "") ? base64_decode($strListId) : 0;
		
		if ($intListId < 1) {
			drupal_goto('Dashboard/ClientMicroList');
		}
		
		//Checking is it client list or not.
		$this->objMicroListDB->fnCheckCountListId($intListId);
		//delete lis by listId
		$this->objMicroListDB->fnDeleteListByClient($intListId);
		
		return true;
	}
	
	/**
	 * Getting all the email of particular list of particular client
	 *
	 * @param int $intListId : List Id
	 *
	 * @return string
	 */
	function fnProcessClientEmailList($intListId)
	{
		
		#Check Client Session
		$this->objMicroListController->fnCheckSession();
		
		global $base_url;
		$intListId = (int) base64_decode($intListId);
		
		//Checking is it client list or not.
		$this->objMicroListDB->fnCheckCountListId($intListId);
		
		$intCompanyOgId = $_SESSION['client_og']; // Company group id.
		
		if (isset($_POST['delete'])) {
		
			$arrEmailId = $_POST['emailid'];
		
			// email-id
			if (!empty($arrEmailId)) {
				$intFlag = $this->objMicroListDB->fnDeleteEmailList($intListId, $arrEmailId);
				$_SESSION['microList']['flag'] = $intFlag;
			}
		}
		
		//Capture Message flag and reset the flag variable
		$intFlag = (int) $_SESSION['microList']['flag'];
		unset($_SESSION['microList']['flag']);
		
		#Assign Message as per the given message flag
		switch ($intFlag) {
			case 1:
				$intFlag = 1;
				$strMessage = 'The entry was successfully removed.';
				break;
		
			case 2:
				$intFlag = 2;
				$strMessage = 'The entry was successfully added.';
				break;
		
			case 3:
				$intFlag = 3;
				$strMessage = 'This email address already exists in this list.';
				break;
		
			case 5:
				$intFlag = 5;
		
				#Get the Max uploading user limit
				$intMaxUserLimit = $this->objMicroListController->fnGetMaxUploadUserLimit("client_micro_list");
				$strMessage = "The maximum number of contacts per list is " . $intMaxUserLimit . ". If you have a need for more than " . $intMaxUserLimit . " in a list, please contact your media consultant. Thank you.";
				break;
		
			default:
				$intFlag = 0;
				$strMessage = '';
		
		}
		
		// Getting the list of Particular Company
		$objArrList = $this->objMicroListDB->fnClientEmailListById($intListId, $intCompanyOgId);
		
		// To get the id name of the list
		$arrListDetails = entity_load("micro_list", array($intListId));
		$strListName = stripslashes($arrListDetails[$intListId]->title);
		$intListOwner = (int) stripslashes($arrListDetails[$intListId]->field_client_micro_list_owner["und"][0]["value"]);
		
		$arrListData = array('arrList' => $objArrList, 'intListId'=> $intListId,
				'strPage' => theme('pager', array('tags' => array())), 'strBaseUrl'=> $base_url,
				'listName'=> $strListName, 'intFlag'=> $intFlag, 'strMessage'=> $strMessage, 'intListOwner'=> $intListOwner,
				'intOgId' => $intCompanyOgId);
		
		
		
		return $arrListData;
		
		
	}
	
	/**
	 * delete email from list by client
	 *
	 * @param int $intListId  : List Id
	 * @param int $intEmailId : Micro List entity Id
	 */
	function fnProcessDeleteClientEmail($intListId, $intEmailId)
	{
		//Check Session
		$this->objMicroListController->fnCheckSession();
		//Checking is it client list or not.
		$this->objMicroListDB->fnCheckCountListId((int) $intListId);
		$this->objMicroListDB->fnDeleteEmailList((int) $intListId, (int) $intEmailId);
		
		return true;
	}
	
	/**
	 * Add user email details using form
	 *
	 * @param int $intListId :  List Id
	 *
	 * @return string
	 */
	function fnProcessAddEmailByClient($intListId)
	{
		//Checking session
		$this->objMicroListController->fnCheckSession();
		
		global $base_url;
		$intListId = (int) base64_decode($intListId);
		
		//Checking is it client list or not.
		$this->objMicroListDB->fnCheckCountListId($intListId);
		$intOgId = $_SESSION['client_og'];
		
		$strUserFirstName = '';
		$strUserLastName = '';
		$strUserEmailId = '';
		
		//Checking the post data
		if (isset($_POST['submitEmail'])) {
			$arrPostData = $_POST;
			$strValidateMessage = trim($this->objMicroListController->fnValidateSubscriber($arrPostData));
			if ($strValidateMessage != "") {
				$strUserFirstName = trim($_POST['fname']);
				$strUserLastName = trim($_POST['lname']);
				$strUserEmailId = trim($_POST['email']);
				//check massage if flag 1
				$intFlag = 4;
				$strMessage = $strValidateMessage;
		
			} else {
				// Adding flag for Client micro list to check subscriber limit.
				$intMessage = $this->objMicroListController->fnAddEmail($arrPostData, $intListId, 1);
				// set flag in  session 1 for success message.
				$_SESSION['microList']['flag'] = $intMessage;
				drupal_goto('Dashboard/SubscriberList/' . base64_encode($intListId));
			}
		}
		//end of post
		
		// To get the id name of the list
		$arrListDetails = entity_load("micro_list", array($intListId));
		$strListName = stripslashes($arrListDetails[$intListId]->title);
		
		$arrListData = array('intListId'     => $intListId, 'strBaseUrl'=> $base_url,
				'listName'      => $strListName, 'strUserFirstName'=> $strUserFirstName, 'strUserLastName'=> $strUserLastName,
				'strUserEmailId'=> $strUserEmailId,
				'intFlag'       => $intFlag, 'strMessage'=> $strMessage);
		
		return $arrListData;
		
	}
	
	public function fnProcessEntityDelete($entity)
	{
		//Checking session
		$this->objMicroListController->fnCheckSession();
		
		$arrEntity = json_decode(json_encode($entity), 1); //entity object
		$strEntityType = $arrEntity['entity_type']; //entity type
		
		if ($strEntityType == 'micro_list') 
		{
			$intEntityId = (int) $arrEntity['etid'];
			//Delete list record from synchronize table
			$this->objMicroListDB->fnDeleteEmailList($intEntityId);
		}
		
		return true;
	}
	
	public function fnProcessDistributeFMRToCustomList()
	{
		//Send FMR Html
		$this->objMicroListController->fnSendFMRToCustomList();
		return true;
	}
	
	/**
	 * Function list-out all micro list assigned by admin and whose owner is other than login client.
	 *
	 * @return mixed
	 */
	function fnProcess3BLMicroList()
	{
		//Checking session
		$this->objMicroListController->fnCheckSession();
		
		global $base_url;
		
		//check massage if flag 1
		$intOgId = $_SESSION['client_og']; //Company Og Id
		//Get all client micro list of Particular Company
		list($objArrList) = $this->objMicroListDB->fnGetListByCompanyId($intOgId, 0);
		
		$arrListData = array('arrList'  => $objArrList, 'strPage'=> theme('pager', array('tags' => array())), 'strBaseUrl'=> $base_url);
		
		return $arrListData;
	}
	
	/**
	 * Function to use the ajax pagination for client micro list
	 */
	function fnProcessCustomListAjax()
	{
		//Check Session
		$this->objMicroListController->fnCheckSession();
		
		#Define variables
		$intPage = 0;
		$strSearchVal = "";
		
		#Assigned all posted values.
		if (isset($_POST['type'])) {
			$strType = $strPostType = $_POST['type']; // Micro list type
			$intPage = $_POST['page'] ? $_POST['page'] : 1; //Set default page number
			$strSearchVal = trim($_POST['searchKey']); //Used the search key value entred in the search box.
		}
		
		$intOgId = $_SESSION['client_og']; //Company Og id
		//Getting the list of Particular Company
		$intLimit = 25; //Set limit of MICRO LIST per page
		$intStart = ($intPage - 1) * $intLimit; //Calculate starting index of page
		
		$strFlag = "client"; // Micro list type flag
		#Get all client micro list of Particular Company
		list($objArrList, $intTotalRecords) = $this->objMicroListDB->fnGetListByCompanyId($intOgId, 2, $intStart, $intLimit, $strFlag, $strSearchVal);
		
		$intNext = 0;
		$intPrev = 0;
		//check next option;
		$intNumPages = ceil($intTotalRecords / $intLimit);
		if ($intNumPages == $intPage) {
			$intNext = 1;
		}
		
		//check for last option
		if ($intPage == 1) {
			$intPrev = 1;
		}
				 
		$arrData = array('arrList'=> $objArrList, 'intNext'=> $intNext,  'intPrev'=> $intPrev, 'intPage'=> $intPage, 'strType'=> $strType, 'intLastPage'=> $intNumPages);
		
		return $arrData;
	}
	
	/**
	 * function return list of all active client for Ajax [used at admin functionality]
	 */
	function fnProcessActiveClientAjax()
	{
		//Checking session
		$this->objMicroListController->fnCheckSession(1);
		
		//Getting all active company
		$arrActiveCompany = json_encode($this->objMicroListDB->fnGetAllActiveClient());
		return $arrActiveCompany;
	}
	
	/**
	 * Function to get client Micro List by ajax
	 */
	function fnProcessGetClientMicroListsAjax()
	{
		//Checking session
		$this->objMicroListController->fnCheckSession();
		
		$intFlag = (int) $_POST['intflag'];
		$intOgId = $_SESSION['client_og']; //Client Og Id
		
		//Get all client micro list of Particular Company
		list($objArrList) = $this->objMicroListDB->fnGetListByCompanyId($intOgId, $intFlag);
		
		$arrAllMicroListArray = array();
		foreach ($objArrList as $row) {
			$arrAllMicroListArray[] = $row->id;
		}
		$arrClientList = json_encode($arrAllMicroListArray);
		
		return $arrClientList;

		
	}
	
	/**
	 * Function to track the fmr view and clicks.
	 */
	function fnProcessTrackFmr($strToken, $arrFMRView)
	{
		//parse Url
		parse_str($strToken, $arrTokenFMRClick);
		
		if (!empty($arrTokenFMRClick['mediaid'])) {
			//Take array for multipal fmr Id's
			foreach (($arrTokenFMRClick['mediaid']) as $intMediaId) {
				$arrFMRView['userid'] = (int) $arrTokenFMRClick['id'];
				$arrFMRView['list_id'] = (int) $arrTokenFMRClick['listid']; // FMR List Id
				$arrFMRView['tracking_flag'] = 'open'; // Tracking flag click : click FMR
				$arrFMRView['action_date'] = $arrTokenFMRClick['date']; // Assign current date
				$arrFMRView['fmr_nid'] = (int) $intMediaId;
		
				//Checking duplicate entry exist or not.
				$intRowCount = $this->objMicroListDB->fnCheckFmrTrackValue($arrFMRView);
		
				if ($intRowCount < 1) {
					//Function for insert tracking details for view
					$this->objMicroListDB->fnInsertFmrUpdateTracking($arrFMRView);
				}
			}
		}

	}
	
	/**
	* Function for tracking click of FMR Updates
	*/
	function fnProcessTrackFmrClicks($strToken, $arrFMRClick)
	{
		//parse Url
		parse_str($strToken, $arrTokenFMRClick);
		$arrFMRClick['userid'] = (int) ($_GET['id'] != "") ? json_decode(base64_decode($_GET['id'])) : $arrTokenFMRClick['id'];
		if ($arrFMRClick['userid'] >0) {
			$intMediaId = ($_GET['action'] != '') ? base64_decode($_GET['action']) : $arrTokenFMRClick['mediaid'][0];
			$arrFMRClick['list_id'] = (int) $arrTokenFMRClick['listid']; // FMR List Id
			$arrFMRClick['tracking_flag'] = 'click'; // Tracking flag click : click FMR
			$arrFMRClick['action_date'] = date("Y-m-d h:i:s"); // Assign current date
			$arrFMRClick['fmr_nid'] = (int) $intMediaId;
	
			//Function for insert tracking details for click
			$this->objMicroListDB->fnInsertFmrUpdateTracking($arrFMRClick);
		}
	}
	
	
	/**
	 * Function to list out FMRs which were using 3BL Micro List between specific period of duration.
	 * @param string $strPageAction : Page Action
	 * @return mixed
	 */
	function fnProcessGetMicroListReport($strPageAction)
	{
		global $base_url, $conf;
		
		// Initialize parameters
		$strErrorMessage = "";
		$strGetEndDate = date('Y-m-d');
		$intPage = 0;
		$strGetStartDate = date('Y-m-d', strtotime(date("Y-m-d", strtotime($strGetEndDate)) . " -30 days"));

		// Get List Type
		$strListType = "3bl";
		$strMicroListName = "3BL Micro List Report";
		if ($strPageAction == "Client-Micro-List-Report") {
			$strListType = "client";
			$strMicroListName = "Client Micro List Report";
		}

		// Check Get values
		if (isset($_GET['startdate'])) {
			$strGetStartDate = trim($_GET['startdate']);
			$strDownloadExcel = trim($_GET['dwn']);
			$strGetEndDate = trim($_GET['enddate']);
			$strGetOrder = trim($_GET['status']);
			$strSortBy = trim(($_GET['sort'] != "") ? $_GET['sort'] : "date");
			$intPage = $_GET['page'] ? $_GET['page'] : 1;

			$strErrorMessage = "";
			//start and end date validation
			if ($strGetStartDate == "" || $strGetEndDate == "") {
				$strErrorMessage .= "Please give exact dates to generate the report";
			} else {

				// Validate date format
				$strStartDateError = $this->objMicroListController->fnValidateDateFormat($strGetStartDate);
				$strEndDateError = $this->objMicroListController->fnValidateDateFormat($strGetEndDate);

				//start and end date validation
				$intValidDate = 1;
				if ($strStartDateError == "error" || $strEndDateError == "error") {
					$strErrorMessage .= "Please enter valid date<br />";
					$intValidDate = 0;
				}

				// check start date condition
				if (strtotime($strGetStartDate) > strtotime($strGetEndDate) && $intValidDate == 1) {
					$strErrorMessage .= "Please enter valid start date <br />";
				}

				// Check Sort order
				if ($strGetOrder != "DESC" && $strGetOrder != "ASC") {
					$strGetOrder = "DESC";
				}

				// Check sort by field name
				$strSortCondition = "PD.field_fmr_date_time_value $strGetOrder";
				if ($strSortBy == "name") {
					$strSortCondition = "og.label $strGetOrder";
				}

				$intLimit = 100; //Set limit of MICRO LIST per page
				$intStart = ($intPage - 1) * $intLimit; //Calculate starting index of page

				$arrResult = array();
				$arrMicroList = array();
				//Check if not error
				if ($strErrorMessage == "") {
					//to get the fmr details which associate to micro list.
					list($arrResult, $intTotalRecords, $arrMicroList) = $this->objMicroListDB->fnGetFMRDetails($strGetStartDate, $strGetEndDate, $intStart, $intLimit, $strSortCondition, $strDownloadExcel, $strListType);
				}
			}

			drupal_set_message($strErrorMessage, "error");

			//check order by value and set opposite value to url
			if ($strGetOrder == "DESC") {
				$strNextStatus = "ASC";
			} else {
				$strNextStatus = "DESC";
			}

			# Add Pagination attributes.
			//Getting the list of Particular Company
			$intNext = 0;
			$intPrev = 0;
			//check next option;
			$intNumPages = ceil($intTotalRecords / $intLimit);

			if ($intNumPages == $intPage) {
				$intNext = 1;
			}

			//check for last option
			if ($intPage == 1) {
				$intPrev = 1;
			}
		}
		
		$arrData = array('arrList' => $arrResult, 'errorMessage' => $strErrorMessage, 'current_order' => $strGetOrder, "intStartDate" => $strGetStartDate, "intEndDate" => $strGetEndDate, "next_status" => $strNextStatus, 'intNext' => $intNext, 'intPrev' => $intPrev, "intPage" => $intPage, 'intLastPage' => $intNumPages, "base_url" => $base_url, "sortby" => $strSortBy, "imagepath" => $conf['IMAGES_PATH_3BL'], "download" => $strDownloadExcel, "arrMicroList" => $arrMicroList, "strPageAction" => $strPageAction, "strMicroListName" => $strMicroListName);
		
		return $arrData;
		
	}
	
	
	
		
		
}