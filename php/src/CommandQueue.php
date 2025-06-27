<?php

interface CommandQueue
{
    public function __construct(
        Context $context,
        Device $device=null,
        QueueProperties $properties=null,
    );
    public function getInfo(cl_command_queue_info $name, mixed $param=null) : mixed;
    public function enqueueReadBuffer(
        Buffer $buffer,
        bool $blocking,
        int $offset,
        int $size,
        HostBuffer $ptr,
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;
    public function enqueueWriteBuffer(
        Buffer $buffer,
        bool $blocking,
        int $offset,
        int $size,
        HostBuffer $ptr,
        array /*<Event>*/ $events = null,
        Event $event = null
    ) : void;
    public function enqueueCopyBuffer(
        Buffer $src,
        Buffer $dst,
        int $src_offset,
        int $dst_offset,
        int $size,
        array /*<Event>*/ $events = null,
        Event $event = null
    ) : void;
#if CL_HPP_TARGET_OPENCL_VERSION >= 110
    public function enqueueReadBufferRect(
        Buffer $buffer,
        bool $blocking,
        array /*<int>*/ $buffer_offset,
        array /*<int>*/ $host_offset,
        array /*<int>*/ $region,
        int $buffer_row_pitch,
        int $buffer_slice_pitch,
        int $host_row_pitch,
        int $host_slice_pitch,
        HostBuffer $ptr,
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;
    public function enqueueWriteBufferRect(
        Buffer $buffer,
        bool $blocking,
        array /*<int>*/ $buffer_offset,
        array /*<int>*/ $host_offset,
        array /*<int>*/ $region,
        int $buffer_row_pitch,
        int $buffer_slice_pitch,
        int $host_row_pitch,
        int $host_slice_pitch,
        HostBuffer $ptr,
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;
    public function enqueueCopyBufferRect(
        Buffer $src,
        Buffer $dst,
        int $src_offset,
        int $dst_offset,
        array /*<int>*/ $src_origin,
        array /*<int>*/ $dst_origin,
        array /*<int>*/ $region,
        int $src_row_pitch,
        int $src_slice_pitch,
        int $dst_row_pitch,
        int $dst_slice_pitch,
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;
#endif
#if CL_HPP_TARGET_OPENCL_VERSION >= 120
    public function enqueueFillBuffer(
        Buffer $buffer,
        PatternType $pattern,
        int $offset,
        int $size,
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;
#endif
    public function enqueueMapBuffer(
        Buffer $buffer,
        bool $blocking,
        cl_map_flags $flags,
        int $offset,
        int $size,
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;
    public function enqueueUnmapMemObject(
        Memory $memory,
        HostBuffer $mapped_ptr,
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;

    // image read,write,copy,fill,CopyImageToBuffer,CopyBufferToImage,MapImage
#if CL_HPP_TARGET_OPENCL_VERSION >= 200
    // svm Memcpy,MemFill,Map,Unmap
#endif
#if CL_HPP_TARGET_OPENCL_VERSION >= 120
    public function enqueueMarkerWithWaitList(
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;
    public function enqueueBarrierWithWaitList(
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;
    public function enqueueMigrateMemObjects(
        array /*<Memory>*/ $memObjects,
        cl_mem_migration_flags $flags,
        array /*<Event>*/ $events = null,
        Event $event = null,
    ) : void;
#endif
#if CL_HPP_TARGET_OPENCL_VERSION >= 210
    // svm migrate
#endif
cl_int enqueueNDRangeKernel(
    const Kernel& kernel,
    const NDRange& offset,
    const NDRange& global,
    const NDRange& local = NullRange,
    const vector<Event>* events = nullptr,
    Event* event = nullptr) const

a
cl_int enqueueNativeKernel(
    void (CL_CALLBACK *userFptr)(void *),
    std::pair<void*, size_type> args,
    const vector<Memory>* mem_objects = nullptr,
    const vector<const void*>* mem_locs = nullptr,
    const vector<Event>* events = nullptr,
    Event* event = nullptr) const
a
cl_int flush() const
{
    return detail::errHandler(::clFlush(object_), __FLUSH_ERR);
}

cl_int finish() const
{
    return detail::errHandler(::clFinish(object_), __FINISH_ERR);
}
}