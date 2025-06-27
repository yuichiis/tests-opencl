<?php

interface Memory
{
    public function getInfo(MemParamName $name, mixed $param=null) : mixed; // enum MemParamName
    #if CL_HPP_TARGET_OPENCL_VERSION >= 110
    public function setDestructorCallback(
        callback $pfn_notify,
        mixed $user_data = null,
    );
    #endif

}