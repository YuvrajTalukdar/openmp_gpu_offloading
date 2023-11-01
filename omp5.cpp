#include <iostream>
#include <omp.h>

using namespace std;

#define ARRAY_SIZE 100
/*
SIMD instruction use demonstration
for cpu simd includes avx instruction
for nvidia gpu it includes operations on int2 int4 double2

simdlen: The simdlen clause specifies the preferred vectorization width for a function. It indicates how many data elements should be processed in parallel using SIMD instructions when the function is vectorized. 

safelen: The safelen clause specifies a minimum loop iteration count for which the loop is safe to vectorize. It indicates that the loop is safe to be transformed into a vectorized form when the number of iterations is greater than or equal to the value specified in safelen. This clause helps the compiler ensure that vectorization won't lead to incorrect results

linear Clause: The linear clause is used to specify how loop iteration variables are distributed among threads. It is often used when you want to distribute loop iterations in a way that maintains a linear relationship between the loop counter and the iteration space.

aligned Clause: The aligned clause is used to specify alignment constraints for data accessed within a loop. It is typically used to ensure that data is aligned in memory, which can improve memory access performance, especially on SIMD architectures.
Memory alignment is typically expressed in terms of bytes, and it determines where the data starts in memory. When a data item is aligned, it means it begins at a memory address that is a multiple of a specific byte boundary. For example, specifying alignment of 16 bytes means that the data should start at an address that is a multiple of 16.

*/
#define NUM_TEAMS 1
#define NUM_THREADS_PER_TEAM 1

int main() 
{
    const int n=16;
    int x[n],y[n],a1=1;
    for(int a=0;a<n;a++)
    {
        y[a]=a;
        x[a]=a;
    }
    int result = 0;
    #pragma omp target teams map(y[:n]) num_teams(NUM_TEAMS)
    {
        if (omp_is_initial_device()) 
        {
            printf("Running on host\n");    
        } 
        else 
        {
            int team_id=omp_get_team_num();
            int nteams=omp_get_num_teams();//returns total no of teams
            int nthreads=omp_get_num_threads();//returns total no of threads
            printf("Running on device with %d teams in total and %d threads in each team. team_id: %d\n",nteams,nthreads,team_id);
        }
        #pragma omp parallel num_threads(NUM_THREADS_PER_TEAM)
        #pragma omp simd safelen(n) aligned(y:64)
        //#pragma omp parallel for num_threads(NUM_THREADS_PER_TEAM)
        for (int i=0; i<n; i++)
        {
            printf(" tid:%d",omp_get_thread_num());
            y[i] = i;
        }
    }
    for(int a=0;a<n;a++)
    {
        cout<<y[a]<<" ";
    }
    return 0;
}
