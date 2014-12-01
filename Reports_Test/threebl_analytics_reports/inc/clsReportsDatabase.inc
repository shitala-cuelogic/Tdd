<?php
module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReportsSQL');
module_load_include('inc', 'threebl_analytics_reports', 'inc/JmClicksViews');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticDatabase');

class clsReportsDatabase
{
	private $objReportsSQL;
	private $objJmClicksViews;
	private $objAnalyticDB;
	
	public function __construct()
	{
		$this->objReportsSQL = new ClsReportsSQL();
		$this->objJmClicksViews = new JmClicksViews();
		$this->objAnalyticDB = new clsAnalyticDatabase();
	}
	
	/**
	 * Function fnGetFMRAnalyticClicks to get the Analytic clicks.
	 *
	 * @param int    $intFMRIds    : FMR id.
	 * @param string $strReportType: Report type (Weekly or Individual).
	 *
	 * @return array
	 */
	public function fnGetFMRAnalyticClicks($intFMRIds, $strReportType = "")
	{
		$objFMRClickInfo = $this->objReportsSQL->fnGetFMRAnalyticClicks($intFmrId, $strReportType);
		return $objFMRClickInfo;
		
	}
	
	/**
	 * Function fnGetFMRAnalyticView to get analytic views
	 *
	 * @param int    $intFMRIds    : FMR id
	 * @param string $strReportType: Report type (Weekly or Individual).
	 *
	 * @return array
	 */
	public function fnGetFMRAnalyticView($intFMRIds, $strReportType = "")
	{
		$objFMRViewInfo = $this->objReportsSQL->fnGetFMRAnalyticView($intFMRIds, $strReportType);
		return $objFMRViewInfo;
	}
	
	/**
	 * Function for Getting click count For 3bl NewsLetter
	 *
	 * @param int    $intFMRId     : Fmr Id
	 * @param string $strReportType: Report type (Weekly or Individual).
	 *
	 * @return int
	 */
	public function fnGetFMRNewsletterClick($intFMRId, $strReportType = "")
	{
		$intClickCount = $this->objReportsSQL->fnGetFMRNewsletterClick($intFmrId, $strReportType);
		return $intClickCount;
	}
	
	/**
	 * Function to get the Justmeans newsletter total clicks for selected FMR
	 *
	 * @param int    $intNodeId     : Node Id
	 * @param string $strChannelType: Channel Type (Jm-Newsletter, Justmeans)
	 * @param string $strReportType : Report type (Weekly or Individual).
	 *
	 * @return mixed
	 */
	public function fnGetJMClicks($intNodeId, $strChannelType, $strReportType = "")
	{
		$intJMClicks = $this->objJmClicksViews->fnGetJMClicks($intNodeId, $strChannelType, $strReportType);
		return $intJMClicks;
	}
	
	/**
	 * Function to get the Justmeans newsletter total views for selected FMR
	 *
	 * @param int    $intNodeId     : Node Id
	 * @param string $strChannelType: Channel Type (Jm-Newsletter, Justmeans)
	 * @param string $strReportType : Report type (Weekly or Individual).
	 *
	 * @return mixed
	 */
	public function fnGetJMViews($intNodeId, $strChannelType, $strReportType = "")
	{
		$intJMViews = $this->objJmClicksViews->fnGetJMViews($intNodeId, $strChannelType, $strReportType);
		return $intJMViews;
	}
	
	/**
	 * Function fnGetMicroListClickCount
	 *
	 * @param int $intFMRId : FMR id
	 *
	 * @return array
	 */
	public function fnGetMicroListClickCount($intFMRId)
	{
		$arrViewCount = $this->objReportsSQL->fnGetMicroListClickCount($intFMRId);
		return $arrViewCount;
	}
	
	/**
	 * * Function for Secondary Category of FMRs
	 *
	 * @param $strPrimaryCategory
	 * @param $strSecondCategory
	 *
	 * @return mixed
	 */
	public function fnGetSecondaryCategory($strPrimaryCategory, $strSecondCategory)
	{
		$arrFMRCategory = $this->objReportsSQL->fnGetSecondaryCategory($strPrimaryCategory, $strSecondCategory);
		return $arrFMRCategory;
	}
	
	/**
	 * Function to get FMRs information distributed in the previous 30 or 7 days
	 *
	 * @param int    $intCalDays   : Number of days
	 * @param string $strReportType: Report type (Weekly or Individual).
	 *
	 * @return array
	 */
	function fnGetLastPublishedFMRInfo($intCalDays = 30, $strReportType = "")
	{
		list($arrFMRDetails,$strCurrentDate, $intPrevMonthDate) = $this->objReportsSQL->fnGetLastPublishedFMRInfo($intCalDays, $strReportType);
		return array($arrFMRDetails,$strCurrentDate, $intPrevMonthDate);
	}
	
	/**
	 * Getting active company records.
	 *
	 * @return array
	 */
	public function fnGetActiveCompanies()
	{
		$arrCompany = $this->objReportsSQL->fnGetActiveCompanies();
		return $arrCompany;
	}
	
	 /**
     * Getting  Email address
     *
     * @param string $strCompanyNid : Company Node Id
     * @param int    $intMonth      : Flag to get result from particular table
     *
     * @return array
     */
    public function fnGetAutoReportEmail($strCompanyNid = "", $intMonth = 0)
    {
    	$arrAutoReportEmail = $this->objReportsSQL->fnGetAutoReportEmail($strCompanyNid, $intMonth);
    	return $arrAutoReportEmail;
	}
	
	/**
	 * Get Latest FMR
	 *
	 * @param int $intCompanyId : Company Id
	 *
	 * @return array
	 */
	public function fnGetCronLatestFMR($intCompanyId = 0)
	{
		$objLatestFMR = $this->objReportsSQL->fnGetCronLatestFMR($intCompanyId = 0);
		return $objLatestFMR;
	}
	
	/**
	 * Function insertCompanyDetails
	 *
	 * @param int    $intMonth                  : Flag to check report type
	 * @param int    $intCompanyNid             : Company Node Id
	 * @param string $strRecipient              : Recipient email
	 * @param string $strDownloadReportFileType : Download Report File Type
	 *
	 * @return mixed
	 */
	public function fnInsertCompanyDetails($intMonth, $intCompanyNid, $strRecipient, $strDownloadReportFileType = 'email')
	{
		$intInsertId = $this->objReportsSQL->fnInsertCompanyDetails($intMonth, $intCompanyNid, $strRecipient, $strDownloadReportFileType);
		return $intInsertId;
	}
}