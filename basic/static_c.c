#include <stdio.h>
#include <stdlib.h>


/*
 * OpenCL Platform Layer API Test
 */
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/opencl.h>


static cl_uint GetNumPlatforms(void)
{
    cl_uint numPlatforms = 0;
    cl_int result = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (result != CL_SUCCESS) {
        printf("clGetPlatformIDs() failed : %d\n", result);
    }

    return numPlatforms;
}

static cl_platform_id* GetPlatformIDs(cl_uint numPlatforms)
{
    size_t allocateSize = sizeof(cl_platform_id) * numPlatforms;
    cl_platform_id* ids = (cl_platform_id*)malloc(allocateSize);
    if (ids != NULL) {
        cl_int result = clGetPlatformIDs(numPlatforms, ids, NULL);
        if (result != CL_SUCCESS) {
            printf("clGetPlatformIDs() failed : %d\n", result);
            free(ids);
            ids = NULL;
        }
    }

    return ids;
}

static cl_int GetPlatformInfo(cl_platform_id id,
    cl_platform_info name,size_t param_value_size,char *param_value)
{
    size_t param_value_size_ret;
    cl_int result = clGetPlatformInfo(id,name,param_value_size,param_value,
                                                        &param_value_size_ret);
    if(result!=CL_SUCCESS) {
        strncpy(param_value,"profile error",param_value_size);
        return result;
    }
    if(param_value_size_ret>=param_value_size) {
        strncpy(param_value,"profile overflow",param_value_size);
        return CL_OUT_OF_HOST_MEMORY;
    }
    param_value[param_value_size_ret] = 0;
    return result;
}

static void printPlatformInfo(cl_platform_id id)
{
    char param_value[256];
    if(GetPlatformInfo(id,CL_PLATFORM_PROFILE,
                sizeof(param_value),param_value)==CL_SUCCESS) {
        printf("  Profile:    %s\n",param_value);
    }
    if(GetPlatformInfo(id,CL_PLATFORM_VERSION,
                sizeof(param_value),param_value)==CL_SUCCESS) {
        printf("  Version:    %s\n",param_value);
    }
    if(GetPlatformInfo(id,CL_PLATFORM_NAME,
                sizeof(param_value),param_value)==CL_SUCCESS) {
        printf("  Name:       %s\n",param_value);
    }
    if(GetPlatformInfo(id,CL_PLATFORM_VENDOR,
                sizeof(param_value),param_value)==CL_SUCCESS) {
        printf("  Vendor:     %s\n",param_value);
    }
    if(GetPlatformInfo(id,CL_PLATFORM_EXTENSIONS,
                sizeof(param_value),param_value)==CL_SUCCESS) {
        printf("  Extensions: %s\n",param_value);
    }
}

static cl_uint GetNumDeviceIDs(cl_platform_id platform, cl_device_type device_type)
{
    cl_uint num_devices = 0;
    cl_uint num_entries = 0;
    cl_int result = clGetDeviceIDs(platform,device_type,num_entries,NULL,&num_devices);
    if (result != CL_SUCCESS) {
        printf("clGetDeviceIDs() failed : %d\n", result);
        return 0;
    }
    return num_devices;
}

static cl_device_id* GetDeviceIDs(cl_platform_id platform,
    cl_device_type device_type, cl_uint num_devices)
{
    cl_device_id* ids = (cl_device_id*)malloc(num_devices*sizeof(cl_device_id));
    if (ids != NULL) {
        cl_int result = clGetDeviceIDs(platform,device_type,num_devices, ids, NULL);
        if (result != CL_SUCCESS) {
            printf("clGetDeviceIDs() failed : %d\n", result);
            free(ids);
            ids = NULL;
        }
    }

    return ids;
}

static cl_int GetDeviceInfo(cl_device_id id,
    cl_device_info name, size_t param_value_size,void *param_value)
{
    size_t param_value_size_ret;
    cl_int result = clGetDeviceInfo(id,name,param_value_size,param_value,
                                                        &param_value_size_ret);
    if(result!=CL_SUCCESS) {
        return result;
    }
    if(param_value_size_ret>=param_value_size) {
        return CL_OUT_OF_HOST_MEMORY;
    }
    return result;
}

