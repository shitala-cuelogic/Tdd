<div>
    <div class="rightSideCont">
    <ul class="anlyticsTab">
       <li class="active">
            <span>
                <a href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList" class="activeanchor">My Email Lists</a>
            </span>
        </li> 
    </ul>
    </div>
</div>

<div class="listStart">
     <div class="brdContNew">
         <span class="frstLink"><a class="fistLink" href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList">My Email Lists</a></span>
         <span class="">
             <a class="otherLink" href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList/edit/<?php echo base64_encode($intListId);?>" title="<?php echo strtoupper($listName);?>"><?php echo strtoupper(substr($listName,0,20));?></a></span>
         <span class="active"><a class="otherLink" href="javascript:void(0);">Entries</a></span>
     </div>

    <?php if ($intFlag > 0 && $intFlag < 5) {?>
    <div id="SuccessMsg"><div class="SuccessMsg"><?php echo $strMessage;?></div></div>
    <?php } else if ($intFlag == 5) {?>
    <div class="warningMessage">
        <b><?php echo $strMessage; ?></b></div>
    <?php } ?>

 <ul class="margin0">
<li class="pageStyle pageoverflowhidden">

<?php if ($intOgId == $intListOwner) {?>

<div>

</div>

<?php } ?>

</li>
</ul>
    <div class="noteOnPageBlockList">
        <p class="noteOnPage">In order to edit an existing contact, you must remove it and then re-add it via the ‘Add Contact’ link.</p>

    </div>
    <div>
        <div  class="addcontactstyle">
            <a href="<?php echo $strBaseUrl;?>/Dashboard/SubscriberList/add/<?php echo base64_encode($intListId);?>" title="Add Contact"><img src="<?php echo $strBaseUrl;?>/sites/all/themes/threebl/images/justmeans/add_user.png"  class="addimage" /><b>Add Contact</b></a>
        </div>
    </div>
    <div class="divclearboth"></div>

    <form id="frmClientEmail" name="frmClientEmail" method="post" action="" enctype="multipart/form-data" onsubmit="return fnCheckValidation();">
<div class="borderOnList">
<ul class="ullist">

<?php if (!empty($arrList)) {?>

<li class="listLoop customEmailListHeader stylewidth">
    <?php if ($intOgId == $intListOwner) {?><span class="width20"><input type="checkbox" name="chkSelectDeselectAll" id="chkSelectDeselectAll"  onclick="SelectDeselect(); " /></span> <?php } ?>
    <span class="width140"><b>Name</b></span>
    <span class="width228"><b>Email</b></span>
    <span class="width72"><b>Status</b></span>
</li>

   <?php
    $intCount = 1;
     foreach ($arrList as $strList) {
         $strEvenRow = ($intCount % 2 == 0)?"evenRow":"";
         ?>
<li class="clientMicroList  <?php echo $strEvenRow;?>">

<div>
<?php
    if ($intOgId == $intListOwner) {?><span class="width20"><input type="checkbox" name="emailid[]" id="emailid<?php echo $strList->uid; ?>"  value="<?php echo $strList->uid; ?>" onclick="delselectcheckbox(); "/> </span> <?php } ?>
<span class="width140 styleBlackColor"><?php echo $strList->field_real_name_value; ?></span>
<span class="width228 styleBlackColor"><?php echo $strList->mail; ?></span>
<span class="width72 styleBlackColor"><?php echo $strStatus = ($strList->field_unsub_fmr_updates_value == 1 || $strList->field_unsub_all_3bl_comm_value == 1)?'Unsubscribed':'Subscribed'; ?></span>
<div class="divclearboth"></div>
</div>

 </li>
    <?php $intCount++; }  ?>
	<?php } else { ?>
<li class="clientMicroList">
<div><span class="styleWidthAuto"><?php echo "No any Contact found." ?></span></div>
</li>
   <?php } ?>
</ul>
</div>

<?php if ($intOgId == $intListOwner && $intCount > 1) {?>
<div class="pageStyle">
    <div class="removesubbtn"><input type="submit" name="delete" id="delete" value="Remove" class="form-submit removesubbtntext"/> &nbsp;
        <span class="errormsg" id="errormsg"></span></div>
    <div class="styleFloat"><?php echo  $strPage;?></div>
</div>
<?php  } ?>

 </form>

</div>


