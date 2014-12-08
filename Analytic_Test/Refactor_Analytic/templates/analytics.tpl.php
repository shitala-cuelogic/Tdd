<?php
global $conf;
$intVCounter = $intKCounter = 0;
// array_pop for getting last value of dates.
$arrDatePop = array_pop($arrDts);
if (!empty($arrViewChart)) {
//view chart
    ?>
<script type="text/javascript">
      google.load("visualization", "1", {packages:["corechart"]});
google.setOnLoadCallback(drawChart);
function drawChart()
{
    var data = google.visualization.arrayToDataTable([
        ['',  'Views'],
        <?php
        foreach($arrDts as $key => $val){?>
            [' <?php echo $val; ?>',<?php echo (int)$arrViewChart[$val]; ?>],
        <?php  $intVCounter++;
        } ?>
        [' <?php echo $arrDatePop; ?>',<?php echo (int)$arrViewChart[$arrDatePop]; ?>]
    ]);
    var options = {
        title: 'The decline of \'The 39 Steps\'',
        vAxis: {title: 'Accumulated Rating'},
        isStacked: true
    };
    var chart = new google.visualization.LineChart(document.getElementById('view_chart_div'));
    chart.draw(data,  {colors: ['#3366CC'], width: 675, height: 200, title: '',curveType: 'none', legend: 'none', lineWidth: 3,
        pointSize: 7, hAxis: {showTextEvery: 4 }});
}

    <?php }
//check value exits or not for click chart
if (!empty($arrClickChart)) { ?>
google.load("visualization", "1", {packages:["corechart"]});
google.setOnLoadCallback(ClickChart);
function ClickChart()
{
    var data = google.visualization.arrayToDataTable([
        ['',  'Clicks'],
        <?php
        foreach($arrDts as $key => $val){?>
            [' <?php echo $val; ?>',<?php echo (int)$arrClickChart[$val]; ?>],
        <?php $intKCounter++;
        }//foreach end ?>
        ['<?php echo $arrDatePop; ?>',<?php echo (int)$arrClickChart[$arrDatePop]; ?>]
    ]);
    var options = {
        title: 'The decline of \'The 39 Steps\'',
        vAxis: {title: 'Accumulated Rating'},
        isStacked: true
    };
    var chart = new google.visualization.LineChart(document.getElementById('click_chart_div'));
    chart.draw(data,  {colors: ['#3366CC'], width: 675, height: 200, title: '',curveType: 'none', legend: 'none', lineWidth: 3,
        pointSize: 7, hAxis: {showTextEvery: 4 }});
}
    </script>
<?php } ?>

<div class="content">
<div id='blMainWrapper'>
<div id='blWrapper'>
<style type="text/css">
    h3{ background:none; padding:0px; font-size:20px;}
    .viewchartdiv{text-align: center;padding: 40px 0 40px 0;clear: both;}
    .displayblock{display: block;}
    .noText{ width: 70px !important; margin-top: 2px !important; }
    .padtop{ padding-top: 3px; }
</style>
<div id="analyticsWrapper" class="analyticsOverflow">
<div class="rightSideCont">
<?php // main tab variable
echo $strTabHtml;?>
<div class="detailsContainer">
<div class="expTabsContainer">
    <div class="brdCont"> <span class="frstLink"><a href="<?php echo  url('Dashboard/Analytics/Views',array('absolute' => TRUE)); ?>" class="fistLink">CONTENT SUMMARY</a></span> </div>
</div>
<?php // define all the views count related Variable
$intArticleFmr = 0;
$intTotalCount = 0;          $intBlogFmr = 0;
$intTotalArticle = 0;        $intNewsletterFmr = 0;
$intTotalBlogs = 0;          $intMultimediaFmr = 0;
$intTotalPressRelease = 0;   $intPressReleaseFmr = 0;
$intTotalNewsletter = 0;
$intTotalMultiMedia = 0;

