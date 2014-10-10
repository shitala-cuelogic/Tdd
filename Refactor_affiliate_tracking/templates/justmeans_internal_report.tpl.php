<style type="text/css">
    #fmr-header{background: none repeat scroll 0 0 #E9EBED; margin: 0 0 0.5em; padding: 1em 20px;}
    .pad5{padding: 5px}
    .padtop10{margin-top: 10px}
    .fontsize{font-size: 18px;}
    .table{border: 1px solid #e1e1e1; width: 50% !important; background: #FFFFFF !important}
    .td{background: #000000; border-right: 1px solid #eee; padding: 5px; border-bottom: 1px solid #e1e1e1;}
    .td2{border-right: 1px solid #eee; padding: 5px; border-bottom: 1px solid #e1e1e1;}
</style>

<div id="fmr-header">
    <div class="client-name pad5">
        <a href="<?php echo $arrFMRInfo["FMRUrl"] ?>" target="_blank"><b><span class="fontsize"><?php echo $arrFMRInfo["strFMRTile"]; ?></span></b></a>
    </div>
    <div class="client-link pad5">
        Media Type: <b><?php echo $arrFMRInfo["FMRType"]; ?></b> | <a href="<?php echo $arrFMRInfo["CompanyUrl"] ?>" target="_blank"> <?php echo $arrFMRInfo["strCompanyname"]; ?></a><br>

        <div class="padtop10">Date: <?php echo $arrFMRInfo["strdate"]; ?>
        </div>
    </div>
    <div>
</div>

<table width="100%" cellpadding="0" cellspacing="0" border="0" class="table">
    <tr>
        <td class="td">
            <b><span style="color:white">View</span></b>
        </td>
        <td class="td">
            <b><span style="color:white">Click</span></b>
        </td>
    </tr>
    <tr>
        <td class="td2">
            <?php echo ($arrFmrViews['viewcount'] >0) ? $arrFmrViews['viewcount'] : 0; ?>
        </td>
        <td class="td2">
            <?php echo ($arrFMRClicks['clickcount'] > 0) ? $arrFMRClicks['clickcount'] : 0;?>
        </td>
    </tr>
</table>