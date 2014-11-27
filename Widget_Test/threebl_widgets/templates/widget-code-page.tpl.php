<style type="text/css">
    .titlediv{padding-bottom: 15px}
    .frontpagecode{vertical-align: top;font-weight: bold;}
    .clearboth{clear: both}
    .fontweigth{font-weight: bold;}
    .valgintop{vertical-align: top;font-weight: bold;}
    .textareadiv{resize: none;float: left;margin-right: 5px}
</style>

<div class="titlediv"><span><strong><?php echo $strTitle;?></strong></span></div>

<table width="90%" border="0" cellspacing="3" cellpadding="3">
    <tr><td colspan="3" valign="top">&nbsp;</td></tr>
    <tr>
        <td width="5%">&nbsp;</td>
        <td width="15%" valign="top" align="left" class="frontpagecode">Front page code:</td>
        <td width="80%" valign="top">
            <div class="clearboth">
                <span class="fontweigth">http://</span><br/>
                <div>
                    <textarea rows="3" cols="110" id="widget_script" class="form-textarea textareadiv" readonly="readonly"><div id="threeblmediawidget"></div><script type="text/javascript" src="<?php echo $strPath;?>/3bl_widgets_data.jsv?t=<?php echo $intWidgetId;?>&p=bW9yZQ==" ></script></textarea>
                    <a href="<?php echo $strPath."/admin/widget-preview/frontpage?act=preview&gid=".$intWidgetPreviewId;?>" target="_blank">View Front page preview</a><br/><br/>
                </div>
            </div>
            <div class="clearboth">
                <br/>
                <span class="fontweigth">https://</span><br/>
                <textarea rows="3" cols="110" id="widget_script" class="form-textarea textareadiv" readonly="readonly"><div id="threeblmediawidget"></div><script type="text/javascript" src="<?php echo str_replace("http://", "https://", $strPath);?>/3bl_widgets_data.jsv?t=<?php echo $intWidgetId;?>&p=bW9yZQ=="></script></textarea>
            </div>
        </td>
    </tr>
    <tr>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
    </tr>
    <tr>
        <td>&nbsp;</td>
        <td valign="top" align="left" class="valgintop"> List and Detail page code:</td>
        <td valign="top">
            <div class="clearboth">
                <span class="fontweigth">http://</span><br/>
                <div>
                    <textarea rows="4" cols="110" id="widget_more_script" readonly="readonly" class="form-textarea textareadiv" ><div id="threeblmediadetaillist"></div><script language="javascript" src="<?php echo $strPath; ?>/sites/all/modules/custom_modules/threebl_widgets/inc/widget_function.js"></script><script type="text/javascript">init_widget_new("<?php echo $intWidgetId;?>","ZGV0YWlscw==");</script></textarea>
                    <a href="<?php echo $strPath."/admin/widget-preview/widget-list?act=preview&gid=".$intWidgetPreviewId;?>" target="_blank">View List page preview</a>
                </div>
                <br/>
            </div>
            <div class="clearboth">
                <br/><span class="fontweigth">https://</span><br/>
                <textarea rows="4" cols="110" id="widget_more_script" readonly="readonly" class="form-textarea textareadiv"><div id="threeblmediadetaillist"></div><script language="javascript" src="<?php echo str_replace("http://", "https://", $strPath); ?>/sites/all/modules/custom_modules/threebl_widgets/inc/widget_function.js"></script><script type="text/javascript">init_widget_new("<?php echo $intWidgetId;?>","ZGV0YWlscw==");</script></textarea>
            </div>
        </td>
    </tr>
    <tr><td>&nbsp;</td><td>&nbsp;</td><td>
        <a href="<?php echo $strPath;?>/admin/3bl-widget-views"><input id="edit-submit" class="form-submit" type="Button" value="Back" name="op"></a></td></tr>
</table>