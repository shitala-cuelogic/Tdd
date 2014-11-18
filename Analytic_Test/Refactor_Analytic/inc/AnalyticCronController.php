<?php

module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalytics');
module_load_include('inc', 'refactor_Analytic', 'inc/clsAnalyticDatabase');

class AnalyticCronController {

	private $analyticProcessing;
	private $analyticDB;

	public function __construct()
	{
		$this->analyticProcessing = new clsAnalytic();
		$this->analyticDB = new clsAnalyticDatabase();
	}

	public function fnProcessAnalyticBenchmarkCron()
	{
		$objLastMonthPublishedFMRs = $this->analyticDB->fnGetLastMonthPublishedFMRIds();

		// Initialize variables
		$arrFMRIds = array();
		$arrBenchmarkFMRInfo = array();
		$arrBenchMarkByMediaType = array();
		$arrBenchmarkFMRIds = array();
		$strFMRIds = '';

		// Group FMR Ids by Media type and Primary category
		if (!empty($objLastMonthPublishedFMRs)) {

			// To get Benchmark information
			list($arrBenchmarkFMRInfo, $arrFMRIds, $arrBenchMarkByMediaType, $arrBenchmarkFMRIds) = $this->analyticProcessing->fnGetBenchmarkInfo($objLastMonthPublishedFMRs);

			//Implode FMR ids
			$strFMRIds = implode(', ', $arrFMRIds);
		}

		if ($strFMRIds != "") {
			// GET the Views Related data
			$arrFMRViews = $this->analyticDB->fnGetBenchmarkViews($strFMRIds); //clsBenchmark

			// GET the Click Related data
			$arrFMRClicks = $this->analyticDB->fnGetBenchmarkClick($strFMRIds); //clsBenchmark

			if (count($arrFMRIds) > 0) {
				
				//Benchmark Calculation with respwc
				// $arrBenchmarkFMRInfo Group FMR Ids by Media type and Primary category
				$this->analyticProcessing->fnGroupFMRByMediaTypeAndPrimaryCategory($arrBenchmarkFMRInfo, $arrBenchmarkFMRIds);
					
				//GET all FMR with Videos
				$arrFMRWithVideos = $this->analyticDB->fnGetBenchmarkFMRWithVideo(); //clsBenchmark
					
				// $arrBenchmarkFMRInfo Group FMR Ids by Media type
				$this->analyticProcessing->fnGroupFMRByMediaType($arrBenchMarkByMediaType);

			}
		}
	}

	public function fnProcessAnalyticViewCron()
	{
		// Get records from threebl_tmp_fmr_headline_views table
		$arrResults = $this->analyticDB->fnGetFMRHeadlineViews();
		// Loop through results

		//if array exist or not.
		if (is_array($arrResults)) {

			foreach ($arrResults as $key => $value) {
				// Get fields from query row
				$strIpAddress = $value['ip_addr'];
				//$view_datetime = $value['view_datetime'];
				$strViewDate = $value['view_date'];
				$strFmrResult = $value['fmr_result'];

				// Put some of the basic info about the views into an associative array
				$tmpViewArray = array();
				$tmpViewArray['ip_addr'] = $strIpAddress;
				//$tmpViewArray['view_datetime'] = $view_datetime;
				$tmpViewArray['view_date'] = $strViewDate;

				// unserialize the results from the view, contained in $fmr_result
				$objArrFMR = unserialize($strFmrResult);

				// Now loop through the FMR info and use the FMR type and nid to build
				// an associative array, keyed by FMR type
				$arrFmrViews = array();
	
				//if not empty
				if (is_array($objArrFMR)) {
					foreach ($objArrFMR as $objFMR) {
						//Getting FMR TYPE with node id
						$arrFmrViews[$objFMR->_field_data['nid']['entity']->field_fmr_type_of_content['und'][0]['value']][] = $objFMR->nid;
					}
				}

				// Call fnViewsProcessing(..)
				//echo "<pre>"; print_r($arrFmrViews); print_r($tmpViewArray); echo "</pre>";echo $key. "<br/>";
				$this->analyticProcessing->fnViewsProcessing($arrFmrViews, $tmpViewArray);

				// Now, delete the record from threebl_tmp_fmr_headline_views
				$this->analyticDB->fnDeleteFMRHeadlineViews($key);
				//Update threebl_tmp_fmr_headline_views flag for added is added in click table
				//$ObjClickView->fnUpdateHeadLineView($key);
				//watchdog("results_id", $key);

			}//foreach end.

			// @mail('sankets.cuelogic@gmail.com', '3BL View END',"KEY = ".$key);
		}

		return true;
	}

