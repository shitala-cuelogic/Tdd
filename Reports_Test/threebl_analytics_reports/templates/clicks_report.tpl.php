<form action="" method="post" name="frmdate" id="frmdate">
  <input type="hidden" name="olddate" id="olddate" value="<?php echo $strolddate; ?>" />
  <input type="hidden" name="currentdate" id="currentdate" value="<?php echo $strcurrentdate; ?>" />
</form>
<script type="text/javascript">
/**
 * We use the initCallback callback
 * to assign functionality to the controls
 */
function mycarousel_initCallback(carousel)
{
    jQuery('.jcarousel-control a').bind('click', function() {
        carousel.scroll(jQuery.jcarousel.intval(jQuery(this).text()));
        return false;
    });

    jQuery('.jcarousel-scroll select').bind('change', function() {
        carousel.options.scroll = jQuery.jcarousel.intval(this.options[this.selectedIndex].value);
        return false;
    });

    jQuery('#mycarousel-next').bind('click', function() {
        carousel.next();
        return false;
    });

    jQuery('#mycarousel-prev').bind('click', function() {
        carousel.prev();
        return false;
    });
};

function mylink_initCallback(carousel)
{
    jQuery('.jcarousel-control a').bind('click', function() {
        carousel.scroll(jQuery.jcarousel.intval(jQuery(this).text()));
        return false;
    });

    jQuery('.jcarousel-scroll select').bind('change', function() {
        carousel.options.scroll = jQuery.jcarousel.intval(this.options[this.selectedIndex].value);
        return false;
    });

   
    jQuery('#mylink-next').bind('click', function() {
        carousel.next();
        return false;
    });

    jQuery('#mylink-prev').bind('click', function() {
        carousel.prev();
        return false;
    });
	
	  
};

// Ride the carousel...
jQuery(document).ready(function()
{
    jQuery("#mycarousel").jcarousel({
        scroll: 1,
        initCallback: mycarousel_initCallback,
        // This tells jCarousel NOT to autobuild prev/next buttons
        buttonNextHTML: null,
        buttonPrevHTML: null
    });
	
	jQuery("#mylink").jcarousel({
        scroll: 1,
        initCallback: mylink_initCallback,
        // This tells jCarousel NOT to autobuild prev/next buttons
        buttonNextHTML: null,
        buttonPrevHTML: null
    });
	
});

</script> 
<script type="text/javascript" src="https://www.google.com/jsapi"></script> 
<script type="text/javascript">
      google.load("visualization", "1", {packages:["corechart"]});
      google.setOnLoadCallback(drawChart);
      function drawChart()
      {
	  var data = google.visualization.arrayToDataTable([
          ['',  'Clicks'],
		<?php $k = 0;  if(!empty($arrcompanyChart))
		        {
				  foreach($arrcompanyChart as $strchart)
				   {?>     
					[' <?php echo date('M d',strtotime($strchart->click_date)); ?>',<?php echo $strchart->totalcount; ?>],	

		  <?php  $k++;  }
		   
		        } ?>
        ]);
        var options = {
          title: 'The decline of \'The 39 Steps\'',
          vAxis: {title: 'Accumulated Rating'},
          isStacked: true
        };
        var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
        chart.draw(data,  {colors: ['#3366CC'], width: 660, height: 200, title: '',curveType: 'none', legend: 'none', lineWidth: 3, 
		pointSize: 7, hAxis: {showTextEvery: <?php if($k <5) {echo $k;} else{echo round($k/5);} ?> }});
      }
    </script> 
<script type='text/javascript'>
   google.load('visualization', '1', {'packages': ['geomap']});
   google.setOnLoadCallback(drawMap);

    function drawMap()
    {
      var data = google.visualization.arrayToDataTable([
        ['Country', 'Count'],
		
        <?php  $j=0; if(!empty($arrCountryName))
		  { 
		   foreach($arrCountryName as $strcountryname)
		   {?>
           ['<?php echo ucfirst(strtolower(trim(addslashes($strcountryname->country))))?>',<?php echo $strcountryname->ipcount;?>],	
		   
		 <?php  $j++; } 
		 
		      } ?>	
      ]);
	  
	  

      var options = {showLegend:false,width:'670px', height:'380px',dataMode:'regions'};
      
      var container = document.getElementById('map_canvas');
      var geomap = new google.visualization.GeoMap(container);
      geomap.draw(data, options);
  };
  </script>
