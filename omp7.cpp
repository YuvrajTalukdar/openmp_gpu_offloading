#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int* array_ptr = vec.data();
    int n = vec.size();

    #pragma omp target data map(tofrom: array_ptr[0:n])
    {
        #pragma omp target
        {
            // Now you can use 'array_ptr' on the target device
            // 'array_ptr' points to the data in 'vec'
            for (int i = 0; i < n; i++) {
                array_ptr[i]++;
            }
        }
    }

    // Data on the host 'vec' remains unchanged
    for (int i = 0; i < n; i++) {
        std::cout << "vec[" << i << "] = " << vec[i] << " ";
    }

    return 0;
}
