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

int main()
{
    printf("OpenCL Platform Layer API Test\n");

    cl_uint numPlatforms = GetNumPlatforms();
    if (numPlatforms > 0) {
        printf("Number Of Platforms : %d\n", numPlatforms);

        cl_platform_id* ids = GetPlatformIDs(numPlatforms);
        if (ids != NULL) {
            for (cl_uint i = 0; i < numPlatforms; i++) {
                printf("Platform No. %d : 0x%08x\n", i, (unsigned int)(ids[i]));
            }
            free(ids);
        }
    }
}
