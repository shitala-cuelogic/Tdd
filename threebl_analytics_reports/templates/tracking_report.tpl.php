<style type="text/css">
    #searchengine{width: 216px}
    .width500 {width: 500px}
    .hieght22{height: 22px}
    #fmrsearch{width: 680px; margin-top: 13px}
</style>
<div class="content">
  <div id='blMainWrapper'>
    <div id='blWrapper'>
      <div id="analyticsWrapper">
        <div class="rightSideCont"><?php echo $strTabHtml;?>
          <div class="detailsContainer"> 
          
             <form method="get" action="http://www.google.com/search" target="_blank" name="frmForm" id="frmForm"> 
             <input type="hidden" name="query" id="query"  size="120" />
             </form>
             
             <form method="get" action="http://twitter.com/search" target="_blank" name="frmFormTwitter" id="frmFormTwitter"> 
             <input type="hidden" name="q" id="q"  size="120" />
             </form>
            
            <ul class="analyticsTrigger">
              <li class="traking-search">
              <div>
               <span><strong>Search Engine:</strong></span>
              <select name="searchengine" id="searchengine">
                 <option name="google">Google</option>
                 <option name="yahoo">Yahoo</option>
                 <option name="bing">Bing</option>
                 <option name="twitter">Twitter</option>
               </select>
              </div>
              </li>

              <li class="traking-search" >
              <div>
              <span><strong>FMR List:</strong></span>
              <select name="frm" id="fmr" class="width500" onchange="getFmr(this.value);">
              <option value="">Select FMR</option>
                <?php if (!empty($arrLatestFmr)) {
					   foreach ($arrLatestFmr as $strFmr) {?>
                        <option value="<?php echo $strFmr->nid;?>"><?php echo $strFmr->title; ?></option>
                  <?php } } ?>
                 
               </select>
              </div>
              </li>
              
              <li class="traking-search">
              <div>
              <span>&nbsp;&nbsp;</span>
              <input type="text" name="search" id="keysearch" size="65" height="28px" class="hieght22" disabled="disabled" />&nbsp; &nbsp;
              <input type="button" name="btsearch" id="btsearch" disabled="disabled" value="Search" onclick="getSearch();"  class="search-button"  />
              </div>
              </li>

               <li class="traking-search"></li>
 
              <li> &nbsp;
                <div class="clr">&nbsp;</div>
              </li>
            </ul>
          </div>
          <div id="fmrsearch"> </div>
        </div>
      </div>
    </div>
  </div>
</div>
