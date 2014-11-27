<?php extract($data); extract($settings);?>
<style type="text/css">
    <?php echo str_replace(array("\n", "\r"),'',$widget_css); ?>
    .marginauto{margin: auto;width: 980px}
    .marginauto .headerdiv{background-color: #09F;text-align: center;padding-top: 20px;height: 35px;margin-top: 20px;color: #FFFFFF;font-weight: bold;}
    .marginauto .floatleft{float: left;width: 780px}
    #threeblmediadetaillist .threebl_teaser .contenttypediv{float: left;padding: 5px;}
    #threeblmediadetaillist .threebl_teaser .datetimevalue{float: left;margin-bottom: 10px}
    #threeblmediadetaillist .threebl_teaser .width100{width: 100%}
    #threeblmediadetaillist .threebl_teaser .width100 .width80{width: 80%;float: right}
    #threeblmediadetaillist .threebl_teaser .width100 .fotolinkdiv{float: left;width: 20%;max-width: 100%}
    .marginauto .clearboth{clear: both}
    .marginauto .sidebardiv{background-color: #09F;text-align: center;padding-top: 175px;height: 175px;width: 175px;margin-top: 35px;color: #FFFFFF;font-weight: bold;float: right}
    .marginauto .footerdiv{background-color: #09F;text-align: center;padding-top: 20px;height: 35px;color: #FFFFFF;font-weight: bold;margin-top: 35px;float: left;width: 100%}
</style>

<div class="marginauto">
    <div class="headerdiv">HEADER</div>
    <div>
        <div class="floatleft">
            <div id="threeblmediadetaillist">
                <div>
                    <h2 class="threebl_title"><?php echo $widget_list_title;?></h2>
                </div>
                <ul class="threebl_sidebar">
                <?php
                //List page FMR description character limit
                $intDescLt = 200;
                if($widget_listpageDesc_limit > 0) {
                    $intDescLt = $widget_listpageDesc_limit;
                }

                foreach($arrContentList as $arr) {

                    $strCompanyName = ucwords($arrCompanyName[$arr->nid]['cname']); //Company Title
                    $objCompanyNode = node_load($arrCompanyName[$arr->nid]['cnid']); // Company Object

                    $strPhotoLink ='';
                    if(!empty($objCompanyNode)) {
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
                            if( $strFilePath == "") {
                                continue;
                            } else {
                                #if image exist then just assign that FMR photo path to news and break loop
                                $strPhotoLink = $strFMRPhotoLink;
                                break;
                            }
                        }//for
                    }//if array
                    ?><li>
                        <h2><a href="javascript:;" title="<?php echo addslashes($strCompanyName);?>" class="threebl_fmr_headlines"><?php echo $arrFMRContentList[$arr->nid]['title']; ?></a></h2>
                        <div class="threebl_teaser">
                            <div>
                                <div class="contenttypediv"><span><?php echo(strtolower($arr->field_fmr_type_of_content_value) == 'press_release')?'Press Release ':ucwords($arr->field_fmr_type_of_content_value); ?> from &nbsp;</span><a href="javascript:;" class="threebl_company_name"><?php echo ucwords($strCompanyName); ?></a></div>
                                <div class="datetimevalue"><span class="threebl_datetime">&nbsp;|&nbsp;&nbsp;<?php echo date("F d, Y",strtotime($arr->date)); ?></span></div>
                            </div>

                            <div class="width100">
                                <div class="threebl_desc16 width80"><?php  $strDetails = trim(addslashes(strip_tags($arrFMRContentList[$arr->nid]['description']))); echo substr($strDetails,0,$intDescLt);
                                    if(strlen($arrFMRContentList[$arr->nid]['description']) > ($intDescLt)){
                                        echo "...";
                                    } ?><a href="javascript:;" class="threebl_read_more">Read More</a></div>
                                <?php if($strPhotoLink!=''){?>
                                <div class="fotolinkdiv">
                                    <img src="<?php echo $strPhotoLink;?>" >
                                </div>
                                <?php } ?>
                            </div>
                            <div class="clearboth"></div>
                        </div>
                    </li>
                <?php } ?>
                </ul>
                <?php if((int)$widget_poweredBy > 0) {?>
                <span class="threebl_link">
                    <a href="javascript:;" target="_blank">Powered By 3BLMedia.com</a>
                </span><?php }?>
            </div>
        </div>
        <div class="sidebardiv">SIDE BAR</div>
        <div class="clearboth"></div>
    </div>
    <div class="footerdiv">FOOTER</div>
</div>
<?php exit; ?>