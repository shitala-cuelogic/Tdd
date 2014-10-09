<?php
// array_pop for getting last value of dates.
$arrDatePop = array_pop($arrDts);
?>

<style type="text/css">
    h3 {background: none;padding: 0px;font-size: 20px;}
    .tbl, .tbl td {border: 1px solid #ddd}

    .tblhead{ background: url("<?php echo url('sites/all/themes/threebl/images/justmeans/table_header2.png', array('absolute' => TRUE));?>") repeat-x scroll left top transparent;}
    .mainTitleCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; color:#00AACC; font-size: 16px; font-weight:bold;}
    .subTitleCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; color:#00AACC; font-size: 14px; font-weight:bold;}
    .countCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; color: #003A45; font-size: 14px; font-weight: bold; margin-left:5px;}
    .descCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; color: #2C2C2B; font-size:12px;}
    .TableTitleCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; color:#656565; font-size:12px; font-weight:bold; padding:5px 0; text-align: right;}
    .TableFirstColCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; color:#00AACC; font-size: 12px; margin-left:5px; vertical-align: middle; display: block; padding-top: 2px;}
    .tableTh-value {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #2C2C2B; font-size:12px;}
    .subTitleBackGround {background-color: #E9F1F2}
    .DateText {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; color: #2C2C2B !important; font-size: 9px !important; text-transform: uppercase;}
    .clsTable {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; font-size: 12px;}
    .clsBColor {color: #000000}
    .clsBPadd18 {padding-bottom: 18px;}
    .clsMediaLink {width: 100%;display: block; margin-bottom:5px; color:#1A97A7; text-decoration: none}
    .datespan{display: block; float: left; width: 80px; padding-right: 10px; text-align: left; padding-bottom: 0; font-size: 9px !important; padding-left: 5px; padding-bottom: 2px;}
    .mediaspan{display: block; float: left; width: 90px; padding-right: 10px; text-align: left; padding-bottom: 0; font-size: 9px !important; padding-bottom: 2px;}
    .Categoryspan{display: block; float: left; width: 280px; text-align: left; padding-bottom: 0; font-size: 9px !important; padding-bottom: 2px;}
    .titlepadding{ padding-left: 124px !important;}
    .imgwidth{ width: 12px; padding-right: 4px;}
    .benchmarkcolor{ color: #8f9c9e }
</style>
<?php
// function for checking user browser is IE8 or not.
list($strClickCss, $intClickFlag) = threebl_Check_user_browser();

?>
<table width="98%" cellpadding="2" cellspacing="2" class="clsTable">
    <tr>
        <td width="90%"><span class="mainTitleCls">
         <?php if ((int)$intCampaignId > 0) {
            echo "3BL Media - "?>
            <span class="clsBColor"> <?php echo " Analytics Summary by Campaign: " . $strCampaignName; ?></span>
            <?php
        } else {
            echo "3BL Media - " ?>
            <span class="clsBColor"> <?php echo " Analytics Summary by Media Type: " . $strMediaType; ?></span>
            <?php }?>

            <br/><?php echo $strCompanyName;?><br/><?php echo date('F d, Y'); ?>
          </span></td>
    </tr>

    <tr>
        <td width="90%">&nbsp;</td>
    </tr>

    <tr>
        <td class="subTitleBackGround"><span class="subTitleCls">Total Views:</span><span
            class="countCls"><?php echo number_format($intTotalViews); ?></span></td>
    </tr>

    <tr>
        <td>
            <p class="descCls">
                <?php echo ((int)$intCampaignId > 0) ? "This is the total number of views, for this campaign for the prior six months. The chart presents the past 60 days for views, summed into weekly intervals." : "This is the total number of views, for all " . $strMediaType . "s content for the prior six months. The chart presents the past 60 days for views, summed into weekly intervals.";?>            </p>
        </td>
    </tr>
    <tr>
        <td class="clsBPadd18">
            <div id="view_chart_div"><?php echo $strViewGrp;?></div>
        </td>
    </tr>
    <tr>
        <td class="subTitleBackGround"><span class="subTitleCls">Total Clicks:</span> <span
            class="countCls"><?php echo number_format($intTotalClicks); ?></span></td>
    </tr>

    <tr>
        <td>
            <p class="descCls">
                <?php echo ((int)$intCampaignId > 0) ? "This is the total number of clicks, for this campaign for the prior six months. The chart presents the past 60 days for clicks, summed into weekly intervals." : "This is the total number of clicks, for all " . $strMediaType . "s content for the prior six months. The chart presents the past 60 days for clicks, summed into weekly intervals.";?>            </p>
        </td>
    </tr>
    <tr>
        <td class="clsBPadd18">
            <div id="click_chart_div"><?php echo $strClickGrp;?></div>
        </td>
    </tr>
    <tr>
        <td class="subTitleBackGround"><span class="subTitleCls">Analytics by FMR</span></td>
    </tr>

    <tr>
        <td>
            <p class="descCls"><?php echo (count($arrFMRInfo) > 0) ? "Select an FMR to view additional details." : "";?></p>
            <p class="descCls"><b>Note:</b> FMRs that were uploaded as "Archive-only" are marked with "(A)" in this list.</p>

            <p class="descCls benchmarkcolor"><img src="<?php echo url("$imagepath/benchmark_info.png", array('absolute' => TRUE));?>" class="imgwidth">The Benchmarks for individual FMRs are the
                site-wide average activity (views and
                clicks) for all FMRs within the same Media Type, the same Primary Category and using the
                same timeframe in which the given FMR was distributed. (The date, media type and primary
                category for a given FMR are all listed below the title in this list.)</p>
            <p class="descCls benchmarkcolor">Benchmarks for a given month are calculated 8 days after the end
                of the month. Before then you will see a dash. Benchmarks before April 2014 were not
                calculated and you will see "N/A".</p>
        </td>
    </tr>
    <tr>
        <td>&nbsp;</td>
    </tr>
    <tr>
        <td>
            <table width="100%" class="tbl" cellpadding="0" cellspacing="0">
                <tr valign="middle" class="tblhead">
                    <td width="60%" rowspan="2">
                        <span class="TableTitleCls titlepadding">Title</span> <br />
                        <?php
                        $strBenchmarkColumnWidth = "18%";
                        if (!empty($arrFMRInfo)) {
                            $strBenchmarkColumnWidth = "12%";
                        ?>
                        <span class="TableTitleCls datespan">Date</span>
                        <span class="TableTitleCls mediaspan">Media Type</span>
                        <span class="TableTitleCls Categoryspan">Category</span>
                        <?php } ?>
                    </td>
                    <?php if ((int)$intCampaignId == 0) { ?>
                        <td width="12%" align="center" rowspan="2"><span class="TableTitleCls"><?php echo 'Campaign';?></span></td>
                    <?php } ?>
                    <td width="8%" align="center" rowspan="2"><span class="TableTitleCls">Views</span></td>
                    <td width="8%" align="center" rowspan="2"><span class="TableTitleCls">Clicks</span></td>
                    <td width="<?php echo $strBenchmarkColumnWidth; ?>" align="center" colspan="2"><span class="TableTitleCls">&nbsp;Benchmark <img src="<?php echo url("$imagepath/benchmark_info.png", array('absolute' => TRUE));?>" class="imgwidth"></span></td>
                </tr>
                <tr>
                    <td align="center"><span class="TableTitleCls">Views</span></td>
                    <td align="center"><span class="TableTitleCls">Clicks</span></td>
                </tr>
                <? //check view and click array result exist or not if yes then show fmr title with viewcount and click count.
                if (!empty($arrFMRInfo)) {
                foreach ($arrFMRInfo as $arrRow) {

                    $strIsArchive = "";
                    if ($arrRow['is_archive'] == 1) {
                            $strIsArchive = " (A)";
                    }
                    ?>
                    <tr>
                        <td>
                            <span class="TableFirstColCls">
                                <a class="clsMediaLink" href="<?php echo url('Dashboard/Analytics/Views/mediaid/' . $arrRow['nid'], array('absolute' => true));?>"> <?php echo $arrRow['title']; ?></a>
                            </span>
                            <span class="DateText datespan"> <?php echo date('M d', strtotime($arrRow['publishdate']));?><?php echo date(', Y', strtotime($arrRow['publishdate'])). $strIsArchive;?></span>
                            <span class="DateText mediaspan"><?php echo $arrShowMediaType[$arrRow['media']]; ?></span>
                            <span class="DateText Categoryspan"><?php echo $arrRow['primary_category']; ?></span>
                        </td>

                        <?php if ((int)$intCampaignId == 0) { ?>
                        <td align="center">
                            <span class="tableTh-value"><?php echo $arrRow['campaign']; ?>&nbsp;</span>
                        </td>
                        <?php } ?>
                        <td align="center"><span class="tableTh-value"><?php echo trim(number_format((int)$arrRow['views'])); ?></span></td>
                        <td align="center"><span class="tableTh-value"><?php echo trim(number_format((int)$arrRow['clicks'])); ?></span></td>

                        <td align="center"><span  class="tableTh-value"><?php echo $arrRow['benchmark_view']; ?></span></td>
                        <td align="center"><span  class="tableTh-value"><?php echo $arrRow['benchmark_click']; ?></span>
                        </td>
                    </tr>

                    <?
                }
            }
                ?>
            </table>
        </td>
    </tr>

    <tr>
        <td>&nbsp;</td>
    </tr>

</table>

