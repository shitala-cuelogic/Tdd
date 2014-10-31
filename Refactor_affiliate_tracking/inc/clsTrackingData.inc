<?php 

class clsTrackingData {
	
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
        $objVerticalIds = $this->fnGetaffiliatesOfTypeWidget($intFMRId);
        
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
        
        #Return Widget records information
        return $objWidgets =  db_query($strQryWidget)->fetchAll();
    }

    public function fnGetaffiliatesOfTypeWidget($intFMRId) {

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

       

      return  $objVerticalIds = db_query($strVerticalCondition)->fetchAll();
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
        $objVerticalIds = $this->fnGetaffiliatesOfTypeWidget($intFMRId);
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
     * Getting the node id by title.
     * @param string $strTitle : Title
     * @return mixed
     */
    public function fnGetNodeIdByTitle($strTitle)
    {
        return db_query("SELECT nid FROM node WHERE trim(title) LIKE '".addslashes(trim($strTitle))."'")->fetchField();
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

         //Include all required files
        module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsTracking');

        //Object of class
        $objTracking = new clsTracking();

        $arrFMRClickData = $objTracking->fnGetFMRNewsLetterClickCount($objFMRClickInfo);

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

        //Include all required files
        module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsTracking');

        //Object of class
        $objTracking = new clsTracking();

        $arrFMRViewData = $objTracking->fnGetFMRNewsLetterViewCount($objFMRViewInfo);
       
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

            //Include all required files
            module_load_include('inc', 'Refactor_affiliate_tracking', 'inc/clsTracking');

            //Object of class
            $objTracking = new clsTracking();

            $arrGetCompanyName = $objTracking->fnGetAffiliateCompanyNameIfExists($objCompany,$arrGetCompanyName);

        }
        return $arrGetCompanyName;
    }

    

}


?>