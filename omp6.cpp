#include <iostream>
#include <omp.h>

using namespace std;

/*
map(to: list): host to the target device. to can be replaced with in. It ensures that the target device has read-only access to the data.

map(from: list): device to the host. from can be replaced with out. It ensures that the host can read the data modified on the target device.

map(tofrom: list): transferred in both directions— from the host to the target device and from the target device to the host. tofrom can be replaced with inout. This is used for data that needs to be both read and modified on the target device.

map(alloc: list): This clause allocates memory on the target device for the specified data and initializes it. It is useful when you want to create a new data structure on the target device.
demonstration of this is in program omp11.cpp

map(release: list): This clause deallocates memory on the target device for the specified data when the associated region is exited. It's used when you no longer need the data on the target device.
void release() {
 double *P = ptr;
 #pragma omp target exit data map(release:p[O:length])
 delete[] p;
 ptr= O;
 length= O;
 }
here when line ptr=0 gets executed the release is actually happen;

map(delete: list): This clause deallocates memory on the target device for the specified data immediately. It is used when you want to explicitly release memory at a specific point in your code.
 #pragma omp target exit data map(delete:S) here immideately the release will happen

map(derived: list): This clause is used when you have derived data structures on the device, and you want to ensure that changes to the base data are reflected in the derived data and vice versa.
map(derived: structArray) clause, which is used when you have derived data structures on the device. Derived data structures are typically a combination of different data elements or arrays on the device. The map(derived: list) clause ensures that changes to the base data are reflected in the derived data and vice versa.
he concept of "derived" in the context of OpenMP map(derived: list) is not about having a subset of the original array. It's about declaring a new data structure or view on the device that is derived from the original data. This derived data structure contains elements that correspond to elements in the original data, but the derived data structure can have a different structure or organization.
*/
int main() 
{
    const int N = 5;
    int a[N], b[N], c[N], d[N], e[N];
    int structArray[N];

    // Initialize data
    for (int i = 0; i < N; i++) {
        a[i] = i;
        structArray[i] = i;
        c[i]=10;
    }

    // map(to: a) - Data transferred from host to device
    //#pragma omp target data map(to: a)
    //{
        // a is read-only on the target device
        #pragma omp target map(to: a)
        {
            // Code on the target device using 'a'
            for (int i = 0; i < N; i++) {
                // Access 'a' (read-only)
                printf("a[%d]= %d ",i,a[i]);
            }
        }
    //}

    // map(from: b) - Data transferred from device to host
    #pragma omp target data map(from: b)
    {
        // b is initially uninitialized on the device

        #pragma omp target
        {
            // Code on the target device modifying 'b'
            for (int i = 0; i < N; i++) {
                b[i] = 2 * i;
            }
        }
    }

    // b is now updated on the host
    cout<<"\n";
    for (int i = 0; i < N; i++) {
        cout << "b[" << i << "] = " << b[i]<<" ";
    }
    //int g[10];
    cout<<"\ndevice_id : "<<omp_get_initial_device()<<"\n";
    int *g=(int*)omp_target_alloc(sizeof(int)*N,omp_get_initial_device());
    // map(tofrom: c) - Data transferred in both directions
    #pragma omp target data map(tofrom: c) map(to: a) map(alloc:g) //is_device_ptr(g)
    {
        //#pragma omp target
        //{
            // Code on the target device reading and modifying 'c'
            for (int i = 0; i < N; i++) {
                c[i] += a[i];
                g[i]=c[i];
            }
        //}
    }

    //#pragma omp target exit data map(delete:g) //#pragma omp target exit data map(delete:v[0:len])

    // c is updated on the host after modification on the device
    cout<<"\n";
    for (int i = 0; i < N; i++) 
    {
        cout << "c[" << i << "] = " << c[i] <<" ";//<<g[i];
    }

    // map(alloc: d) - Allocate memory on the target device
    /*#pragma omp target data map(alloc: d)
    {
        // 'd' is allocated and uninitialized on the device
    }

    // map(release: d) - Deallocate memory on the device when exiting the region
    #pragma omp target data map(release: d)
    {
        // Code on the target device using 'd' (d is allocated)

        // Memory is released when exiting this region
    }*/

    // map(derived: structArray) - Working with derived data
    #pragma omp target data map(to: structArray[0:N])
    {
        #pragma omp target
        {
            // Code on the target device using 'structArray' elements
            for (int i = 0; i < N; i++) {
                structArray[i] *= 2;
            }
        }
        #pragma omp target update from(structArray) //device to host
        //#pragma omp target update to(structArray) //host to device
    }

    // structArray reflects changes made on the device
    cout<<"\n";
    for (int i = 0; i < N; i++) 
    {
        cout << "structArray[" << i << "] = " << structArray[i] <<" ";
        structArray[i]++;
    }


    return 0;
}
