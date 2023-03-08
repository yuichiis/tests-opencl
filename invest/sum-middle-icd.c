#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (30)
///#define MAX_SOURCE_SIZE (0x100000)
#define NUM_PROGRAM 1

int main()
{
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_mem memobj = NULL;
    cl_mem resultobj = NULL;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int n=MEM_SIZE;
    cl_int segments=1;
    cl_int offsetx=0;
    cl_int incx=1;
    cl_int offsetr=0;
    cl_int work_items=MEM_SIZE;
    cl_int ret;
    char *fileNames[NUM_PROGRAM] = {"./sum-middle-0.cl"};
    char *source_str[NUM_PROGRAM];
    size_t source_size[NUM_PROGRAM];
    cl_program programs[NUM_PROGRAM];
    cl_kernel kernels[NUM_PROGRAM];

    char pattern=1;
    char values[MEM_SIZE];
    char result[MEM_SIZE];
    //int pattern=1;
    //int values[MEM_SIZE];
    //int result[1];

    size_t maxWorkItems[4];
    size_t ret_sz;
    size_t local_mem_items;
    int pg_idx;

    for(pg_idx=0; pg_idx<NUM_PROGRAM; pg_idx++) {
        char *fileName;
        FILE *fp;
        size_t filesz,sz;
        char block[8096];
        /* Load the source code containing the kernel*/
        printf("Open file.%s\n",fileNames[pg_idx]);

        fileName=fileNames[pg_idx];
        fp = fopen(fileName,"r");
        if (!fp) {
            fprintf(stderr, "Failed to load kernel.\n");
            exit(1);
        }
        filesz = 0;
        while((sz=fread(block, 1, sizeof(block), fp))>0) {
            filesz+=sz;
        }
        if(fseek(fp,0,SEEK_SET)<0) {
            fprintf(stderr, "Failed to seek.\n");
            exit(1);
        }
        source_size[pg_idx] = filesz;
        source_str[pg_idx] = (char*)malloc(source_size[pg_idx]+1);
        size_t readsize = fread(source_str[pg_idx], 1, source_size[pg_idx], fp);
        printf("file size=%ld\n", source_size[pg_idx]);
        fclose(fp);
        printf("Close file.%s\n",fileName);
    }

    printf("get platform.\n");
    /* Get Platform and Device Info */
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    printf("get deviceid.\n");
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    printf("get info.\n");
    ret = clGetDeviceInfo(device_id,CL_DEVICE_MAX_WORK_ITEM_SIZES,0,NULL,&ret_sz);
    if(ret!=0) {
        printf("clGetDeviceInfo retsize=%d\n",ret);
        exit(1);
    }
    printf("ret_sz=%ld\n",ret_sz);
    printf("info size=%ld\n",sizeof(maxWorkItems));
    ret = clGetDeviceInfo(device_id,CL_DEVICE_MAX_WORK_ITEM_SIZES,sizeof(maxWorkItems),maxWorkItems,&ret_sz);
    if(ret!=0) {
        printf("clGetDeviceInfo=%d\n",ret);
        exit(1);
    }
    for(int i=0;i<3;i++) {
        printf("maxWorkItems[%d]=%ld\n",i,maxWorkItems[i]);
    }


    /* Create OpenCL context */
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    /* Create Command Queue */
    //command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    for(int i=0;i<MEM_SIZE;i++) {
        values[i] = pattern;
    }
    /* Create Memory Buffer */
    memobj = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,
            sizeof(values), (void*)values, &ret);

    /* Create Memory Buffer */
    resultobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(result), NULL, &ret);
 
    for(pg_idx=0;pg_idx<NUM_PROGRAM;pg_idx++) {
        cl_program program;
        printf("build program.%s\n",fileNames[pg_idx]);
        /* Create Kernel Program from the source */
        programs[pg_idx] = program = clCreateProgramWithSource(context, 1, (const char **)&source_str[pg_idx],
                                        (const size_t *)&source_size[pg_idx], &ret);

        /* Build Kernel Program */
        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        if(ret!=0) {
            printf("clBuildProgram(%s)=%d\n",fileNames[pg_idx],ret);
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
    }


    /* Create OpenCL Kernel */
    kernels[0] = clCreateKernel(programs[0], "sum_middle_0", &ret);
    if(ret!=0) {
        printf("clCreateKernel=%d\n",ret);
        exit(1);
    }

    /* Set OpenCL Kernel Parameters */
    ret = clSetKernelArg(kernels[0], 0, sizeof(n), (void *)&n);
    if(ret!=0) {
        printf("clSetKernelArg0=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernels[0], 1, sizeof(segments), (void *)&segments);
    if(ret!=0) {
        printf("clSetKernelArg1=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernels[0], 2, sizeof(cl_mem), (void *)&resultobj);
    if(ret!=0) {
        printf("clSetKernelArg2=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernels[0], 3, sizeof(offsetr), (void *)&offsetr);
    if(ret!=0) {
        printf("clSetKernelArg3=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernels[0], 4, sizeof(cl_mem), (void *)&memobj);
    if(ret!=0) {
        printf("clSetKernelArg4=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernels[0], 5, sizeof(offsetx), (void *)&offsetx);
    if(ret!=0) {
        printf("clSetKernelArg5=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernels[0], 6, sizeof(incx), (void *)&incx);
    if(ret!=0) {
        printf("clSetKernelArg6=%d\n",ret);
        exit(1);
    }
    for(local_mem_items=1; local_mem_items<n;local_mem_items<<=1) {
        if(local_mem_items>=maxWorkItems[0]) {
            printf("local mem is over size\n");
            exit(1);
        }
    }
    size_t localMemSize = local_mem_items*sizeof(values[0]);
    size_t segMemSize = segments*sizeof(values[0]);
    segMemSize += (segMemSize%4) ? 4-(segMemSize%4) : 0;
    printf("local_item_size=%ld\n",local_mem_items);
    printf("localMemSize=%ld\n",localMemSize);
    printf("segMemSize=%ld\n",segMemSize);

    ret = clSetKernelArg(kernels[0], 7, localMemSize, NULL);
    if(ret!=0) {
        printf("clSetKernelArg7=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernels[0], 8, segMemSize, NULL);
    if(ret!=0) {
        printf("clSetKernelArg8=%d\n",ret);
        exit(1);
    }
    ret = clSetKernelArg(kernels[0], 9, sizeof(work_items), (void *)&work_items);
    if(ret!=0) {
        printf("clSetKernelArg9=%d\n",ret);
        exit(1);
    }

    /* Execute OpenCL Kernel */
    size_t globalWorkSize[] = {local_mem_items};
    size_t localWorkSize[] =  {local_mem_items};
    ret = clEnqueueNDRangeKernel(command_queue, kernels[0], 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
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
    ret = clEnqueueReadBuffer(command_queue, resultobj, CL_TRUE, 0,
                            sizeof(result),result, 0, NULL, NULL);
    if(ret!=0) {
        printf("clEnqueueReadBuffer=%d\n",ret);
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
    printf("n=%d\n",n);
    /* Display Result */
    for(int i=0;i<MEM_SIZE;i++) {
        //printf("%3.1f,",values[i]);
        printf("%d,",(int)(values[i]));
    }
    printf("\n");
    for(int i=0;i<MEM_SIZE;i++) {
        //printf("%3.1f,",values[i]);
        printf("%d,",(int)(result[i]));
    }
    printf("\n");
    printf("result=%d\n",result[0]);

    /* Finalization */
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    for(pg_idx=0;pg_idx<NUM_PROGRAM;pg_idx++) {
        ret = clReleaseKernel(kernels[pg_idx]);
        ret = clReleaseProgram(programs[pg_idx]);
        free(source_str[pg_idx]);
    }
    ret = clReleaseMemObject(memobj);
    ret = clReleaseMemObject(resultobj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

 
    return 0;
}