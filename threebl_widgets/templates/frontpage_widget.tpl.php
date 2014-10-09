<?php extract($data);
extract($settings);
//Set front page title limit
$intTitLt = 53;
if ($widget_frontpageTitle_limit > 0) {
    $intTitLt = $widget_frontpageTitle_limit;
}
?>
<div>
    <h2 class="threebl_title"><?php echo $widget_front_title;?></h2>
</div>

<style type="text/css">
    <?php echo str_replace(array("\n", "\r"), '', $widget_css);?>
    #threeblmediawidget ul.threebl_sidebar .clearboth{clear: both}
</style>
<div style="width:<?php echo (isset($widget_width))?$widget_width."px":"0px"; ?>; height:<?php echo (isset($widget_height))?$widget_height."px":"0px"; ?>;">
    <ul class="threebl_sidebar">
        <?php foreach($arrContentList as $arr){
        $objCompany = node_load($arrCompany[$arr->nid]);
        $strCompanyName = $objCompany->title;
        $objCompanyNode = node_load($arrCompanyName[$arr->nid]['cnid']); // Company Object

        //If user selected front page thumbnail check box.
        if (isset($boolFrontPageThumb) && $boolFrontPageThumb == 1) {
            $strPhotoLink ='';
            if (!empty($objCompanyNode)) {
                $strPhotoLink = urldecode(image_style_url('thumbnail', "Clients/". basename(file_create_url($objCompanyNode->field_client_logo['und'][0]['uri']))));
            }
            $href = (strstr($widget_viewmore_link,'?') == false)?$widget_viewmore_link."?mid=".$arr->nid:$widget_viewmore_link."&mid=".$arr->nid;
            $objFMRNode = node_load($arr->nid);

            //for photos
            if(array_key_exists('und',$objFMRNode->field_fmr_photo)){
                if (is_array($objFMRNode->field_fmr_photo['und']) && count($objFMRNode->field_fmr_photo['und'])) {
                    foreach ($objFMRNode->field_fmr_photo['und'] as $k => $arrPhoto) {
                        $ObjFMRFile = file_load($arrPhoto['fid']);
                        #Assign Amazon photo path
                        $strFMRPhotoLink = urldecode(image_style_url('thumbnail', "images/". basename(file_create_url($ObjFMRFile->uri))));
                        $strFilePath = @file_get_contents($strFMRPhotoLink);
                        #Check image exist or not
                        if( $strFilePath == ""){
                            continue;
                        } else {
                            #if image exist then just assign that FMR photo path to news and break loop
                            $strPhotoLink = $strFMRPhotoLink;
                            break;
                        }
                    }//for
                }//if array
            }//array key exists
            $intFrontPageThumbWidth = $intFrontPageThumbWidth."px";
        }
        ?>
        <li>

            <?php if(isset($strPhotoLink) && $strPhotoLink!='' && $boolFrontPageThumb == 1){?>
            <div class="fmrImageDiv thumbnailWidth">
                <img src="<?php echo $strPhotoLink;?>">
            </div>
            <?php } ?>
            <div class="rightdivwidth">
                <h2><a
                    href="<?php echo (strstr($widget_viewmore_link, '?') == false) ? $widget_viewmore_link . "?mid=" . $arr->nid : $widget_viewmore_link . "&mid=" . $arr->nid;?>" <?php echo  $widget_fmr_in_tab; ?>
                    title="<?php echo addslashes($strCompanyName);?>" class="threebl_fmr_headlines"><?php echo
                addslashes(substr($arr->title, 0, $intTitLt)); if (strlen(trim($arr->title)) > ($intTitLt + 1)) {
                    echo "...";
                }?></a></h2>
                <?php
                #Checking the limit of front page description.
                if ($widget_frontpage_desc_limit > 0) {
                    $strFMRDesc = trim(strip_tags(addslashes($arr->description)));
                    #Get the short description as per the limit.
                    $strShortDesc = substr($strFMRDesc, 0, $widget_frontpage_desc_limit);
                    $strAppendDots = "";
                    if (strlen($arr->description) > $widget_frontpage_desc_limit) {
                        #If the description has more content than the limit then append the ...
                        $strAppendDots = "...";
                    }
                    $strShortDesc .= $strAppendDots;
                    ?>
                    <div
                        class="threebl_frontdesc"><?php echo $strShortDesc; ?>
                    </div>
                    <?php }  ?>

                <div class="threebl_datetime">
                    <?php echo date("F d, Y",strtotime($arr->date)); ?>
                </div>
            </div>
            <div class="clearboth"></div>

        </li>
        <?php } if ($widget_viewmore_text !="") { ?>
        <li><a href="<?php echo $widget_viewmore_link; ?>" class="threebl_vew_more"><?php echo $widget_viewmore_text; ?></a></li>
        <?php } ?>
    </ul>
    <?php if((int)$widget_poweredBy > 0){ ?>
    <span class="threebl_link"><a href="<?php echo $path?>" target="_blank" >Powered By 3BLMedia.com</a></span>
    <?php } ?>
</div>