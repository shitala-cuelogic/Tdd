<?php
module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReportsController');
module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReportsDatabase');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticDatabase');

class GatewayReportsController
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

	public function fnMediaTypeViews($strMediaType, $intCampaignId, $intCompanyOgId, $strFileType, $intCompanyNid)
	{
		//Get Campaign Name from campaign id
		$strCampaignName = $this->objAnalyticDB->fnGetCampaignName($intCampaignId);
		//Get Company Name
		$strCompanyName = $this->objAnalyticDB->fnGetCompanyName($intCompanyNid);

		// Query for getting the Views Count for particular media type.
		//Fetch all company FMRs STEP 1
		list($arrFMRInfo, $arrFMRIds, $strFMRIds) = $this->objAnalyticDB->fnGetCompanyFMRs($intCompanyOgId, $strMediaType, $intCampaignId, "archive", $strFileType);

		# Initialize
		$arrFMRInformation = array();
		$arrFMRViews = array();
		$arrFMRClicks = array();
		$arrViewChartSum = array();
		$arrClickChartSum = array();
		$arrDates = array();

		//checking the FMR's array exist or not.
		if ($strFMRIds != '') {

			//Fetch all company FMRs Views STEP 2
			// Query  which give result Count(FMRCount) of each media and ViewCount of each media.
			$arrFMRViews = $this->objAnalyticDB->fnGetViews($strFMRIds);

			//Views Date by date-visited
			$arrViewChart = $this->objAnalyticDB->fnGetViewChartInfo($intCompanyOgId, $strFMRIds, $intCampaignId);

			// Query for getting the Click Count
			$arrFMRClicks = $this->objAnalyticDB->fnGetClicks($strFMRIds);

			//Get non Video FMRs clicks
			$arrClickChart = $this->objAnalyticDB->fnGetClickChartInfo($intCompanyOgId, $strFMRIds, $intCampaignId);

			//Process views and clicks and prepare 7 days interval data for graph
			list($arrViewChartSum, $arrClickChartSum, $arrDates) = $this->objReportsController->fnSumWeekViewsClicks($arrViewChart, $arrClickChart);
		}

		//Combine all views and clicks data
		if (count($arrFMRIds) > 0) {
			#Get Information for all FMR types
			foreach ($arrFMRInfo as $intFMRId => $arrFMRs) {
			#Collect all FMR related information
			$arrFMRInfo[$intFMRId]['views'] = (int)$arrFMRViews[$intFMRId];
			$arrFMRInfo[$intFMRId]['clicks'] = (int)$arrFMRClicks[$intFMRId];

			$arrFMRInformation['views'][] = (int)$arrFMRViews[$intFMRId];
			$arrFMRInformation['clicks'][] = (int)$arrFMRClicks[$intFMRId];
			}
		}
		
		#Total Views
		$intTotalViews = (int)array_sum($arrFMRInformation['views']);
		$intTotalClicks = (int)array_sum($arrFMRInformation['clicks']);
		
		if ($strFileType == "excel") {
			$arrData = array(
					'strMediaType' => $this->objReportsController->arrShowMediaType[$strMediaType],
					'arrFMRInfo' => $arrFMRInfo,
					"intCampaignId" => $intCampaignId,
					'strCompanyName' => $strCompanyName,
					"strCampaignName" => $strCampaignName,
					"intTotalViews" => $intTotalViews,
					"intTotalClicks" => $intTotalClicks,
					'arrShowMediaType' => $this->objReportsController->arrShowMediaType
		
			); //change
			
			//echo theme('view-media-type_report_excel', $arrData);
		} else {
			#View Graph HTML
			$arrViewData = array('arrViewChart' => $arrViewChartSum, "arrDts" => $arrDates);
			$strViewGraph = theme('view_graph', $arrViewData);
		
			#CLICK Graph HTML
			$arrClickData = array('arrClickChart' => $arrClickChartSum, "arrDts" => $arrDates);
			$strClickGraph = theme('click_graph', $arrClickData);
		
			$arrData = array(
					'strMediaType' => $this->objReportsController->arrShowMediaType[$strMediaType],
					"strViewGrp" => $strViewGraph,
					"strClickGrp" => $strClickGraph,
					'arrFMRInfo' => $arrFMRInfo,
					"intCampaignId" => $intCampaignId,
					'strCompanyName' => $strCompanyName,
					"strCampaignName" => $strCampaignName,
					"intTotalViews" => $intTotalViews,
					"intTotalClicks" => $intTotalClicks,
					'arrShowMediaType' => $this->objReportsController->arrShowMediaType,
					"imagepath"        => $conf['IMAGES_PATH_3BL'],
			);
			//echo theme('view-media-type_report', $arrData);
			
		}
		
		return $arrData;
		
	}
	
	public function fnViewAnalytics($intCompanyOgId, $intCompanyNid)
	{
		//Get Company name
		$strCompanyName = $this->objAnalyticDB->fnGetCompanyName($intCompanyNid); //clsCompanyData

		//Fetch all company FMRs STEP 1
		list($arrFMRInfo, $arrFMRIds, $strFMRIds) = $this->objAnalyticDB->fnGetCompanyFMRs($intCompanyOgId, "", "", "non_archive");

		//Get Top five clicks count and their FMR information
		$arrTopClicksFMRInfo = $this->objAnalyticDB->fnGetTopFiveClicksFMRs($intCompanyOgId);

		// Get Benchmarks views information
		$arrBenchmarkViewsFMRInfo = $this->objAnalyticDB->fnGetBenchmarkViewsByMediaType();

		// Get Benchmarks clicks information
		$arrBenchmarkClicksFMRInfo = $this->objAnalyticDB->fnGetBenchmarkClicksByMediaType();

		$arrMediaTypeCount = array();

		//Checking if condition
		if (count($arrFMRIds) >0) {
			//Fetch all company FMRs Views STEP 2
			// Query  which give result Count(FMRCount) of each media and ViewCount of each media.
			$arrFMRViews = $this->objAnalyticDB->fnGetViews($strFMRIds);
			# GET the Click Related data
			$arrFMRClicks = $this->objAnalyticDB->fnGetClicks($strFMRIds);

			//GET all FMR with Videos
			$arrFMRWithVideos = $this->objAnalyticDB->fnGetFMRWithVideo($intCompanyOgId);

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
		}

		// create Google image graph html for reports
		list($strViewGraph, $arrViewGraph) = threebl_analytics_reports_Grp($intCompanyOgId, 'views', $strFMRIds);
		list($strClickGraph, $arrClickGraph) = threebl_analytics_reports_Grp($intCompanyOgId, 'clicks', $strFMRIds);

		
		//Collect Graph HTML

		$arrData = array(
				'arrMediaTypeCount' => $arrMediaTypeCount,
				"strViewGrp" => $strViewGraph,
				"strClickGrp" => $strClickGraph,
				"strCompanyName" => $strCompanyName,
				'arrBenchmarkViewsFMRInfo'  => $arrBenchmarkViewsFMRInfo,
				'arrBenchmarkClicksFMRInfo' => $arrBenchmarkClicksFMRInfo,
				"arrTopClicksFMRInfo" => $arrTopClicksFMRInfo,
				"imagepath"           => $conf['IMAGES_PATH_3BL'],
				"arrViewGraph"  => $arrViewGraph,
				"arrClickGraph" => $arrClickGraph,
		);
		
		return $arrData;
	}
	
	public function fnAnalyticsReportsGrp($intCompanyOgId, $strType, $strFMRIds, $strFileType, $intCampaignId, $strMediaType )
	{
		$arrChartSum = array();
		$arrData = array();
		
		if ($strFMRIds !="") {
		
			$arrChart = array();
		
			if ($strType == 'views') { # View Graph Data Collection
		
				// Query for View Chart between dates, For last 2 months.
				$strChartDateBetweenFlag = "1";
				$arrChart = $this->objAnalyticDB->fnGetViewChartInfo($intCompanyOgId, $strFMRIds, $strChartDateBetweenFlag, $intCampaignId);
		
			} elseif ($strType == 'clicks') { #Click Graph data collection
		
				// Click chart between dates, default before 2 months with Company
				$strClickChartDateBetweenFlag = "1";
				$arrChart = $this->objAnalyticDB->fnGetClickChartInfo($intCompanyOgId, $strFMRIds, $strClickChartDateBetweenFlag, $intCampaignId);
			}
		
			//Process views and clicks and prepare 7 days interval data for graph
			list($arrChartSum, $arrDates) = $this->objReportsController->fnSumWeekViewsClicks($arrChart);
		}
		
		if ($strType == 'clicks') {
			$arrData = array('arrClickChart' => $arrChartSum, "arrDts" => $arrDates);
			return array($arrData, $arrChartSum, );
		} else {
			$arrData =  array('arrViewChart' => $arrChartSum, "arrDts" => $arrDates);
			return array($arrData, $arrChartSum);
		}
		
	}
	
	public function fnProcessReportsMediaTypeId($intMediaId, $strMediaIds, $strFileType, $intCompanyNid, $intCompanyOgId)
	{
		
		$arrTrackingImg = array();
		$boolReport = 0;
		
		//Get Company Name
		$strCompanyName = $this->objAnalyticDB->fnGetCompanyName($intCompanyNid);
		
		//Getting FMR Title by media id.
		if ($intMediaId > 0) {
			$objNode = node_load($intMediaId);
			$strMediaTitle = $objNode->title;
			//getting FMR Publish date
			$arrFmrPublish = $objNode->field_fmr_date_time;
			$strPublishDate = $arrFmrPublish['und'][0]['value'];
		
			//getting Benchmark views and clicks
			if (strtotime($strPublishDate) <= strtotime($this->objReportsController->strBenchmarkNAMonth)) {
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
			$strPrimaryCategory = $this->objAnalyticDB->fnPrimaryCategoryName($intPrimaryCategoryId); // clsFMR
			if ($strPrimaryCategory != "") {
				$strPrimaryCategoryName = $strPrimaryCategory;
			}
		
			// Is Archive
			$strIsArchive = "";
			$strCheckIsArchive = $objNode->field_dist_archive['und'][0]['value'];
			if ($strCheckIsArchive == 1) {
				$strIsArchive = " (A)";
			}
		
			//getting FMR Media type
			$arrFmrType = $objNode->field_fmr_type_of_content;
			$strFmrType = $arrFmrType['und'][0]['value'];
		
			drupal_add_css(drupal_get_path('theme', 'threebl') . '/css/justmeans/jmbackend-global.css');
			drupal_add_js(drupal_get_path('module', 'threebl_analytics') . '/js/jsapi.js');
		
			// Get country name and click count for google map by media id
			$arrCountryName = $this->objAnalyticDB->fnGetFMRClicksByCountryGoogleMap($intCompanyOgId, $intMediaId); //clsFMR
		
			// get referrer website by media id
			$arrReferLink = $this->objAnalyticDB->fnGetFMRReferrerClicksByMediaType($intCompanyOgId, $intMediaId); //clsFMR
		
			$strMediaId = $intMediaId;
		
		} elseif ($strMediaIds != '') {
			$strMediaId = $strMediaIds;
			$arrFMRIds = explode(',', $strMediaIds);
			$boolReport = 1;
		
			foreach ($arrFMRIds as $intMediaID) {
				//Collect all node information
				$arrNode[$intMediaID] = node_load($intMediaID);
		
				// Get country name and click count for google map by media id
				$arrCountryName[$intMediaID] = $this->objAnalyticDB->fnGetFMRClicksByCountryGoogleMap($intCompanyOgId, $intMediaID); //clsFMR
		
				// get referrer website by media id
				$arrReferLink[$intMediaID] = $this->objAnalyticDB->fnGetFMRReferrerClicksByMediaType($intCompanyOgId, $intMediaID); //clsFMR
			}
		}
		
		// Query for View count of Particular FMR
		$arrViewCount = $this->objAnalyticDB->fnGetFMRViewCountByMediaId($intCompanyOgId, $strMediaId, $boolReport); //clsFMR
		
		// Query for Click count of Particular FMR
		$arrClickCount = $this->objAnalyticDB->fnGetFMRClickCountByMediaId($intCompanyOgId, $strMediaId, $boolReport); //clsFMR
		
		$arrData = array(
				'arrClickCount' => $arrClickCount,
				'arrRefereLink' => $arrReferLink,
				'arrCountryListName' => $arrCountryName,
				'arrViewCount' => $arrViewCount,
				'strMediaTitle' => $strMediaTitle,
				'strPublishDate' => $strPublishDate,
				'strFileType' => $strFileType,
				'arrTrackingImg' => $arrTrackingImg,
				"strCompanyName" => $strCompanyName,
				'arrShowMediaType' => $this->objReportsController->arrShowMediaType,
				'intBenchmarkView' => $intBenchmarkView,
				'intBenchmarkClick' => $intBenchmarkClick,
				"strPrimaryCategoryName" => $strPrimaryCategoryName,
				"strIsArchive" => $strIsArchive,
				"strFmrType" => $strFmrType,
				"imagepath" => $conf['IMAGES_PATH_3BL'],
		);
		
		return $arrData;
	}
	
	public function fnProcessReportsPDF($intCompanyOgId, $intCompanyNid, $strType, $strMediaType, $strServerName, $strTime)
	{
		global $base_url;
		
		#Get the repoty type : such as 1  for MCSR,2 for CSMT
		$intTypeOFReport = 0;
	
		#Set the report download by value to pdf : 0
		$strReportFileType = "pdf";
		
		# Email addresses are not available for case pdf
		$strEmail = "Download by pdf";
		
		//start : change
		$strHeader = $base_url . "/Dashboard/Report/Views/pdfhtml/header";
		$strFooter = $base_url . "/Dashboard/Report/Views/pdfhtml/footer";
		switch ($strType) {
			case "MCSR":
				$strUrl = http_build_query(array('client_og' => $intCompanyOgId, 'client_og_nid' => $intCompanyNid));
				$strSrc1 = $base_url . "/Dashboard/Report/Views/" . base64_encode($strUrl);
				exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrc1 . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf");
				$intTypeOFReport = 2;
				break;
		
			case "CSMT":
				if ($strMediaType == '' && ($strMediaType != "")) {
					drupal_goto($base_url);
					die;
				}
				$strUrl = http_build_query(array('client_og_nid' => $intCompanyNid,
						'client_og' => $intCompanyOgId,
						'strMediaType' => "$strMediaType"));
				$strSrc1 = $base_url . "/Dashboard/Report/Views/MediaType/" . base64_encode($strUrl);
				exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrc1 . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf");
				$intTypeOFReport = 3;
				break;
			case "CSC":
				if ((int)$strMediaType == 0) { //Check campaign Id [$steMediaType: is nothing but campaign id]
					drupal_goto($base_url);
					die;
				}
				$strUrl = http_build_query(array('client_og_nid' => $intCompanyNid,
						'client_og' => $intCompanyOgId,
						'intCampaignId' => "$strMediaType")); // here is cmap
				$strSrc1 = $base_url . "/Dashboard/Report/Views/MediaType/" . base64_encode($strUrl);
		
				exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrc1 . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf");
				$intTypeOFReport = 4;
				break;
		
			case "FMR":
				if ((int)$strMediaType == 0) {
					drupal_goto($base_url);
					die;
				}
				$strUrl = http_build_query(array('client_og_nid' => $intCompanyNid,
						'intMediaId' => "$strMediaType",
						'client_og' => $intCompanyOgId));
				$strSrc1 = $base_url . "/Dashboard/Report/Views/MediaId/" . base64_encode($strUrl);
		
				exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrc1 . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf");
				$intTypeOFReport = 5;
				break;
		
			default:
				drupal_goto($base_url);
				die;
		}
		//Insert the details of downloaded report.
		$this->objAnalyticDB->fnInsert3blReportDetails($intCompanyNid, $strEmail, $intTypeOFReport, $strReportFileType); //clsCompanyData
		
		$file = "/var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/Report$strTime.pdf";
		
		if (file_exists($file)) {
			header('Content-Description: File Transfer');
			header('Content-Type: application/octet-stream');
			header("Content-Type: application/force-download");
			header('Content-Disposition: attachment; filename=' . urlencode(basename($file)));
			header('Expires: 0');
			header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
			header('Pragma: public');
			header('Content-Length: ' . filesize($file));
			ob_clean();
			flush();
			readfile($file);
			@unlink($file);
		}
		
		return true;
	}
	
	public function fnProcessReportsExcel($intCompanyOgId, $intCompanyNid, $strType, $strMediaType)
	{
		global $base_url;
		
		#Get the repoty type : such as 1  for MCSR,2 for CSMT
		$intTypeOFReport = 0;

		#Set the report download by value to excel : 1
		$strReportFileType = "excel";
		
		# Email addresses are not available for case pdf
		$strEmail = "Download by Excel";
		
		header("Content-Type: application/vnd.ms-excel;");
		header("Content-type: application/x-msexcel;");
		header("Content-disposition: attachment; filename=Report" . time() . ".xls");
		
		//start : change
		switch ($strType) {
		case "MCSR":
		$strUrl = http_build_query(array('client_og' => $intCompanyOgId,
		'client_og_nid' => $intCompanyNid,
		"fileTyp" => "excel"));
		$strSrc1 = $base_url . "/Dashboard/Report/Views/" . base64_encode($strUrl);
		echo file_get_contents($strSrc1);
		$intTypeOFReport = 2;
		break;
		
		case "CSMT":
		if ($strMediaType == '' && ($strMediaType != "")) {
		drupal_goto($base_url);
		die;
		}
		$strUrl = http_build_query(array('client_og_nid' => $intCompanyNid,
		'strMediaType' => "$strMediaType",
		'client_og' => $intCompanyOgId,
		"fileTyp" => "excel"));
		$strSrc1 = $base_url . "/Dashboard/Report/Views/MediaType/" . base64_encode($strUrl);
				echo file_get_contents($strSrc1);
				$intTypeOFReport = 3;
				break;
		
				case "CSC":
				if ((int)$strMediaType == 0) { //Check campaign Id [$steMediaType: is nothing but campaign id]
		drupal_goto($base_url);
		die;
		}
		$strUrl = http_build_query(array('client_og_nid' => $intCompanyNid,
		'intCampaignId' => "$strMediaType",
		'client_og' => $intCompanyOgId,
		"fileTyp" => "excel"));
		$strSrc1 = $base_url . "/Dashboard/Report/Views/MediaType/" . base64_encode($strUrl);
			echo file_get_contents($strSrc1);
			$intTypeOFReport = 4;
			break;
		
			case "FMR":
			if ((int)$strMediaType == 0) {
			drupal_goto($base_url);
			die;
		}
		$strUrl = http_build_query(array('client_og_nid' => $intCompanyNid,
		'intMediaId' => "$strMediaType",
		"fileTyp" => "excel",
		'client_og' => $intCompanyOgId));
		$strSrc1 = $base_url . "/Dashboard/Report/Views/MediaId/" . base64_encode($strUrl);
			echo file_get_contents($strSrc1);
			$intTypeOFReport = 5;
			break;
		
			default:
			drupal_goto($base_url);
			die;
		}
		// Call function fnInsert3blReportDetails To insert the details of report
			$this->objAnalyticDB->fnInsert3blReportDetails($intCompanyNid, $strEmail, $intTypeOFReport, $strReportFileType);
	}
	
	public function fnProcessFMRReport($intCompanyOgId, $intFmrId)
	{
		global $base_url;
		
		# Get the Analytic Views and clicks.
		$objFMRClickInfo = $this->objReportsDB->fnGetFMRAnalyticClicks($intFmrId);		
		$arrFmrClicks = $this->objReportsController->fnGetAnalyticClickData($objFMRClickInfo);
		
		$objFMRViewInfo = $this->objReportsDB->fnGetFMRAnalyticView($intFmrId);
		$arrFmrViews = $this->objReportsController->fnGetAnalyticViewData($objFMRViewInfo);
		
		$arrAnalyticData = array();
		$arrAnalyticData['3blmedia']['view'] = ($arrFmrViews['3blmedia']['view']) ? $arrFmrViews['3blmedia']['view'] : 0;
		$arrAnalyticData['3blmedia']['click'] = ($arrFmrClicks['3blmedia']['click']) ? $arrFmrClicks['3blmedia']['click'] : 0;
		$arrAnalyticData['3bl_widgets']['view'] = ($arrFmrViews['3bl_widgets']['view']) ? $arrFmrViews['3bl_widgets']['view'] : 0;
		$arrAnalyticData['3bl_widgets']['click'] = ($arrFmrClicks['3bl_widgets']['click']) ? $arrFmrClicks['3bl_widgets']['click'] : 0;
		$arrAnalyticData['click_cron']['view'] = ($arrFmrViews['click_cron']['view']) ? $arrFmrViews['click_cron']['view'] : 0;
		$arrAnalyticData['click_cron']['click'] = ($arrFmrClicks['click_cron']['click']) ? $arrFmrClicks['click_cron']['click'] : 0;
		
		# Combine the clicks and views of 3blmedia and click cron
		$arrAnalyticData['3blmedia']['view'] = $arrAnalyticData['3blmedia']['view'] + $arrAnalyticData['click_cron']['view'];
		$arrAnalyticData['3blmedia']['click'] = $arrAnalyticData['3blmedia']['click'] + $arrAnalyticData['click_cron']['click'];
		
		//Function for Getting 3bl Newsletter Clicks
		$intClickCount = $this->objReportsDB->fnGetFMRNewsletterClick($intFmrId);
		
		$arrAnalyticData['newsletter']['view'] = 0;
		$arrAnalyticData['newsletter']['click'] = ($intClickCount > 0) ? $intClickCount : 0;
		
		# Get the justmeans data.
		//This block of code is to fetch clicks and view of FMR from the Justmeans newsletters from justmeans database.
		# Use Justmeans database.
		db_set_active('justmeans');
		
		// Justmeans views and clicks
		$intJMClicks = $this->objReportsDB->fnGetJMClicks($intFmrId, "Justmeans");
		$intJMViews = $this->objReportsDB->fnGetJMViews($intFmrId, "Justmeans");
		$arrAnalyticData['jm']['view'] = $intJMViews;
		$arrAnalyticData['jm']['click'] = $intJMClicks;
		
		// Justmeans newsletter views and clicks
		$intJMNewsletterClicks = $this->objReportsDB->fnGetJMClicks($intFmrId, "Jm-Newsletter");
		$intJMNewsletterViews = $this->objReportsDB->fnGetJMViews($intFmrId, "Jm-Newsletter");
		$arrAnalyticData['jm-newsletter']['view'] = $intJMNewsletterViews;
		$arrAnalyticData['jm-newsletter']['click'] = $intJMNewsletterClicks;
		#use default databse setting.
		db_set_active();
		
		# Get the micro list clicks and view.
		$arrViewCount = $this->objReportsDB->fnGetMicroListClickCount($intFmrId);
		$arrMicroListDetails = $this->objReportsController->fnGetMicrolistDetails($arrViewCount);
		
		// Ger node information
		$arrInfo = node_load($intFmrId);
		
		# Make total of 3bl media and 3bl widgets view and clicks
		$intTotalClicks = $arrAnalyticData['3blmedia']['click'] + $arrAnalyticData['3bl_widgets']['click'] + $arrAnalyticData['newsletter']['click']+ $intJMClicks + $intJMNewsletterClicks;
		$intTotalViews = $arrAnalyticData['3blmedia']['view'] + $arrAnalyticData['3bl_widgets']['view'] + $arrAnalyticData['newsletter']['view']+ $intJMViews + $intJMNewsletterViews;
		
		$strPrimaryCategory = '';
		$strSecondaryCategory = '';
		
		if (!empty($arrInfo->field_fmr_primary_category['und'])) {
			foreach ($arrInfo->field_fmr_primary_category['und'] as $arrPCategory) {
			$arrPrimaryCategory[] = $arrPCategory['target_id'];
		}
		$strPrimaryCategory = implode(',', $arrPrimaryCategory);
		}
		
			if (!empty($arrInfo->field_fmr_taxonomy['und'])) {
		
			foreach ($arrInfo->field_fmr_taxonomy['und'] as $arrSCategory) {
			$arrSecondaryCategory[] = $arrSCategory['target_id'];
		}
		$strSecondaryCategory = implode(',', $arrSecondaryCategory);
			}
		
			#Get the FMR Category From Taxonomy Table
			$arrFMRCategory = $this->objReportsDB->fnGetSecondaryCategory($strPrimaryCategory, $strSecondaryCategory);
		
			#FMR Title
			$strFMRTitle = $arrInfo->title;
			#FMR MediaType
			$strFMRMediaType = $arrInfo->field_fmr_type_of_content['und']['0']['value'];
			#FMR published Date
			$fmrPublishedDate = $arrInfo->field_fmr_date_time['und']['0']['value'];
			#Company url : node/company nodeId
			$strCompanyUrl = $arrInfo->client_nid;
			#To get the Company related information
			$arrCompInfo = node_load($strCompanyUrl);
			# Get company Name
			$strCompanyName = $arrCompInfo->title;
		
			# Build Array of basic info about the FMR.
			$arrFinalResult = array();
			$arrFinalResult['FMRID'] = $intFmrId;
			$arrFinalResult['title'] = $strFMRTitle;
			$arrFinalResult['FMRTYPE'] = $strFMRMediaType;
			$arrFinalResult['published_date'] = date("Y-m-d", strtotime($fmrPublishedDate));
			$arrFinalResult['totalClicks'] = $intTotalClicks;
			$arrFinalResult['totalviews'] = $intTotalViews;
			$arrFinalResult['CompName'] = $strCompanyName;
			$arrFinalResult['CompUrl'] = $strCompanyUrl;
			$arrFinalResult['arrFMRCategory'] = $arrFMRCategory;
			
			# Build the FMR report in HTML format.
			$strBuildHtml = $this->objReportsController->fnBuildHtmlFMRReport($arrFinalResult, $arrAnalyticData, $arrMicroListDetails);
			return $strBuildHtml;
		
	}
	
	public function fnProcessFMRMonthlyReport($intCompanyOgId, $intFmrId)
	{
		// Object of Report class
		list($arrFMRDetails,$strCurrentDate, $intPrevMonthDate) = $this->objReportsDB->fnGetLastPublishedFMRInfo();
		
		// to get FMRs information distributed in the previous 30 days		
		$arrFMRInfo = array();
		if (is_array($arrFMRDetails) && count($arrFMRDetails) > 0) {
		 
			list($arrFMRInfo, $strFMRIds) = $this->objReportsController->fnGetLatestFMRInfo($arrFMRDetails);
			 
		}

		# Build the FMR report in HTML format and download it in the excel file.
	    $strHtml = $this->objReportsController->fnBuildHtmlFMRMonthlyReport($arrFMRInfo, $strFMRIds);
		return $strHtml;
	}
	
	public function fnProcessFMRWeeklyInternalReport($strActionType)
	{
		global $base_url, $conf;
		
		
		//To get the fmr details which associate to micro list.
		//list($arrFMRInfo, $strFMRIds, $strCurrentDate, $intPrevMonthDate) = $objReports->fnGetLastPublishedFMRInfo(7, "weekly");
		
		list($arrFMRDetails, $strCurrentDate, $intPrevMonthDate) = $this->objReportsDB->fnGetLastPublishedFMRInfo(7, "weekly");
		
		// to get FMRs information distributed in the previous 30 days
		$arrFMRInfo = array();
		if (is_array($arrFMRDetails) && count($arrFMRDetails) > 0) {
				
			list($arrFMRInfo, $strFMRIds) = $this->objReportsController->fnGetLatestFMRInfo($arrFMRDetails);
		
		}
		
		if ($strFMRIds != "") {
		
			# Get the 3BL Views and clicks.
			$obj3BLFMRClickInfo = $this->objReportsDB->fnGetFMRAnalyticClicks($strFMRIds, "weekly"); //clsReportsSQL
			$arr3BLFmrClicks = $this->objReportsController->fnGetAnalyticClickData($obj3BLFMRClickInfo,"weekly");//clsReports
		
			$obj3BLFMRViewInfo = $this->objReportsDB->fnGetFMRAnalyticView($strFMRIds, "weekly");
			$arr3BLFmrViews = $this->objReportsController->fnGetAnalyticViewData($obj3BLFMRViewInfo, "weekly");
		
			//Function for Getting 3bl Newsletter Clicks
			$arr3BLNewsletterClickCount = $this->objReportsDB->fnGetFMRNewsletterClick($strFMRIds, "weekly");
		
			# Get the justmeans data.
			//This block of code is to fetch clicks and view of FMR from the Justmeans newsletters from justmeans database.
			# Use Justmeans database.
			db_set_active('justmeans');
		
			// Justmeans views and clicks
			$arrJMClicks = $this->objReportsDB->fnGetJMClicks($strFMRIds, "Justmeans", "weekly"); //JmClicksViews
			$arrJMViews = $this->objReportsDB->fnGetJMViews($strFMRIds, "Justmeans", "weekly"); //JmClicksViews
		
			// Justmeans newsletter views and clicks
			$arrJMNewsletterClicks = $this->objReportsDB->fnGetJMClicks($strFMRIds, "Jm-Newsletter", "weekly"); //JmClicksViews
		
			#use default database setting.
			db_set_active();
		}
		
		$arrWeeklyFMRInfo = array("arrFMRInfo"                 => $arrFMRInfo,
				"arr3BLFmrClicks"            => $arr3BLFmrClicks,
				"arr3BLFmrViews"             => $arr3BLFmrViews,
				"arr3BLNewsletterClickCount" => $arr3BLNewsletterClickCount,
				"arrJMClicks"                => $arrJMClicks,
				"arrJMViews"                 => $arrJMViews,
				"arrJMNewsletterClicks"      => $arrJMNewsletterClicks,
				"base_url"                   => $base_url,
				"strCurrentDate"             => $strCurrentDate,
				"intPrevMonthDate"           => $intPrevMonthDate,
				"imagepath"                  => $conf['IMAGES_PATH_3BL'],
				"download_report_url"        => "/admin/reports/analytics/week-at-a-glance/excel",
				"strDownloadExcel"           => $strActionType
		);
		
		return array($arrWeeklyFMRInfo, $strCurrentDate, $intPrevMonthDate);
		
	}

	
}