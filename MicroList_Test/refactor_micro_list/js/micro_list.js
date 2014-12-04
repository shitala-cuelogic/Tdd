// JavaScript Document
jQuery(document).ready(function(){
    //form validation
    jQuery('#frmClientEmail').validationEngine();
    jQuery('#edit-edit-field-email-list-details-und-all-clients').click(function(){
        jQuery("input[type='checkbox']").attr('checked')
    });
});

/**
 * Function to to check or uncheck checkbox
 * @constructor
 */
function SelectDeselect()
{
    if (document.getElementById('chkSelectDeselectAll').checked) {
        jQuery("INPUT[type='checkbox']").attr('checked',true);
    } else {
        jQuery("INPUT[type='checkbox']").attr('checked',false);
    }
}

/**
 * Function to to check or uncheck checkbox
 */
function delSelectCheckbox()
{
    if (!jQuery('[id^="emailid"]').is(':checked')) {
        if (jQuery('#chkSelectDeselectAll').is(':checked')) {
            jQuery("#chkSelectDeselectAll").attr('checked',false);
        }
    }
}

/**
 * Function to Validate checkbox before remove subscriber
 * @return {*}
 */
function fnCheckValidation()
{
    if (!jQuery('[id^="emailid"]').is(':checked')) {
        jQuery("#errormsg").append("No record selected to remove.");
        setTimeout( "jQuery('#errormsg').empty();",3000);
        return false;
    } else {
        return fnConfimration(1);
    }

}//end of function.

/**
 * Function to Alert Confirmation box before delete
 * @param intFlag
 * @return {*}
 */
function fnConfimration(intFlag){

    message = (intFlag==1)?"Are you sure to remove entry(s)?":"Are you sure to delete this Email List?";
    var boolStr = confirm(message);
    return boolStr;
}

/**
 * Function to get List data by Ajax
 * @param string action : Prev or Last
 * @param int intPageNumber : Page Number
 */
function clientMicroListAjax(action, intPageNumber){
    var type = jQuery('#mediatype').val();
    var page = jQuery('#pagenumber').val();
    var last = jQuery('#last').val();
    var searchVal = jQuery('#searchVal').val();

    if (action=='next') {
        page = parseInt(page) + 1;
    } else if (action=='prev') {
        page = parseInt(page)-1;
    } else if (action =='last') {
        page = last;
    } else {
        page = parseInt(intPageNumber);
    }
    jQuery("#clientlist").html("<div style='width:100%; text-align:center; margin: 10px 0'><img src='"+Drupal.settings.basePath +"sites/all/themes/threebl/images/justmeans/metrics-ajax-loader.gif'></div>");
    jQuery.post('/Dashboard/custom-list-ajax', {type:type,page:page,searchKey:searchVal},function(data){
            jQuery('#pagenumber').val(page);
            jQuery('#clientlist').html(data);
            jQuery('#searchVal').val(searchVal);
        }
    );
}

/**
 * Checking File Type
 * @return {Boolean}
 */
function fnCheckFileType()
{
    var ext = $('#emailfile').val().split('.').pop().toLowerCase(); // Getting file type.
    if (ext =='') {
        return false;
    }
    //Checking file type
    if ($.inArray(ext, ['xls','csv']) == -1) {
        jQuery("#errormsg").html("Please upload only .xls or .csv file.");
        return false;
    } else {
        return true;
    }
}

function fnResetForm() {
    jQuery("#strdate").val("");
    jQuery("#enddate").val("");
}