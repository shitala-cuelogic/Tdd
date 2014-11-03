<style type="text/css">
    ul.pagination {
        /*width:360px;*/
        margin-right: 8px;
        padding: 0 0 40px
    }

    ul.pagination li {
        float: left;
        display: inline;
        margin-left: 4px;
        color: #fff;
        font-weight: bold
    }

    ul.pagination li a {
        float: left;
        display: block;
        background: #969372;
        border: 1px solid #726f57;
        color: #2f2d1c;
        font-weight: bold;
        height: 18px;
        line-height: 18px;
        padding: 0 6px
    }

    ul.pagination li a:hover, ul.pagination li a.active {
        background: #e3dfe1;
        border: 1px solid rgba(58, 56, 52, 0.59)
    }

    ul.pagination li.first {
        font-weight: bold;
        padding: 0 10px 0 5px;
        background: url(../images/paging_devier.gif) 100% 3px no-repeat;
        margin: 0
    }

    ul.pagination li.first a {
        color: #000000;
        background: none;
        border: none;
        line-height: 12px;
        padding: 0;
        margin-top: 3px
    }

    ul.pagination li.prev {
        background: none;
        padding-right: 3px
    }

    ul.pagination li.dotclr {
        color: #000000
    }

    ul.pagination li.pageinfo {
        padding: 2px 0 0
    }

    .styleFloatRt {
        float: right;
    }

    .styleFloatLt {
        float: left;
    }
</style>
<?php
/**
 * Listing all active and inactive widgets
 */
$strActive = "";
$strPath = $strSortUrl . $intListId;

if (empty($arrEmailList)) {
    $strActive = "<tr><td colspan='6' >There is no any contact exist</td></tr>";
} else {
    $strActive = "";
    $intCount = 0;
    foreach ($arrEmailList as $arrRow) {
        $strClass = ($intCount % 2) ? 'odd' : 'even';
        $strStatus = ($strList->field_unsub_all_alerts_value == 0) ? 'Active' : 'Inactive';
        $strActive .= " <tr class='$strClass'>
                            <td><input type='checkbox' name='emailid[]' id='emailid" . $arrRow->uid . "'  value='" . $arrRow->uid . "' onclick='delselectcheckbox(); '/></td>
                            <td>" . $arrRow->name . "</td>
                            <td>" . $arrRow->mail . "</td>
                            <td>" . $strStatus . "</td>
                        </tr>";
        $intCount++;
    }
    //for-each
}
if ($intFlag > 0) {
//div for delete email message
    ?>
<div class="messages status">
    <h2 class="element-invisible">Status message</h2>
    <em class="placeholder"><?php echo $strMessage;?>
</div>
<?php } ?>

<?php if ($strListType == "3bl_micro_list") { ?>
<div class="styleFloatRt"><a href="<?php echo $strBaseUrl;?>/admin/add/email/<?php echo $intListId;?>"><b> + Add
    Contact</b></a></div>
<?php } ?>
<form id="frmClientEmail" name="frmClientEmail" method="post" action="" enctype="multipart/form-data"
      onsubmit="return fnCheckValidation();">
    <div>
        <h1><?php echo $strListTitle; ?> List</h1>

        <table id="list">
            <thead>
            <tr>
                <th width="5%"><input type="checkbox" name="chkSelectDeselectAll" id="chkSelectDeselectAll"
                                      onclick="SelectDeselect();"/></th>
                <th class="select-all" width="20%"><a class="active" title="sort by First Name"
                                                      href="<?php echo $strPath;?>?sort=<?php echo $arrSort['name'];?>&amp;order=name">Name</a>
                </th>
                <th width="25%"><a class="active" title="sort by Email"
                                   href="<?php echo $strPath;?>?sort=<?php echo $arrSort['mail'];?>&amp;order=mail">Email</a>
                </th>
                <th width="20%">Status</th>
            </tr>
            </thead>
            <tbody>
            <?php echo $strActive; ?>
            </tbody>
        </table>


        <div>
            <div class="styleFloatLt"><input type="submit" name="delete" id="delete" value="Remove"
                                             class="form-submit"/>&nbsp;
                <span class="errormsg" id="errormsg"></span></div>
            <div class="styleFloatRt"><?php echo $page['strCurr'];?></div>
        </div>

    </div>
</form>