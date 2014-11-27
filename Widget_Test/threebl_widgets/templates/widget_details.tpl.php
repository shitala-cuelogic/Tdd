<?php
extract($data); extract($settings);
//Company URL
$strCompanyURL = $path."/company-clicks/".$widget_id."/".base64_encode($arrCompany[$intFmrId]);

//Load Company information using node_load
$objCompany = node_load($arrCompany[$intFmrId]);

//Company Title
$strCompanyName = $objCompany->title;
?>
<style type="text/css">
    <?php echo str_replace(array("\n", "\r"), '', addslashes(html_entity_decode($widget_css)));?>
    #threeblmediadetaillist .threebl_description .paddingbotom{padding-bottom: 10px;}
    #threeblmediadetaillist .fmrImg .width245{width: 245px; text-align: center;}
</style>

<div>
<div class="threebl_title-box">
    <h1><?php echo addslashes($arrContentList['title']); ?></h1>
</div>
<div class="threebl_datetime"><?php echo $arrContentList['date']; ?></div>
<?php if($arrContentList['displayTwitter'] == "block"){ ?>
<div id="fmr-tweet">
    <img src="<?php echo $path?>/sites/all/themes/threebl/images/tweet-box-icon.gif" id="fmr-tweet-icon">
    <span id="fmr-tweet-tweetme-text">tweet me:</span>
    <div class="field threebl_field-name-field-fmr-tweet field-type-text field-label-hidden">
        <div class="field-items">
            <div class="field-item even"><?php echo $arrContentList['twitter'][0]['value'];?></div>
        </div>
    </div>
    <a target="_blank" title="Click this box to share this with your twitter account" href="<?php echo "http://twitter.com/share?text=".urlencode($arrContentList['twitter'][0]['value'])."&url=";?>">
        <div id="fmr-tweet-overlay"></div>
    </a>
</div>
    <?php }?>
<div class="back-to-headlines">
    <div id="back"><a href="<?php echo $widget_viewmore_link?>" class="threebl_back_to_HeadLines">Back to headlines</a></div>
    <div>Source:&nbsp;<a class="threebl_company" href="<?php echo $strCompanyURL; ?>" target="_blank"><?php echo addslashes($strCompanyName);?></a></div>

<?php // Add a link to the campaign if it exists
 if ($arrContentList['campaignTitle']) { ?>
<div class="threebl_campaign threebl_company">This content is part of the campaign: <a class="threebl_company" href="<?php print $arrContentList['campaignUrl'];?>" target="_blank"><?php print $arrContentList['campaignTitle'];?></a></div> 
<?php }?>
</div>
<hr />

<?php if(is_array($arrContentList['blogUrl'])) {

    $strBlogUrl = $arrContentList['blogUrl']['url'];
    $strBlotTitle = $arrContentList['blogUrl']['title'];

    if($strBlogUrl!=''){
    //Checking http or https in url;
    $arrParseUrl = parse_url($strBlogUrl);
    $strScheme =  ($arrParseUrl['scheme']=='')? "http://":"";
    $strBlogUrl = $strScheme.$strBlogUrl;
    }//end if blog url

    ?>
<div class="threebl_description">
    <?php if($strBlogUrl != '' && $strBlotTitle != '') {?>

        <a href="<?php echo $strBlogUrl;?>" target="_blank"><?php echo addslashes($strBlotTitle);?></a>

    <?php } else if( $strBlotTitle != '') {

        echo $strBlotTitle;

    } else if($strBlogUrl != '') { ?>

        <a href="<?php echo $strBlogUrl;?>" target="_blank"><?php echo $strBlogUrl;?></a>

    <?php } ?>

</div>
    <?php }//is array ?>

<div class="threebl_description"><?php echo addslashes($arrContentList['description']); ?></div>

