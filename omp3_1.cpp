#include <iostream>
#include <omp.h>

using namespace std;

#define ARRAY_SIZE 100
/*
Demonstration of teams in cpu
The maximum number of threads participating in the contention group that each team initiates is implementation defined, 
but is less than or equal to the value specified in the thread_limit clause. 

use export OMP_NUM_THREADS=32 to set the max possible thread_limit value for cpu

in cpu, actual no of threads per team = ARGMIN {(THREAD_LIMIT / NUM_TEAMS),NUM_THREADS_PER_TEAM}

in teams for cpu 
if thread_limit < OMP_NUM_THREADS than fine it will work
else thread_limit = OMP_NUM_THREADS
OMP_NUM_THREADS needs to be exported in order to modify it

in teams for gpu
in teams for gpu value of thread_limit is final
num_thrreads needs to be in multiples of 32 but cannot exceed thread_limit value
if no of teams is more than 1 , still the actual threads per team remains same as ARGMIN {(thread_limit,num_threads)}

Say if we have no of physical threads = 12
we set NUM_TEAMS=15 and NUM_THREADS_PER_TEAM=1, than too it looks like total threads will be 12 only but some threads will run more than 1 team.
if NUM_TEAMS>12 than omp will run 12 teams in parallel
if NUM_TEAMS<12 omp will run all the teams in parallel 

The target teams construct is asserting a more restricted form of parallelism than the parallel construct allows.
b. Because the teams that are started by a target teams construct are each in
their own contention group, threads from different teams cannot synchronize
with each other.

a. The only OpenMP constructs that can appear in a teams region are the
parallel, distribute and any other parallel or distribute regions arising
from related constructs. But inside the parallel construct which is itself present inside a team can have all possible omp constructs.

look at pg 279(actual) for pics of teams.
*/
#define NUM_TEAMS 4
#define THREAD_LIMIT 32
#define NUM_THREADS_PER_TEAM 8

int main() {
    int array[ARRAY_SIZE];
    int result[NUM_TEAMS];

    // Initialize the array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    // Initialize the result array
    for (int i = 0; i < NUM_TEAMS; i++) {
        result[i] = 0;
    }

    #pragma omp teams num_teams(NUM_TEAMS) thread_limit(THREAD_LIMIT)
    {
        #pragma omp parallel num_threads(NUM_THREADS_PER_TEAM)
        {
            int nteams=omp_get_num_teams();//returns total no of teams
            int num_threads=omp_get_num_threads();//returns total no of threads
            int thread_id=omp_get_thread_num();
            int team_id=omp_get_team_num();
            printf("Running on device with %d teams in total and %d threads in each team. thread_id: %d, team_id: %d\n",nteams,num_threads,thread_id,team_id);

            // Calculate the sum for each team
            int local_sum = 0;
            int chunk_size = ARRAY_SIZE / num_threads;
            int start = thread_id * chunk_size;
            int end = start + chunk_size;

            for (int i = start; i < end; i++) {
                local_sum += array[i];
            }

            // Use a critical section to update the result array
            #pragma omp critical
            {
                result[team_id] += local_sum;
            }
        }
    }

    // Print the results
    for (int i = 0; i < NUM_TEAMS; i++) {
        cout<<"Team "<<i<<": Sum = "<< result[i]<<"\n";
    }

    return 0;
}
