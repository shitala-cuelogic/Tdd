<?php
module_load_include('inc', 'threebl_widgets', 'inc/clsWidgetsController');
module_load_include('inc', 'threebl_widgets', 'inc/clsWidgetsDatabase');

class GatewayWidgetsController
{
	private $objWidgetsController;
	private $objWidgetsDB;
	
	public function __construct()
	{
		$this->objWidgetsController = new clsWidgetsController();
		$this->objWidgetsDB = new clsWidgetsDatabase();
	}
	
	public function fnProcessFrontPagePreview($intId)
	{
		global $base_url, $user;
		
		$data = array();
		# Get all information about
		if ($intId > 0) 
		{
			//Getting company token id
			$arrEntity = entity_load('affiliates', array($intId));
			$arrResult = json_decode(json_encode($arrEntity[$intId]), 1);
		
			$data = $this->objWidgetsController->fnWidgetNews($arrResult['field_token_id']['und'][0]['value'], 'more', 0, false);
		}
		
		return $data;
	}
	
	public function fnProcessWidgetPreview($intId)
	{
		global $base_url, $user;
		
		$data = array();
		# Get all information about
		if ($intId > 0) {
		
			//Getting company token id
			$arrEntity = entity_load('affiliates', array($intId));
			$arrResult = json_decode(json_encode($arrEntity[$intId]), 1);
		
			$data = $this->objWidgetsController->fnWidgetNews($arrResult['field_token_id']['und'][0]['value'], 'details', 0, false);
		}
		
		return $data;
	
	}

	public function fnProcessGenerateCode($intWidgetId, $strPath)
	{
		//getting the entity details
		$arrEntity = entity_load('affiliates', array($intWidgetId));
		$arrResult = json_decode(json_encode($arrEntity[$intWidgetId]), 1); //object to array
		$arrCodeData = array(
				'strPath' => $strPath,
				'intWidgetId'=>base64_encode($arrResult['field_token_id']['und'][0]['value']),
				'intWidgetPreviewId'=>$intWidgetId,
				'strTitle'=>$arrResult['field_affiliate_title']['und'][0]['value']);
		
		return $arrCodeData;
	}
	
	public function fnProcessAddAffiliatesValidate($strValue)
	{
		$intCount = $this->objWidgetsDB->fnGetAffiliateExistCount($strValue);
		return $intCount;
	}
	
	public function fnProcessEditAffiliatesValidate($intId, $strValue)
	{
		$intCount = $this->objWidgetsDB->fnCheckAffiliateCount($intId, $strValue);
		return $intCount;
	}
	
	public function fnProcessManageWidgetActivities($strTokenId, $strPage, $intNewsId, true, $intPageNo)
	{
		//getting widget data
		$arrData = $this->objWidgetsController->fnWidgetNews($strTokenId, $strPage, $intNewsId, true, $intPageNo);
		return $arrData;
	}
	
	/**
	 * Track increment click and view
	 * @param $strNodeId : FMR node Id
	 * @param $strWidgetId : Widget entity Id
	 * @return bool
	 */
	public function fnProcessIncrementClicksViews($intNodeId, $intWidgetId)
	{
		//Tracking click and view
		$this->objWidgetsController->fnAddCompanyAnalyticsViews($intNodeId, $intWidgetId);
		$strCompanyURL = $this->objWidgetsDB->getCompanyURL($intNodeId);
		
		return $strCompanyURL;
	}
	
	public function fnProcessEditWidgetLanguage()
	{
		$ObjEntity = entity_load('affiliates', false, array("type"=>"widget"));
		$arrAffiliateEntity = json_decode(json_encode($ObjEntity), 1); //object to array
		
		foreach ($arrAffiliateEntity as $arrWidgetInfo) 
		{
		
			$entity = new stdClass();
			$entity->type = 'widget';
			$entity->id = $arrWidgetInfo["id"];
		
			$entity->field_widget_fmr_language = array
			(
					'und' => array
					(
							"0" => array
							(
									'value' => "all"
							)
					)
			);
		
			entity_get_controller('affiliates')->save($entity);
			unset($entity);
		}
		
		return true;
	}

}