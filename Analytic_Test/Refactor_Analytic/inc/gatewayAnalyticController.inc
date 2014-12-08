<?php

module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticDatabase');

class GatewayAnalyticController
{
	private $companyOgId;
	private $companyNid;
	private $analyticProcessing;
	private $analyticDB;
	
	
	public function __construct($intCompanyOgId = 0) {
		
		$this->companyOgId = $intCompanyOgId;
		$this->analyticProcessing = new clsAnalytic();
		$this->analyticDB = new clsAnalyticDatabase();
	}	
	
	public function setCompanyNid($intCompanyNid)
	{
		$this->companyNid = $intCompanyNid;
	}
	
	public function fnViewAnalyticInformation(){
		
		$arrViewChartSum = array();
		$arrClickChartSum = array();
		$arrDates = array();
		
		//Include all required files
	    module_load_include('inc', 'refactor_Analytic', 'inc/commonFunction');
		
		//Object of class
		$this->analyticProcessing = new clsAnalytic();
		
		//Get Top five clicks count and their FMR information
		$arrTopClicksFMRInfo = $this->analyticDB->fnGetTopFiveClicksFMRs($this->companyOgId); //clsAnalyticsClicks
		
		// Get Benchmarks clicks information
		$arrBenchmarkClicksFMRInfo = $this->analyticDB->fnGetBenchmarkClicksByMediaType(); //clsBenchmark
		
		// Get Benchmarks views information
		$arrBenchmarkViewsFMRInfo = $this->analyticDB->fnGetBenchmarkViewsByMediaType(); //clsBenchmark
		
		//checking FMR exist or not
		if ($strFMRIds != '') {
			//Fetch all company FMRs Views STEP 2
			// Query  which give result Count(FMRCount) ViewCount of each media.
			$arrFMRViews = $this->analyticDB->fnGetViews($strFMRIds); //clsAnalyticsViews
		
			# GET the Click Related data
			$arrFMRClicks = $this->analyticDB->fnGetClicks($strFMRIds); //clsAnalyticsClicks
		
			//GET all FMR with Videos
			$arrFMRWithVideos = $this->analyticDB->fnGetFMRWithVideo($this->companyOgId); //clsFMR
			
			//Fetch all company FMRs STEP 1
			list($arrFMRInfo, $arrFMRIds, $strFMRIds) = $this->analyticDB->fnGetCompanyFMRs($this->companyOgId, "", "", "non_archive"); //clsFMR
		
			if (count($arrFMRIds) > 0) {
				#Get Information for all FMR types
				foreach ($arrFMRInfo as $strFMRType => $arrFMRs) {
		
				#Collect All Views for the all FMRs with and without video
				$arrFMRInformation = array();
				$arrFMRViewsInformationWith = array();
		
				foreach ($arrFMRs as $key => $intFMRId) {
					$arrFMRInformation['views'][] = $arrFMRViews[$intFMRId];
					$arrFMRInformation['clicks'][] = $arrFMRClicks[$intFMRId];
		
					#Check that FMR from with video or not.
					if (in_array($intFMRId, $arrFMRWithVideos) == true) {
						$arrFMRViewsInformationWith['views'][] = $arrFMRViews[$intFMRId];
						$arrFMRViewsInformationWith['clicks'][] = $arrFMRClicks[$intFMRId];
					}
				}
		
				#Total Views
				$arrMediaTypeCount[$strFMRType]['viewcount'] = (int)array_sum($arrFMRInformation['views']);
				$arrMediaTypeCount[$strFMRType]['clickcount'] = (int)array_sum($arrFMRInformation['clicks']);
		
						#Get total FMRs
						$arrMediaTypeCount[$strFMRType]['fmrcount'] = (int)count($arrFMRs);
		
				//Assign with video count to total count of media.
				$arrMediaTypeCount[$strFMRType]["withvideo"]['fmrcount'] = count($arrFMRViewsInformationWith['views']);
				$arrMediaTypeCount[$strFMRType]["withvideo"]['viewcount'] = (int)array_sum($arrFMRViewsInformationWith['views']);
				$arrMediaTypeCount[$strFMRType]["withvideo"]['clickcount'] = (int)array_sum($arrFMRViewsInformationWith['clicks']);
		
				//Assign with out video count to 0
				#Views
				$arrMediaTypeCount[$strFMRType]["withoutvideo"]['viewcount'] = (int)($arrMediaTypeCount[$strFMRType]['viewcount'] - $arrMediaTypeCount[$strFMRType]["withvideo"]['viewcount']);
				#Clicks
				$arrMediaTypeCount[$strFMRType]["withoutvideo"]['clickcount'] = (int)($arrMediaTypeCount[$strFMRType]['clickcount'] - $arrMediaTypeCount[$strFMRType]["withvideo"]['clickcount']);
				#FMR Count
				$arrMediaTypeCount[$strFMRType]["withoutvideo"]['fmrcount'] = (int)($arrMediaTypeCount[$strFMRType]['fmrcount'] - $arrMediaTypeCount[$strFMRType]["withvideo"]['fmrcount']);
			}
			}
		
			// Query for View Chart between dates, For last 2 months.
			$arrViewChart = $this->analyticDB->fnGetViewChartInfo($this->companyOgId, $strFMRIds); //condition on
		
			// Condition for Click chart between dates, default before 2 months with Company
				$arrClickChart = $this->analyticDB->fnGetClickChartInfo($this->companyOgId, $strFMRIds); //clsAnalyticsClicks
		
				//Process views and clicks and prepare 7 days interval data for graph
				list($arrViewChartSum, $arrClickChartSum, $arrDates) = $this->analyticProcessing->fnSumWeekViewsClicks($arrViewChart, $arrClickChart);
		}
		//Main tab of (Analytics, Tracking ,Searching Engines, Reports);
		$strTabHtml = $this->analyticProcessing->fnAnalyticsMaintab('View');
		
				//Include JS and CSS files
				drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/analytics.js');
				drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/jsapi.js');
		
				drupal_add_css(drupal_get_path('theme', 'threebl') . '/css/justmeans/jmbackend-global.css');
				drupal_add_css(drupal_get_path('theme', 'threebl') . '/css/dashboard.css');
		
				$arrAnalyticInformation = array(
				'arrViewChart' => $arrViewChartSum,
				'arrMediaTypeCount' => $arrMediaTypeCount,
				'arrBenchmarkViewsFMRInfo' => $arrBenchmarkViewsFMRInfo,
				'arrBenchmarkClicksFMRInfo' => $arrBenchmarkClicksFMRInfo,
				'arrClickChart' => $arrClickChartSum,
				"arrTopClicksFMRInfo" => $arrTopClicksFMRInfo,
				'strTabHtml' => $strTabHtml,
				"arrDts" => $arrDates,
				);
				//echo "<pre>";print_r($arrData);echo "</pre>";exit;
				
				return $arrAnalyticInformation;
	}
	