// getting the views count by company id for each media type
if (!empty($arrMediaTypeCount)) {
    $intTotalArticle = @$arrMediaTypeCount['article']['viewcount'];
    $intTotalBlogs = @$arrMediaTypeCount['blog']['viewcount'];
    $intTotalPressRelease = @$arrMediaTypeCount['press_release']['viewcount'] ;
    $intTotalNewsletter = @$arrMediaTypeCount['newsletter']['viewcount'];
    $intTotalMultiMedia = @$arrMediaTypeCount['multimedia']['viewcount'];

    // With videos
    $intTotalArticleWithVideos = @$arrMediaTypeCount['article']["withvideo"]['viewcount'];
    $intTotalBlogsWithVideos = @$arrMediaTypeCount['blog']["withvideo"]['viewcount'];
    $intTotalPressreleaseWithVideos = @$arrMediaTypeCount['press_release']["withvideo"]['viewcount'] ;
    $intTotalNewsletterWithVideos = @$arrMediaTypeCount['newsletter']["withvideo"]['viewcount'];
    $intTotalMultiMediaWithVideos = @$arrMediaTypeCount['multimedia']["withvideo"]['viewcount'];

    // Without videos
    $intTotalArticleWithoutVideos = @$arrMediaTypeCount['article']["withoutvideo"]['viewcount'];
    $intTotalBlogsWithoutVideos = @$arrMediaTypeCount['blog']["withoutvideo"]['viewcount'];
    $intTotalPressreleaseWithoutVideos = @$arrMediaTypeCount['press_release']["withoutvideo"]['viewcount'] ;
    $intTotalNewsletterWithoutVideos = @$arrMediaTypeCount['newsletter']["withoutvideo"]['viewcount'];
    $intTotalMultiMediaWithoutVideos = @$arrMediaTypeCount['multimedia']["withoutvideo"]['viewcount'];

    // sum of all views count of media type
    $intTotalCount = $intTotalArticle + $intTotalBlogs + $intTotalPressRelease + $intTotalNewsletter + $intTotalMultiMedia;

    // count of FMR with videos of views with company id
    $intArticleFmrWithVideos = @$arrMediaTypeCount['article']["withvideo"]['fmrcount'];;
    $intBlogFmrWithVideos = @$arrMediaTypeCount['blog']["withvideo"]['fmrcount'];
    $intNewsletterFmrWithVideos = @$arrMediaTypeCount['newsletter']["withvideo"]['fmrcount'];
    $intMultimediaFmrWithVideos = @$arrMediaTypeCount['multimedia']["withvideo"]['fmrcount'];
    $intPressreleaseFmrWithVideos = @$arrMediaTypeCount['press_release']["withvideo"]['fmrcount'];

    // count of FMR with videos of views with company id
    $intArticleFmrWithoutVideos = @$arrMediaTypeCount['article']["withoutvideo"]['fmrcount'];;
    $intBlogFmrWithoutVideos = @$arrMediaTypeCount['blog']["withoutvideo"]['fmrcount'];
    $intNewsletterFmrWithoutVideos = @$arrMediaTypeCount['newsletter']["withoutvideo"]['fmrcount'];
    $intMultimediaFmrWithoutVideos = @$arrMediaTypeCount['multimedia']["withoutvideo"]['fmrcount'];
    $intPressreleaseFmrWithoutVideos = @$arrMediaTypeCount['press_release']["withoutvideo"]['fmrcount'];

    // count of FMR  of views with company id
    $intArticleFmr = @$arrMediaTypeCount['article']['fmrcount'];
    $intBlogFmr = @$arrMediaTypeCount['blog']['fmrcount'];
    $intNewsletterFmr = @$arrMediaTypeCount['newsletter']['fmrcount'];
    $intMultimediaFmr = @$arrMediaTypeCount['multimedia']['fmrcount'];
    $intPressReleaseFmr = @$arrMediaTypeCount['press_release']['fmrcount'];

    //Sum of All Total media-type FMR
    $intTotalCompanyFMR =  $intArticleFmr +  $intBlogFmr +  $intNewsletterFmr + $intMultimediaFmr +  $intPressReleaseFmr;

    //array of object total Clicks of each media with Company
    $intClickTotalArticle = @$arrMediaTypeCount['article']['clickcount'];
    $intClickTotalBlogs = @$arrMediaTypeCount['blog']['clickcount'];
    $intClickTotalMedia = @$arrMediaTypeCount['multimedia']['clickcount'];
    $intClickTotalNewsletter = @$arrMediaTypeCount['newsletter']['clickcount'];
    $intClickTotalPress =      @$arrMediaTypeCount['press_release']['clickcount'];
    // press_release

    //Sum of all Total of click-count of each media-type.
    $intClickTotalCount = (int)$intClickTotalArticle + (int)$intClickTotalBlogs + (int)$intClickTotalMedia + (int)$intClickTotalNewsletter
        + (int)$intClickTotalPress;

    $intClickTotalArticleWithoutVideos = @$arrMediaTypeCount['article']["withoutvideo"]['clickcount'];
    $intClickTotalBlogsWithoutVideos = @$arrMediaTypeCount['blog']["withoutvideo"]['clickcount'];
    $intClickTotalMediaWithoutVideos = @$arrMediaTypeCount['multimedia']["withoutvideo"]['clickcount'];
    $intClickTotalNewsletterWithoutVideos = @$arrMediaTypeCount['newsletter']["withoutvideo"]['clickcount'];
    $intClickTotalPressWithoutVideos =      @$arrMediaTypeCount['press_release']["withoutvideo"]['clickcount'];

    $intClickTotalArticleWithVideos = @$arrMediaTypeCount['article']["withvideo"]['clickcount'];
    $intClickTotalBlogsWithVideos =   @$arrMediaTypeCount['blog']["withvideo"]['clickcount'];
    $intClickTotalMediaWithVideos =   @$arrMediaTypeCount['multimedia']["withvideo"]['clickcount'];
    $intClickTotalNewsletterWithVideos =  @$arrMediaTypeCount['newsletter']["withvideo"]['clickcount'];
    $intClickTotalPressWithVideos =       @$arrMediaTypeCount['press_release']["withvideo"]['clickcount'];

}//if-arrMediaTypeViewsCount

//Benchmark Views By Media Type
$intBenchmarkPressViews = @$arrBenchmarkViewsFMRInfo["press_release"]["benchmark_views"];
$intBenchmarkArticleViews = @$arrBenchmarkViewsFMRInfo["article"]["benchmark_views"];
$intBenchmarkBlogViews = @$arrBenchmarkViewsFMRInfo["blog"]["benchmark_views"];
$intBenchmarkMultimediaViews = @$arrBenchmarkViewsFMRInfo["multimedia"]["benchmark_views"];
$intBenchmarkNewsletterViews = @$arrBenchmarkViewsFMRInfo["newsletter"]["benchmark_views"];

//Benchmark Views with video By Media Type
$intBenchmarkPressViewsWithVideo = @$arrBenchmarkViewsFMRInfo["press_release"]["benchmark_views_withvideo"];
$intBenchmarkArticleViewsWithVideo = @$arrBenchmarkViewsFMRInfo["article"]["benchmark_views_withvideo"];
$intBenchmarkBlogViewsWithVideo = @$arrBenchmarkViewsFMRInfo["blog"]["benchmark_views_withvideo"];
$intBenchmarkMultimediaViewsWithVideo = @$arrBenchmarkViewsFMRInfo["multimedia"]["benchmark_views_withvideo"];
$intBenchmarkNewsletterViewsWithVideo = @$arrBenchmarkViewsFMRInfo["newsletter"]["benchmark_views_withvideo"];

//Benchmark Views without video By Media Type
$intBenchmarkPressViewsWithoutVideo = @$arrBenchmarkViewsFMRInfo["press_release"]["benchmark_views_withoutvideo"];
$intBenchmarkArticleViewsWithoutVideo = @$arrBenchmarkViewsFMRInfo["article"]["benchmark_views_withoutvideo"];
$intBenchmarkBlogViewsWithoutVideo = @$arrBenchmarkViewsFMRInfo["blog"]["benchmark_views_withoutvideo"];
$intBenchmarkMultimediaViewsWithoutVideo = @$arrBenchmarkViewsFMRInfo["multimedia"]["benchmark_views_withoutvideo"];
$intBenchmarkNewsletterViewsWithoutVideo = @$arrBenchmarkViewsFMRInfo["newsletter"]["benchmark_views_withoutvideo"];

