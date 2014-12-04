<div> <h2 class="listTitle">Add Subscribers in <?php echo $listName; ?></h2></div>
<form class="frmClientEmail" id="frmClientEmail" method="post" enctype="multipart/form-data">

    <div class="divemailClear">
        <table width="100%" border="0" cellspacing="3" cellpadding="3">
            <tr><td colspan="3">&nbsp;</td></tr>

            <tr>
                <td width="10%">&nbsp;</td>
                <td width="20%">First Name<span class="form-required" title="This field is required.">*</span>:</td>
                <td width="70%"><input type="text" size="60"  name="fname" id="fname" class="form-text validate[required] text-input" /></td>
            </tr>


            <tr>
                <td width="10%">&nbsp;</td>
                <td width="20%">Last Name<span class="form-required" title="This field is required.">*</span>:</td>
                <td width="70%"><input type="text" size="60"  name="lname" id="lname" class="form-text validate[required] text-input" /></td>
            </tr>

            <tr>
                <td width="10%">&nbsp;</td>
                <td width="20%">Email<span class="form-required" title="This field is required.">*</span>:</td>
                <td width="70%"><input type="text" size="60"  name="email" id="email" class="form-text validate[required,custom[email]] text-input" /></td>
            </tr>


            <tr>
                <td>&nbsp;</td>
                <td>&nbsp;</td>
                <td><input type="submit" name="submitEmail" id="submitEmail" value="Save" class="form-submit"/></td>
            </tr>
        </table>
    </div>
</form>
<p>&nbsp;</p>