//
// https://blog.techlab-xe.net/archives/2495
//

#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <stddef.h>
#include <Windows.h>
#include "CL/cl.h"
#include <vector>

typedef cl_int (CL_API_CALL *PFNCLGETPLATFORMIDS)(cl_uint          /* num_entries */,
                 cl_platform_id * /* platforms */,
                 cl_uint *        /* num_platforms */);
 
typedef cl_int (CL_API_CALL  *PFNCLGETPLATFORMINFO)(cl_platform_id   /* platform */, 
                  cl_platform_info /* param_name */,
                  size_t           /* param_value_size */, 
                  void *           /* param_value */,
                  size_t *         /* param_value_size_ret */);
 
/* Device APIs */
typedef cl_int (CL_API_CALL *PFNCLGETDEVICEIDS)(cl_platform_id   /* platform */,
               cl_device_type   /* device_type */, 
               cl_uint          /* num_entries */, 
               cl_device_id *   /* devices */, 
               cl_uint *        /* num_devices */);
 
typedef cl_int (CL_API_CALL *PFNCLGETDEVICEINFO)(cl_device_id    /* device */,
                cl_device_info  /* param_name */, 
                size_t          /* param_value_size */, 
                void *          /* param_value */,
                size_t *        /* param_value_size_ret */);

// #
// # Initialization processing
// #

