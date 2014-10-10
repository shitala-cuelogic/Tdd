<?php

interface affiliatesControllerInterface extends DrupalEntityControllerInterface
{
    public function create();
    public function save($entity);
    public function delete($entity);
}

/**
 * affiliatesController extends DrupalDefaultEntityController.
 * Our subclass of DrupalDefaultEntityController lets us add a few
 * important create, update, and delete methods.
 */
class affiliatesController extends DrupalDefaultEntityController implements affiliatesControllerInterface
{
    /**
     * Create and return a new affiliates entity.
     */
    public function create()
    {
        $entity = new stdClass();
        $entity->type = 'affiliates';
        $entity->id = 0;
        $entity->bundle_type = 'prconnect';
        return $entity;
    }

    /**
     * Saves the custom fields using drupal_write_record()
     * @param $entity : object of an entity
     *
     * @return mixed
     */
    public function save($entity)
    {
        // If our entity has no id, then we need to give it a
        // time of creation.
        if (empty($entity->id)) {
            $entity->created = time();
        }
        // Invoke hook_entity_presave().
        module_invoke_all('entity_presave', $entity, 'affiliates');
        // The 'primary_keys' argument determines whether this will be an insert
        // or an update. So if the entity already has an ID, we'll specify
        // id as the key.
        $primary_keys = $entity->id ? 'id' : array();
        // Write out the entity record.
        drupal_write_record('affiliates', $entity, $primary_keys);
        // We're going to invoke either hook_entity_update() or
        // hook_entity_insert(), depending on whether or not this is a
        // new entity. We'll just store the name of hook_entity_insert()
        // and change it if we need to.
        $invocation = 'entity_insert';
        // Now we need to either insert or update the fields which are
        // attached to this entity. We use the same primary_keys logic
        // to determine whether to update or insert, and which hook we
        // need to invoke.
        if (empty($primary_keys)) {
            field_attach_insert('affiliates', $entity);
        } else {
            field_attach_update('affiliates', $entity);
            $invocation = 'entity_update';
        }
        // Invoke either hook_entity_update() or hook_entity_insert().
        module_invoke_all($invocation, $entity, 'affiliates');
        return $entity;
    }

    /**
     * Delete a single entity.
     *
     * Really a convenience function for delete_multiple().
     * @param $entity
     */
    public function delete($entity)
    {
        $this->delete_multiple(array($entity));
    }

    /**
     * Delete one or more affiliates entities.
     * Deletion is unfortunately not supported in the base
     * DrupalDefaultEntityController class.
     * An array of entity IDs or a single numeric ID.
     * @param $entities
     * @throws Exception
     */
    public function delete_multiple($entities)
    {
        $ids = array();
        if (!empty($entities)) {
            $transaction = db_transaction();
            try {
                foreach ($entities as $entity) {
                    // Invoke hook_entity_delete().
                    module_invoke_all('entity_delete', $entity, 'affiliates');
                    field_attach_delete('affiliates', $entity);
                    $ids[] = $entity->id;
                }
                db_delete('affiliates')
                    ->condition('id', $ids, 'IN')
                    ->execute();
            } catch (Exception $e) {
                $transaction->rollback();
                watchdog_exception('threebl_widget_code', $e);
                throw $e;
            }
        }
    }
}

//This is class for Tracking Function.

class clsTracking extends affiliatesController implements affiliatesControllerInterface
{
    private $intCompanyId;
    private $strDate;
    public $AffiliateActive;
    public $AffiliateInactive;
    public $TrackingInclude;
    public $TrackingExclude;
    public $TrackingReport;
    public $NewsStatusFullText;
    public $NewsStatusHeadline;

    /**
     * class constructor
     */
    public function clsTracking($intCompany = 0)
    {
        $this->intCompanyId = $intCompany;
        $this->strDate = date("Y-m-d");

        //Affiliate Status
        $this->AffiliateActive = 113040;
        $this->AffiliateInactive =113041;

        //Tracking Status
        $this->TrackingInclude = 113036;
        $this->TrackingExclude = 113037;
        $this->TrackingReport =  113038;

        //3bl News Status
        $this->NewsStatusFullText = 113030;
        $this->NewsStatusHeadline = 113031;

        $this->strCurrentDate = date("Y-m-d H:i:s", (time() - 86400)); //(time()-86400) date("Y-m-d", strtotime("2013-12-16"))//
    }

    /**
     * Getting distinct nid ,title using condition (Get Latest FMR Result)
     * @param int $intFMRId : FMR Id
     * @param int $intCompanyId :company Id
     * @return mixed
     */
    public function fnGetLatestFMR($intFMRId = 0, $intCompanyId = 0) //old name
    {
        $strSql ='';
        $strSql.= "SELECT DISTINCT n.nid,n.title
			 FROM  og
			 JOIN og_membership ogm ON og.gid = ogm.gid
			 JOIN node AS n ON (n.nid = ogm.etid AND ogm.entity_type = 'node')
			 JOIN field_data_field_fmr_date_time AS d ON d.entity_id = n.nid
			 WHERE n.status = 1 AND n.type IN('fmr','efmr')";

        if ($intFMRId == 0) {
            //For searching checking company id
            $strCondition = ($intCompanyId > 0)? "  AND og.etid = $this->intCompanyId ORDER BY d.field_fmr_date_time_value DESC LIMIT 0, 60 " :" AND og.etid = $this->intCompanyId  AND (d.field_fmr_date_time_value >= '2012-12-18' AND d.field_fmr_date_time_value <= DATE_SUB (NOW(), INTERVAL 48 hour) ) ORDER BY d.field_fmr_date_time_value DESC";
            $strSql.= $strCondition;
            return  db_query($strSql)->fetchAll();

        } else {
            return  db_query($strSql)->fetchField();
        }
    }