//Benchmark Clicks By Media Type
$intBenchmarkPressClicks = @$arrBenchmarkClicksFMRInfo["press_release"]["benchmark_clicks"];
$intBenchmarkArticleClicks = @$arrBenchmarkClicksFMRInfo["article"]["benchmark_clicks"];
$intBenchmarkBlogClicks = @$arrBenchmarkClicksFMRInfo["blog"]["benchmark_clicks"];
$intBenchmarkMultimediaClicks = @$arrBenchmarkClicksFMRInfo["multimedia"]["benchmark_clicks"];
$intBenchmarkNewsletterClicks = @$arrBenchmarkClicksFMRInfo["newsletter"]["benchmark_clicks"];

//Benchmark Clicks with video By Media Type
$intBenchmarkPressClicksWithVideo = @$arrBenchmarkClicksFMRInfo["press_release"]["benchmark_clicks_withvideo"];
$intBenchmarkArticleClicksWithVideo = @$arrBenchmarkClicksFMRInfo["article"]["benchmark_clicks_withvideo"];
$intBenchmarkBlogClicksWithVideo = @$arrBenchmarkClicksFMRInfo["blog"]["benchmark_clicks_withvideo"];
$intBenchmarkMultimediaClicksWithVideo = @$arrBenchmarkClicksFMRInfo["multimedia"]["benchmark_clicks_withvideo"];
$intBenchmarkNewsletterClicksWithVideo = @$arrBenchmarkClicksFMRInfo["newsletter"]["benchmark_clicks_withvideo"];

//Benchmark Clicks without video By Media Type
$intBenchmarkPressClicksWithoutVideo = @$arrBenchmarkClicksFMRInfo["press_release"]["benchmark_clicks_withoutvideo"];
$intBenchmarkArticleClicksWithoutVideo = @$arrBenchmarkClicksFMRInfo["article"]["benchmark_clicks_withoutvideo"];
$intBenchmarkBlogClicksWithoutVideo = @$arrBenchmarkClicksFMRInfo["blog"]["benchmark_clicks_withoutvideo"];
$intBenchmarkMultimediaClicksWithoutVideo = @$arrBenchmarkClicksFMRInfo["multimedia"]["benchmark_clicks_withoutvideo"];
$intBenchmarkNewsletterClicksWithoutVideo = @$arrBenchmarkClicksFMRInfo["newsletter"]["benchmark_clicks_withoutvideo"];

// Comapany Average Calculation of Views
$intCompanyPressAvg = ($intTotalPressRelease != 0 && $intPressReleaseFmr != 0) ? round($intTotalPressRelease / $intPressReleaseFmr) : 0;
$intCompanyMediaAvg = ($intTotalMultiMedia != 0 && $intMultimediaFmr != 0) ? round($intTotalMultiMedia / $intMultimediaFmr) : 0;
$intCompanyBlogAvg  = ($intTotalBlogs != 0 && $intBlogFmr != 0) ? round($intTotalBlogs / $intBlogFmr) : 0;
$intCompanyNewsAvg  = ($intTotalNewsletter != 0 && $intNewsletterFmr != 0) ? round($intTotalNewsletter / $intNewsletterFmr) : 0;
$intCompanyArticleAvg = ($intTotalArticle != 0 && $intArticleFmr != 0) ? round($intTotalArticle / $intArticleFmr) : 0;

// Company Average Calculation of Views of FMRs with videos
$intCompanyPressAvgWithVideos = ($intTotalPressreleaseWithVideos != 0 && $intPressreleaseFmrWithVideos != 0) ? round($intTotalPressreleaseWithVideos / $intPressreleaseFmrWithVideos) : 0;
$intCompanyMediaAvgWithVideos = ($intTotalMultiMediaWithVideos != 0 && $intMultimediaFmrWithVideos != 0) ? round($intTotalMultiMediaWithVideos / $intMultimediaFmrWithVideos) : 0;
$intCompanyBlogAvgWithVideos  = ($intTotalBlogsWithVideos != 0 && $intBlogFmrWithVideos != 0) ? round($intTotalBlogsWithVideos / $intBlogFmrWithVideos) : 0;
$intCompanyNewsAvgWithVideos  = ($intTotalNewsletterWithVideos != 0 && $intNewsletterFmrWithVideos != 0) ? round($intTotalNewsletterWithVideos / $intNewsletterFmrWithVideos) : 0;
$intCompanyArticleAvgWithVideos = ($intTotalArticleWithVideos != 0 && $intArticleFmrWithVideos != 0) ? round($intTotalArticleWithVideos / $intArticleFmrWithVideos) : 0;

// Company Average Calculation of Views of FMRs without videos
$intCompanyPressAvgWithoutVideos = ($intTotalPressreleaseWithoutVideos != 0 && $intPressreleaseFmrWithoutVideos != 0) ? round($intTotalPressreleaseWithoutVideos / $intPressreleaseFmrWithoutVideos) : 0;
$intCompanyMediaAvgWithoutVideos = ($intTotalMultiMediaWithoutVideos != 0 && $intMultimediaFmrWithoutVideos != 0) ? round($intTotalMultiMediaWithoutVideos / $intMultimediaFmrWithoutVideos) : 0;
$intCompanyBlogAvgWithoutVideos  = ($intTotalBlogsWithoutVideos != 0 && $intBlogFmrWithoutVideos != 0) ? round($intTotalBlogsWithoutVideos / $intBlogFmrWithoutVideos) : 0;
$intCompanyNewsAvgWithoutVideos  = ($intTotalNewsletterWithoutVideos != 0 && $intNewsletterFmrWithoutVideos != 0) ? round($intTotalNewsletterWithoutVideos / $intNewsletterFmrWithoutVideos) : 0;
$intCompanyArticleAvgWithoutVideos = ($intTotalArticleWithoutVideos != 0 && $intArticleFmrWithoutVideos != 0) ? round($intTotalArticleWithoutVideos / $intArticleFmrWithoutVideos) : 0;

//Sum of all Average of Views of media-type
$intAllViewAvg =  ($intTotalCount!=0 && $intTotalCompanyFMR!=0)?round( $intTotalCount/$intTotalCompanyFMR):0;

