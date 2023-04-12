#include <immintrin.h>  // AVX instrintics

#include <cstdlib>
#include <iostream>

#include "matmul.h"

namespace matmul {
void dump_64x8_signed(__m256i &target, char *title) {
    int8_t *ptr = (int8_t *)&target;

    printf("%s:", title);
    for (int i = 0; i < 64; i++) {
        printf("%3d, ", *ptr++);
    }
    printf("\n");
}

void dump_64x8_unsigned(__m256i &target, char *title) {
    uint8_t *ptr = (uint8_t *)&target;

    printf("%s:", title);
    for (int i = 0; i < 64; i++) {
        printf("%3d, ", *ptr++);
    }
    printf("\n");
}

void dump_32x16_signed(__m256i &target, char *title) {
    int16_t *ptr = (int16_t *)&target;

    printf("%s:", title);
    for (int i = 0; i < 32; i++) {
        printf("%d, ", *ptr++);
    }
    printf("\n");
}

// element-wise multiply two vectors of 64 8-bit integers and return the accumulate 32-bit result
// We need to assume int8 is in the range of 127 <-> - 127, otherwise, we will expect overflow in some case
// e,g., a[i] = b[i] = -128 -> a[i] * b[i] = 32768 which is not in the range of int16_t(-32768, 32767)
__m256i zero_vec = _mm256_setzero_si256();
__m256i multiply_signed_int8(__m256i &a, __m256i &b, __m256i &a2, __m256i &b2) {
    __m256i a_sign_mask = _mm256_cmpgt_epi8(zero_vec, a);    // set 0xFF if zero_vec[i] > a[i]
    __m256i b_sign_mask = _mm256_cmpgt_epi8(zero_vec, b);    // set 0xFF if zero_vec[i] > a[i]
    __m256i a2_sign_mask = _mm256_cmpgt_epi8(zero_vec, a2);  // set 0xFF if zero_vec[i] > a[i]
    __m256i b2_sign_mask = _mm256_cmpgt_epi8(zero_vec, b2);  // set 0xFF if zero_vec[i] > a[i]

    // Compute the two's complement of a, put it here for higher throughput with good instruction dep.
    __m256i b_abs = _mm256_abs_epi8(b);
    __m256i b2_abs = _mm256_abs_epi8(b2);
    __m256i a_abs = _mm256_abs_epi8(a);
    __m256i a2_abs = _mm256_abs_epi8(a2);
    __m256i b_negated = _mm256_sub_epi8(_mm256_set1_epi8(0), b_abs);
    __m256i b2_negated = _mm256_sub_epi8(_mm256_set1_epi8(0), b2_abs);

    // Manipulate the `sign` of B to represent the sign of the 16 bit result
    __m256i sign_mask_a_sub_b = _mm256_sub_epi8(a_sign_mask, b_sign_mask);
    __m256i sign_mask_a2_sub_b2 = _mm256_sub_epi8(a2_sign_mask, b2_sign_mask);
    __m256i two_complement_mask = _mm256_cmpeq_epi8(
        sign_mask_a_sub_b, zero_vec);  // two_complement_mask[i] if a[i] and b[i] have different sign bits
    __m256i two_complement_mask2 = _mm256_cmpeq_epi8(sign_mask_a2_sub_b2, zero_vec);
    __m256i corrected_b = _mm256_blendv_epi8(b_negated, b_abs, two_complement_mask);
    __m256i corrected_b2 = _mm256_blendv_epi8(b2_negated, b2_abs, two_complement_mask2);

    // Multiply the absolute values of a_abs (unsigned 8-bit integers) and corrected_b (signed 8-bit integers)
    __m256i product_16x16 = _mm256_maddubs_epi16(a_abs, corrected_b);
    __m256i product_16x16_2 = _mm256_maddubs_epi16(a2_abs, corrected_b2);

    // Sign extend the 16-bit integers in vector to 32-bit integers
    __m256i a_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16, 0));
    __m256i b_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_2, 0));
    __m256i a_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16, 1));
    __m256i b_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_2, 1));

    // Element-wise add the 32-bit integer vectors
    __m256i sum1 = _mm256_add_epi32(a_ext1, b_ext1);
    __m256i sum2 = _mm256_add_epi32(a_ext2, b_ext2);

    __m256i sum_product_8x32 = _mm256_add_epi32(sum1, sum2);

    return sum_product_8x32;
}