    /**
     * Function fetch all affiliates related to asking FMR
     * @param  int $intFMRId : Fmr Id
     * @param bool $boolExcel : Excel
     * @param int $intFmrLanguageType : FMR Language (eg. non-english, english)
     * @return mixed
     */
    public function fnTrackingAffiliateList($intFMRId, $boolExcel = false, $intFmrLanguageType = 0)
    {
        //Getting all the status array.
        if ((int)$intFMRId > 0) {
            //Getting distinct nid from
            $intFmr = $this->fnGetLatestFMR($intFMRId); // int FMR Id

            if ((int)$intFmr > 1) {
                //Collect Widget tracking result for FMR
                $arrWidget = $this->fnWidgetTracking($intFMRId, $boolExcel, $intFmrLanguageType);
                #Collect Tracking result for 3BL and PRConnect affiliates.
                //Checking where condition for excel
                $strWhere = ($boolExcel != false)? " AND ts.field_tracking_status_tid != '".$this->TrackingExclude."' " : " AND ts.field_tracking_status_tid = '".$this->TrackingInclude."'";

                $strFrom = "  FROM eck_affiliates AS a
							  LEFT JOIN  field_data_field_vertical_fmr_taxonomy  AS v ON a.id = v.entity_id
                              LEFT JOIN field_data_field_affiliate_news_url  AS an ON a.id = an.entity_id
                              LEFT JOIN field_data_field_affiliate_status AS affs ON a.id = affs.entity_id
                              LEFT JOIN field_data_field_3bl_news_status AS affnews ON a.id = affnews.entity_id
                              LEFT JOIN field_data_field_tracking_status AS ts ON a.id = ts.entity_id
                              LEFT JOIN field_data_field_affiliate_monthly_traffic as mt ON a.id = mt.entity_id
                              LEFT JOIN field_data_field_affiliate_title AS at ON a.id = at.entity_id
                              LEFT JOIN field_data_field_affiliate_logo AS al ON a.id = al.entity_id
                              LEFT JOIN file_managed AS flm ON flm.fid = al.field_affiliate_logo_fid
                              LEFT JOIN field_data_field_affiliate_description AS ad ON a.id = ad.entity_id
                              LEFT JOIN field_data_field_all_vertcal_category AS avc ON a.id = avc.entity_id";

                $strAFFWhere = " WHERE an.field_affiliate_news_url_value !=''
                                AND affs.field_affiliate_status_tid ='".$this->AffiliateActive."'
                                AND flm.filename != ''
                                AND ad.field_affiliate_description_value != '' ";



                // For widget we manage the condition for 3bl
                $strFrom3bl =" AND t.nid = '".$intFMRId."' AND a.type ='3bl' AND affnews.field_3bl_news_status_tid = '".$this->NewsStatusFullText."' ";

                // For widget we manage the condition prconnect
                $strFromPRCconnect =" AND t.nid = '".$intFMRId."' AND a.type ='prconnect' ";

                $strGroupBy = $strWhere." GROUP BY a.id  ORDER BY rand()";

                //To Select the records
                $strSelect = " SELECT a.id, a.type, at.field_affiliate_title_value, t.tracking_url, an.field_affiliate_news_url_value, mt.field_affiliate_monthly_traffic_value,  ad.field_affiliate_description_value, flm.filename ";


                //3BL & Prconnect has compulsary to exist their records into the tracking table.
                $strCondition = " JOIN affiliate_tracking AS t ON a.id = t.aff_id ";

                // To get the affiliates of type '3bl' for the FMR
                $strQry3bl = $strSelect.$strFrom.$strCondition.$strAFFWhere.$strFrom3bl.$strGroupBy;

                // To get the affiliates of type 'prconnect' for the FMR
                $strQryPRCconnect = $strSelect.$strFrom.$strCondition.$strAFFWhere.$strFromPRCconnect.$strGroupBy;

                //Limits are only required for the front end display results.
                if ($boolExcel == false) {
                    $strQry3bl .= " LIMIT 0,5";
                    $strQryPRCconnect .= " LIMIT 0,10";
                    if ($_GET['print'] == 1) {
                        echo "<br/>3BL: ";
                        echo $strQry3bl .= " LIMIT 0,5";
                        echo "<br/><br/>Prconnect: ";
                        echo $strQryPRCconnect .= " LIMIT 0,10";
                    }
                }

                //Combine all query result (widget,3bl,prconnect)
                $strQryAffiliates = "( ".$strQry3bl." ) UNION ( ".$strQryPRCconnect." )";

                $arrAffiliates = array();
                $arrAffiliates =  db_query($strQryAffiliates)->fetchAll();

                #Return Widget, 3BL and PRConnect information
                return array($arrWidget, $arrAffiliates);

            } else {
                return false;
            }
        }
    }

    /**
     * Function to fetch PRConnect for PDF version of tracking report
     *
     * @param  int $intFMRId  : Fmr Id
     * @param bool $boolExcel : Excel
     * @param int $intFmrLanguageType : FMR Language (eg. non-english, english)
     *
     * @return mixed
     */
    public function fnTrackingAffiliatePdfList($intFMRId, $boolExcel = false, $intFmrLanguageType = 0)
    {
        //Getting all the status array.
        if ((int) $intFMRId > 0) {
            //Getting distinct nid from
            $intFmr = $this->fnGetLatestFMR($intFMRId); // int FMR Id

            if ((int) $intFmr > 1) {
                //Collect Widget tracking result for FMR
                $arrWidget = $this->fnWidgetPDFTracking($intFMRId, $boolExcel, $intFmrLanguageType);
                #Collect Tracking result for 3BL and PRConnect affiliates.
                //Checking where condition for excel
                $strWhere = ($boolExcel != false) ? " AND ts.field_tracking_status_tid != '" . $this->TrackingExclude . "' " : " AND ts.field_tracking_status_tid = '" . $this->TrackingInclude . "'";

                $strFrom = " FROM eck_affiliates AS a
                             LEFT JOIN field_data_field_affiliate_logo AS al ON a.id = al.entity_id
							 LEFT JOIN  field_data_field_vertical_fmr_taxonomy  AS v ON a.id = v.entity_id
                             LEFT JOIN field_data_field_affiliate_news_url  AS an ON a.id = an.entity_id
                             LEFT JOIN field_data_field_affiliate_status AS affs ON a.id = affs.entity_id
                             LEFT JOIN field_data_field_tracking_status AS ts ON a.id = ts.entity_id
                             LEFT JOIN field_data_field_affiliate_monthly_traffic as mt ON a.id = mt.entity_id
                             LEFT JOIN field_data_field_affiliate_title AS at ON a.id = at.entity_id
                             LEFT JOIN file_managed AS flm ON flm.fid = al.field_affiliate_logo_fid
                             LEFT JOIN field_data_field_affiliate_description AS ad ON a.id = ad.entity_id
                             LEFT JOIN field_data_field_all_vertcal_category AS avc ON a.id = avc.entity_id";

                $strAFFWhere = " WHERE an.field_affiliate_news_url_value !=''
                                 AND affs.field_affiliate_status_tid ='" . $this->AffiliateActive . "'
                                 AND flm.filename != ''
                                 AND ad.field_affiliate_description_value != '' ";

                // For widget we manage the condition prconnect
                $strFromPRCconnect = " AND t.nid = '" . $intFMRId . "' AND a.type ='prconnect' ";

                $strGroupBy = $strWhere . " GROUP BY a.id  ORDER BY rand()";

                //To Select the records
                $strSelect = " SELECT a.id, a.type, flm.filename, at.field_affiliate_title_value, t.tracking_url, an.field_affiliate_news_url_value, mt.field_affiliate_monthly_traffic_value,  ad.field_affiliate_description_value, flm.uri";


                //Prconnect has compaulsary to exist their records into the tracking table.
                $strCondition = " JOIN affiliate_tracking AS t ON a.id = t.aff_id ";

                // To get the affiliates of type 'prconnect' for the FMR
                $strQryPRCconnect = $strSelect . $strFrom . $strCondition . $strAFFWhere . $strFromPRCconnect . $strGroupBy;

                //Limits are only required for the front end display results.
                if ($boolExcel == false) {
                    $strQryPRCconnect .= " LIMIT 0,10";
                    if ($_GET['print'] == 1) {
                        echo "<br/>3BL: ";
                        echo "<br/><br/>Prconnect: ";
                        echo $strQryPRCconnect .= " LIMIT 0,10";
                    }
                }

                //query result for prconnect
                $strQryAffiliates = $strQryPRCconnect;
                $arrAffiliates = db_query($strQryAffiliates)->fetchAll();

                #Return Widget, PRConnect information
                return array($arrWidget, $arrAffiliates);

            } else {
                return false;
            }
        }
    }

