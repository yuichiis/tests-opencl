<?php

interface Kernel
{
    public function __construct(Program $program, string $name);
    public function getInfo(cl_kernel_info $name, mixed $param=null) : mixed;
#if CL_HPP_TARGET_OPENCL_VERSION >= 120
    public function getArgInfo(int $argIndex, cl_kernel_arg_info $name, mixed $param=null) : mixed;
#endif
    public function getWorkGroupInfo(Device $device, cl_kernel_work_group_info $name, mixed $param=null) : mixed;
#if defined(CL_HPP_USE_CL_SUB_GROUPS_KHR) || CL_HPP_TARGET_OPENCL_VERSION >= 210
    public function getSubGroupInfo(Device $dev, cl_kernel_sub_group_info $name, NDRange $range, size_type &$param) : int;
#endif
    public function setArg(cl_uint $index, mixed $value, int $size=null) : void;
#if CL_HPP_TARGET_OPENCL_VERSION >= 200
    // svm related functions
#endif
#if CL_HPP_TARGET_OPENCL_VERSION >= 200
    public function setExecInfo(cl_kernel_exec_info $param_name, mixed $val) : void;
#endif
#if CL_HPP_TARGET_OPENCL_VERSION >= 210
    public function clone() : Kernel;
#endif

}
