<?php global $conf; ?>
<style type="text/css">
    h3 {
        background: none;
        padding: 0px;
        font-size: 20px;
    }
    .padtop{padding-top: 3px;}
</style>
<?php
// code fo click chart of second level.

// array_pop for getting last value of dates.
$arrDatePop = array_pop($arrDts);
$intCount = $intCounter = 0;
$arrDisplayDates = array('0' => 0, '1' => 4, '2' => 8);

if (!empty($arrClickChart)) {
    ?>
<script type="text/javascript">
    google.load("visualization", "1", {packages:["corechart"]});
    google.setOnLoadCallback(drawChart);
    function drawChart()
    {
        var data = google.visualization.arrayToDataTable([
            ['', 'Clicks'],
            <?php
            foreach ($arrDts as $key => $val) {
                ?>
                [' <?php echo $val; ?>',<?php echo (int)$arrClickChart[$val]; ?>],
                <?php $intCounter++;
            } ?>
            [' <?php echo $arrDatePop; ?>',<?php echo (int)$arrClickChart[$arrDatePop]; ?>]
        ]);
        var options = {
            title:'The decline of \'The 39 Steps\'',
            vAxis:{title:'Accumulated Rating'},
            isStacked:true
        };
        var chart = new google.visualization.LineChart(document.getElementById('click_chart_div'));
        chart.draw(data, {colors:['#3366CC'], width:675, height:200, title:'', curveType:'none', legend:'none', lineWidth:3,
            pointSize:7, hAxis:{showTextEvery:4 }});
    }
</script>

<?php
}
// code fo View chart of second level.
if (!empty($arrViewChart)) {
    ?>
<script type="text/javascript">
    google.load("visualization", "1", {packages:["corechart"]});
    google.setOnLoadCallback(drawChart);
    function drawChart()
    {
        var data = google.visualization.arrayToDataTable([
            ["", 'Views'],
            <?php
            foreach ($arrDts as $key => $val) {
                ?>
                [' <?php echo $val; ?>',<?php echo (int)$arrViewChart[$val]; ?>],
                <?php $intCount++;
            } ?>
            ["<?php echo $arrDatePop; ?>",<?php echo (int)$arrViewChart[$arrDatePop]; ?>]
        ]);
        var options = {
            title:'The decline of \'The 39 Steps\'',
            vAxis:{title:'Accumulated Rating'},
            isStacked:true
        };
        var chart = new google.visualization.LineChart(document.getElementById('view_chart_div'));
        chart.draw(data, {colors:['#3366CC'], width:675, height:200, title:'', curveType:'none', legend:'none', lineWidth:3,
            pointSize:7, hAxis:{showTextEvery:4 }});
    }
</script>

<?php
} //chcking Country map array
if (!empty($arrCountryName)) {
    // array_pop for getting last value of Country.
    $arrCountryListName = $arrCountryName;
    $arrCountryNamePop = array_pop($arrCountryName);
    ?>

<script type='text/javascript'>
    google.load('visualization', '1', {'packages':['geomap']});
    google.setOnLoadCallback(drawMap);
    function drawMap()
    {
        var data = google.visualization.arrayToDataTable([
            ['Country', 'Count'],
            <?php  $intValue = 0;
            foreach ($arrCountryName as $strcountryname) {
                ?>
                ['<?php echo trim(addslashes($strcountryname->country))?>',<?php echo $strcountryname->ipcount;?>],
                <?php $intValue++;
            } ?>
            ['<?php echo trim(addslashes($arrCountryNamePop->country))?>',<?php echo $arrCountryNamePop->ipcount;?>]

        ]);


        var options = {showLegend:false, width:'670px', height:'380px', dataMode:'regions'};
        var container = document.getElementById('map_canvas');
        var geomap = new google.visualization.GeoMap(container);
        geomap.draw(data, options);
    }
    ;
</script>
<?php } ?>