	public function fnViewAnalyticMediaType()
	{
		
		//Include all required files
		
		module_load_include('inc', 'refactor_Analytic', 'inc/commonFunction');
		// Query for getting all FMRs that get published
		list($arrFMRInfo, $arrFMRIds, $strFMRIds) = $this->analyticDB->fnGetFMRViewClickByCompany($this->companyOgId, $strMediaType);
		
		//Initialize variable
		$arrReferLink = array();
		$arrCountryName = array();
		$arrViewChartSum = array();
		$arrClickChartSum = array();
		$arrDates = array();
		$arrMediaTypeCount = array();
		
		//checking the FMR's array exist or not.
		if ($strFMRIds != '') {
		
			// Query  which give result Count(FMRCount) ViewCount of each media.
			$arrFMRViews = $this->analyticDB->fnGetViews($strFMRIds); //clsAnalyticsViews
		
			# GET the Click Related data
			$arrFMRClicks = $this->analyticDB->fnGetClicks($strFMRIds); //clsAnalyticsClicks
		
			$arrFMRInformation = array();
		
			if (count($arrFMRIds) > 0) {
				#Get Information for all FMR types
				foreach ($arrFMRInfo as $intFMRId => $arrFMRs) {
		
				$arrFMRInformation['views'][] = $arrFMRViews[$intFMRId];
				$arrFMRInformation['clicks'][] = $arrFMRClicks[$intFMRId];
			}
		
			#Total Views and Clicks
			$arrMediaTypeCount['viewcount'] = (int)array_sum($arrFMRInformation['views']);
			$arrMediaTypeCount['clickcount'] = (int)array_sum($arrFMRInformation['clicks']);
			}
		
		
			//Get Views for the specific media type for chart
			$arrViewChart = $this->analyticDB->fnGetViewChartInfo($this->companyOgId, $strFMRIds); //clsAnalyticsViews
		
			// Condition for Click chart between dates, default before 2 months of particular media type
			$arrClickChart = $this->analyticDB->fnGetClickChartInfo($this->companyOgId, $strFMRIds); // clsAnalyticsClicks
		
			//Process views and clicks and prepare 7 days interval data for graph
			list($arrViewChartSum, $arrClickChartSum, $arrDates) = $this->analyticProcessing->fnSumWeekViewsClicks($arrViewChart, $arrClickChart);
		
			// Get country name and click count for google map.
			$arrCountryName = $this->analyticDB->fnGetFMRClicksByCountryGoogleMap($this->companyOgId, $strMediaType); //clsFMR
			//Get referrer website by media type
			$arrReferLink = $this->analyticDB->fnGetFMRReferrerClicksByMediaType($this->companyOgId, $strMediaType); //clsFMR
		}
		
			//Main tab of (Analytics, Tracking ,Searching Engines, Reports);
			$strTabHtml = $this->analyticProcessing->fnAnalyticsMaintab('View');
		
			//Include JS and CSS files
			drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/analytics.js');
			drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/jsapi.js');
			drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/jquery.js');
			drupal_add_css(drupal_get_path('theme', 'threebl') . '/css/justmeans/jmbackend-global.css');
			
			$arrData = array('arrClickChart' => $arrClickChartSum, 'arrViewChart' => $arrViewChartSum, 'strTabHtml' => $strTabHtml, 'strMediaType' => $strMediaType, 'arrRefereLink' => $arrReferLink, 'arrCountryName' => $arrCountryName, 'arrMediaCount' => $arrMediaTypeCount, "arrDts" => $arrDates, 'arrShowSecondLevelType' => $this->analyticProcessing->getMediaTypeArray());
			
			return $arrData;
	}
	
