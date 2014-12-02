<?php
module_load_include('inc', 'refactor_Analytic', 'inc/clsCompanyData');
module_load_include('inc', 'refactor_Analytic', 'inc/clsBenchmark');
module_load_include('inc', 'refactor_Analytic', 'inc/clsFMR');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticsViews');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticsClicks');
module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');

class clsAnalyticDatabase
{
	
	private $objCompanyData;
	private $objBenchmark;
	private $objFMR;
	private $objAnalyticsViews;
	private $objAnalyticsClicks;
	private $objClickView;
	
	public function __construct()
	{
		$this->objCompanyData = new clsCompanyData();
		$this->objBenchmark = new clsBenchmark();
		$this->objFMR = new clsFMR();
		$this->objAnalyticsViews = new clsAnalyticsViews();
		$this->objAnalyticsClicks = new clsAnalyticsClicks();
		$this->objClickView = new clsClickView();
		
	}
	
	/**
	 * Get Top five clicks count and their FMR information
	 * @param int $intCompanyOgId : Company OG Id
	 *
	 * @return array
	 */
	public function fnGetTopFiveClicks($intCompanyOgId)
	{
		//Get Top five clicks count and their FMR information
		$objTopClicksFMRInfo = $this->objAnalyticsClicks->fnGetTopFiveClicksFMRs($intCompanyOgId);
		return $objTopClicksFMRInfo;
	}
	
	
	/**
	 * Function fetch FMR Clicks information
	 *
	 * @param string $strFMRIds : Comma separated FMR Ids
	 *
	 * @return array
	 */
	public function fnGetClicks($strFMRIds)
	{
		# GET the Click Related data
        $objFMRClickInfo = $this->objAnalyticsClicks->fnGetClicksInfo($strFMRIds);
		return $objFMRClickInfo;
	}
	
	/**
	 * Query for getting Click Chart for First Level and Second Level
	 *
	 * @param int    $intCompanyOgId               : Company OG Id
	 * @param string $strFMRIds                    : Comma separated FMR Ids
	 * @param int    $intCampaignId                : Campaign Id
	 *
	 * @return array
	 */
	public function fnGetClickChart($intCompanyOgId, $strFMRIds = "", $intCampaignId = 0)
	{
		// Condition for Click chart between dates, default before 2 months with Company
        $$ObjClickChartData = $this->objAnalyticsClicks->fnGetClickChartInfo($intCompanyOgId, $strFMRIds);
        
		return $$ObjClickChartData;
	}
	
	
	/**
	 * Function return FMRs those have video attached with it.
	 *
	 * @param int $intCompanyOgId : Company OG Id
	 *
	 * @return array
	 */
	public function fnFMRWithVideo($intCompanyOgId)
	{
		 //GET all FMR with Videos
        $arrFMRWithVideos = $this->objFMR->fnGetFMRWithVideoInfo($intCompanyOgId);
        return $arrFMRWithVideos;
	}
	
	/**
	 * Function return all FMRs that get published from last 6 months
	 *
	 * @param int    $intCompanyOgId : Company OG Id
	 * @param string $strMediaType   : FMR Media Type
	 * @param string $intCampaignId  : Campaign Id
	 * @param string $strIsArchiveFlag  : Flag to check condition for archive field
	 * @param string $strFileType  : Flag to check file type (excel)
	 *
	 * @return array
	 */
	public function fnGetCompanyFMRs($intCompanyOgId, $strMediaType = "", $intCampaignId = "", $strIsArchiveFlag = "", $strFileType = "")
	{
		//Fetch all company FMRs STEP 1
		$objCompanyFMRInformation = $this->objFMR->fnGetCompanyFMRsInfo($intCompanyOgId, $strMediaType, $intCampaignId, $strIsArchiveFlag, $strFileType);
		return $objCompanyFMRInformation;
	}
	
	
	/**
	 * Query for Google Map for Click Count for Second Level and Third Level
	 *
	 * @param int   $intCompanyOgId   :  Company OG id
	 * @param mixed $strMediaTypeORId : value will be either media type or id
	 *
	 * @return mixed
	 */
	public function fnFMRClicksByCountryGoogleMap($intCompanyOgId, $strMediaTypeORId)
	{
		// Get country name and click count for google map.
		$arrCountryName = $this->objFMR->fnGetFMRClicksByCountryGoogleMapInfo($intCompanyOgId, $strMediaTypeORId);
		return $arrCountryName;
	}
	
