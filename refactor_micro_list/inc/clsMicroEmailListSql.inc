<?php

class clsMicroEmailListSql 
{

    /**
     * function for getting the email list from database
     *
     * @param int    $intListId    : List Id
     * @param int    $intCountFlag : Flag to get count of records
     * @param string $strSQL       : MySql Query
     * @param int    $intAdminFlag : Flag to check 3BL and client list
     *
     * @return mixed
     */
    public function fnEmailList($intListId = 0, $intCountFlag = 0, $strSQL = '', $intAdminFlag = 0)
    {
        $strWhereList = ''; // where condition for view
        $strSelect = '';
        //if the list for view page
        if ($intListId > 0) {

            $strSelect .= ", RN.field_real_name_value as name";

            $strWhereList .= " JOIN field_data_field_micro_list as sle ON us.uid = sle.entity_id
                             JOIN eck_micro_list as eckl ON eckl.id = sle.field_micro_list_target_id ";

            //When admin no need to check following condition
            if ($intAdminFlag == 0) {
                $strWhereList .= " JOIN field_data_group_audience as ga ON (eckl.id = ga.entity_id AND ga.entity_type = 'micro_list')
                                  JOIN og as OG ON  OG.gid  = ga.group_audience_gid ";
            }

            $strWhereList .= " LEFT JOIN field_data_field_real_name as  RN ON  us.uid = RN.entity_id
                              Where sle.field_micro_list_target_id  =" . $intListId . $strSQL;
        }
        //Checking email list
        $strEmailListQuery = " SELECT distinct us.mail,us.uid " . $strSelect . " FROM users us " . $strWhereList;

        if ($intCountFlag > 0) {
            return db_query($strEmailListQuery)->rowCount();
        } else {
            return db_query($strEmailListQuery)->fetchAll();
        }
    }


    /**
     * Getting all the email from user database.
     *
     * @return mixed
     */
    public function fnGetAllUserEmail()
    {
        $arrUserEmails = db_query('SELECT distinct mail FROM users')->fetchAll();
        return (json_decode(json_encode($arrUserEmails), 1));
    }


    /**
     * Fetch all the respective lists assigned by admin or created by client.
     *
     * @param int    $intOgId      : Company OG Id
     * @param int    $intFlag      : Flag to check client micro list and 3bl micro list
     * @param int    $intStart     : Start query Limit
     * @param int    $intLimit     : Number of list per page
     * @param string $strFlag      : Flag to check client or admin
     * @param string $strSearchVal : Search Value
     *
     * @return array
     */
    public function fnGetListByCompanyId($intOgId, $intFlag = 0, $intStart = 0, $intLimit = 25, $strFlag = "client", $strSearchVal = "")
    {
        $strQuery = db_select('eck_micro_list', 'L');
        $strQuery->join('field_data_field_email_list_details', 'D', 'L.id = D.entity_id');
        $strQuery->join('field_data_group_audience', 'GA', "L.id = GA.entity_id AND GA.entity_type = 'micro_list'");
        $strQuery->join('og', 'OG', 'OG.gid  = GA.group_audience_gid');
        $strQuery->leftjoin('field_data_field_client_micro_list_owner', 'O', 'L.id = O.entity_id');
        $strQuery->fields('L')
            ->fields('D', array('field_email_list_details_value'))
            ->fields('O', array('field_client_micro_list_owner_value'));

        # Condition for the client micro list where flag send as 1
        if ($intFlag == 2) {
            $strQuery->condition('O.field_client_micro_list_owner_value', $intOgId, '='); //$intOgId
        } elseif ($intFlag == 1) { #condition through ajax function to get both assign and created listsmicro
            $strQuery->condition(db_and()->condition('O.field_client_micro_list_owner_value', $intOgId, '!=')->condition('GA.group_audience_gid', $intOgId, '!='));
        } else { #condition TO get the 3bl micro list
            $strQuery->condition(db_and()->condition('GA.group_audience_gid', $intOgId, '=')->condition('O.field_client_micro_list_owner_value', $intOgId, '!='));
        }
        #Contcate search filter
        if ($strSearchVal != "") {
            $strQuery->condition('L.title', "%" . trim($strSearchVal) . "%", 'LIKE');
        }

        $strQuery->groupBy('L.id')->orderBy('L.title', 'ASC');
        $intCount = 0;

        if ($strFlag == "client") {
            $intCount = $strQuery->execute()->rowCount();
            $objArrResult = $strQuery->range($intStart, $intLimit)->execute()->fetchAll();
        } else {
            $objArrResult = $strQuery->execute()->fetchAll();
        }
        return array($objArrResult, $intCount);
    }