<div class="content">
  <div id='blMainWrapper'>
    <div id='blWrapper'>
      <style type="text/css">
       h3{ background:none; padding:0px; font-size:20px;}
      </style>
      <div id="analyticsWrapper">
        <div class="rightSideCont"> <?php echo $strTabHtml;?>
          <div class="detailsContainer">
            <?php 
			     //With Particualr comapany calcualtion variable 
				 
			   $strtitle = '';              $intarticlefmr = 0;               
			   $inttotalcount = 0;          $intblogfmr = 0; 
			   $inttotalarticle = 0;        $intnewsletterfmr = 0; 
			   $inttotalblogs = 0;          $intmultimediafmr = 0; 
			   $inttotalpress = 0;          $intpressfmr = 0; 
			   $inttotalnewsletter = 0; 
			   $inttotaltotalmedia = 0;
			    
			    //WithOut Particualr comapany calcualtion variable 
				 
				$inttotalWithoutcount = 0;				$intTotalarticlefmr = 0; 					 			               
				$inttotalWithoutarticle = 0;            $intTotalblogfmr = 0;                                        
				$inttotalWithoutblogs = 0;	            $intTotalnewsletterfmr = 0;
				$inttotalWithoutpress = 0;              $intTotalmultimediafmr = 0; 
				$inttotalWithoutnewsletter = 0;         $intTotalpressfmr = 0;
				$inttotaltotalWithoutmultimedia = 0;	$intTotalOtherFrm = 0;        
				
				 //array of object total Clicks of each media with Company
				 if (!empty($arrcompanyClick)) {
				     $arrmediacount =''; 

					 foreach($arrcompanyClick as $strclick)  {
					   $arrmediacount[$strclick->fmr_type]['clickcount'] = $strclick->clickcount;
					   $arrmediacount[$strclick->fmr_type]['fmrcount'] = $strclick->fmrcount;
  
					 }
					 
					 if (!empty($arrmediacount)) {
						   $inttotalarticle = $arrmediacount['3bl_article']['clickcount'];
						   $inttotalblogs = $arrmediacount['blogs']['clickcount'];
						   $inttotaltotalmedia = $arrmediacount['multimedia']['clickcount'];
						   $inttotalnewsletter = $arrmediacount['3bl_newsletter']['clickcount'];
						   $inttotalpress =      $arrmediacount['article']['clickcount'];  // press_release
						   
						   $inttotalcount = $inttotalarticle + $inttotalblogs + $inttotaltotalmedia + $inttotalnewsletter + $inttotalpress;      
						   
						   
						   $intarticlefmr = $arrmediacount['3bl_article']['fmrcount'];
						   $intblogfmr = $arrmediacount['blogs']['fmrcount'];
						   $intmultimediafmr = $arrmediacount['multimedia']['fmrcount'];
						   $intnewsletterfmr = $arrmediacount['3bl_newsletter']['fmrcount'];
						   $intpressfmr = $arrmediacount['article']['fmrcount']; // press_release  
						   
						   $intCompanyFmr  = $intarticlefmr + $intblogfmr + $intmultimediafmr + $intnewsletterfmr + $intpressfmr;
						   
						            
					  }
				 }

				 if(!empty($arrOtherCompanyClick))
				 {   
				      $arrOthermediacount =''; 
				 
					 foreach($arrOtherCompanyClick as $strotherclick)
					 { 
					   $arrOthermediacount[$strotherclick->fmr_type]['clickcount'] = $strotherclick->clickcount;
					   $arrOthermediacount[$strotherclick->fmr_type]['fmrcount'] = $strotherclick->fmrcount;
					 }
					 
					 if(!empty($arrOthermediacount))
					  {
						   $inttotalWithoutarticle    = $arrOthermediacount['3bl_article']['clickcount'];
						   $inttotalWithoutblogs      = $arrOthermediacount['blogs']['clickcount'];
						   $inttotaltotalWithoutmultimedia = $arrOthermediacount['multimedia']['clickcount'];
						   $inttotalWithoutnewsletter  = $arrOthermediacount['3bl_newsletter']['clickcount'];
						   $inttotalWithoutpress  = $arrOthermediacount['article']['clickcount'];
						   
						   $inttotalWithoutcount      =  $inttotalWithoutarticle + $inttotalWithoutblogs + 
						   $inttotaltotalWithoutmultimedia + $inttotalWithoutnewsletter + $inttotalWithoutpress;   
						   
					  }
					  
					   if(!empty($arrOtherCompanyFMRClick))
					    {
						   $intTotalarticlefmr = $arrOtherCompanyFMRClick['3bl_article'];
						   $intTotalblogfmr = $arrOtherCompanyFMRClick['blogs'];
						   $intTotalmultimediafmr = $arrOtherCompanyFMRClick['multimedia'];
						   $intTotalnewsletterfmr = $arrOtherCompanyFMRClick['3bl_newsletter'];      
						   $intTotalpressfmr    =   $arrOtherCompanyFMRClick['article'];
						   
						   $intTotalOtherFrm = 	$intTotalarticlefmr + $intTotalblogfmr + $intTotalmultimediafmr + $intTotalnewsletterfmr;	
     
					  }

				 }
				
				/**************************************************Avarage caluation***************************/
				
				//with comapny avarage calculation
				
				$intCompanyPressAvg = round($inttotalpress/$intpressfmr);
				$intCompanyMediaAvg = round($inttotaltotalmedia/$intmultimediafmr);
				$intCompanyBlogAvg = round($inttotalblogs/$intblogfmr);
				$intCompanyNewsAvg = round($inttotalnewsletter/$intnewsletterfmr);
				$intCompanyArticleAvg = round($inttotalarticle/$intarticlefmr);
				
				
				
				//without coampy calculation;
				
				$intOtherPressAvg = round($inttotalWithoutpress/$intTotalpressfmr);
				$intOtherMediaAvg = round($inttotaltotalWithoutmultimedia/$intTotalmultimediafmr);
				$intOtherBlogAvg  = round($inttotalWithoutblogs/$intTotalblogfmr);
				$intOtherNewsAvg  = round($inttotalWithoutnewsletter/$intTotalnewsletterfmr);
				$intOtherArticleAvg =round($inttotalWithoutarticle/$intTotalarticlefmr);
				
				
 				
