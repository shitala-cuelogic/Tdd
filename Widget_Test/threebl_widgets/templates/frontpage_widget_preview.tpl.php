<?php extract($data);extract($settings); ?>
<style type="text/css">
    <?php echo str_replace(array("\n", "\r"), '', $widget_css);
    //Set front page title limit
    $intTitLt = 53;
    if($widget_frontpageTitle_limit > 0){$intTitLt = $widget_frontpageTitle_limit;}
    ?>
    .marginauto{margin: auto;width: 980px}
    .marginauto .headerdiv{background-color: #09F;text-align: center;padding-top: 20px;height: 35px;margin-top: 20px;color: #FFFFFF;font-weight: bold;}
    .marginauto .footerdiv{background-color: #09F;text-align: center;padding-top: 20px;height: 35px;color: #FFFFFF;font-weight: bold;margin-top: 35px;float: left;width: 100%}
    .marginauto .clearboth {clear: both}
    #threeblmediawidget .clearboth {clear: both}
    .marginauto .pagecontent{background-color: #09F;text-align: center;margin-right: 10px;margin-top: 35px;color: #FFFFFF;font-weight: bold;float: left}
    .marginauto .flotright{float: right;}

</style>
<div class="marginauto">
    <div class="headerdiv">HEADER</div>
    <div>
        <div style=" padding-top:<?php echo (isset($widget_height))?($widget_height/2)."px":"0px"; ?>;width:<?php echo (960-$widget_width-40)."px";?>; height:<?php echo (isset($widget_height))?($widget_height/2)."px":"0px"; ?>; " class="pagecontent">PAGE CONTENT</div>
        <div class="flotright">
            <div id="threeblmediawidget">
                <div style="width:<?php echo (isset($widget_width))?$widget_width."px":"0px"; ?>; height:<?php echo (isset($widget_height))?$widget_height."px":"0px"; ?>;">
                    <div>
                        <h2 class="threebl_title"><?php echo $widget_front_title;?></h2>
                    </div>
                    <ul class="threebl_sidebar">
                        <?php foreach($arrContentList as $arr){
                        $objCompany = node_load($arrCompany[$arr->nid]);
                        $objCompanyNode = node_load($arrCompanyName[$arr->nid]['cnid']);
                        $strCompanyName = $objCompany->title;
                        $strPhotoLink ='';
                        //If user selected front page thumbnail check box.
                        if ($boolFrontPageThumb == 1) {
                            if(!empty($objCompanyNode)){
                                $strPhotoLink = urldecode(image_style_url('thumbnail', "Clients/". basename(file_create_url($objCompanyNode->field_client_logo['und'][0]['uri']))));
                            }

                            $objFMRNode = node_load($arr->nid);

                            // for photos
                            if (is_array($objFMRNode->field_fmr_photo['und']) && count($objFMRNode->field_fmr_photo['und'])) {
                                foreach ($objFMRNode->field_fmr_photo['und'] as $k => $arrPhoto) {
                                    $ObjFMRFile = file_load($arrPhoto['fid']);
                                    $strFilePath = "";

                                    #Assign 3BL photo path
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
                        }
                        ?>
                        <li>
                            <?php if($strPhotoLink!='' && $boolFrontPageThumb == 1){?>
                            <div class="fmrImageDiv thumbnailWidth">
                                <img src="<?php echo $strPhotoLink;?>" >
                            </div>
                            <?php } ?>
                            <div class="rightdivwidth">
                                <h2><a href="javascript:;" title="<?php echo addslashes($strCompanyName);?>" class="threebl_fmr_headlines"><?php echo substr(trim($arr->title),0,$intTitLt);if(strlen(trim($arr->title)) > ($intTitLt+1)){echo "...";} ?></a></h2>
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
                                <div class="threebl_datetime"><?php echo date("F d, Y",strtotime($arr->date)); ?></div>
                            </div>
                            <div class="clearboth"></div>
                        </li>
                        <?php } if ($widget_viewmore_text !="") { ?>
                        <li><a href="javascript:;" class="threebl_vew_more"><?php echo $widget_viewmore_text; ?></a></li>
                        <?php } ?>
                    </ul>
                    <?php if((int)$widget_poweredBy > 0){ ?>
                    <span class="threebl_link" ><a href="javascript:;" target="_blank">Powered By 3BLMedia.com</a></span>
                    <?php } ?>
                </div>
            </div>
        </div>
        <div class="clearboth"></div>
    </div>
    <div class="footerdiv">FOOTER</div>
</div>
<?php exit; ?>