//Click company average calculation
$intClickCompanyPressAvg = ($intClickTotalPress != 0 && $intPressReleaseFmr != 0) ? round($intClickTotalPress / $intPressReleaseFmr) : 0;
$intClickCompanyMediaAvg = ($intClickTotalMedia != 0 && $intMultimediaFmr != 0) ? round($intClickTotalMedia / $intMultimediaFmr) : 0;
$intClickCompanyBlogAvg = ($intClickTotalBlogs != 0 && $intBlogFmr != 0) ? round($intClickTotalBlogs / $intBlogFmr) : 0;
$intClickCompanyNewsAvg = ($intClickTotalNewsletter != 0 && $intNewsletterFmr != 0) ? round($intClickTotalNewsletter / $intNewsletterFmr) : 0;
$intClickCompanyArticleAvg = ($intClickTotalArticle != 0 && $intArticleFmr != 0) ? round($intClickTotalArticle / $intArticleFmr) : 0;

//Click company average calculation of FMRs with videos
$intClickCompanyPressAvgWithVideos = ($intClickTotalPressWithVideos != 0 && $intPressreleaseFmrWithVideos != 0) ? round($intClickTotalPressWithVideos / $intPressreleaseFmrWithVideos) : 0;
$intClickCompanyMediaAvgWithVideos = ($intClickTotalMediaWithVideos != 0 && $intMultimediaFmrWithVideos != 0) ? round($intClickTotalMediaWithVideos / $intMultimediaFmrWithVideos) : 0;
$intClickCompanyBlogAvgWithVideos = ($intClickTotalBlogsWithVideos != 0 && $intBlogFmrWithVideos != 0) ? round($intClickTotalBlogsWithVideos / $intBlogFmrWithVideos) : 0;
$intClickCompanyNewsAvgWithVideos = ($intClickTotalNewsletterWithVideos != 0 && $intNewsletterFmrWithVideos != 0) ? round($intClickTotalNewsletterWithVideos / $intNewsletterFmrWithVideos) : 0;
$intClickCompanyArticleAvgWithVideos = ($intClickTotalArticleWithVideos != 0 && $intArticleFmrWithVideos != 0) ? round($intClickTotalArticleWithVideos / $intArticleFmrWithVideos) : 0;

//Click company average calculation of FMRs without videos
$intClickCompanyPressAvgWithoutVideos = ($intClickTotalPressWithoutVideos != 0 && $intPressreleaseFmrWithoutVideos != 0) ? round($intClickTotalPressWithoutVideos / $intPressreleaseFmrWithoutVideos) : 0;
$intClickCompanyMediaAvgWithoutVideos = ($intClickTotalMediaWithoutVideos != 0 && $intMultimediaFmrWithoutVideos != 0) ? round($intClickTotalMediaWithoutVideos / $intMultimediaFmrWithoutVideos) : 0;
$intClickCompanyBlogAvgWithoutVideos = ($intClickTotalBlogsWithoutVideos != 0 && $intBlogFmrWithoutVideos != 0) ? round($intClickTotalBlogsWithoutVideos / $intBlogFmrWithoutVideos) : 0;
$intClickCompanyNewsAvgWithoutVideos = ($intClickTotalNewsletterWithoutVideos != 0 && $intNewsletterFmrWithoutVideos != 0) ? round($intClickTotalNewsletterWithoutVideos / $intNewsletterFmrWithoutVideos) : 0;
$intClickCompanyArticleAvgWithoutVideos = ($intClickTotalArticleWithoutVideos != 0 && $intArticleFmrWithoutVideos != 0) ? round($intClickTotalArticleWithoutVideos / $intArticleFmrWithoutVideos) : 0;

//Sum of all Avarage of Click medai-type
$intAllClickAvg = ($intClickTotalCount!=0 && $intTotalCompanyFMR!=0)?round( $intClickTotalCount/$intTotalCompanyFMR):0;

// function for checking user browser is IE8 or not.
list($strClickCss,$intClickFlag) = refactor_Check_user_browser();

?>
<ul class="analyticsTrigger">
<li class="clr"> &nbsp; </li>
<li id="viewsli">
    <div class="view_click" id="tcount1">
        <div class="togle" id="tgleDiv">&nbsp;</div>
        <div class="viewsnoBox"><?php echo number_format((int)$intTotalCount);?></div>
        <span class="total">Total</span>
        <div class="view-button view-buttn01 viewactive" id="viewtab"> <a  href="javascript:void(0);" onclick="javascript:showChart('views',<?php  echo
        $intClickFlag?>);">Views</a></div>
        <div class="view-button btn_inactive" id="clicktab"> <a href="javascript:void(0);" onclick="javascript:showChart('clicks',<?php  echo
        $intClickFlag?>);">Clicks</a> </div>
    </div>
    <div class="toggle_container_top">
        <p class="note1">This is the total number of views or clicks, as selected, for all content for the prior six months. The chart presents the past 60 days for views or clicks, summed into weekly  intervals.</p>
    </div>
    <div class="toggle_container">
        <div class="calCont calcounttop1"></div>
        <?php  if (!empty($arrViewChart)) {
        // checking if Viewchart array is empty for not if not show graph.if yes then show no analytics image.
        ?>
        <div id="view_chart_div">
            <div class='viewchartdiv'> <img src= '<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/metrics-ajax-loader.gif',array('absolute' => TRUE));?>' align='absmiddle'> </div>
        </div>
        <?php } else {?>
        <div class='viewchartdiv'> <img src= '<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/no_datafor_analytics.png',array('absolute' => TRUE));?>' align='absmiddle'> </div>
        <?php } ?>
    </div>
    <div class="clr"></div>
</li>

