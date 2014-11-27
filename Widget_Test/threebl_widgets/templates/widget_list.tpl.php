<?php extract($data); extract($settings); ?>

<div>
    <h2 class="threebl_title"><?php echo $widget_list_title;?></h2>
</div>
<style type="text/css">
    <?php echo str_replace(array("\n", "\r"), '', $widget_css);?>
    .pglink{border: 1px solid #00AACC;margin: 5px;padding: 1px 3px;text-decoration: none;}
    ul.threebl_sidebar .threebl_teaser .maniDiv{width: 100% ;}
    ul.threebl_sidebar .threebl_teaser .maniDiv .fmrDetailsDiv {width:80%; float:right;}
    ul.threebl_sidebar .threebl_teaser .maniDiv .fmrImageDiv{float:left;width:20% ;}
    ul.threebl_sidebar .threebl_teaser .maniDiv .fmrImageDiv img {max-width: 100% ;}
    ul.threebl_sidebar .threebl_teaser .contentypediv{float: left; padding: 5px;}
    ul.threebl_sidebar .threebl_teaser .datetimediv{float: left; margin-bottom: 10px ;}
    ul.threebl_sidebar .threebl_teaser .clearboth {clear: both ;}

</style>
<link rel="stylesheet" href="<?php echo url('sites/all/themes/threebl/css/widget_paging.css',array('absolute' => TRUE));?>" />
<ul class="threebl_sidebar">
    <?php
    //List page FMR description character limit
    $intDescLt = 200;
    if($widget_listpageDesc_limit > 0) {
        $intDescLt = $widget_listpageDesc_limit;
    }

    $intNoOfPages = (int) ($intCnt / $intLimit);
    $nextLink  = '';
    $prevLink  = '';
    $firstLink = '';
    $lastLink  = '';
    if($intPageNo < $intNoOfPages) {
        $nextLink  = (strstr($widget_viewmore_link,'?') == false)?$widget_viewmore_link."?pgno=".($intPageNo+1):$widget_viewmore_link."&pgno=".($intPageNo+1);
        $lastLink = (strstr($widget_viewmore_link,'?') == false)?$widget_viewmore_link."?pgno=".$intNoOfPages:$widget_viewmore_link."&pgno=".$intNoOfPages;
    }
    if($intPageNo > 1) {
        $prevLink = (strstr($widget_viewmore_link,'?') == false)?$widget_viewmore_link."?pgno=".($intPageNo-1):$widget_viewmore_link."&pgno=".($intPageNo-1);
        $firstLink = (strstr($widget_viewmore_link,'?') == false)?$widget_viewmore_link."?pgno=1":$widget_viewmore_link."&pgno=1";
    }

    foreach($arrContentList as $arr) {
        $strCompanyURL = $path."/company-clicks/".$widget_id."/".base64_encode($arrCompany[$arr->nid]);
        $strCompanyName = ucwords($arrCompanyName[$arr->nid]['cname']); //Company Title
        $objCompanyNode = node_load($arrCompanyName[$arr->nid]['cnid']); // Company Object

        $strPhotoLink ='';
        if(!empty($objCompanyNode)) {
            $strPhotoLink = urldecode(image_style_url('thumbnail', "Clients/". basename(file_create_url($objCompanyNode->field_client_logo['und'][0]['uri']))));
        }
        $href = (strstr($widget_viewmore_link,'?') == false)?$widget_viewmore_link."?mid=".$arr->nid:$widget_viewmore_link."&mid=".$arr->nid;
        $objFMRNode = node_load($arr->nid);

        // for photos
        if(array_key_exists('und',$objFMRNode->field_fmr_photo)) {
            if (is_array($objFMRNode->field_fmr_photo['und']) && count($objFMRNode->field_fmr_photo['und'])) {
                foreach ($objFMRNode->field_fmr_photo['und'] as $k => $arrPhoto) {
                    $ObjFMRFile = file_load($arrPhoto['fid']);
                    #Assign Amazon photo path
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
        }//array key exists
        ?>
        <li>
            <h2><a href="<?php echo $href; ?>" <?php echo  $widget_fmr_in_tab; ?>class="threebl_fmr_headlines" title="<?php echo addslashes($strCompanyName);?>"><?php echo addslashes($arrFMRContentList[$arr->nid]['title']);?></a></h2>
            <div class="threebl_teaser">

                <div>
                    <div class="contentypediv"><span class="threebl_fl"><?php echo(strtolower($arr->field_fmr_type_of_content_value) == 'press_release')?'Press Release ':ucwords($arr->field_fmr_type_of_content_value); ?> from &nbsp;</span><a href="<?php echo addslashes($strCompanyURL); ?>" class="threebl_company_name" target="_blank"><?php echo addslashes($strCompanyName); ?></a></div>
                    <div class="datetimediv"><span class="threebl_datetime">&nbsp;|&nbsp;&nbsp;<?php echo date("F d, Y",strtotime($arr->date)); ?></span></div>
                </div>


                <div class="maniDiv">
                    <div class="threebl_desc16 fmrDetailsDiv"><?php echo substr(trim(strip_tags(addslashes($arrFMRContentList[$arr->nid]['description']))),0,$intDescLt);if(strlen(trim($arrFMRContentList[$arr->nid]['description'])) > ($intDescLt+1)){echo "...";} ?> <a href="<?php echo $href; ?>" class="threebl_read_more">Read More</a></div>
                    <?php if($strPhotoLink!=''){?>
                    <div class="fmrImageDiv">
                        <img src="<?php echo $strPhotoLink;?>" >
                    </div>
                    <?php } ?>
                </div>
                <div class="clearboth"></div>

            </div>
        </li>
    <?php }?>
</ul>
<div class="threebl_pagination">
    <?php if($prevLink != '') { ?>
    <a class="first" href="<?php echo $firstLink; ?>">&nbsp;</a> <a class="prev" href="<?php echo $prevLink; ?>">&nbsp;</a>
    <?php } ?>
    <span><?php echo $intPageNo; ?></span>
    <?php if($nextLink != '') { ?>
    <a class="next" href="<?php echo $nextLink; ?>">&nbsp;</a> <a class="last" href="<?php echo $lastLink; ?>">&nbsp;</a>
    <?php } ?>
</div>
<?php if((int)$widget_poweredBy > 0) {?>
<span class="threebl_link"><a href="<?php echo $path; ?>" target="_blank">Powered By 3BLMedia.com</a></span>
<?php }?>