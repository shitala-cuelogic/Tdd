<?php
class clsTrackingModifier {
	
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

    public function fnUpdateTrackingFlag($intId) {

      db_update('field_data_field_prconnect_tracking')
                ->fields(array('field_prconnect_tracking_value'=>'1'))
                ->condition('entity_id', $intId, '=')
                ->execute();
    }

     public function fnInsertPRConnectTrackingForFMR($arrContent, $arrAffiliate, $intId) {

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

    }
}

?>