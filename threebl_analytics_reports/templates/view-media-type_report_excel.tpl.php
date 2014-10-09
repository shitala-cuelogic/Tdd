<script type="text/javascript" src="https://www.google.com/jsapi"></script>
<?php
// array_pop for getting last value of dates.
$arrDatePop = array_pop($arrDts);
?>
<style type="text/css">
    h3{ background:none; padding:0px; font-size:20px;}
    .tblhead{ background: url("<?php echo url('sites/all/themes/threebl/images/justmeans/table_header1.png',array('absolute' => true));?>")
	 repeat-x scroll left top transparent; height: 38px; overflow: hidden;}
	.mainTitleCls {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color: #00AACC;font-size:16px;font-weight:bold;}
	.TableTitleCls {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif; color: #656565;font-size: 12px; font-weight: bold;padding: 5px 0;text-align: right;}
	.TableFirstColCls {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color:#00AACC;font-size:12px;margin-left:9px; vertical-align:middle;}
	.tableTh-value {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color:#2C2C2B;font-size:12px;}
	.DateText {font-family:Tahoma,Geneva,Arial,Helvetica,sans-serif;color: #687275;font-size:11px;}
    .clsExcelTable {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;font-size: 12px;}
    .clsMediaLink {width: 100%;display: block;margin-bottom: 5px;color: #00AACC;}
    .clsTitleColor {color:#00AACC}
</style>

<table width="98%" cellpadding="2" cellspacing="2" class="clsExcelTable">
    <tr>
        <td width="90%" align="left" colspan="7">
            <table width="100%" class="tbl" cellpadding="0" cellspacing="0" border="1">
                <tr>
                    <td colspan="7" align="left">
                    <span class="mainTitleCls"><?php
                        if ((int)$intCampaignId > 0) {
                            echo "3BL Media - Analytics Summary by Campaign: " . $strCampaignName;
                        } else {
                            echo "3BL Media - Analytics Summary by Media Type: " . $strMediaType;
                        }?>
                        <br/><?php echo $strCompanyName;?><br/><?php echo date('F d, Y'); ?></span></td>
                </tr>
                <tr>
                    <td colspan="7">&nbsp;</td>
                </tr>
                <tr>
                    <td colspan="7">Benchmark: The Benchmarks for individual FMRs are the site-wide average activity (views and clicks) for
                        all FMRs within the same Media Type, the same Primary Category and using the same timeframe in
                        which the given FMR was distributed. (The date, media type and primary category for a given FMR
                        are all listed below the title, above.)

                        Benchmarks for a given month are calculated 8 days after the end of the month. Before then you
                        will see a dash. Benchmarks before April 2014 were not calculated and you will see "N/A".
                    </td>
                </tr>
            </table>
        </td>
    </tr>

    <tr>
        <td width="90%" colspan="7">&nbsp;</td>
    </tr>
    <tr>
        <td width="90%">
            <table width="100%" class="tbl" cellpadding="0" cellspacing="0" border="1">
                <tr align="center" valign="middle" class="tblhead">
                    <td width="55%"><span class="TableTitleCls">Title</span></td>
                    <td width="10%"><span class="TableTitleCls">Primary Category</span></td>
                    <td width="15%"><span
                        class="TableTitleCls"><?php echo ((int)$intCampaignId > 0) ? 'FMR Type' : 'Campaign';?></span>
                    </td>
                    <td width="10%"><span class="TableTitleCls">Views</span></td>
                    <td width="10%"><span class="TableTitleCls">Clicks</span></td>

                    <td width="10%"><span class="TableTitleCls">Benchmark Views</span></td>
                    <td width="10%"><span class="TableTitleCls">Benchmark Clicks</span></td>
                </tr>
                <?php //check view and click array result exist or not if yes then show fmr title with viewcount and click count.
                if (!empty($arrFMRInfo)) {
                    foreach ($arrFMRInfo as $arrRow) {

                        $strIsArchive = "";
                        if ($arrRow['is_archive'] == 1) {
                            $strIsArchive = " (A)";
                        }
                        ?>
                        <tr>
                            <td align="left">&nbsp;<a class="clsMediaLink"
                                                      href="<?php echo url('Dashboard/Analytics/Views/mediaid/' . $arrRow['nid'], array('absolute' => true));?>"><span
                                class="TableFirstColCls clsTitleColor"><?php echo stripslashes($arrRow['title']); ?></span></a>
                                <span
                                    class="DateText"> <?php echo date('M d', strtotime($arrRow['publishdate']));?><?php echo date(', Y', strtotime($arrRow['publishdate'])). $strIsArchive;?></span>
                            </td>
                            <td align="left"><span
                                class="tableTh-value">&nbsp;<?php echo $arrRow['primary_category']; ?></span>
                            </td>
                            <td align="left"><span
                                class="tableTh-value">&nbsp;<?php echo ((int)$intCampaignId > 0) ? $arrShowMediaType[$arrRow['media']] : $arrRow['campaign']; ?>
                                &nbsp;</span></td>
                            <td align="right"><span class="tableTh-value"><?php echo trim(number_format((int)$arrRow['views'])); ?></span></td>
                            <td align="right"><span class="tableTh-value"><?php echo trim(number_format((int)$arrRow['clicks'])); ?></span></td>

                            <td align="right"><span class="tableTh-value"><?php echo $arrRow['benchmark_view']; ?></span></td>
                            <td align="right">  <span class="tableTh-value"><?php echo $arrRow['benchmark_click']; ?></span></td>
                        </tr>
                        <?php
                    }
                }
                ?>
            </table>
        </td>
    </tr>
</table>
