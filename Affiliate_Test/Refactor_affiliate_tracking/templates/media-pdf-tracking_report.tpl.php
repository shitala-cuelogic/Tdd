<style type="text/css">
.trBgcolor{background-color: #f0f5f6}
.DateText{ font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; color: #000000; font-size: 12px; }
.paddingright10{ padding-right: 10px; }
ul.threebl_sidebar{font-family: Tahoma, Geneva, Arial, Helvetica, sans-serif; list-style: none outside none;padding: 0 10px;}
ul.threebl_sidebar li{ padding: 10px 0 1px 10px;}
.maintitle{color: #00aacc; margin-bottom: 0px}
li h2 a.threebl_fmr_headlines{color: #00aacc; font-size: 14px; margin: 0; padding: 0; vertical-align: middle; padding-bottom: 5px;}
ul.threebl_sidebar .threebl_teaser{ margin-top: 5px;}
.threebl_sidebar h2{margin: 0; padding: 0;}
ul.threebl_sidebar .threebl_teaser .maniDiv{width: 100%; margin-left: 115px;}
ul.threebl_sidebar .threebl_teaser .maniDiv .fmrDetailsDiv{width: 80%;}
ul.threebl_sidebar .threebl_teaser .affiliatemaniDiv{ width: 100%; margin-left: 115px; }
ul.threebl_sidebar .threebl_teaser .affiliatemaniDiv .fmrDetailsDiv{ width: 80%; }
.threebl_desc16{margin: 0 0 10px; width: 80% !important; color: #2c2c2b; font-size: 13px; line-height: 16px;}
ul.threebl_sidebar .threebl_teaser .fmrImageDiv{float: left; width: 105px;}
ul.threebl_sidebar .threebl_teaser .fmrImageDiv img{max-width: 100%; display: block !important; float: left;}
ul.threebl_sidebar .threebl_teaser .clearboth{clear: both;}
.mtraffic{color: #848b8e; padding-top: 5px; padding-bottom: 5px;}
.socialmedia{padding-top: 5px; border-top: 1px dotted #D5D5D5; width: 100%;}
.padright5{padding-right: 5px;}
.followus{ color: #848b8e; display: block; float: left;line-height: 15px;padding-right: 5px;}
.imgwidth{width: 20px;}
.floatleft{float: left;}
.categorywidth{width: 250px; padding-left: 5px;}
.break{display: block; clear: both; page-break-after: always;}

</style>

<div>
    <div>&nbsp;</div>
    <h2 class="maintitle"><a href="<?php echo $base_url.'/node/'.$intFMRId; ?>" class="maintitle"><strong><?php echo $strTitle; ?></strong></a></h2>
    <p></p>
        <div class="paddingright10 floatleft DateText ">Distributed on&nbsp;<?php echo $strPublishDate; ?></div>
        <div class="paddingright10 floatleft DateText">Media Type: <?php echo $strGetFMRMediaType; ?></div>
        <div><div class="floatleft DateText">Primary Category: </div> <div class="floatleft categorywidth DateText"> <?php if ($strPrimaryCategoryName != "") { echo $strPrimaryCategoryName; ?><?php } ?></div></div>
</div>

<div style="clear: both;">&nbsp;</div>
<ul class="threebl_sidebar">
    <?php
    #Add Widgets records into the report

    if (!empty($arrWidget)) {

        $intWidgetCount = 0;
        $intTopLiCount = 0;
        $intFirstPage = 0;
        foreach ($arrWidget as $arrWidgetRecord) {
            // Alternate Background color
            $strTrClass = "";
            if ($intWidgetCount % 2 == 0) {
                $strTrClass = "class='trBgcolor'";
            }

            //Widget News Url
            $strNewsWidgetUrl = (strstr($arrWidgetRecord->field_affiliate_news_url_value, '?') == false) ? $arrWidgetRecord->field_affiliate_news_url_value."?mid=".$intFMRId : $arrWidgetRecord->field_affiliate_news_url_value."&mid=".$intFMRId;

           // Widget Publisher
           if ($arrWidgetRecord->field_publisher_value == "") {
               $strWidgetPublisher = $arrWidgetRecord->field_affiliate_title_value;
           } else {
               $strWidgetPublisher = $arrWidgetRecord->field_publisher_value;
           }

           // Publisher Social media accounts
           $strPublisherFacebook = $arrWidgetRecord->field_widget_facebook_value;
           $strPublisherTwitter = $arrWidgetRecord->field_widget_twitter_value;
           $strPublisherLinkedIn = $arrWidgetRecord->field_widget_linkedin_value;
           $strPublisherTumblr = $arrWidgetRecord->field_widget_tumblr_value;
           $strPublisherGoogle = $arrWidgetRecord->field_widget_google_plus_value;
           $strPublisherYouTube = $arrWidgetRecord->field_widget_youtube_value;

           $arrPathInfo = pathinfo($arrWidgetRecord->uri);
           $strFileName = $arrPathInfo['filename'].".".$arrPathInfo['extension'];

        if ($intTopLiCount  == 1) {
            $intTopLiCount = 0;
    ?>
        <li>&nbsp;</li>
    <?php
        }
        
    ?>
        <li <?php echo $strTrClass; ?>>

            <div class="threebl_teaser">
                <div class="fmrImageDiv">
                    <a href="<?php echo $strNewsWidgetUrl ?>" title="<?php echo $strWidgetPublisher; ?>"><img src="<?php echo $base_url."/media/styles/thumbnail/public/affiliates/".$strFileName; ?>"/></a>
                </div>
                <div class="maniDiv">
                    <div class="threebl_desc16 fmrDetailsDiv">
                        <div>
                            <h2><a title="<?php echo $strWidgetPublisher; ?>" class="threebl_fmr_headlines" href="<?php echo $strNewsWidgetUrl; ?>"><?php echo $strWidgetPublisher; ?></a>
                        </h2>
                        </div>
                        <?php echo stripslashes($arrWidgetRecord->field_affiliate_description_value); ?>
                        <div class="mtraffic">
                            <strong>Monthly Traffic: </strong><?php echo ($arrWidgetRecord->field_affiliate_monthly_traffic_value == "" || $arrWidgetRecord->field_affiliate_monthly_traffic_value == 0) ? "N/A" : $arrWidgetRecord->field_affiliate_monthly_traffic_value; ?>
                        </div>
                        <?php if ($strPublisherFacebook != "" || $strPublisherTwitter != "" || $strPublisherLinkedIn != "" || $strPublisherTumblr != "" || $strPublisherGoogle != "" || $strPublisherYouTube) {?>
                        <div class="socialmedia">

                            <span class="followus"><strong>Follow Us: </strong></span>

                            <?php if ($strPublisherFacebook != "") { ?>
                            <span><a href="<?php echo $strPublisherFacebook; ?>" title="Facebook"><img src="<?php echo url("$strImagePath/justmeans/fb_ico.png", array('absolute' => true)); ?>" class="imgwidth"/></a></span>
                            <?php } ?>
                            <?php if ($strPublisherTwitter != "") { ?>
                            <span><a href="<?php echo $strPublisherTwitter; ?>" title="Twitter"><img src="<?php echo url("$strImagePath/justmeans/tw_ico.png", array('absolute' => true)); ?>" class="imgwidth"/></a></span>
                            <?php } ?>
                            <?php if ($strPublisherLinkedIn != "") { ?>
                            <span><a href="<?php echo $strPublisherLinkedIn; ?>" title="LinkedIn"><img src="<?php echo url("$strImagePath/justmeans/ln_ico.png", array('absolute' => true)); ?>" class="imgwidth"/></a></span>
                            <?php } ?>
                            <?php if ($strPublisherTumblr != "") { ?>
                            <span><a href="<?php echo $strPublisherTumblr; ?>" title="Tumblr"><img src="<?php echo url("$strImagePath/justmeans/tm_ico.png", array('absolute' => true)); ?>" class="imgwidth"/></a></span>
                            <?php } ?>
                            <?php if ($strPublisherYouTube != "") { ?>
                            <span><a href="<?php echo $strPublisherYouTube; ?>" title="YouTube"><img src="<?php echo url("$strImagePath/justmeans/yt_ico.png", array('absolute' => true)); ?>" class="imgwidth"/></a></span>
                            <?php } ?>
                            <?php if ($strPublisherGoogle != "") { ?>
                            <span><a href="<?php echo $strPublisherGoogle; ?>" title="Google+"><img src="<?php echo url("$strImagePath/justmeans/gp_ico.png", array('absolute' => true)); ?>" class="imgwidth"/></a></span>
                            <?php } ?>
                        </div>
                        <?php } ?>
                    </div>
                </div>
                <div class="clearboth"></div>
            </div>
        </li>

        <?php
            
            $intWidgetCount ++;
            $intFMRcount = 8; 
            if ($intWidgetCount == 7 && $intFirstPage == 0) {
                $intFMRcount = 7;
                $intWidgetCount = 0;
                $intFirstPage = 1;
            }

            if ($intWidgetCount % $intFMRcount == 0) {
                $intTopLiCount = 1;
        ?>
            <div class="break"></div>
    <?php
            }
            
        }
    }

    if (!empty($arrAffiliates)) {
    //Add PRConnect records into the report.

    $intAffiliateCount = ($intWidgetCount >0 ) ? $intWidgetCount : 0;
    $intAffiliateTopLiCount = 0;

    foreach ($arrAffiliates as $arrPRConnectRecord) {

        // Alternate Background color
        $strAffiliateTrClass = "";
        if ($intAffiliateCount % 2 == 0) {
            $strAffiliateTrClass = "class='trBgcolor'";
        }

        if ($arrPRConnectRecord->type == 'widget') {
            $strNewsPRConnectUrl = (strstr($arrPRConnectRecord->field_affiliate_news_url_value, '?') == false) ? $arrPRConnectRecord->field_affiliate_news_url_value."?mid=".$intFMRId : $arrPRConnectRecord->field_affiliate_news_url_value."&mid=".$intFMRId;

            $strPublisherUrl = $arrPRConnectRecord->field_affiliate_news_url_value;
            $strAffiliateTitle = "<a href='$strPublisherUrl'>".$arrPRConnectRecord->field_affiliate_title_value."</a>";

        } else {
            $strNewsPRConnectUrl = $arrPRConnectRecord->tracking_url;
            $strAffiliateTitle = $arrPRConnectRecord->field_affiliate_title_value;
        }
        $arrAffiliatePathInfo = pathinfo($arrPRConnectRecord->uri);
        $strAffiliateFileName = $arrAffiliatePathInfo['filename'].".".$arrAffiliatePathInfo['extension'];

        if ($intAffiliateTopLiCount == 1) {
            $intAffiliateTopLiCount = 0;
    ?>
        <li>&nbsp;</li>
    <?php
        }
    ?>
        <li <?php echo $strAffiliateTrClass; ?>>
            <div class="threebl_teaser">
                 <div class="fmrImageDiv">
                    <a href="<?php echo $strNewsPRConnectUrl ?>" title="<?php echo $strAffiliateTitle; ?>"> <img src="<?php echo $base_url."/media/styles/thumbnail/public/affiliates/".$strAffiliateFileName; ?>"/></a>
                </div>
                <div class="affiliatemaniDiv">
                    <div class="threebl_desc16 fmrDetailsDiv">
                        <div>
                            <h2><a class="threebl_fmr_headlines" title="<?php echo $strAffiliateTitle; ?>" href="<?php echo $strNewsPRConnectUrl; ?>"><?php echo $strAffiliateTitle; ?></a></h2>
                        </div>
                        <?php echo stripslashes($arrPRConnectRecord->field_affiliate_description_value); ?>
                        <div class="mtraffic">
                            <strong>Monthly Traffic: </strong><?php echo ($arrPRConnectRecord->field_affiliate_monthly_traffic_value == "" || $arrPRConnectRecord->field_affiliate_monthly_traffic_value == 0) ? "N/A" : $arrPRConnectRecord->field_affiliate_monthly_traffic_value; ?>
                        </div>
                    </div>
                </div>
                <div class="clearboth"></div>
            </div>
        </li>
        <?php 
            $intAffiliateCount ++;
            if ($intAffiliateCount % 8 == 0) {
                $intAffiliateTopLiCount = 1;
        ?>
            <div class="break"></div>
    <?php
            }
        }
    }

    if (empty($arrAffiliates) && empty($arrWidget)) {
    ?>
        <li>
            <strong><?php "No data available for the selected FMR"; ?></strong>
        </li>
    <?php
    }
    ?>
</ul>