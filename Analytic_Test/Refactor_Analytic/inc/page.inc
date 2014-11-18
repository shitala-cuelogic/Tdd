<?php

/**
 * Justmeans Social Media Admin wrapper
 **/
//session_start();
function threebl_analytics_page($strAction = 'home')
{

  if(!isset($_SESSION['FORCOMPANYID']))
	{
		drupal_goto('client-multiple-login');
	}
  if ($strAction == '')
	 $strAction = 'home';

  $serverNameStore = $_SERVER['SERVER_NAME'];

  $jm_path = '/var/www/vhosts/jm2.3blmedia.com/httpdocs/';
  $add_to_path = $jm_path . PATH_SEPARATOR .'/var/www/vhosts/3blmedia.com/jm2:/var/www/vhosts/jm2.3blmedia.com/httpdocs/lib/pear/';
  set_include_path(get_include_path() . PATH_SEPARATOR . $add_to_path); 

  require_once($jm_path . "inc/config.inc.php");
  
  # DATA BASE OBJECT
  $hdlDb = clsDbUtil::fnConnectDB($_ARRDSN);
  
  if (!array_key_exists($strAction,$_ARRMODULE))
	return "";
  
  # BUILD CLASSNAME
  list($strModuleName,$strClassName) = explode(":",$_ARRMODULE[$strAction]["module"]);
   
  $strModuleClass = "cls".ucfirst($strModuleName);
  $strClassName = "cls".ucfirst($strClassName);

  # TEMPLATE OBJECT 
  $hdlTpl = ((int)$_ARRMODULE[$strAction]["tpl"])?clsUtil::fnTemplateClass($strModuleName):NULL;

  # INCLUDE CLASS FILE #
  require_once DOCUMENT_ROOT."/modules/$strModuleName/$strModuleClass.php";
  require_once DOCUMENT_ROOT."/modules/$strModuleName/$strClassName.php";

  # CLASS OBJECT AS PER MODULE AND FUSE
  $hdlModule = new $strClassName($hdlDb,$hdlTpl,$strModuleName,$strAction);

  # FLUSH PAGE OUTPUT IF ANY
  return preg_replace($ARR_BADWORDS, "****", $hdlModule->PAGEDATA);
   
  # UNSET OBJECT
  unset($hdlModule);
} // EO jm sm admin 
