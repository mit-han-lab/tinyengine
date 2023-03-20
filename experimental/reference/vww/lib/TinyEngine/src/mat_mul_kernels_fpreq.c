#include <stdio.h>

#include "tinyengine_function.h"

q7_t *mat_mult_kernel_s8_s16_fpreq(const q7_t *input_a, const q15_t *input_b, const uint16_t output_ch,
                                   const float *scales, const int32_t out_offset, const int16_t activation_min,
                                   const int16_t activation_max, const uint16_t num_col_a,
                                   const int32_t *const output_bias, q7_t *out_0) {
    /* set up the second output pointers */
    q7_t *out_1 = out_0 + output_ch;
    const int32_t *bias = output_bias;

    uint16_t row_count = output_ch / 2;
    const q7_t *ip_a0 = input_a;
    /* this loop over rows in A */
    while (row_count) {
        /* setup pointers for B */
        const q15_t *ip_b0 = input_b;
        const q15_t *ip_b1 = ip_b0 + num_col_a;

        /* align the second pointer for A */
        const q7_t *ip_a1 = ip_a0 + num_col_a;
        const float scale_0 = scales[0];
        const float scale_1 = scales[1];

        /* Init accumulator with bias for channel N and N + 1 */
        q31_t ch_0_out_0 = *bias;
        q31_t ch_0_out_1 = *bias++;
        q31_t ch_1_out_0 = *bias;
        q31_t ch_1_out_1 = *bias++;

        uint16_t col_count = num_col_a / 4;
        /* accumulate over the vector */
        while (col_count) {
            ch_0_out_0 += ip_b0[0] * ip_a0[0];
            ch_0_out_0 += ip_b0[1] * ip_a0[1];
            ch_0_out_0 += ip_b0[2] * ip_a0[2];
            ch_0_out_0 += ip_b0[3] * ip_a0[3];
            ch_0_out_1 += ip_b1[0] * ip_a0[0];
            ch_0_out_1 += ip_b1[1] * ip_a0[1];
            ch_0_out_1 += ip_b1[2] * ip_a0[2];
            ch_0_out_1 += ip_b1[3] * ip_a0[3];

            ch_1_out_0 += ip_b0[0] * ip_a1[0];
            ch_1_out_0 += ip_b0[1] * ip_a1[1];
            ch_1_out_0 += ip_b0[2] * ip_a1[2];
            ch_1_out_0 += ip_b0[3] * ip_a1[3];
            ch_1_out_1 += ip_b1[0] * ip_a1[0];
            ch_1_out_1 += ip_b1[1] * ip_a1[1];
            ch_1_out_1 += ip_b1[2] * ip_a1[2];
            ch_1_out_1 += ip_b1[3] * ip_a1[3];

            ip_b0 += 4;
            ip_b1 += 4;
            ip_a0 += 4;
            ip_a1 += 4;
            col_count--;
        } /* while over col_count */

        ch_0_out_0 = (q31_t)((float)ch_0_out_0 * scale_0);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = TN_MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = TN_MIN(ch_0_out_0, activation_max);
        *out_0++ = (q7_t)ch_0_out_0;

        ch_0_out_1 = (q31_t)((float)ch_0_out_1 * scale_0);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = TN_MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = TN_MIN(ch_0_out_1, activation_max);
        *out_1++ = (q7_t)ch_0_out_1;

        ch_1_out_0 = (q31_t)((float)ch_1_out_0 * scale_1);
        ch_1_out_0 += out_offset;
        ch_1_out_0 = TN_MAX(ch_1_out_0, activation_min);
        ch_1_out_0 = TN_MIN(ch_1_out_0, activation_max);
        *out_0++ = (q7_t)ch_1_out_0;

        ch_1_out_1 = (q31_t)((float)ch_1_out_1 * scale_1);
        ch_1_out_1 += out_offset;
        ch_1_out_1 = TN_MAX(ch_1_out_1, activation_min);
        ch_1_out_1 = TN_MIN(ch_1_out_1, activation_max);
        *out_1++ = (q7_t)ch_1_out_1;
        scales += 2;

        /* skip row */
        ip_a0 += num_col_a;
        row_count--;
    }

    if (output_ch % 2 == 1) {
        /* setup pointers for B */
        const q15_t *ip_b0 = input_b;
        const q15_t *ip_b1 = ip_b0 + num_col_a;

        /* Init accumulator with bias for channel N + 1 */
        q31_t ch_0_out_0 = *bias;
        q31_t ch_0_out_1 = *bias;

        int32_t col_count = num_col_a / 4;
        while (col_count) {
            ch_0_out_0 += *ip_b0++ * ip_a0[0];
            ch_0_out_0 += *ip_b0++ * ip_a0[1];
            ch_0_out_0 += *ip_b0++ * ip_a0[2];
            ch_0_out_0 += *ip_b0++ * ip_a0[3];

            ch_0_out_1 += *ip_b1++ * ip_a0[0];
            ch_0_out_1 += *ip_b1++ * ip_a0[1];
            ch_0_out_1 += *ip_b1++ * ip_a0[2];
            ch_0_out_1 += *ip_b1++ * ip_a0[3];

            col_count--;
            ip_a0 += 4;
        } /* while over col_count */

        ch_0_out_0 = (q31_t)((float)ch_0_out_0 * *scales);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = TN_MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = TN_MIN(ch_0_out_0, activation_max);
        *out_0++ = (q7_t)ch_0_out_0;

        ch_0_out_1 = (q31_t)((float)ch_0_out_1 * *scales);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = TN_MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = TN_MIN(ch_0_out_1, activation_max);
        *out_1++ = (q7_t)ch_0_out_1;
    }

    out_0 += output_ch;

    /* return the new output pointer with offset */
    return out_0;
}

