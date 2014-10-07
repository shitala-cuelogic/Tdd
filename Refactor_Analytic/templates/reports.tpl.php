<?php
global $base_url, $conf;
$strSelect = '';

//Create Media Types list.
$strOptions = "<option value=''> Select Media Type</option>";
foreach ($arrShowMediaType as $key => $val) {
    if ($key != "all") {
        $strOptions .= "<option value='$key' {$strSelect}> $val </option>";
    }
}

#Create Campaign list
$strCampaignOptions = "<option value=''> Select Campaign </option>";
foreach ($objCampaign as $record) {
    if ($record->campaign != "") {
        $strCampaignOptions .= "<option value='" . $record->campaignId . "'> " . $record->campaign . " </option>";
    }
}

#Prepare Company FMRs list
$strFMROptions = "<option value=''> Select FMR </option>";
foreach ($objFMR as $record) {
    if ($record->nid != "") {
        $strFMROptions .= "<option value='" . $record->nid . "'> " . $record->title . " </option>";
    }
}
?>
<!-- Page CSS -->
<style type="text/css">
    .fr {
        float: right;
    }

    .trackingtitle {
        background: #E9F1F2 !important;
        padding: 0 0 0 11px !important;
        cursor: default !important;
    }

    #rpt ul li.sub {
        line-height: 30px;
        padding-left: 25px !important;
        margin: 5px 10px;
        background: none !important;
        clear: both;
    }

    #rpt ul li {
        line-height: 30px;
        padding-left: 5px !important;
        margin: 5px 10px;
        clear: both;
    }

    .bgTxt {
        background: url("/sites/all/themes/threebl/images/next_ico.png") no-repeat scroll left center transparent;
        padding-left: 20px
    }

    .selectTP {
        margin: 0 10px;
        width: 475px
    }

    #rpt {
        font-weight: bold;
        color: #00AACC
    }

    .imgs {
        cursor: pointer;
    }

    .error {
        color: #FF0000;
    }

    .hide {
        display: none;
    }

    .note1 {
        color: #565C61;
    }

    .lineHt {
        line-height: 20px
    }

    .sumtxt {
        line-height: 20px;
        font-weight: normal;
        color: #565C61;
        padding-left: 20px;
        width: 500px
    }

        /*popup*/
    .popupbox {
        background: url("/sites/all/themes/threebl/images/justmeans/light_boxbg.png") repeat scroll center center #F9F9F9;
        border: 1px solid #ABBBC7;
        border-radius: 5px 5px 5px 5px;
        padding: 10px;
        width: 550px;
        position: fixed;
        z-index: 10000;
        top: 100px
    }

    .popupbox .popupclose {
        background: url("/sites/all/themes/threebl/images/justmeans/close.png") repeat-x scroll center center transparent;
        cursor: pointer;
        display: block;
        height: 21px;
        position: absolute;
        right: -11px;
        top: -11px;
        width: 21px
    }

    .popupbox .popupboxContents {
        background: none repeat scroll 0 0 #FFFFFF;
        border: 1px solid #FFFFFF;
        border-radius: 5px 5px 5px 5px;
        height: 100%;
        overflow: visible;
        padding: 5px
    }

    .overlay {
        background: #000000;
        bottom: 0;
        left: 0;
        position: fixed;
        right: 0;
        top: 0;
        z-index: 1000;
        opacity: 0.5
    }

    .lbl {
        width: 75px;
        float: left;
        color: #2C2C2B
    }

    .popupfrm input[type="text"], .popupfrm textarea {
        width: 420px;
    }

    .popupfrm textarea {
        resize: none
    }

    #popuptxt {
        line-height: 17px !important;
        font-weight: normal;
        margin: 10px
    }

    .popupbuttons {
        text-align: right;
        padding-right: 10px !important
    }

    .textalignleft {
        text-align: left !important;
    }

    .margin0 {
        margin: 0 !important;
    }
</style>