	public function fnProcessAnalyticClickCron($intAid, $strLimit)
	{

		//getting the value from accesslog table
		$arrGetPathData = $this->analyticDB->fnGetAccesslogValue($intAid, $strLimit); //clsClickView
		//watchdog('Click Cron SQL', $strGetPathDataQ);
		// check if array value exist or not.

		if (is_array($arrGetPathData) && count($arrGetPathData) > 0) {
			@mail('sankets.cuelogic@gmail.com', '3BL Start', "Start of the Code time".date("y-m-d H:i:s"));
			//watchdog('Total Count','<pre>'.Count($arrGetPathData).'</pre>');

			$intCount = 0; //counter for checking record

			//for each start
			foreach ($arrGetPathData as $intKey => $arrGetPathDataDtls) {
				$strPath  	    = $arrGetPathDataDtls->path;
				$strHostName    = $arrGetPathDataDtls->hostname;
				$strReferUrl 	= preg_replace('/[^(\x20-\x7F)]*/', '', $arrGetPathDataDtls->url);
				$strTimeStamp   = $arrGetPathDataDtls->timestamp;
				$strAid 	    = $arrGetPathDataDtls->aid;
				$strDate        = date("Y-m-d H:i:s", $strTimeStamp);
					
				//getting the media id from above path value.
				$arrSource      = explode("/", $strPath);
				$intMediaId     = (int)$arrSource[1];
					
				//checking media id exits or not
				if ((int)$intMediaId) {
					//checking media id exist or not for type FMR
					$intMediaNodePublished = $this->analyticDB->fnCheckMediaExists($intMediaId); //clsClickView
					//if media-id exist or not.
					if ((int)$intMediaNodePublished) {
						//getting media details
						$arrGetMediaData = $this->analyticDB->fnGetMediaDetails($intMediaId);
							
						$intCompanyOgId = 0;
						$strFMRType = "";
							
						//checking media exist or not.
						if (is_array($arrGetMediaData) && count($arrGetMediaData) > 0) {
							$intCompanyOgId 	= $arrGetMediaData[0]->companyOgId;
							//getting FMR Type of particular media-id.
							$strFMRType = $this->analyticDB->fnGetFMRTypeForMedia($intMediaId); //clsClickView
						}//end if
							
						if ($strFMRType !="" && (int)$intCompanyOgId) {
								
							//Checking click count table data already exists or not.
							$intRecordExist = $this->analyticDB->fnCheckClickExists($intMediaId, $intCompanyOgId, $strFMRType, $strDate); //clsClickView
								
							//if click table count is zero.
							if (!(int)$intRecordExist) {
								//getting Refer-Url
								$strReferUrl = addslashes(trim(htmlspecialchars($strReferUrl)));
									
								$arrAllView = array(); // Array for Click and View table
									
								$arrAllView['intCompanyId'] = $intCompanyOgId;
								$arrAllView['intMediaId'] = $intMediaId;
								$arrAllView['strChannel'] = '3bl';
								$arrAllView['strFMRType'] = $strFMRType;
								$arrAllView['intChannelId'] = 0;
								$arrAllView['strRefererUrl'] = $strReferUrl;
								$arrAllView['strClickDate'] = $strDate;
								$arrAllView['strHostName'] = $strHostName;
								//echo "<pre>";print_r($arrAllView);echo "</pre>";
								//Function for insert value in click tables with cron flage is 1
								$strFlagValue = $this->analyticDB->fnAddAllClicks($arrAllView, 1); //clsClickView
									
								// Add respective views for the clicked FMR. It will avoid the click/views ratio issue.
								$arrAllView = array();
								$arrAllView['intCompanyId'] = $intCompanyOgId;
								$arrAllView['intMediaId'] = $intMediaId;
								$arrAllView['strChannel'] = 'click_cron';
								$arrAllView['strFMRType'] = $strFMRType;
									
								$this->analyticDB->fnAddAllViews($arrAllView); //clsClickView
									
							} else {
								watchdog('Data Already', $strGet3BlClicksCnt.'Data Already present Aid==' .$intKey);
							}
						}
						###############################################################
						#print "<br> $strGetMediaDataQ  => $intJm3BlId => $intCompanyOgId";
						### INSERT DATA IN NEW TABLE###################################
					}
				}
			}//end for each

			variable_set("aid", $strAid); //commented by sanket
			watchdog('Clicks Cron', 'Ending AID: '.$strAid);
			//echo $strValue = "\n END Aid = $strAid \n Start Aid => ".$intAid; //commented by sanket
			//@mail('sankets.cuelogic@gmail.com', 'Justmeans - 3BL End',"End of the Code <br/>time".date("y-m-d H:i:s"));
			// echo "<br/><br>Done";die;
		}
	}


}