	public function fnViewAnalyticMediaTypeId($intMediaId)
	{
		//Include all required files
		
		
		module_load_include('inc', 'refactor_Analytic', 'inc/commonFunction');
		
		
		
		
		
		//Getting FMR Title by media id.
		$objNode = node_load($intMediaId);
		$strMediaTitle = $objNode->title; //Get FMR title
		
		//getting FMR Publish date
		$arrFmrPublish = $objNode->field_fmr_date_time;
		$strPublishDate = $arrFmrPublish['und'][0]['value'];
		
		//getting FMR Media type
		$arrFmrType = $objNode->field_fmr_type_of_content;
		$strFmrType = $arrFmrType['und'][0]['value'];
		
		//getting Benchmark views and clicks
		if (strtotime($strPublishDate) <= strtotime($this->analyticDB->strBenchmarkNAMonth)) {
			// Set N/A value for Benchmark views and clicks, if FMR published before Apr 2014
			$intBenchmarkView = "N/A";
			$intBenchmarkClick = "N/A";
		} else {
			// Set value for Benchmark views and clicks
			$intBenchmarkView = ((int) $objNode->field_fmr_average_views['und'][0]['value'] > 0) ? number_format((int) $objNode->field_fmr_average_views['und'][0]['value']) : "-";
			$intBenchmarkClick = ((int) $objNode->field_fmr_average_clicks['und'][0]['value'] > 0) ? number_format((int) $objNode->field_fmr_average_clicks['und'][0]['value']) : "-";
		}
		
		// Primary Category
		$strPrimaryCategoryName = "";
		$intPrimaryCategoryId = (int) $objNode->field_fmr_primary_category['und'][0]['target_id'];
		$strPrimaryCategory = $this->analyticProcessing->fnPrimaryCategoryName($intPrimaryCategoryId);
		if ($strPrimaryCategory != "") {
			$strPrimaryCategoryName = $strPrimaryCategory;
		}
		
		// Is Archive
		$strIsArchive = "";
		$strCheckIsArchive = $objNode->field_dist_archive['und'][0]['value'];
		if ($strCheckIsArchive == 1) {
			$strIsArchive = " (A)";
		}
		
		// check Nid of node present of not.
		if (empty($objNode)) {
			drupal_goto('Dashboard/Analytics/Views');
		}
		
		// Query for View count of Particular FMR
		$arrViewCount = $this->analyticDB->fnGetFMRViewCountByMediaId($this->companyOgId, $intMediaId); //clsFMR
		
		// Query for Click count of Particular FMR
		$arrClickCount = $this->analyticDB->fnGetFMRClickCountByMediaId($this->companyOgId, $intMediaId); //clsFMR
		
		// Get country name and click count for google map by media id
		$arrCountryName = $this->analyticDB->fnGetFMRClicksByCountryGoogleMap($this->companyOgId, $intMediaId); //clsFMR
		
		// get referrer website by media id
		$arrReferLink = $this->analyticDB->fnGetFMRReferrerClicksByMediaType($this->companyOgId, $intMediaId); //clsFMR
		
		//Main tab of (Analytics, Tracking ,Searching Engines, Reports);
		$strTabHtml = $this->analyticProcessing->fnAnalyticsMaintab('View');
		
		//Include JS and CSS files
		drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/analytics.js');
		drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/jsapi.js');
		drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/jquery.js');
		drupal_add_css(drupal_get_path('theme', 'threebl') . '/css/justmeans/jmbackend-global.css');
		
		$arrData = array(
				'arrClickCount' => $arrClickCount,
				'arrRefereLink' => $arrReferLink,
				'arrCountryName' => $arrCountryName,
				'arrViewCount' => $arrViewCount,
				'strTabHtml' => $strTabHtml,
				'strMediaTitle' => $strMediaTitle,
				'strPublishDate' => $strPublishDate,
				'strFmrType' => $strFmrType,
				'arrShowMediaType' => $this->analyticProcessing->getMediaTypeArray(),
				'intBenchmarkView' => $intBenchmarkView,
				'intBenchmarkClick' => $intBenchmarkClick,
				'strIsArchive' => $strIsArchive,
				'strPrimaryCategoryName' => $strPrimaryCategoryName
		);
		
		return $arrData;
	}
	
