<?php

interface UserEvent extends Event
{
    public function __construct(Context  $context);
    public function setStatus(int $status) : void;
}