<style type="text/css">
    .width500{width: 500px}
    .width216{width: 216px}
    .hieght22{height: 22px}
</style>
<div class="content">
  <div id='blMainWrapper'>
    <div id='blWrapper'>
      <div id="analyticsWrapper">
        <div class="rightSideCont"><?php echo $strTabHtml;?>
          <div class="detailsContainer"> 
                        <!-- Form for google searching -->
             <form method="get" action="http://www.google.com/search" target="_blank" name="frmForm" id="frmForm"> 
             <input type="hidden" name="query" id="query"  size="120" />
             </form>
                        <!-- Form for yahoo searching -->
              <form method="get" action="http://in.search.yahoo.com/search" target="_blank" name="frmFormYahoo" id="frmFormYahoo"> 
             <input type="hidden" name="p" id="p"  size="120" />
             </form>
                        <!-- Form for Bing searching -->
              <form method="get" action="http://www.bing.com/search" target="_blank" name="frmFormBing" id="frmFormBing"> 
             <input type="hidden" name="q" id="qbing"  size="120" />
             </form>
            
            <ul class="analyticsTrigger">
              <li class="traking-search">
              <div>
               <span><strong>Search Engine:</strong></span>
              <select name="searchengine" id="searchengine" class="width216">
                 <option name="google">Google</option>
                 <option name="yahoo">Yahoo</option>
                 <option name="bing">Bing</option>
               </select>
              </div>
              </li>
              
              <li class="traking-search" >
              <div>
              <span><strong>FMR List:</strong></span>
              <select name="frm" id="fmr" class="width500" onchange="getFmr(this.value);">
              <option value="">Select FMR</option>
                <?php if (!empty($arrLatestFmr)) {
                                        // check lastes FMR array exits or not.if yes show the FMR Listing in drop-down.
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
        </div>
      </div>
    </div>
  </div>
</div>
