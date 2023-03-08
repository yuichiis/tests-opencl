__kernel void group(
    __global int *y,
    const    int  a
   )
{
    const int lid0 = get_local_id(0);
    const int grid0 = get_group_id(0);
    const int lid1 = get_local_id(1);
    const int grid1 = get_group_id(1);

    const int gid0 = get_global_id(0);
    const int gid1 = get_global_id(1);
    const int gsz = get_global_size(0);

    y[gid1*gsz+gid0] = grid1*1000+lid1*100+grid0*10+lid0;
}