	public function fnProcessAnalyticMediaTypeAjax()
	{
		
		module_load_include('inc', 'refactor_Analytic', 'inc/commonFunction');
		
		$strMediaType = '';
		
		if (isset($_POST['type'])) {
			$strMediaType = trim($_POST['type']);
		}
		
		$intPage = $_POST['page'] ? $_POST['page'] : 1; //Set default page number
		$intLimit = 25; //Set limit of FMR per page
		$intStart = ($intPage - 1) * $intLimit; //Calculate starting index of page
		$intNext = 0;
		$intPrev = 0;
		
		# To get all FMRs information that get published
		//list($arrMediaResult, $arrGetFMRIds, $strFMRIds, $intTotalRecords) = $this->analyticDB->fnGetCompanyPublishedFMRs($this->companyOgId, $strMediaType, $intStart, $intLimit);

		list($objFMRNidInfo, $intTotalRecords) = $this->analyticDB->fnGetCompanyPublishedFMRs($this->companyOgId, $strMediaType, $intStart, $intLimit);
		
		//Checking if array is not empty
		if (!empty($objFMRNidInfo)) {
		
			// To get FMR information
			list($arrMediaResult, $arrGetFMRIds) = $this->analyticProcessing->fnGetFMRInfo($objFMRNidInfo, $strMediaType);
		
			//Implode FMR ids
			$strFMRIds = implode(', ', $arrGetFMRIds);
		}
		
		
		# To fetch FMR views information
		$arrViewFMRInfo = $this->analyticDB->fnGetViews($strFMRIds); //clsAnalyticsViews
		
		# To fetch FMR Clicks information
		$arrClickFMRInfo = $this->analyticDB->fnGetClicks($strFMRIds); // clsAnalyticsClicks
		
		if (!empty($arrMediaResult)) {
			foreach ($arrMediaResult as $intFMRId => $arrMediaResultData) {
				$arrViewClick[$intFMRId]['nid'] = $arrMediaResultData["nid"];
				$arrViewClick[$intFMRId]['title'] = $arrMediaResultData["title"];
				$arrViewClick[$intFMRId]['publishDate'] = $arrMediaResultData["publishdate"];
				$arrViewClick[$intFMRId]['viewCount'] = $arrViewFMRInfo[$intFMRId];
				$arrViewClick[$intFMRId]['type'] = $arrMediaResultData["media"];
		
				$arrViewClick[$intFMRId]['media_type'] = trim($this->analyticProcessing->getMediaType($arrMediaResultData["media"]));
		
				$arrViewClick[$intFMRId]['campaign'] = (string)$arrMediaResultData["campaign"];
				$arrViewClick[$intFMRId]['clickCount'] = $arrClickFMRInfo[$intFMRId];
				$arrViewClick[$intFMRId]['benchmark_view'] = $arrMediaResultData["benchmark_view"];
				$arrViewClick[$intFMRId]['benchmark_click'] = $arrMediaResultData["benchmark_click"];
				$arrViewClick[$intFMRId]['primary_category'] = $arrMediaResultData["primary_category"];
				$arrViewClick[$intFMRId]['is_archive'] = $arrMediaResultData["is_archive"];
			}
		}
		
		//check next option;
		$intNumPages = ceil($intTotalRecords / $intLimit);
		
		if ($intNumPages == $intPage) {
			$intNext = 1;
		}
		//check for last option
		if ($intPage == 1) {
			$intPrev = 1;
		}
		
		// Primary Categories array to replace long title with short title
		$arrLongPrimaryCategoryName = $this->analyticProcessing->fnGetPrimaryCategoryShortName();
		$arrData = array('arrViewClickMediaResult' => $arrViewClick, 'intNext' => $intNext,
				'intPrev' => $intPrev, 'intPage' => $intPage, 'strType' => $strMediaType, 'intLastPage' => $intNumPages, "arrLongPrimaryCategoryName" => $arrLongPrimaryCategoryName);
		
		return $arrData;
		
	}
	
