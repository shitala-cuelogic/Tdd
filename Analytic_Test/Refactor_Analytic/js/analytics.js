// JavaScript Document
/**
 * Function toggle chart and other information on the basis of click and view button click
 * @param strMediaType : FMR media type
 * @param intClickFlage: flag indicate display click data or view data
 *
 *
 */
function showChart(strMediaType, intClickFlage)
{
    if (strMediaType == "views") {
        jQuery("#viewsli").show();
        jQuery("#clicksli").hide();
        jQuery("#viewtab").addClass('viewactive');
        jQuery("#clicktab").removeClass('viewactive');
        jQuery("#geograph").hide();
        jQuery("#referlink").hide();

    } else {

        if (intClickFlage > 0) {
            jQuery("#clicksli").css("visibility", "visible");
            jQuery("#clicksli").css("height", "296px");
        }
        jQuery("#viewsli").hide();
        jQuery("#clicksli").show();
        jQuery("#clicktab").addClass('viewactive');
        jQuery("#viewtab").removeClass('viewactive');
        jQuery("#geograph").show();
        jQuery("#referlink").show();
    }
}
/**
 * For ajax pagination
 * @param action : pagination action
 * @param pagenumber : current page number
 */
function displayMediaInfoAjax(action, pagenumber)
{
    var type = jQuery('#mediatype').val();
    var page = jQuery('#pagenumber').val();
    var last = jQuery('#last').val();

    if (action == 'next') {
        page = parseInt(page) + 1;
    } else if (action == 'prev') {
        page = parseInt(page) - 1;
    } else if (action == 'last') {
        page = last;
    } else {
        page = parseInt(pagenumber);
    }

    jQuery("#mediaresult").html("<div style='width:100%; text-align:center;'><img src='" + Drupal.settings.basePath + "sites/all/themes/threebl/images/justmeans/metrics-ajax-loader.gif'></div>");

    jQuery.post(Drupal.settings.basePath + 'Dashboard/Analytics/Views/mediatype/ajax', {type: type, page: page}, function (data) {
            jQuery('#pagenumber').val(page);
            jQuery('#mediaresult').html(data);
        }
    );
}

jQuery(document).ready(function () {

    jQuery(".benchmark, .benchmarkfmr").hover(function () {
        jQuery(".openme1", this).stop(true, true).fadeIn("slow");
    }, function () {
        jQuery(".openme1", this).stop(true, true).fadeOut("slow");
    });

    jQuery(".trigger").click(function () {
        jQuery(this).toggleClass("active");
        jQuery(this).parent('li').find(".toggle_container").slideToggle();
    });

    jQuery("#tgleDiv").click(function () {
        if (jQuery(this).hasClass("togle")) {
            jQuery(this).attr('class', "toglehd");
        } else {
            jQuery(this).attr('class', "togle");
        }
        var objDom = jQuery(this).parent('div');
        objDom.parent('li').find(".toggle_container").slideToggle();
    });

    jQuery("#tgleDiv1").click(function () {
        if (jQuery(this).hasClass("togle")) {
            jQuery(this).attr('class', "toglehd");
        } else {
            jQuery(this).attr('class', "togle");
        }
        var objDom = jQuery(this).parent('div');
        objDom.parent('li').find(".toggle_container").slideToggle();
    });

  /**
  * We use the initCallback callback
  * to assign functionality to the controls
  */
  //JQuery Slider function for Country name and Click Count list.
  function mycarousel_initCallback(carousel)
  {
        jQuery('.jcarousel-control a').bind('click', function () {
            carousel.scroll(jQuery.jcarousel.intval(jQuery(this).text()));
            return false;
        });

        jQuery('.jcarousel-scroll select').bind('change', function () {
            carousel.options.scroll = jQuery.jcarousel.intval(this.options[this.selectedIndex].value);
            return false;
        });

        jQuery('#mycarousel-next').bind('click', function () {
            carousel.next();
            return false;
        });

        jQuery('#mycarousel-prev').bind('click', function () {
            carousel.prev();
            return false;
        });
  };

    //JQuery Slider function for Refferal website
    function mylink_initCallback(carousel)
    {
        jQuery('.jcarousel-control a').bind('click', function () {
            carousel.scroll(jQuery.jcarousel.intval(jQuery(this).text()));
            return false;
        });

        jQuery('.jcarousel-scroll select').bind('change', function () {
            carousel.options.scroll = jQuery.jcarousel.intval(this.options[this.selectedIndex].value);
            return false;
        });

        jQuery('#mylink-next').bind('click', function () {
            carousel.next();
            return false;
        });

        jQuery('#mylink-prev').bind('click', function () {
            carousel.prev();
            return false;
        });
    } ;

    // Ride the carousel...
        jQuery("#mycarousel").jcarousel({
            scroll: 1,
            initCallback: mycarousel_initCallback,
            // This tells jCarousel NOT to autobuild prev/next buttons
            buttonNextHTML: null,
            buttonPrevHTML: null
        });

        jQuery("#mylink").jcarousel({
            scroll: 1,
            initCallback: mylink_initCallback,
            // This tells jCarousel NOT to autobuild prev/next buttons
            buttonNextHTML: null,
            buttonPrevHTML: null
        });
});