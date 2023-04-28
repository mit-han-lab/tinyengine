#include <immintrin.h>  // AVX instrintics

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "matmul.h"
// #include <omp.h> // currently it is bugged

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
    __m256i sign_mask =
        _mm256_cmpeq_epi8(sign_mask_a_sub_b, zero_vec);  // sign_mask[i] if a[i] and b[i] have different sign bits
    __m256i sign_mask2 = _mm256_cmpeq_epi8(sign_mask_a2_sub_b2, zero_vec);
    __m256i corrected_b = _mm256_blendv_epi8(b_negated, b_abs, sign_mask);
    __m256i corrected_b2 = _mm256_blendv_epi8(b2_negated, b2_abs, sign_mask2);

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

// ([a, a2], [c, c2])  *  ([b, b2], [d, d2])
//  acc0 = a * b + a2 * b2, acc2 = a * d + a2 * d2, acc3 = c * b + c * b2, acc4 = c * d + c2 * d2
void multiply_signed_int8_2x2(__m256i &a, __m256i &b, __m256i &a2, __m256i &b2, __m256i &c, __m256i &c2, __m256i &d,
                              __m256i &d2, __m256i &acc0, __m256i &acc1, __m256i &acc2, __m256i &acc3) {
    __m256i a_sign_mask = _mm256_cmpgt_epi8(zero_vec, a);  // set 0xFF if zero_vec[i] > a[i]
    __m256i b_sign_mask = _mm256_cmpgt_epi8(zero_vec, b);  // set 0xFF if zero_vec[i] > a[i]
    __m256i a2_sign_mask = _mm256_cmpgt_epi8(zero_vec, a2);
    __m256i b2_sign_mask = _mm256_cmpgt_epi8(zero_vec, b2);
    __m256i c_sign_mask = _mm256_cmpgt_epi8(zero_vec, c);
    __m256i d_sign_mask = _mm256_cmpgt_epi8(zero_vec, d);
    __m256i c2_sign_mask = _mm256_cmpgt_epi8(zero_vec, c2);
    __m256i d2_sign_mask = _mm256_cmpgt_epi8(zero_vec, d2);

    // Compute the two's complement of a, put it here for higher throughput with good instruction dep.
    __m256i b_abs = _mm256_abs_epi8(b);
    __m256i b2_abs = _mm256_abs_epi8(b2);
    __m256i a_abs = _mm256_abs_epi8(a);
    __m256i a2_abs = _mm256_abs_epi8(a2);
    __m256i d_abs = _mm256_abs_epi8(d);
    __m256i d2_abs = _mm256_abs_epi8(d2);
    __m256i c_abs = _mm256_abs_epi8(c);
    __m256i c2_abs = _mm256_abs_epi8(c2);
    __m256i b_negated = _mm256_sub_epi8(_mm256_set1_epi8(0), b_abs);
    __m256i b2_negated = _mm256_sub_epi8(_mm256_set1_epi8(0), b2_abs);
    __m256i d_negated = _mm256_sub_epi8(_mm256_set1_epi8(0), d_abs);
    __m256i d2_negated = _mm256_sub_epi8(_mm256_set1_epi8(0), d2_abs);

    // Manipulate the `sign` of B to represent the sign of the 16 bit result
    __m256i sign_mask_a_sub_b = _mm256_sub_epi8(a_sign_mask, b_sign_mask);
    __m256i sign_mask_a_sub_d = _mm256_sub_epi8(a_sign_mask, d_sign_mask);
    __m256i sign_mask_a2_sub_b2 = _mm256_sub_epi8(a2_sign_mask, b2_sign_mask);
    __m256i sign_mask_a2_sub_d2 = _mm256_sub_epi8(a2_sign_mask, d2_sign_mask);
    __m256i sign_mask_c_sub_b = _mm256_sub_epi8(c_sign_mask, b_sign_mask);
    __m256i sign_mask_c_sub_d = _mm256_sub_epi8(c_sign_mask, d_sign_mask);
    __m256i sign_mask_c2_sub_b2 = _mm256_sub_epi8(c2_sign_mask, b2_sign_mask);
    __m256i sign_mask_c2_sub_d2 = _mm256_sub_epi8(c2_sign_mask, d2_sign_mask);

    // sign_mask[i] if a[i] and b[i] have different sign bits
    __m256i sign_mask_ab = _mm256_cmpeq_epi8(sign_mask_a_sub_b, zero_vec);
    __m256i sign_mask2_a2_b2 = _mm256_cmpeq_epi8(sign_mask_a2_sub_b2, zero_vec);
    __m256i sign_mask_ad = _mm256_cmpeq_epi8(sign_mask_a_sub_d, zero_vec);
    __m256i sign_mask2_a2_d2 = _mm256_cmpeq_epi8(sign_mask_a2_sub_d2, zero_vec);
    __m256i sign_mask_cb = _mm256_cmpeq_epi8(sign_mask_c_sub_b, zero_vec);
    __m256i sign_mask2_c2_b2 = _mm256_cmpeq_epi8(sign_mask_c2_sub_b2, zero_vec);
    __m256i sign_mask_cd = _mm256_cmpeq_epi8(sign_mask_c_sub_d, zero_vec);
    __m256i sign_mask2_c2_d2 = _mm256_cmpeq_epi8(sign_mask_c2_sub_d2, zero_vec);

    __m256i corrected_ab = _mm256_blendv_epi8(b_negated, b_abs, sign_mask_ab);
    __m256i corrected_a2b2 = _mm256_blendv_epi8(b2_negated, b2_abs, sign_mask2_a2_b2);
    __m256i corrected_ad = _mm256_blendv_epi8(d_negated, d_abs, sign_mask_ad);
    __m256i corrected_a2d2 = _mm256_blendv_epi8(d2_negated, d2_abs, sign_mask2_a2_d2);
    __m256i corrected_cb = _mm256_blendv_epi8(b_negated, b_abs, sign_mask_cb);
    __m256i corrected_c2b2 = _mm256_blendv_epi8(b2_negated, b2_abs, sign_mask2_c2_b2);
    __m256i corrected_cd = _mm256_blendv_epi8(d_negated, d_abs, sign_mask_cd);
    __m256i corrected_c2d2 = _mm256_blendv_epi8(d2_negated, d2_abs, sign_mask2_c2_d2);

    // Multiply the absolute values of a_abs (unsigned 8-bit integers) and corrected_b (signed 8-bit integers)
    __m256i product_16x16_ab = _mm256_maddubs_epi16(a_abs, corrected_ab);
    __m256i product_16x16_ab2 = _mm256_maddubs_epi16(a2_abs, corrected_a2b2);
    __m256i product_16x16_ad = _mm256_maddubs_epi16(a_abs, corrected_ad);
    __m256i product_16x16_ad2 = _mm256_maddubs_epi16(a2_abs, corrected_a2d2);
    __m256i product_16x16_cb = _mm256_maddubs_epi16(c_abs, corrected_cb);
    __m256i product_16x16_cb2 = _mm256_maddubs_epi16(c2_abs, corrected_c2b2);
    __m256i product_16x16_cd = _mm256_maddubs_epi16(c_abs, corrected_cd);
    __m256i product_16x16_cd2 = _mm256_maddubs_epi16(c2_abs, corrected_c2d2);

    // Sign extend the 16-bit integers in vector to 32-bit integers
    __m256i ab_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ab, 0));
    __m256i ab2_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ab2, 0));
    __m256i ab_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ab, 1));
    __m256i ab2_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ab2, 1));
    __m256i ad_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ad, 0));
    __m256i ad2_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ad2, 0));
    __m256i ad_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ad, 1));
    __m256i ad2_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ad2, 1));
    __m256i cb_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cb, 0));
    __m256i cb2_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cb2, 0));
    __m256i cb_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cb, 1));
    __m256i cb2_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cb2, 1));
    __m256i cd_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cd, 0));
    __m256i cd2_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cd2, 0));
    __m256i cd_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cd, 1));
    __m256i cd2_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cd2, 1));

    // Element-wise add the 32-bit integer vectors
    // acc0 += a * b + a2 * b2, acc2 += a * d + a2 * d2, acc3 += c * b + c * b2, acc4 += c * d + c2 * d2
    acc0 = _mm256_add_epi32(acc0,
                            _mm256_add_epi32(_mm256_add_epi32(ab_ext1, ab2_ext1), _mm256_add_epi32(ab_ext2, ab2_ext2)));
    acc1 = _mm256_add_epi32(acc1,
                            _mm256_add_epi32(_mm256_add_epi32(ad_ext1, ad2_ext1), _mm256_add_epi32(ad_ext2, ad2_ext2)));
    acc2 = _mm256_add_epi32(acc2,
                            _mm256_add_epi32(_mm256_add_epi32(cb_ext1, cb2_ext1), _mm256_add_epi32(cb_ext2, cb2_ext2)));
    acc3 = _mm256_add_epi32(acc3,
                            _mm256_add_epi32(_mm256_add_epi32(cd_ext1, cd2_ext1), _mm256_add_epi32(cd_ext2, cd2_ext2)));
}

