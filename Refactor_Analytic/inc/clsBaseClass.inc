<?php
//This is class for Analytics functions.

abstract class clsBaseClass
{
    protected  $strCurrentDate;
    protected $strChartPrevDate;
    protected $arrShowMediaType;
    protected $strViewTableName;
    protected $strClickTableName;

    public function __construct()
    {
        $this->setDateTime();
        $this->setMediaType();
    }

    /**
     * Set date and time
     */
    private function setDateTime(){

        $this->strCurrentDate = date("Y-m-d", (time() - 86400));
        $this->strChartPrevDate = date("Y-m-d", strtotime(date("Y-m-d", strtotime($this->strCurrentDate)) . " - 61 days"));

        $this->strViewTableName = 'threebl_analytic_views';
        $this->strClickTableName = 'threebl_analytic_clicks';

        $this->strBenchmarkNAMonth = '2014-03-31';
    }

    /**
     * Set fmr media types array
     */
    private function setMediaType() {

        $this->arrShowMediaType = array(
            'press_release' => 'Press Release',
            'blog' => 'Blog',
            'multimedia' => 'Multimedia',
            'article' => 'Article',
            'newsletter' => 'Newsletter',
            'all' => 'All'
        );
    }

    /** To get the total clicks and views
     *
     * @param array $arrViewChart   : Array contain data of getting views for company in last 60 days
     * @param array $arrClickChart  : Array contain data of getting clicks for company in last 60 days
     *
     * @return array
     */
    protected function fnSumWeekViewsClicks($arrViewChart, $arrClickChart)
    {
        // getting previous date from today's date means we need the dates array having 7 days diffrence.(ex 1-11-2012, 7-11-2012)
        $strTodayDate = $this->strCurrentDate;
        $arrDts = $this->fnGetPreviousDates($strTodayDate, 59, 1);
        $arrDts = array_reverse($arrDts);

        //Process views and clicks and prepare 7 days interval data for graph
        $arrDates = array();
        $arrViewChartSum = $arrClickChartSum = array();
        $strStart = "";
        $intDays = $intClickSum = $intViewSum = 0;
        foreach ($arrDts as $val) {
            if ($intDays == 0) {
                $strStart = date("M d", strtotime($val));
            }
            @$intViewSum += (int)$arrViewChart[$val];
            @$intClickSum += (int)$arrClickChart[$val];
            $intDays++;
            if ($intDays == 7) {
                $arrDates[] = $strStart . " - " . date("M d", strtotime($val));
                $arrViewChartSum[$strStart . " - " . date("M d", strtotime($val))] = (int)$intViewSum;
                $arrClickChartSum[$strStart . " - " . date("M d", strtotime($val))] = (int)$intClickSum;
                $strStart = "";
                $intDays = 0;
                $intViewSum = 0;
                $intClickSum = 0;
            }
        }
        //Update Last date range data.
        if ($intDays > 0) {
            $arrDates[] = $strStart . " - " . date("M d", strtotime($val));
            $arrViewChartSum[$strStart . " - " . date("M d", strtotime($val))] = (int)$intViewSum;
            $arrClickChartSum[$strStart . " - " . date("M d", strtotime($val))] = (int)$intClickSum;
        }
        return array($arrViewChartSum, $arrClickChartSum, $arrDates);
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
    protected function fnGetPreviousDates($strToday, $intNoOfDays = 30, $intInterval = 1)
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
    protected function fnCheckSession()
    {
        $intCompanyNid = $_SESSION['client_og_nid']; // nod id
        $intCompanyOGId = $_SESSION['client_og']; // company og id
        if (trim($intCompanyNid) == '' || trim($intCompanyOGId) == '') {
            drupal_goto('client-multiple-login');
        }
    }

    /**
     * Class destructor
     */
    public function __destruct()
    {

    }
}
