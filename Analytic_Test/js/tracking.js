// JavaScript Document
function getFmr(id)
{
    id = parseInt(id);
    if (id > 0) {
        var strfmr = jQuery('#fmr option:selected').text();
        jQuery("#keysearch").removeAttr('disabled');
        jQuery("#btsearch").removeAttr('disabled');
        document.getElementById("keysearch").value = strfmr;
    } else {
        document.getElementById("keysearch").value = "";
        jQuery("#keysearch").attr("disabled", "disabled");
        jQuery("#btsearch").attr("disabled", "disabled");
        jQuery("#fmrsearch").html('');
    }
}

/*
* Function to search
 */
function getSearch()
{
    var strengine = jQuery('#searchengine option:selected').val();
    var strfmr = jQuery.trim(jQuery("#keysearch").val());
    if (strfmr.length < 1) {
        jQuery("#keysearch").val('Please enter keyword');
        return false;
    }

    if (strengine=='Google') {
        jQuery("#fmrsearch").html('');
        document.getElementById("query").value = strfmr;
        jQuery('#frmForm').submit();

    } else if (strengine=='Yahoo') {
        jQuery("#fmrsearch").html('');
        document.getElementById("p").value = strfmr;
        jQuery('#frmFormYahoo').submit();

    } else if (strengine=='Bing') {
        jQuery("#fmrsearch").html('');
        document.getElementById("qbing").value = strfmr;
        jQuery('#frmFormBing').submit();

    } else {
        jQuery("#fmrsearch").html('');
        document.getElementById("q").value = strfmr;
        jQuery('#frmFormTwitter').submit();
    }
}

/**
 * To manage popup
 * @param string pageURL : Page Url
 * @param string title : Title
 * @param int w : width
 * @param int h : Height
 * @constructor
 */
function PopupCenter(pageURL, title, w, h)
{
    var left = (screen.width/2)-(w/2);
    var top = (screen.height/2)-(h/2);
    var targetWin = window.open(pageURL, title, 'toolbar=no, location=no, directories=no, status=no, menubar=no, scrollbars=no, resizable=no, copyhistory=no, width='+w+', height='+h+', top='+top+', left='+left);
}
