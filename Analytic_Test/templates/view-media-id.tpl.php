<?php global $conf;?>
<style type="text/css">
    h3{background: none;padding: 0px;font-size: 20px;}
    .detailsTable .socaialAccounts .rightCont2 .tableTd4 ul.countryList li div.listdiv h2.width235{width: 400px !important;}
</style>
<?php // here we get Click  and View count of selected media id with last click or view date and media type.

$intClickcount = 0;
$intViewcount = 0;
global $arrNewMediaType;
if (!empty($arrClickCount)) {
    $intClickcount = $arrClickCount['totalcount'];
}
if(!empty($arrViewCount)){
    $intViewcount = $arrViewCount['totalcount'];
}
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
            <?php  $intCounter = 0;

            foreach ($arrCountryName as $strcountryname) {
                ?>
                ['<?php echo trim(addslashes($strcountryname->country))?>',<?php echo $strcountryname->ipcount;?>],

                <?php $intCounter++;
            }//end of for-each ?>

            ///show  last record without "," for IE issue
            ['<?php echo trim(addslashes($arrCountryNamePop->country))?>',<?php echo $arrCountryNamePop->ipcount;?>]

        ]);

        var options = {showLegend:false, width:'670px', height:'380px', dataMode:'regions'};

        var container = document.getElementById('map_canvas');
        var geomap = new google.visualization.GeoMap(container);
        geomap.draw(data, options);
    };
</script>
<?php } ?>

