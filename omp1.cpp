/* Copyright (c) 2019 CSC Training */
/* Copyright (c) 2021 ENCCS */
#include<iostream>
#include<omp.h>

int main() 
{
    int num_devices = omp_get_num_devices();
    printf("Number of available devices %d\n", num_devices);
    int default_device = omp_get_default_device();
    printf("default_device %d\n", default_device);
    omp_set_default_device(default_device);

    #pragma omp target 
    {
        if (omp_is_initial_device()) 
        {
            printf("Running on host\n");    
        } 
        else 
        {
            int nteams= omp_get_num_teams(); 
            int nthreads= omp_get_num_threads();
            printf("Running on device with %d teams in total and %d threads in each team\n",nteams,nthreads);
        }
    } 
}
