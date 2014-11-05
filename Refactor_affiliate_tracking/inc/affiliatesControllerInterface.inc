<?php

interface affiliatesControllerInterface extends DrupalEntityControllerInterface
{
    public function create();
    public function save($entity);
    public function delete($entity);
}

?>