<div class="content">
<div id='blMainWrapper'>
    <div id='blWrapper'>
        <div id="analyticsWrapper">
            <div class="rightSideCont">
                <?php echo $strTabHtml;?>
                <div class="detailsContainer">
                    <div class="expTabsContainer">
                        <div class="mediaTitleDate">
                            <h2><b><?php echo $strMediaTitle;?></b></h2>

                            <!-- Get FMR type  -->
                            <?php $strMediatype = trim($strFmrType);
                            $strNodeMediatype = $strMediatype;
                            $strMedia = $arrShowMediaType[$strMediatype];
                            ?>

                            <div class="clr"></div>
				            <span class="DateText media_category_date">
                                <span class="paddingright"><?php echo date('M d', strtotime($strPublishDate));?><?php echo date(', Y', strtotime($strPublishDate)) . $strIsArchive;?></span>
                                <span class="paddingright"><?php echo $strMedia; ?></span>
                                <span><?php echo $strPrimaryCategoryName; ?></span>
                            </span>
                        </div>

                        <div class="brdCont">
                            <span style="display:none"><a href="">All</a></span>
                            <span class="frstLink"><a
                                href="<?php echo url('Dashboard/Analytics/Views', array('absolute' => TRUE));?>"
                                class="fistLink">Media Type</a></span>
                        <span><a href="<?php  echo url('Dashboard/Analytics/Views/mediatype/', array('absolute' => TRUE)) . $strNodeMediatype;?>" class="otherLink">
                         <?php echo $strMedia; ?></a></span>
                            <span class="active" style="display: block"><a href="javascript:void(0);" class="otherLink"><?php echo $strMedia;?> Analytics</a></span>
                        </div>
                    </div>

                    <ul class="analyticsTrigger">
                        <li class="clr"> &nbsp; </li>
                        <li>
                            <h2 class="trigger"><a href="#">Analytics for FMR</a></h2>

                            <div class="toggle_container" style="display: block">
                                <div class="detailsTable">
                                    <div class="tableTh tableThOverflow">
                                        <div class="title paddingtop"> Views</div>
                                        <div class="title paddingtop"> Clicks</div>
                                        <div class="benchmarkfmrdiv">
                                            <span class="benchmarkfmr">
                                                Benchmark
                                                <img class="benchmark_info" src="<?php echo url($conf['IMAGES_PATH_3BL'] . '/benchmark_info.png', array('absolute' => TRUE));?>">
                                                <div style="display: none;" class="openme1">
                                                    <p class="alignleft">The Benchmarks for individual FMRs are the
                                                        site-wide average activity (views and clicks) for all FMRs within the same
                                                        Media Type, the same Primary Category and using the same
                                                        timeframe in which the given FMR was distributed. (The date,
                                                        media type and primary category for a given FMR are all listed
                                                        below the title, above.)</p>
                                                    <p class="alignleft top5">Benchmarks for a given month are
                                                        calculated 8 days after the end of the month. Before then you
                                                        will see a dash. Benchmarks before April 2014 were not
                                                        calculated and you will see "N/A".</p>
                                                    <div class="arwbx">
                                                        <div class="arwpoint">&nbsp;</div>
                                                    </div>
                                                </div>
                                            </span>
                                            <div class="ovfl-hidden">
                                                <span class="benchmarkfmrview">Views</span>
                                                <span class="benchmarkfmrclick">Clicks</span>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="clr"></div>
                                    <div class="tableTd">
                                        <ul>
                                            <li class="height42">
                                                <div
                                                    class="data noText"><?php echo number_format((int)$intViewcount); ?></div>
                                                <div
                                                    class="data noText"><?php echo number_format((int)$intClickcount); ?></div>
                                                <div
                                                    class="data noText"><?php echo $intBenchmarkView; ?>
                                                </div>
                                                <div
                                                    class="data noText"><?php echo $intBenchmarkClick; ?>
                                                </div>
                                            </li>
                                        </ul>
                                    </div>
                                    <div class="clr"></div>
                                </div>
                            </div>
                            <div class="clr"></div>
                        </li>

                        <li> &nbsp; </li>

                        <li>
                            <h2 class="trigger"><a href="javascript:void(0);">Geographical Stats</a></h2>

                            <div class="toggle_container">
                                <div class="ovfl-hidden">
                                    <?php if (!empty($arrCountryListName)) {
                                    // checking if Country Name  array is empty for not if not show Google map.if yes then show  no map image.
                                    ?>
                                    <div id="map_canvas" class="map_canvas_div">
                                        <div class='txtalgincenter'>
                                            <img
                                                src='<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/metrics-ajax-loader.gif', array('absolute' => TRUE));?>'
                                                align='absmiddle'>
                                        </div>
                                    </div>
                                    <?php } else { ?>

                                    <div class='txtalgincenter1'>
                                        <img src='<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/no-map.gif', array('absolute' => TRUE));?>' align='absmiddle'>
                                    </div>
                                    <?php } ?>
                                </div>

                                <?php // country map and count list
                                if (!empty($arrCountryListName)) {
                                    //checking array exits or not
                                    ?>
                                    <div class="detailsTable" class="detailteableoverflowidden">
                                        <div class="socaialAccounts socaialAccounts2 fl">
                                            <div class="rightCont2 rightCont2div" id="id_countryurl">
                                                <div class="tableTh4">
                                                    <h2 class="countryLink">Country</h2>

                                                    <h2>Clicks</h2>
                                                </div>
                                                <div class="tableTd4" id="mycarousel">
                                                    <ul class="countryList">
                                                        <li class="overflow">

                                                        <?php  $intRows = 1;  $intOther = 0; $intTotalCountry = count($arrCountryName);
                                                        foreach ($arrCountryListName as $strCountryName) {
                                                            if (strlen($strCountryName->country) > 1) {
                                                                ?>

                                                                <div
                                                                    class=" listdiv <?php if ($intRows % 2 == 0) { ?> rowBg <?php }?>">
                                                                    <h2><?php echo trim(strtoupper($strCountryName->country));?></h2>

                                                                    <h2 class="graph-text"><?php echo number_format($strCountryName->ipcount);?></h2>
                                                                </div>

                                                                <?php if ($intRows % 10 == 0 && $intRows < $intTotalCountry) { ?>
                                                        </li>
			                                            <li class="overflow">
                                                        <?php $intRows = 0;
                                                                } ?>
                                                                <?php  $intRows++;
                                                            } else {
                                                                $intOther = $intOther + $strCountryName->ipcount;
                                                            }
                                                        }//END FOREACH
                                                        // Country name is blank or '-'
                                                        if ($intOther > 0) {
                                                            ?>
                                                            <div
                                                                class=" listdiv <?php if ($intRows % 2 == 0) { ?> rowBg <?php }?>">
                                                                <h2> OTHER/UNIDENTIFIED </h2>

                                                                <h2 class="graph-text"><?php echo number_format($intOther);?></h2>
                                                            </div>
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
                        <li>
                            <h2 class="trigger"><a href="javascript:void(0);">Top Referring Websites</a></h2>

                            <div class="toggle_container">
                                <?php  if (!empty($arrRefereLink)) {
                                //checking Referring website array exits or not. if yes show the Website list.
                                ?>
                                <div class="detailsTable detailteableoverflowidden">
                                    <div class="socaialAccounts socaialAccounts2 fl">
                                        <div class="rightCont2 rightCont2div" id="id_countryurl">
                                            <div class="tableTh4">
                                                <h2 class="countryLink">Link</h2>
                                            </div>
                                            <div class="tableTd4" id="mylink">
                                                <ul class="countryList">
                                                <li class="countryListli">
                                                    <?php
                                                    $intRow  = 1;
                                                    $arrParseLink = '';

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
                             </li><li class="countryListli">
                             <?php } ?>

                                                        <?php $intRow++;
                                                    }  ?>

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