static inline void multiply_signed_int8_2x2_32epi(__m256i &a, __m256i &b, __m256i &c, __m256i &d, __m256i &acc0,
                                                  __m256i &acc1, __m256i &acc2, __m256i &acc3) {
    __m256i a_sign_mask = _mm256_cmpgt_epi8(zero_vec, a);  // set 0xFF if zero_vec[i] > a[i]
    __m256i b_sign_mask = _mm256_cmpgt_epi8(zero_vec, b);  // set 0xFF if zero_vec[i] > a[i]
    __m256i c_sign_mask = _mm256_cmpgt_epi8(zero_vec, c);
    __m256i d_sign_mask = _mm256_cmpgt_epi8(zero_vec, d);

    // Compute the two's complement of a, put it here for higher throughput with good instruction dep.
    __m256i b_abs = _mm256_abs_epi8(b);
    __m256i a_abs = _mm256_abs_epi8(a);
    __m256i d_abs = _mm256_abs_epi8(d);
    __m256i c_abs = _mm256_abs_epi8(c);
    __m256i b_negated = _mm256_sub_epi8(_mm256_set1_epi8(0), b_abs);
    __m256i d_negated = _mm256_sub_epi8(_mm256_set1_epi8(0), d_abs);

    // Manipulate the `sign` of B to represent the sign of the 16 bit result
    __m256i sign_mask_a_sub_b = _mm256_sub_epi8(a_sign_mask, b_sign_mask);
    __m256i sign_mask_a_sub_d = _mm256_sub_epi8(a_sign_mask, d_sign_mask);
    __m256i sign_mask_c_sub_b = _mm256_sub_epi8(c_sign_mask, b_sign_mask);
    __m256i sign_mask_c_sub_d = _mm256_sub_epi8(c_sign_mask, d_sign_mask);

    // sign_mask[i] if a[i] and b[i] have different sign bits
    __m256i sign_mask_ab = _mm256_cmpeq_epi8(sign_mask_a_sub_b, zero_vec);
    __m256i sign_mask_ad = _mm256_cmpeq_epi8(sign_mask_a_sub_d, zero_vec);
    __m256i sign_mask_cb = _mm256_cmpeq_epi8(sign_mask_c_sub_b, zero_vec);
    __m256i sign_mask_cd = _mm256_cmpeq_epi8(sign_mask_c_sub_d, zero_vec);

    __m256i corrected_ab = _mm256_blendv_epi8(b_negated, b_abs, sign_mask_ab);
    __m256i corrected_ad = _mm256_blendv_epi8(d_negated, d_abs, sign_mask_ad);
    __m256i corrected_cb = _mm256_blendv_epi8(b_negated, b_abs, sign_mask_cb);
    __m256i corrected_cd = _mm256_blendv_epi8(d_negated, d_abs, sign_mask_cd);

    // Multiply the absolute values of a_abs (unsigned 8-bit integers) and corrected_b (signed 8-bit integers)
    __m256i product_16x16_ab = _mm256_maddubs_epi16(a_abs, corrected_ab);
    __m256i product_16x16_ad = _mm256_maddubs_epi16(a_abs, corrected_ad);
    __m256i product_16x16_cb = _mm256_maddubs_epi16(c_abs, corrected_cb);
    __m256i product_16x16_cd = _mm256_maddubs_epi16(c_abs, corrected_cd);

    // Sign extend the 16-bit integers in vector to 32-bit integers
    __m256i ab_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ab, 0));
    __m256i ab_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ab, 1));
    __m256i ad_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ad, 0));
    __m256i ad_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ad, 1));
    __m256i cb_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cb, 0));
    __m256i cb_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cb, 1));
    __m256i cd_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cd, 0));
    __m256i cd_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cd, 1));

    // Element-wise add the 32-bit integer vectors
    // acc0 += a * b + a2 * b2, acc2 += a * d + a2 * d2, acc3 += c * b + c * b2, acc4 += c * d + c2 * d2
    acc0 = _mm256_add_epi32(acc0, _mm256_add_epi32(ab_ext1, ab_ext2));
    acc1 = _mm256_add_epi32(acc1, _mm256_add_epi32(ad_ext1, ad_ext2));
    acc2 = _mm256_add_epi32(acc2, _mm256_add_epi32(cb_ext1, cb_ext2));
    acc3 = _mm256_add_epi32(acc3, _mm256_add_epi32(cd_ext1, cd_ext2));
}

