<?php

/**
 * Interface for creating list-emails entity.
 */
interface micro_listControllerInterface extends DrupalEntityControllerInterface
{
    public function create();
    public function save($entity);
    public function delete($entity);
}//interface


?>