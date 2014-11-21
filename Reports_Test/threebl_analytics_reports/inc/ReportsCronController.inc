<?php
module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReportsController');
module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReportsDatabase');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticDatabase');

class ReportsCronController
{
	private $objReportsController;
	private $objReportsDB;
	private $objAnalyticDB;

	public function __construct()
	{
		$this->objReportsController = new clsReportsController();
		$this->objReportsDB = new clsReportsDatabase();
		$this->objAnalyticDB = new clsAnalyticDatabase();
	}
	
	public function fnProcessAutoFMRReports()
	{
		//Get all active companies
		$arrCompany = array();
		$arrCompany = $this->objReportsDB->fnGetActiveCompanies();
		
		if(count($arrCompany) > 0 ){
			//GET Company Ids
			$arrCompanyIds = array();
			$arrCompanyIds = array_values($arrCompany);
			$strCompanyIds = implode(',',$arrCompanyIds);
		
			//Collect All companies Email Ids
			$arrAutoReportEmail = $this->objReportsDB->fnGetAutoReportEmail($strCompanyIds);
			$intCnt = 0;
		
			foreach($arrCompany as $intCompanySynId => $intCompanyNid){
				$arrEmails = array();
		
				//Get email ids of Company for weekly report.
				$arrEmails = array_unique($arrAutoReportEmail[$intCompanyNid]);
		
				if(is_array($arrEmails) && count($arrEmails) > 0){
					//Get the latest fmr of respective company.
					$arrMediaIds = array();
		
					$objMediaIds = $this->objReportsDB->fnGetCronLatestFMR($intCompanyNid);
					//To get FMR Ids
					$arrMediaIds = $this->objReportsController->fnGetFMRIds($objMediaIds);
		
					if (count($arrMediaIds) > 0) {
						//Get ids of latest fmr of company.
						$strMediaIds = @implode(',', $arrMediaIds);
						list($arrReceivedReport, $intMonth) = $this->objReportsController->fnReportsByCompany($intCompanyNid, $intCompanySynId, $arrEmails, $strMediaIds);
						
						$intInsertSuccess = false;
						if (count($arrReceivedReport) > 0) {
							//If mail sent successfully then insert these details in the table
							$strRecipients = implode(",", $arrReceivedReport);
							$intInsertSuccess = $this->objReportsDB->fnInsertCompanyDetails((int)$intMonth,$intCompanyNid,$strRecipients,'email');
						}
		
						$intCnt ++;
					}
				}
			}
		}
		
		return $intCnt;
	}
	
	
}