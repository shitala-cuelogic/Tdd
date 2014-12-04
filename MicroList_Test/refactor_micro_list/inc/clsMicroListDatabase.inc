<?php
module_load_include('inc', 'refactor_micro_list', 'inc/clsMicroEmailListSql');

class clsMicroListDatabase {
	
	private $objMicroEmailListSql;
	
	public function __construct()
	{
		$this->objMicroEmailListSql = new clsMicroEmailListSql();
	}
	

	
	/**
	 * Function to return the number of users in particular list.
	 *
	 * @param int $intListId : List Id
	 *
	 * @return mixed
	 */
	public function fnGetCountOfUser($intListId)
	{
		$intNumOfExistingUsers = $this->objMicroEmailListSql->fnGetCountOfUser($intListId);
		return $intNumOfExistingUsers;
	}
	
	/**
	 * Insert the value in list.s
	 *
	 * @param Object $objEntity : Entity object
	 *
	 * @return mixed
	 */
	public function fnInsertList(&$objEntity)
	{
		$intId = $this->objMicroEmailListSql->fnInsertList(&$objEntity);
		return $intId;
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
		$boolVal = $this->objMicroEmailListSql->fnDeleteEmailList($intListId, $arrUserIds);
		return $boolVal;
	}
	
	/**
	 * Function to add synchronize micro list.
	 *
	 * @param int $intListId : List Id
	 * @param int $intUserId : User Id
	 *
	 * @return bool
	 */
	public function fnAddSyncMicroList($intListId, $intUserId)
	{
		$boolVal = $this->objMicroEmailListSql->fnAddSyncMicroList($intListId, $intUserId);
		return $boolVal;
	}
	
	/**
	 * Function to insert the list and entity value into the field value.
	 *
	 * @param int $intListId : List Id
	 * @param int $intUserId : User Id
	 *
	 * @return mixed
	 */
	public function fnAddMicroListFieldValue($intListId, $intUserId)
	{
		$intResult = $this->objMicroEmailListSql->fnAddMicroListFieldValue($intListId, $intUserId);
		return $intResult;
	}
	
	/**
	 * Function to add list values in the respected revision table.
	 *
	 * @param int $intListId : List Id
	 * @param int $intUserId : User Id
	 *
	 * @return mixed
	 */
	public function fnAddMicroListRevisionValue($intListId, $intUserId)
	{
		$intResult = $this->objMicroEmailListSql->fnAddMicroListRevisionValue($intListId, $intUserId);
		return $intResult;
	}
	
	/**
	 * Function for getting user id by user-email if it's duplicate entry
	 *
	 * @param string $strEmail : Email
	 * @param int    $intListId : List Id
	 *
	 * @return int
	 */
	public function fnGetUserIdByEmail($intListId, $strEmail = '')
	{
		list($intUserId, $boolList) = $this->objMicroEmailListSql->fnGetUserIdByEmail($intListId, $strEmail);
		return array($intUserId, $boolList);
	}
	
	public function fnGetNewsUserRecord($arrNewsLetterUser) 
	{
		$arrUser = $this->objMicroEmailListSql->fnGetNewsUserRecord($arrNewsLetterUser);
		return $arrUser;
	}
	
	/**
	 * Fetch all the respective lists assigned by admin or created by client.
	 *
	 * @param int    $intOgId      : Company OG Id
	 * @param int    $intFlag      : Flag to check client micro list and 3bl micro list
	 * @param int    $intStart     : Start query Limit
	 * @param int    $intLimit     : Number of list per page
	 * @param string $strFlag      : Flag to check client or admin
	 * @param string $strSearchVal : Search Value
	 *
	 * @return array
	 */
	public function fnGetListByCompanyId($intOgId, $intFlag = 0, $intStart = 0, $intLimit = 25, $strFlag = "client", $strSearchVal = "")
	{
		list($objArrResult, $intCount) = $this->objMicroEmailListSql->fnGetListByCompanyId($intOgId, $intFlag, $intStart, $intLimit, $strFlag, $strSearchVal);
		return array($objArrResult, $intCount);
	}
	
	/**
     * Checking Count Of List
     *
     * @param int $intListId : List Id
     *
     * @return mixed
     */
    public function fnCheckCountListId($intListId = 0)
    {
    	$boolVal = $this->objMicroEmailListSql->fnCheckCountListId($intListId);
    	return $boolVal;
    }
    
