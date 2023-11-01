#include <iostream>
#include <omp.h>

using namespace std;

#define ARRAY_SIZE 100
/*
 The distribute construct has the potential
for better performance because of the restrictions on where it can be used and what
other OpenMP constructs can appear inside the distribute region. This enables the
compiler to be more aggressive with optimizations. The for and do constructs are
more versatile but may not perform as well.

The idea behind the target teams and distribute constructs is to spread the
execution of a loop coarsely across hardware compute units and then more finely
to the threads that execute within those compute units. What we have shown so
far is how to distribute the loop iteration to the compute units.

The distribute construct at line 5 assigns the execution of two iterations in
the outer loop to a league of two initial threads. The parallel construct at line 8
is then encountered by each initial thread with different values for j. Each initial
thread becomes the master thread in a team of four threads. The first team executes
the first half of the loop iterations and the second team executes the other half.
The loop iterations scheduled to execute on an initial thread are then scheduled
according to the for worksharing construct at line 9, to execute on the team of
threads that the initial thread is now the master of.

In accelerated worksharing, loops are first scheduled at a coarse level to teams
and then more finely to the threads in each team. We rewrote the saxpy loop in
order to schedule it across two levels of parallelism: teams and threads. However,
rewriting loops is tedious and is something we want to avoid.
*/

int main() 
{
    int n=100;
    int x[n],y[n],a1=1;
    for(int a=0;a<n;a++)
    {
        y[a]=a;
        x[a]=a;
    }
    int result = 0;
    #pragma omp target teams map(y[:n]) map(to:x[:n]) num_teams(2)
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
        #pragma omp distribute
        for (int j=0; j<n; j+=n/2)
        {
            #pragma omp parallel for num_threads(4)
            //#pragma omp for
            for (int i=j; i<n/2; i++)
            {
                y[i] = y[i] + a1*x[i];
            }
        }
    }
    for(int a=0;a<n;a++)
    {
        cout<<y[a]<<" ";
    }
    return 0;
}
