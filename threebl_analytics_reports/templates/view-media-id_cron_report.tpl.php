<style type="text/css">
.tbl td {
	border:1px solid #ddd
}
.paddCls {
	padding: 5px 0 5px 0;
}
.mainTitleCls {
	font-family:Tahoma, Geneva, Arial, Helvetica, sans-serif;
	color: #00AACC;
	font-size:16px;
	font-weight:bold;
}
.subTitleCls {
	font-family:Tahoma, Geneva, Arial, Helvetica, sans-serif;
	color: #00AACC;
	font-size:14px;
	font-weight:bold;
}
.descCls {
	font-family:Tahoma, Geneva, Arial, Helvetica, sans-serif;
	color:#2C2C2B;
	font-size: 12px;
}
.TitledescCls {
	font-family:Tahoma, Geneva, Arial, Helvetica, sans-serif;
	color:#00AACC;
	font-size: 12px;
	font-weight:bold;
}
.TableTitleCls {
	font-family:Tahoma, Geneva, Arial, Helvetica, sans-serif;
	color: #656565;
	font-size: 12px;
	font-weight: bold;
	padding: 5px 0;
	text-align: right;
	;
	margin-left:10px;
}
.tableTh-value {
	font-family:Tahoma, Geneva, Arial, Helvetica, sans-serif;
	color:#2C2C2B;
	font-size:12px;
	margin-right:5px;
	;
	margin-left:9px;
}
.subTitleBackGround {
	background-color:#E9F1F2;
	padding-bottom:5px;
	width:100%
}
.DateText {
	font-family:Tahoma, Geneva, Arial, Helvetica, sans-serif;
	color: #000000;
	font-size: 11px;
}
.copyright {
	font-family:Tahoma, Geneva, Arial, Helvetica, sans-serif;
	color: #000000;
	font-size: 10px;
	padding-bottom:10px
}
    .clsColor{color: #00AACC}
</style>
<?php  // here we get Click  and View count of selected media id with last click or view date and media type.

global $arrNewMediaType; global $arrShowMediaType;
$arrFMRClickCount =  $arrFMRViewCount = array();

if(count($arrClickCount) > 0){
	foreach($arrClickCount as $row){
		$arrFMRClickCount[$row->nid] = $row->totalcount;
	}
}
if (count($arrViewCount) > 0) {
	foreach ($arrViewCount as $row) {
		$arrFMRViewCount[$row->nid] = $row->totalcount;
	}
}
?>
<table width="98%">
  <tr>
    <td width="90%">
    	<span class="mainTitleCls">3BL Media - FMR Report<br />
		  <?php echo $strCompanyName;?><br />
          <?php echo date('F d, Y'); ?></span>
      </td>
  </tr>
  <?php 
  	// FMR INFORMATION START FROM HERE
  	if (count($arrNodes) > 0) {
  		foreach ($arrNodes as $intFMRId => $arrFMRInfo) {
			$strMediaTitle = $arrFMRInfo->title;
			
			//getting FMR Publish date
			$arrFmrPublish = $arrFMRInfo->field_fmr_date_time;
			$strPublishDate = $arrFmrPublish['und'][0]['value'];
			
			//getting FMR Media type
			$arrFmrType = $arrFMRInfo->field_fmr_type_of_content;
			$strFmrType = $arrFmrType['und'][0]['value'];
  ?>
    <tr>
	    <td width="90%">&nbsp;</td>
    </tr>
  <tr>
    <td width="90%" class="clsColor"><span class="TitledescCls"><?php echo $strMediaTitle;?></span>
      <p class="DateText">Distributed on&nbsp;<?php echo date('M d',strtotime($strPublishDate));?><?php echo date(', Y',strtotime($strPublishDate));?></p></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
    	<div class="subTitleCls subTitleBackGround">Analytics for FMR</div>
      	<div class="paddCls">
      		<p class="descCls">This is the total number of views and clicks on 3blmedia.com for this FMR since it was distributed.</p>
		</div>
      	<table width="50%" class="tbl paddCls" cellpadding="0" cellspacing="0">
        <tr>
          <td width="15%"><span class="TableTitleCls">Views</span></td>
          <td width="15%"><span class="TableTitleCls">Clicks</span></td>
          <td width="20%"><span class="TableTitleCls">Clicks:Views (max = 1)</span></td>
        </tr>
        <tr>
          <td><span class="tableTh-value"><?php echo number_format((int)$arrFMRViewCount[$intFMRId]); ?></span></td>
          <td><span class="tableTh-value"><?php echo number_format((int)$arrFMRClickCount[$intFMRId]); ?></span></td>
          <td><span class="tableTh-value">
            <?php if ((int)$arrFMRViewCount > 0) {  echo number_format((int)$arrFMRClickCount[$intFMRId] /(int)$arrFMRViewCount[$intFMRId],2);} else { echo "0.00";} ?>
            </span></td>
        </tr>
      </table>
      </td>
	</tr>
	<tr>
		<td>&nbsp;</td>
  </tr>
  <tr>
    	<td><div class="subTitleCls subTitleBackGround">Geographical Stats</div>
      <?php // country map and count list 
    if (!empty($arrCountryListName[$intFMRId])) {
 //checking array exits or not ?>
      <table width="50%" class="tbl paddCls" cellpadding="0" cellspacing="0" >
        <tr>
          <td width="30%"><span class="TableTitleCls">Country</span></td>
          <td width="20%"><span class="TableTitleCls">Clicks</span></td>
        <tr>
          <?php  $l=1;  $intOther=0; 
		$intTotalCountry = count($arrCountryName);
        foreach ($arrCountryListName[$intFMRId] as $strCountryName) {
            if (strlen($strCountryName->country) > 1) {?>
        <tr class=" listdiv rowBg">
          <td><span class="tableTh-value"><?php echo trim(strtoupper($strCountryName->country));?></span></td>
          <td><span class="tableTh-value"><?php echo number_format($strCountryName->ipcount);?></span></td>
        </tr>
        <?php } else {
                $intOther = $intOther + $strCountryName->ipcount;
            } 
        }//END FOREACH  
        // Country name is blank or '-'
        if ($intOther > 0 ) {?>
        <tr class=" listdiv rowBg">
          <td width="30%"><span class="tableTh-value">OTHER/UNIDENTIFIED</span></td>
          <td width="20%"><span class="tableTh-value"><?php echo number_format($intOther);?></span></td>
        </tr>
        <?php }?>
      </table>
      <?php } ?>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td><div class="subTitleCls subTitleBackGround">Top Referring Websites</div>
      <?php  if (!empty($arrRefereLink[$intFMRId])) {
    //checking Referring website array exits or not. if yes show the Website list.
    ?>
      <table width="50%" class="tbl paddCls" cellpadding="0" cellspacing="0">
        <tr>
          <td><span class="TableTitleCls">Link</span></td>
        </tr>
        <?php  $arrParseLink = '';
    foreach ($arrRefereLink[$intFMRId] as $strLink) {
        // getting only domain name form website name.
        $arrParse = parse_url(trim($strLink->rlink));
        $arrParseLink[] =  trim($arrParse['host'] ? $arrParse['host'] : array_shift(explode('/', $arrParse['path'], 2))); // parse link
    }//foreach
    //make unique array for website name.
    $arrParseLink = array_unique($arrParseLink);
    $intTotalLink = count($arrParseLink); 
    
    foreach ($arrParseLink as $strRLink) { ?>
        <tr>
          <td><span class="tableTh-value"><?php echo trim($strRLink);?></span></td>
        </tr>
        <?php } ?>
      </table>
      <?php } ?>
    </td>
  </tr>
  <?php } // foreach end
  	} //If end
  // FMR INFORMATION END FROM HERE ?>
  <tr>
    <td>&nbsp;</td>
  </tr>
</table>


