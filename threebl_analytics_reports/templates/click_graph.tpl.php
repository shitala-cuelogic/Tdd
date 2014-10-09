<script type="text/javascript" src="https://www.google.com/jsapi"></script>
<style type="text/css">
    h3 {background: none;padding: 0px;font-size: 20px;}
   .clsOnLoadImage {text-align: center;padding: 40px 0 40px 0;clear: both;}
</style>
<?php
// array_pop for getting last value of dates.
$intCount = count($arrDts);
$arrDatePop = array_pop($arrDts);

//check value exits or not for view chart
if (!empty($arrClickChart)) { //view chart
    $intMax = max($arrClickChart);
    //echo "<pre>"; print_r($arrClickChart);die;
    ?>
<script type="text/javascript">
    var queryString = '';
    var dataUrl = '';

    function onLoadCallbackC()
    {
        if (dataUrl.length > 0) {
            var query = new google.visualization.Query(dataUrl);
            query.setQuery(queryString);
            query.send(handleQueryResponseC);
        } else {
            var dataTableC = new google.visualization.DataTable();
            dataTableC.addRows(<? echo (int)$intCount; ?>);
            dataTableC.addColumn('number');
            dataTableC.addColumn('number');
            <?php
            $arrDisplayDates = array('0', '2', '4', '6', '8');
            $intCount = 0;
            $arrDate = array();
            foreach ($arrClickChart as $strDateSpan => $intClickCount) {
                if (in_array($intCount, $arrDisplayDates)) {
                    $arrDate[] = $strDateSpan; //"Nov 11 - Nov 17 Nov 25 - Dec 01 Dec 09 - Dec 15 Dec 23 - Dec 29 Jan 06 - Jan 10"
                }
                ?>
                dataTableC.setValue(<?php echo $intCount; ?>, 0, null);
                dataTableC.setValue(<?php echo $intCount; ?>, 1, <?php echo $intClickCount;?>);
                <?php $intCount++;
            } ?>
            drawC(dataTableC);
        }
    }

    function drawC(dataTableC)
    {
        var vis1 = new google.visualization.ImageChart(document.getElementById('click_chart_div'));
        var options = {
            chxl:'0:|<? echo implode('|', $arrDate); ?>',
            chxp:'',
            chxr:'0,0,3|1,0,<? echo (int)$intMax; ?>',
            chxs:'',
            chxtc:'',
            chxt:'x,y',
            chs:'800x300',
            cht:'lc',
            chd:'t:<?php echo implode(',', (array_values($arrClickChart)));?>',
            chco:'000000,00AACC',
            chdl:'Sum for Week|Clicks',
            chg:'',
            chls:'0.75,-1,-1|2,4,1',
            chma:'|0,5',
            chm:'o,000000,1,-1,7',
            chtt:'Click Graph'
        };
        vis1.draw(dataTableC, options);
    }

    function handleQueryResponseC(response)
    {
        if (response.isError()) {
            // alert('Error in query: ' + response.getMessage() + ' ' + response.getDetailedMessage());
            return;
        }
        drawC(response.getDataTable());
    }

    google.load("visualization", "1", {packages:["imagechart"]});
    google.setOnLoadCallback(onLoadCallbackC);

</script>
<?php } ?>


<table width="80%" cellpadding="2" cellspacing="2">
    <tr>
        <td><?php  if (!empty($arrClickChart)) {
            // checking if Clickchart array is empty for not if not show graph.if yes then show no analytics image.
            ?>
            <div id="click_chart_div"></div>
            <?php } else { ?>
            <div class="clsOnLoadImage"><img
                src='<?php echo url('sites/all/themes/threebl/images/justmeans/no_datafor_analytics.png', array('absolute' => TRUE));?>'
                align='absmiddle'></div>
            <?php } ?>    </td>
    </tr>
</table>


