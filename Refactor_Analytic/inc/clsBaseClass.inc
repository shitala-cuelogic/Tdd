<?php
//This is class for Analytics functions.

abstract class clsBaseClass
{
    protected  $strCurrentDate;
    protected $strChartPrevDate;

    public function __construct()
    {
        $this->setDateTimes();
    }

    /**
     * Set date and time
     */
    public function setDateTimes()
    {
        $this->strCurrentDate = date("Y-m-d", (time() - 86400));
        $this->strChartPrevDate = date("Y-m-d", strtotime(date("Y-m-d", strtotime($this->strCurrentDate)) . " - 121 days"));
    }

    /**
     * Class destructor
     */
    public function __destruct()
    {

    }
}
