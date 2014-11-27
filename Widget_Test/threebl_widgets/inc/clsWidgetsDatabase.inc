<?php
module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsWidgetSql');
module_load_include('inc', 'refactor_Analytic', 'inc/clsClickView');


class clsWidgetsDatabase {
	
	private $objWidgetSql;
	private $objClickView;
	
	
	public function __construct()
	{
		$this->objWidgetSql = new clsWidgetSql();
		$this->objClickView = new clsClickView();
	}
	
	/**
	 * To get widget Id
	 *
	 * @param int $strTokenId  : Widget Token Id
	 *
	 * @return int
	 */
	function fnGetWidgetId($strTokenId)
	{
		$intWidgetId = $this->objWidgetSql->fnGetWidgetId($strTokenId);
		return $intWidgetId;
	}
	
	function fnGetAffiliateExistCount($strValue)
	{
		$intCount = $this->objWidgetSql->fnGetAffiliateExistCount($strValue);
		return $intCount;
	}
	
	function fnCheckAffiliateCount($intId, $strValue)
	{
		$intCount = $this->objWidgetSql->fnCheckAffiliateCount($intId, $strValue);
		return $intCount;
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
	
	
	function fnGetMediaIDList($intCompanyNodeId)
	{
		$arrCompMediaIds = $this->objWidgetSql->fnGetMediaIDList($intCompanyNodeId);
		return $arrCompMediaIds;
	}
	
	/**
	 *  Function return url for related node
	 *
	 * @param $intNodeId : is the node id from 3BL
	 *
	 * @return string
	 */
	function getCompanyURL($intNodeId)
	{
		$strCompanyURL = $this->objWidgetSql->getCompanyURL($intNodeId);
		return $strCompanyURL;
	}
	
}