static inline void multiply_signed_int8_2x2_32epi_of(__m256i &a, __m256i &b, __m256i &c, __m256i &d, __m256i &acc0,
                                                     __m256i &acc1, __m256i &acc2, __m256i &acc3) {
    // Multiply the absolute values of a_abs (unsigned 8-bit integers) and corrected_b (signed 8-bit integers)
    __m256i product_16x16_ab = _mm256_maddubs_epi16(a, b);
    __m256i product_16x16_ad = _mm256_maddubs_epi16(a, d);
    __m256i product_16x16_cb = _mm256_maddubs_epi16(c, b);
    __m256i product_16x16_cd = _mm256_maddubs_epi16(c, d);

    // Sign extend the 16-bit integers in vector to 32-bit integers
    __m256i ab_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ab, 0));
    __m256i ab_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ab, 1));
    __m256i ad_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ad, 0));
    __m256i ad_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_ad, 1));
    __m256i cb_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cb, 0));
    __m256i cb_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cb, 1));
    __m256i cd_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cd, 0));
    __m256i cd_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_cd, 1));

    // Element-wise add the 32-bit integer vectors
    // acc0 += a * b + a2 * b2, acc2 += a * d + a2 * d2, acc3 += c * b + c * b2, acc4 += c * d + c2 * d2
    acc0 = _mm256_add_epi32(acc0, _mm256_add_epi32(ab_ext1, ab_ext2));
    acc1 = _mm256_add_epi32(acc1, _mm256_add_epi32(ad_ext1, ad_ext2));
    acc2 = _mm256_add_epi32(acc2, _mm256_add_epi32(cb_ext1, cb_ext2));
    acc3 = _mm256_add_epi32(acc3, _mm256_add_epi32(cd_ext1, cd_ext2));
}
// Note: This implementation could have potential overflow!
// __m256i multiply_signed_int8(__m256i &a, __m256i &b, __m256i &a2, __m256i &b2) {
//     // Multiply the absolute values of a_abs (unsigned 8-bit integers) and corrected_b (signed 8-bit integers)
//     __m256i product_16x16 = _mm256_maddubs_epi16(a, b);
//     __m256i product_16x16_2 = _mm256_maddubs_epi16(a2, b2);

//     // Sign extend the 16-bit integers in vector to 32-bit integers
//     __m256i a_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16, 0));
//     __m256i b_ext1 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_2, 0));
//     __m256i a_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16, 1));
//     __m256i b_ext2 = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(product_16x16_2, 1));

