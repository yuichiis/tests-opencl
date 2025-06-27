<?php

interface Device
{
    static public function createSubDevices(
        /*const*/ cl_device_partition_property &$properties,
        Device &$devices,
    ) : void;
    
}