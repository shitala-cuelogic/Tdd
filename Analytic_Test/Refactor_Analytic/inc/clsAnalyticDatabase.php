<?php
class clsAnalyticDatabase
{
	public function __construct()
	{
		
	}
	
	/**
	 * Get Top five clicks count and their FMR information
	 * @param int $intCompanyOgId : Company OG Id
	 *
	 * @return array
	 */
	public function fnGetTopFiveClicksFMRs($intCompanyOgId)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticsClicks');
		$objClick = new clsAnalyticsClicks();
		//Get Top five clicks count and their FMR information
		$arrTopClicksFMRInfo = $objClick->fnGetTopFiveClicksFMRs($intCompanyOgId);
		
		return $arrTopClicksFMRInfo;
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
		module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticsClicks');
		$objClick = new clsAnalyticsClicks();
		# GET the Click Related data
        $arrFMRClicks = $objClick->fnGetClicks($strFMRIds);
		
		return $arrFMRClicks;
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
	public function fnGetClickChartInfo($intCompanyOgId, $strFMRIds = "", $intCampaignId = 0)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticsClicks');
		$objClick = new clsAnalyticsClicks();
		// Condition for Click chart between dates, default before 2 months with Company
        $arrClickChart = $objClick->fnGetClickChartInfo($intCompanyOgId, $strFMRIds);
        
