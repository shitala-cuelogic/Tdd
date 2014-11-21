<script type="text/javascript" src="https://www.google.com/jsapi"></script>
<?php
// array_pop for getting last value of dates.
$arrDatePop = array_pop($arrDts);
?>

<style type="text/css">
    h3{ background:none; padding:0px; font-size:20px;}
    .tbl, .tbl td{border:1px solid #ddd}
    .tblhead{ background: url("<?php echo url('sites/all/themes/threebl/images/justmeans/table_header1.png',array('absolute' => TRUE));?>") repeat-x scroll left top transparent;
        height: 38px;
        overflow: hidden;}
	 .mainTitleCls {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color: #00AACC;font-size:16px;font-weight:bold;}	
	 .subTitleCls {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color: #00AACC;font-size:14px;font-weight:bold;}
	 .countCls {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color:#003A45;font-size:14px;font-weight:bold; margin-left: 5px;}
	 .descCls {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color:#2C2C2B;font-size: 12px;}
	 .TableTitleCls {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif; color: #656565;font-size: 12px; font-weight: bold;padding: 5px 0;text-align: right;}
	 .TableFirstColCls {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color:#00AACC;font-size:12px;margin-left:9px; vertical-align:middle;}
	 .tableTh-value {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color:#2C2C2B;font-size:12px;margin-right:5px}
	 .subTitleBackGround{background-color:#E9F1F2}
	 
</style>
<?php // define all the views count related Variable
$intArticleFmr = 0;
$intTotalCount = 0;          $intBlogFmr = 0;
$intTotalArticle = 0;        $intNewsletterFmr = 0;
$intTotalBlogs = 0;          $intMultimediaFmr = 0;
$intTotalPressrelease = 0;   $intPressreleaseFmr = 0;
$intTotalNewsletter = 0;
$intTotalMultiMedia = 0;

$arrMediaTypeViewsCount = (isset($arrMediaTypeViewsCount)) ? $arrMediaTypeViewsCount : array(); //change

// geting the views count by company id for each media type
if (!empty($arrMediaTypeViewsCount)) {
    $intTotalArticle = $arrMediaTypeViewsCount['3bl_article']['viewcount'];
    $intTotalBlogs = $arrMediaTypeViewsCount['blogs']['viewcount'];
    $intTotalPressrelease = $arrMediaTypeViewsCount['pressrelease']['viewcount'] + $arrMediaTypeViewsCount['press_release']['viewcount'] ;
    $intTotalNewsletter = $arrMediaTypeViewsCount['3bl_newsletter']['viewcount'];
    $intTotalMultiMedia = $arrMediaTypeViewsCount['multimedia']['viewcount'];

    // sun of all viewscount of media type
    $intTotalCount = $intTotalArticle + $intTotalBlogs + $intTotalPressrelease + $intTotalNewsletter + $intTotalMultiMedia;

    // count of FMR  of views with company id
    $intArticleFmr = $arrMediaTypeViewsCount['3bl_article']['fmrcount'];
    $intBlogFmr = $arrMediaTypeViewsCount['blogs']['fmrcount'];
    $intNewsletterFmr = $arrMediaTypeViewsCount['3bl_newsletter']['fmrcount'];
    $intMultimediaFmr = $arrMediaTypeViewsCount['multimedia']['fmrcount'];
    $intPressreleaseFmr = $arrMediaTypeViewsCount['pressrelease']['fmrcount'] + $arrMediaTypeViewsCount['press_release']['fmrcount'];

    //Sum of All Total media-type FMR
    $intTotalComapnyFMR =  $intArticleFmr +  $intBlogFmr +  $intNewsletterFmr + $intMultimediaFmr +  $intPressreleaseFmr;

}//if-arrMediaTypeViewsCount

//start : change
$intPressreleaseFmr = ($intPressreleaseFmr <= 0 )?1:$intPressreleaseFmr;
$intMultimediaFmr = ($intMultimediaFmr <= 0 )?1:$intMultimediaFmr;
$intBlogFmr = ($intBlogFmr <= 0 )?1:$intBlogFmr;
$intNewsletterFmr = ($intNewsletterFmr <= 0 )?1:$intNewsletterFmr;
$intArticleFmr = ($intArticleFmr <= 0 )?1:$intArticleFmr;
//end : change

// Comapy Avarage Calculation of Views
$intCompanyPressAvg = round($intTotalPressrelease/$intPressreleaseFmr);
$intCompanyMediaAvg = round($intTotalMultiMedia/$intMultimediaFmr);
$intCompanyBlogAvg  = round($intTotalBlogs/$intBlogFmr);
$intCompanyNewsAvg  = round($intTotalNewsletter/$intNewsletterFmr);
$intCompanyArticleAvg = round($intTotalArticle/$intArticleFmr);

//Sum of all Avarage of Views of media-type
$intAllViewAvg =  (int)$intCompanyPressAvg +  (int)$intCompanyMediaAvg + (int)$intCompanyBlogAvg + (int)$intCompanyNewsAvg+ (int)$intCompanyArticleAvg;



//array of object total Clicks of each media with Company
if (!empty($objMediaTypeClickCount)) {
    $arrMediaTypeClickCount = array();
    foreach ($objMediaTypeClickCount as $strclick) {
        $arrMediaTypeClickCount[$strclick->fmr_type]['clickcount'] = $strclick->clickcount;
        $arrMediaTypeClickCount[$strclick->fmr_type]['fmrcount'] = $strclick->fmrcount;
    }//if
	
     $arrMediaTypeClickCount = (isset($arrMediaTypeClickCount)) ? $arrMediaTypeClickCount : array(); //change
    //Checking Click-Count of each media
    if (!empty($arrMediaTypeClickCount)) {
        $intClickTotalArticle = $arrMediaTypeClickCount['3bl_article']['clickcount'];
        $intClickTotalBlogs = $arrMediaTypeClickCount['blogs']['clickcount'];
        $intClickTotalMedia = $arrMediaTypeClickCount['multimedia']['clickcount'];
        $intClickTotalNewsletter = $arrMediaTypeClickCount['3bl_newsletter']['clickcount'];
        $intClickTotalPress =      $arrMediaTypeClickCount['article']['clickcount'] +  $arrMediaTypeClickCount['press_release']['clickcount'];
        // press_release

        //Sum of all Total of click-count of each media-type.
        $intClickTotalCount = (int)$intClickTotalArticle + (int)$intClickTotalBlogs + (int)$intClickTotalMedia + (int)$intClickTotalNewsletter
            + (int)$intClickTotalPress;

    }//if

}//obj-if

//Click comapny avarage calculation
$intClickCompanyPressAvg = round($intClickTotalPress/$intPressreleaseFmr);
$intClickCompanyMediaAvg = round($intClickTotalMedia/$intMultimediaFmr);
$intClickCompanyBlogAvg = round($intClickTotalBlogs/$intBlogFmr);
$intClickCompanyNewsAvg = round($intClickTotalNewsletter/$intNewsletterFmr);
$intClickCompanyArticleAvg = round($intClickTotalArticle/$intArticleFmr);

//Sum of all Avarage of Click media-type
$intAllClickAvg = (int)$intClickCompanyPressAvg +  (int)$intClickCompanyMediaAvg +  (int)$intClickCompanyBlogAvg +  (int)$intClickCompanyNewsAvg + (int)$intClickCompanyArticleAvg;

// function for checking user browser is IE8 or not.
list($strClickCss,$intClickFlag) = threebl_Check_user_browser();
$strFileType = '';  //change
?>
<table width="98%" cellpadding="2" cellspacing="2" >
    <tr>
        <td width="90%"><span class="mainTitleCls">Main Content Summary Report: A summary of views and clicks over the past six months</span></td>
    </tr>
    
     <tr>
        <td width="90%">&nbsp;</td>
    </tr>
    
    <tr>
        <td class="subTitleBackGround"><span class="subTitleCls">Total Views:</span>&nbsp;<span class="countCls">&nbsp;<?php echo number_format((int)$intTotalCount);?></span></td>
    </tr>
    <tr>
        <td><p class="descCls">This is the total number of views, for all content for the prior six months. The chart presents the past 60 days for views.</p></td>
    </tr>
    <tr>
        <td height="320" width="900">
            <?php echo $viewGrp;?>
        </td>
    </tr>
    <tr>
        <td class="subTitleBackGround"><span class="subTitleCls">Total Clicks:</span><span class="countCls"><?php echo number_format((int)$intClickTotalCount);?>
        </span></td>
    </tr>
    <tr>
        <td><p class="descCls">This is the total number of clicks, for all content for the prior six months. The chart presents the past 60 days for clicks.</p></td>
    </tr>
    <tr>
        <td height="320" width="900">
            <?php echo $clickGrp;?>
        </td>
    </tr>
    <tr>
        <td class="subTitleBackGround"><span class="subTitleCls">Analytics by Media Type</span></td>
    </tr>
    <tr>
        <td><p class="descCls">The table below presents data for the prior six months. Select a media type to drill down to specific FMRs for additional information and historical data.</p></td>
    </tr>
    <tr>
        <td><table width="100%" class="tbl" cellpadding="0" cellspacing="0" <?php if ($strFileType == "excel") { echo 'border="1"';} ?>>
            <tr align="center" valign="middle" class="tblhead">
                <td width="25%"><span class="TableTitleCls">Media Type</span></td>
                <td width="10%"><span class="TableTitleCls">FMRs</span></td>
                <td width="15%"><span class="TableTitleCls">Views</span></td>
                <td width="15%"><span class="TableTitleCls">Clicks</span></td>
                <td width="10%"><span class="TableTitleCls">Avg. Views</span></td>
                <td width="10%"><span class="TableTitleCls">Avg. Clicks</span></td>
                <td width="15%"><span class="TableTitleCls">Clicks:Views (max = 1)</span></td>
            </tr>
            <tr>
                <td align="left"><span class="TableFirstColCls">&nbsp;Press Release</span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intPressreleaseFmr); ?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalPressrelease); ?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalPress); ?></span></td>
                <td align="right" ><span class="tableTh-value"><?php if((int)$intPressreleaseFmr > 0) {echo number_format((int)$intCompanyPressAvg);}else { echo "0"; } ?></span></td>
                <td align="right" ><span class="tableTh-value"><?php if((int)$intPressreleaseFmr > 0) {echo number_format((int)$intClickCompanyPressAvg); } else {echo "0";}?></span></td>
                <td align="right" ><span class="tableTh-value"><?php if((int)$intCompanyPressAvg > 0 && (int)$intClickCompanyPressAvg > 0) {echo   number_format(( (int)$intClickCompanyPressAvg / (int)$intCompanyPressAvg),2); } else {echo "0.00";}?>&nbsp;</span></td>
            </tr>
            <tr>
                <td align="left"><span class="TableFirstColCls">&nbsp;Multimedia</span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intMultimediaFmr); ?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalMultiMedia);?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalMedia);?></span></td>
                <td align="right" ><span class="tableTh-value">
            <?php  if ((int)$intMultimediaFmr > 0) {echo  number_format((int)$intCompanyMediaAvg); } else { echo "0";}?>
          </span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intMultimediaFmr > 0) { echo   number_format((int)$intClickCompanyMediaAvg); } else {echo "0";}?>
          </span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intCompanyMediaAvg > 0 && (int)$intClickCompanyMediaAvg > 0) {echo   number_format(( (int)$intClickCompanyMediaAvg / (int)$intCompanyMediaAvg),2); } else {echo "0.00";}?>&nbsp;
          </span></td>
            </tr>
            <tr>
                <td align="left"><span class="TableFirstColCls">&nbsp;Blogs</span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intBlogFmr); ?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalBlogs);?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalBlogs);?></span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intBlogFmr > 0) { echo  number_format((int)$intCompanyBlogAvg);} else { echo "0";}?>
          </span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intBlogFmr > 0){echo  number_format((int)$intClickCompanyBlogAvg);} else { echo "0";}?>
          </span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intCompanyBlogAvg > 0 && (int)$intClickCompanyBlogAvg > 0) {echo   number_format(( (int)$intClickCompanyBlogAvg / (int)$intCompanyBlogAvg),2); } else {echo "0.00";}?>&nbsp;
          </span></td>
            </tr>
            <tr>
                <td align="left"><span class="TableFirstColCls">&nbsp;Newsletters</span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intNewsletterFmr); ?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalNewsletter);?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalNewsletter);?></span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intNewsletterFmr > 0) { echo  number_format((int)$intCompanyNewsAvg);} else { echo "0";}?>
          </span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intNewsletterFmr > 0){echo  number_format((int)$intClickCompanyNewsAvg);} else { echo "0";}?>
          </span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intCompanyNewsAvg > 0 && (int)$intClickCompanyNewsAvg > 0) {echo   number_format(( (int)$intClickCompanyNewsAvg / (int)$intCompanyNewsAvg),2); } else {echo "0.00";}?>&nbsp;
          </span></td>
            </tr>
            <tr>
                <td align="left"><span class="TableFirstColCls">&nbsp;Articles</span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intArticleFmr); ?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalArticle);?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalArticle);?></span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intArticleFmr > 0) {echo   number_format((int)$intCompanyArticleAvg);} else {echo "0";} ?>
          </span></td>
                <td align="right" ><span class="tableTh-value">
            <?php  if ((int)$intArticleFmr > 0) { echo  number_format((int)$intClickCompanyArticleAvg);} else { echo "0";} ?>
          </span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intCompanyArticleAvg > 0 && (int)$intClickCompanyArticleAvg > 0) {echo
                number_format(( (int)$intClickCompanyArticleAvg / (int)$intCompanyArticleAvg),2); } else {echo "0.00";}?>
          </span>
          &nbsp;
          </td>
            </tr>
            <tr>
                <td align="left"><span class="TableFirstColCls">&nbsp;All</span></td>
                <td align="right"><span class="tableTh-value"><?php echo  number_format((int)$intTotalComapnyFMR); ?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intTotalCount);?></span></td>
                <td align="right"><span class="tableTh-value"><?php echo number_format((int)$intClickTotalCount);?></span></td>
                <td align="right" ><span class="tableTh-value"><?php echo number_format((int)$intAllViewAvg); ?></span></td>
                <td align="right" ><span class="tableTh-value"><?php echo number_format((int)$intAllClickAvg); ?></span></td>
                <td align="right" ><span class="tableTh-value">
            <?php if ((int)$intAllViewAvg > 0) {echo number_format(((int)$intAllClickAvg /(int)$intAllViewAvg),2);} else {echo "0.00";}?>
          </span>
          &nbsp;
          </td>
            </tr>
        </table></td>
    </tr>
</table>