<div class="content">
    <div id='blMainWrapper'>
        <div id='blWrapper'>
            <div id="analyticsWrapper">
                <div class="rightSideCont"><?php echo $strTabHtml;?>
                    <div class="detailsContainer">
                        <div class="expTabsContainer">
                            <div class="brdCont"><span class="frstLink"><a class="fistLink"
                                                                           href="<?php echo $base_url;?>/Dashboard/Analytics/Views">CONTENT
                                REPORTS</a></span>
                            </div>
                        </div>
                        <div id="popupoverlay" class="overlay" style="display:none"></div>
                        <ul class="analyticsTrigger">
                            <li><p class="note1">Below you can click on PDF and Excel icons to download various
                                Analytics and Tracking reports for the content you have distributed through 3BL
                                Media.</p>&nbsp;</li>
                            <li class="textalignleft">
                                <h2 class="trigger trackingtitle clr "><strong> Summary Reports </strong></h2>
                            </li>
                            <li>&nbsp; </li>
                            <li id="rpt">
                                <ul class="report-ul">
                                    <li>
                                        <div>
                                            <div class="fl">
                                                <span class="bgTxt">Analytics Summary Report</span>

                                                <p class="sumtxt">A summary of views and clicks over the past six
                                                    months.</p>
                                            </div>
                                            <div class="fr"><img align="absmiddle"
                                                                 src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL'] ?>/PDF.png"
                                                                 class="imgs"
                                                                 onclick="javascript:return window.location= '<?php echo $base_url;?>/Dashboard/Report/PDF/MCSR';"
                                                                 title="PDF Analytics Summary Report">&nbsp;&nbsp;<img
                                                align="absmiddle"
                                                src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/email.png"
                                                class="imgs" title="Email Analytics Summary Report"
                                                onclick="javascript: fnOpenEmailDialog(this,'MCSR');"/>
                                            </div>
                                        </div>
                                        <p>&nbsp;</p>
                                    </li>
                                    <li><span class="bgTxt">Analytic Summary by Media Type</span>

                                        <p class="sumtxt">A summary of views and clicks over the past six months for a
                                            given media type.</p></li>
                                    <li class="sub lineHt">
                                        <div>
                                            <div class="fl">
                                                <form name="frmCSMT" id="frmCSMT">
                                                    <select class="form-select selectTP validate[required]"
                                                            name="media_type" id="media_type">
                                                        <?php echo $strOptions;?>
                                                    </select><br/>

                                                    <p class="error hide">Please Select Media</p>
                                                </form>
                                            </div>
                                            <div class="fr">&nbsp;&nbsp;<img align="absmiddle"
                                                                             src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/PDF.png"
                                                                             class="imgs"
                                                                             onclick="javascript:return mediaReport('pdf');"
                                                                             title="PDF Analytic Summary by Media Type">&nbsp;&nbsp;<img
                                                align="absmiddle"
                                                src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/excels.png"
                                                class="imgs" onclick="javascript:return mediaReport('excl');"
                                                title="Excel Analytic Summary by Media Type">&nbsp;&nbsp;<img
                                                align="absmiddle"
                                                src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/email.png"
                                                class="imgs" title="Email Analytic Summary by Media Type"
                                                onclick="javascript: fnOpenEmailDialog(this,'CSMT');"/>

                                            </div>
                                        </div>
                                        <p>&nbsp;</p>
                                    </li>
                                    <li><span class="bgTxt">Analytics Summary by Campaign</span>

                                        <p class="sumtxt">A summary of views and clicks over the past six months for a
                                            given campaign.</p></li>
                                    <li class="sub">
                                        <div>
                                            <div class="fl">
                                                <form name="frmCSC" id="frmCSC">
                                                    <select class="form-select selectTP validate[required]"
                                                            name="campaign_type" id="campaign_type">
                                                        <?php echo $strCampaignOptions;?>
                                                    </select>
                                                </form>
                                            </div>
                                            <div class="fr">
                                                &nbsp;&nbsp;<img align="absmiddle"
                                                                 src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/PDF.png"
                                                                 class="imgs"
                                                                 onclick="javascript:return campaignReport('pdf');"
                                                                 title="PDF Analytics Summary by Campaign"/>&nbsp;&nbsp;<img
                                                align="absmiddle"
                                                src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/excels.png"
                                                class="imgs" onclick="javascript:return campaignReport('excl');"
                                                title="Excel Analytics Summary by Campaign">&nbsp;&nbsp;<img
                                                align="absmiddle"
                                                src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/email.png"
                                                class="imgs" title="Email Analytics Summary by Campaign"
                                                onclick="javascript: fnOpenEmailDialog(this,'CSC');"/>
                                            </div>
                                        </div>
                                        <br/>
                                        <br/>
                                    </li>
                                </ul>
                            </li>
                            <li>&nbsp;</li>

                            <li>&nbsp;</li>
                            <li class="textalignleft">
                                <h2 class="trigger trackingtitle clr "><strong>FMR Reports</strong></h2>
                            </li>
                            <li id="rpt">
                                <ul class="report-ul">
                                    <li>
                                        <div>
                                            <div class="fl"><span class="bgTxt">Detailed Analytics for all FMRs</span>

                                                <p class="sumtxt">This is a complete listing of FMRs with date, title,
                                                    distribution network(s), campaign (when available), media type,
                                                    views and clicks.</p></div>
                                            <div class="fr">&nbsp;&nbsp;<img align="absmiddle"
                                                                             src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/excels.png"
                                                                             class="imgs"
                                                                             onclick="javascript:return window.location= '<?php echo $base_url;?>/Dashboard/Analytics/Views/Excel';"
                                                                             title="Excel Detailed Analytics for all FMRs">&nbsp;&nbsp
                                            </div>
                                        </div>
                                    </li>
                                    <li><span class="bgTxt">FMR Detail Report</span>

                                        <p class="sumtxt">Views, clicks, geographic breakdown and referring sites for a
                                            given FMR.</p></li>
                                    <li class="sub">
                                        <div>
                                            <div class="fl">
                                                <form name="frmFMR" id="frmFMR">
                                                    <select class="form-select selectTP validate[required]" name="fmr"
                                                            id="fmr">
                                                        <?php echo $strFMROptions; ?>
                                                    </select>
                                                </form>
                                            </div>
                                            <div class="fr">
                                                &nbsp;&nbsp;<img align="absmiddle"
                                                                 src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/PDF.png"
                                                                 class="imgs" title="PDF FMR Detail Report"
                                                                 onclick="javascript:return FMRReport('pdf');">&nbsp;&nbsp;<img
                                                align="absmiddle"
                                                src="<?php echo $base_url;?>/<?php echo $conf['IMAGES_PATH_3BL']; ?>/email.png"
                                                class="imgs" title="Email FMR Detail Report"
                                                onclick="javascript: fnOpenEmailDialog(this,'FMR');">
                                            </div>
                                        </div>
                                        <p>&nbsp;</p>
                                    </li>
                                    <li><span class="bgTxt">FMR Tracking Report</span>

                                        <p class="sumtxt">Please use the <a
                                            href="<?php echo $base_url;?>/Dashboard/Analytics/Tracking">Tracking tab</a>
                                            for this report.</p></li>
                                    <li class="sub lineHt">&nbsp;
                                    <li>
                                    <li>
                                        <div style="display:none" class="popupbox" id="divPopup">
                                            <div class="popupboxContents margin0">
                                                <h2 class="trigger trackingtitle clr " id="popuptitle"></h2>

                                                <div id="popuptxt" class="note1"></div>
                                                <form name="frmECSMT" id="frmECSMT"
                                                      action="<?php echo $base_url;?>/Dashboard/Analytics/Reports"
                                                      method="post">
                                                    <ul class="popupfrm">
                                                        <li>
                                                            <div class="lbl">Email:</div>
                                                            <input type="text" name="email" id="email"
                                                                   class="form-text validate[required,custom[email]]"/>
                                                        </li>
                                                        <li>
                                                            <div class="lbl">Subject:</div>
                                                            <input class="form-text validate[required]" type="text"
                                                                   name="subject" id="subject"/></li>
                                                        <li>
                                                            <div class="lbl">Message:</div>
                                                            <textarea class="form-textarea" rows="4" name="msg"
                                                                      id="msg"></textarea>
                                                            <input type="hidden" name="sendemail" id="sendemail"
                                                                   value=""/>
                                                            <input type="hidden" name="mediaTypOrId"
                                                                   id="mediaTypIDorFMRId" value=""/>
                                                        </li>
                                                        <li class="popupbuttons"><input class="form-submit"
                                                                                        type="submit" name="submit"
                                                                                        value="Send"
                                                                                        onclick="fnResetValidate('frmECSMT');"/>
                                                            <input class="form-submit" type="button" name="cancel"
                                                                   value="Cancel"
                                                                   onclick="javascript: fnCloseEmailDialog();"/></li>
                                                    </ul>
                                                </form>
                                            </div>
                                            <div onclick="javascript: fnCloseEmailDialog();" class="popupclose"></div>
                                        </div>
                                    </li>
                                </ul>
                            </li>
                        </ul>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>