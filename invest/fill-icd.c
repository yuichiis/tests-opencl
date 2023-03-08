#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define BUF_SIZE (16)
#define MAX_SOURCE_SIZE (0x100000)

int main()
{
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_mem memobj = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;
    cl_int memsize;
    size_t param_value_size_ret;

    cl_uint host_buf[BUF_SIZE];
    cl_uint pattern = 123;

    cl_event event_ret;

    /* Get Platform and Device Info */
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);


    ret = clGetDeviceInfo(device_id,
            (cl_device_info)CL_DEVICE_VERSION,
            0, NULL, &param_value_size_ret);
    char *param_value = malloc(param_value_size_ret);
    ret = clGetDeviceInfo(device_id,
            (cl_device_info)CL_DEVICE_VERSION,
            param_value_size_ret, param_value, NULL);
    printf("device version=%s\n",param_value);
    free(param_value);

    /* Create OpenCL context */
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);   

    /* Create Command Queue */    
    //command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    /* Create Memory Buffer */
    printf("createbuf\n");
    //memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(host_buf), NULL, &ret);
    //memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(host_buf)*2, NULL, &ret);
    memobj = clCreateBuffer(context, CL_MEM_READ_ONLY,  sizeof(host_buf), NULL, &ret);
    printf("ret=%d\n",ret);
    if(ret!=CL_SUCCESS) {
        exit(1);
    }
    //memsize = sizeof(host_buf);
    memsize = 16;

    printf("pattern=%d\n",pattern);
    printf("memsize=%d\n",memsize);
    printf("sizeof(pattern)=%ld\n",sizeof(pattern));
    printf("fill\n");

    ret = clEnqueueFillBuffer(
        command_queue,
        memobj,
        &pattern,
        sizeof(pattern),
        (size_t)0,
        (size_t)memsize,
        0,
        NULL,
        &event_ret);
    printf("ret=%d\n",ret);
    if(ret!=CL_SUCCESS) {
        exit(1);
    }

    printf("finish\n");
    ret = clFinish(command_queue);

    printf("read\n");
    /* Copy results from the memory buffer */
    ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
                            sizeof(host_buf),host_buf, 0, NULL, NULL);

    printf("finish\n");
    ret = clFinish(command_queue);

    /* Display Result */
    for(int i=0;i<BUF_SIZE;i++) {
        printf("%d ",host_buf[i]);
    }
    printf("\n");

    /* Finalization */
    //ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(memobj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    return 0;
}