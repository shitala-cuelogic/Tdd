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
              <select name="searchengine" id="searchengine" style="width:216px">
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
              <select name="frm" id="fmr" style="width:500px" onchange="getFmr(this.value);">
              <option value="">Select FMR</option>
                <?php if(!empty($arrLatestFmr)) {
					   foreach($arrLatestFmr as $strFmr)
					   {?>
                        <option value="<?php echo $strFmr->nid;?>"><?php echo $strFmr->title; ?></option>
                  <?php } } ?>
                 
               </select>
              </div>
              </li>
           
              
              <li class="traking-search">
              <div>
              <span>&nbsp;&nbsp;</span>
              <input type="text" name="search" id="keysearch" size="65" height="28px" style="height:22px" disabled="disabled" />&nbsp; &nbsp;
              <input type="button" name="btsearch" id="btsearch" disabled="disabled" value="Search" onclick="getSearch();"  class="search-button"  />
              </div>
              </li>
            
              
               <li class="traking-search"></li>
 
              <li> &nbsp;
                <div class="clr">&nbsp;</div>
              </li>
            </ul>
          </div>
          <div id="fmrsearch" style="width:680px;margin-top:13px"> </div>
        </div>
      </div>
    </div>
  </div>
</div>
