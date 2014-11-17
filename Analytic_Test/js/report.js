// JavaScript Document
jQuery(document).ready(function()
{
    // binds form submission and fields to the validation engine
    jQuery("#frmCSMT").validationEngine();
    jQuery("#frmCSC").validationEngine();
    jQuery("#frmFMR").validationEngine();
    jQuery("#frmECSMT").validationEngine();

    //Function manage FMR Media type report actions
    mediaReport = function (Rtype, mdtyp){
        fnResetValidate('CSMT');
        if (jQuery("#frmCSMT").validationEngine("validate", "#media_type") == true) {
            var mdType = jQuery("#media_type").val();
            if ( mdType != "") {
                if (Rtype == 'pdf') {
                    window.location = "http://"+document.domain+"/Dashboard/Report/PDF/CSMT/"+mdType;
                } else if (Rtype == 'excl') {
                    window.location = "http://"+document.domain+"/Dashboard/Report/Excel/CSMT/"+mdType;
                } else if (Rtype == 'email') {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    //Function manage FMR report actions
    FMRReport= function (Rtype){
        fnResetValidate('FMR');
        if (jQuery("#frmFMR").validationEngine("validate", "#fmr") == true) {
            var fmrId = jQuery("#fmr").val();
            if ( fmrId != "") {
                if (Rtype == 'pdf') {
                    window.location = "http://"+document.domain+"/Dashboard/Report/PDF/FMR/"+fmrId;
                } else if (Rtype == 'excl') {
                    window.location = "http://"+document.domain+"/Dashboard/Report/Excel/FMR/"+fmrId;
                } else if (Rtype == 'email') {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    //Function manage campaign report actions
    campaignReport = function(Rtype){
        fnResetValidate('CSC');
        if (jQuery("#frmCSC").validationEngine("validate", "#campaign_type")) {
            var cmpType = jQuery("#campaign_type").val();
            if ( cmpType != "") {
                if (Rtype == 'pdf') {
                    window.location = "http://"+document.domain+"/Dashboard/Report/PDF/CSC/"+cmpType;
                } else if (Rtype == 'excl') {
                    window.location = "http://"+document.domain+"/Dashboard/Report/Excel/CSC/"+cmpType;
                } else if (Rtype == 'email') {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
});

/**
 * Function reset validation messages.
 * @param id
 * @return {Boolean}
 */
function fnResetValidate(id) {
    if (id == 'CSMT') {
        jQuery("#frmCSC").validationEngine('hide');
        jQuery("#frmFMR").validationEngine('hide');
        jQuery("#frmECSMT").validationEngine('hide');
    } else if (id == 'CSC') {
        jQuery("#frmCSMT").validationEngine('hide');
        jQuery("#frmFMR").validationEngine('hide');
        jQuery("#frmECSMT").validationEngine('hide');
    } else if (id == 'FMR') {
        jQuery("#frmCSMT").validationEngine('hide');
        jQuery("#frmCSC").validationEngine('hide');
        jQuery("#frmECSMT").validationEngine('hide');
    } else if (id == 'frmECSMT') {
        jQuery("#frmCSMT").validationEngine('hide');
        jQuery("#frmCSC").validationEngine('hide');
        jQuery("#frmFMR").validationEngine('hide');
    } else {
        jQuery("#frmCSMT").validationEngine('hide');
        jQuery("#frmCSC").validationEngine('hide');
        jQuery("#frmFMR").validationEngine('hide');
        jQuery("#frmECSMT").validationEngine('hide');
    }
    return true;
}

/*
* Function to close Email Dialog box
 */
function fnCloseEmailDialog(e)
{
    fnResetValidate('');
    jQuery('#divPopup').hide();
    jQuery('#popupoverlay').removeClass("overlay");
}

/**
 * Function manage email popup box
 * @param e
 * @param typ
 */
function fnOpenEmailDialog(e, typ)
{
    fnResetValidate(typ);
    var flag = true;
    jQuery('#sendemail').val(typ);
    var strType = '';
    var strSub = '';
    if (typ == 'CSMT') {
        flag = mediaReport('email');
        var mediaType = jQuery('#media_type').val();
        var mediaTypeTxt = jQuery('#media_type option:selected').text();
        jQuery('#mediaTypIDorFMRId').val(mediaType);
        strType = 'Analytic Summary by Media Type';
        strSub = '3BL Media - Analytics Summary by Media Type: ' + mediaTypeTxt;
    }
    if (typ == 'CSC') {
        flag = campaignReport('email');
        var campaignType = jQuery('#campaign_type').val()
        var campaignTypeTxt = jQuery('#campaign_type option:selected').text();
        jQuery('#mediaTypIDorFMRId').val(campaignType);
        strType = 'Analytics Summary by Campaign';
        strSub = '3BL Media - Analytics Summary by Campaign: ' + campaignTypeTxt;
    }
    if (typ == 'FMR') {
        flag = FMRReport('email');
        var FMRId = jQuery('#fmr').val();
        jQuery('#mediaTypIDorFMRId').val(FMRId);
        strType = 'FMR Detail Report';
        strSub  = '3BL Media - FMR Report';
    }
    if (typ == 'MCSR') {
        strType = 'Analytics Summary Report';
        strSub  = '3BL Media - Analytics Summary Report';
    }
    if (typ == 'BADF') {
        strType = 'Detailed Analytics for all FMRs';
        strSub  = 'Detailed Analytics for all FMRs';
    }
    var txt = 'Enter an email address to which to send the ' + strType + '.  Adding a message is optional.  Check your spam folder if it doesn\'t show up in your inbox; then add 3blmedia.com to your contacts.';
    if (flag == true) {
        jQuery('#popuptitle').html(strType);
        jQuery('#popuptxt').html(txt);
        jQuery('#subject').val(strSub);
        jQuery('#divPopup').show();
        jQuery('#popupoverlay').addClass("overlay");
        jQuery('#popupoverlay').show();
        jQuery(e).next().center();
    }
}

jQuery.fn.center = function () {
    this.css("left", ((jQuery(window).width() - this.outerWidth()) / 2) + jQuery(window).scrollLeft() + "px");
    return this;
}
