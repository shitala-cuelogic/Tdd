<?php
/**
 * Function fnGetPagerData is pagination function
 *
 * @param int    $intTotal        : Total Records per page
 * @param int    $intLimit        : Query Limit
 * @param string $strTempAction   : Temporary Action
 * @param int    $isFirstLast     : First and Last Page Flag
 * @param string $strAnchorParam  : <a> value
 * @param string $strGetPageParam : Page No. Value
 * @param string $strUseImages    : Image
 *
 * @return array
 */
function fnGetPagerData($intTotal, $intLimit, $strTempAction = "", $isFirstLast = 1, $strAnchorParam = "", $strGetPageParam = "page", $strUseImages = "")
{

    $strPrevPage = " ";
    if (isset($_GET[$strGetPageParam])) {
        $intPage = $_GET[$strGetPageParam];
    } else {
        $intPage = 1;
    }
    $strPageLink = $strTempAction;

    if (!$intPage) {
        $intPage = 1;
    }

    $intTotal = (int) $intTotal;
    $intLimit = max((int) $intLimit, 1);
    $intNumPages = ceil($intTotal / $intLimit);

    $intPage = max($intPage, 1);
    $intPage = min($intPage, $intNumPages);

    $intOffset = ($intPage - 1) * $intLimit;

    $isLessFlag = 0;
    /* TEMPARORY CLASS FOR OBJECT FORWARDING */

    if ($isFirstLast == 1) {
        $strFnLinkCls = "fnlink";
        $strCOnPageCls = "conpage";
    } else {
        $strFnLinkCls = "";
        $strCOnPageCls = "";
    }

    if ($intPage <= 1) { // this is the first page - there is no previous page
        $strPrevPage = " ";
    }
    ######### Current Pages #########
    if ($intPage >= 1 && $intPage <= 5) {
        $intPreCnt = 1;
        $isLessFlag = 1;
    } else {
        //$intTempPre = $intPage / 2;
        $intPreCnt = $intPage - 5;
    }

    if ($intPage >= ($intNumPages - 4) && $intPage <= $intNumPages) {
        $intPostCnt = $intNumPages;

        if ($intPage == $intNumPages && $isLessFlag == 0) {
            $intPreCnt = $intPage - 9;
        } elseif ($intPage >= ($intNumPages - 4) && $intPage < $intNumPages && $isLessFlag == 0) {
            $intPreCnt = $intNumPages - 9;
        } else {
            if ($intPage == 1) {
                $intPostCnt = $intPage + 9;
            } elseif ($intPage > 1 && $intPage <= 5) {
                $intPostCnt = $intPage + (10 - $intPage);
            } else {
                $intPostCnt = $intPage + 4;
            }
        }

        if ($intPostCnt > $intNumPages) {
            $intPostCnt = $intNumPages;
        }
        $strCurrPage = "";
        $strCurrPage .= "<ul class='reset fr pagination'>";

        if ($intPage <= 1) { // this is the first page - there is no previous page
            //$strTempPrevPage = " <a href='javascript:void(0);' class='fnlink'>Previous</a> | ";
            $strTempPrevPage = " ";
        } else {
            // not the first page, link to the previous page
            if ($strUseImages == 1) {
                $strCurrPage .= "<li class='first'><a class='btn'  href=\"$strPageLink&$strGetPageParam=1$strAnchorParam" . "\" class='$strFnLinkCls'>First</a> </li>";
                $strCurrPage .= "<li class='first prev'><a class='btn'  href=\"$strPageLink&$strGetPageParam=" . ($intPage - 1) . "$strAnchorParam" . "\" class='$strFnLinkCls'>Prev</a> </li>";
            } else {
                $strCurrPage .= "<li class='first'><a href=\"$strPageLink&$strGetPageParam=1$strAnchorParam" . "\" class='$strFnLinkCls'>First</a> </li>";
                $strCurrPage .= "<li class='first prev'><a href=\"$strPageLink&$strGetPageParam=" . ($intPage - 1) . "$strAnchorParam" . "\" class='$strFnLinkCls'>Prev</a> </li>";
            }
        }
        for ($intTempCnt = $intPreCnt; $intTempCnt <= $intPostCnt; $intTempCnt++) {
            if ($intTempCnt > 0) {
                if ($intTempCnt == $intPage) {
                    $strCurrPage .= "<li><a href='javascript:void(0);' class='active'>$intTempCnt</a> </li>";
                } else {
                    $strCurrPage .= "<li><a href=\"$strPageLink&$strGetPageParam=" . $intTempCnt . $strAnchorParam . "\" class='$strCOnPageCls'>$intTempCnt</a></li>";
                }
                //$strCurrPage .= "<a href=\"index.php?$strParams&$strGetPageParam=" . $intTempCnt . "\" class='$strCOnPageCls'>$intTempCnt</a> ";
            }
        }
        if ($intPage == $intNumPages) { // this is the last page - there is no next page
            $strTempNextPage = " ";
        } else { // not the last page, link to the next page
            $strCurrPage .= "<li class='first'><a href=\"$strPageLink&$strGetPageParam=" . ($intPage + 1) . "$strAnchorParam" . "\" class='$strFnLinkCls'>Next</a> </li>";
            $strCurrPage .= "<li class='first prev'><a href=\"$strPageLink&$strGetPageParam=" . ($intNumPages) . "$strAnchorParam" . "\" class='$strFnLinkCls'>Last</a> </li>";
        }
        $strCurrPage .= "<li class='pageinfo'>$intPage of $intNumPages</li></ul>";
        ######### Current Pages #########
        $strNextPage = "";
        if ($intPage == $intNumPages) { // this is the last page - there is no next page
            $strNextPage = " $strTempNextPage | <a href='javascript:void(0);' class='fnlink'>Last</a>";
            $strNextPage = " ";
        }
        // use pager values to fetch data
        if ($intOffset < 0) {
            $intOffset = 0;
        }
        /*-------------------*/
        //print $intOffset." ".$intLimit;
        $hdlReturn = array();
        $hdlReturn['intOffset'] = $intOffset;
        $hdlReturn['intLimit'] = $intLimit;
        $hdlReturn['strNext'] = $strNextPage;
        $hdlReturn['strCurr'] = $strCurrPage;
        $hdlReturn['intNumPages'] = $intNumPages;
        return $hdlReturn;
    }
}