?>
            <div class="expTabsContainer">
              <?php $strcompany =''; 
			       if(!empty($strCompanyName))
					{ 
						  $strcompany = $strCompanyName[0]->title;
					}
			   ?>
              <h2><strong>Clicks</strong> are the total number of times <strong><?php echo $strcompany; ?></strong> Company media  has been clicked.</h2>
            </div>
            <ul class="analyticsTrigger">
              <li>
                <div class="viewsBox" style="top:1px;left:200px;"><?php echo number_format($inttotalcount);?></div>
                <h2 class="trigger"><a href="#">Total number of Clicks </a></h2>
                <div class="toggle_container">
                  <div class="calCont" style="top:1px;"> </div>
                  <?php   if($k!= 0) {?>
                  <div id="chart_div">
                    <div style='text-align:center; padding: 40px 0  40px 0; clear:both;'> <img src='<?php echo url('sites/all/themes/threebl/images/justmeans/metrics-ajax-loader.gif',array('absolute' => TRUE));?>' align='absmiddle'> </div>
                  </div>
                  <?php } else {?>
                  <div style='text-align:center; padding: 40px 0  40px 0; clear:both;'> <img src= '<?php echo url('sites/all/themes/threebl/images/justmeans/no_datafor_analytics.png',array('absolute' => TRUE));?>' align='absmiddle'> </div>
                  <?php } ?>
                </div>
                <div class="clr"></div>
              </li>
              <li> &nbsp; </li>
              <li>
                <h2 class="trigger"> <a href="#">Click per media</a> </h2>
                <div class="toggle_container" style="display: block;">
                  <p class="note1">( Select a media type to view analytics for just that type )</p>
                  <div class="detailsTable">
                    <div class="tableTh">
                      <ul class="ul-tableTh">
                        <li><span class="tableTh-title">Media Type</span></li>
                        <li><span class="tableTh-title">#FMR</span></li>
                        <li><span class="tableTh-title">#Clicks </span></li>
                        <li><span class="tableTh-title">AVG # Of Clicks</span></li>
                        <li><span class="tableTh-title">AVG # Of Clicks All 3BL-Members</span></li>
                      </ul>
                    </div>
                    <div class="tableTd">
                      <ul>
                        <li class="rowBg">
                          <ul class="ul-tableTh">
                            <li><span class=""><a href="<?php if((int)$intpressfmr > 0) { echo url('Dashboard/Analytics/Clicks/mediatype/article',array('absolute' => TRUE));} else { echo "javascript:void(0)";}?>">Press Release</a>&nbsp;</span> </li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$intpressfmr); ?></span></li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$inttotalpress); ?> </span></li>
                            <li><span class="tableTh-value">
                              <?php if($intpressfmr > 0) {echo   number_format((int)$intCompanyPressAvg); }
							                                        else { echo "0"; } ?>
                              </span></li>
                            <li><span class="tableTh-value">
                              <?php  if($intTotalpressfmr > 0) { echo   number_format((int)$intOtherPressAvg); } 
							                                        else { echo "0"; }?>
                              </span></li>
                          </ul>
                        </li>
                        <li class="">
                          <ul class="ul-tableTh">
                            <li><span class=""> <a  href="<?php if((int)$intmultimediafmr > 0 ) { echo url('Dashboard/Analytics/Clicks/mediatype/multimedia', array('absolute' => TRUE));
							    } else {echo "javascript:void(0)";}?>">Multimedia</a>&nbsp;</span> </li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$intmultimediafmr); ?></span></li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$inttotaltotalmedia);?></span></li>
                            <li><span class="tableTh-value">
                              <?php  if($intmultimediafmr > 0) {echo   number_format((int)$intCompanyMediaAvg); }
							                                        else { echo "0";} ?>
                              </span></li>
                            <li><span class="tableTh-value">
                          <?php if($intTotalmultimediafmr) { echo  number_format((int)$intOtherMediaAvg); }
							                                       else {echo "0";}?>
                              </span> </li>
                          </ul>
                        </li>
                        <li class="rowBg">
                          <ul class="ul-tableTh">
                            <li><span class=""><a  href="<?php if((int)$intblogfmr > 0) { echo url('Dashboard/Analytics/Clicks/mediatype/blogs',array('absolute' => TRUE)); } else {echo "javascript:void(0)";}?>" >Blogs</a></span> </li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$intblogfmr); ?></span></li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$inttotalblogs);?></span></li>
                            <li><span class="tableTh-value">
                              <?php if($intblogfmr > 0) { echo number_format((int)$intCompanyBlogAvg);}else{ echo "0";}?>
                              </span></li>
                            <li><span class="tableTh-value">
                              <?php if($intTotalblogfmr > 0){echo  number_format((int)$intOtherBlogAvg);}else { echo "0";}	
															   ?>
                              </span></li>
                          </ul>
                        </li>
                        <li class="">
                          <ul class="ul-tableTh">
                            <li><span class=""> <a  href="<?php  if((int)$intnewsletterfmr > 0) { echo url('Dashboard/Analytics/Clicks/mediatype/3bl_newsletter',array('absolute' => TRUE)); }
					  else {echo "javascript:void(0)";}?>">Newsletter</a>&nbsp;</span> </li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$intnewsletterfmr); ?></span></li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$inttotalnewsletter); ?> </span></li>
                            <li><span class="tableTh-value">
                              <?php if($intnewsletterfmr > 0) {echo number_format((int)$intCompanyNewsAvg);}
															else {echo "0";}  
														?>
                              </span></li>
                            <li><span class="tableTh-value">
                              <?php if($intTotalnewsletterfmr > 0){echo  number_format((int)$intOtherNewsAvg);}
															else{echo "0";}
														 ?>
                              </span></li>
                          </ul>
                        </li>
                        <li class="rowBg">
                          <ul class="ul-tableTh">
                            <li><span class=""> <a href="<?php if((int)$intarticlefmr > 0 ) { echo url('Dashboard/Analytics/Clicks/mediatype/3bl_article',array('absolute' => TRUE));
					   } else { echo "javascript:void(0)";  } ?>">Article</a>&nbsp;</span> </li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$intarticlefmr); ?></span></li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$inttotalarticle);?></span></li>
                            <li><span class="tableTh-value">
                              <?php if($intarticlefmr > 0) {echo number_format((int)$intCompanyArticleAvg);}
															else{echo "0";} 
														?>
                              </span></li>
                            <li><span class="tableTh-value">
                              <?php  if($intTotalarticlefmr > 0) { echo number_format((int)$intOtherArticleAvg);}
															 else { echo "0";} 
													  ?>
                              </span></li>
                          </ul>
                          <div class="clr"></div>
                        </li>
                        <li class="">
                          <ul class="ul-tableTh">
                            <li><span class="">All</span></li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$intCompanyFmr); ?></span></li>
                            <li><span class="tableTh-value"><?php echo number_format((int)$inttotalcount); ?> </span></li>
                            <li><span class="tableTh-value"><?php echo number_format(($intCompanyArticleAvg + $intCompanyBlogAvg + $intCompanyNewsAvg + $intCompanyMediaAvg + $intCompanyPressAvg)); ?></span></li>
                            <li><span class="tableTh-value"> <?php echo number_format($intOtherArticleAvg + $intOtherBlogAvg + $intOtherNewsAvg + $intOtherMediaAvg + $intOtherPressAvg);  ?></span></li>
                          </ul>
                        </li>
                        <li>&nbsp;</li>
                      </ul>
                    </div>
                  </div>
                </div>
                <div class="clr"></div>
              </li>
              <li>
                <h2 class="trigger"><a href="javascript:void(0);">Geographical Stats</a></h2>
                <div class="toggle_container" style="">
                  <div class="ovfl-hidden">
                    <?php if($j==0) { ?>
                    <div style='text-align:center; padding: 40px 0  40px 0; clear:both;'> <img src= '<?php echo url('sites/all/themes/threebl/images/justmeans/no-map.gif',array('absolute' => TRUE));?>' align='absmiddle'> </div>
                    <?php } else {?>
                    <div id="map_canvas" style="margin: 5px; position: relative;">
                      <div style='text-align:center; padding: 40px 0  40px 0;'> <img src='<?php echo url('sites/all/themes/threebl/images/justmeans/metrics-ajax-loader.gif',array('absolute' => TRUE));?>' align='absmiddle'> </div>
                    </div>
                    <?php } ?>
                  </div>
                  <?php  if(!empty($arrCountryName)) {?>
                  <div class="detailsTable" style="overflow:hidden;">
                    <div class="socaialAccounts socaialAccounts2 fl">
                      <div class="rightCont2" id="id_countryurl" style="height:350px!important; z-index:9999!important; width:100% !important; visibility:;">
                        <div class="tableTh4">
                          <h2>Country</h2>
                          <h2>#Clicks</h2>
                        </div>
                        <div class="tableTd4" id="mycarousel">
                          <ul class="countryList">
                            <li style="overflow:visible; width:662px !important; height:315px important">
                              <?php  $l=1; $ipcount = 0; 
		                 foreach($arrCountryName as $strcountryname)
						 { 
							 ?>
                              <div class=" listdiv <?php if($l%2 == 0) {?> rowBg <?php }?>">
                                <h2><?php echo trim($strcountryname->country);?></h2>
                                <h2><?php echo number_format((int)$strcountryname->ipcount);?></h2>
                              </div>
                              <?php  if($l%10 == 0 ) {?>
                            </li>
                            <li style="overflow:visible; width:662px !important; height:315px important">
                              <?php } ?>
                              <?php $l++; }  ?>
                            </li>
                          </ul>
                          <a id="mycarousel-prev" class="leftArrow"></a> <a id="mycarousel-next" class="rightArrow"></a>
                          <div class="clr">&nbsp;</div>
                        </div>
                      </div>
                      <div class="clr"></div>
                    </div>
                    <div class="clr"></div>
                  </div>
                  <?php } ?>
                  <div class="clr"></div>
                </div>
                <div class="clr"></div>
              </li>
              <li> &nbsp;
                <div class="clr">&nbsp;</div>
              </li>
              <li>
                <h2 class="trigger"><a href="javascript:void(0);">Other Referrer Websites</a></h2>
                <div class="toggle_container" style="">
                  <?php  if(!empty($arrRefereLink)){?>
                  <div class="detailsTable" style="overflow:hidden;">
                    <div class="socaialAccounts socaialAccounts2 fl">
                      <div class="rightCont2" id="id_countryurl" style="height:350px!important; z-index:9999!important; width:100% !important; visibility:;">
                        <div class="tableTh4">
                          <h2>Link</h2>
                        </div>
                        <div class="tableTd4" id="mylink">
                          <ul class="countryList">
                            <li style="overflow:visible; width:662px !important; height:315px important">
                              <?php $rl=1; $intTotalLink = count($arrRefereLink); $intTotalCountry = count($arrCountryName);
		                 foreach($arrRefereLink as $strlink)
						 { ?>
                              <div class=" listdiv <?php if($rl%2 == 0 && $rl < $intTotalLink) {?> rowBg <?php }?>">
                                <h2><?php echo trim($strlink->rlink);?></h2>
                              </div>
                              <?php if($rl%10 == 0 && $l < $intTotalCountry ) {?>
                            </li>
                            <li style="overflow:visible; width:662px !important; height:315px important">
                              <?php } ?>
                              <?php $rl++; }  ?>
                            </li>
                          </ul>
                          <a id="mylink-prev" class="leftArrow"></a> <a id="mylink-next" class="rightArrow"></a>
                          <div class="clr">&nbsp;</div>
                        </div>
                      </div>
                      <div class="clr"></div>
                    </div>
                    <div class="clr"></div>
                  </div>
                  <?php } ?>
                  <div class="clr"></div>
                </div>
                <div class="clr"></div>
              </li>
            </ul>
          </div>
        </div>
      </div>
    </div>
  </div>
</div>
