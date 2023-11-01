#include<iostream>
#include<omp.h>

using namespace std;

int main() 
{
    int num_devices = omp_get_num_devices();
    cout<<"Number of available devices "<<num_devices<<"\n";

    //teams are also available for cpu based omp code.
    /*In OpenMP, a "team" is a group of threads that 
    work together to execute a parallel region of code. 
    The concept of teams is particularly relevant when 
    dealing with nested parallelism, which occurs when 
    you have parallel regions within other parallel 
    regions. OpenMP allows you to organize threads 
    into teams to control the execution of code more flexibly.*/
    
    //here we can think teams as cuda blocks and threads as cuda threads , which may not be exactly true but help us at least understand the contept currently.
    //no of threads per blocks/teams is set using thread_limit(64) or num_threads clause. Any one of them is sufficient.
    //we CANNOT create any arbirary number of threads, the no of threads per block/teams need to be in multiples of 32.
    //example if thread_limit=125 max possible value of num_threads is 95 as 95%32=3. If num_teams=2 than too no of threads per team will be 95.
    //no of teams/blocks can be any arbitary number.
    //this may be because cuda documentation recommends having no of threads in multiple of 32 for better hardware specific optimization.
    //max no of threads per block is 384 , which is true at least for sm_86. 

    #pragma omp target
    #pragma omp teams num_teams(1) thread_limit(256) 
    #pragma omp parallel num_threads(256)
    {
        if (omp_is_initial_device()) 
        {
            printf("Running on host\n");    
        } 
        else 
        {
            int nteams=omp_get_num_teams();//returns total no of teams
            int nthreads=omp_get_num_threads();//returns total no of threads
            printf("Running on device with %d teams in total and %d threads in each team\n",nteams,nthreads);
        }
        int team_id=omp_get_team_num();//returns current team id
        int thread_id=omp_get_thread_num();//returns current thread id
        printf("\nRunning on team_id %d and thread_id %d",team_id,thread_id);
    }

    return 0;
}
