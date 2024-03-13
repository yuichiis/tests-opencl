#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <stdio.h>
#include <clblast_c.h>

#define NWITEMS 3

int main(int argc, char ** argv)
{
    cl_float2 alpha;
    alpha.s[0] = 2;
    alpha.s[1] = 0;
    cl_float2 hostX[NWITEMS];
    for(int i = 0; i < NWITEMS; i++)
    {
        hostX[i].s[0] = (cl_float)i;             // 0           : 255
        hostX[i].s[1] = 0;             // 0           : 255
    }

    // 1. Get a platform.
    cl_platform_id platform;
    cl_uint num_platforms=0;
    clGetPlatformIDs( 1,          // cl_uint num_entries
                      &platform,  // cl_platform_id * platforms
                      &num_platforms );     // cl_uint * num_platforms
    printf("%d platforms are found.\n",num_platforms);
    printf("platforms_id is %llx.\n",(long long)platform);

    // 2. Find a gpu device.
    cl_device_id device;
    cl_uint num_devices=0;
    clGetDeviceIDs( platform,           // cl_platform_id platform
                    CL_DEVICE_TYPE_GPU, // cl_device_type device_type
                    1,                  // cl_uint num_entries
                    &device,            // cl_device_id *devices
                    &num_devices);      // cl_uint *num_devices
    printf("%d devices are found.\n",num_devices);
    if(num_devices==0) {
        return 0;
    }
    printf("device_id is %llx.\n",(long long)device);

    // 3. Create a context and command queue on that device.
    cl_context context = clCreateContext(
        NULL,    // const cl_context_properties * properties,
        1,       // cl_uint num_devices,
        &device, // const cl_device_id * devices,
        NULL,    // CL_CALLBACK * pfn_notify
        NULL,    // void * user_data,
        NULL);   // cl_int * errcode_ret

    cl_command_queue queue = clCreateCommandQueue(
        context, // cl_context context,
        device,  // cl_device_id device,
        0,       // cl_command_queue_properties properties,
        NULL );  // cl_int * errcode_ret

    // 5. Create a data buffer.
    //cl_mem buffer = clCreateBuffer( context,
    //                                CL_MEM_WRITE_ONLY,
    //                                NWITEMS * sizeof(cl_uint),
    //                                NULL, NULL );
    cl_float2 *pX = hostX;
    cl_mem bufX = clCreateBuffer( context,
                                    CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,
                                    NWITEMS * sizeof(cl_float2),
                                    (void*)pX, NULL );

    cl_event event = NULL;
    size_t offX = 0;
    size_t incX = 1;
    CLBlastStatusCode status;

    status = CLBlastCscal(
        NWITEMS,
        alpha,
        bufX, offX, incX,
        &queue, &event);
    if(status!=CLBlastSuccess) {
        printf("Error: CLBlastCscal code=%d\n",status);
        clReleaseMemObject( bufX );
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        return 0;
    }

    // clFinish( queue );
    clWaitForEvents(1, &event);
    clReleaseEvent(event);

    int i;

    //for(i=0; i < NWITEMS; i++) {
    //    printf("%d:%6.2f, ", i, hostY[i]);
    //}
    //printf("\n");


    // 7. Look at the results via synchronous buffer map.
    //cl_uint *ptr;
    //ptr = (cl_uint *) clEnqueueMapBuffer( queue,
    //                                      buffer,
    //                                      CL_TRUE,
    //                                      CL_MAP_READ,
    //                                      0,
    //                                      NWITEMS * sizeof(cl_uint),
    //                                      0, NULL, NULL, NULL );


    clEnqueueReadBuffer(queue,
                        bufX,                       // c_mem buffer
                        CL_TRUE,                    // blocking_read,
                        0,                          // offset,
                        NWITEMS * sizeof(cl_float2), // size,
                        (void *)hostX,              // ptr,
                        0,                          // num_events_in_wait_list,
                        NULL,                       // event_wait_list,
                        NULL                        //event
                    );


    for(i=0; i < NWITEMS; i++) {
        printf("%d:(%6.2f,%6.2f), ", i, hostX[i].s[0],hostX[i].s[1]);
    }
    printf("\n");

    clReleaseMemObject( bufX );
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    return 0;
}
