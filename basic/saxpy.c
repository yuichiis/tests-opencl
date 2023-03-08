//
// Copyright (c) 2010 Advanced Micro Devices, Inc. All rights reserved.
//

// A minimalist OpenCL program.

#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <stdio.h>

#define NWITEMS 256
// A simple memset kernel
const char *source =
//"kernel void memset(   global uint *dst )             \n"
//"{                                                    \n"
//"    dst[get_global_id(0)] = get_global_id(0);        \n"
//"}                                                    \n";

"__kernel void saxpy(const global float * x,\n"
"                    __global float * y,\n"
"                    const float a)\n"
"{\n"
"   uint gid = get_global_id(0);\n"
"   y[gid] = a* x[gid] + y[gid];\n"
"}\n";

int main(int argc, char ** argv)
{
  cl_float a      = 2.0f;
  cl_float hostX[NWITEMS];
  cl_float hostY[NWITEMS];
  for(int i = 0; i < NWITEMS; i++)
  {
    hostX[i] = (cl_float)i;             // 0           : 255
    hostY[i] = (cl_float)(NWITEMS-1-i); // 256-1-0=255 : 256-1-255 = 0
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

  // 4. Perform runtime source compilation, and obtain kernel entry point.
  cl_program program = clCreateProgramWithSource(
      context, // cl_context context,
      1,       // cl_uint count,
      &source, // const char ** strings,
      NULL,    // const size_t * lengths,
      NULL );  // cl_int * errcode_ret

  clBuildProgram(
      program, // cl_program program,
      1,       // cl_uint num_devices,
      &device, // const cl_device_id * device_list,
      NULL,    // const char * options,
      NULL,    // CL_CALLBACK *  pfn_notify
      NULL );  // void * user_data

  cl_kernel kernel = clCreateKernel(
      program, // cl_program program,
      "saxpy", // const char * kernel_name,
      NULL );  // cl_int * errcode_ret

  // 5. Create a data buffer.
  //cl_mem buffer = clCreateBuffer( context,
  //                                CL_MEM_WRITE_ONLY,
  //                                NWITEMS * sizeof(cl_uint),
  //                                NULL, NULL );
  cl_float *pX = hostX;
  cl_float *pY = hostY;
  cl_mem bufX = clCreateBuffer( context,
                                  CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                                  NWITEMS * sizeof(cl_float),
                                  (void*)pX, NULL );
  cl_mem bufY = clCreateBuffer( context,
                                  CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,
                                  NWITEMS * sizeof(cl_float),
                                  (void*)pY, NULL );

  // 6. Launch the kernel. Let OpenCL pick the local work size.
  size_t global_work_size = NWITEMS;
  size_t local_work_size = 64;
  //clSetKernelArg(kernel, 0, sizeof(buffer), (void*) &buffer);
  clSetKernelArg(kernel, 0, sizeof(bufX), (void*) &bufX);
  clSetKernelArg(kernel, 1, sizeof(bufY), (void*) &bufY);
  clSetKernelArg(kernel, 2, sizeof(a),    (void*) &a);
  cl_event event = NULL;

  clEnqueueNDRangeKernel( queue,    // cl_command_queue
                          kernel,   // cl_kernel
                          1,        // cl_uint work_dim
                          NULL,     // const size_t * global_work_offset
                          &global_work_size, // const size_t *
                          &local_work_size,  // const size_t *
                          0,        // cl_uint num_events_in_wait_list
                          NULL,     // const cl_event * event_wait_list
                          &event);    // cl_event * event

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
                      bufY,                       // c_mem buffer
                      CL_TRUE,                    // blocking_read,
                      0,                          // offset,
                      NWITEMS * sizeof(cl_float), // size,
                      (void *)hostY,              // ptr,
                      0,                          // num_events_in_wait_list,
                      NULL,                       // event_wait_list,
                      NULL                        //event
                  );


  for(i=0; i < NWITEMS; i++) {
      printf("%d:%6.2f, ", i, hostY[i]);
  }
  printf("\n");

  return 0;
}
