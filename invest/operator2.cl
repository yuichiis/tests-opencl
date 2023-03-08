__kernel void operator(
    __global int *y,
    const    int  a,
    __global const int *b
    )
{
    const int gid = get_global_id(0);
    y[gid] = a / b[gid];
}
