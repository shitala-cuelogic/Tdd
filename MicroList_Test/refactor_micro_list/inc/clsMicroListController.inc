<?php
module_load_include('inc', 'refactor_micro_list', 'inc/clsMicroList');
module_load_include('inc', 'refactor_micro_list', 'inc/clsMicroListHtml');

class clsMicroListController
{

	private $objMicroList;
	private $objMicroListHtml;

	public function __construct()
	{
		$this->objMicroList = new clsMicroList();
		$this->objMicroListHtml = new clsMicroListHtml();
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
    	#Check Admin Session
    	$boolVal = $this->objMicroList->fnCheckSession($intAdmin);
    	return $boolVal;
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
    	$intMaxUserLimit = $this->objMicroList->fnGetMaxUploadUserLimit($strListType);
    	return $intMaxUserLimit;
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
    	$intStatusMessage = $this->objMicroList->fnAddAdminClientList($arrPostValue, $arrFileData, $intCompanyNid, $intOgId, $intEditId);
    	return $intStatusMessage;
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
    	$intMessage = $this->objMicroList->fnAddEmail($arrPostData, $intListId, $intNumOfExistingUsers, $intCheckFlag);
    	return $intMessage;
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
    	$strMessage = $this->objMicroList->fnValidateAddListFormData($arrPostData, $arrFileData);
    	return $strMessage;
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
    	$strMessage = $this->objMicroList->fnValidateSubscriber($arrPostData);
    	return $strMessage;
    }
    
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
    	$strEmailBody = $this->objMicroListHtml->fnBuildFMRDetailsHTML($arrCompanyFMRDetails, $strListType);
    	return $strEmailBody;
    }
    
    /**
     * Sending the fmr details via email to the users which are in the 3bl micro list
     */
    public function fnSendFMRToCustomList()
    {
    	$this->objMicroList->fnSendFMRToCustomList();
    	return true;
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
    	$strEmailBody = $this->objMicroListHtml->fnBuildFinalHtml($strClientMessage, $strMainHtmlBody, $strListName, $strListType, $strProcessingContact);
    	return $strEmailBody;
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
    	$strError = $this->objMicroList->fnValidateDateFormat($strDate);
    	return $strError;
    }
    
    	
	
	
	
}