    /**
     *  Client all email by list id
     *
     * @param int $intListId : List Id
     * @param int $intOgId   : Company OG Id
     *
     * @return mixed
     */
    public function fnClientEmailListById($intListId, $intOgId)
    {
        $strQuery = db_select('users', 'U');
        $strQuery->join('field_data_field_micro_list', 'FDF', 'FDF.entity_id = U.uid');
        $strQuery->join('eck_micro_list', 'ECKL', 'ECKL.id = FDF.field_micro_list_target_id');
        $strQuery->join('field_data_group_audience', 'GA', "ECKL.id = GA.entity_id AND GA.entity_type = 'micro_list'");
        $strQuery->join('og', 'OG', 'OG.gid  = GA.group_audience_gid');
        $strQuery->join('field_data_field_unsub_all_alerts', 'N', 'U.uid = N.entity_id');
        $strQuery->join('field_data_field_unsub_all_3bl_comm', 'USA', 'U.uid = USA.entity_id');
        $strQuery->leftjoin('field_data_field_unsub_fmr_updates', 'USF', 'U.uid = USF.entity_id');
        $strQuery->leftjoin('field_data_field_real_name', 'RN', 'U.uid = RN.entity_id');
        $strQuery->where('USF.field_unsub_fmr_updates_value != 1 OR USF.field_unsub_fmr_updates_value IS NULL');
        $strQuery->fields('U', array('mail', 'uid'))
            ->fields('N', array('field_unsub_all_alerts_value'))
            ->fields('USA', array('field_unsub_all_3bl_comm_value'))
            ->fields('USF', array('field_unsub_fmr_updates_value'))
            ->fields('RN', array('field_real_name_value'))
            ->fields('ECKL', array('id'))
            ->condition('FDF.field_micro_list_target_id', $intListId, '=')
            ->condition('OG.gid', $intOgId, '=')
            ->GroupBy('U.uid')
            ->orderBy('U.mail', 'ASC');
        $arrResult = $strQuery->extend('PagerDefault')->limit(100)->execute();
        $objArrResult = $arrResult->fetchAll();
        return $objArrResult;
    }

     /**
     * Function for getting list details
     *
     * @param int $intListId    : List Id
     * @param int $intOgId      : Company OG Id
     * @param int $intCountFlag : Flag to count number of records
     *
     * @return mixed
     */
    public function fnGetListDetails($intListId, $intOgId = 0, $intCountFlag = 0)
    {
        if ($intOgId > 0) {
            $strQuery = db_select('eck_micro_list', 'L');
            //$strQuery->join('field_data_field_email_list_title', 'T', 'L.id = T.entity_id');
            $strQuery->join('field_data_field_email_list_details', 'D', 'L.id = D.entity_id');
            $strQuery->join('field_data_group_audience', 'GA', "L.id = GA.entity_id AND GA.entity_type = 'micro_list'");
            $strQuery->join('og', 'OG', 'OG.gid  = GA.group_audience_gid');
            $strQuery->join('field_data_field_client_micro_list_owner', 'O', 'L.id = O.entity_id');
            $strQuery->fields('L')
                ->fields('D', array('field_email_list_details_value'))
                ->fields('O', array('field_client_micro_list_owner_value'))
                ->condition('L.id', $intListId, '=')
                ->condition('OG.gid', $intOgId, '=');
        } else {
            $strQuery = db_select('eck_micro_list', 'L');
            $strQuery->join('field_data_field_email_list_details', 'D', 'L.id = D.entity_id');
            $strQuery->leftjoin('field_data_group_audience', 'GA', "L.id = GA.entity_id AND GA.entity_type = 'micro_list'");
            $strQuery->leftjoin('field_data_field_client_micro_list_owner', 'O', 'L.id = O.entity_id');
            $strQuery->fields('L')
                ->fields('D', array('field_email_list_details_value'))
                ->condition('L.id', $intListId, '=');
        }

        $arrResult = $strQuery->execute();
        if ($intCountFlag > 0) {
            $objArrResult = $arrResult->rowCount();
        } else {
            $objArrResult = $arrResult->fetchAssoc();
        }
        return $objArrResult;
    }


    /**
     * function fnGetUserList to get the email address list
     *
     * @param int $intListId : List Id
     */
    public function fnGetUserList($intListId)
    {
        //Getting the members in the list depending on the list id and entity id of fmr
        $strQuery = "SELECT u.mail AS mail
                    FROM field_data_field_this_is_only_a_test as fdf
                    JOIN field_data_field_micro_list as  tce ON fdf .field_this_is_only_a_test_target_id = tce.field_micro_list_target_id
                    JOIN users as u on u.uid = tce.entity_id
                    WHERE tce.field_micro_list_target_id  =" . $intListId;
        $objArrResult = db_query($strQuery)->fetchAll();
        return $objArrResult;
    }

