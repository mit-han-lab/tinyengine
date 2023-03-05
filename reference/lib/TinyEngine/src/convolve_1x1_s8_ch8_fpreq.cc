/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   convolve_1x1_s8_ch8_fpreq.c
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

// #include "arm_nnfunctions.h"
// #include "img2col_element.h"
#include "tinyengine_function.h"

#define DIM_KER_X (1U)
#define DIM_KER_Y (1U)

tinyengine_status convolve_1x1_s8_ch8_fpreq(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                            const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
                                            const float *scales, const int32_t out_offset, const int32_t input_offset,
                                            const int32_t out_activation_min, const int32_t out_activation_max,
                                            q7_t *output, const uint16_t output_x, const uint16_t output_y,
                                            const uint16_t output_ch, q15_t *runtime_buf) {
    int32_t i_element;
    (void)input_x;
    (void)input_y;

    /* Partial(two columns) im2col buffer */
    q15_t *two_column_buffer = runtime_buf;
    q7_t *out = output;
    const int32_t num_elements = output_x * output_y;
    const int channel_div4 = (input_ch >> 2);

    for (i_element = 0; i_element < num_elements / 2; i_element++) {
        /* Fill buffer for partial im2col - two columns at a time */
        const q7_t *src = &input[i_element * input_ch * 2];
        q15_t *dst = two_column_buffer;

        int cnt = channel_div4;  // two columns
        while (cnt > 0) {
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            cnt--;
        }

        out = mat_mult_kernel_s8_s16_reordered_fpreq(kernel, two_column_buffer, output_ch, scales, (q7_t)out_offset,
                                                     out_activation_min, out_activation_max,
                                                     input_ch * DIM_KER_Y * DIM_KER_X, bias, out);
    }

    /* check if there is an odd column left-over for computation */
    if (num_elements & 0x1) {
        int32_t ch_out;
        const q7_t *ker_a = kernel;
        const q7_t *src = &input[(num_elements - 1) * input_ch];
        q15_t *dst = two_column_buffer;

        int cnt = channel_div4;  // two * numof2col columns
        while (cnt > 0) {
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            cnt--;
        }

        for (ch_out = 0; ch_out < output_ch; ch_out++) {
            q31_t sum = bias[ch_out];

            /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
            const q15_t *ip_as_col = runtime_buf;
            uint16_t col_count = (input_ch * DIM_KER_X * DIM_KER_Y) >> 2;

            while (col_count) {
                sum += *ip_as_col++ * *ker_a++;
                sum += *ip_as_col++ * *ker_a++;
                sum += *ip_as_col++ * *ker_a++;
                sum += *ip_as_col++ * *ker_a++;
                col_count--;
            }

            sum = (float)sum * scales[ch_out];
            sum += out_offset;
            sum = TN_MAX(sum, out_activation_min);
            sum = TN_MIN(sum, out_activation_max);
            *out++ = (q7_t)sum;
        }
    }

    /* Return to application */
    return STATE_SUCCESS;
}
