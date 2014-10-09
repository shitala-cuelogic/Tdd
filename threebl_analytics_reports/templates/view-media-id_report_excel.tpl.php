<style type="text/css">
    .clsTitleColor {color: #00AACC}
    .clsPageTitle {color: #097F95;background-color: #E9F1F2}
    .clsGeographical {color: #097F95;background-color: #E9F1F2}
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
<table width="80%">
    <tr>
        <td width="10%" rowspan="11"></td>
        <td></td>
    </tr>
    <tr>
        <td width="90%" class="clsTitleColor"><h2><?php echo $strMediaTitle;?></h2>

            <p>Distributed
                on&nbsp;<?php echo date('M d', strtotime($strPublishDate));?><?php echo date(', Y', strtotime($strPublishDate));?></p>
        </td>
    </tr>
    <tr>
        <td><h2 class="clsPageTitle">Analytics for FMR</h2>
            <table>
                <tr>
                    <td>Views</td>
                    <td>Clicks</td>
                    <td>Clicks:Views (max = 1)</td>
                </tr>
                <tr>
                    <td><?php echo number_format((int)$intViewCount); ?></td>
                    <td><?php echo number_format((int)$intClickCount); ?></td>
                    <td><?php if ((int)$intViewCount > 0) {
                        echo number_format((int)$intClickCount / (int)$intViewCount, 2);
                    } else {
                        echo "0.00";
                    } ?></td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td><h2 class="clsGeographical">Geographical Stats</h2>
            <?php // country map and count list
            if (!empty($arrCountryListName)) {
                //checking array exits or not
                ?>
                <table>
                    <tr>
                        <td><strong>Country</strong></td>
                        <td><strong>Clicks</strong></td>
                <tr>
                    <?php   $intOther = 0; $intTotalCountry = count($arrCountryName);
                    foreach ($arrCountryListName as $objArrCountryName) {
                        if (strlen($objArrCountryName->country) > 1) {
                            ?>
                            <tr class=" listdiv rowBg">
                                <td><?php echo trim(strtoupper($objArrCountryName->country));?></td>
                                <td><?php echo number_format($objArrCountryName->ipcount);?></td>
                            </tr>
                            <?php
                        } else {
                            $intOther = $intOther + $objArrCountryName->ipcount;
                        }
                    }//END FOREACH
                    // Country name is blank or '-'
                    if ($intOther > 0) {
                        ?>
                        <tr class=" listdiv rowBg">
                            <td> OTHER/UNIDENTIFIED</td>
                            <td><?php echo number_format($intOther);?></td>
                        </tr>
                        <?php }?>
                </table>
                <?php } ?>
        </td>
    </tr>
    <tr>
        <td><h2 class="clsGeographical">Top Referring Websites</h2>
            <?php  if (!empty($arrRefereLink)) {
                //checking Referring website array exits or not. if yes show the Website list.
                ?>
                <table>
                    <tr>
                        <td><strong>Link</strong></td>
                    </tr>
                    <?php  $arrParseLink = '';
                    foreach ($arrRefereLink as $strLink) {
                        // getting only domain name form website name.
                        $arrParse = parse_url(trim($strLink->rlink));
                        $arrParseLink[] = trim($arrParse['host'] ? $arrParse['host'] : array_shift(explode('/', $arrParse['path'], 2))); // parse link
                    }//foreach
                    //make unique array for website name.
                    $arrParseLink = array_unique($arrParseLink);
                    $intTotalLink = count($arrParseLink);

                    foreach ($arrParseLink as $strRLink) {
                        ?>
                        <tr>
                            <td><?php echo trim($strRLink);?></td>
                        </tr>
                        <?php } ?>
                </table>
                <?php } ?>
        </td>
    </tr>
    <tr>
        <td>
            <h2 class="clsGeographical">Tracking Information</h2>

            <div>Below are a couple samples of your content on affiliate/syndication partners of 3BL Media. The complete
                report is available on the Tracking tab.
            </div>
        </td>
    </tr>
    <tr>
        <td>
            <table>
                <tr>
                    <?php
                    $intTrackCount = count($arrTrackingImg);
                    for ($intCount = 0; $intCount < $intTrackCount; $intCount++) {
                        ?>
                        <td width="50%" align='left' valign='middle'>&nbsp;<?php echo $arrTrackingImg[$i];?>&nbsp;</td>
                        <?php
                        if ($intCount % 2 == 0 && $intCount != 0) {
                            echo "</tr><tr>";
                        }
                    }//foreach  ?></tr>
            </table>
        </td>
    </tr>
</table>
