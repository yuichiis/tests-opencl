__kernel void fill(__global float * y,
                    const float a)
{
    size_t gid;
    gid = get_global_id(0);
    y[gid] = a;
}
