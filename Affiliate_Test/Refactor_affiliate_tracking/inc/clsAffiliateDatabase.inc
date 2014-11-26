<?php
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsTrackingData');
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsTrackingModifier');

class clsAffiliateDatabase {
	
	private $objTrackingData;
	private $objTrackingModifier;
	
	public function __construct()
	{
		$this->objTrackingData = new clsTrackingData();
		$this->objTrackingModifier = new clsTrackingModifier();
	}
	
	/**
	 * Getting Tracking Url from affiliate_tracking table redirect to the affiliate URL
	 * @param int $intNid : Node Id
	 * @param int $intAffId : Affiliate Id
	 * @return mixed
	 */
	public function getAffiliateTrackingUrl($intNid = 0, $intAffId = 0)
	{
		$strURL = $this->objTrackingData->getAffiliateTrackingUrl($intNid, $intAffId);
		return $strURL;
	}
	
	/**
	 * Getting distinct nid ,title using condition (Get Latest FMR Result)
	 * @param int $intFMRId : FMR Id
	 * @param int $intCompanyId :company Id
	 * @return mixed
	 */
	public function fnGetLatestFMR($intFMRId = 0, $intCompanyId = 0) //old name
	{
		$arrLatestFmr = $this->objTrackingData->fnGetLatestFMR($intFMRId, $intCompanyId);
		return $arrLatestFmr;
	}
	
	/**
	 * Function fetch all affiliates related to asking FMR
	 * @param  int $intFMRId : Fmr Id
	 * @param bool $boolExcel : Excel
	 * @param int $intFmrLanguageType : FMR Language (eg. non-english, english)
	 * @return mixed
	 */
	public function fnTrackingAffiliateList($intFMRId, $boolExcel = false, $intFmrLanguageType = 0)
	{
		$arrAffiliates = $this->objTrackingData->fnTrackingAffiliateList($intFMRId, $boolExcel, $intFmrLanguageType);
		return $arrAffiliates;
	}
	
	/**
	 * Function fetch the records for the affiliate tracking.
	 * @param string $strType : Type
	 * @return mixed
	 */
	public function fnFMRTracking($strType = "3BL")
	{
		$arrPRCFMRList = $this->objTrackingData->fnFMRTracking($strType);
		return $arrPRCFMRList;
	}
	
	/**
	 * To Get Justmeans Node Id
	 *
	 * @param int $intFMRId :  FMR Id
	 *
	 * @return string
	 */
	public function fnGetJMNodeId($intFMRId)
	{
		$intJMNodeId = $this->objTrackingData->fnGetJMNodeId($intFMRId);
		return $intJMNodeId;
	}
	
	/**
	 * Function to get FMR Click count
	 *
	 * @param int $intJMNodeId : Justmeans Node Id
	 *
	 * @return array
	 */
	public function fnGetFMRNewsLetterClicks($intJMNodeId)
	{
		$objFMRClickInfo = $this->objTrackingData->fnGetFMRNewsLetterClicks($intJMNodeId);
		return $objFMRClickInfo;
	}
	 /**
     * Function to get FMR View count
     *
     * @param int $intJMNodeId : Justmeans Node Id
     *
     * @return array
     */
    public function fnGetFMRNewsLetterViews($intJMNodeId)
    {
    	$objFMRViewInfo = $this->objTrackingData->fnGetFMRNewsLetterViews($intJMNodeId);
    	return $objFMRViewInfo;
	}
	
	/**
	 * To Get Company Name
	 *
	 * @param $intJMNodeId : Justmeans Node Id
	 *
	 * @return mixed
	 */
	public function fnGetCompanyInfo($intJMNodeId)
	{
		$arrCompanyInfo = $this->objTrackingData->fnGetCompanyInfo($intJMNodeId);
		return $arrCompanyInfo;
	}
	
