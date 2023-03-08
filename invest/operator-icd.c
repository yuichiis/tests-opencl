#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (8)
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
    cl_int a=3;
    cl_int b=5;

    cl_int values[MEM_SIZE];

    FILE *fp;
    char fileName[] = "./operator.cl";
    char *source_str;
    size_t source_size;

    /* Load the source code containing the kernel*/
    fp = fopen(fileName, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    /* Get Platform and Device Info */
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    /* Create OpenCL context */
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    /* Create Command Queue */
    //command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    /* Create Memory Buffer */
    memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(values), NULL, &ret);

    /* Create Kernel Program from the source */
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
                                    (const size_t *)&source_size, &ret);

    /* Build Kernel Program */
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if(ret!=0) {
        printf("clBuildProgram=%d\n",ret);
        switch(ret) {
            case CL_BUILD_PROGRAM_FAILURE: {
                size_t log_sz;
                printf("CL_BUILD_PROGRAM_FAILURE\n");
                ret = clGetProgramBuildInfo(program,device_id,CL_PROGRAM_BUILD_LOG,0,NULL,&log_sz);
                if(ret!=0) {
                    printf("clGetProgramBuildInfo(LOG) retsize=%d\n",ret);
                    exit(1);
                }
                printf("logsize=%ld\n",log_sz);
                char *log = malloc(log_sz);

                ret = clGetProgramBuildInfo(program,device_id,CL_PROGRAM_BUILD_LOG,log_sz,log,&log_sz);
                if(ret!=0) {
                    printf("clGetProgramBuildInfo(LOG)=%d\n",ret);
                    exit(1);
                }
                printf("%s",log);
                break;
            }
            case CL_INVALID_BUILD_OPTIONS: {
                printf("CL_INVALID_BUILD_OPTIONS\n");
                break;
            }
        }
        exit(1);
    }

    /* Create OpenCL Kernel */
    kernel = clCreateKernel(program, "operator", &ret);
    if(ret!=0) {
        printf("clCreateKernel=%d\n",ret);
        exit(1);
    }

    /* Set OpenCL Kernel Parameters */
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
    if(ret!=0) {
        printf("clSetKernelArg0=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernel, 1, sizeof(a), (void *)&a);
    if(ret!=0) {
        printf("clSetKernelArg1=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernel, 2, sizeof(b), (void *)&b);
    if(ret!=0) {
        printf("clSetKernelArg2=%d\n",ret);
        exit(1);
    }

    /* Execute OpenCL Kernel */
    size_t globalWorkSize[] = {1};
    //size_t localWorkSize[] = {1};
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    if(ret!=0) {
        printf("clEnqueueNDRangeKernel=%d\n",ret);
        exit(1);
    }

    ret = clFinish(command_queue);
    if(ret!=0) {
        printf("clFinish=%d\n",ret);
        exit(1);
    }

    /* Copy results from the memory buffer */
    ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
                            sizeof(values),values, 0, NULL, NULL);
    if(ret!=0) {
        printf("clEnqueueReadBuffer=%d\n",ret);
        exit(1);
    }

    printf("\n");
    /* Display Result */
    for(int i=0;i<MEM_SIZE;i++) {
        printf("%d,",values[i]);
    }
    printf("\n");

    /* Finalization */
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(memobj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(source_str);

    return 0;
}