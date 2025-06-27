<?php

interface Platform
{
    /** @param array<Platform> &$platforms */
    static public function get(array &$platforms) : void;
    static public function getDefault() : Platform;
    static public function setDefault(Platform $p) : Platform;
    public function __construct(/*const*/cl_platform_id &$platform, bool $retainObject=false);
    public function getInfo(PlatformParamName $info, mixed $param=null) : mixed; // enum PlatformParamName
    /** @param array<Device> &$devices */
    public function getDevices(cl_device_type $type, array &$devices) : void;
}