<style type="text/css">
.tdtitle { border-right: 1px solid #ACACAC; padding: 5px; border-bottom: 1px solid #e1e1e1;}

.tddata{ border-right: 1px solid #ACACAC; padding: 5px; border-bottom: 1px solid #e1e1e1;}
<?php if ($strDownloadExcel == "") { ?>
.TRClass{background: #E5E5E5}
.toptrclass td{ background: #000000;}
.tdtitlecolor{ color: white }
<?php } ?>
.divstyle{padding-bottom: 10px; float: left;}
.padleft{padding-left: 10px}
.padbot{ padding-bottom: 10px; }
.verticalalg{ vertical-align: middle; }
.download_report:hover{text-decoration: none !important;}
.fontsize{font-size: 14px;}
</style>

<div class="divstyle">
    <div>
        <span class="fontsize">There are total <b><?php echo count($arrFMRInfo) ?></b> FMRs</span>
        <span class="fontsize"><b> From:</b></span> <?php echo date("M d, Y", strtotime($intPrevMonthDate)); ?>
        <span class="fontsize padleft"><b> To:</b></span> <?php echo date("M d, Y", strtotime($strCurrentDate)); ?>
    </div>
</div>
<?php if ($strDownloadExcel == "") {?>
<div align="right" class="padbot">
    <a href="<?php echo $download_report_url; ?>" class="download_report">
        <img title="Download Report" src='<?php echo url("$imagepath/excels.png", array('absolute' => TRUE));?>' class="verticalalg" border="0">
    </a>
    <span><a href="<?php echo $download_report_url; ?>">Download Report</a></span>
</div>
<?php } ?>
<table width="100%" cellpadding="0" cellspacing="0" border="0" style="border:1px solid #e1e1e1">
    <tr class="toptrclass">
        <td class ="tdtitle" align="center" rowspan="2"><b><span class="tdtitlecolor">Nid</span></b></td>
        <td class="tdtitle" align="center" rowspan="2" width="32%"><b><span class="tdtitlecolor">FMR Title</span></b></td>
        <td class="tdtitle" align="center" rowspan="2" width="15%"><b><span class="tdtitlecolor">Company Name</span></b></td>
        <td class="tdtitle" align="center" rowspan="2" width="12%"><b><span class="tdtitlecolor">Published Date</span></b></td>
        <td class="tdtitle" align="center" colspan="2" ><b><span class="tdtitlecolor">3BL</span></b></td>

        <td class="tdtitle" align="center" colspan="2"><b><span class="tdtitlecolor">Jutsmeans</span></b></td>
        <td class="tdtitle" align="center" colspan="2"><b><span class="tdtitlecolor">Widget</span></b></td>
        <td class="tdtitle" align="center" colspan="2"><b><span class="tdtitlecolor">Newsletter Clicks</span></b></td>

    </tr>
    <tr class="toptrclass">
        <td class="tdtitle" align="center"><span class="tdtitlecolor">Views</span></td>
        <td class="tdtitle" align="center"><span class="tdtitlecolor">Clicks</span></td>
        <td class="tdtitle" align="center"><span class="tdtitlecolor">Views</span></td>
        <td class="tdtitle" align="center"><span class="tdtitlecolor">Clicks</span></td>
        <td class="tdtitle" align="center"><span class="tdtitlecolor">Views</span></td>
        <td class="tdtitle" align="center"><span class="tdtitlecolor">Clicks</span></td>
        <td class="tdtitle" align="center"><span class="tdtitlecolor">3BL</span></td>
        <td class="tdtitle" align="center"><span class="tdtitlecolor">Justmeans</span></td>
    </tr>
<?php
    $intCount = 0;
    if (is_array($arrFMRInfo) && count($arrFMRInfo) >0) {
        foreach ($arrFMRInfo as $intFMRId => $arrFMRData) {
            $strTRClass = "";
            // Apply background color to alternate TR
            if ($intCount % 2 == 0) {
                $strTRClass= "class='TRClass'";
            }

            $int3BLMediaViews = $arr3BLFmrViews[$intFMRId]["3blmedia"]["view"] + $arr3BLFmrViews[$intFMRId]["click_cron"]["view"];
            $int3BLMediaClicks = $arr3BLFmrClicks[$intFMRId]["3blmedia"]["click"] + $arr3BLFmrClicks[$intFMRId]["click_cron"]["click"];
?>
            <tr <?php echo $strTRClass; ?>>
                <td class="tddata"><?php echo $arrFMRData["fmrid"]; ?></td>
                <td class="tddata"><a href="<?php echo $base_url.'/node/'.$intFMRId; ?>" title="<?php echo $arrFMRData["title"]; ?>" target="_blank"><?php echo (strlen($arrFMRData["title"]) >60)? substr($arrFMRData["title"], 0, 60)."..." : $arrFMRData["title"]; ?></a></td>
                <td class="tddata"><a href="<?php echo $base_url . '/node/' . $arrFMRData["companyid"]; ?>"
                                      target="_blank"> <?php echo $arrFMRData["companyname"]; ?></a></td>
                <td class="tddata"><?php echo date("M d, Y h:i", strtotime($arrFMRData["publishdate"])); ?></td>

                <!-- 3BL Media Views and Clicks -->
                <td align="center" class="tddata"><?php echo ($int3BLMediaViews > 0) ? $int3BLMediaViews : 0; ?></td>
                <td align="center" class="tddata"><?php echo ($int3BLMediaClicks > 0) ? $int3BLMediaClicks : 0; ?></td>

                <!-- Justmeans Views and Clicks -->
                <td align="center" class="tddata"><?php echo ($arrJMViews[$intFMRId]["JMViews"] > 0) ? $arrJMViews[$intFMRId]["JMViews"] : 0; ?></td>
                <td align="center" class="tddata"><?php echo ($arrJMClicks[$intFMRId]["JMClicks"] > 0) ? $arrJMClicks[$intFMRId]["JMClicks"] : 0; ?></td>

                <!-- 3BL Widget Views and Clicks -->
                <td align="center" class="tddata"><?php echo ($arr3BLFmrViews[$intFMRId]["3bl_widgets"]["view"] > 0) ? $arr3BLFmrViews[$intFMRId]["3bl_widgets"]["view"] : 0; ?></td>
                <td align="center" class="tddata"><?php echo ($arr3BLFmrClicks[$intFMRId]["3bl_widgets"]["click"] > 0) ? $arr3BLFmrClicks[$intFMRId]["3bl_widgets"]["click"] : 0; ?></td>

                <!-- 3BL and Justmeans Newsletter Clicks -->
                <td align="center" class="tddata"><?php echo ($arr3BLNewsletterClickCount[$intFMRId]["3bl_newsletter_click"] > 0) ? $arr3BLNewsletterClickCount[$intFMRId]["3bl_newsletter_click"] : 0; ?></td>
                <td align="center" class="tddata"><?php echo ($arrJMNewsletterClicks[$intFMRId]["JMClicks"] > 0) ? $arrJMNewsletterClicks[$intFMRId]["JMClicks"] : 0; ?></td>
            </tr>
<?php
            $intCount ++;
        }
    } else {
?>
<tr><td colspan="12" align="center"><h4>Records not found</h4></td></tr>
<?php
    }
?>
</table>
