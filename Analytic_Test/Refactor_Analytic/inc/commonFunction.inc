<?php
    /**
     *  File contain common helper functions
     * Date: 29/10/14
     */


    /**
     * This function Removes special character's added through Msword.
     *
     * @param sting $strMsWordText : MS word text
     * @param string $strSection
     * @return string
     */
    function fnRemoveWordFormatting($strMsWordText, $strSection = "")
    {
        $search = array(
            '/(%E2%80%93|%u2013|%u2026|%u2014|%96|%E2%80%94)/i',
            '/(%E2%80%9C|%E2%80%9D|%u201C|%u201D|%93|%94)/i',
            '/(%E2%80%98|%E2%80%99|%u2018|%u2019|%91|%92|%E2%80%9B)/i',
            '/(%95%09)/',
            '/(%E2%80%A6)/',
            '/(%E2%80%9A)/',
            '/(%E2%80%8B)/',
            '/(%E2%80%A8)/'
        );

        $replace = array(
            '-',
            '"',
            "'",
            "- ",
            "...",
            ",",
            " ",
            " "
        );

        $strMsWordText = preg_replace($search, $replace, $strMsWordText);
        $strMsWordUrlDecodeText = urldecode(preg_replace($search, $replace, urlencode($strMsWordText)));

        if ($strSection == "widget") {
            $strMsWordFinalText = html_entity_decode(htmlentities($strMsWordUrlDecodeText));
        } else {
            $strMsWordFinalText = utf8_decode(html_entity_decode(htmlentities($strMsWordUrlDecodeText)));
        }
        return $strMsWordFinalText;
    }

    /**
     * //Get start and End date of Previous month
     * @param int $strMonthDuration : Month duration for start and end date
     *
     * @return array
     */
    function fnGetStartEndDate($strMonthDuration = 1)
    {
        $intCurMonth = date("m");

        $intCurYear = date("Y");
        // Get days in previous month
        $intDaysInPrevMonth = date('t', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));

        //Get previous month for start date
        $intStartPrevMonth = date('m', mktime(0, 0, 0, $intCurMonth - $strMonthDuration, 1, $intCurYear));

        //Get previous month for end date
        $intEndPrevMonth = date('m', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));

        //Get previous month year for start date
        $intStartPrevMonthYear = date('Y', mktime(0, 0, 0, $intCurMonth - $strMonthDuration, 1, $intCurYear));

        //Get previous month year for end date
        $intEndPrevMonthYear = date('Y', mktime(0, 0, 0, $intCurMonth - 1, 1, $intCurYear));

        //Get previous month date
        $intPrevMonthDate = date('d', mktime(0, 0, 0, $intEndPrevMonth, 0 + $intDaysInPrevMonth, $intCurYear));

        // Start date
        $strGetStartDate = "$intStartPrevMonthYear-$intStartPrevMonth-01";
        // End date
        $strGetEndDate = "$intEndPrevMonthYear-$intEndPrevMonth-$intPrevMonthDate";

        return $arrStartEndDate = array("start_date" => $strGetStartDate, "end_date" => $strGetEndDate);
    }

    /**
     * Function calculate previous date as per the requirement
     *
     * @param string $strToday    : Starting data for calculation
     * @param int    $intNoOfDays : total number of days.
     * @param int    $intInterval : collection of days in array.
     *
     * @return array
     */
    function fnGetPreviousDates($strToday, $intNoOfDays = 30, $intInterval = 1)
    {
        $strStart = date("Y-m-d", strtotime(date("Y-m-d", strtotime($strToday)) . " - $intNoOfDays days"));
        $arrDts[] = $strToday;
        while (strtotime($strStart) <= strtotime($strToday)) {
            $strNow = date("Y-m-d", strtotime(date("Y-m-d", strtotime($strToday)) . " - $intInterval days"));
            $arrDts[] = $strNow;
            $strToday = $strNow;
        }
        return $arrDts;
    }

    /**
     * Check session value that is company id && nid
     */
    function fnCheckSession()
    {
        $intCompanyNid = $_SESSION['client_og_nid']; // nod id
        $intCompanyOGId = $_SESSION['client_og']; // company og id
        if (trim($intCompanyNid) == '' || trim($intCompanyOGId) == '') {
            drupal_goto('client-multiple-login');
        }
    }