     /**
     * Function to return published FMR Ids
     *
     * @param int   $intUserId  : User Id
     * @param Array $arrUserFMR : User's FMR
     *
     * @return object
     */
    public function fnGetPublishedFMRIds($intUserId, $arrUserFMR)
    {
        if ((int)$intUserId > 0 && is_array($arrUserFMR) && count($arrUserFMR) > 0) {
            $strFMRIds = implode(",", $arrUserFMR);

            $strGetGrmIds = "SELECT mml.entity_id
                              FROM field_data_field_3bl_micro_list AS mml
                             JOIN field_data_field_micro_list AS ml ON
                             (ml.field_micro_list_target_id = mml.field_3bl_micro_list_target_id )
                             JOIN eck_micro_list AS eml ON ( eml.id = ml.field_micro_list_target_id)
                             WHERE mml.entity_id IN ($strFMRIds) AND eml.type = 'client_micro_list'
                             AND ml.entity_id = $intUserId
                             GROUP BY mml.entity_id ";
            return db_query($strGetGrmIds)->fetchCol();
        }
    }

    /**
     * Function to get the latest published fmrs
     *
     * @return mixed
     */
    public function fnGetAllPublishedFMR()
    {
        $strFMRCustomListQuery = "SELECT N.nid, N.title, PD.field_fmr_date_time_value AS publishdate,
                                    U.alias, og.label, og.etid, smd.field_3bl_micro_list_distributio_value
                                    FROM node AS N
                                    JOIN field_data_field_fmr_date_time AS PD ON PD.entity_id = N.nid
                                    JOIN url_alias AS U ON U.source = CONCAT( 'node/', N.nid )
                                    JOIN field_data_group_audience as ga ON (ga.entity_id = N.nid AND ga.bundle = 'fmr' AND ga.entity_type = 'node')
                                    JOIN og ON ( og.gid = ga.group_audience_gid)
                                    JOIN field_data_field_3bl_micro_list AS ml ON (ml.entity_id = N.nid)
                                    LEFT JOIN field_data_field_3bl_micro_list_distributio AS  smd ON (smd. entity_id = N.nid )
                                    WHERE N.status = 1
                                    AND N.type = 'fmr'
                                    AND (PD.field_fmr_date_time_value <= NOW())
                                    AND ( smd.field_3bl_micro_list_distributio_value <> 1 OR smd. field_3bl_micro_list_distributio_value IS NULL)
                                    GROUP BY N.nid
                                    ORDER BY PD.field_fmr_date_time_value DESC";
        return db_query($strFMRCustomListQuery)->fetchAll();
    }

     /** function to get list ids assigned for FMR and user information assigned to List
     *
     * @param string $strFMRIds: Comma separated FMR Ids.
     *
     * @return object
     */
    public function fnGetFMRAssignListUserDetails($strFMRIds)
    {
        $strGetRelListFMRIds = "SELECT DISTINCT(tml.field_3bl_micro_list_target_id), tml.entity_id, eml.title
                             FROM field_data_field_3bl_micro_list AS tml
                             JOIN eck_micro_list eml ON tml.field_3bl_micro_list_target_id = eml.id
                             WHERE tml.entity_id IN ($strFMRIds)";
        $objGetRelListFMRIds = db_query($strGetRelListFMRIds)->fetchAll();

        //Include all required files
        module_load_include('inc', 'threebl_micro_list', 'inc/customEmailList');

        //Object of class
        $objcustomEmailList = new customEmailList();
        
        $arrListInfo = array();
        $arrListInfo = $objcustomEmailList->fnGetFMRAssignUserListInfo($objGetRelListFMRIds);

        return $arrListInfo;
    }

    /**
     * Function to get user information assigned to list
     *
     * @param int $strFMRListId : FMR Id.
     *
     * @return mixed
     */
    public function fnGetListUserInfo($strFMRListId)
    {
        $strGetRelListFMRIds = "SELECT ml.entity_id as userid,u.mail,ml.field_micro_list_target_id AS listid
                                FROM field_data_field_micro_list AS ml
                                JOIN users AS u ON ml.entity_id = u.uid
                                LEFT JOIN field_data_field_unsub_all_3bl_comm as un ON un.entity_id = u.uid
                                LEFT JOIN field_data_field_unsub_fmr_updates as ufu ON u.uid = ufu.entity_id
                                WHERE ml.field_micro_list_target_id = '$strFMRListId'
                                AND un.field_unsub_all_3bl_comm_value <> 1
                                AND (field_unsub_fmr_updates_value <> 1 Or field_unsub_fmr_updates_value is NULL)";
        return db_query($strGetRelListFMRIds)->fetchAll();
    }

    /**
     * Query for getting particular fmr custom email list with user email
     *
     * @param int $intFMRId : FMR Id
     *
     * @return mixed
     */
    public function fnGetUserListByFMRId($intFMRId = 0)
    {
        $strClientEmailQuery = "SELECT fdf.entity_id, fdf.field_this_is_only_a_test_target_id, T.userid, U.mail
                           FROM field_data_field_this_is_only_a_test AS fdf
                           JOIN eck_micro_list as EL  ON EL.id = fdf.field_this_is_only_a_test_target_id
                           JOIN field_data_field_micro_list T ON T.field_micro_list_target_id = fdf.field_this_is_only_a_test_target_id
                           JOIN users AS U ON U.uid = T.entity_id
                           WHERE fdf.entity_id =" . $intFMRId . "
                           AND U.status = 1
                           GROUP BY userid ";
        return db_query($strClientEmailQuery)->fetchAll();
    }