<div class="content">
<div id='blMainWrapper'>
<div id='blWrapper'>
<div id="analyticsWrapper" class="analyticsOverflow">
<div class="rightSideCont">
<?php echo $strTabHtml;?>
<div class="detailsContainer">
<div class="expTabsContainer">
    <div class="brdCont">
        <span style="display:none"><a
            href="<?php echo url('Dashboard/Analytics/Views', array('absolute' => TRUE));?>" class="">Media
            Type</a></span>
        <span class="frstLink"><a
            href="<?php echo  url('Dashboard/Analytics/Views', array('absolute' => TRUE)); ?>" class="fistLink">Media
            Type</a></span>
               <span class="active top1x"><a href="javascript:void(0);" class="otherLink">
                   <?php  echo $arrShowSecondLevelType[trim($strMediaType)];   ?>
               </a></span>
        <span style="display:none"><a href="" class="otherLink"></a></span>
    </div>
</div>

<?php
// function for checking user browser is IE8 or not.
list($strClickCss, $intClickFlag) = threebl_Check_user_browser();
?>
<ul class="analyticsTrigger">
<li class="clr"> &nbsp; </li>
<li id="viewsli">
    <div class="view_click" id="tcount1">
        <div class="togle" id="tgleDiv">&nbsp;</div>
        <div class="viewsnoBox"><?php echo number_format((int)$arrMediaCount["viewcount"]);?></div>
        <span class="total">Total</span>

        <div class="view-button view-buttn01 viewactive" id="viewtab"><a href="javascript:void(0);" onclick="javascript:showChart('views',<?php echo $intClickFlag ?>);">Views</a>
        </div>
        <div class="view-button btn_inactive" id="clicktab"><a href="javascript:void(0);" onclick="javascript:showChart('clicks',<?php echo $intClickFlag ?>);">Clicks</a>
        </div>
    </div>

    <div class="toggle_container_top"><p class="note1">This is the total number of views or clicks, as selected, for the
        selected media type for the prior six months. The chart presents the past 60 days for views or clicks, summed
        into weekly intervals.</p>
    </div>
    <div class="toggle_container">
        <div class="calCont top1x"></div>
        <?php if (!empty($arrViewChart)) {
        // checking if View Chart array is empty for not if not show graph.if yes then show no analytics image.
        ?>
        <div id="view_chart_div">
            <div class='viewchartdiv'>
                <img
                    src="<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/metrics-ajax-loader.gif', array('absolute' => TRUE));?>"
                    align='absmiddle'>
            </div>
        </div>
        <?php } else { ?>

        <div class='viewchartdiv'>
            <img
                src="<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/no_datafor_analytics.png', array('absolute' => TRUE));?>"
                align='absmiddle'>
        </div>

        <?php } ?>

    </div>
    <div class="clr"></div>
</li>

<li id="clicksli" style="<?php echo $strClickCss;?>">
    <div class="view_click" id="tcount2">
        <div class="togle" id="tgleDiv1">&nbsp;</div>
        <div class="viewsnoBox"><?php echo number_format((int)$arrMediaCount["clickcount"]);?></div>
        <span class="total">Total</span>

        <div class="view-button view-buttn01 btn_inactive" id="viewtab"><a href="javascript:void(0);"
                                                                           onclick="javascript:showChart('views',<?php echo $intClickFlag ?>);">Views</a>
        </div>
        <div class="view-button viewactive" id="clicktab"><a href="javascript:void(0);"
                                                             onclick="javascript:showChart('clicks',<?php echo $intClickFlag ?>);">Clicks</a>
        </div>
    </div>

    <div class="toggle_container_top"><p class="note1">This is the total number of views or clicks, as selected, for the
        selected media type for the prior six months. The chart presents the past 60 days for views or clicks.</p></div>
    <div class="toggle_container">
        <div class="calCont top1x"></div>
        <?php if (!empty($arrClickChart)) {
        // checking if Click Chart array is empty for not if not show graph.if yes then show no analytics image.
        ?>
        <div id="click_chart_div">
            <div class='viewchartdiv'>
                <img
                    src="<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/metrics-ajax-loader.gif', array('absolute' => TRUE));?>"
                    align='absmiddle'>
            </div>
        </div>
        <?php } else { ?>

        <div class='viewchartdiv'>
            <img
                src="<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/no_datafor_analytics.png', array('absolute' => TRUE));?>"
                align='absmiddle'>
        </div>

        <?php } ?>

    </div>
    <div class="clr"></div>