//     // Element-wise add the 32-bit integer vectors
//     __m256i sum1 = _mm256_add_epi32(a_ext1, b_ext1);
//     __m256i sum2 = _mm256_add_epi32(a_ext2, b_ext2);

//     __m256i sum_product_8x32 = _mm256_add_epi32(sum1, sum2);

//     return sum_product_8x32;
// }

void MatmulOperator::mat_mul_avx_int8(const struct matmul_params *params) {
    int i, j, k;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr, *data_C = C->int8_data_ptr;
    const int8_t q_min = C->qparams.q_min, q_max = C->qparams.q_max;

    assert(A->column % 64 == 0);

    for (i = 0; i < C->row; i++)
        for (j = 0; j < C->column; j++) {
            int acc = 0;
            __m256i acc0_8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 64) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]),
                        aa2 = _mm256_loadu_si256((const __m256i_u *)(&data_A[i * A->column + k + 32]));
                // assume B is transposed
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]),
                        bb2 = _mm256_loadu_si256((const __m256i_u *)(&data_B[j * B->row + k + 32]));

                acc0_8x32 = _mm256_add_epi32(acc0_8x32, multiply_signed_int8(aa, bb, aa2, bb2));
            }
            int32_t *accptr = (int32_t *)&acc0_8x32;
            acc = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc = (int32_t)((float)acc * effective_scale);
            acc -= C_zp;
            acc = MAX(acc, q_min);
            acc = MIN(acc, q_max);
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
    const int8_t q_min = C->qparams.q_min, q_max = C->qparams.q_max;

    for (i = start_i; i < end_i; i++)
        for (j = 0; j < C->column; j++) {
            int acc = 0;
            __m256i acc0_8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 64) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]),
                        aa2 = _mm256_loadu_si256((const __m256i_u *)(&data_A[i * A->column + k + 32]));
                // assume B is transposed
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]),
                        bb2 = _mm256_loadu_si256((const __m256i_u *)(&data_B[j * B->row + k + 32]));

                acc0_8x32 = _mm256_add_epi32(acc0_8x32, multiply_signed_int8(aa, bb, aa2, bb2));
            }
            int32_t *accptr = (int32_t *)&acc0_8x32;
            acc = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc = (int32_t)std::round((float)acc * effective_scale);
            acc -= C_zp;
            acc = MAX(acc, q_min);
            acc = MIN(acc, q_max);
            data_C[i * C->column + j] = (int8_t)acc;
        }
    return NULL;
}

void MatmulOperator::mat_mul_avx_int8_fast(const struct matmul_params *params) {
    int j, num_thread = params->opt_params.num_thread;

    assert(params->A.column % 64 == 0);

    pthread_t thread_pool[num_thread];
    struct thread_args threads_args[num_thread];

    // Thread creation
    for (j = 0; j < num_thread; j++) {
        threads_args[j].start_i = j * (params->C.row / num_thread);
        threads_args[j].end_i = (j + 1) * (params->C.row / num_thread);
        threads_args[j].blk_size = params->opt_params.blk_size;
        threads_args[j].params = params;
        pthread_create(&thread_pool[j], NULL, mat_mul_avx_int8_thread_func, &threads_args[j]);
    }
    // Join threads
    for (j = 0; j < num_thread; j++) {
        pthread_join(thread_pool[j], NULL);
    }
}

void *mat_mul_avx_int8_thread_func_2x2(void *args) {
    int i, j, k;
    struct thread_args *thread_args = (struct thread_args *)args;
    const struct matmul_params *params = thread_args->params;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr, *data_C = C->int8_data_ptr;
    int start_i = thread_args->start_i, end_i = thread_args->end_i;
    const int8_t q_min = C->qparams.q_min, q_max = C->qparams.q_max;

    assert((end_i - start_i) % 2 == 0);

    for (i = start_i; i < end_i; i += 2)

        for (j = 0; j < C->column; j += 2) {
            // (i, j), (i, j+1), (i+1, j), (i+1, j+1)
            int acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
            __m256i acc0_8x32 = _mm256_setzero_si256(), acc1_8x32 = _mm256_setzero_si256(),
                    acc2_8x32 = _mm256_setzero_si256(), acc3_8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 64) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]),
                        aa2 = _mm256_loadu_si256((const __m256i_u *)(&data_A[i * A->column + k + 32]));
                __m256i cc = _mm256_loadu_si256((const __m256i_u *)&data_A[(i + 1) * A->column + k]),
                        cc2 = _mm256_loadu_si256((const __m256i_u *)(&data_A[(i + 1) * A->column + k + 32]));
                // assume B is transposed
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]),
                        bb2 = _mm256_loadu_si256((const __m256i_u *)(&data_B[j * B->row + k + 32]));
                __m256i dd = _mm256_loadu_si256((const __m256i_u *)&data_B[(j + 1) * B->row + k]),
                        dd2 = _mm256_loadu_si256((const __m256i_u *)(&data_B[(j + 1) * B->row + k + 32]));

                multiply_signed_int8_2x2(aa, bb, aa2, bb2, cc, cc2, dd, dd2, acc0_8x32, acc1_8x32, acc2_8x32,
                                         acc3_8x32);
            }
            int32_t *accptr = (int32_t *)&acc0_8x32;
            acc0 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7] +
                   params->bias.int32_data_ptr[j];
            accptr = (int32_t *)&acc1_8x32;
            acc1 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7] +
                   params->bias.int32_data_ptr[j + 1];
            accptr = (int32_t *)&acc2_8x32;
            acc2 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7] +
                   params->bias.int32_data_ptr[j];
            accptr = (int32_t *)&acc3_8x32;
            acc3 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7] +
                   params->bias.int32_data_ptr[j + 1];

            acc0 = (int32_t)std::round((float)acc0 * effective_scale);
            acc1 = (int32_t)std::round((float)acc1 * effective_scale);
            acc2 = (int32_t)std::round((float)acc2 * effective_scale);
            acc3 = (int32_t)std::round((float)acc3 * effective_scale);

            acc0 -= C_zp;
            acc1 -= C_zp;
            acc2 -= C_zp;
            acc3 -= C_zp;

            acc0 = MAX(acc0, q_min);
            acc1 = MAX(acc1, q_min);
            acc2 = MAX(acc2, q_min);
            acc3 = MAX(acc3, q_min);
            acc0 = MIN(acc0, q_max);
            acc1 = MIN(acc1, q_max);
            acc2 = MIN(acc2, q_max);
            acc3 = MIN(acc3, q_max);
            data_C[i * C->column + j] = (int8_t)acc0;
            data_C[i * C->column + j + 1] = (int8_t)acc1;
            data_C[(i + 1) * C->column + j] = (int8_t)acc2;
            data_C[(i + 1) * C->column + j + 1] = (int8_t)acc3;
        }
    return NULL;
}