q7_t *mat_mult_kernel_s8_s16_multithread_fpreq(const q7_t *input_a, const q15_t *input_b, const uint16_t output_ch,
                                               const float *scales, const int32_t out_offset,
                                               const int16_t activation_min, const int16_t activation_max,
                                               const uint16_t num_col_a, const int32_t *const output_bias,
                                               q7_t *out_0) {
    /* set up the second output pointers */
    q7_t *out_1 = out_0 + output_ch;
    const int32_t *bias = output_bias;

    uint16_t row_count = output_ch / 2;
    const q7_t *ip_a0 = input_a;
    /* this loop over rows in A */
    while (row_count) {
        /* setup pointers for B */
        const q15_t *ip_b0 = input_b;
        const q15_t *ip_b1 = ip_b0 + num_col_a;

        /* align the second pointer for A */
        const q7_t *ip_a1 = ip_a0 + num_col_a;
        const float scale_0 = scales[0];
        const float scale_1 = scales[1];

        /* Init accumulator with bias for channel N and N + 1 */
        q31_t ch_0_out_0 = *bias;
        q31_t ch_0_out_1 = *bias++;
        q31_t ch_1_out_0 = *bias;
        q31_t ch_1_out_1 = *bias++;

        uint16_t col_count = num_col_a / 4;
        /* accumulate over the vector */
        while (col_count) {
            ch_0_out_0 += ip_b0[0] * ip_a0[0];
            ch_0_out_0 += ip_b0[1] * ip_a0[1];
            ch_0_out_0 += ip_b0[2] * ip_a0[2];
            ch_0_out_0 += ip_b0[3] * ip_a0[3];
            ch_0_out_1 += ip_b1[0] * ip_a0[0];
            ch_0_out_1 += ip_b1[1] * ip_a0[1];
            ch_0_out_1 += ip_b1[2] * ip_a0[2];
            ch_0_out_1 += ip_b1[3] * ip_a0[3];

            ch_1_out_0 += ip_b0[0] * ip_a1[0];
            ch_1_out_0 += ip_b0[1] * ip_a1[1];
            ch_1_out_0 += ip_b0[2] * ip_a1[2];
            ch_1_out_0 += ip_b0[3] * ip_a1[3];
            ch_1_out_1 += ip_b1[0] * ip_a1[0];
            ch_1_out_1 += ip_b1[1] * ip_a1[1];
            ch_1_out_1 += ip_b1[2] * ip_a1[2];
            ch_1_out_1 += ip_b1[3] * ip_a1[3];

            ip_b0 += 4;
            ip_b1 += 4;
            ip_a0 += 4;
            ip_a1 += 4;
            col_count--;
        } /* while over col_count */

        ch_0_out_0 = (q31_t)((float)ch_0_out_0 * scale_0);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = TN_MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = TN_MIN(ch_0_out_0, activation_max);
        *out_0++ = (q7_t)ch_0_out_0;

        ch_0_out_1 = (q31_t)((float)ch_0_out_1 * scale_0);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = TN_MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = TN_MIN(ch_0_out_1, activation_max);
        *out_1++ = (q7_t)ch_0_out_1;

        ch_1_out_0 = (q31_t)((float)ch_1_out_0 * scale_1);
        ch_1_out_0 += out_offset;
        ch_1_out_0 = TN_MAX(ch_1_out_0, activation_min);
        ch_1_out_0 = TN_MIN(ch_1_out_0, activation_max);
        *out_0++ = (q7_t)ch_1_out_0;

        ch_1_out_1 = (q31_t)((float)ch_1_out_1 * scale_1);
        ch_1_out_1 += out_offset;
        ch_1_out_1 = TN_MAX(ch_1_out_1, activation_min);
        ch_1_out_1 = TN_MIN(ch_1_out_1, activation_max);
        *out_1++ = (q7_t)ch_1_out_1;
        scales += 2;

        /* skip row */
        ip_a0 += num_col_a;
        row_count--;
    }

    if (output_ch % 2 == 1) {
        /* setup pointers for B */
        const q15_t *ip_b0 = input_b;
        const q15_t *ip_b1 = ip_b0 + num_col_a;

        /* Init accumulator with bias for channel N + 1 */
        q31_t ch_0_out_0 = *bias;
        q31_t ch_0_out_1 = *bias;

        int32_t col_count = num_col_a / 4;
        while (col_count) {
            ch_0_out_0 += *ip_b0++ * ip_a0[0];
            ch_0_out_0 += *ip_b0++ * ip_a0[1];
            ch_0_out_0 += *ip_b0++ * ip_a0[2];
            ch_0_out_0 += *ip_b0++ * ip_a0[3];

            ch_0_out_1 += *ip_b1++ * ip_a0[0];
            ch_0_out_1 += *ip_b1++ * ip_a0[1];
            ch_0_out_1 += *ip_b1++ * ip_a0[2];
            ch_0_out_1 += *ip_b1++ * ip_a0[3];

            col_count--;
            ip_a0 += 4;
        } /* while over col_count */

        ch_0_out_0 = (q31_t)((float)ch_0_out_0 * *scales);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = TN_MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = TN_MIN(ch_0_out_0, activation_max);
        *out_0++ = (q7_t)ch_0_out_0;

        ch_0_out_1 = (q31_t)((float)ch_0_out_1 * *scales);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = TN_MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = TN_MIN(ch_0_out_1, activation_max);
        *out_1++ = (q7_t)ch_0_out_1;
    }

    out_0 += output_ch;

    /* return the new output pointer with offset */
    return out_0;
}

