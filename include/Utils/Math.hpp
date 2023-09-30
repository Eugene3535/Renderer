#ifndef MATH_HPP
#define MATH_HPP

#include <emmintrin.h>

void MatrixMultiplySSE(const float* a, const float* b, float* c) 
{
//  Load rows of matrix A
    __m128 row1 = _mm_load_ps(&a[0]);
    __m128 row2 = _mm_load_ps(&a[4]);
    __m128 row3 = _mm_load_ps(&a[8]);
    __m128 row4 = _mm_load_ps(&a[12]);

// Perform matrix multiplication

    {// First column
//      Load row of matrix B
        __m128 col1 = _mm_set1_ps(b[0]);
        __m128 col2 = _mm_set1_ps(b[1]);
        __m128 col3 = _mm_set1_ps(b[2]);
        __m128 col4 = _mm_set1_ps(b[3]);

//      Multiply rows of A with columns of B
        __m128 m1 = _mm_mul_ps(row1, col1);
        __m128 m2 = _mm_mul_ps(row2, col2);
        __m128 m3 = _mm_mul_ps(row3, col3);
        __m128 m4 = _mm_mul_ps(row4, col4);

//      Add the results to get the final dot product
        __m128 sum1   = _mm_add_ps(m1, m2);
        __m128 sum2   = _mm_add_ps(m3, m4);
        __m128 result = _mm_add_ps(sum1, sum2);

//      Store the result in the output matrix
        _mm_store_ps(&c[0], result);
    }

    {// Second column
        __m128 col1 = _mm_set1_ps(b[4]);
        __m128 col2 = _mm_set1_ps(b[5]);
        __m128 col3 = _mm_set1_ps(b[6]);
        __m128 col4 = _mm_set1_ps(b[7]);

        __m128 m1 = _mm_mul_ps(row1, col1);
        __m128 m2 = _mm_mul_ps(row2, col2);
        __m128 m3 = _mm_mul_ps(row3, col3);
        __m128 m4 = _mm_mul_ps(row4, col4);

        __m128 sum1   = _mm_add_ps(m1, m2);
        __m128 sum2   = _mm_add_ps(m3, m4);
        __m128 result = _mm_add_ps(sum1, sum2);

        _mm_store_ps(&c[4], result);
    }

    {// Third column
        __m128 col1 = _mm_set1_ps(b[8]);
        __m128 col2 = _mm_set1_ps(b[9]);
        __m128 col3 = _mm_set1_ps(b[10]);
        __m128 col4 = _mm_set1_ps(b[11]);

        __m128 m1 = _mm_mul_ps(row1, col1);
        __m128 m2 = _mm_mul_ps(row2, col2);
        __m128 m3 = _mm_mul_ps(row3, col3);
        __m128 m4 = _mm_mul_ps(row4, col4);

        __m128 sum1   = _mm_add_ps(m1, m2);
        __m128 sum2   = _mm_add_ps(m3, m4);
        __m128 result = _mm_add_ps(sum1, sum2);

        _mm_store_ps(&c[8], result);
    }

    {// Forth column
        __m128 col1 = _mm_set1_ps(b[12]);
        __m128 col2 = _mm_set1_ps(b[13]);
        __m128 col3 = _mm_set1_ps(b[14]);
        __m128 col4 = _mm_set1_ps(b[15]);

        __m128 m1 = _mm_mul_ps(row1, col1);
        __m128 m2 = _mm_mul_ps(row2, col2);
        __m128 m3 = _mm_mul_ps(row3, col3);
        __m128 m4 = _mm_mul_ps(row4, col4);

        __m128 sum1   = _mm_add_ps(m1, m2);
        __m128 sum2   = _mm_add_ps(m3, m4);
        __m128 result = _mm_add_ps(sum1, sum2);

        _mm_store_ps(&c[12], result);
    }
}

#endif