void *mat_mul_avx_int8_thread_func_2x2_32unroll(void *args) {
    int i, j, k;
    struct thread_args *thread_args = (struct thread_args *)args;
    const struct matmul_params *params = thread_args->params;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr, *data_C = C->int8_data_ptr;
    int start_i = thread_args->start_i, end_i = thread_args->end_i;
    const int8_t q_min = C->qparams.q_min, q_max = C->qparams.q_max;

    assert((end_i - start_i) % 2 == 0);

    for (i = start_i; i < end_i; i += 2)

        for (j = 0; j < C->column; j += 2) {
            // (i, j), (i, j+1), (i+1, j), (i+1, j+1)
            int acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
            __m256i acc0_8x32 = _mm256_setzero_si256(), acc1_8x32 = _mm256_setzero_si256(),
                    acc2_8x32 = _mm256_setzero_si256(), acc3_8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 32) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]);
                __m256i cc = _mm256_loadu_si256((const __m256i_u *)&data_A[(i + 1) * A->column + k]);
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]);
                __m256i dd = _mm256_loadu_si256((const __m256i_u *)&data_B[(j + 1) * B->row + k]);

                // multiply_signed_int8_2x2_32epi_of(aa, bb, cc, dd, acc0_8x32, acc1_8x32, acc2_8x32, acc3_8x32);
                multiply_signed_int8_2x2_32epi(aa, bb, cc, dd, acc0_8x32, acc1_8x32, acc2_8x32, acc3_8x32);
            }
            int32_t *accptr = (int32_t *)&acc0_8x32;
            acc0 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc0 += params->bias.int32_data_ptr[j];
            accptr = (int32_t *)&acc1_8x32;
            acc1 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc1 += params->bias.int32_data_ptr[j + 1];
            accptr = (int32_t *)&acc2_8x32;
            acc2 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc2 += params->bias.int32_data_ptr[j];
            accptr = (int32_t *)&acc3_8x32;
            acc3 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc3 += params->bias.int32_data_ptr[j + 1];

            acc0 = (int32_t)std::round((float)acc0 * effective_scale);
            acc1 = (int32_t)std::round((float)acc1 * effective_scale);
            acc2 = (int32_t)std::round((float)acc2 * effective_scale);
            acc3 = (int32_t)std::round((float)acc3 * effective_scale);

            acc0 -= C_zp;
            acc1 -= C_zp;
            acc2 -= C_zp;
            acc3 -= C_zp;

            acc0 = MAX(acc0, q_min);
            acc1 = MAX(acc1, q_min);
            acc2 = MAX(acc2, q_min);
            acc3 = MAX(acc3, q_min);
            acc0 = MIN(acc0, q_max);
            acc1 = MIN(acc1, q_max);
            acc2 = MIN(acc2, q_max);
            acc3 = MIN(acc3, q_max);
            data_C[i * C->column + j] = (int8_t)acc0;
            data_C[i * C->column + j + 1] = (int8_t)acc1;
            data_C[(i + 1) * C->column + j] = (int8_t)acc2;
            data_C[(i + 1) * C->column + j + 1] = (int8_t)acc3;
        }
    return NULL;
}

void MatmulOperator::mat_mul_avx_int8_fast_2x2_32unroll(const struct matmul_params *params) {
    int j, num_thread = params->opt_params.num_thread;

    assert(params->A.column % 64 == 0);
    assert((params->C.column) % 2 == 0);

    pthread_t thread_pool[num_thread];
    struct thread_args threads_args[num_thread];

    // Thread creation
    for (j = 0; j < num_thread; j++) {
        threads_args[j].start_i = j * (params->C.row / num_thread);
        threads_args[j].end_i = (j + 1) * (params->C.row / num_thread);
        threads_args[j].blk_size = params->opt_params.blk_size;
        threads_args[j].params = params;
        pthread_create(&thread_pool[j], NULL, mat_mul_avx_int8_thread_func_2x2_32unroll, &threads_args[j]);
    }
    // Join threads
    for (j = 0; j < num_thread; j++) {
        pthread_join(thread_pool[j], NULL);
    }
}