</li>
<li> &nbsp; </li>
<li>
    <h2 class="trigger clr"><a href="#">Analytics by FMR</a></h2>

    <div class="toggle_container" style="display: block;">
        <p class="note1">Select an FMR to view additional details. Hover over the information icon for an explanation of
            Benchmarks on this page.</p>
        <p class="note1"><b>Note:</b> FMRs that were uploaded as "Archive-only" are marked with "(A)" in this list.</p>

        <div class="detailsTable">
            <div class="tableTh tableThOverflow padtop">
                <div class="tablethdiv">
                    <span class="tablethspan">Title</span>
                    <div>
                        <div class="date">Date</div>
                        <div class="media">Media Type</div>
                        <div class="category">Category</div>
                    </div>
                </div>
                <h3 class="viewsclickswidth"> Views </h3>
                <h3 class="viewsclickswidth"> Clicks </h3>
                <div class="benchmarkdiv">
                    <span class="benchmark">
                        Benchmark
                        <img class="benchmark_info" src="<?php echo url($conf['IMAGES_PATH_3BL'].'/benchmark_info.png', array('absolute' => TRUE));?>">
                        <div style="display: none;" class="openme1">
                            <p class="alignleft">The Benchmarks for individual FMRs are the site-wide average activity (views and
                                clicks) for all FMRs within the same Media Type, the same Primary Category and using the
                                same timeframe in which the given FMR was distributed. (The date, media type and primary
                                category for a given FMR are all listed below the title in this list.)</p>
                            <p class="alignleft top5">Benchmarks for a given month are calculated 8 days after the end
                                of the month. Before then you will see a dash. Benchmarks before April 2014 were not
                                calculated and you will see "N/A".</p>
                            <div class="arwbx">
                                <div class="arwpoint">&nbsp;</div>
                            </div>
                        </div>
                    </span>
                    <div class="ovfl-hidden">
                        <span class="benchmarkview">Views</span>
                        <span class="benchmarkclick">Clicks</span>
                    </div>
                </div>
            </div>
            <div class="tableTd" id="mediaresult">

                <input id="mediatype" name="mediatype" value="<?php echo $strMediaType;?>" type="hidden">
                <input id="pagenumber" name="pagenumber" value="1" type="hidden"/>

            </div>
            <div class="clr"></div>
        </div>
    </div>
    <div class="clr"></div>
</li>

