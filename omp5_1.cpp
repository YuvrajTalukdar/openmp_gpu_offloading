//clang++ omp5_1.cpp -O2 -mavx2
#include<immintrin.h>
#include<iostream>
#include<chrono>
#include<math.h>

using namespace std;
using namespace std::chrono;

#define N 100000

void vector_add_avx(float* a, float* b, float* c,long int n) 
{
    long int i = 0;
    for (;!(n-i<8); i += 8) 
    {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        __m256 vc = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(&c[i], vc);
    }

    // Handle any remaining elements
    for (; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void vector_add(float *a,float *b,float *c,long int n)
{
    for(long int i=0;i<n;i++)
    {
        c[i]=a[i]+b[i];
    }
}


int main()
{
    float a[N],b[N],c[N];
    for(long int i=0;i<N;i++)
    {
        a[i]=5;
        b[i]=7;        
    }
    auto start = high_resolution_clock::now();
    vector_add(&a[0],&b[0],&c[0],N);
    //vector_add_avx(&a[0],&b[0],&c[0],N);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout<<"\n\nTime Taken vector_add= "+to_string(duration.count()/pow(10,6))+"\n\n";
    
    for(int i=0;i<10;i++)
    {
        cout<<" "<<c[i];
    }
    return 0;
}