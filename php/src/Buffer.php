<?php

interface Buffer extends Memory
{
    static public function new(
        HostBuffer $from,
        int $begin=null,
        int $end=null,
        bool $readOnly=false,
    );
    static public function copy(
        HostBuffer|Buffer $from,
        int $begin,
        int $end,
        HostBuffer|Buffer $to,
    ) : void;
    public function __construct(
        Context $context,
        cl_mem_flags $flags,
        int $size,
        ?HostBuffer $host_ptr = null,
#if CL_HPP_TARGET_OPENCL_VERSION >= 300
        array/*<cl_mem_properties>*/ $properties=null,
#endif
    );
#if CL_HPP_TARGET_OPENCL_VERSION >= 110
    public function createSubBuffer(
        cl_mem_flags $flags,
        cl_buffer_create_type $buffer_create_type,
        mixed $buffer_create_info,
    ) : Buffer;
#endif
}
