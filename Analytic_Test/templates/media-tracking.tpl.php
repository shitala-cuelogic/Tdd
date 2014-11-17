<style type="text/css">
	.pging{margin-top: 10px; height:20px}
	.mht{min-height: 100px}
	.imgmargin{margin:10px; height: 100px; width: 100px;}
	.trackingtitle{background:#E9F1F2 !important; padding: 0 0 0 11px !important; cursor:default !important;}
	.mtraffic{margin-top:5px;}
</style>

<div class="content">
    <div id='blMainWrapper'>
        <div id='blWrapper'>
            <div id="analyticsWrapper">
                <div class="rightSideCont"><?php echo $strTabHtml;?>
                    <div class="detailsContainer">
                        <form name="frmSub" id="frmSub" method="get" action="">
                            <ul class="analyticsTrigger">
                            	<li>
                                    <div class="expTabsContainer">
                                        <div class="brdCont" style="margin: 0 0 5px"> <span class="frstLink"><a href="javascript:void(0);" class="fistLink" style="cursor:default">  TRACKING SUMMARY </a></span> </div>
                                    </div>
                                </li>
								<li><div style="float: left; font-size: 12px; text-align: left; color: rgb(86, 92, 97); margin: 0px 5px 25px;">
                                	Please select from the list of your recent FMRs, below, in order to see Tracking results.  Results are available for FMRs distributed after December 18, 2012.  A partial list is displayed.  An expanded capture of results is available for download. Your 3BL Consultant is available to discuss, as always.</div></li>
                                <li class="traking-search" >
                                    <div>
                                        <span style="width:101px !important; padding-left:5px !important"><strong>Select FMR:</strong></span>
                                        <select name="fmr" id="fmr" style="width:550px" onchange="jQuery('#frmSub').submit()">
                                            <?php if(!empty($arrLatestFmr)) {
                                                foreach($arrLatestFmr as $strFmr)
                                                {?>
                                                    <option value="<?php echo base64_encode($strFmr->nid);?>" <?php if((int)$selectedFMR == (int)$strFmr->nid){echo 'selected="selected"';}?> ><?php echo $strFmr->title; ?></option>
                                                <?php }
                                            } ?>
                                        </select>
                                    </div>
                                </li>
                                <?php if(count($arrAffiliates) > 0){?>
                                <li style="text-align: right; padding-right:30px;">
                                    <a title="Export of Affiliates" href="<?php echo $base_url;?>/Dashboard/Analytics/Tracking/Excel/?fmr=<?php echo (int)$selectedFMR; ?>">
                                    <img src= '<?php echo url('sites/all/themes/threebl/images/justmeans/excel16x16.png',array('absolute' => TRUE)); ?>' align='absmiddle'>
                                    <strong>Download Tracking Results (Expanded)</strong>
                                    </a>
                                </li>
                                <?php } ?>
                                <li style="text-align: right; border-top: 2px solid #F4F4F4; margin-top: 10px;">&nbsp;</li>
                                <li style="text-align: left; ">  
                                <h2 class="trigger trackingtitle clr " > <strong>Tracking Results (Partial)</strong></a> </h2>
                                </li>                                
                                <li>
                                    <div class="view view-news-pages view-id-news_pages view-display-id-page ">
                                        <div class="view-content">
                                            <?php
                                            /**
                                             * List out all affiliates related to selected FMR
                                             */
											 if(count($arrAffiliates) > 0){
												foreach($arrAffiliates as $arrAff){ ?>
												<div class="views-row views-row-odd views-row-first mht">
													<div class="views-field ">
														<div class="fl imgmargin"><a href="<?php if($arrAff->affiliate_type=='widget'){echo $arrAff->affiliate_news_url."?mid=".$selectedFMR;}else{echo $arrAff->tracking_url;} ?>" target="_blank"><img src="<?php echo $base_url; ?>/sites/default/files/affiliate_logo/scale/<?php echo ($arrAff->logo)?$arrAff->logo:"noimage.png"; ?>" /></a></div>
														<span class="field-content"><a class="title-popup" href="<?php if($arrAff->affiliate_type=='widget'){echo $arrAff->affiliate_news_url."?mid=".$selectedFMR;}else{echo $arrAff->tracking_url;} /*echo $widget_viewmore_link.'&vc='.base64_encode($arrAff->aff_id);*/ ?>" target="_blank"><?php echo $arrAff->title; ?></a></span>
													</div>
													<div class="views-field views-field-field-fmr-date-time">
														<div class="field-content">
										<div class="date-display-single">
											<?php echo $arrAff->description; ?>
											<?php if((int)$arrAff->monthly_traffic > 0 ){ ?> <div class="mtraffic"><strong>Monthly Traffic: </strong><?php echo $arrAff->monthly_traffic;?></div> <?php }?>
                                    </div>
                                </div>
                            </div>  
                        </div><div class="clr">&nbsp;</div>
                        <?php } 
						 }
						 else
						 {
						 	?>
                             <div class="views-row views-row-odd views-row-first mht" style="text-align:center">
                             	<strong><?php echo (!empty($arrLatestFmr))? "No data available for the selected FMR": "No FMRs have been distributed since December 18, 2012."; ?></strong>
                             </div>
                         <?php } ?>
                    </div>  
                </div>
               </li>
               <li><div class="clr">&nbsp;</div></li>
            </ul>
            </form>
          </div>
          <div id="fmrsearch" style="width:680px;margin-top:13px"> </div>
        </div>
      </div>
    </div>
  </div>
</div>
