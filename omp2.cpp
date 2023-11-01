/* Copyright (c) 2019 CSC Training */
/* Copyright (c) 2021 ENCCS */
#include<iostream>
#include<math.h>
#include<omp.h>

using namespace std;

#define NX 102400

int main(void)
{
  double vecA[NX],vecB[NX],vecC[NX];
  double r=0.2;

  /* Initialization of vectors */
  for (int i = 0; i < NX; i++) 
  {
      vecA[i] = pow(r, i);
      vecB[i] = 1.0;
  }

  /* dot product of two vectors */
  #pragma omp target 
  {
      if (omp_is_initial_device()) 
      {
          printf("Running on host\n");    
      } 
      else 
      {
          //cout<<"Running on device";//c++ code not allowed inside omp target clause
          printf("Running on device \n");
      }
      for (int i = 0; i < NX; i++) 
      { 
          vecC[i] = vecA[i] * vecB[i];
      }
      int team_id=omp_get_team_num();//returns current team id
        int thread_id=omp_get_thread_num();//returns current thread id
        printf("\nRunning on team_id %d and thread_id %d",team_id,thread_id);
  }

  double sum = 0.0;
  /* calculate the sum */
  for (int i = 0; i < NX; i++) 
  {
      sum += vecC[i];
  }
  cout<<"The sum is:"<<sum;
  return 0;
}