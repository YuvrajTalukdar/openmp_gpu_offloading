#include <iostream>
#include <vector>
#include <omp.h>

/*
An if clause may appear on a target, target data, target update, target
enter data or target exit data construct.
Except for the target update construct, when the expression in an if clause
evaluates to false, then host fall back occurs and:
• The device for the construct is the host.
• The execution of the region occurs on the host device.
• Variables appearing in map clauses are mapped to the host's device data en­
vironment.
• If a device clause appears on the construct, it is ignored.
When the if clause expression evaluates to false on a target update construct,
then assignments resulting from the construct do not occur. The if clause is useful
for setting a threshold on the amount of computation in a target region versus the
*/
int main() 
{
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int* array_ptr = vec.data();
    int n = vec.size();

    #pragma omp target data map(tofrom: array_ptr[0:n])
    //#pragma omp target data 
    {
        #pragma omp target if(n > 3)//try changing this value
        {
            if (omp_is_initial_device()) 
            {
                printf("Running on host\n");    
            }
            // Now you can use 'array_ptr' on the target device
            // 'array_ptr' points to the data in 'vec'
            for (int i = 0; i < n; i++) {
                // Access 'array_ptr' on the device
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
