<div class="rightSideCont">
    <?php if ($intFlag > 0) { ?>
    <div id="SuccessMsg">
        <div class="SuccessMsg"><?php echo $strMessage;?></div>
    </div>
    <?php } ?>
    <div>
        <ul class="anlyticsTab">
            <li class="inactive inactivemargin">
                <span>
                    <a href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList" class="activeanchor">My Email
                        Lists</a>
                </span>
            </li>
            <li class="active">
                <span>
                    <a href="<?php echo $strBaseUrl;?>/Dashboard/3blMicroList" class="activeanchor">3BL Media Micro
                        Lists</a>
                </span>
            </li>
        </ul>
    </div>
    <div class="detailsContainer">
        <div class="expTabsContainer tabcontainerPad">
            <div class="brdContNew">
            	<span class="frstLink">
           		 	<a class="fistLink">3BL Media Micro Lists</a>
            	</span>
            </div>
        </div>
        <div class="noteOnPageBlockList">
            <p class="noteOnPage">
                The following 3BL Media Micro Lists are available to you. They can be selected for distribution with any
                of your content, using the “Influencers” tab of the upload/edit form for a given FMR. If you don’t see
                any lists and are interested in Micro List distribution, please contact your Media Consultant.
            </p>
        </div>
        <div class="borderOnList">
            <ul class="ullist">
                <li class="listLoop customEmailListHeader">
                    <span class="pad20"><b>List Name</b></span>

                </li>
                <?php if (!empty($arrList)) {
                $intCount = 1;
                $intLastRecord = count($arrList);

                foreach ($arrList as $strList) {
                    $strEvenRow = ($intCount % 2 == 0) ? "evenRow" : "";
                    ?>
                    <?php if ($intLastRecord == $intCount) { ?>
                        <li class="listLoop bottomspace borderbottom <?php echo $strEvenRow;?>">
                   <?php } else { ?>
                    <li class="listLoop <?php echo $strEvenRow;?>">
                    <?php } ?>
                    <div>
                        <span
                            class="listheading1 pad20 listheading1font"><?php echo $strList->field_email_list_title_value; ?></span>

                        <p class="textMargin textMarginlineHt"><?php echo $strList->field_email_list_details_value; ?></p>
                    </div>
                    <div class="divclearboth"></div>
                    </li>
                <?php $intCount++;
                }
            } else {
                ?>
                <li class="listLoop">
                    <div><span><?php echo "No any record found." ?></span></div>
                </li>
                <?php } ?>

                <?php if ($strPage) { ?>
                <li class="pageStyle">
                    <div><?php echo  $strPage;?></div>
                </li>
                <?php } ?>
            </ul>
        </div>
    </div>
</div>