    /**
     * Widget Tracking records
     *
     * @param $intFMRId:  FMR nid
     * @param int $boolExcel : download tracking report flag.
     * @param int $intFmrLanguageType : FMR Language (eg. non-english, english)
     */
    public function fnWidgetTracking($intFMRId, $boolExcel = 0, $intFmrLanguageType = 0)
    {

        // To get the affiliates of type 'widget' for the FMR
        $strVerticalCondition = " SELECT t.tid
                FROM `taxonomy_term_data` AS t
                LEFT JOIN taxonomy_term_hierarchy AS th ON (th.tid = t.tid OR t.tid = th.parent)
                WHERE t.vid = 4 AND
                ( th.parent IN
                    ( SELECT st.tid
                    FROM node n1
                    LEFT JOIN field_data_field_fmr_primary_category AS p ON n1.nid = p.entity_id
                    LEFT JOIN field_data_field_dist_vertical AS v ON n1.nid = v.entity_id
                    LEFT JOIN field_data_field_fmr_taxonomy AS v1 ON n1.nid = v1.entity_id
                    LEFT JOIN taxonomy_term_data AS t ON (v1.field_fmr_taxonomy_target_id = t.tid OR v.field_dist_vertical_target_id = t.tid OR p.field_fmr_primary_category_target_id = t.tid )
                    JOIN taxonomy_term_data AS st ON (st.vid = 4 AND t.name = st.name )
                    WHERE t.tid != '' AND n1.nid = ".$intFMRId." )
                    OR
                    th.tid IN
                    ( SELECT st.tid
                    FROM node n1
                    LEFT JOIN field_data_field_fmr_primary_category AS p ON n1.nid = p.entity_id
                    LEFT JOIN field_data_field_dist_vertical AS v ON n1.nid = v.entity_id
                    LEFT JOIN field_data_field_fmr_taxonomy AS v1 ON n1.nid = v1.entity_id
                    LEFT JOIN taxonomy_term_data AS t ON (v1.field_fmr_taxonomy_target_id = t.tid OR v.field_dist_vertical_target_id = t.tid OR p.field_fmr_primary_category_target_id = t.tid )
                    JOIN taxonomy_term_data AS st ON (st.vid = 4 AND t.name = st.name )
                    WHERE t.tid != '' AND n1.nid = ".$intFMRId." )
                )
                GROUP BY t.tid ";

        $objVerticalIds = db_query($strVerticalCondition)->fetchAll();

        //Get all tids
        $arrTid = array();

        #Find all vertical category ids.
        foreach($objVerticalIds as $key => $objTid){
            $arrTid[] = $objTid->tid;
        }

        //Filter Widget by Language (non-english or english-only)
        $strJoinWidgetFMRLanguage = " JOIN field_data_field_widget_fmr_language wfl ON a.id = wfl.entity_id";
        $strORWidgetFMRLanguage   = " OR wfl.field_widget_fmr_language_value = 'all'";
        
        $strCondWidgetFMRLanguage = ($intFmrLanguageType == 1) ? " AND (wfl.field_widget_fmr_language_value = 'non_english' $strORWidgetFMRLanguage) " : " AND (wfl.field_widget_fmr_language_value = 'english_only' $strORWidgetFMRLanguage) ";

        $strSelect = " SELECT a.id, a.type, at.field_affiliate_title_value, an.field_affiliate_news_url_value, mt.field_affiliate_monthly_traffic_value,  ad.field_affiliate_description_value, flm.filename ";

        $strFrom = "  FROM eck_affiliates AS a
                      LEFT JOIN  field_data_field_vertical_fmr_taxonomy  AS v ON a.id = v.entity_id
                      LEFT JOIN field_data_field_affiliate_news_url  AS an ON a.id = an.entity_id
                      LEFT JOIN field_data_field_affiliate_status AS affs ON a.id = affs.entity_id
                      LEFT JOIN field_data_field_3bl_news_status AS affnews ON a.id = affnews.entity_id
                      LEFT JOIN field_data_field_tracking_status AS ts ON a.id = ts.entity_id
                      LEFT JOIN field_data_field_affiliate_monthly_traffic as mt ON a.id = mt.entity_id
                      LEFT JOIN field_data_field_affiliate_title AS at ON a.id = at.entity_id
                      LEFT JOIN field_data_field_affiliate_logo AS al ON a.id = al.entity_id
                      LEFT JOIN file_managed AS flm ON flm.fid = al.field_affiliate_logo_fid
                      LEFT JOIN field_data_field_affiliate_description AS ad ON a.id = ad.entity_id
                      LEFT JOIN field_data_field_all_vertcal_category AS avc ON a.id = avc.entity_id $strJoinWidgetFMRLanguage";

        $strAFFWhere = " WHERE an.field_affiliate_news_url_value !=''
                        AND affs.field_affiliate_status_tid ='".$this->AffiliateActive."'
                        AND ad.field_affiliate_description_value != '' $strCondWidgetFMRLanguage";
        //Checking where condition for excel
        $strWhere = ($boolExcel != false)? " AND ts.field_tracking_status_tid != '".$this->TrackingExclude."' " : " AND ts.field_tracking_status_tid = '".$this->TrackingInclude."' AND flm.filename != ''";

        $strGroupBy = $strWhere." GROUP BY a.id  ORDER BY rand()";

        //For widget we manage the Taxonomy condition and widget type
        $strFromWidget = " AND ( (avc.field_all_vertcal_category_value ='1') OR v.field_vertical_fmr_taxonomy_tid IN (".implode(',', $arrTid).") ) AND a.type ='widget'";

        // To get the affiliates of type widget for the FMR
        $strQryWidget = $strSelect.$strFrom.$strAFFWhere.$strFromWidget.$strGroupBy;

        //Check excel report condition
        if ($boolExcel == false) {
            $strQryWidget .= " LIMIT 0,8";
        }
        if($_GET['print'] == 1){
            echo $strVerticalCondition;
            echo "<br/>>>>";
            echo $strQryWidget; die;
        }
        #Return Widget records information
        return $objWidgets =  db_query($strQryWidget)->fetchAll();
    }