	/**
	 * Query for Referral website for Second Level and Third Level
	 *
	 * @param int   $intCompanyOgId    :  Company OG id
	 * @param mixed $strMediaTypeORId  : value will be either media type or id
	 *
	 * @return mixed
	 */
	public function fnFMRReferrerClicksByMediaType($intCompanyOgId, $strMediaTypeORId)
	{
		//Get referrer website by media type
		$arrReferLink = $this->objFMR->fnGetFMRReferrerClicksByMediaTypeInfo($intCompanyOgId, $strMediaTypeORId);
		return $arrReferLink;
	}
	
	
	/**
	 * Getting View-Count By Media-Id
	 *
	 * @param int  $intCompanyOgId : Company OG Id
	 * @param int  $intMediaId     : Media Id
	 * @param bool $boolReport     : Boolean Flag
	 *
	 * @return mixed
	 */
	public function fnFMRViewCountByMediaId($intCompanyOgId, $intMediaId, $boolReport = 0)
	{
		// Query for View count of Particular FMR
		$arrViewCount = $this->objFMR->fnGetFMRViewCountByMediaIdInfo($intCompanyOgId, $intMediaId);
		return $arrViewCount;
	}
	
	/**
	 * Getting Click-Count By Media-Id
	 *
	 * @param int  $intCompanyOgId : Company OG Id
	 * @param int  $intMediaId     : Media Id
	 * @param bool $boolReport     : Boolean Flag
	 *
	 * @return mixed
	 */
	public function fnFMRClickCountByMediaId($intCompanyOgId, $intMediaId, $boolReport = 0)
	{
		// Query for Click count of Particular FMR
		$arrClickCount = $this->objFMR->fnGetFMRClickCountByMediaIdInfo($intCompanyOgId, $intMediaId);
		return $arrClickCount;
	}
	
	/**
	 * Function to get Primary Category Name
	 *
	 * @param int $intPrimaryCategoryId : Primary category id
	 *
	 * @return array
	 */
	public function fnPrimaryCategoryName($intPrimaryCategoryId)
	{
		$strPrimaryCategoryName = $this->objFMR->fnGetPrimaryCategoryName($intPrimaryCategoryId);
		return $strPrimaryCategoryName;
	}
	
	
	/**
	 * Function fetch FMR views information
	 *
	 * @param string $strFMRIds : Comma separated FMR Ids
	 *
	 * @return array
	 */
	public function fnGetViews($strFMRIds)
	{
		//Fetch all company FMRs Views STEP 2
		$objFMRViewInfo = $this->objAnalyticsViews->fnGetViewsInfo($strFMRIds);
		return $objFMRViewInfo;
	}
	
	/**
	 * Query for getting View Chart for First Level and Second Level
	 *
	 * @param int    $intCompanyOgId          : Company OG Id
	 * @param string $strFMRIds               : Comma separated FMR Ids
	 * @param int    $intCampaignId           : Campaign Id
	 *
	 * @return array
	 */
	public function fnGetViewChart($intCompanyOgId, $strFMRIds = "", $intCampaignId = 0)
	{
		// Query for View Chart between dates, For last 2 months.
		$arrViewChart = $this->objAnalyticsViews->fnGetViewChartInfo($intCompanyOgId, $strFMRIds);
		return $arrViewChart;
	}
	
	/**
	 * Get Benchmarks views information
	 *
	 * @return array
	 */
	public function fnBenchmarkClicksByMediaType($intCurrentMonthDate, $intNextMonthDate)
	{
		// Get Benchmarks clicks information
		$ObjBenchmarkClicksFMRInfo = $this->objBenchmark->fnGetBenchmarkClicksByMediaType($intCurrentMonthDate, $intNextMonthDate);
		return $ObjBenchmarkClicksFMRInfo;
	}
	