<?php if(is_array($arrContentList['resourceLink'])) {?>

<div class="threebl_description"><p>Resource:</p>
    <?php
    //foreach start
    foreach($arrContentList['resourceLink'] as $arrResource) {
        $strResourceUrl = $arrResource['url'];
        $strResourceTitle = $arrResource['title'];

        if($strResourceUrl!='') {
        //Checking http or https in url;
        $arrParseRUrl = parse_url($strResourceUrl);
        $strResourceScheme =  ($arrParseRUrl['scheme']=='')? "http://":"";
        $strResourceUrl = $strResourceScheme.$strResourceUrl;
        }//end if 

        ?>
        <div>
            <?php if($strResourceUrl!="" && $strResourceTitle!="") {?>

            <a href="<?php echo $strResourceUrl;?>" target="_blank"><?php echo addslashes($strResourceTitle);?></a>

            <?php } else if($strResourceTitle!="") {

            echo $strResourceTitle;

        } else if($strResourceUrl !="" ) {?>

            <a href="<?php echo $strResourceUrl;?>" target="_blank"><?php echo $strResourceUrl;?></a>
            <?php }?>

        </div>
        <?php  }//foreach end  ?>

    <div><?php echo $arrContentList['resourceText'];?></div>

</div>
    <?php }//if ?>

<?php if($arrContentList['contactName']!='') {?>

<div class="threebl_description">
    <p><b>Contact:</b></p>

    <div class="paddingbotom"><span><?php echo addslashes($arrContentList['contactName']);?></span></div>

    <div class="paddingbotom">
        <?php if($arrContentList['contactPhone']['number']!='') {?>

        <div><span>+1-<?php  echo $arrContentList['contactPhone']['number'];
            if($arrContentList['contactPhone']['extension']!='') {
                echo " ext. ".$arrContentList['contactPhone']['extension'];
            }?>
       </span> </div>

        <?php }?>
    </div>

    <div class="paddingbotom"><span><a href="mailto:<?php echo $arrContentList['contactEmail'];?>"><?php echo $arrContentList['contactEmail'];?></a></span></div>

    <div class="paddingbotom"><span><?php echo addslashes($arrContentList['contactOrganization']);?></span></div>

    <div class="paddingbotom">
        <?php
        if(is_array( $arrContentList['contactOther'])) {
            foreach( $arrContentList['contactOther'] as $arrContact) {?>
                <div><?php echo addslashes($arrContact['value']);?></div>
            <?php }
        }?>
    </div>
</div>
<?php }

//count image counter greater than zero if yes then display image slider.
$intCountImage = count($arrContentList['photo']);
if($intCountImage > 0 ) { ?>
<div style="display:<?php echo $arrContentList['playImage'];?>;" class="fmrImg margin20">
    <p>Image:</p>
    <!-- Image slider id -->
    <div id="slides">
        <div class="slides_container">
            <?php foreach($arrContentList['photo'] as $strContentList) {?>
            <div class="width245">
                <img src="<?php echo urldecode($strContentList['photolink']);?>"  alt="<?php echo $strContentList['name'];?>"></div>
            <?php } ?>
        </div>
        <?php //count image counter greater than one if yes then display next and prev images.
        if($intCountImage > 1) { ?>
            <a href="javascript:void(0);" class="prev"></a>
            <a href="javascript:void(0);" class="next"></a>
        <?php } ?>
    </div>
</div>
<?php }
$intVideoWidth = 640; $intVideoHeight = 264; $strNextStyle =''; $strPrevStyle=''; $strSlidesContainer ='';

if($widget_video_width!='' ) {

    //set dynamic width and height of video
    $intVideoWidth = (int)$widget_video_width;
    $intVideoHeight = ceil(($widget_video_width * 9)/16);

    //set dynamic top and left of next button depending on video.
    $intNextButtonLeft = $intVideoWidth + 5;
    $intNextButtonTop =  ceil($intVideoHeight/2);

    $strNextStyle = 'style="left:'.$intNextButtonLeft.'px ;'.'top:'.$intNextButtonTop.'px"';
    $strPrevStyle = 'style="top:'.$intNextButtonTop.'px"';

    //slides container height set by dynamic
    $intSlidesContainerHeight = $intVideoHeight + 45;
    $strSlidesContainer = 'style=" height:'.$intSlidesContainerHeight.'px"';
}

