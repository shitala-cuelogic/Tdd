<div id="media_view" style="display: block;">
<?php 
 if($intTotalRecords > 0) {?>
 
<h2 class="trigger"><a href="#">Views per <?php echo ucwords($strTitle); ?></a></h2>
<div class="clr"></div>
<div class="toggle_container">

<input id="mediatype" name="mediatype" value="<?php echo $strtype;?>" type="hidden">
<input id="pagenumber" name="pagenumber" value="<?php echo $intpage; ?>" type="hidden" />


<div class="detailsTable">
<div class="tableTh2">
<h2><?php echo ucwords($strTitle); ?></h2>
<h3 style="margin-top:0px !important;"> Views </h3>
</div>
<div class="tableTd2">
<ul>

<?php $i=0;
if(!empty($arraymediaresult))
{
  list($arrMedia, $arrUrls) = $arraymediaresult;
  $intCountarr =  count($arrMedia);
   for($i=0; $i < $intCountarr; $i++){
     $media = $arrMedia[$i];
	 $url = $arrUrls[$i]->fmrurl;
	 
?>
<li <?php if((int)($i+1)%2 ==0) {?> class="rowBg" <?php } ?>>
<div>
<a href="<?php echo url($url,array('absolute' => TRUE));?>" target="_blank"><?php echo $media->title;?></a><p><?php echo date('d',strtotime($media->datevisited));?> <sup>th</sup>
 <?php echo date('M',strtotime($media->datevisited));?> <?php echo date('Y',strtotime($media->datevisited));?></p></div>
<h3><?php echo number_format($media->totalcount);?></h3>
<p class="clr"></p>
</li>
<?php 
     } 
 } ?>
 
</ul>
</div>
<div class="paginationDC"><?php if($intprev == 0){?> <lable> 
<a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('prev')">Back</a></lable>&nbsp;
<?php } ?>

<lable>   
<a href="javascript:void(0);"><?php echo $intpage; ?></a>
</lable>

<?php if($intnext == 0){?>
<lable>   
<a href="javascript:void(0);" onclick="javascript:displayMediaInfoAjax('next')">Next</a></lable> <?php }?></div>
</div>
</div>
<div class="clr"></div>

<?php } else {?>

<div class="pad10">
<span style="color: #0070C0 !important;font-size: 14px;">No View data available for the current selected <strong><?php echo ucwords($strTitle); ?></strong> media from  
last six months.<br>
</span>
</div>

<?php } ?>


</div>

