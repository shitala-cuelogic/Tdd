<?php 

/**
 * affiliatesController extends DrupalDefaultEntityController.
 * Our subclass of DrupalDefaultEntityController lets us add a few
 * important create, update, and delete methods.
 */
class clsAffiliatesController extends DrupalDefaultEntityController implements affiliatesControllerInterface
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


?>