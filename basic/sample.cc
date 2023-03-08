//
// https://blog.techlab-xe.net/archives/2495
//

#include <stdio.h>
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
 
PFNCLGETPLATFORMIDS _g_clGetPlatformIDs = NULL;
PFNCLGETPLATFORMINFO _g_clGetPlatformInfo = NULL;
PFNCLGETDEVICEIDS   _g_clGetDeviceIDs = NULL;
PFNCLGETDEVICEINFO  _g_clGetDeviceInfo = NULL;
 
int main() {
    HMODULE h = LoadLibraryA( "OpenCL.dll" );
    _g_clGetPlatformIDs = (PFNCLGETPLATFORMIDS)GetProcAddress( h, "clGetPlatformIDs" );
    _g_clGetPlatformInfo = (PFNCLGETPLATFORMINFO) GetProcAddress( h, "clGetPlatformInfo" );
    _g_clGetDeviceIDs = (PFNCLGETDEVICEIDS)GetProcAddress( h, "clGetDeviceIDs" );
    _g_clGetDeviceInfo = (PFNCLGETDEVICEINFO)GetProcAddress( h, "clGetDeviceInfo" );
 
    cl_uint numPlatforms = 0;
    cl_int retCl = _g_clGetPlatformIDs( 0, NULL, &numPlatforms );
    if( retCl != CL_SUCCESS ) {
        return -1;
    }
 
    std::vector<cl_platform_id> clPlatforms( numPlatforms );
    retCl = _g_clGetPlatformIDs( numPlatforms, &(clPlatforms[0]), NULL );
    if( retCl != CL_SUCCESS ) {
        return -1;
    }
 
    for( auto pid : clPlatforms ) {
        cl_uint numDevices = 0;
        char buf[4096] = { 0 };
        size_t bufSize = sizeof(buf);
        retCl = _g_clGetPlatformInfo( pid, CL_PLATFORM_NAME, bufSize, buf, NULL );
        if( retCl == CL_SUCCESS ) {
            printf( "PlatformName: %s\n", buf );
        }
 
        retCl = _g_clGetDeviceIDs( pid, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices );
        if( retCl != CL_SUCCESS ) {
            continue;
        }
        std::vector<cl_device_id> devices( numDevices );
        retCl = _g_clGetDeviceIDs( pid, CL_DEVICE_TYPE_ALL, numDevices, &(devices[0]), NULL );
        if( retCl != CL_SUCCESS ) {
            continue;
        }
        for( auto did : devices ) {
            bufSize = sizeof(buf);
            retCl = _g_clGetDeviceInfo( did, CL_DEVICE_NAME, bufSize, buf, NULL );
            if( retCl == CL_SUCCESS ) {
                printf( "  DeviceName: %s\n", buf );
            }
            bufSize = sizeof(buf);
            retCl = _g_clGetDeviceInfo( did, CL_DEVICE_EXTENSIONS, bufSize, buf, NULL );
            if( retCl == CL_SUCCESS ) {
                printf( "  > DeviceExtensions: %s\n", buf );
            }
            printf( "\n" );
        }
    }
 
    FreeLibrary( h );
 
    return 0;
}
