<div class="rightSideCont" xmlns="http://www.w3.org/1999/html">
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

    <div class="detailsContainer">
        <div class="expTabsContainer">
                <div class="brdContNew">
                    <span class="frstLink">
                        <a class="fistLink">My Email Lists</a>
                    </span>
                </div>
            </div>
            <?php if ($intFlag > 0 && $intFlag < 5) {?>
    <div id="SuccessMsg"><div class="SuccessMsg"><?php echo $strMessage;?></div></div>
    <?php } else if ($intFlag == 5) {?>
        <div class="warningMessage">
            <b><?php echo $strMessage; ?></b></div>
    <?php } ?>
    	<div class="noteOnPageBlockList">
            <p class="noteOnPage styleFloat">Manage your email lists, here. These can then be selected for distribution with any of your content, using the “My Networks” tab of the upload/edit form for a given FMR. Email messages will be sent when the content is published.</p>
        </div>
        <div class="styleEmailList">
            <a href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList/Form"  title="Add Email List"><img src="<?php echo $strBaseUrl;?>/sites/all/themes/threebl/images/justmeans/add-list-xxl.png"  class="addimage"/><b>Add
                Email List
            </b></a>
        </div>
        
        <div class="searchbox">
        	<input type="text" name="search" id="searchVal" placeholder="Enter list name" class="serchtext"  /><input type="button" name="submit" id=" sub" value="Search" onclick="clientMicroListAjax('',1); " class="search-button"/>
        </div>
        <div class="borderOnList" id="clientlist">
       
			<input id="last" name="last" value="<?php echo $intLastPage; ?>"  type="hidden"/>
            <input id="mediatype" name="mediatype" value="client" type="hidden">
            <input id="pagenumber" name="pagenumber" value="<?php echo $intPage; ?>" type="hidden" />

   		<div class="borderOnList">
        <ul class="ullist">
            <li class="listLoop clsMicroListHeader">
                <div class="titleL"><b>List Name</b></div>
                <div class="titleR"><b>Actions</b></div>
            </li>
            <?php if (!empty($arrList)) {
            $intCount = 1;

            foreach ($arrList as $strList) {
                $strEvenRow = ($intCount % 2 == 0)?"evenRow":"";
                ?>
                <?php if (count($arrList) == $intCount) { ?>
                    <li class="microlist <?php echo $strEvenRow;?> borderbottom">
               <?php } else { ?>
                <li class="microlist <?php echo $strEvenRow;?>">
                <?php } ?>
                    <div>
                        <div class="leftSide">
                            <span class="listheading"><a title="<?php echo $strList->title; ?> " href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList/edit/<?php echo base64_encode($strList->id); ?>"><?php echo $strList->title; ?></a></span>
                            <p class="textMargin"><?php echo $strList->field_email_list_details_value; ?></p>
                        </div>
                        <div class="rightSide rightsidemargin">
                            <ul class="links inline">
                                <li class="node-readmore first"><a title="Edit " href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList/edit/<?php echo base64_encode($strList->id); ?>"><img src="<?php echo $strBaseUrl;?>/sites/all/themes/threebl/images/justmeans/edit_list.png"></a></li>
                                <li class="node-readmore first"><a title="Remove " href="<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList/delete/<?php echo base64_encode($strList->id); ?>"><img src="<?php echo $strBaseUrl;?>/sites/all/themes/threebl/images/justmeans/delete-list.png" id="deleteImage<?php echo $strList->id; ?>" onclick="javascript:return fnConfimration(2);"></a></li>
                            </ul>
                        </div>
                        <div class="divclearboth"></div>
                    </div>
                </li>
                <?php $intCount++;
                      }
                   } else {
                ?>
            <li class="listLoop">
                <div><span><?php echo "You haven't added any Email lists added yet,"; ?>
                    <a href='<?php echo $strBaseUrl;?>/Dashboard/ClientMicroList/Form'  title='Add Email List'> Click Here</a>
                    <?php echo " to add Email list"; ?></span></div>
            </li>
            <?php } ?>
            
        </ul>
    	
    	<?php
         if (!empty($arrList)) {?>
                <div class="paginationDC">
                    <!-- Chekcing previous or back button  -->
                    <?php
                    if ($intPrev == 0) { ?>
                        <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('',1)"><img src= '<?php echo url('sites/all/themes/threebl/images/first_ico.png',array('absolute' => TRUE));?>' align='absmiddle' title="First"></a></lable>
                        <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('prev')"><img src= '<?php echo url('sites/all/themes/threebl/images/prev_ico.png',array('absolute' => TRUE));?>' align='absmiddle' title="Prev"></a></lable>
                        &nbsp;
                        <!-- Chekcing current page is first page  with if yes also check current page is last or not  if not then show next pages  -->
                        <?php } if ($intPage == 1) { ?>
                    <lable> <a href="javascript:void(0);" class="active-page"><?php echo $intPage ; ?></a> </lable>
                    <?php if(($intPage + 1) <= $intLastPage ) {?>
                        <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('',<?php echo $intPage + 1; ?>)"><?php echo $intPage + 1; ?></a> </lable>
                        <?php } if (($intPage + 2) <= $intLastPage ) {  ?>
                        <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('',<?php echo $intPage + 2; ?>)"><?php echo $intPage + 2; ?></a> </lable>
                        <?php  }

                }
// Chekcing for last page if current page is last  then check it's previous pages by subtracting  -1, -2
                else if ($intLastPage==$intPage) {
                    if ($intPage - 2 >= 1 ) { ?>
                        <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('',<?php echo $intPage - 2; ?>)"><?php echo $intPage -2; ?></a> </lable>
                        <?php } if (($intPage - 1) >= 1 ) { ?>
                        <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('',<?php echo $intPage - 1; ?>)"><?php echo $intPage -1; ?></a> </lable>
                        <?php } ?>
                    <lable> <a href="javascript:void(0);" class="active-page"><?php echo $intPage; ?></a> </lable>

                    <!-- Chekcing for current pages between first and last -->
                    <?php } else {
                    if (($intPage - 1) >= 1) { ?>
                        <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('',<?php echo $intPage - 1; ?>)"><?php echo $intPage - 1; ?></a> </lable>
                        <?php } ?>
                    <lable> <a href="javascript:void(0);" class="active-page"><?php echo $intPage; ?></a> </lable>
                    <?php if (($intPage + 1) <= $intLastPage ) { ?>
                        <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('',<?php echo $intPage + 1; ?>)"><?php echo $intPage + 1; ?></a> </lable>

                        <!-- Chekcing for next last button -->
                        <?php }
                         } if ($intNext == 0) {?>
                    <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('next')"><img src= '<?php echo url('sites/all/themes/threebl/images/next_ico.png',array('absolute' => TRUE));?>' align='absmiddle' title="Next"></a></lable>
                    <lable> <a href="javascript:void(0);" onclick="javascript:clientMicroListAjax('last')" title="Last"><img src= '<?php echo url('sites/all/themes/threebl/images/last_ico.png',array('absolute' => TRUE));?>' align='absmiddle'></a></lable>
                    <?php }?>
                </div>
       <?php } ?>

    </div>
        </div>
	</div>
</div>