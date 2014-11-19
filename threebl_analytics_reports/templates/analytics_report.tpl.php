<?php
// array_pop for getting last value of dates.
$arrDatePop = array_pop($arrDts);
?>
<style type="text/css">
    h3 {background: none;padding: 0px;font-size: 20px;}
    a { text-decoration: none}
   .tbl, .tbl td {border: 1px solid #ddd}
   .tblhead {background: url("<?php echo url('sites/all/themes/threebl/images/justmeans/table_header2.png', array('absolute' => true));?>") repeat-x scroll left top transparent;}
   .mainTitleCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #00AACC;font-size: 16px;font-weight: bold;}
   .subTitleCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #00AACC;font-size: 14px;font-weight: bold;}
   .countCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #003A45;font-size: 14px;font-weight: bold;margin-left: 5px;}
   .descCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #2C2C2B;font-size: 12px;}
   .TableTitleCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #656565;font-size: 12px;font-weight: bold;padding: 5px 0;text-align: right;}
   .TableFirstColCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #00AACC;font-size: 12px;margin-left: 5px;vertical-align: middle; display: block; padding-top: 2px;}
   .tableTh-value {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #2C2C2B;font-size: 12px;margin-right: 5px}
   .tableTh-valueVideos {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #2C2C2B;font-size: 12px;margin-right: 5px;margin-left: 15px }
   .subTitleBackGround {background-color: #E9F1F2;}
   .clsBPadd18 {padding-bottom: 18px;}
   .clsBColor {color: #000000}
   .titlepadding{ padding-left: 124px !important; }
   .datespan{ display: block; float: left; width: 80px; padding-right: 10px; text-align: left; padding-bottom: 0; font-size: 9px !important; padding-left: 5px; padding-bottom: 2px;}
   .mediaspan{ display: block; float: left; width: 90px; padding-right: 10px; text-align: left; padding-bottom: 0; font-size: 9px !important; padding-bottom: 2px;}
   .Categoryspan{ display: block; float: left; width: 280px; text-align: left; padding-bottom: 0; font-size: 9px !important; padding-bottom: 2px;}
   .clsMediaLink{ width: 100%; display: block; margin-bottom: 5px; color: #1A97A7; text-decoration: none }
   .DateText{ font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; color: #2C2C2B !important; font-size: 9px !important; text-transform: uppercase; font-weight: normal;}
   .imgwidth{width: 12px; padding-right: 4px;}
   .nodatafont{font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; font-size: 13px; color: #2c2c2b; padding: 5px;}
   .benchmarkcolor{color: #8f9c9e}
</style>

<?php // define all the views count related Variable
$intArticleFmr = 0;
$intTotalCount = 0;
$intBlogFmr = 0;
$intTotalArticle = 0;
$intNewsletterFmr = 0;
$intTotalBlogs = 0;
$intMultimediaFmr = 0;
$intTotalPressRelease = 0;
$intPressReleaseFmr = 0;
$intTotalNewsletter = 0;
$intTotalMultiMedia = 0;
// getting the views count by company id for each media type
if (!empty($arrMediaTypeCount)) {
    $intTotalArticle = $arrMediaTypeCount['article']['viewcount'];
    $intTotalBlogs = $arrMediaTypeCount['blog']['viewcount'];
    $intTotalPressRelease = $arrMediaTypeCount['press_release']['viewcount'];
    $intTotalNewsletter = $arrMediaTypeCount['newsletter']['viewcount'];
    $intTotalMultiMedia = $arrMediaTypeCount['multimedia']['viewcount'];

    // sun of all ViewCount of media type
    $intTotalCount = $intTotalArticle + $intTotalBlogs + $intTotalPressRelease + $intTotalNewsletter + $intTotalMultiMedia;

    // With videos
    $intTotalArticleWithVideos = $arrMediaTypeCount['article']["withvideo"]['viewcount'];
    $intTotalBlogsWithVideos = $arrMediaTypeCount['blog']["withvideo"]['viewcount'];
    $intTotalPressReleaseWithVideos = $arrMediaTypeCount['press_release']["withvideo"]['viewcount'];
    $intTotalNewsletterWithVideos = $arrMediaTypeCount['newsletter']["withvideo"]['viewcount'];
    $intTotalMultiMediaWithVideos = $arrMediaTypeCount['multimedia']["withvideo"]['viewcount'];

    // Without videos
    $intTotalArticleWithoutVideos = $arrMediaTypeCount['article']["withoutvideo"]['viewcount'];
    $intTotalBlogsWithoutVideos = $arrMediaTypeCount['blog']["withoutvideo"]['viewcount'];
    $intTotalPressReleaseWithoutVideos = $arrMediaTypeCount['press_release']["withoutvideo"]['viewcount'];
    $intTotalNewsletterWithoutVideos = $arrMediaTypeCount['newsletter']["withoutvideo"]['viewcount'];
    $intTotalMultiMediaWithoutVideos = $arrMediaTypeCount['multimedia']["withoutvideo"]['viewcount'];

    // count of FMR with videos of views with company id
    $intArticleFmrWithVideos = $arrMediaTypeCount['article']["withvideo"]['fmrcount'];    
    $intBlogFmrWithVideos = $arrMediaTypeCount['blog']["withvideo"]['fmrcount'];
    $intNewsletterFmrWithVideos = $arrMediaTypeCount['newsletter']["withvideo"]['fmrcount'];
    $intMultimediaFmrWithVideos = $arrMediaTypeCount['multimedia']["withvideo"]['fmrcount'];
    $intPressReleaseFmrWithVideos = $arrMediaTypeCount['press_release']["withvideo"]['fmrcount'];

    // count of FMR with videos of views with company id
    $intArticleFmrWithoutVideos = $arrMediaTypeCount['article']["withoutvideo"]['fmrcount'];    
    $intBlogFmrWithoutVideos = $arrMediaTypeCount['blog']["withoutvideo"]['fmrcount'];
    $intNewsletterFmrWithoutVideos = $arrMediaTypeCount['newsletter']["withoutvideo"]['fmrcount'];
    $intMultimediaFmrWithoutVideos = $arrMediaTypeCount['multimedia']["withoutvideo"]['fmrcount'];
    $intPressReleaseFmrWithoutVideos = $arrMediaTypeCount['press_release']["withoutvideo"]['fmrcount'];

// count of FMR  of views with company id

    $intArticleFmr = $arrMediaTypeCount['article']['fmrcount'];
    $intBlogFmr = $arrMediaTypeCount['blog']['fmrcount'];
    $intNewsletterFmr = $arrMediaTypeCount['newsletter']['fmrcount'];
    $intMultimediaFmr = $arrMediaTypeCount['multimedia']['fmrcount'];
    $intPressReleaseFmr = $arrMediaTypeCount['press_release']['fmrcount'];

    //Sum of All Total media-type FMR
    $intTotalCompanyFMR = $intArticleFmr + $intBlogFmr + $intNewsletterFmr + $intMultimediaFmr + $intPressReleaseFmr;

}

//start : change
$intPressReleaseFmrs = ($intPressReleaseFmr <= 0) ? 1 : $intPressReleaseFmr;
$intMultimediaFmrs = ($intMultimediaFmr <= 0) ? 1 : $intMultimediaFmr;
$intBlogFmrs = ($intBlogFmr <= 0) ? 1 : $intBlogFmr;
$intNewsletterFmrs = ($intNewsletterFmr <= 0) ? 1 : $intNewsletterFmr;
$intArticleFmrs = ($intArticleFmr <= 0) ? 1 : $intArticleFmr;
//end : change

// Company Average Calculation of Views
$intCompanyPressAvg = round($intTotalPressRelease / $intPressReleaseFmrs);
$intCompanyMediaAvg = round($intTotalMultiMedia / $intMultimediaFmrs);
$intCompanyBlogAvg = round($intTotalBlogs / $intBlogFmrs);
$intCompanyNewsAvg = round($intTotalNewsletter / $intNewsletterFmrs);
$intCompanyArticleAvg = round($intTotalArticle / $intArticleFmrs);

// Company Average Calculation of Views of FMRs with videos
$intCompanyPressAvgWithVideos = round($intTotalPressReleaseWithVideos / $intPressReleaseFmrWithVideos);
$intCompanyMediaAvgWithVideos = round($intTotalMultiMediaWithVideos / $intMultimediaFmrWithVideos);
$intCompanyBlogAvgWithVideos = round($intTotalBlogsWithVideos / $intBlogFmrWithVideos);
$intCompanyNewsAvgWithVideos = round($intTotalNewsletterWithVideos / $intNewsletterFmrWithVideos);
$intCompanyArticleAvgWithVideos = round($intTotalArticleWithVideos / $intArticleFmrWithVideos);

// Company Average Calculation of Views of FMRs without videos
$intCompanyPressAvgWithoutVideos = round($intTotalPressReleaseWithoutVideos / $intPressReleaseFmrWithoutVideos);
$intCompanyMediaAvgWithoutVideos = round($intTotalMultiMediaWithoutVideos / $intMultimediaFmrWithoutVideos);
$intCompanyBlogAvgWithoutVideos = round($intTotalBlogsWithoutVideos / $intBlogFmrWithoutVideos);
$intCompanyNewsAvgWithoutVideos = round($intTotalNewsletterWithoutVideos / $intNewsletterFmrWithoutVideos);
$intCompanyArticleAvgWithoutVideos = round($intTotalArticleWithoutVideos / $intArticleFmrWithoutVideos);

//Sum of all Avarage of Views of media-type
$intAllViewAvg = round($intTotalCount / $intTotalCompanyFMR);

//array of object total Clicks of each media with Company
if (!empty($arrMediaTypeCount)) {

    $intClickTotalArticle = $arrMediaTypeCount['article']['clickcount'];
    $intClickTotalBlogs = $arrMediaTypeCount['blog']['clickcount'];
    $intClickTotalMedia = $arrMediaTypeCount['multimedia']['clickcount'];
    $intClickTotalNewsletter = $arrMediaTypeCount['newsletter']['clickcount'];
    $intClickTotalPress = $arrMediaTypeCount['press_release']['clickcount'];
    // press_release

    //Sum of all Total of click-count of each media-type.
    $intClickTotalCount = (int)$intClickTotalArticle + (int)$intClickTotalBlogs + (int)$intClickTotalMedia + (int)$intClickTotalNewsletter + (int)$intClickTotalPress;


    $intClickTotalArticleWithoutVideos = $arrMediaTypeCount['article']["withoutvideo"]['clickcount'];
    $intClickTotalBlogsWithoutVideos = $arrMediaTypeCount['blog']["withoutvideo"]['clickcount'];
    $intClickTotalMediaWithoutVideos = $arrMediaTypeCount['multimedia']["withoutvideo"]['clickcount'];
    $intClickTotalNewsletterWithoutVideos = $arrMediaTypeCount['newsletter']["withoutvideo"]['clickcount'];
    $intClickTotalPressWithoutVideos = $arrMediaTypeCount['press_release']["withoutvideo"]['clickcount'];

    $intClickTotalArticleWithVideos = $arrMediaTypeCount['article']["withvideo"]['clickcount'];
    $intClickTotalBlogsWithVideos = $arrMediaTypeCount['blog']["withvideo"]['clickcount'];
    $intClickTotalMediaWithVideos = $arrMediaTypeCount['multimedia']["withvideo"]['clickcount'];
    $intClickTotalNewsletterWithVideos = $arrMediaTypeCount['newsletter']["withvideo"]['clickcount'];
    $intClickTotalPressWithVideos = $arrMediaTypeCount['press_release']["withvideo"]['clickcount'];
}

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

//Click comapny avarage calculation
$intClickCompanyPressAvg = round($intClickTotalPress / $intPressReleaseFmr);
$intClickCompanyMediaAvg = round($intClickTotalMedia / $intMultimediaFmr);
$intClickCompanyBlogAvg = round($intClickTotalBlogs / $intBlogFmr);
$intClickCompanyNewsAvg = round($intClickTotalNewsletter / $intNewsletterFmr);
$intClickCompanyArticleAvg = round($intClickTotalArticle / $intArticleFmr);

//Click company average calculation of FMRs with videos
$intClickCompanyPressAvgWithVideos = round($intClickTotalPressWithVideos / $intPressReleaseFmrWithVideos);
$intClickCompanyMediaAvgWithVideos = round($intClickTotalMediaWithVideos / $intMultimediaFmrWithVideos);
$intClickCompanyBlogAvgWithVideos = round($intClickTotalBlogsWithVideos / $intBlogFmrWithVideos);
$intClickCompanyNewsAvgWithVideos = round($intClickTotalNewsletterWithVideos / $intNewsletterFmrWithVideos);
$intClickCompanyArticleAvgWithVideos = round($intClickTotalArticleWithVideos / $intArticleFmrWithVideos);

//Click company average calculation of FMRs without videos
$intClickCompanyPressAvgWithoutVideos = round($intClickTotalPressWithoutVideos / $intPressReleaseFmrWithoutVideos);
$intClickCompanyMediaAvgWithoutVideos = round($intClickTotalMediaWithoutVideos / $intMultimediaFmrWithoutVideos);
$intClickCompanyBlogAvgWithoutVideos = round($intClickTotalBlogsWithoutVideos / $intBlogFmrWithoutVideos);
$intClickCompanyNewsAvgWithoutVideos = round($intClickTotalNewsletterWithoutVideos / $intNewsletterFmrWithoutVideos);
$intClickCompanyArticleAvgWithoutVideos = round($intClickTotalArticleWithoutVideos / $intArticleFmrWithoutVideos);

//Sum of all Avarage of Click media-type
$intAllClickAvg = round($intClickTotalCount / $intTotalCompanyFMR);

// function for checking user browser is IE8 or not.
list($strClickCss, $intClickFlag) = threebl_Check_user_browser();
$strFileType = ''; //change
?>
<table width="100%" cellpadding="2" cellspacing="2">
<tr>
    <td width="90%"><span class="mainTitleCls">3BL Media -
                        <span class="clsBColor"> Analytics Summary Report</span><br/>
        <?php echo $strCompanyName;?><br/><?php echo date('F d, Y'); ?></span></td>
</tr>

<tr>
    <td width="90%">&nbsp;</td>
</tr>

<tr>
    <td class="subTitleBackGround"><span class="subTitleCls">Total Views:</span> <span
        class="countCls"> <?php echo number_format((int)$intTotalCount);?></span></td>
</tr>
<tr>
    <td><p class="descCls">The chart presents the past 60 days for views, summed into weekly intervals.</p></td>
</tr>
<tr>
    <td class="clsBPadd18">
        <?php echo $strViewGrp;?>
    </td>
</tr>
<tr>
    <td class="subTitleBackGround"><span class="subTitleCls">Total Clicks:</span><span
        class="countCls"><?php echo number_format((int)$intClickTotalCount);?></span></td>
</tr>
<tr>
    <td><p class="descCls">The chart presents the past 60 days for clicks, summed into weekly intervals.</p></td>
</tr>
<tr>
    <td class="clsBPadd18">
        <?php echo $strClickGrp;?>
    </td>
</tr>
<?php if (!empty($arrViewGraph)) { ?>
<tr>
    <td>&nbsp;</td>
</tr>
<?php } ?>
<tr>
    <td class="subTitleBackGround"><span class="subTitleCls">Best Performers</span></td>
</tr>
<tr>
    <td><p class="descCls">Below are up to five of the best performing FMRs in the past 60 days, based on clicks.</p></td>
</tr>
<tr>
    <td>
        <table width="100%" class="tbl" cellpadding="0" cellspacing="0">
            <tr valign="middle" class="tblhead">
                <td width="80%">
                    <span class="TableTitleCls titlepadding">Title</span> <br/>

                    <?php if (!empty($arrTopClicksFMRInfo)) { ?>
                    <span class="TableTitleCls datespan">Date</span>
                    <span class="TableTitleCls mediaspan">Media Type</span>
                    <span class="TableTitleCls Categoryspan">Category</span>
                    <?php } ?>
                </td>
                <td width="20%" align="center"><span class="TableTitleCls">Clicks</span></td>
            </tr>
            <? //check view and click array result exist or not if yes then show fmr title with viewcount and click count.
            if (!empty($arrTopClicksFMRInfo)) {
            foreach ($arrTopClicksFMRInfo as $arrTopClicks) {
                ?>
                <tr>
                    <td>
                        <span class="TableFirstColCls">
                            <a class="clsMediaLink"
                               href="<?php echo url('Dashboard/Analytics/Views/mediaid/' . $arrTopClicks['nid'], array('absolute' => true));?>"> <?php echo $arrTopClicks['title']; ?></a>
                        </span>
                        <span class="DateText datespan"> <?php echo date('M d', strtotime($arrTopClicks['publishdate']));?><?php echo date(', Y', strtotime($arrTopClicks['publishdate']));?></span>
                        <span class="DateText mediaspan"><?php echo $arrTopClicks['media']; ?></span>
                        <span class="DateText Categoryspan"><?php echo $arrTopClicks['primary_category']; ?></span>
                    </td>
                    <td align="center"><span class="tableTh-value"><?php echo number_format((int) $arrTopClicks['clickcount']); ?></span></td>
                </tr>
                <?
            }
        } else {
                ?>
                <tr>
                    <td colspan="2" align="center">
                        <div class="lidiv nodatafont"><b>No FMR exists for Best Performers.</b></div>
                    </td>
                </tr>
            <?php
            }
            ?>
        </table>
    </td>
</tr>
<tr>
    <td>&nbsp;</td>
</tr>
<?php if (!empty($arrViewGraph)) { ?>
<tr>
    <td>&nbsp;</td>
</tr>
<?php } ?>
<tr>
    <td class="subTitleBackGround"><span class="subTitleCls">Analytics by Media Type</span></td>
</tr>
<tr>
    <td>
        <p class="descCls">The table below presents data for the prior six months.</p>
        <p class="descCls"><b>Note:</b> FMRs that were uploaded as "Archive-only" are excluded from the numbers below. You can
            view the analytics for such FMRs in the detail report.</p>

        <p class="descCls benchmarkcolor"><img
            src="<?php echo url("$imagepath/benchmark_info.png", array('absolute' => TRUE));?>" class="imgwidth">At this
            level of detail, Benchmarks are the site-wide average activity
            for all FMRs within the given Media Type (e.g. Press Releases or Blogs) using
            the most recent month for which Benchmarks have been calculated (i.e. the
            Benchmark does not cover the full 6 month period, but it is a reasonable proxy).</p>
    </td>
</tr>
<tr>
<td>
<table width="100%" class="tbl" cellpadding="0" cellspacing="0" <?php if ($strFileType == "excel") {
    echo 'border="1"';
} ?>>
<tr align="center" valign="middle" class="tblhead">
    <td width="25%" rowspan="2"><span class="TableTitleCls">Media Type</span></td>
    <td width="11%" rowspan="2"><span class="TableTitleCls">FMRs</span></td>
    <td width="11%" rowspan="2"><span class="TableTitleCls">Views</span></td>
    <td width="11%" rowspan="2"><span class="TableTitleCls">Clicks</span></td>
    <td width="11%" colspan="2"><span class="TableTitleCls">Average</span></td>
    <td width="11%" colspan="2"><span class="TableTitleCls">Benchmark <img src="<?php echo url("$imagepath/benchmark_info.png", array('absolute' => TRUE));?>" class="imgwidth"></span></td>
</tr>
<tr>
    <td align="center"><span class="TableTitleCls">Views</span></td>
    <td align="center"><span class="TableTitleCls">Clicks</span></td>
    <td align="center"><span class="TableTitleCls">Views</span></td>
    <td align="center"><span class="TableTitleCls">Clicks</span></td>
</tr>

<tr>
    <td align="left"><a
        href="<?php echo url('Dashboard/Analytics/Views/mediatype/press_release', array('absolute' => true));?>">
        <span class="TableFirstColCls">Press Release</span></a></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intPressReleaseFmr); ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalPressRelease); ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalPress); ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intPressReleaseFmr > 0) {
        echo number_format((int)$intCompanyPressAvg);
    } else {
        echo "0";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intPressReleaseFmr > 0) {
        echo number_format((int)$intClickCompanyPressAvg);
    } else {
        echo "0";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkPressViews > 0) ? number_format((int) $intBenchmarkPressViews) : 0; ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkPressClicks > 0) ? number_format((int) $intBenchmarkPressClicks) : 0; ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">With Videos</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intPressReleaseFmr > 0) {
        echo number_format((int)$intPressReleaseFmrWithVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalPressRelease > 0) {
        echo number_format((int)$intTotalPressReleaseWithVideos);
    } else {
        echo "0.00";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalPress > 0) {
        echo number_format((int)$intClickTotalPressWithVideos);
    } else {
        echo "0.00";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intCompanyPressAvg > 0) {
        if ((int)$intPressReleaseFmrWithVideos > 0) {
            echo number_format((int)$intCompanyPressAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickCompanyPressAvg > 0) {
        if ((int)$intPressReleaseFmrWithVideos > 0) {
            echo number_format((int)$intClickCompanyPressAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkPressViews>0) { echo ((int) $intBenchmarkPressViewsWithVideo > 0) ? number_format((int) $intBenchmarkPressViewsWithVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkPressViews > 0) { echo ((int) $intBenchmarkPressClicksWithVideo > 0) ? number_format((int) $intBenchmarkPressClicksWithVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">Without Videos</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intPressReleaseFmr > 0) {
        echo number_format((int)$intPressReleaseFmrWithoutVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalPressRelease > 0) {
        echo number_format((int)$intTotalPressReleaseWithoutVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalPress > 0) {
        echo number_format((int)$intClickTotalPressWithoutVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intCompanyPressAvg > 0) {
        if ((int)$intPressReleaseFmrWithoutVideos > 0) {
            echo number_format((int)$intCompanyPressAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickCompanyPressAvg > 0) {
        if ((int)$intPressReleaseFmrWithoutVideos > 0) {
            echo number_format((int)$intClickCompanyPressAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkPressViews > 0) { echo ((int) $intBenchmarkPressViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkPressViewsWithoutVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkPressViews > 0) { echo ((int) $intBenchmarkPressClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkPressClicksWithoutVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left">
        <a href="<?php echo url('Dashboard/Analytics/Views/mediatype/multimedia', array('absolute' => true));?>">
            <span class="TableFirstColCls">Multimedia</span></a></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intMultimediaFmr); ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalMultiMedia);?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalMedia);?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php  if ((int)$intMultimediaFmr > 0) {
        echo  number_format((int)$intCompanyMediaAvg);
    } else {
        echo "0";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intMultimediaFmr > 0) {
        echo   number_format((int)$intClickCompanyMediaAvg);
    } else {
        echo "0";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkMultimediaViews > 0) ? number_format((int) $intBenchmarkMultimediaViews) : 0; ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkMultimediaClicks > 0) ? number_format((int) $intBenchmarkMultimediaClicks) : 0; ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">With Video</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intMultimediaFmr > 0) {
        echo number_format((int)$intMultimediaFmrWithVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalMultiMedia > 0) {
        echo number_format((int)$intTotalMultiMediaWithVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalMedia > 0) {
        echo number_format((int)$intClickTotalMediaWithVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php  if ((int)$intCompanyMediaAvg > 0) {
        if ((int)$intMultimediaFmrWithVideos > 0) {
            echo  number_format((int)$intCompanyMediaAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intClickCompanyMediaAvg > 0) {
        if ((int)$intMultimediaFmrWithVideos > 0) {
            echo   number_format((int)$intClickCompanyMediaAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkMultimediaViews >0) { echo ((int) $intBenchmarkMultimediaViewsWithVideo > 0) ? number_format((int) $intBenchmarkMultimediaViewsWithVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkMultimediaViews > 0) { echo ((int) $intBenchmarkMultimediaClicksWithVideo > 0) ? number_format((int) $intBenchmarkMultimediaClicksWithVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">Without Video</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intMultimediaFmr > 0) {
        echo number_format((int)$intMultimediaFmrWithoutVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalMultiMedia > 0) {
        echo number_format((int)$intTotalMultiMediaWithoutVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalMedia > 0) {
        echo number_format((int)$intClickTotalMediaWithoutVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php  if ((int)$intCompanyMediaAvg > 0) {
        if ((int)$intMultimediaFmrWithoutVideos > 0) {
            echo  number_format((int)$intCompanyMediaAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intClickCompanyMediaAvg > 0) {
        if ((int)$intMultimediaFmrWithoutVideos > 0) {
            echo   number_format((int)$intClickCompanyMediaAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkMultimediaViews > 0) { echo ((int) $intBenchmarkMultimediaViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkMultimediaViewsWithoutVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkMultimediaViews > 0) { echo ((int) $intBenchmarkMultimediaClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkMultimediaClicksWithoutVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left"><a href="<?php echo url('Dashboard/Analytics/Views/mediatype/blog', array('absolute' => true));?>">
        <span class="TableFirstColCls">Blogs</span></a></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intBlogFmr); ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalBlogs);?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalBlogs);?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intBlogFmr > 0) {
        echo  number_format((int)$intCompanyBlogAvg);
    } else {
        echo "0";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intBlogFmr > 0) {
        echo  number_format((int)$intClickCompanyBlogAvg);
    } else {
        echo "0";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkBlogViews > 0) ? number_format((int) $intBenchmarkBlogViews) : 0; ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkBlogClicks > 0) ? number_format((int) $intBenchmarkBlogClicks) : 0; ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">With Videos</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intBlogFmr > 0) {
        echo number_format((int)$intBlogFmrWithVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalBlogs > 0) {
        echo number_format((int)$intTotalBlogsWithVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalBlogs > 0) {
        echo number_format((int)$intClickTotalBlogsWithVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intCompanyBlogAvg > 0) {
        if ((int)$intBlogFmrWithVideos > 0) {
            echo  number_format((int)$intCompanyBlogAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intClickCompanyBlogAvg > 0) {
        if ((int)$intBlogFmrWithVideos > 0) {
            echo  number_format((int)$intClickCompanyBlogAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkBlogViews >0) { echo ((int) $intBenchmarkBlogViewsWithVideo > 0) ? number_format((int) $intBenchmarkBlogViewsWithVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkBlogViews > 0) { echo ((int) $intBenchmarkBlogClicksWithVideo > 0) ? number_format((int) $intBenchmarkBlogClicksWithVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">Without Videos</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intBlogFmr > 0) {
        echo number_format((int)$intBlogFmrWithoutVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalBlogs > 0) {
        echo number_format((int)$intTotalBlogsWithoutVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalBlogs > 0) {
        echo number_format((int)$intClickTotalBlogsWithoutVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intCompanyBlogAvg > 0) {
        if ((int)$intBlogFmrWithoutVideos > 0) {
            echo  number_format((int)$intCompanyBlogAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intClickCompanyBlogAvg > 0) {
        if ((int)$intBlogFmrWithoutVideos > 0) {
            echo  number_format((int)$intClickCompanyBlogAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkBlogViews > 0) { echo ((int) $intBenchmarkBlogViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkBlogViewsWithoutVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkBlogViews > 0) { echo ((int) $intBenchmarkBlogClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkBlogClicksWithoutVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left">
        <a href="<?php echo url('Dashboard/Analytics/Views/mediatype/newsletter', array('absolute' => true));?>"><span
            class="TableFirstColCls">Newsletters</span></a></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intNewsletterFmr); ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalNewsletter);?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalNewsletter);?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intNewsletterFmr > 0) {
        echo  number_format((int)$intCompanyNewsAvg);
    } else {
        echo "0";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intNewsletterFmr > 0) {
        echo  number_format((int)$intClickCompanyNewsAvg);
    } else {
        echo "0";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkNewsletterViews > 0) ? number_format((int) $intBenchmarkNewsletterViews) : 0; ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkNewsletterClicks > 0) ? number_format((int) $intBenchmarkNewsletterClicks) : 0; ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">With Videos</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intNewsletterFmr > 0) {
        echo number_format((int)$intNewsletterFmrWithVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalNewsletter > 0) {
        echo number_format((int)$intTotalNewsletterWithVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalNewsletter > 0) {
        echo number_format((int)$intClickTotalNewsletterWithVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intCompanyNewsAvg > 0) {
        if ((int)$intNewsletterFmrWithVideos > 0) {
            echo  number_format((int)$intCompanyNewsAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intClickCompanyNewsAvg > 0) {
        if ((int)$intNewsletterFmrWithVideos > 0) {
            echo  number_format((int)$intClickCompanyNewsAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkNewsletterViews >0) { echo ((int) $intBenchmarkNewsletterViewsWithVideo > 0) ? number_format((int) $intBenchmarkNewsletterViewsWithVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkNewsletterViews > 0) { echo ((int) $intBenchmarkNewsletterClicksWithVideo > 0) ? number_format((int) $intBenchmarkNewsletterClicksWithVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">Without Videos</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intNewsletterFmr > 0) {
        echo number_format((int)$intNewsletterFmrWithoutVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalNewsletter > 0) {
        echo number_format((int)$intTotalNewsletterWithoutVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalNewsletter > 0) {
        echo number_format((int)$intClickTotalNewsletterWithoutVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intCompanyNewsAvg > 0) {
        if ((int)$intNewsletterFmrWithoutVideos > 0) {
            echo  number_format((int)$intCompanyNewsAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intClickCompanyNewsAvg > 0) {
        if ((int)$intNewsletterFmrWithoutVideos > 0) {
            echo  number_format((int)$intClickCompanyNewsAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    }?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkNewsletterViews > 0) { echo ((int) $intBenchmarkNewsletterViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkNewsletterViewsWithoutVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkNewsletterViews > 0) { echo ((int) $intBenchmarkNewsletterClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkNewsletterClicksWithoutVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left">
        <a href="<?php echo url('Dashboard/Analytics/Views/mediatype/article', array('absolute' => true));?>">
            <span class="TableFirstColCls">Articles</span></a></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intArticleFmr); ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalArticle);?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalArticle);?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intArticleFmr > 0) {
        echo   number_format((int)$intCompanyArticleAvg);
    } else {
        echo "0";
    } ?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php  if ((int)$intArticleFmr > 0) {
        echo  number_format((int)$intClickCompanyArticleAvg);
    } else {
        echo "0";
    } ?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkArticleViews > 0) ? number_format((int) $intBenchmarkArticleViews) : 0; ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo ((int) $intBenchmarkArticleClicks > 0) ? number_format((int) $intBenchmarkArticleClicks) : 0; ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">With Videos</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intArticleFmr > 0) {
        echo number_format((int)$intArticleFmrWithVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalArticle > 0) {
        echo number_format((int)$intTotalArticleWithVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalArticle > 0) {
        echo number_format((int)$intClickTotalArticleWithVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intCompanyArticleAvg > 0) {
        if ((int)$intArticleFmrWithVideos > 0) {
            echo   number_format((int)$intCompanyArticleAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    } ?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php  if ((int)$intClickCompanyArticleAvg > 0) {
        if ((int)$intArticleFmrWithVideos > 0) {
            echo  number_format((int)$intClickCompanyArticleAvgWithVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    } ?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkArticleViews >0) { echo ((int) $intBenchmarkArticleViewsWithVideo > 0) ? number_format((int) $intBenchmarkArticleViewsWithVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkArticleViews > 0) { echo ((int) $intBenchmarkArticleClicksWithVideo > 0) ? number_format((int) $intBenchmarkArticleClicksWithVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left"><span class="tableTh-valueVideos">Without Videos</span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intArticleFmr > 0) {
        echo number_format((int)$intArticleFmrWithoutVideos);
    } else {
        echo "-";
    } ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intTotalArticle > 0) {
        echo number_format((int)$intTotalArticleWithoutVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int)$intClickTotalArticle > 0) {
        echo number_format((int)$intClickTotalArticleWithoutVideos);
    } else {
        echo "-";
    }?></span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intCompanyArticleAvg > 0) {
        if ((int)$intArticleFmrWithoutVideos > 0) {
            echo   number_format((int)$intCompanyArticleAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    } ?>
          </span></td>
    <td align="right"><span class="tableTh-value">
            <?php if ((int)$intClickCompanyArticleAvg > 0) {
        if ((int)$intArticleFmrWithoutVideos > 0) {
            echo  number_format((int)$intClickCompanyArticleAvgWithoutVideos);
        } else {
            echo "0";
        }
    } else {
        echo "-";
    } ?>
          </span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkArticleViews > 0) { echo ((int) $intBenchmarkArticleViewsWithoutVideo > 0) ? number_format((int) $intBenchmarkArticleViewsWithoutVideo) : '0';} else { echo "-";} ?></span></td>
    <td align="right"><span class="tableTh-value"><?php if ((int) $intBenchmarkArticleViews > 0) { echo ((int) $intBenchmarkArticleClicksWithoutVideo > 0) ? number_format((int) $intBenchmarkArticleClicksWithoutVideo) : '0';} else { echo "-";} ?></span></td>
</tr>
<tr>
    <td align="left"><a href="<?php echo url('Dashboard/Analytics/Views/mediatype/all', array('absolute' => true));?>">
        <span class="TableFirstColCls">All</span></a></td>
    <td align="right"><span class="tableTh-value"><?php echo  number_format((int)$intTotalCompanyFMR); ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalCount);?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalCount);?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intAllViewAvg); ?></span></td>
    <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intAllClickAvg); ?></span></td>
    <td align="right"><span class="tableTh-value">N/A</span></td>
    <td align="right"><span class="tableTh-value">N/A</span></td>
</tr>
</table>
</td>
</tr>
</table>