	public function fnProcessAnalyticExcel($strSubType = '')
	{
		global $base_url;
		
		//Include all required files
		
		module_load_include('inc', 'refactor_Analytic', 'inc/commonFunction');
		
		//Get Company Name
		$strCompanyName = fnGetCompanyName($this->companyNid);
		
		//Get Media type array
		$arrShowMediaType = $this->analyticProcessing->getMediaTypeArray(); //clsFMR
		
		# To get all FMRs that get published from last 6 months
		list($arrMediaResult, $arrGetFMRIds, $strFMRIds) = $this->analyticDB->fnGetCompanyFMRsExcel($this->companyOgId, "all"); //clsCompanyData
		
		# To fetch FMR views information
		$arrViewFMRInfo = $this->analyticDB->fnGetViews($strFMRIds); //clsAnalyticsViews
		
		# To fetch FMR Clicks information
		$arrClickFMRInfo = $this->analyticDB->fnGetClicks($strFMRIds); //clsAnalyticsClicks
		
		if (!empty($arrMediaResult)) {
			foreach ($arrMediaResult as $intFMRId => $arrMediaResultData) {
				$arrViewClick[$intFMRId]['nid'] = $arrMediaResultData["nid"];
				$arrViewClick[$intFMRId]['title'] = $arrMediaResultData["title"];
				$arrViewClick[$intFMRId]['publishDate'] = $arrMediaResultData["publishdate"];
				$arrViewClick[$intFMRId]['viewCount'] = $arrViewFMRInfo[$intFMRId];
				$arrViewClick[$intFMRId]['type'] = $arrMediaResultData["media"];
				$arrViewClick[$intFMRId]['campaign'] = (string)$arrMediaResultData["campaign"];
				$arrViewClick[$intFMRId]['primary_category'] = $arrMediaResultData["primary_category"];
				$arrViewClick[$intFMRId]['benchmark_view'] = $arrMediaResultData["benchmark_view"];
				$arrViewClick[$intFMRId]['benchmark_click'] = $arrMediaResultData["benchmark_click"];
				$arrViewClick[$intFMRId]['is_archive'] = $arrMediaResultData["is_archive"];
				$arrViewClick[$intFMRId]['clickCount'] = $arrClickFMRInfo[$intFMRId];
				$arrViewClick[$intFMRId]['video'] = ($arrMediaResultData["video"]) ? 'Yes' : 'No';
			}
		}
		
		if ($strSubType != 'email') {
			
			$strHTMLTable = $this->analyticProcessing->fnCreateAnalyticExcelNotForEmail($strCompanyName, $arrViewClick, $arrShowMediaType);
			
			echo $strHTMLTable;
			
			$intTypeOFReport = 8;
			 
			#get the company id
			$this->companyNid = $_SESSION['client_og_nid']; // node id
			 
			#Set the report download by value to excel : 1
			$strReportFileType = "excel";
			 
			# Email addresses are not available for case pdf
			$strEmail = "Download by Excel";
			 
			// Call function fnInsert3blReportDetails To insert the details of report
			fnInsert3blReportDetails($this->companyNid, $strEmail, $intTypeOFReport, $strReportFileType);
			exit;
			
		} else {
			
			$strHTMLTable = $this->analyticProcessing->fnCreateAnalyticExcelForEmail($strCompanyName, $arrViewClick, $arrShowMediaType);
			
			#Get the repoty type : such as 1  for MCSR,2 for CSMT
			$intTypeOFReport = 0;		
		
			#Set the report download by value to excel : 1
			$strReportFileType = "excel";
		
			# Email addresses are not available for case pdf
			$strEmail = "Download by Excel";
		
			// Call function fnInsert3blReportDetails To insert the details of report
			fnInsert3blReportDetails($this->companyNid, $strEmail, $intTypeOFReport, $strReportFileType);
		
			return $strHTMLTable;
		}
	}
	