static void printDeviceInfo(cl_device_id id)
{
    char *string_true = "true";
    char *string_false = "false";
    char *string_bool;
    cl_int result;
    cl_bool param_bool;
    cl_uint param_uint;
    cl_ulong param_ulong;
    size_t param_size_t;
    char param_string[1024];
    size_t param_value_size_ret;
    cl_device_type param_device_type;
    if(clGetDeviceInfo(id,CL_DEVICE_NAME,
                sizeof(param_string),param_string,&param_value_size_ret)==CL_SUCCESS) {
        if(param_value_size_ret>=sizeof(param_string)) {
            param_value_size_ret = sizeof(param_string)-1;
        }
        param_string[param_value_size_ret] = 0;
        printf("    DEVICE_NAME:         %s\n",param_string);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_VENDOR,
                sizeof(param_string),param_string,&param_value_size_ret)==CL_SUCCESS) {
        if(param_value_size_ret>=sizeof(param_string)) {
            param_value_size_ret = sizeof(param_string)-1;
        }
        param_string[param_value_size_ret] = 0;
        printf("    DEVICE_VENDOR:       %s\n",param_string);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_PROFILE,
                sizeof(param_string),param_string,&param_value_size_ret)==CL_SUCCESS) {
        if(param_value_size_ret>=sizeof(param_string)) {
            param_value_size_ret = sizeof(param_string)-1;
        }
        param_string[param_value_size_ret] = 0;
        printf("    DEVICE_PROFILE:      %s\n",param_string);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_AVAILABLE,
                sizeof(param_bool),&param_bool,NULL)==CL_SUCCESS) {
        if(param_bool==CL_TRUE) { string_bool = string_true; }
        else { string_bool = string_false; }
        printf("    AVAILABLE:           %s\n",string_bool);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_TYPE,
                sizeof(param_device_type),&param_device_type,NULL)==CL_SUCCESS) {
        char *string_device_type="Unknown";
        switch(param_device_type) {
            case CL_DEVICE_TYPE_CPU: string_device_type = "CPU"; break;
            case CL_DEVICE_TYPE_GPU: string_device_type = "GPU"; break;
            case CL_DEVICE_TYPE_ACCELERATOR: string_device_type = "ACCELERATOR"; break;
            case CL_DEVICE_TYPE_DEFAULT: string_device_type = "DEFAULT"; break;
            case CL_DEVICE_TYPE_CUSTOM: string_device_type = "CUSTOM"; break;
        }
        printf("    DEVICE_TYPE:         %s\n",string_device_type);
    }
    if(clGetDeviceInfo(id,CL_DRIVER_VERSION,
                sizeof(param_string),param_string,&param_value_size_ret)==CL_SUCCESS) {
        if(param_value_size_ret>=sizeof(param_string)) {
            param_value_size_ret = sizeof(param_string)-1;
        }
        param_string[param_value_size_ret] = 0;
        printf("    DRIVER_VERSION:      %s\n",param_string);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_VERSION,
                sizeof(param_string),param_string,&param_value_size_ret)==CL_SUCCESS) {
        if(param_value_size_ret>=sizeof(param_string)) {
            param_value_size_ret = sizeof(param_string)-1;
        }
        param_string[param_value_size_ret] = 0;
        printf("    DEVICE_VERSION:      %s\n",param_string);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_OPENCL_C_VERSION,
                sizeof(param_string),param_string,&param_value_size_ret)==CL_SUCCESS) {
        if(param_value_size_ret>=sizeof(param_string)) {
            param_value_size_ret = sizeof(param_string)-1;
        }
        param_string[param_value_size_ret] = 0;
        printf("    OPENCL_C_VERSION:    %s\n",param_string);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_ADDRESS_BITS,
                sizeof(param_uint),&param_uint,NULL)==CL_SUCCESS) {
        printf("    ADDRESS_BITS:        %d\n",param_uint);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_HOST_UNIFIED_MEMORY,
                sizeof(param_bool),&param_bool,NULL)==CL_SUCCESS) {
        if(param_bool==CL_TRUE) { string_bool = string_true; }
        else { string_bool = string_false; }
        printf("    HOST_UNIFIED_MEMORY: %s\n",string_bool);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_ENDIAN_LITTLE,
                sizeof(param_bool),&param_bool,NULL)==CL_SUCCESS) {
        if(param_bool==CL_TRUE) { string_bool = string_true; }
        else { string_bool = string_false; }
        printf("    ENDIAN_LITTLE:       %s\n",string_bool);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_IMAGE_SUPPORT,
                sizeof(param_bool),&param_bool,NULL)==CL_SUCCESS) {
        if(param_bool==CL_TRUE) { string_bool = string_true; }
        else { string_bool = string_false; }
        printf("    IMAGE_SUPPORT:       %s\n",string_bool);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_BUILT_IN_KERNELS,
                sizeof(param_string),param_string,&param_value_size_ret)==CL_SUCCESS) {
        if(param_value_size_ret>=sizeof(param_string)) {
            param_value_size_ret = sizeof(param_string)-1;
        }
        param_string[param_value_size_ret] = 0;
        printf("    BUILT_IN_KERNELS:    %s\n",param_string);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_COMPILER_AVAILABLE,
                sizeof(param_bool),&param_bool,NULL)==CL_SUCCESS) {
        if(param_bool==CL_TRUE) { string_bool = string_true; }
        else { string_bool = string_false; }
        printf("    COMPILER_AVAILABLE:  %s\n",string_bool);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_GLOBAL_MEM_SIZE,
                sizeof(param_ulong),&param_ulong,NULL)==CL_SUCCESS) {
        printf("    GLOBAL_MEM_SIZE:     %lldMB\n",param_ulong/1024/1024);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_LOCAL_MEM_SIZE,
                sizeof(param_ulong),&param_ulong,NULL)==CL_SUCCESS) {
        printf("    LOCAL_MEM_SIZE:      %lldKB\n",param_ulong/1024);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                sizeof(param_ulong),&param_ulong,NULL)==CL_SUCCESS) {
        printf("    MAX_MEM_ALLOC_SIZE:  %lldMB\n",param_ulong/1024/1024);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_MAX_CLOCK_FREQUENCY,
                sizeof(param_uint),&param_uint,NULL)==CL_SUCCESS) {
        printf("    MAX_CLOCK_FREQUENCY: %dMHz\n",param_uint);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_MAX_COMPUTE_UNITS,
                sizeof(param_uint),&param_uint,NULL)==CL_SUCCESS) {
        printf("    MAX_COMPUTE_UNITS:   %d\n",param_uint);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                sizeof(param_uint),&param_uint,NULL)==CL_SUCCESS) {
        printf("    MAX_WORK_ITEM_DIMENSIONS: %d\n",param_uint);
    }
    if(clGetDeviceInfo(id,CL_DEVICE_MAX_WORK_GROUP_SIZE,
                sizeof(param_size_t),&param_size_t,NULL)==CL_SUCCESS) {
        printf("    MAX_WORK_GROUP_SIZE: %lld\n",param_size_t);
    }

    if(clGetDeviceInfo(id,CL_DEVICE_EXTENSIONS,
                sizeof(param_string),param_string,&param_value_size_ret)==CL_SUCCESS) {
        if(param_value_size_ret>=sizeof(param_string)) {
            param_value_size_ret = sizeof(param_string)-1;
        }
        param_string[param_value_size_ret] = 0;
        printf("    DEVICE_EXTENSIONS:    %s\n",param_string);
    }

}

int main()
{
    printf("OpenCL Platform Layer API Test\n");

    cl_uint numPlatforms = GetNumPlatforms();
    if (numPlatforms > 0) {
        printf("Number Of Platforms : %d\n", numPlatforms);

        cl_platform_id* ids = GetPlatformIDs(numPlatforms);
        if (ids != NULL) {
            for (cl_uint i = 0; i < numPlatforms; i++) {
                printf("Platform No. %d : \n", i);
                printPlatformInfo(ids[i]);
                cl_uint numDeviceIds = GetNumDeviceIDs(ids[i],CL_DEVICE_TYPE_ALL);
                if(numDeviceIds==0) {
                    continue;
                }
                cl_device_id *deviceIds = GetDeviceIDs(ids[i],CL_DEVICE_TYPE_ALL,numDeviceIds);
                if(deviceIds==NULL) {
                    continue;
                }
                for(cl_uint j=0; j<numDeviceIds; j++) {
                    printf("  Device No. %d : \n", j);
                    printDeviceInfo(deviceIds[j]);
                }
                free(deviceIds);
            }
            free(ids);
        }
    }
}
