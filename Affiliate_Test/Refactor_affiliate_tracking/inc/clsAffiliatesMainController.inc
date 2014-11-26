<?php
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsAffiliatesController');
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsTracking');

class clsAffiliatesMainController
{

	private $objAnalytics;
	private $objAffiliates;
	private $objTracking;
	private $arrShowMediaType = array();
	

	public function __construct()
	{
		$this->objAnalytics = new clsAnalytic();
		$this->objAffiliates = new clsAffiliatesController();
		$this->objTracking = new clsTracking();
		$this->arrShowMediaType = $this->objAnalytics->fnGetAllMediaTypes();

		
	}
	
	public function setCompanyTrackingId($intCompany)
	{
		$this->objTracking = new clsTracking($intCompany); 
	}
	
	/**
	 * Function manage current selected tab css.
	 *
	 * @param string $strAction : Current selected tab.
	 *
	 * @return string
	 */
	public function fnAnalyticsMaintab($strAction)
	{
		$strTabHtml =  $this->objAnalytics->fnAnalyticsMaintab($strAction);
		return $strTabHtml;
	}
	
	/**
	 * Function for prconnect cron
	 * @param int $intId : FMR id
	 * @return bool|string
	 */
	public function fnPrconnectCron($intId = 0)
	{
		$intResult = $this->objTracking->fnPrconnectCron($intId);
		return $intResult;
		
	}
	
	/**
	 * Function which will fetch all affiliate those are from 3bl
	 * and get all FMR information from their sites and insert it into affiliate_tracking table.
	 */
	public function fnTrackingAffiliates()
	{
		$this->objTracking->fnTrackingAffiliates();
	}
	
	public function fnGetFMRNewsLetterClickCount($objFMRClickInfo) 
	{
		$arrFMRClickData = $this->objTracking->fnGetFMRNewsLetterClickCount($objFMRClickInfo);
		return $arrFMRClickData;
	}
	
	
	public function fnGetFMRNewsLetterViewCount($objFMRViewInfo) 
	{
		$arrFMRViewData = $this->objTracking->fnGetFMRNewsLetterViewCount($objFMRViewInfo);
		return $arrFMRViewData;
	}
	
	public function fnGetAffiliateCompanyNameIfExists($objCompany,$arrGetCompanyName) 
	{
		$arrGetCompanyName = $this->objTracking->fnGetAffiliateCompanyNameIfExists($objCompany,$arrGetCompanyName);
		return $arrGetCompanyName;
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
		$strRecipientsInfoHTML = $this->objTracking->fnAffiliateReportsByCompany($intCompanyNid, $strCompanyName, $arrRecipient, $arrFMRInfo);
		return $strRecipientsInfoHTML;
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
		$this->objTracking->fnSendRecipientInfoToMCs($strRecipientsHTML);
		return true;
	}
	
	public function fnSendRemainingFMRMail($objClsScript,$objClsTracking,$arrMedia)
	{
		$this->objTracking->fnSendRemainingFMRMail($objClsScript,$objClsTracking,$arrMedia);
		return true;
	}
	
}