// JavaScript Document
jQuery(document).ready(function(){


    //Function for my group clicks
    jQuery("#edit-group-audience-und-my-groups-input").click(function(){
        var isChecked = jQuery("#edit-group-audience-und-my-groups-input").is(':checked');

        if (isChecked) {
            jQuery("input[name^='group_audience[und][my_groups]']").attr('checked', true);
        } else {
            jQuery("input[name^='group_audience[und][my_groups]']").attr('checked', false);
        }
    });

    //Function for other group clicks
    jQuery("#edit-group-audience-und-other-groups-input").click(function(){

        var isChecked = jQuery("#edit-group-audience-und-other-groups-input").is(':checked');

        if (isChecked) {
            jQuery("input[name^='group_audience[und][other_groups]']").attr('checked', true);
        } else {
            jQuery("input[name^='group_audience[und][other_groups]']").attr('checked', false);
        }

    });

    jQuery.post('/Dashboard/micro-list-admin-ajax', {},function(data){
            if (data) {
                var arrData = JSON.parse(data);
                jQuery.each(arrData, function( index, value ){
                    intDivId = this.gid;
                    jQuery(".form-item-group-audience-und-my-groups-"+intDivId).hide();
                    jQuery(".form-item-group-audience-und-other-groups-"+intDivId).hide();

                });
            }

        }
    );

    // ajax function call to get the login user created and assign micro lists.
    jQuery.post('/Dashboard/get-client-micro-lists-ajax', {intflag:1},function(data){
            if (data) {
                var arrData = JSON.parse(data);
                jQuery.each(arrData, function( index, value ){
                    //$('p:not(.go-1)').hide();
                    //jQuery('.form-item-field-my-micro-list-und-'+value).remove();
                    //jQuery('.).hide();
                });

            }
            var current_name = jQuery('input[name="field_social_media_distribution[und][0][value]"]').val();

            if (current_name == 1) {
                jQuery("#edit-field-my-micro-list input[type=checkbox]").each(function(){
                   //jQuery(this).attr('disabled','disabled');
                });
            }
        }
    );
});