     /**
     * Function to return the company Name
     *
     * @param int $intOgId : Company OG Id
     */
    public function fnGetCompanyName($intOgId = 0)
    {
        $strSql = "SELECT label, etid, alias
                   FROM og AS og
                   LEFT JOIN url_alias AS ua ON ua.source = CONCAT( 'node/', og.etid )
                   WHERE gid =" . $intOgId;
        $objArrResult = db_query($strSql)->fetchAssoc();
        return $objArrResult;
    }

    /**
     * Get Company Url
     *
     * @param int $intNid : Node Id
     *
     * @return mixed
     */
    public function fnGetCompanyUrl($intNid = 0)
    {
        $strSql = "SELECT alias
                   FROM og AS og
                   LEFT JOIN url_alias AS ua ON ua.source = CONCAT( 'node/', og.etid )
                   WHERE og.etid =" . $intNid;

        $objArrResult = db_query($strSql)->fetchAssoc();
        return array($objArrResult);
    }

    /**
     * Getting all active company for admin form
     *
     * @return mixed
     */
    public function fnGetAllActiveClient()
    {
        $strClientSql = " Select og.gid  from node as n
                    JOIN og ON n.nid = og.etid
                    Where type ='client'
                    AND  status !=1
                    Group by n.nid";

        $objArrResult = db_query($strClientSql)->fetchAll();
        return $arrResult = json_decode(json_encode($objArrResult), true);
    }

    /**
     * Function to update the value of title for eck_listmail
     *
     * @param int    $intListId    : List Id
     * @param string $strListTitle : List Title
     *
     * @return DatabaseStatementInterface
     */
    public function fnUpdateEckListMailTitle($intListId, $strListTitle)
    {
        $strUpdateQuery = "UPDATE eck_micro_list SET title  = '$strListTitle'  WHERE id ='" . $intListId . "'";
        $intResult = db_query($strUpdateQuery);
        return $intResult;
    }

        /**
     * Function to check whether the duplicate the values exist.
     *
     * @param int $intListId : List Id
     * @param int $intUserId : User Id
     *
     * @return mixed
     */
    public function fnDuplicateSynMicroEmail($intListId, $intUserId)
    {
        //Get the list id and user id and check whether such combination get exists.
        $strDuplicateQuery = " Select field_micro_list_target_id From field_data_field_micro_list
                                Where field_micro_list_target_id = $intListId
                                AND  entity_id = $intUserId ";

        return db_query($strDuplicateQuery)->rowCount();
    }

    /**
     * Function to insert the list and entity value into the field value.
     *
     * @param int $intListId : List Id
     * @param int $intUserId : User Id
     *
     * @return mixed
     */
    public function fnAddMicroListFieldValue($intListId, $intUserId)
    {
        $intDeltaValue = $this->fnGetDeltaFromMicroList($intUserId);
        $strQuery = "INSERT INTO field_data_field_micro_list(entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_micro_list_target_id) VALUES ('user', 'user', 0, $intUserId, $intUserId, 'und', $intDeltaValue, $intListId)";
        $intResult = db_query($strQuery);
        return $intResult;
    }

    /**
     * Function to add list values in the respected revision table.
     *
     * @param int $intListId : List Id
     * @param int $intUserId : User Id
     *
     * @return mixed
     */
    public function fnAddMicroListRevisionValue($intListId, $intUserId)
    {
        $intDeltaValue = $this->fnGetDeltaFromMicroList($intUserId);
        $strQuery = "INSERT INTO field_revision_field_micro_list(entity_type, bundle, deleted, entity_id, revision_id , language, delta, field_micro_list_target_id) VALUES ('user', 'user', 0, $intUserId, $intUserId, 'und', $intDeltaValue, $intListId)";
        $intResult = db_query($strQuery);
        return $intResult;
    }

    /**
     * Function to return list Ids respect with fmr ids.
     *
     * @param int    $strFmrIds : Comma separated FMR Ids
     * @param string $strType   : Micro List Type
     *
     * @return mixed
     */
    public function fnGetAllListIds($strFmrIds = 0, $strType = '3bl_micro_list')
    {
        //Checking the table for client and admin list
        if ($strType == '3bl_micro_list') {
            $strSelect = " field_3bl_micro_list_target_id ";
            $strWhereMicroList = " field_data_field_3bl_micro_list as ml
                                  JOIN eck_micro_list eml
                                  ON ml.field_3bl_micro_list_target_id = eml.id ";
        } else {
            $strSelect = " field_client_micro_list_target_id ";
            $strWhereMicroList = " field_data_field_client_micro_list as ml
                                  JOIN eck_micro_list eml
                                  ON ml.field_client_micro_list_target_id = eml.id ";
        }


        $strGetListUsers = "SELECT " . $strSelect . " ,entity_id
                            FROM " . $strWhereMicroList . "
                            WHERE entity_id IN(" . $strFmrIds . ")
                            AND type = '" . $strType . "'";
        return db_query($strGetListUsers)->fetchAll();
    }