void *mat_mul_avx_int8_thread_func_2x2_32unroll_nobias(void *args) {
    int i, j, k;
    struct thread_args *thread_args = (struct thread_args *)args;
    const struct matmul_params *params = thread_args->params;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr, *data_C = C->int8_data_ptr;
    int start_i = thread_args->start_i, end_i = thread_args->end_i;
    const int8_t q_min = C->qparams.q_min, q_max = C->qparams.q_max;

    assert((end_i - start_i) % 2 == 0);

    for (i = start_i; i < end_i; i += 2)

        for (j = 0; j < C->column; j += 2) {
            // (i, j), (i, j+1), (i+1, j), (i+1, j+1)
            int acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
            __m256i acc0_8x32 = _mm256_setzero_si256(), acc1_8x32 = _mm256_setzero_si256(),
                    acc2_8x32 = _mm256_setzero_si256(), acc3_8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 32) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]);
                __m256i cc = _mm256_loadu_si256((const __m256i_u *)&data_A[(i + 1) * A->column + k]);
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]);
                __m256i dd = _mm256_loadu_si256((const __m256i_u *)&data_B[(j + 1) * B->row + k]);

                // multiply_signed_int8_2x2_32epi_of(aa, bb, cc, dd, acc0_8x32, acc1_8x32, acc2_8x32, acc3_8x32);
                multiply_signed_int8_2x2_32epi(aa, bb, cc, dd, acc0_8x32, acc1_8x32, acc2_8x32, acc3_8x32);
            }
            int32_t *accptr = (int32_t *)&acc0_8x32;
            acc0 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            accptr = (int32_t *)&acc1_8x32;
            acc1 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            accptr = (int32_t *)&acc2_8x32;
            acc2 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            accptr = (int32_t *)&acc3_8x32;
            acc3 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];

            acc0 = (int32_t)std::round((float)acc0 * effective_scale);
            acc1 = (int32_t)std::round((float)acc1 * effective_scale);
            acc2 = (int32_t)std::round((float)acc2 * effective_scale);
            acc3 = (int32_t)std::round((float)acc3 * effective_scale);

            acc0 -= C_zp;
            acc1 -= C_zp;
            acc2 -= C_zp;
            acc3 -= C_zp;

            acc0 = MAX(acc0, q_min);
            acc1 = MAX(acc1, q_min);
            acc2 = MAX(acc2, q_min);
            acc3 = MAX(acc3, q_min);
            acc0 = MIN(acc0, q_max);
            acc1 = MIN(acc1, q_max);
            acc2 = MIN(acc2, q_max);
            acc3 = MIN(acc3, q_max);
            data_C[i * C->column + j] = (int8_t)acc0;
            data_C[i * C->column + j + 1] = (int8_t)acc1;
            data_C[(i + 1) * C->column + j] = (int8_t)acc2;
            data_C[(i + 1) * C->column + j + 1] = (int8_t)acc3;
        }
    return NULL;
}

void MatmulOperator::mat_mul_avx_int8_fast_2x2_32unroll_nobias(const struct matmul_params *params) {
    int j, num_thread = params->opt_params.num_thread;

    assert(params->A.column % 64 == 0);
    assert((params->C.column) % 2 == 0);

    pthread_t thread_pool[num_thread];
    struct thread_args threads_args[num_thread];

    // Thread creation
    for (j = 0; j < num_thread; j++) {
        threads_args[j].start_i = j * (params->C.row / num_thread);
        threads_args[j].end_i = (j + 1) * (params->C.row / num_thread);
        threads_args[j].blk_size = params->opt_params.blk_size;
        threads_args[j].params = params;
        pthread_create(&thread_pool[j], NULL, mat_mul_avx_int8_thread_func_2x2_32unroll_nobias, &threads_args[j]);
    }
    // Join threads
    for (j = 0; j < num_thread; j++) {
        pthread_join(thread_pool[j], NULL);
    }
}

void *mat_mul_avx_int8_thread_func_2x2_32unroll_nobias_ofp32(void *args) {
    int i, j, k;
    struct thread_args *thread_args = (struct thread_args *)args;
    const struct matmul_params *params = thread_args->params;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr;
    float *data_C = C->data_ptr;
    int start_i = thread_args->start_i, end_i = thread_args->end_i;
    const int8_t q_min = C->qparams.q_min, q_max = C->qparams.q_max;

    assert((end_i - start_i) % 2 == 0);

    for (i = start_i; i < end_i; i += 2)

        for (j = 0; j < C->column; j += 2) {
            // (i, j), (i, j+1), (i+1, j), (i+1, j+1)
            int acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
            __m256i acc0_8x32 = _mm256_setzero_si256(), acc1_8x32 = _mm256_setzero_si256(),
                    acc2_8x32 = _mm256_setzero_si256(), acc3_8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 32) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]);
                __m256i cc = _mm256_loadu_si256((const __m256i_u *)&data_A[(i + 1) * A->column + k]);
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]);
                __m256i dd = _mm256_loadu_si256((const __m256i_u *)&data_B[(j + 1) * B->row + k]);

                multiply_signed_int8_2x2_32epi(aa, bb, cc, dd, acc0_8x32, acc1_8x32, acc2_8x32, acc3_8x32);
            }
            int32_t *accptr = (int32_t *)&acc0_8x32;
            acc0 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            accptr = (int32_t *)&acc1_8x32;
            acc1 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            accptr = (int32_t *)&acc2_8x32;
            acc2 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            accptr = (int32_t *)&acc3_8x32;
            acc3 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];

            data_C[i * C->column + j] = ((float)acc0 * effective_scale);
            data_C[i * C->column + j + 1] = ((float)acc1 * effective_scale);
            data_C[(i + 1) * C->column + j] = ((float)acc2 * effective_scale);
            data_C[(i + 1) * C->column + j + 1] = ((float)acc3 * effective_scale);
        }
    return NULL;
}

