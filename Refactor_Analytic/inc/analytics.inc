<?php

/*function threebl_analytics_tracking_page($var = NULL) {

return 'TRACKING PAGE GOES HERE';
}*/

/**
 * Page callback for Dashboard Tracking Page
 **/
function threebl_analytics_tracking_page($node){
drupal_set_title('Tracking data for: ' . $node->title);
return '<pre>'.print_r($node,1).'</pre>';

}

/**
  *  Implements analytics click tracking.
  */

function threebl_analytics_clicks_tracking_page($var = NULL) {
		/**
		  *  We are grabbing host, channel account id, media id, media type from the URL.
		  */
		
		// Database connection details for inserting views data at 3blmedia.
		$other_database = array(
		  'database' => 'justmeans_prod',
		  'username' => 'csrnet', // assuming this is necessary
		  'password' => 'd3rh4cK', // assuming this is necessary
		  'host' => 'dbsrv.3blmedia.com', // assumes localhost
		  //'host' => '10.153.118.151', // assumes localhost
		  'driver' => 'mysqli', // replace with your database driver
		);
	  
		// replace 'YourDatabaseKey' with something that's unique to your module
	  
		Database::addConnectionInfo('justmeans_prod', 'default', $other_database);
		db_set_active('justmeans_prod');
		
		// If media is clicked from any other source other than 3blmedia or justmeans then we will get the host using HTTP REFERER
		/*if($strHost == "")
		{
			$strRefer = parse_url($_SERVER['HTTP_REFERER']);
			$strHost = $strRefer['host'];
		}
		// If we did not get any host we by default setting 3blmedia as host
		if($strHost == "")
		{
			$strHost = "3blmedia";
		}*/
		$strHost = "3blmedia";
		$strChannel = "";
		$intMediaId ="19047";
		$strMediaType = "pressrelease";
		$strCategoryType = '';
		$strActionName ='';
		$strEventLabel ='';
		$strGoogleAnalyticsCode ='';
		
		// Here we are going to validate all the hosts of 3blmedia, we are going to process google analytics code only after validating host. Also we are checking wether clicks are coming from justmeans,facebook, facebook pages, twitter, linkedin, tumblr or delicious accounts.
		if($strHost != "")
		{
			if(strstr($strHost,'3blmedia') || strstr($strHost,'live1.3blmedia'))
			{  
			    $strChannel = "3blmedia";  
			}
			/**
			*	 We are generating and event array for each media. 
			*/
			$arrEventArray = array();		
			
			// This is to check each media type and generate analytics code accordingly.
			if( $strMediaType == "pressrelease" )
			{
				// We will be getting id from company sync table, and media sync table. and we are generating a event with all the media separating the same with delimeter ('-'). 
				if($intMediaId)
				{
					$arrCompanyPressrelease = db_query("SELECT companyid, jm3blmediaid FROM {jm_company_article}  WHERE threeblmediaid = '$intMediaId'")->fetchAssoc();;
					
					if(is_array($arrCompanyPressrelease) && count($arrCompanyPressrelease)>0)
					{
						$intJMCompanyId = $arrCompanyPressrelease[0]["companyid"];
						$intJm3blMediaId = $arrCompanyPressrelease[0]["jm3blmediaid"];
						
						if($intJMCompanyId)
						{
							$intCompanyId = db_query("SELECT id FROM {jm_3bl_company} where 3bl_companyid = '$intJMCompanyId'")->fetchField();
						}
					}
				}
				
				if($intCompanyId && $intJm3blMediaId)
				{
					$strCategoryType = "$intCompanyId";
					$strActionName = $intCompanyId."-".$strMediaType;
					$strEventLabel = $intJm3blMediaId;
					$arrEventArray[] = array($strCategoryType,$strActionName,$strEventLabel);
				}
				
			}
			
			else if( $strMediaType == "3bl_article" )
			{
				
				// We will be getting id from company sync table, and media sync table. and we are generating a event with all the media separating the same with delimeter ('-').

				if($intMediaId)
				{
					$arrCompanyArticle = db_query("SELECT companyid,jm3blmediaid  FROM {jm_company_3blarticle} where threeblmediaid = '$intMediaId'")->fetchArray();
					if(is_array($arrCompanyArticle) && count($arrCompanyArticle)>0)
					{
						$intJMCompanyId = $arrCompanyArticle[0]["companyid"];
						$intJm3blMediaId = $arrCompanyArticle[0]["jm3blmediaid"];
						
						if($intJMCompanyId)
						{
							$intCompanyId = db_query("SELECT id FROM {jm_3bl_company} where 3bl_companyid = '$intJMCompanyId'")->fetchField();
						}
					}
				}
				
				if($intCompanyId && $intJm3blMediaId)
				{
					$strCategoryType = "$intCompanyId";
					$strActionName = $intCompanyId."-".$strMediaType;
					$strEventLabel = $intJm3blMediaId;
					$arrEventArray[] = array($strCategoryType,$strActionName,$strEventLabel);
				}
				
			}
			else if( $strMediaType == "3bl_newsletter" )
			{
				// We will be getting id from company sync table, and media sync table. and we are generating a event with all the media separating the same with delimeter ('-'). 
				
				if($intMediaId)
				{
					$arrCompanyNewsletter = db_query("SELECT companyid,jm3blmediaid  FROM {jm_company_3blnewsletter} where threeblmediaid = '$intMediaId'")->fetchArray();
					if(is_array($arrCompanyNewsletter) && count($arrCompanyNewsletter)>0)
					{
						$intJMCompanyId = $arrCompanyNewsletter[0]["companyid"];
						$intJm3blMediaId = $arrCompanyNewsletter[0]["jm3blmediaid"];
						
						if($intJMCompanyId)
						{
							$intCompanyId = db_query("SELECT id FROM {jm_3bl_company} where 3bl_companyid = '$intJMCompanyId'")->fetchField();
						}
					}
				}
				
				if($intCompanyId && $intJm3blMediaId)
				{
					$strCategoryType = "$intCompanyId";
					$strActionName = $intCompanyId."-".$strMediaType;
					$strEventLabel = $intJm3blMediaId;
					$arrEventArray[] = array($strCategoryType,$strActionName,$strEventLabel);
				}
				
			}
			
			else if( $strMediaType == "multimedia" )
			{
				// We will be getting id from company sync table, and media sync table. and we are generating a event with all the media separating the same with delimeter ('-'). 
				
				if($intMediaId)
				{
					$arrCompanyMultimedia = db_query("SELECT companyid,jm3blmediaid  FROM {jm_company_3blmultimedia} where threeblmediaid = '$intMediaId'")->fetchArray();
					if(is_array($arrCompanyMultimedia) && count($arrCompanyMultimedia)>0)
					{
						$intJMCompanyId = $arrCompanyMultimedia[0]["companyid"];
						$intJm3blMediaId = $arrCompanyMultimedia[0]["jm3blmediaid"];
						
						if($intJMCompanyId)
						{
							$intCompanyId = db_query("SELECT id FROM {jm_3bl_company} where 3bl_companyid = '$intJMCompanyId'")->fetchField();
						}
					}
				}
				
				if($intCompanyId && $intJm3blMediaId)
				{
					$strCategoryType = "$intCompanyId";
					$strActionName = $intCompanyId."-".$strMediaType;
					$strEventLabel = $intJm3blMediaId;
					$arrEventArray[] = array($strCategoryType,$strActionName,$strEventLabel);
					
				}
				
			}
			
			else if($strMediaType == "blog")
			{
				// We will be getting id from company sync table, and media sync table. and we are generating a event with all the media separating the same with delimeter ('-'). 
				
				if($intMediaId)
				{
					$arrCompanyBlogs = db_query("SELECT companyid,jm3blmediaid  FROM {jm_company_externalblogs} where threeblmediaid = '$intMediaId'")->fetchArray();
					if(is_array($arrCompanyBlogs) && count($arrCompanyBlogs)>0)
					{
						$intJMCompanyId = $arrCompanyBlogs[0]["companyid"];
						$intJm3blMediaId = $arrCompanyBlogs[0]["jm3blmediaid"];
						
						if($intJMCompanyId)
						{
							$intCompanyId = db_query("SELECT id FROM {jm_3bl_company} where 3bl_companyid = '$intJMCompanyId'")->fetchField();
						}
					}
				}
				
				if($intCompanyId && $intJm3blMediaId)
				{
					$strCategoryType = "$intCompanyId";
					$strActionName = $intCompanyId."-".$strMediaType;
					$strEventLabel = $intJm3blMediaId;
					$arrEventArray[] = array($strCategoryType,$strActionName,$strEventLabel);
				}
			}
			
			/**
			*	Create google analytics code here, we are getting event strings for perticular media in an event array.
			*/
			$strEventPush = '';
			if(is_array($arrEventArray) && count($arrEventArray)>0)
			{
				// we are separating each string from array and will get all the neccssary values for anaytic code.
				foreach ($arrEventArray as $arrEventDtl)
				{
					$strTempCategory = trim($arrEventDtl[0]);
					$strTempActionName = trim($arrEventDtl[1]);
					$strTempEventLabelName = trim($arrEventDtl[2]);
					$strEventPush .= "_gaq.push(['_trackEvent', '".$strTempCategory."', '".$strTempActionName."', '".$strTempEventLabelName."']);";
				}
			}
			
			echo "event push :->".$strEventPush;
			exit;
			
			// We generate google anayltic code and we push this code on media detail page.
			
			
			/**
			 *  For example we get the analytic code after processing above structure as 
			 
				_gaq.push(['_trackEvent', '2', '2-video', '64']);
				_gaq.push(['_trackEvent', '2', '2-justmeans', '64']);
				_gaq.push(['_trackEvent', '2', '2-justmeans', 'video']);
				_gaq.push(['_trackEvent', '2', '2-justmeans', '64-video']);
			 */
			
			$strGoogleAnalyticsCode ="<script type='text/javascript'>var _gaq = _gaq || [];
									_gaq.push(['_setAccount', '".GOOGLE_ACC_ID."']);
									_gaq.push(['_trackPageview']);
									$strEventPush
									(function() {
									var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
									ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
									var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
								})();</script>";
			###################################################################
				
		}	
			
		return $strGoogleAnalyticsCode;
		
} // EO clicks tracking
  
  
  /**
  *  Implements analytics Views Tracking
  */
	 // Database connection details for inserting views data at 3blmedia.
	 /*
	 $other_database = array(
	  'database' => 'justmeans_prod',
	  'username' => 'jmdb_user_prod', // assuming this is necessary
	  'password' => 'dg00gle', // assuming this is necessary
	  'host' => 'dbsrv.3blmedia.com', // assumes localhost
	  'driver' => 'mysqli', // replace with your database driver
	 );
	 */
	// Connect to JM db
	//Database::addConnectionInfo('justmeans_prod', 'default', $other_database);
	