    /**
     * Function to get the list of email list.
     *
     * @param int $strListIds : List Id
     *
     * @return mixed
     */
    public function fnGetUserFromClientMicroList($strListIds = 0)
    {
        $strGetListUsers = "SELECT ml.entity_id AS userid, ml.field_micro_list_target_id AS listid, u.mail
                            FROM field_data_field_micro_list as ml
                            JOIN users AS u ON ml.entity_id = u.uid
                            LEFT JOIN field_data_field_unsub_all_3bl_comm as un ON un.entity_id = u.uid
                            LEFT JOIN field_data_field_unsub_fmr_updates as ufu ON u.uid = ufu.entity_id
                            WHERE ml.field_micro_list_target_id IN(" . $strListIds . ") AND un.field_unsub_all_3bl_comm_value <> 1
                            AND (field_unsub_fmr_updates_value <> 1 Or field_unsub_fmr_updates_value is NULL)";
        return db_query($strGetListUsers)->fetchAll();
    }

    /**
     * Function for Getting User from FMR
     *
     * @param int    $intFmrId : FMR Id
     * @param string $strType  : Micro List Type
     *
     * @return mixed
     */
    public function fnGetUserFromFmrId($intFmrId = 0, $strType = '')
    {
        if ($strType == '3bl_micro_list') {
            $strJoinTable = " JOIN  field_data_field_3bl_micro_list as 3bl ON ml.field_micro_list_target_id = 3bl.field_3bl_micro_list_target_id ";
            $strSelectListId = "3bl.field_3bl_micro_list_target_id as list_id";
        } else {
            $strJoinTable = " JOIN  field_data_field_client_micro_list as 3bl ON ml.field_micro_list_target_id = 3bl.field_client_micro_list_target_id ";
            $strSelectListId = "3bl.field_client_micro_list_target_id as list_id";
        }

        //SELECT DISTINCT in case same email is on multiple lists that are selected
        $strGetListUsers = "SELECT DISTINCT ml.entity_id AS userid,u.mail, " . $strSelectListId . "
                            FROM field_data_field_micro_list as ml
                            JOIN users AS u ON ml.entity_id = u.uid "
            . $strJoinTable .
            " LEFT JOIN field_data_field_unsub_all_3bl_comm as un ON un.entity_id = u.uid
                            LEFT JOIN field_data_field_unsub_fmr_updates as ufu ON u.uid = ufu.entity_id
                            WHERE 3bl.entity_id = $intFmrId
                            AND un.field_unsub_all_3bl_comm_value <> 1
                            AND (field_unsub_fmr_updates_value <> 1 Or field_unsub_fmr_updates_value is NULL) ";

        return db_query($strGetListUsers)->fetchAll();
    }

    /**
     * Function to get details of company by using fmr id
     *
     * @param int $intFmrId : FMR Id
     *
     * @return mixed
     */
    public function fnGetFmrCompanyDetails($intFmrId)
    {
        $strCompanyQuery = "SELECT og.etid, og.label
                                 FROM node as N
                                 JOIN field_data_group_audience as ga ON ga.entity_id = N.nid
                                 JOIN og ON ( og.gid = ga.group_audience_gid)
                                 WHERE N.nid =" . $intFmrId;
        return db_query($strCompanyQuery)->fetchAll();
    }

    /**
     * Function to get the mail address from user id.
     *
     * @param int $intUserId : User Id
     *
     * @return mixed
     */
    public function fnGetEmailAddressById($intUserId = 0)
    {
        $strQuery = "SELECT mail FROM users WHERE uid =" . $intUserId;
        return db_query($strQuery)->fetchAll();
    }

    /**
     * Delete List From list table.
     *
     * @param int $intListId : List Id
     *
     * @return DatabaseStatementInterface
     */
    public function fnDeleteListByClient($intListId)
    {
        $ObjResult = $this->fnGetAllUsersForList($intListId);
        if (count($ObjResult) > 0) {
            foreach ($ObjResult as $row) {
                $this->fnDeleteMicroListFieldItem($row->entity_id, $intListId);
                $this->fnDeleteMicroListRevItem($row->entity_id, $intListId);
            }
        }
        return $arrDelete = db_delete('eck_micro_list')->condition('id', $intListId)->execute();
    }

    /**
     * Function to get All users for the particular List
     *
     * @param int $intListId : List Id
     *
     * @return mixed
     */
    public function fnGetAllUsersForList($intListId)
    {
        $strQuery = " SELECT entity_id FROM field_data_field_micro_list WHERE field_micro_list_target_id = " . $intListId;
        $arrCount = db_query($strQuery)->fetchAll();
        return $arrCount;
    }