		return $arrClickChart;
	}
	
	
	/**
	 * Function return FMRs those have video attached with it.
	 *
	 * @param int $intCompanyOgId : Company OG Id
	 *
	 * @return array
	 */
	public function fnGetFMRWithVideo($intCompanyOgId)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsFMR');
		$objFMR = new clsFMR();
		 //GET all FMR with Videos
        $arrFMRWithVideos = $objFMR->fnGetFMRWithVideo($intCompanyOgId);
        
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
		module_load_include('inc', 'refactor_Analytic', 'inc/clsFMR');
		$objFMR = new clsFMR();
		
		//Fetch all company FMRs STEP 1
		$arrCompanyFMRInformation = $objFMR->fnGetCompanyFMRs($intCompanyOgId, "", "", "non_archive");
		return $arrCompanyFMRInformation;
	}
	
	
	/**
	 * Query for Google Map for Click Count for Second Level and Third Level
	 *
	 * @param int   $intCompanyOgId   :  Company OG id
	 * @param mixed $strMediaTypeORId : value will be either media type or id
	 *
	 * @return mixed
	 */
	public function fnGetFMRClicksByCountryGoogleMap($intCompanyOgId, $strMediaTypeORId)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsFMR');
		$objFMR = new clsFMR();
		// Get country name and click count for google map.
		$arrCountryName = $objFMR->fnGetFMRClicksByCountryGoogleMap($intCompanyOgId, $strMediaTypeORId);
		
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
	public function fnGetFMRReferrerClicksByMediaType($intCompanyOgId, $strMediaTypeORId)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsFMR');
		$objFMR = new clsFMR();
		//Get referrer website by media type
		$arrReferLink = $objFMR->fnGetFMRReferrerClicksByMediaType($intCompanyOgId, $strMediaTypeORId);
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
	public function fnGetFMRViewCountByMediaId($intCompanyOgId, $intMediaId, $boolReport = 0)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsFMR');
		$objFMR = new clsFMR();
		// Query for View count of Particular FMR
		$arrViewCount = $objFMR->fnGetFMRViewCountByMediaId($intCompanyOgId, $intMediaId);
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
	public function fnGetFMRClickCountByMediaId($intCompanyOgId, $intMediaId, $boolReport = 0)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsFMR');
		$objFMR = new clsFMR();
		// Query for Click count of Particular FMR
		$arrClickCount = $objFMR->fnGetFMRClickCountByMediaId($intCompanyOgId, $intMediaId);
		return $arrClickCount;
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
		module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticsViews');
		$objView = new clsAnalyticsViews();
		//Fetch all company FMRs Views STEP 2
		// Query  which give result Count(FMRCount) ViewCount of each media.
		$arrFMRViews = $objView->fnGetViews($strFMRIds);
		return $arrFMRViews;
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
	public function fnGetViewChartInfo($intCompanyOgId, $strFMRIds = "", $intCampaignId = 0)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticsViews');
		$objView = new clsAnalyticsViews();
		// Query for View Chart between dates, For last 2 months.
		$arrViewChart = $objView->fnGetViewChartInfo($intCompanyOgId, $strFMRIds);
		return $arrViewChart;
		
	}
	
	/**
	 * Get Benchmarks views information
	 *
	 * @return array
	 */
	public function fnGetBenchmarkClicksByMediaType()
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsBenchmark');
		$objBenchmark = new clsBenchmark();
		// Get Benchmarks clicks information
		$arrBenchmarkClicksFMRInfo = $objBenchmark->fnGetBenchmarkClicksByMediaType();
		return $arrBenchmarkClicksFMRInfo;
	}
	
	/**
	 * Get Benchmarks views information
	 *
	 * @return array
	 */
	public function fnGetBenchmarkViewsByMediaType()
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsBenchmark');
		$objBenchmark = new clsBenchmark();
		// Get Benchmarks views information
		$arrBenchmarkViewsFMRInfo = $objBenchmark->fnGetBenchmarkViewsByMediaType();
		return $arrBenchmarkViewsFMRInfo;
		
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
		module_load_include('inc', 'refactor_Analytic', 'inc/clsBenchmark');
		$objBenchmark = new clsBenchmark();
		$arrFMRClicks = $objBenchmark->fnGetBenchmarkClick($strFMRIds);
		return $arrFMRClicks;
	}
	
	
	/**
	 * Function return FMRs those have video attached with it for  Benchmark.
	 *
	 * @return array
	 */
	public function fnGetBenchmarkFMRWithVideo()
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsBenchmark');
		$objBenchmark = new clsBenchmark();
		$arrFMRWithVideos = $objAnalytics->fnGetBenchmarkFMRWithVideo();
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
	public function fnGetFMRViewClickByCompany($intCompanyOgId, $strMediaType = '')
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsCompanyData');
		$objCompanyData = new clsCompanyData();
		// Query for getting all FMRs that get published
		
		list($arrFMRInfo, $arrFMRIds, $strFMRIds) = $objCompanyData->fnGetFMRViewClickByCompany($intCompanyOgId, $strMediaType);
		
		return array($arrFMRInfo, $arrFMRIds, $strFMRIds);
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
		module_load_include('inc', 'refactor_Analytic', 'inc/clsCompanyData');
		$objCompanyData = new clsCompanyData();
		$objGetCampaign = $objCompanyData->fnGetAllCampaign($intCompanyOGId);
		
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
		module_load_include('inc', 'refactor_Analytic', 'inc/clsCompanyData');
		$objCompanyData = new clsCompanyData();
		# To get all FMRs that get published from last 6 months
		$arrResult = $objCompanyData->fnGetCompanyFMRsExcel($intCompanyOgId, "all");
		return $arrResult;
		
	}
	
	/**
	 * To get FMR Ids published in last month
	 *
	 * @return array
	 */
	public function fnGetLastMonthPublishedFMRIds()
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsFMR');
		$objFMR = new clsFMR();
		$arrLastMonthPublishedFMRIds = array();
		$objLastMonthPublishedFMRs = $objFMR->fnGetLastMonthPublishedFMRIds();
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
		module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');
		$objClickView = new clsClickView();
		$objClickView->fnDeleteFMRHeadlineViews($key);
		return true;
	}
	
	/**
	 * Function for getting value from 'accesslog' table
	 * @return array
	 */
	public function fnGetAccesslogValue($intAid, $strLimit)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');
		$objClickView = new clsClickView();
		$arrGetPathData = $objClickView->fnGetAccesslogValue($intAid, $strLimit);
		return $arrGetPathData;
	}
	
	
	/**
	 * Function for checking media id exist or not for type FMR
	 * @return array
	 */
	public function fnCheckMediaExists($intMediaId)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');
		$objClickView = new clsClickView();
		$intMediaNodePublished = $objClickView->fnCheckMediaExists($intMediaId);
		return $intMediaNodePublished;
	}
	
	
	/**
	 * Function for getting media details
	 * @return array
	 */
	public function fnGetMediaDetails($intMediaId)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');
		$objClickView = new clsClickView();
		$arrGetMediaData = $objClickView->fnGetMediaDetails($intMediaId);
		return $arrGetMediaData;
	}
	
	/**
	 * Function for getting FMR Type of particular media-id.
	 * @return string
	 */
	public function fnGetFMRTypeForMedia($intMediaId)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');
		$objClickView = new clsClickView();
		$strFMRType = $objClickView->fnGetFMRTypeForMedia($intMediaId);
		return $strFMRType;
	}
	
	/**
	 * Function for checking click record exists or not
	 * @return int
	 */
	public function fnCheckClickExists($intMediaId, $intCompanyOgId, $strFMRType, $strDate)
	{
		module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');
		$objClickView = new clsClickView();
		$intRecordExist = $objClickView->fnCheckClickExists($intMediaId, $intCompanyOgId, $strFMRType, $strDate);
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
		module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');
		$objClickView = new clsClickView();
		$strFlagValue = $objClickView->fnAddAllClicks($arrAllClick, $int3blCron);
		return $strFlagValue;
	}
	
	/**
    * Add View Count Function
    *
    * @param array $arrAllView : array contain information about the all views
    */
    public function fnAddAllViews($arrAllView)
    {
		module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');
		$objClickView = new clsClickView();
		$objClickView->fnAddAllViews($arrAllView);
		return true;
	}
	
	
	
}