int main()
{

//
// Function pointer. Define the type in advance
//

PFNCLCREATECONTEXT _g_clCreateContext = NULL;
PFNCLRELEASECONTEXT _g_clReleaseContext = NULL;
PFNCLCREATECOMMANDQUEUE _g_clCreateCommandQueue = NULL;
PFNCLRELEASECOMMANDQUEUE _g_clReleaseCommandQueue = NULL;
PFNCLCREATEBUFFER  _g_clCreateBuffer = NULL;
PFNCLRELEASEMEMOBJECT _g_clReleaseMemObject = NULL;
PFNCLCREATEPROGRAMWITHSOURCE _g_clCreateProgramWithSource = NULL;
PFNCLRELEASEPROGRAM _g_clReleaseProgram = NULL;
PFNCLBUILDPROGRAM _g_clBuildProgram = NULL;
PFNCLCREATEKERNEL _g_clCreateKernel = NULL;
PFNCLRELEASEKERNEL _g_clReleaseKernel = NULL;
PFNCLSETKERNELARG  _g_clSetKernelArg = NULL;
PFNCLENQUEUEREADBUFFER  _g_clEnqueueReadBuffer = NULL;
PFNCLENQUEUEWRITEBUFFER _g_clEnqueueWriteBuffer = NULL;
PFNCLENQUEUENDRANGEKERNEL _g_clEnqueueNDRangeKernel = NULL;
PFNCLFINISH _g_clFinish = NULL;
 
//
// Function pointer extraction unit
//
_g_clCreateContext = (PFNCLCREATECONTEXT)GetProcAddress( h, "clCreateContext" );
_g_clReleaseContext = (PFNCLRELEASECONTEXT)GetProcAddress( h, "clReleaseContext" );
_g_clCreateCommandQueue = (PFNCLCREATECOMMANDQUEUE)GetProcAddress( h, "clCreateCommandQueue" );
_g_clReleaseCommandQueue = (PFNCLRELEASECOMMANDQUEUE)GetProcAddress( h, "clReleaseCommandQueue" );
_g_clCreateBuffer = (PFNCLCREATEBUFFER)GetProcAddress( h, "clCreateBuffer" );
_g_clReleaseMemObject = (PFNCLRELEASEMEMOBJECT)GetProcAddress( h, "clReleaseMemObject" );
_g_clCreateProgramWithSource = (PFNCLCREATEPROGRAMWITHSOURCE) GetProcAddress( h, "clCreateProgramWithSource" );
_g_clBuildProgram = (PFNCLBUILDPROGRAM) GetProcAddress( h, "clBuildProgram" );
_g_clReleaseProgram = (PFNCLRELEASEPROGRAM) GetProcAddress( h, "clReleaseProgram" );
_g_clCreateKernel = (PFNCLCREATEKERNEL) GetProcAddress( h, "clCreateKernel" );
_g_clReleaseKernel = (PFNCLRELEASEKERNEL)GetProcAddress( h, "clReleaseKernel" );
_g_clSetKernelArg  = (PFNCLSETKERNELARG) GetProcAddress( h, "clSetKernelArg" );
_g_clEnqueueNDRangeKernel = (PFNCLENQUEUENDRANGEKERNEL) GetProcAddress( h, "clEnqueueNDRangeKernel" );
_g_clFinish = (PFNCLFINISH)GetProcAddress( h, "clFinish" );
_g_clEnqueueReadBuffer = (PFNCLENQUEUEREADBUFFER) GetProcAddress( h, "clEnqueueReadBuffer" );
_g_clEnqueueWriteBuffer = (PFNCLENQUEUEWRITEBUFFER) GetProcAddress( h, "clEnqueueWriteBuffer" );


//
// Do the kernel preparation
//
cl_platform_id platformId = NULL;
cl_device_id deviceId = NULL;
_g_clGetPlatformIDs( 1, &platformId, NULL );
retCl = _g_clGetDeviceIDs( platformId, CL_DEVICE_TYPE_GPU, 1, &deviceId, NULL );
if( retCl != CL_SUCCESS ) {
    return -1;
}
cl_context contextCL = _g_clCreateContext( 0, 1, &deviceId, NULL, NULL, &retCl );
if( retCl != CL_SUCCESS ) {
    return -1;
}
cl_command_queue commands = _g_clCreateCommandQueue( contextCL, deviceId, NULL, &retCl );
if( retCl != CL_SUCCESS ) {
    return -1;
}
 
size_t programLength = strlen( kernelSrc );
cl_program program = _g_clCreateProgramWithSource( contextCL, 1, (const char**)&kernelSrc, &programLength, &retCl );
if( retCl != CL_SUCCESS ) {
    return -1;
}
retCl = _g_clBuildProgram( program, 1, &deviceId, NULL, NULL, NULL );
if( retCl != CL_SUCCESS ) {
    return -1;
}
cl_kernel kernel = _g_clCreateKernel( program, "vector_add", &retCl );
if( retCl != CL_SUCCESS ) {
    return -1;
}


// #
// # Processing for array addition
// #


const char* kernelSrc =
    "__kernel void vector_add(__global const int* inputA, __global const int* inputB, __global int* outputC ){\n"
    "  int idx = get_global_id(0);\n"
    "  outputC[idx] = inputA[idx] + inputB[idx];\n"
    "}\0";



const int BUFFER_SIZE = 1024 * 1024;
const int BUFFER_BYTE_SIZE = sizeof(int) * BUFFER_SIZE;
cl_mem bufA = _g_clCreateBuffer( contextCL, CL_MEM_READ_ONLY, BUFFER_BYTE_SIZE, NULL, &retCl );
cl_mem bufB = _g_clCreateBuffer( contextCL, CL_MEM_READ_ONLY, BUFFER_BYTE_SIZE, NULL, &retCl );
cl_mem bufC = _g_clCreateBuffer( contextCL, CL_MEM_WRITE_ONLY, BUFFER_BYTE_SIZE, NULL, &retCl );
 
int* hostArrayA = (int*)malloc( BUFFER_BYTE_SIZE );
int* hostArrayB = (int*)malloc( BUFFER_BYTE_SIZE );
int* hostArrayC = (int*)malloc( BUFFER_BYTE_SIZE );
    
memset( hostArrayC, 0, BUFFER_BYTE_SIZE);
for( int i = 0; i < BUFFER_SIZE; ++i ) {
    hostArrayA[i] = i;
    hostArrayB[i] = BUFFER_SIZE - i;
}
_g_clEnqueueWriteBuffer( commands, bufA, CL_FALSE, 0, BUFFER_BYTE_SIZE, hostArrayA, 0, NULL, NULL );
_g_clEnqueueWriteBuffer( commands, bufB, CL_FALSE, 0, BUFFER_BYTE_SIZE, hostArrayB, 0, NULL, NULL );
_g_clSetKernelArg( kernel, 0, sizeof(cl_mem), &bufA );
_g_clSetKernelArg( kernel, 1, sizeof(cl_mem), &bufB );
_g_clSetKernelArg( kernel, 2, sizeof(cl_mem), &bufC );
 
size_t arraySize = BUFFER_SIZE;
_g_clEnqueueNDRangeKernel( commands, kernel, 1, NULL, &arraySize, NULL, 0, NULL, NULL );
_g_clEnqueueReadBuffer( commands, bufC, CL_TRUE, 0, BUFFER_BYTE_SIZE, hostArrayC, 0, NULL, NULL );
_g_clFinish( commands );
 
for( int i = 0; i < 10; ++i ) {
     printf( " %d : %d\n", i, hostArrayC[i] );
}


// #
// # Clean up
// #


_g_clReleaseKernel( kernel );
_g_clReleaseProgram( program );
_g_clReleaseMemObject( bufA );
_g_clReleaseMemObject( bufB );
_g_clReleaseMemObject( bufC );
_g_clReleaseCommandQueue( commands );
_g_clReleaseContext( contextCL );
 
free( hostArrayA );
free( hostArrayB );
free( hostArrayC );

	
}