    /**
     * Function to get the deleted attribute value from the table against user id.
     *
     * @param int $intUserId : User Id
     *
     * @return int
     */
    public function fnGetDeltaFromMicroList($intUserId)
    {
        $strQuery = "SELECT delta FROM field_data_field_micro_list WHERE entity_id = " . $intUserId;
        $intCount = db_query($strQuery)->rowCount();
        return (int)$intCount;
    }

    /**
     * Function fnDeleteMicroListItem to delete the entry from the table.
     *
     * @param int $intUserId : User Id
     * @param int $intListId : List Id
     * @param int $intFlag   : Flag to check Delete entry from revision table or not
     *
     * @return mixed
     */
    public function fnGetDeltaForList($intUserId, $intListId, $intFlag = 0)
    {
        if ($intFlag == 0) {
            $strQuery = "SELECT delta FROM field_data_field_micro_list WHERE entity_id = " . $intUserId . " AND field_micro_list_target_id = " . $intListId;
        } else {
            $strQuery = "SELECT delta FROM field_revision_field_micro_list WHERE entity_id = " . $intUserId . " AND field_micro_list_target_id = " . $intListId;
        }
        $arrCount = db_query($strQuery)->fetchAll();

        return $arrCount[0]->delta;
    }

    /**
     * Function fnDeleteMicroListItem to delete the entry from the table.
     *
     * @param int $intUserId : User Id
     * @param int $intListId : List Id
     *
     * @return mixed
     */
    public function fnDeleteMicroListFieldItem($intUserId, $intListId)
    {
        $intDeltaValue = $this->fnGetDeltaForList($intUserId, $intListId);
        $strQuery = "DELETE FROM field_data_field_micro_list WHERE entity_id= " . $intUserId . " AND field_micro_list_target_id = " . $intListId;
        $intCount = db_query($strQuery);
        $intResult = $this->fnUpdateOtherDeltaValues($intUserId, $intDeltaValue);
        return $intResult;
    }

    /**
     * Function fnDeleteMicroListItem to delete the entry from the table.
     *
     * @param int $intUserId : User Id
     * @param int $intListId : List Id
     *
     * @return mixed
     */
    public function fnDeleteMicroListRevItem($intUserId, $intListId)
    {
        $intDeltaValue = $this->fnGetDeltaForList($intUserId, $intListId, 1);
        $strQuery = "DELETE FROM field_revision_field_micro_list WHERE entity_id= " . $intUserId . " AND field_micro_list_target_id = " . $intListId;
        $intCount = db_query($strQuery);
        $intResult = $this->fnUpdateOtherRevDeltaValues($intUserId, $intDeltaValue);
        return $intResult;
    }

    /**
     * fnUpdateOtherDeltaValues to update Delta values
     *
     * @param int $intUserId     : User Id
     * @param int $intDeltaValue : Delta Value
     *
     * @return bool
     */
    public function fnUpdateOtherDeltaValues($intUserId, $intDeltaValue)
    {
        $strQuery = " SELECT * FROM field_data_field_micro_list WHERE entity_id = " . $intUserId . " ORDER BY delta DESC LIMIT 0 , 1";
        $arrCount = db_query($strQuery)->fetchAll();

        if (count($arrCount) > 0 && $arrCount[0]->delta > $intDeltaValue) {
            $strUpdateQuery = "UPDATE field_data_field_micro_list SET delta  = " . $intDeltaValue . "  WHERE entity_id = " . $arrCount[0]->entity_id . " And field_micro_list_target_id = " . $arrCount[0]->field_micro_list_target_id;
            $intResult = db_query($strUpdateQuery);
        }
        return true;
    }

    /**
     * Function to update Delta value in Revision table
     *
     * @param int $intUserId     : User Id
     * @param int $intDeltaValue : Delta Value
     *
     * @return bool
     */
    public function fnUpdateOtherRevDeltaValues($intUserId, $intDeltaValue)
    {
        $strQuery = " SELECT * FROM field_revision_field_micro_list WHERE entity_id = " . $intUserId . " ORDER BY delta DESC LIMIT 0 , 1";
        $arrCount = db_query($strQuery)->fetchAll();

        if (count($arrCount) > 0 && $arrCount[0]->delta > $intDeltaValue) {
            $strUpdateQuery = "UPDATE field_revision_field_micro_list SET delta  = " . $intDeltaValue . "  WHERE entity_id = " . $arrCount[0]->entity_id . " And field_micro_list_target_id = " . $arrCount[0]->field_micro_list_target_id;
            $intResult = db_query($strUpdateQuery);
        }
        return true;
    }

    /**
     * Function to check whether user already exist for the particular list.
     *
     * @param int $intUserId : User Id
     * @param int $intListId : List Id
     *
     * @return int
     */
    public function fnCheckUserInList($intUserId, $intListId)
    {
        $strQuery = " SELECT field_micro_list_target_id FROM field_data_field_micro_list WHERE entity_id = " . $intUserId . " AND field_micro_list_target_id =" . $intListId;
        $intCount = db_query($strQuery)->rowCount();
        return $intCount;
    }

