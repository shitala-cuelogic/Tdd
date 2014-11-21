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
		<?php $intCnt=0;    if(!empty($arrMediatypeChart))
		        {
				   foreach ($arrDts as $key => $val) {
					   ?>     
					[' <?php echo date('M d',strtotime($val)); ?>',<?php echo (int)$arrClickChart[$val]; ?>],	
					
		  <?php  $intCnt++;  }
		   
		        } ?>
        ]);
        var options = {
          title: 'The decline of \'The 39 Steps\'',
          vAxis: {title: 'Accumulated Rating'},
          isStacked: true
        };
        var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
        chart.draw(data,  {colors: ['#3366CC'], width: 675, height: 200, title: '',curveType: 'none', legend: 'none', lineWidth: 3, 
		pointSize: 7, hAxis: {showTextEvery: <?php echo ($intCnt < 7)? 1 : 2; ?> }});
      }
    </script>
  <script type='text/javascript'>
   google.load('visualization', '1', {'packages': ['geomap']});
   google.setOnLoadCallback(drawMap);

    function drawMap()
    {
      var data = google.visualization.arrayToDataTable([
        ['Country', 'Count'],
         <?php  $intCountJ = 0; if(!empty($arrCountryName))
		  { 
		   foreach($arrCountryName as $strcountryname)
		   {?>
           ['<?php echo trim(addslashes($strcountryname->country))?>',<?php echo $strcountryname->ipcount;?>],	
		   
		 <?php  $intCountJ++;}
		 
		      } ?>	
      ]);

      var options = {showLegend:false,width:'670px', height:'380px',dataMode:'regions'};
      
      var container = document.getElementById('map_canvas');
      var geomap = new google.visualization.GeoMap(container);
      geomap.draw(data, options);
  };
  </script>

