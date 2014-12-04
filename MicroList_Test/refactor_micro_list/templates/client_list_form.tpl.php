<div>
    <ul class="anlyticsTab">
        <li class="active">
            <span>
                <a href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList" class="activeanchor">My Email Lists</a>
            </span>
        </li>
    </ul>
</div>

<div class="listStart">
    <div class="brdContNew">
        <span class="frstLink"><a class="fistLink" href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList">
            My Email Lists
        </a></span>
        <?php

        $strEditListName = strtoupper($strListTitle);
        $strAction = ($intListId == '') ? "NEW LIST" : "EDIT $strEditListName";?>
        <span class="active"><a class="otherLink"><?php echo $strAction;?></a></span>
    </div>
    <div>

        <?php if ($intListId > 0) { ?>

        <div class="noteOnPageBlockForm">
            <p class="noteOnPage">Click on ‘Edit Contact List’ if you want to add/delete contacts for this list.</p>
        </div>

        <div>
            <div class="editContactList">
                <a title="Manage Contacts"
                   href="<?php echo $strBaseUrl;?>/Dashboard/SubscriberList/<?php echo base64_encode($intListId); ?>"><img
                    src="<?php echo $strBaseUrl;?>/sites/all/themes/threebl/images/justmeans/edit_sub.png"
                    class="addimage"/><b>Edit Contact List</b></a>
            </div>
        </div>
        <?php } else { ?>

        <div class="noteOnPageBlockForm">
            <p class="noteOnPage">List Name and Description are only visible to you – not to email recipients. See
                examples of list format – all fields are required. </p>
        </div>

        <?php } ?>

        <div class="divborderstyle">
        </div>

        <?php if ($intFlag > 0) { ?>
        <div class="warningMessage">
            <b><?php echo $strMessage; ?></b></div>
        <?php } ?>

        <div class="fieldset-wrapper styleDisplayBlock">

            <form id="frmClientEmail" name="frmClientEmail" method="post" action="" enctype="multipart/form-data"
                  onsubmit="return fnCheckFileType();">
                <div class="frmLeft">

                    <div class="form-item">
                        <label for="Title">List Name<span class="form-required" title="This field is required.">*</span></label>
                        <input type="text" class="form-text validate[required]" maxlength="60" size="60"
                               value="<?php echo $strListTitle;?>" name="title" id="title"
                               placeholder="Enter email list name.">
                    </div>

                    <div class="form-item form-type-password form-item-pass">
                        <label for="details">Description<span class="form-required"
                                                              title="This field is required.">*</span></label>
                        <textarea rows="13" cols="46" name="details" id="details"
                                  class="form-textarea validate[required] text-input styleResize"
                                  placeholder="Enter email list description."><?php echo $strListDetails;?></textarea>
                    </div>

                    <?php if ($intListId == '') { ?>
                    <div class="form-item form-type-password form-item-pass">
                        <label for="details">Upload List<span class="form-required"
                                                              title="This field is required.">*</span> </label>
                        <input type="file" class="form-file validate[required]" maxlength="128" size="60"
                               name="emailfile" id="emailfile">

                        <div class="description">Upload email list of .xls or .csv format.</div>
                        <span id="errormsg" class="errormsg"></span>

                        <div class="samplePadding">You can download the following examples, to see the appropriate
                            fields and their order. Note that the header row should be included. The maximum list size
                            is <?php echo $intMaxUserLimit ?> records.
                        </div>
                        <div class="stylePadTop4">
                            <span class="styleMarginRt2"><a
                                href="<?php echo $strBaseUrl;?>/sample-excel-download/xls"><img class="imgs"
                                                                                                align="absmiddle"
                                                                                                title="Sample .xls Email List"
                                                                                                src="<?php echo $strBaseUrl;?>/sites/all/themes/threebl/images/excels.png"></a></span>
                            <span><a href="<?php echo $strBaseUrl;?>/sample-excel-download/csv"><img class="imgs"
                                                                                                     align="absmiddle"
                                                                                                     title="Sample .csv Email List"
                                                                                                     src="<?php echo $strBaseUrl;?>/sites/all/themes/threebl/images/CSV.png"></a></span>
                        </div>
                    </div>

                    <?php } ?>

                    <div id="" class="form-actions form-wrapper">
                        <input type="submit" class="form-submit" value="Save" name="submitList" id="submitList">
                        <input type="Button" class="form-submit" value="Cancel" name="cancelList" id="CancelCustList"
                               onclick="location ='<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList';">
                    </div>
                </div>
            </form>

        </div>
    </div>
</div>
