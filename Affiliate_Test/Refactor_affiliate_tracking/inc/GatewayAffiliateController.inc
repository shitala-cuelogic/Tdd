<?php
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsAffiliatesMainController');
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsAffiliateDatabase');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticDatabase');
module_load_include('inc', 'refactor_Analytic', 'inc/commonFunction');
module_load_include('inc', 'threebl_analytics_reports', 'inc/sendEmail');

class GatewayAffiliateController
{
	private $objAffiliatesMain;
	private $objAffiliateDB;
	private $objAnalyticDB;

	public function __construct()
	{
		$this->objAffiliatesMain = new clsAffiliatesMainController();
		$this->objAffiliateDB = new clsAffiliateDatabase();
		$this->objAnalyticDB = new clsAnalyticDatabase();
	}
	
	public function setCompanyTrackingId($intCompany)
	{
		$this->objAffiliatesMain->setCompanyTrackingId($intCompany);
	}
	
	public function fnProcessEditNodeScript()
	{
		$arrMedia = $this->objAffiliateDB->fnEditNodeScript();
		if (count($arrMedia) > 0) {
			foreach ($arrMedia as $arrVal) {
				$node = node_load($arrVal->nid);
				$node->field_prconnect_tracking['und'][0]['value'] = '0';
				node_save($node);
				unset($node);
			}
		}
		
		return true;
	}
	
	public function fnProcessAffiliateTracking($intVC, $intFmr, $intNid, $intAffId)
	{
		global $conf, $base_url;
		
		fnCheckSession();
		
		if ((int)$intAffId > 0) {
			// To check whether the affiliate is Widget or not
		
			$arrEntity = entity_load('affiliates', array($intAffId));
			$arrResult = json_decode(json_encode($arrEntity[$intAffId]), 1);
		
			if (is_array($arrResult) && count($arrResult) > 0) {
				if ($arrResult['type'] == 'widget') {
					//To redirect to the affiliate URL
					drupal_goto($arrResult['field_affiliate_news_url']['und'][0]['value'], array("query"=>array("mid"=>$intNid)));
				} else {
					//To redirect to the affiliate URL
					$strURL = $this->objAffiliateDB->getAffiliateTrackingUrl($intNid, $intAffId);
					drupal_goto($strURL);
				}
			}
		}
		
		$arrAffiliates = array();
		// Get Latest FMR
		$arrLatestFmr = $this->objAffiliateDB->fnGetLatestFMR(); //Getting the list of all FMR Title
		
		if (count($arrLatestFmr) < 1) {
			drupal_goto($base_url);
		}
		
		//USE DEFAULT FIRST FMR SELECTED
		$intFMRId = $arrLatestFmr[0]->nid;
		
		$strParams = '';    //change
		
		//CHECK SELECTED FMR
		if ($intNid != '') {
		
			$intFMRId = $intNid;
			$intFmr = $this->objAffiliateDB->fnGetLatestFMR($intFMRId);
		
			//if Fmr not exit then redirect
			if ($intFmr == '') {
				drupal_goto($base_url."/Dashboard/Analytics/Tracking");
			}
			$strParams = "?fmr=".base64_encode($intFMRId);
		}
		
		//Main tab of (Analytics, Tracking ,Searching Engines, Reports);
		$strTabHtml =  $this->objAffiliatesMain->fnAnalyticsMaintab('Track');
		
		//Check if there fmrs are exist for the company or not.
		if ((int)$intFMRId > 0) {
		
			//use node details for fmr-Id
			$arrResult = node_load($intFMRId);
			$arrFMRInfo = json_decode(json_encode($arrResult), 1);
		
			//getting FMR Language type
			$intFmrLanguageType = $arrFMRInfo["field_fmr_non_english"]["und"][0]["value"];
		
			//Get all affiliates related to selected FMR
			$arrAffiliates = $this->objAffiliateDB->fnTrackingAffiliateList($intFMRId, '', $intFmrLanguageType);
		}
		
		$arrAffiliateRecords = array();
		
		//Collect Widget information
		foreach($arrAffiliates[0] as $arrAff ){
			$arrTemp = array();
			$arrTemp['filename'] = $arrAff->filename;
			$arrTemp['tracking_url'] = $arrAff->tracking_url;
			$arrTemp['type'] = $arrAff->type;
			$arrTemp['field_affiliate_news_url_value'] = $arrAff->field_affiliate_news_url_value;
			$arrTemp['field_affiliate_title_value'] = $arrAff->field_affiliate_title_value;
			$arrTemp['field_affiliate_description_value'] = $arrAff->field_affiliate_description_value;
			$arrTemp['field_affiliate_monthly_traffic_value'] = $arrAff->field_affiliate_monthly_traffic_value;
			$arrAffiliateRecords[] = $arrTemp;
		}
		
		foreach($arrAffiliates[1] as $arrAff ){
			$arrTemp = array();
			$arrTemp['filename'] = $arrAff->filename;
			$arrTemp['tracking_url'] = $arrAff->tracking_url;
			$arrTemp['type'] = $arrAff->type;
			$arrTemp['field_affiliate_news_url_value'] = $arrAff->field_affiliate_news_url_value;
			$arrTemp['field_affiliate_title_value'] = $arrAff->field_affiliate_title_value;
			$arrTemp['field_affiliate_description_value'] = $arrAff->field_affiliate_description_value;
			$arrTemp['field_affiliate_monthly_traffic_value'] = $arrAff->field_affiliate_monthly_traffic_value;
			$arrAffiliateRecords[] = $arrTemp;
		}
		
		if (is_array($arrAffiliateRecords) && count($arrAffiliateRecords) > 0) {
			shuffle($arrAffiliateRecords);
		}
		$vars = array(  'strTabHtml'	=> $strTabHtml,
				'arrLatestFmr'	=> $arrLatestFmr,
				'base_url'		=> $base_url,
				'arrAffiliates'	=> $arrAffiliateRecords,
				'selectedFMR'	=> $intFMRId,
				'widget_viewmore_link' => $base_url."/Dashboard/Analytics/Tracking".$strParams,
				"strImagePath" => $conf['IMAGES_PATH_3BL']
		);
		
		return $vars;
	}
	
