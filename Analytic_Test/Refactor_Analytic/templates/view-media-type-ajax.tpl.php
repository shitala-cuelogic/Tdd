<?php global $conf; ?>
<style type="text/css">
    .noText{width: 70px !important; margin-top: 2px !important;}
    .paginationDC{width: 248px !important;}
    .detailsTable .socaialAccounts .rightCont2 .tableTd4 ul.countryList li div.listdiv h2.width235{ width: 400px !important; }
</style>

<ul>
    <input id="mediatype" name="mediatype" value="<?php echo $strType;?>" type="hidden">
    <input id="pagenumber" name="pagenumber" value="<?php echo $intPage; ?>" type="hidden" />
    <input id="last" name="last" value="<?php echo $intLastPage; ?>"  type="hidden"/>
    <?php

    //check view and click array result exist or not if yes then show fmr title with viewcount and click count.
    $intCount=1;
    if (!empty($arrViewClickMediaResult)) {
    foreach ($arrViewClickMediaResult as $strViewClick) {

        //to skipp the incomplete word or wrap the word
        $strTitle = $strViewClick['title'];
        if (strlen($strTitle) > 58) {
            $strString = wordwrap($strTitle, 58);
            $intPosition = strpos($strString, "\n");
            if ($intPosition) {
                $strFMRTitle = substr($strString, 0, $intPosition)."...";
            }
        } else {
            $strFMRTitle = $strTitle;
        }

        // Primary Category
        $strPrimaryCategory = "";
        if ($strViewClick['primary_category'] != "") {
            $strPrimaryCategory = $strViewClick['primary_category'];

            if (array_key_exists($strPrimaryCategory, $arrLongPrimaryCategoryName)) {
                $strPrimaryCategory = $arrLongPrimaryCategoryName[$strPrimaryCategory];
            }
        }

        // Media Type
        $strMediaType = $strViewClick['media_type'];

        // Is Archive
        $strIsArchive = "";
        if ($strViewClick['is_archive'] == 1) {
            $strIsArchive = " (A)";
        }
    ?>
        <li class="<?php if ($intCount % 2 == 0 ) { ?>rowBg <?php } ?> height42">
            <h2 class="h2padingleft h2width"><a class='awidthandmargin' href="<?php echo url('Dashboard/Analytics/Views/mediaid/'.$strViewClick['nid'],array('absolute' => TRUE) );?>" title="<?php echo $strTitle; ?>"> <?php echo $strFMRTitle; ?></a>

            </h2>

            <div class="secdiv">
                <div class="datefloatleft">
                    <div class="DateText datediv"><?php echo date('M d', strtotime($strViewClick['publishDate']));?><?php echo date(', Y', strtotime($strViewClick['publishDate'])) . $strIsArchive;?></div>
                    <div class="DateText spanpadright mediawidth"><?php echo $strMediaType; ?></div>
                    <div class="DateText spanpadright categorywidth"><?php echo $strPrimaryCategory; ?></div>
                </div>

                <h3 class="noText"><?php echo number_format((int)$strViewClick['viewCount']); ?></h3>
                <h3 class="noText"><?php echo number_format((int)$strViewClick['clickCount']); ?></h3>
                <!--<h3 class="noText h3paddingright"><?php  if((int)$strViewClick['clickCount'] > 0) { echo number_format((int)$strViewClick['clickCount'] /(int)$strViewClick['viewCount'],2);} else { echo "0.00";} ?></h3>-->

                <div class="benchmarkviewval"><?php echo $strViewClick['benchmark_view']; ?></div>
                <div class="benchmarkclickval"><?php echo $strViewClick['benchmark_click']; ?></div>
            </div>
        </li>

        <?php  $intCount++; } }
// if no fmr exits then
else {?>

    <li class=""><div class="lidiv"><b>No FMR exists for selected media type.</b></div></li>
    <?php }?>
</ul>

<!-- Chekcing FMR LIST Array exits or not for pagintion -->
<?php
if (!empty($arrViewClickMediaResult)) {?>

<div class="paginationDC">
    <!-- Chekcing previous or back button  -->
    <?php
    if($intPrev == 0){?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',1)"><img src= '<?php echo url($conf['IMAGES_PATH_3BL'].'/first_ico.png',array('absolute' => TRUE));?>' align='absmiddle'></a></lable>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('prev')"><img src= '<?php echo url($conf['IMAGES_PATH_3BL'].'/prev_ico.png',array('absolute' => TRUE));?>' align='absmiddle'></a></lable>
        &nbsp;
        <!-- Chekcing current page is first page  with if yes also check current page is last or not  if not then show next pages  -->
        <?php } if ($intPage == 1) { ?>
    <lable> <a href="javascript:void(0);" class="active-page"><?php echo $intPage ; ?></a> </lable>
    <?php if (($intPage + 1) <= $intLastPage ) {?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage + 1; ?>)"><?php echo $intPage + 1; ?></a> </lable>
        <?php } if (($intPage + 2) <= $intLastPage ) {  ?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage + 2; ?>)"><?php echo $intPage + 2; ?></a> </lable>
        <?php  }

}
// Chekcing for last page if current page is last  then check it's previous pages by subtracting  -1, -2
else if ($intLastPage == $intPage) {
    if($intPage - 2 >= 1 ) { ?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage - 2; ?>)"><?php echo $intPage -2; ?></a> </lable>
        <?php } if(($intPage - 1) >= 1 ) { ?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage - 1; ?>)"><?php echo $intPage -1; ?></a> </lable>
        <?php } ?>
    <lable> <a href="javascript:void(0);" class="active-page"><?php echo $intPage; ?></a> </lable>

    <!-- Chekcing for current pages between first and last -->
    <?php }else {
    if(($intPage - 1) >= 1) { ?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage - 1; ?>)"><?php echo $intPage - 1; ?></a> </lable>
        <?php } ?>
    <lable> <a href="javascript:void(0);" class="active-page"><?php echo $intPage; ?></a> </lable>
    <?php if(($intPage + 1) <= $intLastPage ) { ?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage + 1; ?>)"><?php echo $intPage + 1; ?></a> </lable>

        <!-- Chekcing for next last button -->
        <?php } } if ($intNext == 0) {?>
    <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('next')"><img src= '<?php echo url($conf['IMAGES_PATH_3BL'].'/next_ico.png',array('absolute' => TRUE));?>' align='absmiddle'></a></lable>
    <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('last')"><img src= '<?php echo url($conf['IMAGES_PATH_3BL'].'/last_ico.png',array('absolute' => TRUE));?>' align='absmiddle'></a></lable>
    <?php }?>
</div>
<?php } ?>
