<style type="text/css">
    select {
        background: none repeat scroll 0 0 #FFFFFF;
        border: 1px solid #C3C3C3;
        height: 28px;
        min-height: 25px;
        padding: 3px;
    }

    .detailsContainer {
        display: block;
        padding: 10px 10px 10px 6px;
        z-index: 9;
    }

    ul.analyticsTrigger {
        display: block;
        list-style: none outside none;
    }

    ul.analyticsTrigger li {
        padding: 0;
    }

    .traking-search div span {
        display: block;
        float: left;
        padding-left: 20px;
        width: 120px !important;
    }

    .traking-search {
        margin-bottom: 15px;
        margin-top: 5px;
    }

    ul.analyticsTrigger li .trigger {
        background: url("/sites/all/themes/threebl/images/justmeans/acc_h2_bg.png") no-repeat scroll left top #E9F1F2;
        color: #097F95;
        cursor: pointer;
        font-size: 20px;
        font-weight: bold;
        height: 29px;
        line-height: 29px;
        padding: 0 0 0 35px;
        width: auto;
        text-align: center;
    }

    .bordernone { border: none !important;}
    .marginleft { margin-left: 20px;}
    .bordertable{ border: 1px solid #e1e1e1}
    .inlineBlk{ display: inline-block !important;}
    .tdcls{ border-right: 1px solid #eee; padding: 5px; border-bottom: 1px solid #e1e1e1;}
    .clientname{float:left; margin-right: 10px;}
    .verticalalg {vertical-align: middle;}
    .padbot{padding-bottom: 10px;}
</style>

<?php if ($download == "excel") { ?>
    <style type="text/css">
        .tbl, .tbl td, th{border:1px solid #ddd}
    </style>
<?php } ?>

<?php if ($download != "excel" || $download == "") { ?>
<form action="" method="get" id="">
    <div class="detailsContainer bordernone">
        <ul class="analyticsTrigger">

            <li class="traking-search">
                <div>
<span>
<strong>Enter Start Date:</strong>
</span>
                    <input type="text" name="startdate" id="strdate" placeholder="Enter Start Date" value="<?php echo $intStartDate;?>" class="form-text" size="45"/>
                    <small>Ex. 2013-12-16</small>
                </div>
            </li>

            <li class="traking-search">
                <div>
<span>
<strong>Enter End Date:</strong>
</span>
                    <input type="text" name="enddate" id="enddate" placeholder="Enter End Date" value="<?php echo $intEndDate;?>" class="form-text" size="45"/>
                    <small>Ex. 2013-12-16</small>
                </div>
            </li>
            <li class="traking-search">
                <div class="marginleft">
                    <span>  </span>
                    <input type="submit" name="sub" value="Generate" class="form-submit" />
                    <input type="button" name="reset" value="Reset" class="form-submit" id="adminlistreset" onclick="fnResetForm();"/>
                </div>
            </li>
        </ul>
    </div>
</form>

<?php }
if (count($arrList) > 0) {

    $strUrlParams = "$strPageAction?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $next_status."&page=".$intPage."&sort=date";
    $strCompanyNameUrl = "$strPageAction?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $next_status."&page=".$intPage."&sort=name";
    $strExcelDownloadUrl = "$strPageAction?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&dwn=excel";
?>
<div class="divemailClear">

    <?php if ($download != "excel" || $download == "") { ?>
    <div align="right" class="padbot">
            <a href="<?php echo $strExcelDownloadUrl; ?>">
                <img title="Download Report" src='<?php echo url("$imagepath/excels.png", array('absolute' => TRUE));?>' class="verticalalg">
            </a>
        <span><a href="<?php echo $strExcelDownloadUrl; ?>">Download Report</a></span>
    </div>
    <?php } ?>

    <table width="100%" cellpadding="0" cellspacing="0" border="0" class="bordertable">

        <?php if ($download == "excel") { ?>
        <tr>
            <td colspan="5"><?php echo $strMicroListName; ?></td>
        </tr>
        <tr>
            <td colspan="5">From: <?php echo date("F j, Y", strtotime($intStartDate)); ?> To: <?php echo date("F j, Y", strtotime($intEndDate)); ?></td>
        </tr>
        <?php } ?>
        <tr>
            <th>
                <?php if ($download != "excel" || $download == "") { ?>
                    <a href="<?php echo $strCompanyNameUrl; ?>" class="clientname">Client Name</a>
                <?php
                    if ($sortby == "name") {
                        if ($current_order == "ASC") {
                ?>
                            <img src='<?php echo url("$imagepath/up.gif", array('absolute' => TRUE));?>' align='absmiddle' title="First">
                <?php
                        } else {
                ?>
                            <img src='<?php echo url("$imagepath/down.gif", array('absolute' => TRUE));?>' align='absmiddle'>
                <?php
                        }
                    }
                } else {
                ?>
                    Client Name
                <?php } ?>
            </th>
            <th>FMR Title</th>
            <th>Assigned List</th>
            <th>Assigned List Count</th>
            <th width="12%">
                <?php if ($download != "excel" || $download == "") { ?>
                    <a href="<?php echo $strUrlParams; ?>" class="inlineBlk">Published Date</a>
                <?php
                    if ($sortby == "date") {
                        if ($current_order == "ASC") { ?>
                            <img src = '<?php echo url('sites/all/themes/threebl/images/up.gif', array('absolute' => TRUE));?>' align = 'absmiddle' title ="First" >
                <?php   } else { ?>
                            <img src='<?php echo url('sites/all/themes/threebl/images/down.gif', array('absolute' => TRUE));?>' align='absmiddle'>
                <?php   }
                    }
                } else {
                ?>
                    Published Date
                <?php } ?>
            </th>
            <!--<th style="background:#000000; border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;"><span
          style="color:white">publish Status</span></th>
      <th style="background:#000000; border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;"><span
          style="color:white">Distribution Status</span></th> -->
        </tr>
        <?php
        $intCount = 1;
        foreach ($arrList as $strList) {
            $strStyleParam = "";
            if ($download != "excel" || $download == "") {
                if ($intCount % 2 == 0) {
                    $strStyleParam = "background-color: #E1E2DC;";
                }
            }

            $intTotalMicroList = count($arrMicroList[$strList->fmrId]);

            if ($intTotalMicroList == 0) {
                continue;
            }

            ?>
            <tr style="<?php echo $strStyleParam; ?>">
                <td class="tdcls">
                    <a href="<?php echo $base_url . '/node/' .$strList->compId; ?>"><?php echo stripslashes($strList->label);?> </a>
                </td>
                <td class="tdcls">
                    <a href="<?php echo $base_url . '/node/' . $strList->fmrId; ?>"><?php echo stripslashes($strList->fmrTitle);?></a>
                </td>

                <td class="tdcls">
                <?php
                    $intCnt = 1;
                    foreach ($arrMicroList[$strList->fmrId] as $strMicroListName) {
                        echo $intCnt.". ".$strMicroListName."<br />";
                        $intCnt ++;
                    }
                ?>
                </td>

                <td class="tdcls"><?php echo $intTotalMicroList; ?></td>
                <td class="tdcls"><?php echo date("F j, Y", strtotime($strList->publishdate));?></td>
                <!--<td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;"><?php if ($strList->status) {
                    echo "Published";
                } else {
                    echo "Unpublished";
                }?></td>
        <td style="border-right:1px solid #eee;padding: 5px; border-bottom:1px solid #e1e1e1;"><?php if ($strList->pub > 0) {
                    echo "Delivered";
                } else {
                    echo "Undelivered";
                }?></td> -->
            </tr>
            <?php
        $intCount++;
        } ?>
    </table>

    <?php if ($download != "excel" || $download == "") { ?>
    <div class="paginationDC">
        <!-- Chekcing previous or back button  -->
        <?php
        if ($intPrev == 0) {
            $intPageValue = 1;
            $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
            ?>
            <lable><a href="<?php echo $strUrlParams;?>"><img
                src='<?php echo url('sites/all/themes/threebl/images/first_ico.png', array('absolute' => TRUE));?>'
                align='absmiddle' title="First"></a></lable>
            <?php
            $intPageValue = $intPage - 1;
            $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
            ?>
            <lable><a href="<?php echo $strUrlParams;?>"><img
                src='<?php echo url('sites/all/themes/threebl/images/prev_ico.png', array('absolute' => TRUE));?>'
                align='absmiddle' title="Prev"></a></lable>
            &nbsp;
            <!-- Chekcing current page is first page  with if yes also check current page is last or not  if not then show next pages  -->
            <?php } if ($intPage == 1) {
            $intPageValue = $intPage = 1;
            $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
            ?>
        <lable><a href="<?php  echo $strUrlParams;?>"><?php echo $intPage; ?></a></lable>
        <?php if (($intPage + 1) <= $intLastPage) {
            $intPageValue = $intPage + 1;
            $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
            ?>
            <lable><a href="<?php  echo $strUrlParams;?>"><?php echo $intPage+1; ?></a></lable>
            <?php }
        if (($intPage + 2) <= $intLastPage) {
            $intPageValue = $intPage + 2;
            $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
            ?>
            <lable><a href="<?php echo $strUrlParams;?>"><?php echo $intPage + 2; ?></a>
            </lable>
            <?php
        }

    } // Chekcing for last page if current page is last  then check it's previous pages by subtracting  -1, -2
    else if ($intLastPage == $intPage) {
        if ($intPage - 2 >= 1) {
            $intPageValue = $intPage - 2;
            $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
            ?>
            <lable><a href="<?php echo $strUrlParams;?>"><?php echo $intPage - 2; ?></a>
            </lable>
            <?php }
        if (($intPage - 1) >= 1) {
            $intPageValue = $intPage - 1;
            $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
            ?>
            <lable><a href="<?php echo $strUrlParams;?>"><?php echo $intPage - 1; ?></a>
            </lable>
            <?php } ?>
        <lable><a href="javascript:void(0);" class="active-page"><?php echo $intPage; ?></a></lable>

        <!-- Chekcing for current pages between first and last -->
        <?php
    } else {
        if (($intPage - 1) >= 1) {
            $intPageValue = $intPage - 1;
            $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
            ?>
            <lable><a href="<?php echo $strUrlParams; ?>"><?php echo $intPage - 1; ?></a>
            </lable>
            <?php } ?>
        <lable><a href="javascript:void(0);" class="active-page"><?php echo $intPage; ?></a></lable>
        <?php if (($intPage + 1) <= $intLastPage) {
            $intPageValue = $intPage + 1;
            $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
            ?>
            <lable><a href="<?php echo $strUrlParams; ?>"><?php echo $intPage + 1; ?></a>
            </lable>

            <!-- Chekcing for next last button -->
            <?php
        }
    } if ($intNext == 0) {
        $intPageValue = $intPage + 1;
        $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
        ?>
        <lable><a href="<?php echo $strUrlParams;?>"><img
            src='<?php echo url('sites/all/themes/threebl/images/next_ico.png', array('absolute' => TRUE));?>'
            align='absmiddle' title="Next"></a></lable>
         <?php
        $intPageValue = $intLastPage;
        $strUrlParams = "Micro-List-Report?startdate=" . $intStartDate . "&enddate=" . $intEndDate . "&status=" . $current_order . "&page=" . $intPageValue;
        ?>
        <lable><a href="<?php echo $strUrlParams;?>" title="Last"><img
            src='<?php echo url('sites/all/themes/threebl/images/last_ico.png', array('absolute' => TRUE));?>'
            align='absmiddle'></a></lable>
        <?php }?>
    </div>
    <?php } ?>

</div>
<?php
} else {
        ?>
    <div align="center"><span><h1><?php echo "Records not found." ?></h1></span></div>
<?php
}
?>

