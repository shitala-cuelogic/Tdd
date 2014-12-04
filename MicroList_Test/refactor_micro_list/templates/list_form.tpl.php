<form class="frmClientEmail" id="frmClientEmail" method="post" enctype="multipart/form-data"
      onsubmit="return fnCheckFileType();">

    <div class="listClearBoth">
        <table width="100%" border="0" cellspacing="3" cellpadding="3">
            <tr>
                <td colspan="3">&nbsp;</td>
            </tr>

            <tr>
                <td width="10%">&nbsp;</td>
                <td width="20%" class="styleVerticalTop">List Title<span class="form-required"
                                                                         title="This field is required.">*</span>:
                </td>
                <td width="70%"><input type="text" size="60" name="title" id="title"
                                       class="form-text validate[required] text-input"
                                       value="<?php if ($strListTitle != "") echo $strListTitle; ?>"/>

                    <div class="description">Enter email list name.</div>
                </td>
            </tr>

            <tr>
                <td width="10%">&nbsp;</td>
                <td width="20%" class="styleVerticalTop">Details<span class="form-required"
                                                                      title="This field is required.">*</span>:
                </td>
                <td width="70%"><textarea rows="10" cols="70" id="details" name="details"
                                          class="form-textarea validate[required] text-input"><?php if ($strListDetails != "") echo $strListDetails; ?></textarea>

                    <div class="description">Enter email list description.</div>
                </td>
            </tr>

            <tr>
                <td width="10%">&nbsp;</td>
                <td width="20%" class="styleVerticalTop">Upload List<span class="form-required"
                                                                          title="This field is required.">*</span>:
                </td>
                <td width="70%"><input type="file" size="60" name="emailfile" id="emailfile"
                                       class="form-text validate[required]"/>

                    <div class="description">Upload email list of .xls or .csv format.</div>
                    <span id="errormsg" class="listFormErrMsg"></span>

                    <div class="description">
                        <div>You can download the examples, to see the appropriate fields and their order. Note that the
                            header row should be included. The maximum list size is <?php echo $intMaxUserLimit ?>
                            records.
                        </div>
                        <span class="listMarginRt"><a href="<?php echo $strBaseUrl;?>/sample-excel-download/xls"><img
                            class="imgs" align="absmiddle" title="Sample .xls Email List"
                            src="<?php echo $strBaseUrl;?>/sites/all/themes/threebl/images/excels.png"></a></span>
                        <span><a href="<?php echo $strBaseUrl;?>/sample-excel-download/csv"><img class="imgs"
                                                                                                 align="absmiddle"
                                                                                                 title="Sample .csv Email List"
                                                                                                 src="<?php echo $strBaseUrl;?>/sites/all/themes/threebl/images/CSV.png"></a></span>
                    </div>
                </td>
            </tr>

            <tr>
                <td>&nbsp;</td>
                <td>&nbsp;</td>
                <td><input type="submit" name="submitList" id="submitList" value="Save" class="form-submit"/></td>
            </tr>
        </table>
    </div>
</form>
<p>&nbsp;</p>