    /**
     * Insert value in fmr update tracking tables
     *
     * @param Array $arrFmrDetails : FMR Details
     */
    public function fnInsertFmrUpdateTracking($arrFmrDetails)
    {
        db_insert('threebl_microlist_tracking')->fields($arrFmrDetails)->execute();
    }


    /**
     * Function for checking duplicate value for FMR Tracking
     *
     * @param Array $arrFMRView : FMR View
     *
     * @return mixed
     */
    public function fnCheckFmrTrackValue($arrFMRView)
    {
        $intUserId = $arrFMRView['userid'];
        $intFmrId = $arrFMRView['fmr_nid'];
        $strActionDate = $arrFMRView['action_date'];
        $strFlag = $arrFMRView['tracking_flag'];
        $intListId = $arrFMRView['list_id'];

        $strQuery = " SELECT userid FROM threebl_microlist_tracking
                      WHERE userid = " . $intUserId . "
                      AND fmr_nid = " . $intFmrId . "
                      AND action_date = '" . $strActionDate . "'
                      AND tracking_flag = '" . $strFlag . "'
                      AND list_id =" . $intListId;

        return db_query($strQuery)->rowCount();
    }

    /**
     * Function to return the number of users in particular list.
     *
     * @param int $intListId : List Id
     *
     * @return mixed
     */
    public function fnGetCountOfUser($intListId)
    {
        $strQuery = " SELECT entity_id FROM field_data_field_micro_list WHERE field_micro_list_target_id =" . $intListId;
        return db_query($strQuery)->rowCount();
    }

    /**
     * Function for getting user id by user-email if it's duplicate entry
     *
     * @param string $strEmail : Email
     * @param int    $intListId : List Id
     *
     * @return int
     */
    public function fnGetUserIdByEmail($intListId, $strEmail = '')
    {
        $intUserId = 0;
        $strQuery = " SELECT U.uid, sml.field_micro_list_target_id
         FROM users AS U
         LEFT JOIN field_data_field_micro_list AS sml ON ( sml.entity_id = U.uid AND sml.field_micro_list_target_id = '" . $intListId . "')
         WHERE U.mail ='" . $strEmail . "'";
        $arrUserDetails = db_query($strQuery)->fetchAssoc();
        if (!empty($arrUserDetails)) {
            $intUserId = $arrUserDetails['uid'];
            $boolList = (int) $arrUserDetails['field_micro_list_target_id'];
        }
        return array($intUserId, $boolList);
    }

    /**
     * Function to get the fmr details which associate to micro list.
     *
     * @param string $strGetStartDate  : Start Date
     * @param string $strGetEndDate    : End Date
     * @param string $intStart         : Start count
     * @param string $intLimit         : Number of records per page
     * @param string $strSortCondition : Sort by condition
     * @param string $strDownloadExcel : Flag to Download Report
     * @param string $strListType : List Type
     *
     * @return mixed
     */
    public function fnGetFMRDetails($strGetStartDate, $strGetEndDate, $intStart = "", $intLimit = "", $strSortCondition = "", $strDownloadExcel, $strListType)
    {
        // 3BL Micro List conditions
        $strSelect = ", LD.field_3bl_micro_list_distributio_value as pub";
        $strJoin = " JOIN field_data_field_3bl_micro_list m ON n.nid = m.entity_id
                     JOIN field_data_field_3bl_micro_list_distributio as LD on LD.entity_id = n.nid ";
        $strWhereCondition = " AND LD.field_3bl_micro_list_distributio_value = 1";
        $strListJoin = "JOIN field_data_field_3bl_micro_list m ON e.id = m.field_3bl_micro_list_target_id";

        // Client Micro List conditions
        if ($strListType == "client") {
            $strSelect = "";
            $strJoin = " JOIN field_data_field_client_micro_list m ON n.nid = m.entity_id ";
            $strWhereCondition = "AND n.status = 1";

            $strListJoin = "JOIN field_data_field_client_micro_list m ON e.id = m.field_client_micro_list_target_id";
        }

        $strSql = "SELECT sql_calc_found_rows n.title as fmrTitle, n.status,
                   PD.field_fmr_date_time_value AS publishdate, og.label,
                   og.etid AS compId, n.nid AS fmrId
                   $strSelect
                   FROM node n
                   JOIN field_data_field_fmr_date_time AS PD ON PD.entity_id = n.nid
                   JOIN field_data_group_audience as ga on ga.entity_id = n.nid
                   JOIN og as og on og.gid = ga.group_audience_gid
                   $strJoin
                   WHERE DATE_FORMAT(PD.field_fmr_date_time_value,'%Y-%m-%d') BETWEEN '" . $strGetStartDate . "' AND '" . $strGetEndDate . "'
                   $strWhereCondition GROUP BY n.nid";

        $strLimit = "";
        $strOrderBy = "";
        // check condition for Order by and query Limit
        if ($strDownloadExcel == "excel") {
            $strOrderBy = " ORDER BY PD.field_fmr_date_time_value DESC";
        } else {
            $strOrderBy = " ORDER BY " . $strSortCondition;
            $strLimit = " LIMIT " . $intStart . " ," . $intLimit;
        }

        //Get Records
        $strSql = $strSql . $strOrderBy . $strLimit;
        $arrResult = db_query($strSql)->fetchAll();


        //Get count of all Records
        list($arrRecordsCount) = db_query('SELECT FOUND_ROWS()')->fetchAll();
        $arrRecordsCount = (array) ($arrRecordsCount);
        $intRecordsCount = $arrRecordsCount['FOUND_ROWS()'];

        
        // Get Multiple micro list assigned to particular FMR
        if ($intRecordsCount >0) {
            $arrFMRIds = $this->fnGetGroupFMRIds($arrResult);
            //check condition for FMR ids
            if (is_array($arrFMRIds) && count($arrFMRIds) >0) {
                $arrMicroList = $this->fnGetMicroListName($arrFMRIds, $strListJoin);
            }
        }
        return array($arrResult, $intRecordsCount, $arrMicroList);
    }