    /**
     * Widget PDF Version of Tracking report
     *
     * @param     $intFMRId  :  FMR nid
     * @param int $boolExcel : download tracking report flag.
     * @param int $intFmrLanguageType : FMR Language (eg. non-english, english)
     */
    public function fnWidgetPDFTracking($intFMRId, $boolExcel = 0, $intFmrLanguageType = 0)
    {
        // To get the affiliates of type 'widget' for the FMR
        $strVerticalCondition = " SELECT t.tid
                FROM `taxonomy_term_data` AS t
                LEFT JOIN taxonomy_term_hierarchy AS th ON (th.tid = t.tid OR t.tid = th.parent)
                WHERE t.vid = 4 AND
                ( th.parent IN
                    ( SELECT st.tid
                    FROM node n1
                    LEFT JOIN field_data_field_fmr_primary_category AS p ON n1.nid = p.entity_id
                    LEFT JOIN field_data_field_dist_vertical AS v ON n1.nid = v.entity_id
                    LEFT JOIN field_data_field_fmr_taxonomy AS v1 ON n1.nid = v1.entity_id
                    LEFT JOIN taxonomy_term_data AS t ON (v1.field_fmr_taxonomy_target_id = t.tid OR v.field_dist_vertical_target_id = t.tid OR p.field_fmr_primary_category_target_id = t.tid )
                    JOIN taxonomy_term_data AS st ON (st.vid = 4 AND t.name = st.name )
                    WHERE t.tid != '' AND n1.nid = " . $intFMRId . " )
                    OR
                    th.tid IN
                    ( SELECT st.tid
                    FROM node n1
                    LEFT JOIN field_data_field_fmr_primary_category AS p ON n1.nid = p.entity_id
                    LEFT JOIN field_data_field_dist_vertical AS v ON n1.nid = v.entity_id
                    LEFT JOIN field_data_field_fmr_taxonomy AS v1 ON n1.nid = v1.entity_id
                    LEFT JOIN taxonomy_term_data AS t ON (v1.field_fmr_taxonomy_target_id = t.tid OR v.field_dist_vertical_target_id = t.tid OR p.field_fmr_primary_category_target_id = t.tid )
                    JOIN taxonomy_term_data AS st ON (st.vid = 4 AND t.name = st.name )
                    WHERE t.tid != '' AND n1.nid = " . $intFMRId . " )
                )
                GROUP BY t.tid ";

        $objVerticalIds = db_query($strVerticalCondition)->fetchAll();

        //Get all tids
        $arrTid = array();

        #Find all vertical category ids.
        foreach ($objVerticalIds as $key => $objTid) {
            $arrTid[] = $objTid->tid;
        }

        //Filter Widget by Language (non-english or english-only)
        $strJoinWidgetFMRLanguage = " JOIN field_data_field_widget_fmr_language wfl ON a.id = wfl.entity_id";
        $strORWidgetFMRLanguage   = " OR wfl.field_widget_fmr_language_value = 'all'";
        
        $strCondWidgetFMRLanguage = ($intFmrLanguageType == 1) ? " AND (wfl.field_widget_fmr_language_value = 'non_english' $strORWidgetFMRLanguage) " : " AND (wfl.field_widget_fmr_language_value = 'english_only' $strORWidgetFMRLanguage) ";

        $strSelect = " SELECT a.id, a.type, at.field_affiliate_title_value, an.field_affiliate_news_url_value, mt.field_affiliate_monthly_traffic_value,  ad.field_affiliate_description_value, flm.filename, pub.field_publisher_value, fb.field_widget_facebook_value, tw.field_widget_twitter_value, ln.field_widget_linkedin_value, tm.field_widget_tumblr_value, gp.field_widget_google_plus_value, yt.field_widget_youtube_value, flm.uri";

        $strFrom = "  FROM eck_affiliates AS a
                      LEFT JOIN field_data_field_affiliate_logo AS al ON a.id = al.entity_id
                      LEFT JOIN  field_data_field_vertical_fmr_taxonomy  AS v ON a.id = v.entity_id
                      LEFT JOIN field_data_field_affiliate_news_url  AS an ON a.id = an.entity_id
                      LEFT JOIN field_data_field_affiliate_status AS affs ON a.id = affs.entity_id
                      LEFT JOIN field_data_field_tracking_status AS ts ON a.id = ts.entity_id
                      LEFT JOIN field_data_field_affiliate_monthly_traffic as mt ON a.id = mt.entity_id
                      LEFT JOIN field_data_field_affiliate_title AS at ON a.id = at.entity_id
                      LEFT JOIN field_data_field_publisher AS pub ON a.id = pub.entity_id
                      LEFT JOIN file_managed AS flm ON flm.fid = al.field_affiliate_logo_fid
                      LEFT JOIN field_data_field_affiliate_description AS ad ON a.id = ad.entity_id
                      LEFT JOIN field_data_field_all_vertcal_category AS avc ON a.id = avc.entity_id
                      LEFT JOIN field_data_field_widget_facebook AS fb ON a.id = fb.entity_id
                      LEFT JOIN  field_data_field_widget_twitter AS tw ON a.id = tw.entity_id
                      LEFT JOIN field_data_field_widget_linkedin AS ln ON a.id = ln.entity_id
                      LEFT JOIN field_data_field_widget_tumblr AS tm ON a.id = tm.entity_id
                      LEFT JOIN field_data_field_widget_google_plus AS gp ON a.id = gp.entity_id
                      LEFT JOIN field_data_field_widget_youtube AS yt ON a.id = yt.entity_id $strJoinWidgetFMRLanguage";

        $strAFFWhere = " WHERE an.field_affiliate_news_url_value !=''
                        AND affs.field_affiliate_status_tid ='" . $this->AffiliateActive . "'
                        AND flm.filename != ''
                        AND ad.field_affiliate_description_value != '' $strCondWidgetFMRLanguage";
        //Checking where condition for excel
        $strWhere = ($boolExcel != false) ? " AND ts.field_tracking_status_tid != '" . $this->TrackingExclude . "' " : " AND ts.field_tracking_status_tid = '" . $this->TrackingInclude . "'";

        $strGroupBy = $strWhere . " GROUP BY a.id  ORDER BY rand()";

        //For widget we manage the Taxonomy condition and widget type
        $strFromWidget = " AND ( (avc.field_all_vertcal_category_value ='1') OR v.field_vertical_fmr_taxonomy_tid IN (" . implode(',', $arrTid) . ") ) AND a.type ='widget'";

        // To get the affiliates of type widget for the FMR
        $strQryWidget = $strSelect . $strFrom . $strAFFWhere . $strFromWidget . $strGroupBy;

        //Check excel report condition
        if ($boolExcel == false) {
            $strQryWidget .= " LIMIT 0,8";
        }
        if ($_GET['print'] == 1) {
            echo $strVerticalCondition;
            echo "<br/>>>>";
            echo $strQryWidget;
            die;
        }
        #Return Widget records information
        return $objWidgets = db_query($strQryWidget)->fetchAll();
    }

    /**
     * Function fetch all affiliates related to asking FMR
     * @param  int $intEFMRId : Fmr Id
     * @return mixed
     */
    public function fnTracking_EFMR_AffiliateList($intEFMRId)
    {
        //Getting all the status array.
        if ((int)$intEFMRId > 0) {

            //Getting distinct nid from
            $intFmr = $this->fnGetLatestFMR($intEFMRId); // int FMR Id

            if ((int)$intFmr > 1) {
                // To get the affiliates of type 'widget' for the FMR
                //  Remove this join -  JOIN jm_3bl_media AS fmr ON (fmr.3bl_mediaid = n1.nid AND fmr.prconnect_tracking != '1')
                // Adding new table  field_data_field_prconnect_tracking for field_prconnect_tracking_value

                //Checking where condition for excel
                $strWhere = " AND ts.field_tracking_status_tid != '".$this->TrackingExclude."' ";

                $strFrom = "  FROM eck_affiliates AS a
							  LEFT JOIN  field_data_field_vertical_fmr_taxonomy  AS v ON a.id = v.entity_id
                              LEFT JOIN field_data_field_affiliate_news_url  AS an ON a.id = an.entity_id
                              LEFT JOIN field_data_field_affiliate_status AS affs ON a.id = affs.entity_id
                              LEFT JOIN field_data_field_3bl_news_status AS affnews ON a.id = affnews.entity_id
                              LEFT JOIN field_data_field_tracking_status AS ts ON a.id = ts.entity_id
                              LEFT JOIN field_data_field_affiliate_monthly_traffic as mt ON a.id = mt.entity_id
                              LEFT JOIN field_data_field_affiliate_title AS at ON a.id = at.entity_id
                              LEFT JOIN field_data_field_affiliate_logo AS al ON a.id = al.entity_id
                              LEFT JOIN file_managed AS flm ON flm.fid = al.field_affiliate_logo_fid
                              LEFT JOIN field_data_field_affiliate_description AS ad ON a.id = ad.entity_id
                              LEFT JOIN field_data_field_all_vertcal_category AS avc ON a.id = avc.entity_id";

                $strAFFWhere = " WHERE an.field_affiliate_news_url_value !=''
                                AND affs.field_affiliate_status_tid ='".$this->AffiliateActive."'
                                AND flm.filename != ''
                                AND ad.field_affiliate_description_value != '' ";

                // For widget we manage the condition prconnect
                $strFromPRCconnect =" AND t.nid = '".$intEFMRId."' AND a.type ='prconnect' ";

                $strGroupBy = $strWhere." GROUP BY a.id  ORDER BY rand()";

                //To Select the records
                /*t.tracking_url, a.title, a.affiliate_news_url, a.affiliate_type, a.monthly_traffic, a.description, a.monthly_traffic, a.logo, a.aff_id*/
                $strSelect = " SELECT a.id, a.type, at.field_affiliate_title_value, t.tracking_url, an.field_affiliate_news_url_value, mt.field_affiliate_monthly_traffic_value,  ad.field_affiliate_description_value, flm.filename ";

                //3BL & Prconnect has compulsary to exist their records into the tracking table.
                $strCondition = " JOIN affiliate_tracking AS t ON a.id = t.aff_id ";

                // To get the affiliates of type 'prconnect' for the FMR
                $strQryPRCconnect = $strSelect.$strFrom.$strCondition.$strAFFWhere.$strFromPRCconnect.$strGroupBy;

                //Combine all query result (widget,3bl,prconnect)
                $strQryAffiliates = $strQryPRCconnect;

                $arrAffiliates = array();
                return $arrAffiliates =  db_query($strQryAffiliates)->fetchAll();
            } else {
                return false;
            }
        }
    }

