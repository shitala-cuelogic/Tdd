<?php
//include file
module_load_include('inc', 'threebl_widgets', 'inc/widget_analytics');

/**
 *  Function return url for related node
 *
 * @param $intNodeId : is the node id from 3BL
 *
 * @return string
 */
function getCompanyURL($intNodeId)
{
    $result = db_select('url_alias', 'n')
        ->fields('n')
        ->condition('source', 'node/' . $intNodeId, '=')
        ->execute()
        ->fetchAssoc();
    return trim($result['alias']);
}


