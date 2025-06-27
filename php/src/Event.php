<?php

interface Event
{
    public function __construct(/*const*/ cl_event &$event,bool $retainObject = false);
    public function getInfo(EventParamName $info, mixed $param=null) : mixed; // enum EventParamName
    public function getProfilingInfo(cl_profiling_info $name, mixed $param) : int; 
    public function wait() : void;
    static public function waitForEvents(array /*Event*/ $events) : int;
    #if CL_HPP_TARGET_OPENCL_VERSION >= 110
    public function setCallback(int $type, callback $pfn_notify, mixed $user_data=null) : int;
    #endif
}
