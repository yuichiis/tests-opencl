#include <stdio.h>

typedef struct _cl_platform_id *    cl_platform_id;

int main(int argc, char *argv)
{
    cl_platform_id *ids;
    printf("cl_platform_id size=%zd\n",sizeof(cl_platform_id));
    printf("long size=%zd\n",sizeof(long));
    printf("size_t size=%zd\n",sizeof(size_t));
    printf("long long size=%zd\n",sizeof(long long));
    return 0;
}