    public function fnGetGroupFMRIds($arrResult) {
        $arrFMRIds = array();
        //Group FMR ids in array
        foreach ($arrResult as $arrFMRInfo) {
            $intFMRId = $arrFMRInfo->fmrId;
            $arrFMRIds[] = $intFMRId;
        }
        return $arrFMRIds;
    }

    public function fnGetMicroListName($arrFMRIds, $strListJoin) {

        // Implode FMR Ids to get Micro list name
        $strFMRIds = implode(",", $arrFMRIds);

        $strMicroListSql = "SELECT e.title, m.entity_id
                            FROM eck_micro_list e
                            $strListJoin
                            WHERE m.entity_id IN ($strFMRIds)";

        $arrMicroListResult = db_query($strMicroListSql)->fetchAll();

        $arrMicroList = array();
        // Move FMR id wise Micro List Name in array
        foreach ($arrMicroListResult as $arrMicroListData) {
            $arrMicroList[$arrMicroListData->entity_id][] = stripslashes($arrMicroListData->title);
        }

        return $arrMicroList;
    }

    /**
     * Function to track Micro Lists Processing
     *
     * @param int    $intFMRId    : FMR Id
     * @param int    $intListId   : List Id
     * @param string $strListType : List Type
     *
     * @return mixed
     */
    public function fnTrackMicroListProcessing($intFMRId, $intListId)
    {
        //set the current date
        $dateCurrentDate = date("Y-m-d");
        $arrInsert = array(
            'fmr_id'    => $intFMRId,
            'list_id'   => $intListId,
            'processing_on'   => $dateCurrentDate,
        );

        //Insert all details in the table.
        $intInsertId = db_insert('threebl_microlist_process_tracking')
            ->fields($arrInsert)
            ->execute();
    }

    /**
     * Function to get Micro List Total opens and clicks count of FMR
     *
     * @param $strTrackingType Tracking type view or click
     *
     * @return object
     */
    public function fnGetMicroListTotalOpenClickCnt($strTrackingType)
    {

        if (trim($strTrackingType) != "") {
            # Micro List FMR Tracking type view or click
            $strTrackingType = ($strTrackingType == "view") ? "open" : "click";

            $strQuery = db_select('threebl_microlist_tracking', 'tmt');
            $strQuery->join('node', 'N', 'N.nid = tmt.fmr_nid');
            $strQuery->join('url_alias', 'U', "U.source = CONCAT( 'node/', N.nid )");
            $strQuery->join('field_data_group_audience', 'ga', 'ga.entity_id = N.nid');
            $strQuery->join('og', 'og', 'og.gid = ga.group_audience_gid');
            $strQuery->addExpression('COUNT(tmt.tracking_flag)', 'totalCnt');
            $strQuery->fields('tmt', array('fmr_nid'))
                ->fields('N', array('title'))
                ->fields('U', array('alias'))
                ->fields('og', array('label'))
                ->condition('tmt.tracking_flag', $strTrackingType, '=')
                ->GroupBy('tmt.fmr_nid');

            $arrResult = $strQuery->extend('PagerDefault')->limit(4)->execute();
            $objFMRViewClickCnt = $arrResult->fetchAll();
            //$objFMRViewClickCnt = db_query($strTrackingQuery)->fetchAll();
            return $objFMRViewClickCnt;
        }
    }
    
    /**
     * Insert the value in list.s
     *
     * @param Object $objEntity : Entity object
     *
     * @return mixed
     */
    public function fnInsertList(&$objEntity)
    {
        entity_get_controller('micro_list')->save($objEntity);
        return $objEntity->id;
    }

    public function fnGetNewsUserRecord($arrNewsLetterUser) {
        //save the new user record
        $arrUser = user_save(null, $arrNewsLetterUser);
        return $arrUser;
    }
}

?>