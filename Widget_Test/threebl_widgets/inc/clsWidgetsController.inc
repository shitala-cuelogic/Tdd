<?php
module_load_include('inc', 'threebl_widgets', 'inc/ClsWidgets');
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsTracking');
module_load_include('inc', 'threebl_widgets', 'inc/widget_analytics');

class clsWidgetsController 
{
	
	private $objWidgets;
	private $objTracking;
	public $AffiliateActive;
	
	public function __construct()
	{
		$this->objWidgets = new ClsWidgets();
		$this->objTracking = new clsTracking();
		$this->AffiliateActive = $this->objTracking->AffiliateActive;
	}
	
	/**
	 * Function for widget details
	 *
	 * @param int        $strTokenId  : Widget Id
	 * @param string     $strPage     : calling page
	 * @param int        $intNewsId   : FMR nid
	 * @param bool       $boolAddView : Flag which indicate views added into the database or not.
	 * @param int        $intPageNo   : page number for pagination
	 *
	 * @return array
	 */
	function fnWidgetNews($strTokenId, $strPage, $intNewsId = 0, $boolAddView = true, $intPageNo = 1)
	{
		$arrData = $this->objWidgets->fnWidgetNews($strTokenId, $strPage, $intNewsId, $boolAddView, $intPageNo);
		return $arrData;
	}
	
	/**
	 * Add Views and Clicks Count for FMR
	 *
	 * @param array  $arrNodeIds  : Array of FMR node ids
	 * @param bool   $boolAddView : Flag which indicate add views and clicks for FMRs
	 * @param int    $intWidgetId : Widget entity id
	 * @param string $strPage     : widget page type
	 *
	 * @return array
	 */
	
	function fnAddCompanyAnalyticsViews($intCompanyNodeId = 0, $intWidgetId = 0, $intFlag = 0)
	{
		fnAddCompanyAnalyticsViews($intCompanyNodeId, $intWidgetId, $intFlag);
		return true;
	}
}