void MatmulOperator::mat_mul_avx_int8_fast_2x2_32unroll_nobias_ofp32(const struct matmul_params *params) {
    int j, num_thread = params->opt_params.num_thread;

    assert(params->A.column % 64 == 0);
    assert((params->C.column) % 2 == 0);

    pthread_t thread_pool[num_thread];
    struct thread_args threads_args[num_thread];

    // Thread creation
    for (j = 0; j < num_thread; j++) {
        threads_args[j].start_i = j * (params->C.row / num_thread);
        threads_args[j].end_i = (j + 1) * (params->C.row / num_thread);
        threads_args[j].blk_size = params->opt_params.blk_size;
        threads_args[j].params = params;
        pthread_create(&thread_pool[j], NULL, mat_mul_avx_int8_thread_func_2x2_32unroll_nobias_ofp32, &threads_args[j]);
    }
    // Join threads
    for (j = 0; j < num_thread; j++) {
        pthread_join(thread_pool[j], NULL);
    }
}

void *mat_mul_avx_int8_thread_func_2x2_32unroll_bfp32_ofp32(void *args) {
    int i, j, k;
    struct thread_args *thread_args = (struct thread_args *)args;
    const struct matmul_params *params = thread_args->params;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr;
    float *data_C = C->data_ptr;
    int start_i = thread_args->start_i, end_i = thread_args->end_i;
    const int8_t q_min = C->qparams.q_min, q_max = C->qparams.q_max;

    assert((end_i - start_i) % 2 == 0);

    for (i = start_i; i < end_i; i += 2)

        for (j = 0; j < C->column; j += 2) {
            // (i, j), (i, j+1), (i+1, j), (i+1, j+1)
            int acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
            __m256i acc0_8x32 = _mm256_setzero_si256(), acc1_8x32 = _mm256_setzero_si256(),
                    acc2_8x32 = _mm256_setzero_si256(), acc3_8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 32) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]);
                __m256i cc = _mm256_loadu_si256((const __m256i_u *)&data_A[(i + 1) * A->column + k]);
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]);
                __m256i dd = _mm256_loadu_si256((const __m256i_u *)&data_B[(j + 1) * B->row + k]);

                multiply_signed_int8_2x2_32epi(aa, bb, cc, dd, acc0_8x32, acc1_8x32, acc2_8x32, acc3_8x32);
            }
            int32_t *accptr = (int32_t *)&acc0_8x32;
            acc0 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            accptr = (int32_t *)&acc1_8x32;
            acc1 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            accptr = (int32_t *)&acc2_8x32;
            acc2 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            accptr = (int32_t *)&acc3_8x32;
            acc3 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];

            data_C[i * C->column + j] = ((float)acc0 * effective_scale) + params->bias.data_ptr[j];
            data_C[i * C->column + j + 1] = ((float)acc1 * effective_scale) + params->bias.data_ptr[j + 1];
            data_C[(i + 1) * C->column + j] = ((float)acc2 * effective_scale) + params->bias.data_ptr[j];
            data_C[(i + 1) * C->column + j + 1] = ((float)acc3 * effective_scale) + params->bias.data_ptr[j + 1];
        }
    return NULL;
}

void MatmulOperator::mat_mul_avx_int8_fast_2x2_32unroll_bfp32_ofp32(const struct matmul_params *params) {
    int j, num_thread = params->opt_params.num_thread;

    assert(params->A.column % 64 == 0);
    assert((params->C.column) % 2 == 0);

    pthread_t thread_pool[num_thread];
    struct thread_args threads_args[num_thread];

    // Thread creation
    for (j = 0; j < num_thread; j++) {
        threads_args[j].start_i = j * (params->C.row / num_thread);
        threads_args[j].end_i = (j + 1) * (params->C.row / num_thread);
        threads_args[j].blk_size = params->opt_params.blk_size;
        threads_args[j].params = params;
        pthread_create(&thread_pool[j], NULL, mat_mul_avx_int8_thread_func_2x2_32unroll_bfp32_ofp32, &threads_args[j]);
    }
    // Join threads
    for (j = 0; j < num_thread; j++) {
        pthread_join(thread_pool[j], NULL);
    }
}