function threebl_analytics_views_processing($fmrViews, $otherInfo, $strChannel="3blmedia"){
	 
	//watchdog('Analytics final view processing','FMRs:'.print_r($fmrViews,1));
	//watchdog('Analytics final view processing','Other info:'.print_r($otherInfo,1));
	
	//db_set_active('justmeans_prod');
		
	// We will get the 3blmedia id and create an array of inbound.
	/*
	$fmrViews = array (
								'pressrelease' => array(17),
							);
	*/
	
	// Get out the IP addr and date
	$strIpAddress = $otherInfo['ip_addr'];
	$strJustDate = $otherInfo['view_date'];
								
	// fmrViews is a 2-D array with the first dimension being associative with FMR types and the second
	// dimension being an array of FMR IDs (nids) for each FMR type.
	// There can be multiple media's for inbound and we will process each of them.
	if(count($fmrViews) && is_array($fmrViews))
	{
		foreach ($fmrViews as $strFMRType => $FMRids)
		{
			//watchdog('Analytics final view processing','FMR type:'.$strFMRType);
			if(count($FMRids) && is_array($FMRids))
			{
				foreach ($FMRids as $FMRid)
				{
					if((int)$FMRid > 0)
					{
						
						//watchdog('Analytics final view processing','FMR:'.$FMRid);
						
						//////////////////////////////////////////////////////////////////////////////////////////
						// for each FMR, we need to get the master ids, from the sync tables, for
						// both the FMR and Company.
						//////////////////////////////////////////////////////////////////////////////////////////
						$row = db_query("select m.id master_mediaid, c.id master_companyid 
										from node n join jm_3bl_media m on m.3bl_mediaid = n.nid
										join og_membership ogm on n.nid = ogm.etid 
										join og on og.gid = ogm.gid
										join jm_3bl_company c on c.3bl_companyid = og.etid
										where n.nid = $FMRid")->fetchAssoc();
						if (!empty($row['master_mediaid'])){
							$mediaid = $row['master_mediaid'];
						}
						if (!empty($row['master_companyid'])){
							$intCompanyId = $row['master_companyid'];
						}
						//watchdog('Analytics final view processing','Media ID:'.$mediaid.'; Comp ID: '.$intCompanyId);
						
						// Set $strMediaType  to the appropriate jm_mediawise_views column name, to be used in
						// the update/insert queries, below.
						switch ($strFMRType)
						{
							case "pressrelease":		
							case "press_release":	
							case "press":	
								/*							
								$intCompanyId = db_query("SELECT companyid FROM {jm_company_article} where jm3blmediaid='$intMediaId'")->fetchField();			
								*/		
								$strMediaType	= 'pressrelease';	// this is how it is stored in jm_..._views tables
								//$table = 'jm_company_article';
							break;
							case "article":
							case "3bl_article":
							case "summary":
								//////////////////////////
								// ****** ?? ******
								//////////////////////////
								$strMediaType	= '3bl_article';	
								//$table = 'jm_company_3blarticle';	
							break;
							case "3bl_newsletter":
								//////////////////////////
								// ****** ?? ******
								//////////////////////////
								$strMediaType	= '3bl_newsletter';		
								//$table = 'jm_company_3blnewsletter';	
							break;								
							case "video":					
							case "multimedia":
								$strMediaType	= 'multimedia';	
								//$table = 'jm_company_multimedia';	
							break;
							case "blog":
							case "blogs":
								$strMediaType	= 'blogs';		
								//$table = 'jm_company_externalblogs';										
							break;
							default:
								$mediaid = "";
								$strMediaType = "";								
								$intCompanyId = "";
							break;
						}
						
						// This is to insert count in views table as well as in the dummy views table, we took dummy table because it will have data for a month which 
						// will automatically gets deleted after that this will keep fast processing of data. We can grab all the historic data from main views table, 
						// also count gets increamented as media is viewed.
						
						if($strMediaType && (int)$mediaid > 0 && (int)$intCompanyId > 0)
						{
							$intTotalViews = 0;
							$intTotalViews = db_query("SELECT count(*) FROM {jm_mediawise_views}
														WHERE datevisited = '$strJustDate' 
														AND channel = '$strChannel' 
														AND companyid = '$intCompanyId'"
														)->fetchField();
							
							//watchdog('Analytics final view processing','Total views:'.$intTotalViews);
							
							// Mediawise views count gets increamented, also we keep track of total view count.
							if($intTotalViews == 0 )
							{
								if((int)$intCompanyId)
								{
									
									
									$isInserted = db_insert('jm_mediawise_views')
													  ->fields(array(
														  $strMediaType => 1,
														  "companyid" => $intCompanyId,
														  "totalcount" => 1,
														  "channel" => "$strChannel",
														  "datevisited" => $strJustDate
														))
													  ->execute();
													  
								
									
									$isInsertedDummy = db_insert('jm_mediawise_views_dummy')
													  ->fields(array(
														  $strMediaType => 1,
														  "companyid" => $intCompanyId,
														  "totalcount" => 1,
														  "channel" => "$strChannel",
														  "datevisited" => $strJustDate
														))
													  ->execute();
													  
								
									
								}
							}
							else
							{
								
								// if views data is already exists and count is > 0 then we are updating the view count by increament value by 1.
								
								 $strUpdate = db_query("UPDATE {jm_mediawise_views} 
														SET $strMediaType = $strMediaType + 1,
														totalcount = totalcount + 1
														WHERE channel = '$strChannel'
														AND datevisited = '$strJustDate'
														AND companyid = '$intCompanyId'");
								
								 $strUpdate = db_query("UPDATE {jm_mediawise_views_dummy}
														SET $strMediaType = $strMediaType + 1,
														totalcount = totalcount + 1
														WHERE channel = '$strChannel'
														AND datevisited = '$strJustDate'
														AND companyid = '$intCompanyId'");
							}
							
							
							// This individual views of the media's according to there id in the table. This will have a media id and it's view count.
							$intTotalViews = 0;
							$intTotalViews = $strGetMediaIdViews = db_query("SELECT totalcount FROM {jm_mediaidwise_views}
																			 WHERE datevisited = '$strJustDate'
																			 AND channel = '$strChannel'
																			 AND companyid = '$intCompanyId'
																			 AND mediaid = '$mediaid'"
																			 )->fetchField();
							if($intTotalViews == 0)
							{	
								$isInserted = db_insert('jm_mediaidwise_views')
													  ->fields(array(
													  "mediaid"   => $mediaid,
													  "mediatype" => "$strMediaType",
													  "companyid" => $intCompanyId,
													  "totalcount" => 1,
													  "uniquecount" => 1,
													  "channel" => "$strChannel",
													  "datevisited" => $strJustDate
													))
													  ->execute();
			
								
								$isInserted = db_insert('jm_mediaidwise_uniqueviews')
													  ->fields(array(
													  "mediaid"   => $mediaid,
													  "mediatype" => "$strMediaType",
													  "companyid" => $intCompanyId,
													  "iptrack" => "$strIpAddress",
													  "channel" => "$strChannel"
													))
													  ->execute();
							}
							else
							{	
								// We are updating tables if media is already viewed.
								 $strUpdate = db_query("UPDATE {jm_mediaidwise_views}
														SET totalcount = totalcount + 1
														WHERE channel = '$strChannel'
														AND datevisited = '$strJustDate'
														AND companyid = '$intCompanyId'
														AND mediaid = '$mediaid'
														AND mediatype = '$strMediaType'");
										
							}
			
							// This is to keep track of visiting ip addresses, To get the unique views of the media.
							$intUniqueViews = 0;
							$strGetUniqueViews = db_query("SELECT iptrack FROM {jm_mediaidwise_uniqueviews}
															WHERE channel = '$strChannel'
															AND companyid = '$intCompanyId'
															AND mediaid = '$mediaid'
															AND mediatype = '$strMediaType'
															AND iptrack = '$strIpAddress'"
															)->fetchField();
							if ($strGetUniqueViews) {
								$intUniqueViews = (int)$strGetUniqueViews;
							}
							if($intUniqueViews == 0)
							{
								$isInserted = db_insert('jm_mediaidwise_uniqueviews')
													  ->fields(array(
													  "mediaid"   => $mediaid,
													  "mediatype" => "$strMediaType",
													  "companyid" => $intCompanyId,
													  "iptrack" => "$strIpAddress",
													  "channel" => "$strChannel"
													))
													  ->execute();
								
								 $strUpdate = db_query("UPDATE {jm_mediaidwise_views}
														SET uniquecount = uniquecount + 1
														WHERE channel = '$strChannel'
														AND datevisited = '$strJustDate'
														AND companyid = '$intCompanyId'
														AND mediaid = '$mediaid'
														AND mediatype = '$strMediaType'");
							}
						}
					}
				}
			}
		}
	}
		
	//db_set_active();
}