    /**
     * function for getting the email list from database
     *
     * @param int    $intListId    : List Id
     * @param int    $intCountFlag : Flag to get count of records
     * @param string $strSQL       : MySql Query
     * @param int    $intAdminFlag : Flag to check 3BL and client list
     *
     * @return mixed
     */
    public function fnEmailList($intListId = 0, $intCountFlag = 0, $strSQL = '', $intAdminFlag = 0)
    {
    	$intCountEmail = $this->objMicroEmailListSql->fnEmailList($intListId, $intCountFlag, $strSQL, $intAdminFlag);
    	return $intCountEmail;
    }
    
    /**
     * Delete List From list table.
     *
     * @param int $intListId : List Id
     *
     * @return DatabaseStatementInterface
     */
    public function fnDeleteListByClient($intListId)
    {
    	$arrDelete = $this->objMicroEmailListSql->fnDeleteListByClient($intListId);
    	return $arrDelete;
    }
    
    
    /**
     *  Client all email by list id
     *
     * @param int $intListId : List Id
     * @param int $intOgId   : Company OG Id
     *
     * @return mixed
     */
    public function fnClientEmailListById($intListId, $intOgId)
    {
    	$objArrList = $this->objMicroEmailListSql->fnClientEmailListById($intListId, $intOgId);
    	return $objArrList;
    }
    
    /**
     * Function to get the latest published fmrs
     *
     * @return mixed
     */
    public function fnGetAllPublishedFMR()
    {
    	$objFMRResult = $this->objMicroEmailListSql->fnGetAllPublishedFMR();
    	return $objFMRResult;
    }
    
    /**
     * Function to get the fmr details which associate to micro list.
     *
     * @param string $strGetStartDate  : Start Date
     * @param string $strGetEndDate    : End Date
     * @param string $intStart         : Start count
     * @param string $intLimit         : Number of records per page
     * @param string $strSortCondition : Sort by condition
     * @param string $strDownloadExcel : Flag to Download Report
     * @param string $strListType : List Type
     *
     * @return mixed
     */
    public function fnGetFMRDetails($strGetStartDate, $strGetEndDate, $intStart = "", $intLimit = "", $strSortCondition = "", $strDownloadExcel, $strListType)
    {
    	list($arrResult, $intTotalRecords, $arrMicroList) = $this->objMicroEmailListSql->fnGetFMRDetails($strGetStartDate, $strGetEndDate, $intStart, $intLimit, $strSortCondition, $strDownloadExcel, $strListType);
    	return array($arrResult, $intTotalRecords, $arrMicroList);
    }
    
    /** function to get list ids assigned for FMR and user information assigned to List
     *
     * @param string $strFMRIds: Comma separated FMR Ids.
     *
     * @return object
     */
    public function fnGetFMRAssignListUserDetails($strFMRIds)
    {
    	$objGetRelListFMRIds = $this->objMicroEmailListSql->fnGetFMRAssignListUserDetails($strFMRIds);
    	return $objGetRelListFMRIds;
    }
    
    /**
     * Function to get user information assigned to list
     *
     * @param int $strFMRListId : FMR Id.
     *
     * @return mixed
     */
    public function fnGetListUserInfo($strFMRListId)
    {
    	$arrUserArrResult = $this->objMicroEmailListSql->fnGetListUserInfo($strFMRListId);
    	return $arrUserArrResult;
    }
    
    /**
     * Function to track Micro Lists Processing
     *
     * @param int    $intFMRId    : FMR Id
     * @param int    $intListId   : List Id
     * @param string $strListType : List Type
     *
     * @return mixed
     */
    public function fnTrackMicroListProcessing($intFMRId, $intListId)
    {
    	$intInsertId = $this->objMicroEmailListSql->fnTrackMicroListProcessing($intFMRId, $intListId);
    	return $intInsertId;
    }
    
    /**
     * Getting all active company for admin form
     *
     * @return mixed
     */
    public function fnGetAllActiveClient()
    {
    	$arrResult = $this->objMicroEmailListSql->fnGetAllActiveClient();
    	return $arrResult;
    }
    
    /**
     * Function for checking duplicate value for FMR Tracking
     *
     * @param Array $arrFMRView : FMR View
     *
     * @return mixed
     */
    public function fnCheckFmrTrackValue($arrFMRView)
    {
    	$intRowCount = $this->objMicroEmailListSql->fnCheckFmrTrackValue($arrFMRView);
    	return $intRowCount;
    }
    
    /**
     * Insert value in fmr update tracking tables
     *
     * @param Array $arrFmrDetails : FMR Details
     */
    public function fnInsertFmrUpdateTracking($arrFmrDetails)
    {
    	$this->objMicroEmailListSql->fnInsertFmrUpdateTracking($arrFmrDetails);
    	return true;
    }
	
}
