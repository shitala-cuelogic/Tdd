<style type="text/css">
    .fmrsearchcls{width: 680px; margin-top: 13px}
    .font12{font-size: 12px}
</style>
<div class="content">
  <div id='blMainWrapper'>
    <div id='blWrapper'>
      <div id="analyticsWrapper">
        <div class="rightSideCont"><?php echo $strTabHtml;?>
          <div class="detailsContainer"> 
            <ul class="analyticsTrigger">
                <li>
                 <div class="gray-line">
                 <a href="<?php echo url('Dashboard/Analytics/Views/Excel',array('absolute' => true)); ?>" title="Export of Analytics">
                     <img src= '<?php echo url('sites/all/themes/threebl/images/justmeans/excel.png',array('absolute' => TRUE)); ?>' align='absmiddle'>
                     </a>
                     <a href="<?php echo url('Dashboard/Analytics/Views/Excel',array('absolute' => true)); ?>" title="Export of Analytics"><strong>Export of Analytics</strong></a>
                     <div class="toggle_container_top"><p class="note1">This is a complete listing of the prior 6 months of FMRs with date, title, campaign (when available), media type, views and clicks. To download, click on the icon or title, above.</p></div>
                     </div>
                      <div class="toggle_container font12">
                     This module will include automated and custom reporting with email notification.
                     <p class="reportBold">Release date: week of January 14, 2013.</p></div>
                </li> 
              <li> &nbsp;
                <div class="clr">&nbsp;</div>
              </li>
            </ul>
          </div>
          <div id="fmrsearch" class="fmrsearchcls"> </div>
        </div>
      </div>
    </div>
  </div>
</div>
