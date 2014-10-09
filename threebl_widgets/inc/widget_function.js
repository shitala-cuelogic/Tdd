var protocol = ('https:' == location.protocol ? 'https://' : 'http://');
var path = protocol + '3blmedia.com/sites/all/';
var basePath = protocol + '3blmedia.com/';

if (!window.jQuery) {
    document.write("<script language='javascript' src='" + path + "themes/threebl/js/jquery.js'><\/script>");
}

/**
 * Function initiate request for widget respective page data for old widget
 * @param token : this is unique widget token
 * @param page : widget page type
 */
function init_widget(token, page) {
    var mid = getUrlVars()["mid"];
    var pgno = getUrlVars()["pgno"];
    if (!pgno) {
        pgno = 1;
    }
    var surl = basePath + "3bl_widgets_data.jsv?t=" + token + "&p=" + page + "&md=" + mid + "&pg=" + pgno;
    callPage(surl);
}
//init_widget_function.

/**
 * Function initiate request for widget respective page data for new widget
 * @param token : this is unique widget token
 * @param page : widget page type
 */
function init_widget_new(token, page) {
    var mid = getUrlVars()["mid"];
    var pgno = getUrlVars()["pgno"];
    if (!pgno) {
        pgno = 1;
    }
    var nw = 1;
    var surl = basePath + "3bl_widgets_data.jsv?t=" + token + "&p=" + page + "&md=" + mid + "&pg=" + pgno + "&nw=" + nw;
    callPage(surl);
}
/**
 * Function parse url and get the parameters for widget
 * @return {Object}
 */
function getUrlVars() {
    var vars = {};
    var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function (m, key, value) {
        vars[key] = value;
    });
    return vars;
}
/**
 * Include video js and load widget page news data.
 * @param surl
 */
function callPage(surl) {
    if (!window.VideoJS) {
        document.write("<script language='javascript' src='" + path + "themes/threebl/js/video.js'><\/script>");
    }

    document.write("<script type='text/javascript' src='" + surl + "'><\/script>");
    jQuery(document).ready(function () {
        //jQuery('audio,video').mediaelementplayer();
    });
}