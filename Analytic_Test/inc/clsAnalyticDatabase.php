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
		return list($arrMediaResult, $arrGetFMRIds, $strFMRIds) = $objCompanyData->fnGetCompanyFMRsExcel($intCompanyOgId, "all");
		
	}
	
	
	
	
}
