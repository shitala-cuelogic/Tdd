<style type="text/css">
    .tbl td {border: 1px solid #ddd}
    .paddCls {padding: 5px 0 5px 0;}
    .mainTitleCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #00AACC;font-size: 16px;font-weight: bold;}
    .subTitleCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #00AACC;font-size: 14px;font-weight: bold;}
    .descCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #2C2C2B;font-size: 12px;}
    .TitledescCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #00AACC;font-size: 12px;font-weight: bold;}
    .TableTitleCls {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #656565;font-size: 12px;font-weight: bold;padding: 5px 0;text-align: right;margin-left: 10px;}
    .tableTh-value { font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #2C2C2B;font-size: 12px;margin-right: 5px;;margin-left: 10px;}
    .subTitleBackGround {background-color: #E9F1F2;padding-bottom: 5px;width: 100%}
    .DateText {font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif;color: #000000;font-size: 11px;}
    .clsBColor {color: #000000}
    .clsTDMediaTitle {color: #00AACC}
    .imgwidth{ width: 12px; padding-right: 4px;}
    .benchmarkcolor{ color: #8f9c9e }
    .paddingright10{padding-right: 10px;}
</style>
<?php // here we get Click  and View count of selected media id with last click or view date and media type.
$intClickCount = 0;
$intViewCount = 0;
if (!empty($arrClickCount)) {
    $intClickCount = $arrClickCount['totalcount'];
}
if (!empty($arrViewCount)) {
    $intViewCount = $arrViewCount['totalcount'];
}
?>

<table width="98%">
    <tr>
        <td width="90%"><span class="mainTitleCls">3BL Media -
        				<span class="clsBColor"> FMR Report</span><br/>
            <?php echo $strCompanyName;?><br/><?php echo date('F d, Y'); ?></span></td>
    </tr>

    <tr>
        <td width="90%">&nbsp;</td>
    </tr>

    <tr>
        <td width="90%" class="clsTDMediaTitle">
            <span class="TitledescCls">
	            <?php
                //Get FMR type
                $strMedia = $arrShowMediaType[$strFmrType];
                echo $strMediaTitle;
                ?>
            </span>

            <p class="DateText"><span class="paddingright10">Distributed on&nbsp;<?php echo date('M d', strtotime($strPublishDate));?><?php echo date(', Y', strtotime($strPublishDate)). $strIsArchive;?></span> <span
                class="paddingright10"><?php echo $strMedia; ?></span> <span><?php if ($strPrimaryCategoryName !="") { ?>
            <?php echo $strPrimaryCategoryName; ?>
            <?php } ?></span></p>
        </td>
    </tr>

    <tr>
        <td>&nbsp;</td>
    </tr>
    <tr>
        <td>
            <div class="subTitleCls subTitleBackGround">Analytics for FMR</div>

            <div class="paddCls">
                <p class="descCls">This is the total number of views and clicks on 3blmedia.com for
                this FMR since it was distributed.</p>

                <p class="descCls benchmarkcolor"><img src="<?php echo url("$imagepath/benchmark_info.png", array('absolute' => TRUE));?>" class="imgwidth">The Benchmarks for individual FMRs are the
                    site-wide average activity (views and clicks) for all FMRs within the same
                    Media Type, the same Primary Category and using the same
                    timeframe in which the given FMR was distributed. (The date,
                    media type and primary category for a given FMR are all listed
                    below the title, above.)</p>

                <p class="descCls benchmarkcolor">Benchmarks for a given month are
                    calculated 8 days after the end of the month. Before then you
                    will see a dash. Benchmarks before April 2014 were not
                    calculated and you will see "N/A".</p>
            </div>

            <table width="72%" class="tbl paddCls" cellpadding="0" cellspacing="0">
                <tr>
                    <td width="15%" rowspan="2"><span class="TableTitleCls">Views</span></td>
                    <td width="15%" rowspan="2"><span class="TableTitleCls">Clicks</span></td>
                    <td width="15%" colspan="2" align="center"><span class="TableTitleCls">Benchmark <img src="<?php echo url("$imagepath/benchmark_info.png", array('absolute' => TRUE));?>" class="imgwidth"></span></td>
                </tr>
                <tr>
                    <td><span class="TableTitleCls">Views</span></td>
                    <td><span class="TableTitleCls">Clicks</span></td>
                </tr>
                <tr>
                    <td><span class="tableTh-value"><?php echo number_format((int)$intViewCount); ?></span></td>
                    <td><span class="tableTh-value"><?php echo number_format((int)$intClickCount); ?></span></td>
                    <td><span class="tableTh-value"><?php echo $intBenchmarkView; ?></span></td>
                    <td><span class="tableTh-value"><?php echo $intBenchmarkClick; ?></span></td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td>&nbsp;</td>
    </tr>
    <tr>
        <td>
            <div class="subTitleCls subTitleBackGround">Geographical Stats</div>
            <?php // country map and count list
            if (!empty($arrCountryListName)) {
                //checking array exits or not
                ?>
                <table width="50%" class="tbl paddCls" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="30%"><span class="TableTitleCls">Country</span></td>
                        <td width="20%"><span class="TableTitleCls">Clicks</span></td>
                <tr>
                    <?php $intOther = 0; $intTotalCountry = count($arrCountryName);
                    foreach ($arrCountryListName as $strCountryName) {
                        if (strlen($strCountryName->country) > 1) {
                            ?>
                            <tr class=" listdiv rowBg">
                                <td><span
                                    class="tableTh-value"><?php echo trim(strtoupper($strCountryName->country));?></span>
                                </td>
                                <td><span
                                    class="tableTh-value"><?php echo number_format($strCountryName->ipcount);?></span>
                                </td>
                            </tr>
                            <?php
                        } else {
                            $intOther = $intOther + $strCountryName->ipcount;
                        }
                    }//END FOREACH
                    // Country name is blank or '-'
                    if ($intOther > 0) {
                        ?>
                        <tr class=" listdiv rowBg">
                            <td width="30%"><span class="tableTh-value">OTHER/UNIDENTIFIED</span></td>
                            <td width="20%"><span class="tableTh-value"><?php echo number_format($intOther);?></span>
                            </td>
                        </tr>
                        <?php }?>
                </table>
                <?php } ?>
        </td>
    </tr>
    <tr>
        <td>&nbsp;</td>
    </tr>
    <tr>
        <td>
            <div class="subTitleCls subTitleBackGround">Top Referring Websites</div>
            <?php
            if (!empty($arrRefereLink)) {
            //checking Referring website array exits or not. if yes show the Website list.
            ?>
            <table width="50%" class="tbl paddCls" cellpadding="0" cellspacing="0">
                <tr>
                    <td><span class="TableTitleCls">Link</span></td>
                </tr>
                <?php  $arrParseLink = '';
                foreach ($arrRefereLink as $objArrLink) {
                    // getting only domain name form website name.
                    $arrParse = parse_url(trim($objArrLink->rlink));
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
                    <tr>
                        <td><span class="tableTh-value"><?php echo trim($strRLink);?></span></td>
                    </tr>
                    <?php } ?>
            </table>
            <?php } ?>
        </td>
    </tr>
    <tr>
        <td>&nbsp;</td>
    </tr>
</table>