#define reference_mac_2x2(k_ptr_q7, k1_ptr_q7, ip_q15, ip1_q15, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, idx) \
    ch_0_out_0 += k_ptr_q7[idx] * ip_q15[idx];                                                                       \
    ch_0_out_1 += k_ptr_q7[idx] * ip1_q15[idx];                                                                      \
    ch_1_out_0 += k1_ptr_q7[idx] * ip_q15[idx];                                                                      \
    ch_1_out_1 += k1_ptr_q7[idx] * ip1_q15[idx];

#define reference_mac_2x1(k_ptr_q7, k1_ptr_q7, ip_q15, ip1_q15, ch_0_out_0, ch_0_out_1, idx) \
    ch_0_out_0 += k_ptr_q7[idx] * ip_q15[idx];                                               \
    ch_0_out_1 += k_ptr_q7[idx] * ip1_q15[idx];

q7_t *mat_mult_kernel3_input3_s8_s16_fpreq(const q7_t *input_a, const q15_t *input_b, const uint16_t output_ch,
                                           const float *scales, const int32_t out_offset, const int16_t activation_min,
                                           const int16_t activation_max, const uint16_t num_col_a,
                                           const int32_t *const output_bias, q7_t *out_0, q15_t *kbuf) {
    (void)kbuf;
    /* set up the second output pointers */
    q7_t *out_1 = out_0 + output_ch;
    const int32_t *bias = output_bias;

    uint16_t row_count = output_ch / 2;

    /* this loop over rows in A */
    while (row_count) {
        /* setup pointers for B */
        const q15_t *ip_b0 = input_b;
        const q15_t *ip_b1 = ip_b0 + num_col_a;
        const float scale_0 = *scales++;
        const float scale_1 = *scales++;

        /* align the second pointer for A */
        const q7_t *input1_a = input_a + num_col_a;

        /* Init accumulator with bias for channel N and N + 1 */
        q31_t ch_0_out_0 = *bias;
        q31_t ch_0_out_1 = *bias++;
        q31_t ch_1_out_0 = *bias;
        q31_t ch_1_out_1 = *bias++;

        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 0);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 1);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 2);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 3);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 4);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 5);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 6);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 7);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 8);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 9);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 10);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 11);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 12);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 13);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 14);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 15);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 16);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 17);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 18);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 19);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 20);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 21);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 22);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 23);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 24);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 25);
        reference_mac_2x2(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, ch_1_out_0, ch_1_out_1, 26);

        ch_0_out_0 = (q31_t)((float)ch_0_out_0 * scale_0);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = TN_MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = TN_MIN(ch_0_out_0, activation_max);
        *out_0++ = (q7_t)ch_0_out_0;

        ch_0_out_1 = (q31_t)((float)ch_0_out_1 * scale_0);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = TN_MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = TN_MIN(ch_0_out_1, activation_max);
        *out_1++ = (q7_t)ch_0_out_1;

        ch_1_out_0 = (q31_t)((float)ch_1_out_0 * scale_1);
        ch_1_out_0 += out_offset;
        ch_1_out_0 = TN_MAX(ch_1_out_0, activation_min);
        ch_1_out_0 = TN_MIN(ch_1_out_0, activation_max);
        *out_0++ = (q7_t)ch_1_out_0;

        ch_1_out_1 = (q31_t)((float)ch_1_out_1 * scale_1);
        ch_1_out_1 += out_offset;
        ch_1_out_1 = TN_MAX(ch_1_out_1, activation_min);
        ch_1_out_1 = TN_MIN(ch_1_out_1, activation_max);
        *out_1++ = (q7_t)ch_1_out_1;

        /* skip row */
        input_a += 54;
        row_count--;
    }

    if (output_ch % 2 == 1) {
        /* setup pointers for B */
        const q15_t *ip_b0 = input_b;
        const q15_t *ip_b1 = ip_b0 + num_col_a;

        /* Init accumulator with bias for channel N + 1 */
        q31_t ch_0_out_0 = *bias;
        q31_t ch_0_out_1 = *bias;
        const q7_t *input1_a = input_a + num_col_a;

        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 0);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 1);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 2);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 3);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 4);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 5);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 6);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 7);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 8);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 9);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 10);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 11);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 12);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 13);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 14);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 15);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 16);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 17);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 18);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 19);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 20);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 21);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 22);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 23);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 24);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 25);
        reference_mac_2x1(input_a, input1_a, ip_b0, ip_b1, ch_0_out_0, ch_0_out_1, 26);

        ch_0_out_0 = (q31_t)((float)ch_0_out_0 * *scales);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = TN_MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = TN_MIN(ch_0_out_0, activation_max);
        *out_0++ = (q7_t)ch_0_out_0;

        ch_0_out_1 = (q31_t)((float)ch_0_out_1 * *scales);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = TN_MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = TN_MIN(ch_0_out_1, activation_max);
        *out_1++ = (q7_t)ch_0_out_1;
    }

    out_0 += output_ch;

    /* return the new output pointer with offset */
    return out_0;
}