	/**
	 * Get Benchmarks views information
	 *
	 * @return array
	 */
	public function fnBenchmarkViewsByMediaType($intCurrentMonthDate, $intNextMonthDate)
	{
		// Get Benchmarks views information
		$ObjBenchmarkViewsByMediaType = $this->objBenchmark->fnGetBenchmarkViewsByMediaType($intCurrentMonthDate, $intNextMonthDate);
		return $ObjBenchmarkViewsByMediaType;
	}
	
	/**
	 * Fetch FMR views information for Benchmark
	 *
	 * @param string $strFMRIds : Comma separated FMR Ids
	 *
	 * @return mixed
	 */
	public function fnGetBenchmarkViews($strFMRIds)
	{
		$arrFMRClicks = $this->objBenchmark->fnGetBenchmarkClick($strFMRIds);
		return $arrFMRClicks;
	}
	
	
	/**
	 * Function return FMRs those have video attached with it for  Benchmark.
	 *
	 * @return array
	 */
	public function fnGetBenchmarkFMRWithVideo()
	{
		$arrFMRWithVideos = $this->objBenchmark->fnGetBenchmarkFMRWithVideo();
		return $arrFMRWithVideos;
	}
	
	/**
	 * Function return all FMRs that get published
	 *
	 * @param int    $intCompanyOgId  : Company OG id
	 * @param string $strMediaType    : FMR media type
	 *
	 * @return array
	 */
	public function fnFMRViewClickByCompany($intCompanyOgId, $strMediaType = '')
	{
		//for getting all FMRs that get published
		$objFMRNidInfo = $this->objCompanyData->fnGetFMRViewClickByCompanyInfo($intCompanyOgId, $strMediaType);
		return $objFMRNidInfo;
	}
	
	/**
	 * Function fetch all campaign related to company
	 *
	 * @param int   $intCompOgId : Company OG id
	 *
	 * @return mixed
	 */
	public function fnGetAllCampaign($intCompOgId)
	{
		$objGetCampaign = $this->objCompanyData->fnGetAllCampaign($intCompanyOGId);
		return $objGetCampaign;
	}
	
	
	/**
	 * Function return all FMRs that get published for excel sheet download from report
	 *
	 * @param int    $intCompanyOgId : Company OG id
	 * @param string $strMediaType   : FMR media type
	 *
	 * @return array
	 */
	public function fnGetCompanyFMRsExcel($intCompanyOgId, $strMediaType = "")
	{
		# To get all FMRs that get published from last 6 months
		$arrResult = $this->objCompanyData->fnGetCompanyFMRsExcel($intCompanyOgId, $strMediaType);
		return $arrResult;
	}
	
	/**
	 * Function for Getting Campaign Name
	 *
	 * @param int $intCampaignId : Campaign Id
	 *
	 * @return string
	 */
	function fnGetCampaignName($intCampaignId = 0)
	{
		$strCampaignName = $this->objCompanyData->fnGetCampaignName($intCampaignId);
		return $strCampaignName;
	}
	
	/**
	 * Function return name of requested company id
	 *
	 * @param int $intCompanyNid : Company Node Id
	 *
	 * @return string
	 */
	function fnGetCompanyName($intCompanyNid = 0)
	{
		$strCompanyName = $this->objCompanyData->fnGetCompanyName($intCompanyNid);
		return $strCompanyName;
	}
	
	/**
	 * Function to insert 3BL report details
	 *
	 * @param int    $intCompanyId      : Company Id
	 * @param string $strEmailIds       : Email Ids
	 * @param string $strReportType     : Report type
	 * @param string $strReportFileType : Report File type
	 *
	 * @return mixed
	 */
	function fnInsert3blReportDetails($intCompanyId, $strEmailIds, $strReportType, $strReportFileType)
	{
		$intInsertId = $this->objCompanyData->fnInsert3blReportDetails($intCompanyId, $strEmailIds, $strReportType, $strReportFileType);
		return $intInsertId;
	}
	
