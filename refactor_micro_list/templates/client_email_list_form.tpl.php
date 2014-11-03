<div>
    <div>
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
        <span><a class="otherLink" href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList/edit/<?php echo base64_encode($intListId);?>" title="<?php echo strtoupper($listName);?>"><?php echo strtoupper(substr($listName,0,20));?></a></span>
        <span><a class="otherLink" href="<?php echo $strBaseUrl;?>/Dashboard/SubscriberList/<?php echo base64_encode($intListId);?>">Entries</a></span>
        <span class="active"><a class="otherLink">ADD SUBSCRIBER</span>
    </div>

    <div class="noteOnPageBlockForm">
        <p class="noteOnPage">All fields are required.</p>
    </div>
    <div class="divborderstyle">

    </div>

    <?php if ($intFlag > 0) {?>
    <div class="warningMessage">
        <b><?php echo $strMessage; ?></b></div>
    <?php } ?>

        <div class="fieldset-wrapper styleDisplayBlock">
        
    <form id="frmClientEmail" name="frmClientEmail" method="post" action="" enctype="multipart/form-data">
     <div class="frmLeft">
    
    <div class="form-item">
        <label for="fname">First Name<span class="form-required" title="This field is required.">*</span></label>
        <input type="text" class="form-text validate[required]" maxlength="60" size="60" value="<?php echo $strUserFirstName;?>" name="fname" id="fname" placeholder="Enter First Name.">
    </div>
    
    <div class="form-item form-type-password form-item-pass">
        <label for="fname">Last Name<span class="form-required" title="This field is required.">*</span></label>
        <input type="text" class="form-text validate[required]" maxlength="128" size="60" name="lname" id="lname" placeholder="Enter Last Name." value="<?php echo $strUserLastName;?>">
    </div>

    <div class="form-item form-type-password form-item-pass">
                 <label for="email">Email<span class="form-required" title="This field is required.">*</span></label>
                 <input type="text" class="form-text validate[required,custom[email]]" maxlength="128" size="60" name="email" id="email" placeholder="Enter email address." value="<?php echo $strUserEmailId;?>">
    </div>

     <div id="" class="form-actions form-wrapper">
     <input type="submit" class="form-submit" value="Save" name="submitEmail" id="submitEmail">
     <input type="Button" class="form-submit" value="Cancel" name="cancelList" id="CancelEmailList" onclick="window.location='<?php echo $strBaseUrl;?>/Dashboard/SubscriberList/<?php echo $intListId; ?>'">
     </div>
  </div>
     </form> 
    
</div>
</div>