//Text area style
$strEmbedCodestyle= 'style="width:'.$intVideoWidth.'px;resize:none;height:auto"';

//count video counter greater than zero if yes then display video slider.
$intCountVideo = count($arrContentList['video']); if($intCountVideo > 0 ) {?>
<div style="display:<?php echo $arrContentList['playVideo'];?>;" class=" frmvideo margin20">
    <p>Video:</p>
    <!-- Video slider id -->
    <div id="videoslides">
        <div class="slides_container" <?php echo $strSlidesContainer;?> >
            <?php $i=0; foreach($arrContentList['video'] as $arrVideo){?>
            <!-- Each video div id + videocounter -->
            <div id="video<?php echo $i; ?>">
                <video id="example_video_<?php echo $i;?>" class="video-js vjs-default-skin" controls preload="none" width="<?php echo $intVideoWidth;?>" height="<?php echo $intVideoHeight;?>" poster="<?php echo $arrVideo['videothumb'];?>"
                       data-setup="{}">
                    <source src="<?php echo $arrVideo['videourl'];?>" type="video/mp4" />
                    <track kind="captions" src="captions.vtt" srclang="en" label="English" />
                </video>
                <?php if($arrVideo['videothumb']!='') {
                $strVideo =str_replace('http://3blmedia.com/media/videos/thumbnails/','',$arrVideo['videothumb']);
                $arrVideoId = explode("/",$strVideo);
                ?>
                <div id="embedcode<?php echo $i;?>"><a href="javascript:void(0);" onclick="javascript:document.getElementById(\'videourl<?php echo $i;?>\').style.display = \'block\'; document.getElementById(\'embedcode<?php echo $i;?>\').style.display = \'none\';">Get Embed Code</a>
                </div>
                <div id="videourl<?php echo $i;?>" style="display: none;">
                    <textarea <?php echo $strEmbedCodestyle;?> rows="1" readonly="readonly"><iframe src="http://3blmedia.com/embed/video/<?php echo $arrVideoId[0];?>" scrolling="no" width="300" height="300"></iframe></textarea></div>
                <?php } ?>
            </div>

            <?php $i++; } ?>
        </div>
        <?php //count video counter greater than one if yes then display next and prev images.
        if(($intCountVideo) > 1) {?>
            <a href="javascript:void(0);" class="prev" <?php echo $strPrevStyle;?> ></a>
            <a href="javascript:void(0);" class="next" <?php echo $strNextStyle;?> ></a>
        <?php } ?>
    </div>
</div>
<?php }  ?>

<div style="display:<?php echo $arrContentList['playAudio'];?>;" class="fmrAudio margin20">
    <?php  //count audio counter greater than one if yes then display next and prev images.
    $intCountAudio = $arrContentList['audio'];
    if($intCountAudio > 0) {?>
        <p>Audio:</p>
        <?php foreach($arrContentList['audio'] as $strContentList) {
            //display audio-player for each audio.
            ?>
            <div class="threebl_clr">
                <object type="application/x-shockwave-flash" data="<?php echo url('sites/all/libraries/soundmanager/swf/zplayer.swf',array('absolute' => TRUE));?>?mp3=<?php echo $strContentList['audioLink'];?>" width="200" height="20">
                    <param name="movie" value="<?php echo url('sites/all/libraries/soundmanager/swf/zplayer.swf',array('absolute' => TRUE));?>?mp3=<?php echo $strContentList['audioLink'];?>"/>
                </object>
            </div>
        <?php }
    } ?>
</div>
<div class="threebl_clr"></div>
<div id="back1"><a href="<?php echo $widget_viewmore_link?>" class="threebl_back_to_HeadLines">Back to headlines</a></div>
<?php if((int)$widget_poweredBy > 0) { ?>
<span><a href="<?php echo $path?>" class="threebl_link" target="_blank">Powered By 3BLMedia.com</a></span>
    <?php }?>
</div>

