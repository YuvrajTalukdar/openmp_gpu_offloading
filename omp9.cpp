#include <iostream>
#include <omp.h>
#include<unistd.h>

/*
 The nowait clause changes the target
task into a deferrable task. This means that after generating the target task, the
host thread may immediately continue executing the code after the device construct.

*/
int main() {
    const int N = 100;
    int data[N];

    // Initialize data
    for (int i = 0; i < N; i++) {
        data[i] = i;
    }

    // Offload a task to the GPU
    #pragma omp target teams distribute parallel for nowait map(to: data[0:N]) num_teams(1) num_threads(1)
    for (int i = 0; i < N; i++) {
        // Perform some computation on the GPU
        data[i] += 1;
        printf("%d ",data[i]);
    }
    //#pragma omp taskwait
    // Continue with additional work on the CPU
    for (int i = 0; i < N; i++) {
        data[i] -= 1;
    }

    // The 'taskwait' construct ensures all previous tasks are completed
    #pragma omp taskwait

    // Print the results
    for (int i = 0; i < N; i++) {
        std::cout << "data[" << i << "] = " << data[i] << std::endl;
    }

    return 0;
}