void MatmulOperator::mat_mul_avx_int8_fast_2x2(const struct matmul_params *params) {
    int j, num_thread = params->opt_params.num_thread;

    assert(params->A.column % 64 == 0);
    assert((params->C.column) % 2 == 0);

    pthread_t thread_pool[num_thread];
    struct thread_args threads_args[num_thread];

    // Thread creation
    for (j = 0; j < num_thread; j++) {
        threads_args[j].start_i = j * (params->C.row / num_thread);
        threads_args[j].end_i = (j + 1) * (params->C.row / num_thread);
        threads_args[j].blk_size = params->opt_params.blk_size;
        threads_args[j].params = params;
        pthread_create(&thread_pool[j], NULL, mat_mul_avx_int8_thread_func_2x2, &threads_args[j]);
    }
    // Join threads
    for (j = 0; j < num_thread; j++) {
        pthread_join(thread_pool[j], NULL);
    }
}

void MatmulOperator::mat_mul_avx_int8_fast_2x2_omp(const struct matmul_params *params) {
    int i, j, k;
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    int32_t A_zp = A->qparams.zero_point, C_zp = C->qparams.zero_point;
    float A_sc = A->qparams.scale, B_sc = B->qparams.scale, C_sc = C->qparams.scale;
    float effective_scale = A_sc * B_sc / C_sc;
    int8_t *data_A = A->int8_data_ptr, *data_B = B->int8_data_ptr, *data_C = C->int8_data_ptr;
    const int8_t q_min = C->qparams.q_min, q_max = C->qparams.q_max;

    assert(A->column % 64 == 0);
    assert((C->column) % 2 == 0);
    assert((C->row) % 2 == 0);

    // #pragma omp parallel for
    for (i = 0; i < C->row; i += 2)
        for (j = 0; j < C->column; j += 2) {
            // (i, j), (i, j+1), (i+1, j), (i+1, j+1)
            int acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
            __m256i acc0_8x32 = _mm256_setzero_si256(), acc1_8x32 = _mm256_setzero_si256(),
                    acc2_8x32 = _mm256_setzero_si256(), acc3_8x32 = _mm256_setzero_si256();
            for (k = 0; k < A->column; k += 64) {
                __m256i aa = _mm256_loadu_si256((const __m256i_u *)&data_A[i * A->column + k]),
                        aa2 = _mm256_loadu_si256((const __m256i_u *)(&data_A[i * A->column + k + 32]));
                __m256i cc = _mm256_loadu_si256((const __m256i_u *)&data_A[(i + 1) * A->column + k]),
                        cc2 = _mm256_loadu_si256((const __m256i_u *)(&data_A[(i + 1) * A->column + k + 32]));
                // assume B is transposed
                __m256i bb = _mm256_loadu_si256((const __m256i_u *)&data_B[j * B->row + k]),
                        bb2 = _mm256_loadu_si256((const __m256i_u *)(&data_B[j * B->row + k + 32]));
                __m256i dd = _mm256_loadu_si256((const __m256i_u *)&data_B[(j + 1) * B->row + k]),
                        dd2 = _mm256_loadu_si256((const __m256i_u *)(&data_B[(j + 1) * B->row + k + 32]));

                multiply_signed_int8_2x2(aa, bb, aa2, bb2, cc, cc2, dd, dd2, acc0_8x32, acc1_8x32, acc2_8x32,
                                         acc3_8x32);
            }
            int32_t *accptr = (int32_t *)&acc0_8x32;
            acc0 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc0 += params->bias.int32_data_ptr[j];
            accptr = (int32_t *)&acc1_8x32;
            acc1 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc1 += params->bias.int32_data_ptr[j + 1];
            accptr = (int32_t *)&acc2_8x32;
            acc2 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc2 += params->bias.int32_data_ptr[j];
            accptr = (int32_t *)&acc3_8x32;
            acc3 = accptr[0] + accptr[1] + accptr[2] + accptr[3] + accptr[4] + accptr[5] + accptr[6] + accptr[7];
            acc3 += params->bias.int32_data_ptr[j + 1];

            acc0 = (int32_t)std::round((float)acc0 * effective_scale);
            acc1 = (int32_t)std::round((float)acc1 * effective_scale);
            acc2 = (int32_t)std::round((float)acc2 * effective_scale);
            acc3 = (int32_t)std::round((float)acc3 * effective_scale);

            acc0 -= C_zp;
            acc1 -= C_zp;
            acc2 -= C_zp;
            acc3 -= C_zp;

            acc0 = MAX(acc0, q_min);
            acc1 = MAX(acc1, q_min);
            acc2 = MAX(acc2, q_min);
            acc3 = MAX(acc3, q_min);
            acc0 = MIN(acc0, q_max);
            acc1 = MIN(acc1, q_max);
            acc2 = MIN(acc2, q_max);
            acc3 = MIN(acc3, q_max);
            data_C[i * C->column + j] = (int8_t)acc0;
            data_C[i * C->column + j + 1] = (int8_t)acc1;
            data_C[(i + 1) * C->column + j] = (int8_t)acc2;
            data_C[(i + 1) * C->column + j + 1] = (int8_t)acc3;
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

//     __m256i acc0_8x32 = multiply_signed_int8(aa, bb, aa2, bb2);
//     int32_t *accptr = (int32_t*)&acc0_8x32;
//     acc = accptr[0] + accptr[1] + accptr[2] + accptr[3]+ accptr[4] + accptr[5] + accptr[6] + accptr[7];

//     printf("%d, %d\n", acc, ref_acc);
//     assert(acc == ref_acc);

//     return 0;
// }