<li id="clicksli" style="<?php echo $strClickCss;?>">
    <div class="view_click" id="tcount2">
        <div class="togle" id="tgleDiv1">&nbsp;</div>
        <div class="viewsnoBox"><?php echo number_format((int)$intClickTotalCount);?></div>
        <span class="total">Total</span>
        <div class="view-button view-buttn01 btn_inactive" id="viewtab"> <a  href="javascript:void(0);" onclick="javascript:showChart('views',<?php  echo
        $intClickFlag?>);">Views</a></div>
        <div class="view-button viewactive" id="clicktab"> <a href="javascript:void(0);" onclick="javascript:showChart('clicks',<?php  echo
        $intClickFlag?>);">Clicks</a> </div>
    </div>
    <div class="toggle_container_top">
        <p class="note1">This is the total number of views or clicks, as selected, for all content for the prior six months. The chart presents the past 60 days for views or clicks.</p>
    </div>
    <div class="toggle_container">
        <div class="calCont calcounttop1"> </div>
        <?php   if (!empty($arrClickChart)) {
        // checking if Click Chart array is empty for not if not show graph.if yes then show no analytics image.
        ?>
        <div id="click_chart_div">
            <div class='viewchartdiv'> <img src='<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/metrics-ajax-loader.gif',array('absolute' => TRUE));?>' align='absmiddle'> </div>
        </div>
        <?php } else {?>
        <div class='viewchartdiv'> <img src= '<?php echo url($conf['IMAGES_PATH_3BL'].'/justmeans/no_datafor_analytics.png',array('absolute' => TRUE));?>' align='absmiddle'> </div>
        <?php } ?>
    </div>
    <div class="clr"></div>
</li>
<li> &nbsp; </li>
<li>
    <h2 class="trigger clr"><a href="#">Best Performers</a></h2>

    <div class="toggle_container" style="display: block;">
        <p class="note1">Below are up to five of the best performing FMRs in the past 60 days, based on clicks.</p>

        <div class="detailsTable">
            <div class="tableTh tableThOverflow padtop">
                <div class="toptablethdiv">
                    <span class="tablethspan">Title</span>

                    <div>
                        <div class="date">Date</div>
                        <div class="media">Media Type</div>
                        <div class="category">Category</div>
                    </div>
                </div>
                <h3 class="topviewsclickswidth"> Clicks </h3>
            </div>
            <div class="tableTd" id="mediaresult">
               <ul>
                    <?php
                    //check view and click array result exist or not if yes then show fmr title with viewcount and click count.
                    $intCount = 1;

                    if (!empty($arrTopClicksFMRInfo)) {
                        foreach ($arrTopClicksFMRInfo as $arrTopClicks) {

                            //to skipp the incomplete word or wrap the word
                            $strTitle = $arrTopClicks['title'];
                            if (strlen($strTitle) > 85) {
                                $strString = wordwrap($strTitle, 85);
                                $intPosition = strpos($strString, "\n");
                                if ($intPosition) {
                                    $strFMRTitle = substr($strString, 0, $intPosition) . "...";
                                }
                            } else {
                                $strFMRTitle = $strTitle;
                            }

                            // Primary Category
                            $strPrimaryCategory = "";
                            if ($arrTopClicks['primary_category'] != "") {
                                $strPrimaryCategory = $arrTopClicks['primary_category'];
                            }

                            // Media Type
                            $strMediaType = $arrTopClicks['media'];
                            ?>
                            <li class="<?php if ($intCount % 2 == 0) { ?>rowBg <?php } ?> height42">
                                <h2 class="h2padingleft h2width">
                                    <a class='awidthandmargin' href="<?php echo url('Dashboard/Analytics/Views/mediaid/' . $arrTopClicks['nid'], array('absolute' => TRUE));?>" title="<?php echo $strTitle; ?>"> <?php echo $strFMRTitle; ?></a>
                                </h2>

                                <div class="secdiv">
                                    <div class="topdatefloatleft">
                                        <div
                                            class="DateText datediv"><?php echo date('M d', strtotime($arrTopClicks['publishdate']));?><?php echo date(', Y', strtotime($arrTopClicks['publishdate']));?></div>
                                        <div class="DateText spanpadright mediawidth"><?php echo $strMediaType; ?></div>
                                        <div
                                            class="DateText spanpadright topcategorywidth"><?php echo $strPrimaryCategory; ?></div>
                                    </div>
                                    <h3 class="noText"><?php echo number_format((int) $arrTopClicks['clickcount']); ?></h3>
                                </div>
                            </li>
                            <?php  $intCount++;
                        }
                    } // if no fmr exits then
                    else {
                        ?>

                        <li class="">
                            <div class="lidiv"><b>No FMR exists for Best Performers.</b></div>
                        </li>
                        <?php }?>
                </ul>
            </div>
            <div class="clr"></div>
        </div>
    </div>
    <div class="clr"></div>
