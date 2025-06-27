<?php

interface Context
{
    static public function getDefault() : Context;
    static public function setDefault(Context $p) : Context;
    public function __construct(
        /*const*/ array/*<Device>*/|DeviceType $devices, // create from devices or device type
        /*const*/ cl_context_properties $properties,
        callable $notifyFptr=null,
        mixed $data = null,
    );
    public function getInfo(ContextParamName $info, mixed $param=null) : mixed; // enum ContextParamName
    public function getSupportedImageFormats(
        cl_mem_flags $flags,
        cl_mem_object_type $type,
        array/*<ImageFormat>*/ $formats,
    ) : int;

    #if CL_HPP_TARGET_OPENCL_VERSION >= 300
    public function setDestructorCallback(
        callback $pfn_notify,
        mixed $user_data = null,
    );
    #endif
}
