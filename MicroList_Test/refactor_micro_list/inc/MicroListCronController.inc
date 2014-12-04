<?php
module_load_include('inc', 'refactor_micro_list', 'inc/clsMicroListController');
module_load_include('inc', 'refactor_micro_list', 'inc/clsMicroListDatabase');
module_load_include('inc', 'threebl_analytics_reports', 'inc/sendEmail');

class MicroListCronController 
{
	private $objMicroListController;
	private $objMicroListDB;
	
	public function __construct()
	{
		$this->objMicroListController = new clsMicroListController();
		$this->objMicroListDB = new clsMicroListDatabase();
	}
	
	public function fnProcessClientMicroListReportCron()
	{
		
		global $base_url, $conf;
		
		// Get List Type
		$strListType = "client";
		$strMicroListName = "Client Micro List Report";
		$strMicroListFileName = "Client-Micro-List-Report";
		
		//Get start and End date of Previous month
		$intCurMonth = date("m");
		$intCurYear = date("Y");
		$intDaysInPrevMonth = date('t', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));
		$intPrevMonth = date('m', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));
		$intPrevMonthYear = date('Y', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));
		$intPrevMonthDate = date('d', mktime(0, 0, 0, $intPrevMonth, 0+ $intDaysInPrevMonth, $intCurYear));
		
		$strGetStartDate = "$intPrevMonthYear-$intPrevMonth-01";
		$strGetEndDate   = "$intPrevMonthYear-$intPrevMonth-$intPrevMonthDate";
		$strDownloadExcel = "excel";
		
		//To get the fmr details which associate to micro list.
		list($arrResult, $intTotalRecords, $arrMicroList) = $this->objMicroListDB->fnGetFMRDetails($strGetStartDate, $strGetEndDate, "", "", "", "excel", $strListType);
		
		// set template variables
		$arrData = array('arrList' => $arrResult, "base_url" => $base_url, "imagepath" => $conf['IMAGES_PATH_3BL'], "download" => $strDownloadExcel, "arrMicroList" => $arrMicroList, "strMicroListName" => $strMicroListName, "intStartDate" => $strGetStartDate, "intEndDate" => $strGetEndDate);
		
		// Get HTML
		$strHTML =  theme('view-micro-list-report', $arrData);
		
		// Get Server Name
		$strServerName = $_SERVER['HTTP_HOST'];
		//Created xls file Path
		$strMicroListDetailReport = "/var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/excel/$strMicroListFileName" . time(). ".xls";
		
		// Open and write HTML content to xls file
		$fileHandler = fopen($strMicroListDetailReport, 'w+');
		fwrite($fileHandler, $strHTML);
		
		//Set Subject for report cron email on the basis of report type
		$strSubject = "Monthly $strMicroListName";
		
		//Set the message for the email on the basis of report type
		$strMessage =  "This report is sent automatically at the beginning of the month, covering the previous month. It contains all FMRs that included a Client Micro List for distribution.<br/><br/>Thank you,<br/>The 3BL Media Team<br/>";
		
		$strRecipientMail = "dfulton@3blmedia.com, consult@3blmedia.com, rkreke@3blmedia.com";
		
		// Send Monthly report
		fnSendEmail("3BL Media Admin", "no-reply@3blmedia.com", $strRecipientMail, $strSubject, $strMessage, $strMicroListDetailReport);
		
		//Unlink Micro List report excel file after mail sent
		if (is_file($strMicroListDetailReport)) {
			@unlink($strMicroListDetailReport);
		}
		exit;
		
	}
	
	/**
	 * Analytic Click Cron.
	 */
	public function fnProcessMicroListReportCron()
	{
		global $base_url, $conf;
		
		// Get List Type
		$strListType = "3bl";
		$strMicroListName = "3BL Micro List Report";
		$strMicroListFileName = "3BL-Micro-List-Report";
	
		//Get start and End date of Previous month
		$intCurMonth = date("m");
		$intCurYear = date("Y");
		$intDaysInPrevMonth = date('t', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));
		$intPrevMonth = date('m', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));
		$intPrevMonthYear = date('Y', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));
		$intPrevMonthDate = date('d', mktime(0, 0, 0, $intPrevMonth, 0+ $intDaysInPrevMonth, $intCurYear));
	
		$strGetStartDate = "$intPrevMonthYear-$intPrevMonth-01";
		$strGetEndDate   = "$intPrevMonthYear-$intPrevMonth-$intPrevMonthDate";
		$strDownloadExcel = "excel";
	
		//To get the fmr details which associate to micro list.
		list($arrResult, $intTotalRecords, $arrMicroList) = $this->objMicroListDB->fnGetFMRDetails($strGetStartDate, $strGetEndDate, "", "", "", "excel", $strListType);
	
		// set template variables
		$arrData = array('arrList' => $arrResult, "base_url" => $base_url, "imagepath" => $conf['IMAGES_PATH_3BL'], "download" => $strDownloadExcel, "arrMicroList" => $arrMicroList, "strMicroListName" => $strMicroListName, "intStartDate" => $strGetStartDate, "intEndDate" => $strGetEndDate);
	
		// Get HTML
		$strHTML =  theme('view-micro-list-report', $arrData);
	
		// Get Server Name
		$strServerName = $_SERVER['HTTP_HOST'];
		//Created xls file Path
		$strMicroListDetailReport = "/var/www/vhosts/" . $strServerName . "/httpdocs/sites/default/files/excel/$strMicroListFileName" . time(). ".xls";
	
		// Open and write HTML content to xls file
		$fileHandler = fopen($strMicroListDetailReport, 'w+');
		fwrite($fileHandler, $strHTML);
	
		//Set Subject for report cron email on the basis of report type
		$strSubject = "Monthly $strMicroListName";
	
		//Set the message for the email on the basis of report type
		$strMessage =  "This report is sent automatically at the beginning of the month, covering the previous month. It contains all FMRs that included a 3BL Micro List for distribution.<br/><br/>Thank you,<br/>The 3BL Media Team<br/>";
	
		$strRecipientMail = "dfulton@3blmedia.com, accounting@3blmedia.com, rkreke@3blmedia.com";
	
		// Send Monthly report
		fnSendEmail("3BL Media Admin", "no-reply@3blmedia.com", $strRecipientMail, $strSubject, $strMessage, $strMicroListDetailReport);
	
		//Unlink Micro List report excel file after mail sent
		if (is_file($strMicroListDetailReport)) {
			@unlink($strMicroListDetailReport);
		}
		exit;
	}
	
}