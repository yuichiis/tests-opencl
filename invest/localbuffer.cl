__kernel void localbuffer(
    const int n,
    const uchar a,
    __global uchar * y,
    __local uchar * local_work)
{
    int lid = get_local_id(0);
    int lws = get_local_size(0);
    if(lid<n) {
        local_work[lid] = a;
    } else {
        local_work[lid] = 0;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    int i = lws;
    while(i>1) {
        i >>= 1;
        if(lid < i) {
            local_work[lid] += local_work[lid + i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if(lid<n) {
        y[lid] = local_work[lid];
    }
}