<style type="text/css">
    h3{ background: none; padding: 0px; font-size: 20px; }
    .viewBox2{top: 1px; left: 200px;}
    .calConttop{top: 1px;}
    .chart_divtext{text-align: center; padding: 40px 0 40px 0; clear: both;}
    .hieght42{height: 42px;}
    .h2anchor{width: 100%; display: block; margin-bottom: 5px;}
    .fmrspan{clear: both; color: #687275; font-size: 11px; padding-top: 5px;}
    .mapcanvaspos{margin: 5px; position: relative;}
    .mapcanvastxt{text-align: center; padding: 40px 0 40px 0;}
    .mapcanvasclr{text-align: center; padding: 40px 0 40px 0; clear: both;}
    .detailsTableoverflow{overflow: hidden;}
    .rightCont2visibility{height: 350px !important; z-index: 9999 !important; width: 100% !important; visibility:;}
    .overflowVisible{overflow: visible; width: 662px !important; height: 315px !important}
</style>

<div class="content">
  <div id='blMainWrapper'>
    <div id='blWrapper'>

      <div id="analyticsWrapper">
        <div class="rightSideCont">
          <?php echo $strTabHtml;?>
            <div class="detailsContainer">
              <div class="expTabsContainer">
              <div class="brdCont">
                <span style="display:none" class=""><a href="<?php echo url('Dashboard/Analytics/Clicks',array('absolute' => TRUE));?>" class="">Media Type</a></span>
                <span class="frstLink"><a href="<?php echo  url('Dashboard/Analytics/Clicks',array('absolute' => TRUE)); ?>" class="fistLink">Media Type</a></span>
                <span class="active"><a href="javascript:void(0);" class="otherLink">
                 <?php switch ($strmediatype) {
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
                <span style="display:none" class=""><a href="" class="otherLink"></a></span>
                </div>
              </div>	
            <ul class="analyticsTrigger">	
              <li>
                <div class="viewsBox viewBox2">
 	
                 <?php   $intTotalcount = 0;
			
				        if (!empty($arrMediatypeTotalCount)) {
					      foreach ($arrMediatypeTotalCount as $strmediacount) {
                           $intTotalcount+= $strmediacount->totalcount;
						 }
					   }
					   
					   ?>
				<?php echo number_format((int)$intTotalcount);?></div>
                <h2 class="trigger"><a href="#">Total number of Clicks </a></h2>
                <div class="toggle_container">
                  <div class="calCont calConttop"> </div>
                  
                  <?php if ($intCnt != 0) {?>
                  <div id="chart_div">
                    <div class="chart_divtext">
                    <img src="<?php echo url('sites/all/themes/threebl/images/justmeans/metrics-ajax-loader.gif',array('absolute' => TRUE));?>" align='absmiddle'> 
                    </div>
                  </div>
                  <?php } else { ?>
                  
                  <div class="chart_divtext">
                  <img src= "<?php echo url('sites/all/themes/threebl/images/justmeans/no_datafor_analytics.png',array('absolute' => TRUE));?>" align='absmiddle'> 
                  </div>

                  <?php } ?>
                  
                </div>
                <div class="clr"></div>
              </li>
              <li> &nbsp; </li>
              <li>
              <h2 class="trigger"> <a href="#">Analytics for FMR</a> </h2>
              <div class="toggle_container" style="display: block;">
              <p class="note1">( Select a media type to view analytics for just that type )</p>
              <div class="detailsTable">
                <div class="tableTh">
                <h2>Media Title </h2>
                <h3> Clicks </h3>
                </div>
                <div class="tableTd">
                <ul>
                <?php  $intCounter = 1;  if (!empty($arrMediatypeCount)) {
					     foreach ($arrMediatypeCount as $strmediacount) {?>
                            <li class="hieght42 <?php if ($intCounter % 2 == 0) {?> rowBg <?php }?>">
 <h2><a class="h2anchor" href="<?php echo url('Dashboard/Analytics/Clicks/mediaid/'.$strmediacount->nid,array('absolute' => TRUE) );?>">
                            <?php echo substr($strmediacount->title,0,55);if(strlen($strmediacount->title) > 55){echo "..."; }?></a>
                            <span class="fmrspan">
                            <?php echo date('d',strtotime($strmediacount->click_date));?><sup>th</sup>
                            <?php echo date('M Y',strtotime($strmediacount->click_date));?></span>
                            </h2>
                            <h3><?php echo number_format((int)$strmediacount->totalcount); ?></h3>
                            </li>
                            
                 <?php $intCounter++; } } ?>
                
                </ul>
                </div>
             <div class="clr"></div>
            </div>
              </div>
              <div class="clr"></div>
              </li>
              
               <li>
               <h2 class="trigger"><a href="javascript:void(0);">Geographical Stats</a></h2>
                <div class="toggle_container">
                <div class="ovfl-hidden">
                <?php if ($intCountJ != 0) {?>
                
                <div id="map_canvas" class="mapcanvaspos">
                    <div class="mapcanvastxt">
                    <img src="<?php echo url('sites/all/themes/threebl/images/justmeans/metrics-ajax-loader.gif',array('absolute' => TRUE));?>" align='absmiddle'>
                    </div>
                </div>
                
                <?php } else {?>
                
                <div class="mapcanvasclr">
                <img src= "<?php echo url('sites/all/themes/threebl/images/justmeans/no-map.gif',array('absolute' => TRUE));?>" align='absmiddle'> 
                </div>
                <?php } ?>
                
                </div>
                <?php  if (!empty($arrCountryName)) {?>
                
                <div class="detailsTable detailsTableoverflow">
                <div class="socaialAccounts socaialAccounts2 fl">
                <div class="rightCont2 rightCont2visibility" id="id_countryurl">
                <div class="tableTh4">
                <h2>Country</h2>
                <h2>Clicks</h2>
                </div>
                <div class="tableTd4" id="mycarousel">
                <ul class="countryList">
                <li class="overflowVisible">
                
		               <?php  $intCountL = 1; $ipcount = 0; $intCountJ = 1; $intTotalCountry = count($arrCountryName);
		                 foreach ($arrCountryName as $strcountryname) { ?>
                            <div class=" listdiv <?php if ($intCountL % 2 == 0) {?> rowBg <?php }?>">
                            <h2><?php echo trim($strcountryname->country);?></h2>
                            <h2><?php echo number_format($strcountryname->ipcount);?></h2>
                            </div>   
						 <?php if ($intCountL % 10 == 0 && $intCountL < $intTotalCountry) {?>
                         </li><li class="overflowVisible">
                         <?php } ?>
                             
                        <?php $intCountL++; }  ?>
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
                 </div>
                 

                <div class="clr"></div>
              </li>
              
              
               
               <li>
               <h2 class="trigger"><a href="javascript:void(0);">Referring Websites</a></h2>
                <div class="toggle_container">
                 <?php  if (!empty($arrRefereLink)) {?>
                <div class="detailsTable detailsTableoverflow">
                <div class="socaialAccounts socaialAccounts2 fl">
                <div class="rightCont2 rightCont2visibility" id="id_countryurl">
                <div class="tableTh4">
                <h2>Link</h2>
                </div>
                <div class="tableTd4" id="mylink">
                <ul class="countryList">
                <li class="overflowVisible">
                        <?php $intCountrl = 1; $intTotalLink = count($arrRefereLink);
		                 foreach ($arrRefereLink as $strlink) { ?>
                            <div class=" listdiv <?php if ($intCountrl % 2 == 0) {?> rowBg <?php }?>">
                            <h2><?php echo trim($strlink->rlink);?></h2>
                           
                            </div>
                             <?php if ($intCountrl % 10 == 0 && $intCountrl < $intTotalLink) {?>
                             </li><li class="overflowVisible">
                             <?php } ?>
                             
                         <?php $intCountrl++; }  ?>
		           
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