<li> &nbsp; </li>
<li id="geograph" style="display:none">
    <h2 class="trigger"><a href="javascript:void(0);">Geographical Stats</a></h2>

    <div class="toggle_container">
        <div class="ovfl-hidden">
            <?php if (!empty($arrCountryListName)) {
            // checking if Country Name  array is empty for not if not show Google map.if yes then show no no map image.
            ?>
            <div id="map_canvas" class="mapcanvasdiv">
                <div class='viewchartdiv'>
                    <img
                        src='<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/metrics-ajax-loader.gif', array('absolute' => TRUE));?>'
                        align='absmiddle'>
                </div>
            </div>
            <?php } else { ?>
            <div class='viewchartdiv'>
                <img
                    src='<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/no-map.gif', array('absolute' => TRUE));?>'
                    align='absmiddle'>
            </div>
            <?php } ?>

        </div>

        <?php  if (!empty($arrCountryListName)) {
        //checking CompanyName array exits or not. if yes show the country name with list count.
        ?>
        <div class="detailsTable overflowhdn">
            <div class="socaialAccounts socaialAccounts2 fl">
                <div class="rightCont2 rightCont2div" id="id_countryurl">
                    <div class="tableTh4">
                        <h2 class="countryLink">Country</h2>

                        <h2>Clicks</h2>
                    </div>
                    <div class="tableTd4" id="mycarousel">
                        <ul class="countryList">
                        <li class="overflowVis">

                            <?php  $intValue = 1;  $intOther = 0; $intTotalCountry = count($arrCountryName);

                            foreach ($arrCountryListName as $strCountryName) {
                                if (strlen($strCountryName->country) > 1) {
                                    ?>
                                    <div class=" listdiv <?php if ($intValue % 2 == 0) { ?> rowBg <?php }?>">
                                        <h2><?php echo trim(strtoupper($strCountryName->country));?></h2>

                                        <h2 class="graph-text"><?php echo number_format($strCountryName->ipcount);?></h2>
                                    </div>

                                    <?php if ($intValue % 10 == 0 && $intValue < $intTotalCountry) { ?>
                        </li><li class="overflowVis">
                        <?php $intValue = 0;
                                    } ?>
                                    <?php  $intValue++;
                                } else {
                                    $intOther = $intOther + $strCountryName->ipcount;
                                }
                            }//END FOREACH
                            // Country name is blank or '-'

                            if ($intOther > 0) {
                                ?>
                                <div class=" listdiv <?php if ($intValue % 2 == 0) { ?> rowBg <?php }?>">
                                    <h2> OTHER/UNIDENTIFIED</h2>

                                    <h2 class="graph-text"><?php echo number_format($intOther);?></h2></div>
                                <?php } ?>
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
<li id="referlink" style="display:none">
    <h2 class="trigger"><a href="javascript:void(0);">Top Referring Websites</a></h2>

    <div class="toggle_container">
        <?php  if (!empty($arrRefereLink)) {
        //checking Referring website array exits or not. if yes show the Website list.
        ?>
        <div class="detailsTable overflowhdn">
            <div class="socaialAccounts socaialAccounts2 fl">
                <div class="rightCont2 rightCont2div" id="id_countryurl">
                    <div class="tableTh4"><h2 class="countryLink">Link</h2></div>
                    <div class="tableTd4" id="mylink">
                        <ul class="countryList">
                        <li class="overflowVis">
                            <?php $intRow = 1; $arrParseLink = '';
                            foreach ($arrRefereLink as $strLink) {
                                // getting only domain name form website name.
                                $arrParse = parse_url(trim($strLink->rlink));
                                $strParseLink = trim($arrParse['host'] ? $arrParse['host'] : array_shift(explode('/', $arrParse['path'], 2))); // parse link

                                $arrExplodeLink = explode(".", $strParseLink);
                                $intParseLink = count($arrExplodeLink);

                                $boolIPValid = preg_match('/^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\z/', $strParseLink);

                                if ($intParseLink == 1 || $intParseLink == 0 || $boolIPValid) {
                                    continue;
                                }

                                $arrParseLink[] = $strParseLink;
                            }//foreach

                            //make unique array for website name.
                            $arrParseLink = array_unique($arrParseLink);
                            $intTotalLink = count($arrParseLink);
                            foreach ($arrParseLink as $strRLink) {
                                ?>
                                <div class=" listdiv <?php if ($intRow % 2 == 0) { ?> rowBg <?php }?>">
                                    <h2 class="width235"><?php echo trim($strRLink);?></h2>
                                </div>
                                <?php if ($intRow % 10 == 0 && $intRow < $intTotalLink) { ?>
                                </li><li class="overflowVis">
                                <?php } ?>
                                <?php $intRow++;
                            }?>
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
<script type="text/javascript">
    displayMediaInfoAjax('', 1);
</script>