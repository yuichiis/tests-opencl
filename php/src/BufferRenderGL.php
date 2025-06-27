<?php

interface BufferRenderGL extends Buffer
{
    public function __construct(
        Context $context,
        cl_mem_flags $flags,
        cl_GLuint $bufobj,
    );
    public function getObjectInfo(
        cl_gl_object_type &$type,
        cl_GLuint &$gl_object_name
    ) : int;
}