<form action="" method="post" name="frmdate" id="frmdate">
<input type="hidden" name="olddate" id="olddate" value="<?php echo $strolddate; ?>" />
<input type="hidden" name="currentdate" id="currentdate" value="<?php echo $strcurrentdate; ?>" />
</form>

<script type="text/javascript">
/**
 * We use the initCallback callback
 * to assign functionality to the controls
 */
function mycarousel_initCallback(carousel) {
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


function mylink_initCallback(carousel) {
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
jQuery(document).ready(function() {
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
      function drawChart() {
	  var data = google.visualization.arrayToDataTable([
          ['',  'Clicks'],
		<?php $k=0;    if(!empty($arrMediaidChart))
		        {
				     foreach($arrMediaidChart as $strchart)
				   { 
					   ?>     
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
		pointSize: 7, hAxis: {showTextEvery: <?php if($k < 5) {echo $k; }else {echo round($k/5);} ?> }});
		
	 
      }
	  
	  
    </script>
    

    
    
  <script type='text/javascript'>
   google.load('visualization', '1', {'packages': ['geomap']});
   google.setOnLoadCallback(drawMap);

    function drawMap() {
      var data = google.visualization.arrayToDataTable([
        ['Country', 'Count'],
        <?php  $j=0; if(!empty($arrCountryName))
		  { 
		   foreach($arrCountryName as $strcountryname)
		   {?>
           ['<?php echo trim(addslashes($strcountryname->country))?>',<?php echo $strcountryname->ipcount;?>],	
		   
		 <?php $j++; } 
		 
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
        <div class="rightSideCont">
         <?php echo $strTabHtml;?>
            <?php if(!empty($arrMediaidCount))
			       {
					 foreach($arrMediaidCount as $strmedia)
					 {
						 $intTotalcount = $strmedia->totalcount;
						 $strMediaTitle = $strmedia->title;
						 $strClickdate  = $strmedia->click_date; 
						 $strfmrType =    $strmedia->fmr_type; 
					 }
				   }
				?>   
          
            <div class="detailsContainer">
            
              <div class="expTabsContainer">
              <div class="mediaTitleDate">
                <h2><?php echo $strMediaTitle;?></h2>
                <div class="clr"></div>
                <span style="font-size:10px;" class="txtGray">
				<?php echo date('d',strtotime($strClickdate));?><sup>th</sup><?php echo date('M Y',strtotime($strClickdate));?></span>
                </div>
              <div class="brdCont">
                <span style="display:none" class=""><a href="" class="">All</a></span>
                <span style="" class="frstLink"><a href="<?php echo url('Dashboard/Analytics/Clicks',array('absolute' => TRUE));?>" class="fistLink">Media Type</a></span>
                <span style="" class=""><a href="<?php if($strfmrType=="press_release"){$strfmrType='pressrelease';} echo  url('Dashboard/Analytics/Views/mediatype/',array('absolute' => TRUE)).$strfmrType; ?>" class="otherLink">
                 <?php switch($strfmrType)
						{
						   case 'article':	
						   $strtype = 'Press Release';
						   break;
						   case 'multimedia':	
						   $strtype = 'Multimedia';
						   break;
						   case 'blogs':	
						   $strtype = 'Blogs';
						   break;
						   case '3bl_newsletter':	
						   $strtype = 'Newsletter';
						   break;
						   case '3bl_article':	
						   $strtype = 'Article';
						   break;
						}
						echo $strtype;
                  ?>
                  
                </a></span>
                <span style="display:block" class="active"><a href="javascript:void(0);" class="otherLink"><?php echo $strtype;?> Analytics</a></span>
                </div>
              </div>	
            <ul class="analyticsTrigger">	
              <li>
                <div class="viewsBox" style="top:1px;left:200px;">
				<?php echo number_format((int)$intTotalcount);?></div>
                <h2 class="trigger"><a href="#">Total</a></h2>
                <div class="toggle_container">
                  <div class="calCont" style="top:1px;"> </div>
                  <?php if($k!=0){?>
                  <div id="chart_div">
                    <div style='text-align:center; padding: 40px 0  40px 0; clear:both;'> 
                    <img src='<?php echo url('sites/all/themes/threebl/images/justmeans/metrics-ajax-loader.gif',array('absolute' => TRUE));?>' align='absmiddle'> 
                    </div>
                  </div>
                  <?php }else{?>
                  <div style='text-align:center; padding: 40px 0  40px 0; clear:both;'> 
                  
                 <img src= '<?php echo url('sites/all/themes/threebl/images/justmeans/no_datafor_analytics.png',array('absolute' => TRUE));?>' align='absmiddle'> 
                 </div>
                  
                  <?php } ?>
                  
                </div>
                <div class="clr"></div>
              </li>
              <li> &nbsp; </li>
              <li>
               <h2 class="trigger"><a href="javascript:void(0);">Geographical Stats</a></h2>
                <div class="toggle_container" style="">
                <div class="ovfl-hidden">
                
                <?php if($j!=0) { ?>
                
                <div id="map_canvas" style="margin: 5px; position: relative;">
                <div style='text-align:center; padding: 40px 0  40px 0;'>
                <img src='<?php echo url('sites/all/themes/threebl/images/justmeans/metrics-ajax-loader.gif',array('absolute' => TRUE));?>' align='absmiddle'>
                </div>
                </div>
                
                 <?php }else{?>
                 
                 
                 <div style='text-align:center; padding: 40px 0  40px 0; clear:both;'> 
                 <img src= '<?php echo url('sites/all/themes/threebl/images/justmeans/no-map.gif',array('absolute' => TRUE));?>' align='absmiddle'> 
                 </div>
                
                <?php } ?>
               
                
                
                </div>
                
                 <?php  if(!empty($arrCountryName)){?>
                <div class="detailsTable" style="overflow:hidden;">
                <div class="socaialAccounts socaialAccounts2 fl">
                <div class="rightCont2" id="id_countryurl" style="height:350px!important; z-index:9999!important; width:100% !important; visibility:;">
                <div class="tableTh4">
                <h2>Country</h2>
                <h2>Clicks</h2>
                </div>
                <div class="tableTd4" id="mycarousel">
                <ul class="countryList">
                <li style="overflow:visible; width:662px !important; height:315px important">
                
		               <?php  $l=1; $ipcount = 0; $j=1; $intTotalCountry = count($arrCountryName);
		                 foreach($arrCountryName as $strcountryname)
						 { ?>
                                <div class=" listdiv <?php if($l%2 == 0) {?> rowBg <?php }?>">
                                <h2><?php echo trim($strcountryname->country);?></h2>
                                <h2><?php echo number_format($strcountryname->ipcount);?></h2>
                                </div>
                                
                             <?php if($l%10 == 0 && $l < $intTotalCountry) {?>
                             </li><li style="overflow:visible; width:662px !important; height:315px important"> 
                             <?php } ?>
                             
                        <?php $l++; }  ?>
                </li>
                </ul>
                <a id="mycarousel-prev" class="leftArrow"></a>
                <a id="mycarousel-next" class="rightArrow"></a>
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
              
               <li> &nbsp; </li>
               <li>
               <h2 class="trigger"><a href="javascript:void(0);">Referring Websites</a></h2>
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
                        <?php $rl=1; $intTotalLink = count($arrRefereLink);
		                 foreach($arrRefereLink as $strlink)
						 { ?>
                            <div class=" listdiv <?php if($rl%2 == 0) {?> rowBg <?php }?>">
                            <h2 style="width:200px;"><?php echo trim($strlink->rlink);?></h2>
                           
                            </div>
                             <?php if($rl%10 == 0 && $rl < $intTotalLink) {?>
                             </li><li style="overflow:visible; width:662px !important; height:315px important"> 
                             <?php } ?>
                             
                         <?php  $rl++; }  ?>
		           
                </li>
                 
                </ul>
                <a id="mylink-prev" class="leftArrow"></a>
                <a id="mylink-next" class="rightArrow"></a>
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

