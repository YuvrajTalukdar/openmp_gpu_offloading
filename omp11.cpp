#include <iostream>
#include <omp.h>

/*
map(alloc: list): This clause allocates memory on the target device for the specified data and initializes it. It is useful when you want to create a new data structure on the target device.
exit is used for notifying 
*/
int main() {
    const int N = 5;
    int *deviceArray = nullptr; // Pointer for data allocated on the device

    // Explicitly allocate memory on the device using omp_target_alloc
    deviceArray = (int *)omp_target_alloc(N * sizeof(int), omp_get_initial_device());
    int data[10]={0};
    // Initialize 'deviceArray' with data on the device
    /*in this pattern, the map(alloc: variable) clause within the #pragma omp target data construct ensures that memory for variable is allocated on the device when entering the data region and deallocated when exiting the region. 
    Explicit use of #pragma omp target enter data is not required in most cases.
    However, there may be situations where you need more explicit control over data management, 
    and that's where #pragma omp target enter data and #pragma omp target exit data directives can be useful. 
    These directives provide fine-grained control over when and how data is allocated and transferred between the host and the device.*/
    #pragma omp target enter data map(alloc: deviceArray[0:N]) //map(tofrom: data)
    #pragma omp target teams distribute parallel for 
    for (int i = 0; i < N; i++) {
        deviceArray[i] = i * 2;
        //printf(" %d ",deviceArray[i]);
    }

    // Code on the device using 'deviceArray'
    // ...

    // Transfer 'deviceArray' data back to the host
    #pragma omp target exit data map(from: deviceArray[0:N])
    /*#pragma omp target exit data: When this directive is encountered, it triggers the transfer of data from the offloaded device back to the host. The map(from: variable) clause is often used with this directive to specify which data should be transferred. The data on the device is copied back to the host.*/
    // Print the results on the host
    for (int i = 0; i < N; i++) {
        std::cout << "deviceArray[" << i << "] = " << deviceArray[i] << std::endl;
    }

    // Deallocate memory on the device
    omp_target_free(deviceArray, omp_get_initial_device());

    return 0;
}
