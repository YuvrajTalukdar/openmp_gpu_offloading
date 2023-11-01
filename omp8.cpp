#include <stdio.h>
#include <omp.h>


int copy_2d(void *dst, void *src, int dst_dev, int src_dev,const unsigned long sz,const unsigned long vol_sz,const unsigned long offset)
{
    const int num_dims = 2;
    const unsigned long vol_dims [2] = {vol_sz, vol_sz};
    const unsigned long dst_dims [2] = {sz, sz};
    const unsigned long src_dims [2] = {sz, sz};
    const unsigned long dst_offset [2] = {offset, offset};
    const unsigned long src_offset [2] = {0, 0};
    return omp_target_memcpy_rect(dst, src, sizeof(int),num_dims,vol_dims,dst_offset, src_offset,dst_dims, src_dims,dst_dev, src_dev);
}


#define N 8
int main()
{
    int dst_dev=omp_get_initial_device();
    int src_dev=omp_get_default_device();
    int DST[N][N];
    //int *SRC = nullptr; // Pointer for data allocated on the device
    // Explicitly allocate memory on the device using omp_target_alloc
    int (*SRC)[N] = (int(*)[N])omp_target_alloc(sizeof(int[N][N]), src_dev);
    //int (*SRC)[N] = omp_target_alloc(N*N, src_dev);
    #pragma omp target /*map(alloc: SRC)*/ is_device_ptr(SRC)
    {
        for (int i=0; i<N; i++)
        {
            for (int j=0; j<N; j++) 
            {   SRC[i][j] = 1;}
        }
    }

    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++) 
        {   DST[i] [j] = 0;}
    }

    #pragma taskwait
    copy_2d(DST, SRC, dst_dev, src_dev, N, 4, 2);
    omp_target_free(SRC, src_dev);
    for (int i=0; i<N; i++) 
    {
        for (int j=0; j<N; j++) 
        {   printf("%d" , DST[i] [j]);}
        printf("\n");
    }
    return 0;
}