#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable

__kernel void sum_middle_0(
    const        uint total_local_items,
    const        uint segments,
        __global char * r,  // ${type}
    const        uint offset_r,
        __global char * x, // ${type}
    const        uint offset_x,
    const        uint incx,
         __local char * local_work, // ${type}
         __local char * seg_work, // ${type}
    const        uint work_items)
{
    {
        const uint lid = get_local_id(0);
        const uint lws = get_local_size(0);
        uint seg_count = segments;
        uint local_items = work_items;
        uint left_local_items = total_local_items;
        int is_first=1;
        while(1) {
            for(int seg=0;seg<seg_count;seg++) {
                if(lid<local_items) {
                    if(is_first) {
                        ////// ${inputs} /////
                        local_work[lid] = x[(seg*lws+lid)+offset_x]; /// ${index_x}
                    } else {
                        local_work[lid] = seg_work[seg*lws+lid];
                    }
                } else {
                    ///// ${initial}; /////
                    local_work[lid] = 0; 
                }
                barrier(CLK_LOCAL_MEM_FENCE);
                int i = lws;
                while( i>1 ) {
                    ////// ${operation} //////
                    i >>= 1;
                    if(lid < i) {
                        local_work[lid] += local_work[lid + i];
                    }
                    barrier(CLK_LOCAL_MEM_FENCE);
                }
                if(lid == 0) {
                    seg_work[seg] = local_work[0];
                }
                barrier(CLK_LOCAL_MEM_FENCE);
                left_local_items -= local_items;
                if(left_local_items<local_items) {
                    local_items = left_local_items;
                }
            }
            if(seg_count<=1) {
                break;
            }
            is_first = 0;
            left_local_items = seg_count;
            if(left_local_items<lws) {
                local_items = left_local_items;
            } else {
                local_items = lws;
            }
            seg_count = (seg_count+lws-1)/lws;
        }
        if(lid < work_items) {
            ////// ${outputs} //////
            r[offset_r+lid] = local_work[lid];
        }
        if(lid == 0) {
            ////// ${outputs} //////
            r[offset_r] = seg_work[0];
        }
    }
}