    /**
     * getting all title and link form 3bl_affiliate
     * @return array
     */
    public function fnGetAllTitleOfAffiliate() //  old name -getAllTitleOfAffiliate
    {
        $arrAffiliate = array();
        //getting the entity details
        $arrEntity = entity_load('affiliates', false, array("type"=>"prconnect"));
        $arrAffEntity = json_decode(json_encode($arrEntity), 1); //object to array

        if (!empty($arrAffEntity)) {
            foreach ($arrAffEntity as $arrTitle) {
                if ($arrTitle['field_affiliate_title']['und'][0]['value'] != '') {
                    $arrAffiliate[$arrTitle['id']] =  $arrTitle['field_affiliate_title']['und'][0]['value'];
                }
            }
        }
        return $arrAffiliate;
    }

    /**
     * Function which will fetch all affiliate those are from 3bl
     * and get all FMR information from their sites and insert it into affiliate_tracking table.
     */
    public function fnTrackingAffiliates()
    {
        # Get all the affiliates
        $arrEntity = entity_load('affiliates', false, array("type"=>"3bl"));
        $arrAffEntity = json_decode(json_encode($arrEntity), 1); //object to array
        if (!empty($arrAffEntity)) {
            $intNoOfPages = 3; // number of HTML pages to parse
            foreach ($arrAffEntity as $arrAffData) {
                //field_affiliate_status = 108419 = Active
                //field_3bl_news_status = 108417 = Full Text
                if ($arrAffData['field_affiliate_status']['und'][0]['tid']== $this->AffiliateActive && $arrAffData['field_3bl_news_status']['und'][0]['tid']== $this->NewsStatusFullText && $arrAffData['field_affiliate_news_url']['und'][0]['value'] != '') {
                    $strLink = $arrAffData['field_affiliate_news_url']['und'][0]['value'];
                    $strQS  = $arrAffData['field_affiliate_query_string']['und'][0]['value'];
                    $strUrlPrefix = $arrAffData['field_affiliate_prefix']['und'][0]['value'];
                    $intAffId = $arrAffData['id'];
                    $strPaging = $arrAffData['field_affiliate_paging']['und'][0]['value'];

                    $arrElements = array(
                        'title' => $strQS,
                        'url'   => $strQS.'/@href'
                    );
                    # Start to get html and filter html
                    $arrData = $this->fnParseHTML($strLink, $arrElements, $strPaging, $intNoOfPages);
                    $intCnt = count($arrData);
                    for ($k = 0; $k < $intCnt; $k++) {
                        //CHECK if already exist
                        $intCount = $this->fnDuplicateTrackingUrl($strUrlPrefix.$arrData[$k]['url']);

                        if ($intCount == 0) {
                            // Fetch FMR node_id
                            //Getting the node id by node title.
                            $intNodeId = $this->fnGetNodeIdByTitle($arrData[$k]['title']);
                            if ($intNodeId > 0) {
                                //Tracking data array
                                $arrTrackingDate = array('nid'=>$intNodeId,
                                    'aff_id'=>$intAffId,
                                    'tracking_url'=> $strUrlPrefix.$arrData[$k]['url'],
                                    'added_date'=> $this->strDate
                                );
                                $this->fnInsertAffTracking($arrTrackingDate); // Add value in affiliate_tracking
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Function for prconnect cron
     * @param int $intId : FMR id
     * @return bool|string
     */
    public function fnPrconnectCron($intId = 0)
    {
        global $conf;
        //$intId is the FMR id
        if ((int)$intId == 0) {
            return;
        }
        global $base_url;

        // get all affiliate from 3bl_affiliate tables by
        $arrAffiliate = $this->fnGetAllTitleOfAffiliate();

        //prconnect tracking link
        $strURL = "http://tracking.prconnect.com/3blmedia?Module=clipping-all&SourceID=".$intId;
        $fileContents = file_get_contents($strURL);
        $fileContents = str_replace(array("\n", "\r", "\t"), '', $fileContents);
        $fileContents = trim(str_replace('"', "'", $fileContents));
        $simpleXml    = simplexml_load_string($fileContents, null, LIBXML_NOCDATA);

        $i=0;
        $arrComp = array();
        $arrContent = array();
        // To process the links from the parsed html
        if (count($simpleXml->channel->item) > 0) {
            foreach ($simpleXml->channel->item as $objChannelItem) {
                $strSource = (string) $objChannelItem->source[0];
                $arrContent[$i]['title'] = (string) addslashes($objChannelItem->title[0]);
                $arrContent[$i]['affiliate_news_url'] = (string) addslashes($objChannelItem->link[0]);
                $arrContent[$i]['affiliate_home_url'] = (string) $objChannelItem->source[0];

                //store all the affiliate title in particular array
                $arrComp[$i] =  (string)($objChannelItem->title[0]);
                $i++;
            }
        } else {
            return "2";
        }
        #Check the FMR distribute on the PRConnect or not.
        if (count($arrComp) < 1) {
            return "2";
        }
        //Get array diff from above getting array and existing database affiliate title array for key mapping
        $arrDiffer = array_diff($arrComp, $arrAffiliate);

        #If there is any new affiliate present in the list then email to admin about that.
        if (!empty($arrDiffer)) {
            // All system mails need to specify the module and template key (mirrored from
            // hook_mail()) that the message they want to send comes from.
            $strModule = 'email';
            $strMailKey = 'new_prconnect_message';

            // Specify 'to' and 'from' addresses.
            $strTo = $conf['prconnect_cron_distribution_to'];
            $strFrom = $conf['no_reply_from'];
            $strMailContent = "Hi,<br><br>We got new PRconnect affiliate(s) in the affiliate admin section. Please check the following details:<br><br>";
            $i = 1;
            foreach ($arrDiffer as $key => $value) {
                // insert unique record in affiliate table
                $strLink = (string) $arrContent[$key]['affiliate_news_url'];
                $strLnk = strstr($strLink, "news/", true);
                if ($strLnk != '') {
                    $strLnk.="news/channelinfo?ChannelID=6906";
                } else {
                    $strLnk = $strLink;
                }
                $arrContent[$key]['affiliate_news_url'] = (string) $strLnk;

                //Create an entity object to insert the affiliate into the db
                $entity = new stdClass();
                $entity->type = 'prconnect';
                $entity->id = 0;
                $entity->field_affiliate_title = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' =>$arrContent[$key]['title']
                        )
                    )
                );
                $entity->field_affiliate_news_url = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' =>$arrContent[$key]['affiliate_news_url']
                        )
                    )
                );
                $entity->field_affiliate_source = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' =>$arrContent[$key]['affiliate_home_url']
                        )
                    )
                );
                $entity->field_affiliate_status = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'tid' => $this->AffiliateInactive // inactive status (108420)
                        )
                    )
                );
                $entity->field_tracking_status = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'tid' => $this->TrackingInclude       //Include Status (108421)
                        )
                    )
                );

                $entity->field_affiliate_description = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' => ""
                        )
                    )
                );
                $entity->field_affiliate_monthly_traffic = array
                (
                    'und' => array
                    (
                        "0" => array
                        (
                            'value' => "0"
                        )
                    )
                );

                $aff_id = $this->fnInsertAffiliate($entity);

                unset($entity);
                $arrAffiliate[$aff_id] = $arrContent[$key]['title'];
                $strEditLink = $base_url."/admin/affiliate/edit?aid=".$aff_id;
                $strMailContent .= $i.". ".stripslashes($arrContent[$key]['title'])." <a href='".$strEditLink."'>(Edit)</a><br>
									Source link: ".stripslashes((string) $arrContent[$key]['affiliate_home_url'])."<br>
									News link: ".stripslashes($strLnk)."<br><br>";
                $strLnk = '';
                $i++;
            }//foreach
            $strMailContent .="Thanks,<br>3BL Media.<br>";
            $arrParams['message'] = $strMailContent;
            $arrParams['cc'] = $conf['tech_email'];
            $strLanguage = language_default();
            // Whether or not to automatically send the mail when drupal_mail() is
            // called. This defaults to TRUE, and is normally what you want unless you
            // need to do additional processing before drupal_mail_send() is called.
            $boolSend = true;
            // Send the mail, and check for success. Note that this does not guarantee
            // message delivery; only that there were no PHP-related issues encountered
            // while sending.

            drupal_mail($strModule, $strMailKey, $strTo, $strLanguage, $arrParams, $strFrom, $boolSend);
        }//Differ-array

        #Insert PRConnect Tracking for the FMR.
        foreach ($arrContent as $arrValue) {
            $intAffID = array_search($arrValue['title'], $arrAffiliate);
            $arrEntityAff = entity_load('affiliates', array($intAffID));
            $arrResult = json_decode(json_encode($arrEntityAff[$intAffID]), 1); //object to array
            $strStatus = $arrResult['field_affiliate_status']['und'][0]['tid'];

            if ((int)$strStatus == $this->AffiliateActive) {
                //To check if the tracking url already exists
                $intCount =$this->fnDuplicateTrackingUrl(trim($arrValue['affiliate_news_url']));

                if ((int)$intCount == 0) {
                    //Tracking data array
                    $arrTrackingDate = array('nid'=>$intId,
                        'aff_id'=>$intAffID,
                        'tracking_url'=>$arrValue['affiliate_news_url'],
                        'added_date'=> $this->strDate
                    );
                    $this->fnInsertAffTracking($arrTrackingDate); // Add value in  affiliate_tracking
                }
            }
        }

        //Checking Count of FMR if Yes Then Update Flag.
        if (count($arrComp) > 0) {
            //update the prconnect tracking flag in field_data_field_prconnect_tracking table
            db_update('field_data_field_prconnect_tracking')
                ->fields(array('field_prconnect_tracking_value'=>'1'))
                ->condition('entity_id', $intId, '=')
                ->execute();
        }
        return true;
    }

    /**
     * Function for insert affiliate
     * @param $objEntity
     * @return mixed
     */
    public function fnInsertAffiliate(&$objEntity)
    {
        entity_get_controller('affiliates')->save($objEntity);
        return $objEntity->id;
    }

    /**
     * Function for delete affiliate
     * @param array $arrIds : array of affiliates Ids
     */
    public function fnDeleteAffiliate($arrIds = array())
    {
        if (count($arrIds) > 0) {
            $arrEntity = entity_load('affiliates', $arrIds);
            $arrAffEntity = json_decode(json_encode($arrEntity), 1); //object to array
            entity_get_controller('affiliates')->delete($arrAffEntity);
        }
    }

    /**
     * Insert value in affiliate_tracking table
     * @param array $arrData : Array of tracking data
     * @return DatabaseStatementInterface|int|null
     */
    public function fnInsertAffTracking($arrData = array())
    {
        return $aff_id  = db_insert('affiliate_tracking')->fields($arrData)->execute();
    }

    /**
     * Checking Tracking news url exits or not
     * @param string $strUrl : Tracking url
     * @return mixed
     */
    public function fnDuplicateTrackingUrl($strUrl)
    {
        $strQuery = "SELECT count(*)as cnt FROM affiliate_tracking
        WHERE tracking_url = '".$strUrl."'";
        return db_query($strQuery)->fetchField();
    }

    /**
     * Getting the node id by title.
     * @param string $strTitle : Title
     * @return mixed
     */
    public function fnGetNodeIdByTitle($strTitle)
    {
        return db_query("SELECT nid FROM node WHERE trim(title) LIKE '".addslashes(trim($strTitle))."'")->fetchField();
    }

    /**
     * Getting  old affiliate  logo
     * @param string $strLogo : name of Logo
     * @return array
     */
    public function fnLogo($strLogo)
    {
        $_FILES['image']['name'] = $strLogo;
        $_FILES['image']['tmp_name'] = '/var/www/vhosts/3blmedia.com/httpdocs/sites/default/files/affiliate_logo_live/scale/'.$strLogo;
        $_FILES['image']['type'] = 'image/jpeg';
        $_FILES['image']['error'] = 0;
        //Get the uploaded file from the temp directory.
        $image = file_get_contents($_FILES['image']['tmp_name']);

        //Saves a file to the specified destination and creates a database entry.
        $objFile = file_save_data($image, 'public://affiliates/'.$_FILES['image']['name'], FILE_EXISTS_RENAME);

        //Set the file status to permanent so it is not deleted in next cron run
        $objFile->status = FILE_STATUS_PERMANENT;
        file_save($objFile);
        return (array) $objFile;
    }

    /**
     * Getting Tracking Url from affiliate_tracking table redirect to the affiliate URL
     * @param int $intNid : Node Id
     * @param int $intAffId : Affiliate Id
     * @return mixed
     */
    public function getAffiliateTrackingUrl($intNid = 0, $intAffId = 0)
    {
        $strQuery = "SELECT tracking_url FROM affiliate_tracking
        WHERE nid = '".$intNid."' AND aff_id = '".$intAffId."'";
        return db_query($strQuery)->fetchField();
    }

    /**
     *    Function to insert the tracking data from the given links.
     *    $strLink 		: url of the html page to be parsed.
     *    $arrElements 	: array of parameters as keys and DOM query string as value.
     *    Ex.: array(
     *    'title' => '//h2/a',
     *    'url'  => '//h2/a/@href'
     *    );
     *    $strPaging 		: pageing parameters for the link specified by $strLink
     *	   $intNoOfPages 	: number of pages to parse
     */
    public function fnParseHTML($strLink = '', $arrElements = '', $strPaging = '', $intNoOfPages = 0)
    {
        $arrData = array();
        $dom = new DOMDocument();
        $fileContents = '';
        if ($strPaging == '') {
            $fileContents = file_get_contents($strLink);
        } else {
            // Loop to get the HTML contents for the number of pages specified by $intNoOfPages
            for ($intCount = 1; $intCount <= $intNoOfPages; $$intCount++) {
                if ($intCount > 1) {
                    // #pgno is used to replace the string with the page number to get the contents of that page
                    $strTmp = str_replace("#pgno", $intCount, $strPaging);
                }
                // Concat HTML of all the pages
                $fileContents .= @file_get_contents($strLink.$strTmp);
            }
        }

        @$dom->loadHTML($fileContents);
        $xpath = new DOMXPath($dom);

        foreach ($arrElements as $strKey => $strVal) {
            $arrParams = @$xpath->query($strVal);
            $intCounter = 0;
            if (is_array($arrParams)) {
                foreach ($arrParams as $arrParam) {
                    $arrData[$intCounter++][$strKey] = $arrParam->childNodes->item(0)->nodeValue;
                }
            }
        }
        return $arrData;
    }

    /**
     * Function fetch the records for the affiliate tracking.
     * @param string $strType : Type
     * @return mixed
     */
    public function fnFMRTracking($strType = "3BL")
    {
        $sqlLimit ='';
        //GET LATEST FMR
        $sqlSelect  = " SELECT n1.nid, n1.title, d.field_fmr_date_time_value as date,
        pt.field_prconnect_tracking_value as prflag ";
        $sqlFrom    = " FROM node AS n1
                                 JOIN field_data_field_fmr_date_time AS d ON d.entity_id = n1.nid
                                 JOIN field_data_field_prconnect_tracking AS pt ON pt.entity_id = n1.nid
                                 LEFT JOIN affiliate_tracking AS at ON at.nid = n1.nid  ";

        $sqlWhere   = " WHERE n1.type = 'fmr' AND n1.status = 1
        AND ( d.field_fmr_date_time_value <= DATE_SUB( NOW() , INTERVAL 24 HOUR)
        AND  d.field_fmr_date_time_value >= DATE_SUB( NOW() , INTERVAL 30 DAY ))
        AND ((at.nid IS NULL) OR (pt.field_prconnect_tracking_value = 0)) ";

        $sqlGroupBy = " GROUP BY n1.nid ";
        $sqlOrderBy = " ORDER BY prflag, d.field_fmr_date_time_value DESC ";
        $sqlQuery 	 =  $sqlSelect." ".$sqlFrom." ".$sqlWhere." ".$sqlGroupBy." ".$sqlOrderBy." ".$sqlLimit;
        return  $arrFMRList  = db_query($sqlQuery)->fetchAll();
    }

    /**
     * Function to get FMR Click count
     *
     * @param int $intJMNodeId : Justmeans Node Id
     *
     * @return array
     */
    public function fnGetFMRNewsLetterClicks($intJMNodeId)
    {
        $strSQL = "SELECT COUNT(C.mediaid) AS clickcount
                   FROM jm_analytic_clicks AS C WHERE C.mediaid = '$intJMNodeId' AND C.channel = 'Jm-Newsletter'";
        $objFMRClickInfo = db_query($strSQL)->fetchAll();

        $arrFMRClickData = array();
        if (!empty($objFMRClickInfo)) {
            foreach ($objFMRClickInfo as $arrFMRClickInfo) {
                $arrFMRClickData['clickcount'] = $arrFMRClickInfo->clickcount;
            }
        }
        return $arrFMRClickData;
    }

    /**
     * Function to get FMR View count
     *
     * @param int $intJMNodeId : Justmeans Node Id
     *
     * @return array
     */
    public function fnGetFMRNewsLetterViews($intJMNodeId)
    {
        $strSQL = "SELECT SUM(v.totalcount) AS viewcount
                   FROM jm_analytic_views AS v WHERE v.mediaid = '$intJMNodeId' AND v.channel = 'Jm-Newsletter'";
        $objFMRViewInfo = db_query($strSQL)->fetchAll();

        $arrFMRViewData = array();
        if (!empty($objFMRViewInfo)) {
            foreach ($objFMRViewInfo as $arrFMRViewInfo) {
                $arrFMRViewData['viewcount'] = $arrFMRViewInfo->viewcount;
            }
        }
        return $arrFMRViewData;
    }

    /**
     * To Get FMR Information
     *
     * @param int $intJMNodeId :  Justmeans Node Id
     *
     * @return array
     */
    public function fnGetFMRInformation($intJMNodeId)
    {
        $strSQL = "SELECT n.title, n.created, ft.field_fmr_type_of_content_value as type
                   FROM node AS n
                   JOIN field_data_field_fmr_type_of_content AS ft ON (ft.entity_id = n.nid)
                   WHERE nid = '$intJMNodeId'";
        $arrFMRInfo = db_query($strSQL)->fetchAll();

        return $arrFMRInfo;
    }

    /**
     * To Get Justmeans Node Id
     *
     * @param int $intFMRId :  FMR Id
     *
     * @return string
     */
    public function fnGetJMNodeId($intFMRId)
    {
        $strSQL = "SELECT nid_jm
                   FROM nid_to_nid WHERE nid_3bl = '$intFMRId'";
        $intJMNodeId = db_query($strSQL)->fetchField();

        if ((int) $intJMNodeId > 0) {

            return $intJMNodeId;
        }
        return '';
    }

    /**
     * To Get Company Name
     *
     * @param $intJMNodeId : Justmeans Node Id
     *
     * @return mixed
     */
    public function fnGetCompanyInfo($intJMNodeId)
    {
        $strSQL = "SELECT og.label, og.etid
                   FROM og AS og
                   JOIN field_data_group_audience AS ga ON (ga.group_audience_gid  = og.gid)
                   WHERE entity_id = '$intJMNodeId'";
        $arrCompanyInfo = db_query($strSQL)->fetchAll();

        return $arrCompanyInfo;
    }

    /**
     * Function Create PDF and send email to respective company
     *
     * @param int    $intCompanyNid  : Company Node Id
     * @param string $strCompanyName : Company Name
     * @param array  $arrRecipient   : Recipients email array
     * @param array  $arrFMRInfo     : FMR Info
     *
     * @return bool|DatabaseStatementInterface|int|null
     */
    public function fnAffiliateReportsByCompany($intCompanyNid, $strCompanyName, $arrRecipient, $arrFMRInfo)
    {
        // Base Url
        global $base_url, $conf;
        $strTime = time();
        $strServerName = $_SERVER['HTTP_HOST'];

        $strSender = "info@3blmedia.com";
        $strSenderName = "3blmedia Team";       

        if (is_array($arrFMRInfo) && count($arrFMRInfo) >0) {
            $intFMRId = $arrFMRInfo['nid'];

            ## FMR Detail Report Attachment
            if ((int)$intFMRId >0) {
            
                // Build query URL
                $strUrl = http_build_query(array('fmr' => $intFMRId));

                $strMediaTitle = stripslashes($arrFMRInfo['title']);
                $strMediaUrl = $base_url."/".$arrFMRInfo['alias'];

                // Destination URL
                $strSrc1 = $base_url."/Dashboard/Analytics/Affiliate/PDF/".base64_encode($strUrl);
                $strApiKey = '31q8AjO7u0Ej';
                $strResultPDF = file_get_contents("http://api.htm2pdf.co.uk/urltopdf?apikey=$strApiKey&url=$strSrc1");
                
                 // PDF file path
                $strFilePath = "/var/www/vhosts/".$strServerName."/httpdocs/sites/default/files/pdf/TrackingResults_".$intFMRId."_".$intCompanyNid."_$strTime.pdf";
                $strAffiliatesDetailReport = $strFilePath;
                file_put_contents($strFilePath, $strResultPDF);
            
                $strHeaderImagePath = $base_url."/".$conf['IMAGES_PATH_3BL']."/ConfirmHeader.png";
                $strFooterImagePath = $base_url."/".$conf['IMAGES_PATH_3BL']."/ConfirmFooter.png";

                //Set Subject for report cron email on the basis of report type
                $strSubject = "3BL Media: Tracking Report";

                //Set the message for the email on the basis of report type
                $strMessage = "<div><img src='$strHeaderImagePath'/></div><br/>Attached to this message is the Tracking Report for your most recent* Flexible Media Release (FMR) distributed through the 3BL Media platform (<a href='$strMediaUrl'>".$strMediaTitle."</a>).<br/><br/>You can view and download additional Tracking Reports, as well as our full suite of Analytics, at any time via<br/> 
                  ".$base_url."/Dashboard/Analytics/Views  (you must be logged in)<br/><br/>If you have any questions, please contact your media consultant at 866.508.0993 or reply to this message.<br/><br/>Thank you,<br/>The 3BL Media Team<br/><br/>* FMRs must have been distributed at least 48 hours ago to be included.<br/><br/><div><img src='$strFooterImagePath'/></div>";

                //User list those who got report Email
                $arrReceivedReport = array();

                //Send email with attachments
                if (is_array($arrRecipient) && count($arrRecipient) >0) {
                    foreach ($arrRecipient as $strRecipientAddress) {
                        $boolSent = fnSendEmail($strSenderName, $strSender, $strRecipientAddress, $strSubject, $strMessage, $strAffiliatesDetailReport);

                        if ($boolSent) {
                            $arrReceivedReport[] = $strRecipientAddress;
                        }
                    }  
                }

                //Delete file after email those attachment.
                @unlink($strAffiliatesDetailReport);
                
                if (count($arrReceivedReport) > 0) {
                    //If mail sent successfuly then insert these details in the table
                    $strRecipients = implode(", ", $arrReceivedReport);

                    // Send Recipients information to Media Consultant
                    $strRecipientsInfoHTML = $this->fnGetRecipientsInfoHTML($strCompanyName, $strRecipients, $strMediaTitle, $strMediaUrl);

                    return $strRecipientsInfoHTML;
                }
            }
        }
    }

    /**
     * Get Latest FMR
     *
     * @param int $intCompanyId : Company Id
     *
     * @return array
     */
    public function fnGetAffiliatesLatestFMR($intCompanyId = 0)
    {
        if ((int)$intCompanyId > 0) {
             $strSql = "SELECT n.nid, n.title, U.alias  
                        FROM  field_data_group_audience AS ga
                        JOIN node AS n ON (n.nid = ga.entity_id AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                        JOIN field_data_field_fmr_date_time AS d ON d.entity_id = n.nid
                        JOIN url_alias AS U ON U.source = CONCAT( 'node/', n.nid )
                        WHERE n.status = 1 AND n.type ='fmr' AND ga.group_audience_gid = $intCompanyId
                        AND d.field_fmr_date_time_value >= DATE_SUB( '$this->strCurrentDate', INTERVAL 216 HOUR) 
                        AND d.field_fmr_date_time_value <= DATE_SUB( '$this->strCurrentDate', INTERVAL 48 HOUR)
                        ORDER BY d.field_fmr_date_time_value DESC LIMIT 0,1";
            $arrFMRInfo = db_query($strSql)->fetchAssoc();

            //fetch all FMRs
            if (count($arrFMRInfo) >0) {
                return $arrFMRInfo;
            }
        }
        return "";
    }

    /**
     * To Build Recipients information HTML (Company name and Email)
     *
     * @param string $strCompanyName : Company Name
     * @param string $strRecipients  : Comma separated email address
     * @param string $strMediaTitle  : FMR Tile
     * @param string $strMediaUrl    : FMR Url
     *
     * @return string
     */
    public function fnGetRecipientsInfoHTML($strCompanyName, $strRecipients, $strMediaTitle, $strMediaUrl)
    {
        //Include all required files
        module_load_include('inc', 'threebl_analytics_reports', 'inc/clsReports');

        //Object of class
        $objReports = new ClsReports();

        $strGetMediaTitle = $objReports->fnRemoveWordFormatting($strMediaTitle);
        return $strHTML = "<tr><td>$strCompanyName</td><td>$strRecipients</td><td><a href='$strMediaUrl'>$strGetMediaTitle</a></td></tr>";
    }

    /**
     * To Send Recipients Information to Media Consultant
     *
     * @param string $strRecipientsHTML : Recipients Info HTML
     *
     * @return string
     */
    public function fnSendRecipientInfoToMCs($strRecipientsHTML)
    {
        $strCurrentDate = date('F j, Y');

        $intCurMonth = date("m", strtotime($this->strCurrentDate));
        $intCurYear = date("Y", strtotime($this->strCurrentDate));
        $intCurDay = date("d", strtotime($this->strCurrentDate));

        $strGetStartDate = date('F j, Y', mktime(0, 0, 0, $intCurMonth, $intCurDay - 9, $intCurYear));
        $strGetEndDate   = date('F j, Y', mktime(0, 0, 0, $intCurMonth, $intCurDay - 2, $intCurYear));
        
        $strHTML = "<table width='100%' cellpadding='0' cellspacing='0' border='1' >
                    <tr><td colspan='3' style='font-size:14px;''>Weekly Tracking Reports were sent on $strCurrentDate for the period from $strGetStartDate to $strGetEndDate.</td></tr>
                    <tr><td colspan='3'>&nbsp;</td></tr>
                    <tr><td><strong>Company Name</strong></td><td><strong>Recipients Email</strong></td><td><strong>FMR Title</strong></td></tr>".$strRecipientsHTML."</table>";

        // Get Server Name
        $strServerName = $_SERVER['HTTP_HOST'];
        //Created xls file Path
        $strRecipientsReport = "/var/www/vhosts/".$strServerName."/httpdocs/sites/default/files/excel/Tracking_Report_Recipients".time().".xls";

        // Open and write HTML content to xls file
        file_put_contents($strRecipientsReport, $strHTML);

        //Set Subject for report cron email on the basis of report type
        $strSubject = "Notice to MCs: Tracking Reports sent";

        //Set the message for the email on the basis of report type
        $strMessage =  "Dear MCs:<br/><br/>The weekly process to send Tracking Reports to clients was just completed, for the period from $strGetStartDate to $strGetEndDate. If a client published any FMRs in this period, they received a PDF of the Tracking Report for the most recent FMR in the period.  If a client did NOT have activity in the period, then no message was sent to them.<br /><br />We have attached a log of recipients - Company and email address(es) along with the relevant FMR - to this message.<br/><br/>Thank you,<br/>The 3BL Media Tech Team<br/>";

        $strRecipientMail = "dfulton@3blmedia.com, consult@3blmedia.com";

        // Send Monthly report
        fnSendEmail("3BL Media Admin", "no-reply@3blmedia.com", $strRecipientMail, $strSubject, $strMessage, $strRecipientsReport);

        //Unlink Micro List report excel file after mail sent
        if (is_file($strRecipientsReport)) {
            @unlink($strRecipientsReport);
        }
    }

    /**
     * Function return name of requested company id
     *
     * @param string $strCompanyNid : Comma Separated Company Node Id
     *
     * @return string
     */
    public function fnGetAffiliateCompanyName($strCompanyNid = 0)
    {
        $arrGetCompanyName = array();

        if ($strCompanyNid != "") {

            $objCompany = db_query("SELECT title, nid FROM node WHERE nid IN ($strCompanyNid)")->fetchAll();

            if (is_array($objCompany) && count($objCompany) > 0) {
                foreach ($objCompany as $objCompanyName) {
                    $arrGetCompanyName[$objCompanyName->nid] = $objCompanyName->title;
                }
            }
        }
        return $arrGetCompanyName;
    }

    /**
     * Class destructor
     */
    public function __destruct()
    {

    }
}
