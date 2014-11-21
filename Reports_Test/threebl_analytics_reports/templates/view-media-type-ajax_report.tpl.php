<style type="text/css">
    .height42{height: 42px;}
    .h2pad{padding-left: 10px;}
    .anchorcls{width: 100%; display: block; margin-bottom: 5px;}
    .h3cls{padding-right: 10px; padding-left: 45px;}
    .noFMRstyle{color: #495761; padding-top: 5px; text-align: center;}
</style>
<ul>
    <input id="mediatype" name="mediatype" value="<?php echo $strType;?>" type="hidden">
    <input id="pagenumber" name="pagenumber" value="<?php echo $intPage; ?>" type="hidden" />
    <input id="last" name="last" value="<?php echo $intLastPage; ?>"  type="hidden"/>
    <?php  $arrViewClick=''; //define array which conation view and click array result.

    // Getting FMR View-list with View-Count. Here Checking array exits or not
    if (!empty($arrViewMediaResult)) {
        foreach ($arrViewMediaResult as $strViewMedia) {
            $arrViewClick[$strViewMedia->nid]['nid'] = $strViewMedia->nid;
            $arrViewClick[$strViewMedia->nid]['title'] = $strViewMedia->title;
            $arrViewClick[$strViewMedia->nid]['publishdate'] = $strViewMedia->publishdate;
            $arrViewClick[$strViewMedia->nid]['viewcount'] = $strViewMedia->viewcount;
            $arrViewClick[$strViewMedia->nid]['clickcount'] = '0';
        }//for each
    }//if

    // Getting FMR Click-list with Click-Count. Here Checking array exits or not
    if (!empty($arrClickMediaResult)) {
        foreach ($arrClickMediaResult as $strClickMedia) {
            if (array_key_exists($strClickMedia->nid, $arrViewClick)) {
                $arrViewClick[$strClickMedia->nid]['clickcount']= $strClickMedia->clickcount;
            } else {
                $arrViewClick[$strClickMedia->nid]['nid'] = $strClickMedia->nid;
                $arrViewClick[$strClickMedia->nid]['title'] = $strClickMedia->title;
                $arrViewClick[$strClickMedia->nid]['clickcount'] = $strClickMedia->clickcount;
                $arrViewClick[$strClickMedia->nid]['viewcount'] = '0';
            }//else
        }//foreach
    }//if

    //check view and click array result exist or not if yes then show fmr title with viewcount and click count.
    $i=1;  if (!empty($arrViewClick)) {
    foreach ($arrViewClick as $strViewClick) {?>
        <li class="height42 <?php if ($i%2==0) {?>rowBg <?php }?>">
            <h2 class="h2pad"><a class="anchorcls" href="<?php echo url('Dashboard/Analytics/Views/mediaid/'.$strViewClick['nid'],array('absolute' => TRUE) );?>"> <?php echo substr($strViewClick['title'],0,51);if(strlen($strViewClick['title']) > 51){echo "..."; }?></a>
                <span class="DateText"> <?php echo date('M d',strtotime($strViewClick['publishdate']));?><?php echo date(', Y',strtotime($strViewClick['publishdate']));?></span>
            </h2>
            <h3 class="noText"><?php echo number_format((int)$strViewClick['viewcount']); ?></h3>
            <h3 class="noText"><?php echo number_format((int)$strViewClick['clickcount']); ?></h3>
            <h3 class="noText h3cls"><?php  if((int)$strViewClick['clickcount'] > 0) { echo number_format((int)$strViewClick['clickcount'] /(int)$strViewClick['viewcount'],2);} else { echo "0.00";} ?></h3>
        </li>

        <?php  $i++; } }
// if no fmr exits then
else {?>

    <li class=""><div class="noFMRstyle"><b>No FMR exists for selected media type.</b></div></li>
    <?php }?>
</ul>

<!-- Chekcing FMR LIST Array exits or not for pagintion -->
<?php
if (!empty($arrViewClick)) {?>

<div class="paginationDC">
    <!-- Chekcing previous or back button  -->
    <?php
    if ($intPrev == 0) {?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',1)"><img src= '<?php echo url('sites/all/themes/threebl/images/first_ico.png',array('absolute' => TRUE));?>' align='absmiddle'></a></lable>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('prev')"><img src= '<?php echo url('sites/all/themes/threebl/images/prev_ico.png',array('absolute' => TRUE));?>' align='absmiddle'></a></lable>
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
    if ($intPage - 2 >= 1 ) { ?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage - 2; ?>)"><?php echo $intPage -2; ?></a> </lable>
        <?php } if(($intPage - 1) >= 1 ) { ?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage - 1; ?>)"><?php echo $intPage -1; ?></a> </lable>
        <?php } ?>
    <lable> <a href="javascript:void(0);" class="active-page"><?php echo $intPage; ?></a> </lable>

    <!-- Chekcing for current pages between first and last -->
    <?php }else {
    if (($intPage - 1) >= 1) { ?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage - 1; ?>)"><?php echo $intPage - 1; ?></a> </lable>
        <?php } ?>
    <lable> <a href="javascript:void(0);" class="active-page"><?php echo $intPage; ?></a> </lable>
    <?php if (($intPage + 1) <= $intLastPage ) { ?>
        <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('',<?php echo $intPage + 1; ?>)"><?php echo $intPage + 1; ?></a> </lable>

        <!-- Chekcing for next last button -->
        <?php } } if ($intNext == 0) {?>
    <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('next')"><img src= '<?php echo url('sites/all/themes/threebl/images/next_ico.png',array('absolute' => TRUE));?>' align='absmiddle'></a></lable>
    <lable> <a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('last')"><img src= '<?php echo url('sites/all/themes/threebl/images/last_ico.png',array('absolute' => TRUE));?>' align='absmiddle'></a></lable>
    <?php }?>
</div>
<?php } ?>