	/**
	 * To get FMR Ids published in last month
	 *
	 * @return array
	 */
	public function fnGetLastMonthPublishedFMRIds()
	{
		$arrLastMonthPublishedFMRIds = array();
		$objLastMonthPublishedFMRs = $this->objFMR->fnGetLastMonthPublishedFMRIds();
		return $objLastMonthPublishedFMRs;
	}
	
	/**
	 * Function for getting 'threebl_tmp_fmr_headline_views' view
	 * @return array
	 */
	public function fnGetFMRHeadlineViews()
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');
		$objClickView = new clsClickView();
		$arrClickView = $objClickView->fnGetFMRHeadlineViews();
		return $arrClickView;
	}
		
	/**
	 * Function for deleting 'threebl_tmp_fmr_headline_views' view
	 * @return array
	 */
	public function fnDeleteFMRHeadlineViews($key)
	{
		$this->objClickView->fnDeleteFMRHeadlineViews($key);
		return true;
	}
	
	/**
	 * Function for getting value from 'accesslog' table
	 * @return array
	 */
	public function fnGetAccesslogValue($intAid, $strLimit)
	{
		$arrGetPathData = $this->objClickView->fnGetAccesslogValue($intAid, $strLimit);
		return $arrGetPathData;
	}
	
	
	/**
	 * Function for checking media id exist or not for type FMR
	 * @return array
	 */
	public function fnCheckMediaExists($intMediaId)
	{
		$intMediaNodePublished = $this->objClickView->fnCheckMediaExists($intMediaId);
		return $intMediaNodePublished;
	}
	
	
	/**
	 * Function for getting media details
	 * @return array
	 */
	public function fnGetMediaDetails($intMediaId)
	{
		$arrGetMediaData = $this->objClickView->fnGetMediaDetails($intMediaId);
		return $arrGetMediaData;
	}
	
	/**
	 * Function for getting FMR Type of particular media-id.
	 * @return string
	 */
	public function fnGetFMRTypeForMedia($intMediaId)
	{
		$strFMRType = $this->objClickView->fnGetFMRTypeForMedia($intMediaId);
		return $strFMRType;
	}
	
	/**
	 * Function for checking click record exists or not
	 * @return int
	 */
	public function fnCheckClickExists($intMediaId, $intCompanyOgId, $strFMRType, $strDate)
	{
		$intRecordExist = $this->objClickView->fnCheckClickExists($intMediaId, $intCompanyOgId, $strFMRType, $strDate);
		return $intRecordExist;
	}
	
	/**
    * Add Click Count Function
    *
    * @param array $arrAllClick : array contain click related data
    * @param int   $int3blCron  : flag indicate function called by cron functionality or not
    * @return bool
    */
    public function fnAddAllClicks($arrAllClick, $int3blCron = 0)
    {

		$strFlagValue = $this->objClickView->fnAddAllClicks($arrAllClick, $int3blCron);
		return $strFlagValue;
	}
	
	/**
    * Add View Count Function
    *
    * @param array $arrAllView : array contain information about the all views
    */
    public function fnAddAllViews($arrAllView)
    {
		$this->objClickView->fnAddAllViews($arrAllView);
		return true;
	}

	/**
     * Function return all FMRs that get published for analytic second level ajax
     *
     * @param int       $intCompanyOgId  : Company OG id
     * @param string    $strMediaType    : FMR media type
     * @param int       $intStart        : Starting record for pagination
     * @param int       $intLimit        : number of records per page
     *
     * @return array
    */
	function fnCompanyPublishedFMRs($intCompanyOgId, $strMediaType, $intStart, $intLimit)
	{
		list($objFMRNidInfo, $intViewClickCntResult) = $this->objCompanyData->fnGetCompanyPublishedFMRsInfo($intCompanyOgId, $strMediaType, $intStart, $intLimit);
		
		return array($objFMRNidInfo, $intViewClickCntResult);
	}
}
