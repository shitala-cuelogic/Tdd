<?php
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsAffiliatesMainController');
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsAffiliateDatabase');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticDatabase');
module_load_include('inc', 'refactor_Analytic', 'inc/commonFunction');
module_load_include('inc', 'threebl_analytics_reports', 'inc/sendEmail');
module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReportsDatabase');

class AffiliateCronController
{
	private $objAffiliatesMain;
	private $objAffiliateDB;
	private $objAnalyticDB;
	private $objReportsDB;

	public function __construct()
	{
		$this->objAffiliatesMain = new clsAffiliatesMainController();
		$this->objAffiliateDB = new clsAffiliateDatabase();
		$this->objAnalyticDB = new clsAnalyticDatabase();
		$this->objReportsDB = new clsReportsDatabase();
	}
	
	public function fnProcessEFMRTrackingCron()
	{
		$arrFMRList = array();
		
		$arrFMRList = $this->objAffiliateDB->fnGetEFMRTrackingList();
		
		if (count($arrFMRList) > 0) {
			//COLLECT prconnect information
			foreach ($arrFMRList as $efmr) {
				//This is function for prconnect cron
				watchdog('eFMR Tracking', 'Processing nid = '.$efmr->nid);
				$this->objAffiliatesMain->fnPrconnectCron($efmr->nid);
			}
		}
		
		return true;
	}
	
	public function fnProcessRemaningFMRAffiliateCron()
	{
		//Include all required files
		module_load_include('inc', 'threebl_affiliate', 'inc/clsTracking');
		
		//Include required class files
		module_load_include('inc', 'threebl_script', 'inc/clsScript');
		
		//Object of class
		$objClsScript = new clsScript();
		
		//Check session
		$objClsTracking = new clsTracking($_SESSION['client_og_nid']);
		
		$arrMedia = $this->objAffiliateDB->fnGetAllRemainingFMRMediaCron();
		
		$this->objAffiliatesMain->fnSendRemainingFMRMail($objClsScript,$objClsTracking,$arrMedia);
	
	}
	
	public function fnProcessTrackingAutoAffiliatesPdfReportsCron()
	{
		//Get all active companies
		$arrCompany = array();
		$arrCompany = $this->objReportsDB->fnGetActiveCompanies();
		
		$strRecipientsHTML = "";
		
		if(count($arrCompany) > 0 ){
			//GET Company Ids
			$arrCompanyIds = array();
			$arrCompanyIds = array_values($arrCompany);
			$strCompanyIds = implode(',',$arrCompanyIds);
		
			//Collect All companies Email Ids
			$arrAutoReportEmail = $this->objReportsDB->fnGetAutoReportEmail($strCompanyIds);
			$intCnt = 0;
		
			// Get Compnay Name
			$objCompany = $this->objAffiliateDB->fnGetAffiliateCompanyName($strCompanyIds);
			
			$arrCompanyName = array();
			
			if ($strCompanyIds != "") 
			{
				$arrCompanyName = $this->objAffiliatesMain->fnGetAffiliateCompanyNameIfExists($objCompany,$arrCompanyName);
			}
		
			foreach($arrCompany as $intCompanySynId => $intCompanyNid){
		
				$arrEmails = array();
		
				//Get email ids of Company for weekly report.
				$arrEmails = array_unique($arrAutoReportEmail[$intCompanyNid]);
		
				if(is_array($arrEmails) && count($arrEmails) > 0){
		
					//Get the latest fmr of respective company.
					$arrFMRInfo = $this->objAffiliateDB->fnGetAffiliatesLatestFMR($intCompanySynId);
					if (count($arrFMRInfo) >0) {
		
						$strCompanyName = stripslashes($arrCompanyName[$intCompanyNid]);
		
						$strRecipientsHTML .= $this->objAffiliatesMain->fnAffiliateReportsByCompany($intCompanyNid, $strCompanyName, $arrEmails, $arrFMRInfo);
						$intCnt ++;
					}
				}
			}
		}
		
		// Send Recipients information to Media Consultant
		$this->objAffiliatesMain->fnSendRecipientInfoToMCs($strRecipientsHTML);
		
		return $intCnt;
	}
	
	
}