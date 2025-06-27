<?php

interface Program
{
    /** @var string|array<string>|IL|Binaly $source */
    public function __construct(
        Context $context,
        string|array|IL|Binaly $source,
        bool $build = false,
        Device $device=null,
    );
    public function build(
        Device $devices,
        string $options = nullptr,
        callback $notifyFptr,
        mixed $data = null
    ) : int;
#if CL_HPP_TARGET_OPENCL_VERSION >= 120
    public function compile(
        string $options = null,
        callback $notifyFptr=null,
        mixed $data=null,
    ) : int;
#endif
    public function getInfo(cl_program_info $name, mixed $param=null) : mixed;
    public function getBuildInfo(Device $device, cl_program_build_info $name, mixed $param=null) : mixed;
    public function createKernels(array /*<Kernel>*/ &$kernels) : void;
#if CL_HPP_TARGET_OPENCL_VERSION >= 220
#if defined(CL_USE_DEPRECATED_OPENCL_2_2_APIS)
    public function setReleaseCallback(callback $pfn_notify,mixed $user_data = null) : mixed;
#endif
    public function setSpecializationConstant(int $index, int $size, mixed $value) : void;
#endif
#if CL_HPP_TARGET_OPENCL_VERSION >= 120
    public function link(Program $program, mixed $options=null, mixed $data=null) : Program;
#endif
}
