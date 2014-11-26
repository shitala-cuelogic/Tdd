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
                                        <div class="brdCont brdcontdiv"> <span class="frstLink"><a href="javascript:void(0);" class="fistLink crsdefault">  TRACKING SUMMARY </a></span> </div>
                                    </div>
                                </li>
								<li><div class="feildselect">
                                	Please select from the list of your recent FMRs, below, in order to see Tracking results.  Results are available for FMRs distributed after December 18, 2012.  A partial list is displayed.  An expanded capture of results is available for download. Your 3BL Consultant is available to discuss, as always.</div></li>
                                <li class="traking-search" >
                                    <div>
                                        <span class="fmrselect"><strong>Select FMR:</strong></span>
                                        <select name="fmr" id="fmr" class="width550" onchange="jQuery('#frmSub').submit()">
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
                                <li class="downloadtracking">
                                    <strong>Download Tracking Results (Expanded)</strong>
                                    <a title="Export of Affiliates in Excel" href="<?php echo $base_url;?>/Dashboard/Analytics/Tracking/Excel/?fmr=<?php echo base64_encode($selectedFMR); ?>">
                                        <img src='<?php echo url("$strImagePath/excel.png", array('absolute' => true)); ?>' align='absmiddle'>
                                    </a>
                                    <a title="Export of Affiliates in PDF" href="<?php echo $base_url;?>/Dashboard/Analytics/Tracking/PDF/?fmr=<?php echo base64_encode($selectedFMR); ?>">
                                        <img src='<?php echo url("$strImagePath/PDF.png", array('absolute' => true)); ?>' align='absmiddle'>
                                    </a>
                                </li>
                                <?php } ?>
                                <li class="trackingresultli">&nbsp;</li>
                                <li class="textalignleft">
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
												foreach($arrAffiliates as $arrAff){
                                                    $arrImageDetails = @getimagesize($base_url."/media/styles/thumbnail/public/affiliates/".$arrAff['filename']);
                                                    //checking image exist or not.
                                                  if(!empty($arrImageDetails)){?>
												<div class="views-row views-row-odd views-row-first mht">
													<div class="views-field ">
                                                    <div class="fl imgmargin">
                                                        <a href="<?php if($arrAff['type'] == 'widget') {
                                                                          echo (strstr($arrAff['field_affiliate_news_url_value'], '?') == false) ? $arrAff['field_affiliate_news_url_value']."?mid=".$selectedFMR : $arrAff['field_affiliate_news_url_value'] . "&mid=" . $selectedFMR;
                                                                      } else {
                                                                          echo $arrAff['tracking_url'];} ?>" target="_blank">
                                                            <img src="<?php echo $base_url."/media/styles/thumbnail/public/affiliates/".$arrAff['filename'] ?>" />
                                                        </a>
                                                    </div>
														<span class="field-content"><a class="title-popup" href="<?php if ($arrAff['type'] == 'widget') {
                                                            echo (strstr($arrAff['field_affiliate_news_url_value'], '?') == false) ? $arrAff['field_affiliate_news_url_value']."?mid=".$selectedFMR : $arrAff['field_affiliate_news_url_value'] . "&mid=" . $selectedFMR;
                                                        } else {
                                                            echo $arrAff['tracking_url'];}?>" target="_blank"><?php echo $arrAff['field_affiliate_title_value']; ?></a></span>
													</div>
													<div class="views-field views-field-field-fmr-date-time">
														<div class="field-content">
										<div class="date-display-single">
											<?php echo $arrAff['field_affiliate_description_value']; ?>
											<?php if((int)$arrAff['field_affiliate_monthly_traffic_value'] > 0 ){ ?> <div class="mtraffic"><strong>Monthly Traffic: </strong><?php echo $arrAff['field_affiliate_monthly_traffic_value'];?></div> <?php }?>
                                    </div>
                                </div>
                            </div>  
                        </div><div class="clr">&nbsp;</div>
                                                <?php  } }
						    }
						 else
						 {
						 	?>
                             <div class="views-row views-row-odd views-row-first mht textaligncenter">
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
          <div id="fmrsearch" class="searchfmr"> </div>
        </div>
      </div>
    </div>
  </div>
</div>