void MatmulOperator::mat_mul_avx_int8(const struct matmul_params *params) {
    int i, j, k;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr, *data_C = C->int8_data_ptr;

    assert(A->column % 64 == 0);

    for (i = 0; i < C->row; i++)
        for (j = 0; j < C->column; j++) {
            int acc = 0;
            __m256i aa = _mm256_loadu_si256((const __m256i_u *)A), bb = _mm256_loadu_si256((const __m256i_u *)B);
            __m256i aa2 = _mm256_loadu_si256((const __m256i_u *)(&A[32])),
                    bb2 = _mm256_loadu_si256((const __m256i_u *)(&B[32]));

            __m256i acc8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 64) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]),
                        aa2 = _mm256_loadu_si256((const __m256i_u *)(&data_A[i * A->column + k + 32]));
                // assume B is transposed
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]),
                        bb2 = _mm256_loadu_si256((const __m256i_u *)(&data_B[j * B->row + k + 32]));

                acc8x32 = _mm256_add_epi32(acc8x32, multiply_signed_int8(aa, bb, aa2, bb2));
            }
            int32_t *accptr = (int32_t *)&acc8x32;
            acc = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc = (int32_t)((float)acc * effective_scale);
            acc -= C_zp;
            acc = MAX(acc, -128);
            acc = MIN(acc, 127);
            data_C[i * C->column + j] = (int8_t)acc;
        }
}

void *mat_mul_avx_int8_thread_func(void *args) {
    int i, j, k;
    struct thread_args *thread_args = (struct thread_args *)args;
    const struct matmul_params *params = thread_args->params;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr, *data_C = C->int8_data_ptr;
    int start_i = thread_args->start_i, end_i = thread_args->end_i;

    assert(A->column % 64 == 0);

    for (i = start_i; i < end_i; i++)
        for (j = 0; j < C->column; j++) {
            int acc = 0;
            __m256i aa = _mm256_loadu_si256((const __m256i_u *)A), bb = _mm256_loadu_si256((const __m256i_u *)B);
            __m256i aa2 = _mm256_loadu_si256((const __m256i_u *)(&A[32])),
                    bb2 = _mm256_loadu_si256((const __m256i_u *)(&B[32]));

            __m256i acc8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 64) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]),
                        aa2 = _mm256_loadu_si256((const __m256i_u *)(&data_A[i * A->column + k + 32]));
                // assume B is transposed
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]),
                        bb2 = _mm256_loadu_si256((const __m256i_u *)(&data_B[j * B->row + k + 32]));

                acc8x32 = _mm256_add_epi32(acc8x32, multiply_signed_int8(aa, bb, aa2, bb2));
            }
            int32_t *accptr = (int32_t *)&acc8x32;
            acc = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc = (int32_t)((float)acc * effective_scale);
            acc -= C_zp;
            acc = MAX(acc, -128);
            acc = MIN(acc, 127);
            data_C[i * C->column + j] = (int8_t)acc;
        }
    return NULL;
}

void MatmulOperator::mat_mul_avx_int8_fast(const struct matmul_params *params) {
    int j, num_thread = params->opt_params.num_thread;

    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr, *data_C = C->int8_data_ptr;

    assert(A->column % 64 == 0);

    pthread_t thread_pool[num_thread];
    struct thread_args threads_args[num_thread];

    // Thread creation
    for (j = 0; j < num_thread; j++) {
        threads_args[j].start_i = j * (C->row / num_thread);
        threads_args[j].end_i = (j + 1) * (C->row / num_thread);
        threads_args[j].blk_size = params->opt_params.blk_size;
        threads_args[j].params = params;
        pthread_create(&thread_pool[j], NULL, mat_mul_avx_int8_thread_func, &threads_args[j]);
    }
    // Join threads
    for (j = 0; j < num_thread; j++) {
        pthread_join(thread_pool[j], NULL);
    }
}

}  // namespace matmul

// void initialize_vector(int8_t A[], int size) {
//     for (int i = 0; i < size; i++) {
//         // A[i] = (rand() % 2) - 1;
//         A[i] = (rand() % 254) - 127;
//     }
// }

// int main(){
//     int8_t A[64], B[64];
//     initialize_vector(A, 64);
//     initialize_vector(B, 64);

//     int32_t ref_acc = 0, acc;
//     for (int i = 0; i < 64; i++){
//         ref_acc += A[i] * B[i];
//     }

//     __m256i aa = _mm256_loadu_si256((const __m256i_u *)A), bb = _mm256_loadu_si256((const __m256i_u *)B);
//     __m256i aa2 = _mm256_loadu_si256((const __m256i_u *)(&A[32])), bb2 = _mm256_loadu_si256((const __m256i_u
//     *)(&B[32]));

//     __m256i acc8x32 = multiply_signed_int8(aa, bb, aa2, bb2);
//     int32_t *accptr = (int32_t*)&acc8x32;
//     acc = accptr[0] + accptr[1] + accptr[2] + accptr[3]+ accptr[4] + accptr[5] + accptr[6] + accptr[7];

//     printf("%d, %d\n", acc, ref_acc);
//     assert(acc == ref_acc);

//     return 0;
// }
