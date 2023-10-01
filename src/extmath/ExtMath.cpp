#include <emmintrin.h>

#include "extmath/ExtMath.hpp"

void MatrixMultiplySSE(const float* left, const float* right, float* result) 
{
//  Load rows of matrix A
    __m128 row1 = _mm_load_ps(&left[0]);
    __m128 row2 = _mm_load_ps(&left[4]);
    __m128 row3 = _mm_load_ps(&left[8]);
    __m128 row4 = _mm_load_ps(&left[12]);

// Perform matrix multiplication

    {// First column
//      Load row of matrix B
        __m128 col1 = _mm_set1_ps(right[0]);
        __m128 col2 = _mm_set1_ps(right[1]);
        __m128 col3 = _mm_set1_ps(right[2]);
        __m128 col4 = _mm_set1_ps(right[3]);

//      Multiply rows of A with columns of B
        __m128 m1 = _mm_mul_ps(row1, col1);
        __m128 m2 = _mm_mul_ps(row2, col2);
        __m128 m3 = _mm_mul_ps(row3, col3);
        __m128 m4 = _mm_mul_ps(row4, col4);

//      Add the results to get the final dot product
        __m128 sum1 = _mm_add_ps(m1, m2);
        __m128 sum2 = _mm_add_ps(m3, m4);
        __m128 res  = _mm_add_ps(sum1, sum2);

//      Store the result in the output matrix
        _mm_store_ps(&result[0], res);
    }

    {// Second column
        __m128 col1 = _mm_set1_ps(right[4]);
        __m128 col2 = _mm_set1_ps(right[5]);
        __m128 col3 = _mm_set1_ps(right[6]);
        __m128 col4 = _mm_set1_ps(right[7]);

        __m128 m1 = _mm_mul_ps(row1, col1);
        __m128 m2 = _mm_mul_ps(row2, col2);
        __m128 m3 = _mm_mul_ps(row3, col3);
        __m128 m4 = _mm_mul_ps(row4, col4);

        __m128 sum1 = _mm_add_ps(m1, m2);
        __m128 sum2 = _mm_add_ps(m3, m4);
        __m128 res  = _mm_add_ps(sum1, sum2);

        _mm_store_ps(&result[4], res);
    }

    {// Third column
        __m128 col1 = _mm_set1_ps(right[8]);
        __m128 col2 = _mm_set1_ps(right[9]);
        __m128 col3 = _mm_set1_ps(right[10]);
        __m128 col4 = _mm_set1_ps(right[11]);

        __m128 m1 = _mm_mul_ps(row1, col1);
        __m128 m2 = _mm_mul_ps(row2, col2);
        __m128 m3 = _mm_mul_ps(row3, col3);
        __m128 m4 = _mm_mul_ps(row4, col4);

        __m128 sum1 = _mm_add_ps(m1, m2);
        __m128 sum2 = _mm_add_ps(m3, m4);
        __m128 res  = _mm_add_ps(sum1, sum2);

        _mm_store_ps(&result[8], res);
    }

    {// Forth column
        __m128 col1 = _mm_set1_ps(right[12]);
        __m128 col2 = _mm_set1_ps(right[13]);
        __m128 col3 = _mm_set1_ps(right[14]);
        __m128 col4 = _mm_set1_ps(right[15]);

        __m128 m1 = _mm_mul_ps(row1, col1);
        __m128 m2 = _mm_mul_ps(row2, col2);
        __m128 m3 = _mm_mul_ps(row3, col3);
        __m128 m4 = _mm_mul_ps(row4, col4);

        __m128 sum1 = _mm_add_ps(m1, m2);
        __m128 sum2 = _mm_add_ps(m3, m4);
        __m128 res  = _mm_add_ps(sum1, sum2);

        _mm_store_ps(&result[12], res);
    }
}