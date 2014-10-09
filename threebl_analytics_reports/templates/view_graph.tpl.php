<script type="text/javascript" src="https://www.google.com/jsapi"></script>
<style type="text/css">
    h3 { background: none;padding: 0px;font-size: 20px;}
    .clsOnloadImage {text-align: center;padding: 40px 0 40px 0;clear: both;}
</style>
<?php
// array_pop for getting last value of dates.
$intDateCount = count($arrDts);
$arrDatePop = array_pop($arrDts);

//check value exits or not for view chart
if (!empty($arrViewChart)) { //view chart
    $intMax = (int)(max($arrViewChart));
    ?>

<script type="text/javascript">
    var queryString = '';
    var dataUrl = '';

    /**
     * Following Functions for google View Chart
     */
    function onLoadCallback()
    {
        if (dataUrl.length > 0) {
            var query = new google.visualization.Query(dataUrl);
            query.setQuery(queryString);
            query.send(handleQueryResponse);
        } else {
            var dataTable = new google.visualization.DataTable();
            dataTable.addRows(<? echo (int)$intDateCount; ?>);

            dataTable.addColumn('number');
            dataTable.addColumn('number');
            <?php
            $arrDisplayDates = array('0', '2', '4', '6', '8');
            $intCount = 0;
            $arrDate = array();
            foreach ($arrViewChart as $strDateSpan => $intViewCount) {
                if (in_array($intCount, $arrDisplayDates)) {
                    $arrDate[] = $strDateSpan; //"Nov 11 - Nov 17 Nov 25 - Dec 01 Dec 09 - Dec 15 Dec 23 - Dec 29 Jan 06 - Jan 10"
                }
                ?>
                dataTable.setValue(<?php echo $intCount; ?>, 0, null);
                dataTable.setValue(<?php echo $intCount; ?>, 1, <?php echo $intViewCount;?>);
                <?php $intCount++;
            } ?>
            draw(dataTable);
        }
    }

    function draw(dataTable)
    {
        var vis = new google.visualization.ImageChart(document.getElementById('view_chart_div'));
        var options = {
            chxl:'0:|<? echo implode('|', $arrDate);?>',
            chxp:'',
            chxr:'0,0,3|1,0,<? echo (int)$intMax; ?>',
            chxs:'',
            chxtc:'',
            chxt:'x,y',
            chs:'800x300',
            cht:'lc',
            chd:'t:<?php echo implode(',', (array_values($arrViewChart)));?>',
            chg:'0',
            chco:'000000,00AACC',
            chdl:'Sum for Week|Views',
            chls:'0.75,-1,-1|2,4,1',
            chma:'|0,5',
            chm:'o,000000,1,-1,7',
            chtt:'View Graph'
        };
        vis.draw(dataTable, options);
    }

    function handleQueryResponse(response)
    {
        if (response.isError()) {
            // alert('Error in query: ' + response.getMessage() + ' ' + response.getDetailedMessage());
            return;
        }
        draw(response.getDataTable());
    }

    google.load("visualization", "1", {packages:["imagechart"]});
    google.setOnLoadCallback(onLoadCallback);

</script>
<?php } ?>
<table width="80%" cellpadding="2" cellspacing="2">
    <tr>
        <td><?php  if (!empty($arrViewChart)) {
            // checking if Viewchart array is empty for not if not show graph.if yes then show no analytics image.
            ?>
            <div id="view_chart_div"></div>
            <?php } else { ?>
            <div class="clsOnloadImage"><img
                src='<?php echo url('sites/all/themes/threebl/images/justmeans/no_datafor_analytics.png', array('absolute' => true));?>'
                align='absmiddle'></div>
            <?php } ?>    </td>
    </tr>
</table>


