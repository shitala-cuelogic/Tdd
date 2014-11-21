<?php
module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReportsHTML');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');
module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReports');
module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReportGenerate');

class clsReportsController
{

	private $objReportHtml;
	private $objAnalytics;
	private $objReports;
	private $objReportGenerate;
	private $arrShowMediaType = array();
	public $strBenchmarkNAMonth;
	
	public function __construct()
	{
		$this->objReportHtml = new ClsReportHTML();
		$this->objAnalytics = new clsAnalytic();
		$this->objReports = new ClsReports();
		$this->objReportGenerate = new ClsReportGenerate();
		
		$this->arrShowMediaType = $this->objAnalytics->fnGetAllMediaTypes();
		$this->strBenchmarkNAMonth = $this->objAnalytics->fnGetBenchmarkNAMonth();
	}
	
	public function fnSumWeekViewsClicks($arrViewChart, $arrClickChart = array())
	{
		$arrSumViewsClicks = array();
		$arrSumViewsClicks = $this->objAnalytics->fnSumWeekViewsClicks($arrViewChart, $arrClickChart);
		return $arrSumViewsClicks;
	}
	
	public function fnGetAnalyticClickData($objFMRClickInfo, $strReportType = "") {
		$arrAnalyticData = $this->objReports->fnGetAnalyticClickData($objFMRClickInfo, $strReportType);
		return $arrAnalyticData;
	}
	
	public function fnGetAnalyticViewData($objFMRViewInfo, $strReportType="") {
		$arrAnalyticData = $this->objReports->fnGetAnalyticViewData($objFMRViewInfo);
		return $arrAnalyticData;
	}
	
	public function fnGetMicrolistDetails($arrViewCount) { 
		$arrMicroListDetails = $this->objReports->fnGetMicrolistDetails($arrViewCount);
		return $arrMicroListDetails;
	}
	
	/**
     * @param array $arrFinalResult      : All information regarding channel like 3bl media, widget
     * @param array $arrAnalyticData     : Analytic data
     * @param array $arrMicroListDetails : Data of micro list details
     *
     * @return mixed
     */
    public function fnBuildHtmlFMRReport($arrFinalResult, $arrAnalyticData, $arrMicroListDetails)
    {
    	$strHtml = $this->objReportHtml->fnBuildHtmlFMRReport($arrFinalResult, $arrAnalyticData, $arrMicroListDetails);
    	return $strHtml;
    }
    
    public function fnGetLatestFMRInfo($arrFMRDetails) {
    	$arrFMRResult = $this->objReports->fnGetLatestFMRInfo($arrFMRDetails);
    	return $arrFMRResult;
    }
    
    /**
     * Build the FMR report in HTML format and download it in the excel file.
     *
     * @param array  $arrFMRInfo : array of published FMR information
     * @param string $strFMRIds  : Comma separated FMR ids
     *
     */
    function fnBuildHtmlFMRMonthlyReport($arrFMRInfo, $strFMRIds)
    {
    	$strHtml = $this->objReportHtml->fnBuildHtmlFMRMonthlyReport($arrFMRInfo, $strFMRIds);
    	return $strHtml;
    }
    
    /**
     * To get FMR Ids
     * @param object $arrFMRRecords: Object FMR Information
     * @return array
     */
    public function fnGetFMRIds($arrFMRRecords)
    {
    	$arrFMRIds = $this->objAnalytics->fnGetFMRIds($arrFMRRecords);
    	return $arrFMRIds;
    }
	
    /**
     * Function Create PDF and send email to respective company
     *
     * @param int    $intCompanyNid  : Company Node Id
     * @param int    $intCompanyOgId : Company Og Id
     * @param array  $arrRecipient   : Recipients email array
     * @param string $strMediaIds    : Comma separated Media Id
     * @param int    $intMonth
     *
     * @return bool|DatabaseStatementInterface|int|null
     */
    public function fnReportsByCompany($intCompanyNid, $intCompanyOgId, $arrRecipient, $strMediaIds, $intMonth = 0)
    {
    	list($arrReceivedReport, $intMonth) = $this->objReports->fnReportsByCompany($intCompanyNid, $intCompanyOgId, $arrRecipient, $strMediaIds, $intMonth);
    	return array($arrReceivedReport, $intMonth);
    }
	
	
}