</li>
<li> &nbsp; </li>
<li>
    <h2 class="trigger clr"> <a href="#">Analytics by Media Type</a> </h2>
    <div class="toggle_container" class="displayblock">
        <p class="note1">The table below presents data for the prior six months. Select a media type to drill down to
            specific FMRs, including historical data. Hover on the information icon for an explanation of Benchmarks on
            this page.</p>
        <p class="note1"><b>Note:</b> FMRs that were uploaded as "Archive-only" are excluded from the numbers below. You can
            view the analytics for such FMRs in the detail pages that follow.</p>
        <div class="detailsTable">
            <div class="tableTh tableThOverflow">
                <ul class="ul-tableTh">
                    <li class="width15"><span class="tableTh-title padtop19">Media Type</span></li>
                    <li class="width12"><span class="tableTh-title padtop20">FMRs</span></li>
                    <li class="width12"><span class="tableTh-title padtop20">Views </span></li>
                    <li class="width12"><span class="tableTh-title padtop20">Clicks </span></li>
                    <li class="avgcol">
                        <div class="tableTh-title"> <span class="tableTh-title">Average</span>
                            <div class="ovfl-hidden"> <span class="subcol">Views</span> <span class="subcol">Clicks</span> </div>
                        </div>
                    </li>
                    <li class="avgcol width22">
                        <div class="tableTh-title">
                            <span class="tableTh-title benchmarkfmr">
                                Benchmark
                                <img class="benchmark_info" src="<?php echo url($conf['IMAGES_PATH_3BL'] . '/benchmark_info.png', array('absolute' => TRUE));?>">
                                <div style="display: none;" class="openme1 openme1front">
                                    <p class="alignleft">At this level of detail, Benchmarks are the site-wide average activity
                                        for all FMRs within the given Media Type (e.g. Press Releases or Blogs) using
                                        the most recent month for which Benchmarks have been calculated (i.e. the
                                        Benchmark does not cover the full 6 month period, but it is a reasonable proxy).</p>
                                    <div class="arwbx">
                                        <div class="arwpoint">&nbsp;</div>
                                    </div>
                                </div>
                            </span>
                            <div class="ovfl-hidden"><span class="subcol">Views</span> <span class="subcol">Clicks</span></div>
                        </div>
                    </li>
                </ul>
            </div>
            <div class="tableTd">
                <ul>
                    <!-- Pressrelease -->
                    <li class="rowBg">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="mtype-left"> <a  href="<?php echo url('Dashboard/Analytics/Views/mediatype/',array('absolute'=>TRUE));?>press_release">Press Releases</a>&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php echo number_format((int)$intPressReleaseFmr); ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intTotalPressRelease); ?> </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intClickTotalPress); ?> </span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intPressReleaseFmr > 0) {echo number_format((int)$intCompanyPressAvg);} else { echo "0"; } ?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intPressReleaseFmr > 0) {echo number_format((int)$intClickCompanyPressAvg); } else {echo "0";}?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php echo ((int) $intBenchmarkPressViews >0) ? number_format((int)$intBenchmarkPressViews) : 0; ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php echo ((int) $intBenchmarkPressClicks > 0) ? number_format((int) $intBenchmarkPressClicks) : 0; ?></span></li>
                        </ul>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos"> With Videos&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php if ((int)$intPressReleaseFmr > 0){echo number_format((int)$intPressreleaseFmrWithVideos);} else {echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalPressRelease > 0){ echo number_format((int)$intTotalPressreleaseWithVideos);} else {echo "-";} ?> </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalPress > 0){echo number_format((int)$intClickTotalPressWithVideos);} else {echo "-";} ?> </span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intCompanyPressAvg > 0) { if ((int)$intPressreleaseFmrWithVideos > 0) {echo number_format((int)$intCompanyPressAvgWithVideos);}else { echo "0"; }}else{echo "-";} ?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intClickCompanyPressAvg > 0) { if ((int)$intPressreleaseFmrWithVideos > 0) {echo number_format((int)$intClickCompanyPressAvgWithVideos); } else {echo "0";}}else{echo "-";}?>
                              </span></li>

                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkPressViews >0) { echo ((int) $intBenchmarkPressViewsWithVideo > 0) ? number_format((int) $intBenchmarkPressViewsWithVideo) : '0'; } else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkPressViews > 0) { echo ((int) $intBenchmarkPressClicksWithVideo > 0) ? number_format((int) $intBenchmarkPressClicksWithVideo) : '0'; } else { echo "-";} ?></span></li>
                        </ul>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos"> Without Videos&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php if ((int)$intPressReleaseFmr > 0) {echo number_format((int)$intPressreleaseFmrWithoutVideos);} else {echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalPressRelease > 0) {echo number_format((int)$intTotalPressreleaseWithoutVideos);} else {echo "-";} ?> </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalPress > 0) {echo number_format((int)$intClickTotalPressWithoutVideos);} else {echo "-";} ?> </span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intCompanyPressAvg > 0) { if ((int)$intPressreleaseFmrWithoutVideos > 0) {echo number_format((int)$intCompanyPressAvgWithoutVideos);}else { echo "0"; }}else{echo "-";} ?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intClickCompanyPressAvg > 0){ if ((int)$intPressreleaseFmrWithoutVideos > 0) {echo number_format((int)$intClickCompanyPressAvgWithoutVideos); } else {echo "0";}}else{echo "-";}?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkPressViews > 0) { echo ((int) $intBenchmarkPressViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkPressViewsWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkPressViews > 0) { echo ((int) $intBenchmarkPressClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkPressClicksWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                        </ul>
                    </li>
                    <!-- Multimeida -->
                    <li class="rowBg">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="mtype-left"> <a href="<?php echo url('Dashboard/Analytics/Views/mediatype/',array('absolute'=>TRUE));?>multimedia">Multimedia</a>&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php echo number_format((int)$intMultimediaFmr); ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intTotalMultiMedia);?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intClickTotalMedia);?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php  if ((int)$intMultimediaFmr > 0) {echo  number_format((int)$intCompanyMediaAvg); } else { echo "0";}?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intMultimediaFmr > 0) { echo   number_format((int)$intClickCompanyMediaAvg); } else {echo "0";}?>
                              </span> </li>
                            <li class="width11"><span class="tableTh-value"><?php echo ((int) $intBenchmarkMultimediaViews > 0) ? number_format((int) $intBenchmarkMultimediaViews) : 0; ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php echo ((int) $intBenchmarkMultimediaClicks > 0) ? number_format((int) $intBenchmarkMultimediaClicks) : 0; ?></span></li>
                        </ul>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos"> With Videos&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php if ((int)$intMultimediaFmr > 0) {echo number_format((int)$intMultimediaFmrWithVideos);} else {echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalMultiMedia > 0) {echo number_format((int)$intTotalMultiMediaWithVideos);} else {echo "-";}?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalMedia > 0) {echo number_format((int)$intClickTotalMediaWithVideos);} else {echo "-";}?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php  if ((int)$intCompanyMediaAvg > 0) { if ((int)$intMultimediaFmrWithVideos > 0) {echo number_format((int)$intCompanyMediaAvgWithVideos); } else { echo "0";}} else {echo "-";}?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intClickCompanyMediaAvg > 0) { if ((int)$intMultimediaFmrWithVideos > 0) { echo number_format((int)$intClickCompanyMediaAvgWithVideos); } else {echo "0";}} else {echo "-";}?>
                              </span> </li>
                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkMultimediaViews >0) { echo ((int) $intBenchmarkMultimediaViewsWithVideo > 0) ? number_format((int) $intBenchmarkMultimediaViewsWithVideo) : '0';} else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkMultimediaViews > 0) { echo ((int) $intBenchmarkMultimediaClicksWithVideo > 0) ? number_format((int) $intBenchmarkMultimediaClicksWithVideo) : '0';} else { echo "-";} ?></span></li>
                        </ul>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos"> Without Videos&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php if ((int)$intMultimediaFmr > 0) {echo number_format((int)$intMultimediaFmrWithoutVideos);} else {echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalMultiMedia > 0) { echo number_format((int)$intTotalMultiMediaWithoutVideos);} else {echo "-";}?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalMedia > 0) { echo number_format((int)$intClickTotalMediaWithoutVideos);} else {echo "-";}?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intCompanyMediaAvg > 0) { if ((int)$intMultimediaFmrWithoutVideos > 0) {echo  number_format((int)$intCompanyMediaAvgWithoutVideos); } else { echo "0";}} else {echo "-";}?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intClickCompanyMediaAvg > 0) { if ((int)$intMultimediaFmrWithoutVideos > 0) { echo   number_format((int)$intClickCompanyMediaAvgWithoutVideos); } else {echo "0";}} else {echo "-";}?>
                              </span> </li>
                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkMultimediaViews > 0) { echo ((int) $intBenchmarkMultimediaViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkMultimediaViewsWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkMultimediaViews > 0) { echo ((int) $intBenchmarkMultimediaClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkMultimediaClicksWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                        </ul>
                    </li>
                    <!-- Blogs -->
                    <li class="rowBg">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="mtype-left"><a href="<?php echo url('Dashboard/Analytics/Views/mediatype/',array('absolute'=>TRUE));?>blog">Blogs</a></span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php echo number_format((int)$intBlogFmr); ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intTotalBlogs);?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intClickTotalBlogs);?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intBlogFmr > 0) { echo  number_format((int)$intCompanyBlogAvg);} else { echo "0";}?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intBlogFmr > 0) {echo  number_format((int)$intClickCompanyBlogAvg);} else { echo "0";}?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php echo ((int) $intBenchmarkBlogViews > 0) ? number_format((int) $intBenchmarkBlogViews) : 0; ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php echo ((int) $intBenchmarkBlogClicks > 0) ? number_format((int) $intBenchmarkBlogClicks) : 0; ?></span></li>
                        </ul>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos">With Videos</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php  if ((int)$intBlogFmr > 0) { echo number_format((int)$intBlogFmrWithVideos); } else {echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalBlogs > 0) {echo number_format((int)$intTotalBlogsWithVideos);} else {echo "-";}?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalBlogs > 0) {echo number_format((int)$intClickTotalBlogsWithVideos);} else {echo "-";}?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intCompanyBlogAvg > 0) { if((int)$intBlogFmrWithVideos > 0) { echo  number_format((int)$intCompanyBlogAvgWithVideos);} else { echo "0";}}else{ echo "-";}?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intClickCompanyBlogAvg > 0){ if ((int)$intBlogFmrWithVideos > 0) {echo number_format((int)$intClickCompanyBlogAvgWithVideos);} else { echo "0";}} else { echo "-";}?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkBlogViews >0) { echo ((int) $intBenchmarkBlogViewsWithVideo > 0) ? number_format((int) $intBenchmarkBlogViewsWithVideo) : '0';} else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkBlogViews > 0) { echo ((int) $intBenchmarkBlogClicksWithVideo > 0) ? number_format((int) $intBenchmarkBlogClicksWithVideo) : '0';} else { echo "-";} ?></span></li>
                        </ul>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos">Without Videos</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php if ((int)$intBlogFmr > 0) {echo number_format((int)$intBlogFmrWithoutVideos);} else { echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalBlogs > 0) {echo number_format((int)$intTotalBlogsWithoutVideos);} else { echo "-";}?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalBlogs > 0) {echo number_format((int)$intClickTotalBlogsWithoutVideos);} else { echo "-";}?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intCompanyBlogAvg > 0) { if((int)$intBlogFmrWithoutVideos > 0) { echo  number_format((int)$intCompanyBlogAvgWithoutVideos);} else { echo "0";}} else { echo "-";}?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intClickCompanyBlogAvg > 0) {if ((int)$intBlogFmrWithoutVideos > 0) {echo  number_format((int)$intClickCompanyBlogAvgWithoutVideos);} else { echo "0";}} else { echo "-";}?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkBlogViews > 0) { echo ((int) $intBenchmarkBlogViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkBlogViewsWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkBlogViews > 0) { echo ((int) $intBenchmarkBlogClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkBlogClicksWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                        </ul>
                    </li>
                    <!-- Newsletter -->
                    <li class="rowBg">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="mtype-left"><a href="<?php echo url('Dashboard/Analytics/Views/mediatype/',array('absolute'=>TRUE));?>newsletter">Newsletters</a></span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php echo number_format((int)$intNewsletterFmr); ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intTotalNewsletter);?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intClickTotalNewsletter);?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intNewsletterFmr > 0) { echo  number_format((int)$intCompanyNewsAvg);} else { echo "0";}?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intNewsletterFmr > 0) {echo  number_format((int)$intClickCompanyNewsAvg);} else { echo "0";}?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php echo ((int) $intBenchmarkNewsletterViews > 0) ? number_format((int) $intBenchmarkNewsletterViews) : 0; ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php echo ((int) $intBenchmarkNewsletterClicks > 0) ? number_format((int) $intBenchmarkNewsletterClicks) : 0; ?></span></li>
                        </ul>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos">With Videos</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php if ((int)$intNewsletterFmr > 0) {echo number_format((int)$intNewsletterFmrWithVideos);} else {echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalNewsletter > 0) {echo number_format((int)$intTotalNewsletterWithVideos);} else {echo "-";}?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalNewsletter > 0) {echo number_format((int)$intClickTotalNewsletterWithVideos);} else {echo "-";}?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if((int)$intCompanyNewsAvg > 0) {if ((int)$intNewsletterFmrWithVideos > 0) { echo  number_format((int)$intCompanyNewsAvgWithVideos);} else { echo "0";}} else {echo "-";}?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intClickCompanyNewsAvg > 0) {if((int)$intNewsletterFmrWithVideos > 0){echo  number_format((int)$intClickCompanyNewsAvgWithVideos);} else { echo "0";}} else {echo "-";}?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkNewsletterViews >0) { echo ((int) $intBenchmarkNewsletterViewsWithVideo > 0) ? number_format((int) $intBenchmarkNewsletterViewsWithVideo) : '0';} else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkNewsletterViews > 0) { echo ((int) $intBenchmarkNewsletterClicksWithVideo > 0) ? number_format((int) $intBenchmarkNewsletterClicksWithVideo) : '0';} else { echo "-";} ?></span></li>
                        </ul>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos">Without Videos</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php if ((int)$intNewsletterFmr > 0) {echo number_format((int)$intNewsletterFmrWithoutVideos);} else {echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalNewsletter > 0) {echo number_format((int)$intTotalNewsletterWithoutVideos);} else {echo "-";}?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalNewsletter > 0) {echo number_format((int)$intClickTotalNewsletterWithoutVideos);} else {echo "-";}?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intCompanyNewsAvg > 0) { if ((int)$intNewsletterFmrWithoutVideos > 0) { echo  number_format((int)$intCompanyNewsAvgWithoutVideos);} else { echo "0";}} else {echo "-";}?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php if ((int)$intClickCompanyNewsAvg > 0) { if ((int)$intNewsletterFmrWithoutVideos > 0) {echo  number_format((int)$intClickCompanyNewsAvgWithoutVideos);} else { echo "0";}} else {echo "-";}?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkNewsletterViews > 0) { echo ((int) $intBenchmarkNewsletterViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkNewsletterViewsWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkNewsletterViews > 0) { echo ((int) $intBenchmarkNewsletterClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkNewsletterClicksWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                        </ul>
                    </li>
                    <!-- Article -->
                    <li class="rowBg">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="mtype-left"><a href="<?php echo url('Dashboard/Analytics/Views/mediatype/',array('absolute'=>TRUE));?>article">Articles</a>&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php echo number_format((int)$intArticleFmr); ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intTotalArticle);?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intClickTotalArticle);?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intArticleFmr > 0) {echo   number_format((int)$intCompanyArticleAvg);} else {echo "0";} ?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php  if ((int)$intArticleFmr > 0) { echo  number_format((int)$intClickCompanyArticleAvg);} else { echo "0";} ?>
                              </span></li>
                           <li class="width11"><span class="tableTh-value"><?php echo ((int) $intBenchmarkArticleViews > 0) ? number_format((int) $intBenchmarkArticleViews) : 0; ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php echo ((int) $intBenchmarkArticleClicks > 0) ? number_format((int) $intBenchmarkArticleClicks) : 0; ?></span></li>
                        </ul>
                        <div class="clr"></div>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos">With Videos&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php if ((int)$intArticleFmr > 0) {echo number_format((int)$intArticleFmrWithVideos);} else {echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalArticle > 0) {echo number_format((int)$intTotalArticleWithVideos);} else {echo "-";}?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalArticle > 0) {echo number_format((int)$intClickTotalArticleWithVideos);} else {echo "-";}?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intCompanyArticleAvg > 0) { if ((int)$intArticleFmrWithVideos > 0) {echo   number_format((int)$intCompanyArticleAvgWithVideos);} else {echo "0";}} else {echo "-";} ?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php  if ((int)$intClickCompanyArticleAvg > 0) { if ((int)$intArticleFmrWithVideos > 0) { echo  number_format((int)$intClickCompanyArticleAvgWithVideos);} else { echo "0";}} else {echo "-";} ?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkArticleViews >0) { echo ((int) $intBenchmarkArticleViewsWithVideo > 0) ? number_format((int) $intBenchmarkArticleViewsWithVideo) : '0';} else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkArticleViews > 0) { echo ((int) $intBenchmarkArticleClicksWithVideo > 0) ? number_format((int) $intBenchmarkArticleClicksWithVideo) : '0';} else { echo "-";} ?></span></li>
                        </ul>
                        <div class="clr"></div>
                    </li>
                    <li class="">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="tableTh-valueVideos">Without Videos&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php if ((int)$intArticleFmr > 0) {echo number_format((int)$intArticleFmrWithoutVideos);} else {echo "-";} ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intTotalArticle > 0) {echo number_format((int)$intTotalArticleWithoutVideos);} else {echo "-";}?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php if ((int)$intClickTotalArticle > 0) {echo number_format((int)$intClickTotalArticleWithoutVideos);} else {echo "-";}?></span></li>
                            <li class="width11"><span class="tableTh-value width96">
                              <?php if ((int)$intCompanyArticleAvg > 0) { if ((int)$intArticleFmrWithoutVideos > 0) {echo   number_format((int)$intCompanyArticleAvgWithoutVideos);} else {echo "0";}} else {echo "-";} ?>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96">
                              <?php  if ((int)$intClickCompanyArticleAvg > 0) {if ((int)$intArticleFmrWithoutVideos > 0) { echo  number_format((int)$intClickCompanyArticleAvgWithoutVideos);} else { echo "0";}} else {echo "-";} ?>
                              </span></li>
                            <li class="width11"><span class="tableTh-value"><?php if ((int) $intBenchmarkArticleViews > 0) { echo ((int) $intBenchmarkArticleViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkArticleViewsWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                            <li class="width12"><span class="tableTh-value width86"><?php if ((int) $intBenchmarkArticleViews > 0) { echo ((int) $intBenchmarkArticleClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkArticleClicksWithoutVideo) : '0';} else { echo "-";} ?></span></li>
                        </ul>
                        <div class="clr"></div>
                    </li>
                    <!-- All -->
                    <li class="rowBg">
                        <ul class="ul-tableTh">
                            <li class="width15"><span class="mtype-left"> <a href="<?php echo url('Dashboard/Analytics/Views/mediatype/',array('absolute'=>TRUE));?>all">All</a>&nbsp;</span> </li>
                            <li class="width12"><span class="tableTh-value width96">
                              <div class=""><?php echo  number_format((int)$intTotalCompanyFMR); ?></div>
                              </span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intTotalCount);?></span></li>
                            <li class="width12"><span class="tableTh-value width96"><?php echo number_format((int)$intClickTotalCount);?></span></li>
                            <li class="width11"><span class="tableTh-value width96"> <?php echo number_format((int)$intAllViewAvg); ?></span></li>
                            <li class="width12"><span class="tableTh-value width96"> <?php echo number_format((int)$intAllClickAvg); ?></span></li>
                            <li class="width11"><span class="tableTh-value">N/A</span></li>
                            <li class="width12"><span class="tableTh-value width86">N/A</span></li>
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
    <div id="medaiview" style="display:block;"></div>
</li>
</ul>
</div>
</div>
</div>
</div>
</div>
</div>