	public function fnProcessAffiliateExcel($intFMRId)
	{
		fnCheckSession();
		
		//use node details for fmr-Id
		$arrResult = node_load($intFMRId);
		$arrFMRInfo = json_decode(json_encode($arrResult), 1);
		
		$strTitle = $arrFMRInfo['title']; //get node title
		
		//getting FMR Language type
		$intFmrLanguageType = $arrFMRInfo["field_fmr_non_english"]["und"][0]["value"];
		
		header("Content-Type: application/vnd.ms-excel;");
		header("Content-type: application/x-msexcel;");
		header("Content-disposition: attachment; filename=TrackingResults.xls");
		
		$strHTMLTable = '<table width="100%" border="1" cellspacing="2" cellpadding="2" bordercolor="#000000">
		<tr>
		<td colspan="4" align="left"><h2><strong>'. fnRemoveWordFormatting($strTitle).'</strong>&nbsp[<strong>'.date("F d, Y", strtotime($arrFMRInfo['field_fmr_date_time']['und'][0]['value'])).'</strong>] &nbsp;</h2></td>
		</tr>
		<tr>
		<td colspan="4">&nbsp;</td>
		</tr>
		<tr>
		<td align="center" width="25%"><strong>&nbsp;Publisher / Website&nbsp;</strong></td>
		<td align="center" width="15%"><strong>&nbsp;News    URL&nbsp;</strong></td>
		<td align="center" width="50%"><strong>&nbsp;Site    Description&nbsp;</strong></td>
		<td align="center" width="10%"><strong>&nbsp;Monthly Traffic&nbsp;</strong></td>
		</tr>';
		
		//Creating the Object of Class
		//$objClsTracking = new clsTracking((int)$_SESSION['client_og_nid']);
		
		$this->objAffiliatesMain->setCompanyTrackingId((int)$_SESSION['client_og_nid']);
		//Create List of affiliate for selected FMR.
		list($arrWidget, $arrAffiliates) = $this->objAffiliateDB->fnTrackingAffiliateList($intFMRId, true, $intFmrLanguageType);
		
		#Add Widgets records into the report
		foreach ($arrWidget as $arrRecord) {
			$strNewsUrl =  (strstr($arrRecord->field_affiliate_news_url_value, '?') == false) ? $arrRecord->field_affiliate_news_url_value."?mid=".$intFMRId : $arrRecord->field_affiliate_news_url_value."&mid=".$intFMRId;
		
			$strHTMLTable .="<tr>
			<td align='left' valign ='middle'>".$arrRecord->field_affiliate_title_value."</td>
			<td valign ='middle' align='center'>=HYPERLINK(\"".$strNewsUrl."\",\"Click to view\")</td>
			<td align='left' valign ='top'>".str_replace(array("\n", "\r", "\t"), '', fnRemoveWordFormatting($arrRecord->field_affiliate_description_value))."</td>
			<td valign ='middle' align='right'>".$arrRecord->field_affiliate_monthly_traffic_value."</td>
			</tr>";
		}
		
		//$arrViewClick contain all the combination of views and clicks fields.
		foreach ($arrAffiliates as $arrRecord) {
		
			if ($arrRecord->type == 'widget') {
				$strNewsUrl = (strstr($arrRecord->field_affiliate_news_url_value, '?') == false) ? $arrRecord->field_affiliate_news_url_value."?mid=".$intFMRId : $arrRecord->field_affiliate_news_url_value."&mid=".$intFMRId;
		
			} else {
				$strNewsUrl =  $arrRecord->tracking_url;
			}
			$strHTMLTable .="<tr>
			<td align='left' valign ='middle'>".$arrRecord->field_affiliate_title_value."</td>
			<td valign ='middle' align='center'>=HYPERLINK(\"".$strNewsUrl."\",\"Click to view\")</td>
			<td align='left' valign ='top'>".str_replace(array("\n", "\r", "\t"), '', utf8_decode($arrRecord->field_affiliate_description_value))."</td>
			<td valign ='middle' align='right'>".$arrRecord->field_affiliate_monthly_traffic_value."</td>
			</tr>";
		}
		
		$strHTMLTable .= '</table>';
		
		return $strHTMLTable;
	}
	
	public function fnProcessAffiliateCron()
	{
		global $conf;
		$arrFMRList = array();

		//Include all required files
		module_load_include('inc', 'threebl_affiliate', 'inc/clsTracking');

		//Creating the Object of Class
		$objClsTracking = new clsTracking();

		//Prconnect Affiliate Tracking
		$arrPRCFMRList = $this->objAffiliateDB->fnFMRTracking("prconnect");
		$arrMissingFMRs = array();

		//Process the FMR for PRConnect affilaite tracking
		if (count($arrPRCFMRList) > 0) {
			//COLLECT prconnect information
			foreach ($arrPRCFMRList as $fmr) {
				if ($fmr->prflag != 1) {
					//This is function for prconnect cron
					if ($this->objAffiliatesMain->fnPrconnectCron($fmr->nid) == "2") {
						$arrMissingFMRs[] = $fmr->nid;
					}
				}
				//Call the prconnect function which will collect all information about this FMR
			}
		}
		#Check Missing FMRs and email those to admin.
		if (count($arrMissingFMRs) > 0) {
			$strTo = $conf['threebl_affiliate_cron_to'];
			$strFrom = $conf['no_reply_from'];
			$strSubject = 'Missing FMR from PRConnect network';
			$strMailContent = 'Following FMRs are missing on PRConnect Network <br/>';
			$intCount = 1;
			foreach ($arrMissingFMRs as $rowFMR) {
				$strMailContent .= "$intCount. $rowFMR <br/>";
				$intCount++;
			}
			$strMailContent .= "Thanks,<br>3BL Media.<br>";
			$arrParams['message'] = $strMailContent;
			$boolSend = false;
			fnSendEmail("3BL Media", $strFrom, $strTo, $strSubject, $strMailContent, "", $boolSend);
		}

		//If there are any published FMRs on portal
		if (count($arrPRCFMRList) > 0) {
			//Call the spider Affiliate function and get information for all new added FMRs
			$this->objAffiliatesMain->fnTrackingAffiliates();
		}

	}
	
	public function fnProcessFMRInternalReport($intFMRId)
	{
		if ((int) $intFMRId > 0) {
			db_set_active('justmeans_prod');
		
			$arrShowMediaType = array(
					'press_release' => 'Press Release',
					'blog'          => 'Blog',
					'multimedia'    => 'Multimedia',
					'article'       => 'Article',
					'newsletter'    => 'Newsletter',
					'all'           => 'All'
			);
		
			$strJumeansUrl = "http://www.justmeans.com/node/";
		
			$intJMNodeId = $this->objAffiliateDB->fnGetJMNodeId($intFMRId);
		
			if ((int) $intJMNodeId > 0) {
				
				$objFMRClickInfo = $this->objAffiliateDB->fnGetFMRNewsLetterClicks($intJMNodeId);
				$arrFmrClicks = $this->objAffiliatesMain->fnGetFMRNewsLetterClickCount($objFMRClickInfo);
				
				$objFMRViewInfo = $this->objAffiliateDB->fnGetFMRNewsLetterViews($intJMNodeId);
				$arrFmrViews = $this->objAffiliatesMain->fnGetFMRNewsLetterViewCount($objFMRViewInfo);
				
				$arrFmrInfo = $this->objAffiliateDB->fnGetFMRInformation($intJMNodeId);
				$arrCompanyInfo = $this->objAffiliateDB->fnGetCompanyInfo($intJMNodeId);
		
				if (is_array($arrCompanyInfo) && count($arrCompanyInfo) > 0) {
					foreach ($arrCompanyInfo as $arrCompanyData) {
						$strCompanyName = stripslashes($arrCompanyData->label);
						$intCompanyId = (int) $arrCompanyData->etid;
					}
				}
		
				if (is_array($arrFmrInfo) & count($arrFmrInfo) > 0) {
					foreach ($arrFmrInfo as $arrFmrData) {
						$strFMRTile = stripslashes($arrFmrData->title);
						$intCreatedDate = stripslashes($arrFmrData->created);
						$strFMRType = stripslashes($arrFmrData->type);
						$strGetFMTType = $arrShowMediaType[$strFMRType];
						if ($intCreatedDate != "") {
							$strCreatedDate = date("l, F j, Y", $intCreatedDate);
						}
					}
				}
		
				$arrFMRInfo = array(
						"strFMRTile"     => $strFMRTile,
						"strdate"        => $strCreatedDate,
						"strCompanyname" => $strCompanyName,
						"FMRType"        => $strGetFMTType,
						"FMRUrl"         => $strJumeansUrl . $intJMNodeId,
						"CompanyUrl"     => $strJumeansUrl . $intCompanyId,
				);
			}
		}
		
		return array($arrFmrClicks, $arrFmrViews, $arrFMRInfo);
	}
	
	public function fnProcessTrackingPDF($intFMRId)
	{
		// global variables
		global $base_url, $conf;
		
		//use node details for fmr-Id
		$arrResult = node_load($intFMRId);
		$arrFMRInfo = json_decode(json_encode($arrResult), 1);
		
		// FMR Title
		$strTitle = $arrFMRInfo['title'];
		// FMR Publish Date
		$strPublishDate = date("F d, Y", strtotime($arrFMRInfo['field_fmr_date_time']['und'][0]['value']));
		
		// Primary Category
		$strPrimaryCategoryName = "";
		$intPrimaryCategoryId = (int) $arrFMRInfo['field_fmr_primary_category']['und'][0]['target_id'];
		$strPrimaryCategory = $this->objAnalyticDB->fnPrimaryCategoryName($intPrimaryCategoryId);
		if ($strPrimaryCategory != "") {
			$strPrimaryCategoryName = $strPrimaryCategory;
		}
		
		//getting FMR Media type
		$strFmrType = $arrFMRInfo["field_fmr_type_of_content"]["und"][0]["value"];
		$strGetFMRMediaType = $this->objAffiliatesMain->arrShowMediaType[$strFmrType];
		
		//getting FMR Language type
		$intFmrLanguageType = $arrFMRInfo["field_fmr_non_english"]["und"][0]["value"];
		
		//Creating the Object of Class
		//$objClsTracking = new clsTracking((int) $_SESSION['client_og_nid']);
		$this->objAffiliatesMain->setCompanyTrackingId((int) $_SESSION['client_og_nid']);
		
		//Create List of affiliate for selected FMR.
		list($arrWidget, $arrAffiliates) = $this->objAffiliateDB->fnTrackingAffiliatePdfList($intFMRId, true, $intFmrLanguageType);
		
		return array($strTitle, $strPublishDate, $arrWidget, $arrAffiliates, $strPrimaryCategoryName, $strGetFMRMediaType);
	}
	
	public function fnProcessDownloadAffiliateTrackingPDF($intFMRId)
	{
		
		// Base Url
		global $base_url;
		
		// Server Name
		$strServerName = $_SERVER['HTTP_HOST'];
		
		// Build query URL
		$strUrl = http_build_query(array('fmr' => $intFMRId));
		
		$strTime = time();
		
		// Destination URL
		$strSrc1 = $base_url."/Dashboard/Analytics/Affiliate/PDF/" . base64_encode($strUrl);
		$strApiKey = '31q8AjO7u0Ej';
		$strResultPDF = file_get_contents("http://api.htm2pdf.co.uk/urltopdf?apikey=$strApiKey&url=$strSrc1");
		
		// PDF file path
		$strFile = "/var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/TrackingResults_$strTime.pdf";
		file_put_contents($strFile, $strResultPDF);
		
		/******************************************************************************
		 Originally we used X virtual Frame Buffer to create PDF this was the command:
		exec('xvfb-run -a -s "-screen 0 640x480x16" /var/www/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html '.$strHeader.' --footer-html "'.$strFooter.'" '.$strSrc1." /var/www/vhosts/".$strServerName."/httpdocs/sites/default/files/pdf/TrackingResults_$strTime.pdf");
		
		To debug from the commandline without the "--quite" this is the tracking info for an old FMR:
		/usr/local/bin/wkhtmltopdf  --margin-top 15mm --header-html http://3blmedia.com/Dashboard/Report/Views/pdfhtml/header --footer-html http://3blmedia.com/Dashboard/Report/Views/pdfhtml/footer http://3blmedia.com/Dashboard/Analytics/Affiliate/PDF/Zm1yPTQ4ODE4 TrackingResults_1406660641.pdf
		
		There is an issue with wkhtmltopdf pulling in all of the thumbnail images and if for some reason drupal returns a 500 error wkhtmltopdf will just hang.
		
		// wkhtmltopdf V 0.12 is patched for QT support so XVFB shouldn't be necessary
		exec('/usr/local/bin/wkhtmltopdf --quiet --margin-top 15mm --header-html ' . $strHeader . ' --footer-html "' . $strFooter . '" ' . $strSrc1 . " /var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/pdf/TrackingResults_$strTime.pdf");
		******************************************************************************/
		
		// Download PDF file
		if (file_exists($strFile)) {
			header('Content-Description: File Transfer');
			header('Content-Type: application/octet-stream');
			header("Content-Type: application/force-download");
			header('Content-Disposition: attachment; filename=' . urlencode(basename($strFile)));
			header('Expires: 0');
			header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
			header('Pragma: public');
			header('Content-Length: ' . filesize($strFile));
			ob_clean();
			flush();
			readfile($strFile);
			@unlink($strFile);
		}
	}
	
}