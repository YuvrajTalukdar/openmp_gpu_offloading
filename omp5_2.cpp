//clang++ omp5_1.cpp -O2 -mavx512f
#include <immintrin.h>
#include <iostream>

void vector_add(float* a, float* b, float* c, int n) {
    int i;
    for (i = 0;!(n-i<8); i += 16) {
        __m512 va = _mm512_loadu_ps(&a[i]);
        __m512 vb = _mm512_loadu_ps(&b[i]);
        __m512 vc = _mm512_add_ps(va, vb);
        _mm512_storeu_ps(&c[i], vc);
    }
    // Handle any remaining elements
    for (; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    float a[16], b[16], c[16];
    for (int i = 0; i < 16; i++) {
        a[i] = 5;
        b[i] = 7;
    }
    vector_add(&a[0], &b[0], &c[0], 16);
    for (int i = 0; i < 16; i++) {
        std::cout << " " << c[i];
    }
    return 0;
}