	public function fnProcessAnalyticSearching()
	{
		//Include required files
		module_load_include('inc', 'threebl_affiliate', 'inc/clsTrackingData');
	
		//Creating the Object of Class
		$objClsTrackingData = new clsTrackingData();
		
		// get latest fmr
		$arrLatestFmr = $objClsTrackingData->fnGetLatestFMR(0, $this->companyNid);
		
		//Main tab of (Analytics, Tracking ,Searching Engines, Reports);
		$strTabHtml = $this->analyticProcessing->fnAnalyticsMaintab('Search');
		
		//Include required JS and CSS files
		drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/tracking.js');
		drupal_add_css(drupal_get_path('theme', 'threebl') . '/css/justmeans/jmbackend-global.css');
		
		$arrData = array('strTabHtml' => $strTabHtml,
				'arrLatestFmr' => $arrLatestFmr
		);
		
		return $arrData;
		
	}
	
	public function fnProcessAnalyticReports()
	{
		//Include required class files
		module_load_include('inc', 'refactor_Analytic', 'inc/commonFunction');
		module_load_include('inc', 'threebl_affiliate', 'inc/clsTracking');
		module_load_include('inc', 'refactor_reports', 'inc/sendEmail');

		//Define variables
		global $base_url, $conf;
		
		//Include all required java script files
		drupal_add_js(drupal_get_path('theme', 'threebl') . '/js/jquery-latest.js');
		drupal_add_js(drupal_get_path('theme', 'threebl') . '/js/validator/jquery.validationEngine.js');
		drupal_add_js(drupal_get_path('theme', 'threebl') . '/js/validator/jquery.validationEngine-en.js');
		drupal_add_js(drupal_get_path('module', 'refactor_Analytic') . '/js/report.js');
		
		//Include all necessary css files
		drupal_add_css(drupal_get_path('theme', 'threebl') . '/css/justmeans/jmbackend-global.css');
		drupal_add_css(drupal_get_path('theme', 'threebl') . '/css/validator/validationEngine.jquery.css');
		
		$strTime = time();
		
		$strSubmit = (isset($_POST['submit'])) ? $_POST['submit'] : ''; //change
		
		if ($strSubmit == "Send") {    //change
			$strType = trim($_POST['sendemail']);
			$strMediaType = trim($_POST['mediaTypOrId']);
			$strServerName = trim($_SERVER['HTTP_HOST']);
			$strEmail = trim($_POST['email']);
			$strSub = addslashes(trim($_POST['subject']));
			$strMsg = addslashes(nl2br(trim($_POST['msg'])));
		
			$strSender = $conf['no_reply_from'];
		
			$strRecipient = $strEmail;
		
			$strSubject = ($strSub != '') ? $strSub : "Report";
			$strMessage = ($strMsg != '') ? $strMsg : "Hi,<br><br>   Please find the attached file.<br><br>Thanks,<br>3BLMedia";
		
			$strSenderName = $conf['sender_name'];
			$strHeader = $base_url . "/Dashboard/Report/Views/pdfhtml/header";
			$strFooter = $base_url . "/Dashboard/Report/Views/pdfhtml/footer";
		
			#get the number as the type of the report such as 1:bdaf,2:MCSR
			$intTypeOFReport = 0;
		
			#set how report get send either i.e. email
			$strReportFileType = "email";
			//start :change
			switch ($strType) {
				case "BADF":
					$strExcelData = refactor_Analytic_excel("email");
					$xlfile = @fopen("/var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/excel/Report$strTime.xls", 'wb+');
					fwrite($xlfile, $strExcelData);
					fclose($xlfile);
					$strAttachment = "/var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/excel/Report$strTime.xls";
		
					$boolSent = fnSendEmail($strSenderName, $strSender, $strRecipient, $strSubject, $strMessage, $strAttachment);
					if ($boolSent) {
						drupal_set_message("Report sent successfully");
					}
					$intTypeOFReport = 1;
					break;
		
				case "MCSR":
					$strUrl = http_build_query(array('client_og_nid' => $this->companyNid, 'client_og' => $_SESSION['client_og']));
					$strSrc1 = $base_url . "/Dashboard/Report/Views/" . base64_encode($strUrl);
		
					exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrc1 . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf");
					$intTypeOFReport = 2;
					break;
		
				case "CSMT":
					if ($strMediaType == '') {
						drupal_goto($base_url);
						die;
					}
					$strUrl = http_build_query(array('client_og_nid' => $this->companyNid, 'strMediaType' => "$strMediaType", 'client_og' => $_SESSION['client_og']));
					$strSrc1 = $base_url . "/Dashboard/Report/Views/MediaType/" . base64_encode($strUrl);
		
					exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrc1 . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf");
					$intTypeOFReport = 3;
					break;
		
				case "CSC":
					if ((int)$strMediaType == 0) { //Check campaign Id [$steMediaType: is nothing but campaign id]
						drupal_goto($base_url);
						die;
					}
					$strUrl = http_build_query(array('client_og_nid' => $this->companyNid, 'intCampaignId' => "$strMediaType", 'client_og' => $_SESSION['client_og']));
					$strSrc1 = $base_url . "/Dashboard/Report/Views/MediaType/" . base64_encode($strUrl);
		
					exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrc1 . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf");
					$intTypeOFReport = 4;
					break;
		
				case "FMR":
					if ((int)$strMediaType == 0) {
						drupal_goto($base_url);
						die;
					}
					$strUrl = http_build_query(array('client_og_nid' => $this->companyNid, 'intMediaId' => "$strMediaType", 'client_og' => $_SESSION['client_og']));
					$strSrc1 = $base_url . "/Dashboard/Report/Views/MediaId/" . base64_encode($strUrl);
		
					exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrc1 . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf");
					$intTypeOFReport = 5;
					break;
		
			}
			//switch
			//end :change --changed if-else to switch case
			$strAttachment = "/var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf";
		
			if (file_exists($strAttachment)) {
		
				//Send email function
				$boolSent = fnSendEmail($strSenderName, $strSender, $strRecipient, $strSubject, $strMessage, $strAttachment);
				if ($boolSent) {
		
					// Call function fnInsert3blReportDetails To insert the details of report
					fnInsert3blReportDetails($this->companyNid, $strEmail, $intTypeOFReport, $strReportFileType);
					drupal_set_message("Report sent successfully");
				}
			}
		}
		
		$strTabHtml = $this->analyticProcessing->fnAnalyticsMaintab('Reports');
		
		//Get Company Campaign List
		$objGetCampaign = $this->analyticDB->fnGetAllCampaign($this->companyOgId); //clsCompanyData
		//Creating the Object of Class
		$objClsTrackingData = new clsTrackingData();
		
		//Get Latest FMRs.
		$arrLatestFMR = $objClsTrackingData->fnGetLatestFMR(0, $this->companyNid);
		
		//Get Company All FMRs [last 6 Months]
		$vars = array(
				'strTabHtml' => $strTabHtml,
				'base_url' => $base_url,
				'objCampaign' => $objGetCampaign,
				'objFMR' => $arrLatestFMR,
				'arrShowMediaType' => $this->analyticProcessing->getMediaTypeArray()
		);
		
		return $vars;
		
	}
	
	
}