	/**
	 * To Get FMR Information
	 *
	 * @param int $intJMNodeId :  Justmeans Node Id
	 *
	 * @return array
	 */
	public function fnGetFMRInformation($intJMNodeId)
	{
		$arrFMRInfo = $this->objTrackingData->fnGetFMRInformation($intJMNodeId);
		return $arrFMRInfo;
	}
	
	/**
	 * Function fetch all affiliates related to asking FMR
	 * @param  int $intFMRId : Fmr Id
	 * @param bool $boolExcel : Excel
	 * @param int $intFmrLanguageType : FMR Language (eg. non-english, english)
	 * @return mixed
	 */
	public function fnTrackingAffiliateList($intFMRId, $boolExcel = false, $intFmrLanguageType = 0)
	{
		$arrAffiliateList = $this->objTrackingData->fnTrackingAffiliateList($intFMRId, $boolExcel, $intFmrLanguageType);
		return $arrAffiliateList;
	}
	
	public function fnEditNodeScript()
	{
		$arrMedia = $this->objTrackingData->fnEditNodeScript();
		return $arrMedia;
	}
	
	public function fnGetEFMRTrackingList()
	{
		$arrFMRList = $this->objTrackingData->fnGetEFMRTrackingList();
		return $arrFMRList;
	}
	
	public function fnGetAllRemainingFMRMediaCron()
	{
		$arrMedia = $this->objTrackingData->fnGetAllRemainingFMRMediaCron();
		return $arrMedia;
	}
	
	/**
	 * Function return name of requested company id
	 *
	 * @param string $strCompanyNid : Comma Separated Company Node Id
	 *
	 * @return string
	 */
	public function fnGetAffiliateCompanyName($strCompanyNid = 0)
	{
		$objCompany = $this->objTrackingData->fnGetAffiliateCompanyName($strCompanyNid);
		return $objCompany;
	}
	
	/**
	 * Get Latest FMR
	 *
	 * @param int $intCompanyId : Company Id
	 *
	 * @return array
	 */
	public function fnGetAffiliatesLatestFMR($intCompanyId = 0)
	{
		$arrFMRInfo = $this->objTrackingData->fnGetAffiliatesLatestFMR($intCompanyId);
		return $arrFMRInfo;
	}
	
	/**
	 * Function for insert affiliate
	 * @param $objEntity
	 * @return mixed
	 */
	public function fnInsertAffiliate(&$objEntity)
	{
		$aff_id = $this->objTrackingModifier->fnInsertAffiliate(&$objEntity);
		return $aff_id;
	}
	
	public function fnInsertPRConnectTrackingForFMR($arrContent, $arrAffiliate, $intId) 
	{
		$this->objTrackingModifier->fnInsertPRConnectTrackingForFMR($arrContent, $arrAffiliate, $intId);
		return true;
	}
	
	public function fnUpdateTrackingFlag($intId) 
	{
		$this->objTrackingModifier->fnUpdateTrackingFlag($intId);
		return true;
	}
	
	
	/**
	 * Checking Tracking news url exits or not
	 * @param string $strUrl : Tracking url
	 * @return mixed
	 */
	public function fnDuplicateTrackingUrl($strUrl)
	{
		$intCount = $this->objTrackingModifier->fnDuplicateTrackingUrl($strUrl);
		return $intCount;
	}
	
	/**
	 * Getting the node id by title.
	 * @param string $strTitle : Title
	 * @return mixed
	 */
	public function fnGetNodeIdByTitle($strTitle)
	{
		$intNodeId = $this->objTrackingData->fnGetNodeIdByTitle($strTitle);
		return $intNodeId;
	}
	
	/**
	 * Insert value in affiliate_tracking table
	 * @param array $arrData : Array of tracking data
	 * @return DatabaseStatementInterface|int|null
	 */
	public function fnInsertAffTracking($arrData = array())
	{
		$this->objTrackingModifier->fnInsertAffTracking($arrData);
		return true;
	}
	
	
	

}
