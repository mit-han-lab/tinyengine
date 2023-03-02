/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.c
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

// #include "arm_math.h"
// #include "arm_nnfunctions.h"
// #include "arm_nnsupportfunctions.h"
// #include "img2col_element.h"
#include "tinyengine_function.h"

tinyengine_status convolve_s8_kernel3_inputch3_stride2_pad1_fpreq(
    const q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const float *scales, const int32_t output_offset, const int32_t input_offset,
    const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output, const uint16_t output_x,
    const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q15_t *kbuf, q7_t pad_value) {
    const int kernel_y = 3;
    const int kernel_x = 3;

    int16_t i_out_y, i_out_x;

    /* Generate two columns from the input tensor a GEMM computation */
    q15_t *two_column_buf = runtime_buf;
    q7_t *out = output;

    q15_t pad16 = pad_value;
    const int16_t inoff16 = input_offset;
    q15_t pad_out = pad16 + inoff16;
    // q31_t pad_out_q15x2 = __PKHBT(pad_out, pad_out, 16);
    // q31_t offset_q15x2 = __PKHBT(inoff16, inoff16, 16);

    // const q7_t *ip_a0 = kernel;

    // arm specific
    // for (int i = 0; i < output_ch; i += 2) {
    // 	q15_t *dst1 = &kbuf[i * 27]; //each q31_t store 2 elements
    // 	q15_t *dst2 = dst1 + 27;

    // 	const q7_t *ip_a1 = ip_a0 + 27;

    // 	//27 for each output_ch
    // 	q31_t *dst1_31 = dst1;
    // 	q31_t *dst2_31 = dst2;
    // 	ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
    // 	ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
    // 	dst1_31 += 2;
    // 	dst2_31 += 2;

    // 	ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
    // 	ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
    // 	dst1_31 += 2;
    // 	dst2_31 += 2;

    // 	ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
    // 	ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
    // 	dst1_31 += 2;
    // 	dst2_31 += 2;

    // 	ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
    // 	ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
    // 	dst1_31 += 2;
    // 	dst2_31 += 2;

    // 	ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
    // 	ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
    // 	dst1_31 += 2;
    // 	dst2_31 += 2;

    // 	ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
    // 	ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
    // 	dst1_31 += 2;
    // 	dst2_31 += 2;
    // 	//25, 26, 27
    // 	dst1 = dst1_31;
    // 	dst2 = dst2_31;
    // 	dst1[0] = *ip_a0++;
    // 	dst1[1] = *ip_a0++;
    // 	dst1[2] = *ip_a0++;
    // 	dst2[0] = *ip_a1++;
    // 	dst2[1] = *ip_a1++;
    // 	dst2[2] = *ip_a1++;

    // 	/* skip row */
    // 	ip_a0 += 27;
    // }

    for (i_out_y = 0; i_out_y < output_y; i_out_y++) {
        for (i_out_x = 0; i_out_x < output_x; i_out_x++) {
            /* This part implements the im2col function */
            const int16_t base_idx_y = (i_out_y * 2) - 1;
            const int16_t base_idx_x = (i_out_x * 2) - 1;
            q15_t *col_buffer = two_column_buf;

            // ARM specific variables
            // q31_t in_q7x4;
            // q31_t in_q15x2_1;
            // q31_t in_q15x2_2;
            // q31_t out_q15x2_1;
            // q31_t out_q15x2_2;

            /* load address:8bit */
            const q7_t *src;
            const q7_t *src2;
            const q7_t *src3;

            /* buffer for load:16bit */
            q15_t *dst;
            q15_t *dst2;
            q15_t *dst3;

            int input_row_offset = 3 * input_x;
            dst = col_buffer;
            dst2 = dst + 9;
            dst3 = dst2 + 9;
            if (base_idx_y != -1) {
                if (base_idx_x != -1) {  // load all for now and unroll all
                    // 3x3 = 9 elements
                    src = input + (base_idx_y * input_x + base_idx_x) * input_ch;
                    src2 = src + input_row_offset;
                    src3 = src2 + input_row_offset;

                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;

                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;

                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    // ARM specific
                    // //4 * 2 = 8
                    // q7_q15_offset_ele(src, dst)
                    // q7_q15_offset_ele(src, dst)
                    // *dst++ = *src++ + input_offset;
                    // //
                    // q7_q15_offset_ele(src2, dst2)
                    // q7_q15_offset_ele(src2, dst2)
                    // *dst2++ = *src2++ + input_offset;
                    // //
                    // q7_q15_offset_ele(src3, dst3)
                    // q7_q15_offset_ele(src3, dst3)
                    // *dst3++ = *src3++ + input_offset;
                } else {  // first element is pad
                          // 3x3 = 9 elements
                    src = input + (base_idx_y * input_x) * input_ch;
                    src2 = src + input_row_offset;
                    src3 = src2 + input_row_offset;

                    // pad the first one: 1x3 = 3
                    *dst++ = pad_out;
                    *dst++ = pad_out;
                    *dst++ = pad_out;
                    *dst2++ = pad_out;
                    *dst2++ = pad_out;
                    *dst2++ = pad_out;
                    *dst3++ = pad_out;
                    *dst3++ = pad_out;
                    *dst3++ = pad_out;

                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;
                    *dst++ = *src++ + input_offset;

                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;

                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    // ARM specific
                    // //load 6 elements
                    // //4 * 1 = 6
                    // q7_q15_offset_ele(src, dst)
                    // *dst++ = *src++ + input_offset;
                    // *dst++ = *src++ + input_offset;
                    // //
                    // q7_q15_offset_ele(src2, dst2)
                    // *dst2++ = *src2++ + input_offset;
                    // *dst2++ = *src2++ + input_offset;
                    // //
                    // q7_q15_offset_ele(src3, dst3)
                    // *dst3++ = *src3++ + input_offset;
                    // *dst3++ = *src3++ + input_offset;
                }
            } else {  // first row is padded
                // 3x3 = 9 elements
                *dst++ = pad_out;
                *dst++ = pad_out;
                *dst++ = pad_out;
                *dst++ = pad_out;
                *dst++ = pad_out;
                *dst++ = pad_out;
                *dst++ = pad_out;
                *dst++ = pad_out;
                *dst++ = pad_out;
                // ARM specific
                // *dst++ = pad_out;
                // q31_t *dst_31 = (q31_t *)dst;
                // *dst_31++ = pad_out_q15x2;
                // *dst_31++ = pad_out_q15x2;
                // *dst_31++ = pad_out_q15x2;
                // *dst_31++ = pad_out_q15x2;
                if (base_idx_x != -1) {  // load all for now and unroll all
                    // 3x3 = 9 elements
                    src2 = input + (base_idx_x)*input_ch;
                    src3 = src2 + input_row_offset;

                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;

                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    // ARM specific
                    // //4 * 2 = 8
                    // q7_q15_offset_ele(src2, dst2)
                    // q7_q15_offset_ele(src2, dst2)
                    // *dst2++ = *src2++ + input_offset;
                    // //
                    // q7_q15_offset_ele(src3, dst3)
                    // q7_q15_offset_ele(src3, dst3)
                    // *dst3++ = *src3++ + input_offset;
                } else {  // first element is pad
                          // 3x3 = 9 elements
                    src2 = input;
                    src3 = src2 + input_row_offset;

                    // pad the first one: 1x3 = 3
                    *dst2++ = pad_out;
                    *dst2++ = pad_out;
                    *dst2++ = pad_out;
                    *dst3++ = pad_out;
                    *dst3++ = pad_out;
                    *dst3++ = pad_out;

                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;
                    *dst2++ = *src2++ + input_offset;

                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;
                    *dst3++ = *src3++ + input_offset;

                    // ARM specific
                    // //load 6 elements
                    // q7_q15_offset_ele(src2, dst2)
                    // *dst2++ = *src2++ + input_offset;
                    // *dst2++ = *src2++ + input_offset;
                    // //
                    // q7_q15_offset_ele(src3, dst3)
                    // *dst3++ = *src3++ + input_offset;
                    // *dst3++ = *src3++ + input_offset;
                }
            }

            two_column_buf += 27;
            /* Computation is filed for every 2 columns */
            if (two_column_buf == runtime_buf + 2 * 27) {
                out = mat_mult_kernel3_input3_s8_s16_fpreq(kernel, runtime_buf, output_ch, scales, output_offset,
                                                           output_activation_min, output_activation_max,
                                                           input_ch * kernel_y * kernel_x, bias, out, kbuf);

                /* counter reset */
                two_column_buf = runtime_buf;
            }
        }
    }

    /* left-over because odd number of output pixels */
    if (two_column_buf != runtime_buf) {
        const q7_t *ker_a = kernel;
        int i;

        for (i = 0; i < output_ch; i++) {
            /* Load the accumulator with bias first */
            q31_t sum = bias[i];

            /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
            const q15_t *ip_as_col = runtime_buf;

            /* 4 multiply and accumulates are done in one loop. */
            uint16_t col_count = (input_ch * kernel_y * kernel_x) >> 2;

            while (col_count) {
                sum += *ker_a++ * *ip_as_col++;
                sum += *ker_a++ * *ip_as_col++;
                sum += *ker_a++ * *ip_as_col++;
                sum += *ker_a++ * *ip_as_col++;
                // ARM specific
                // q31_t ker_a1, ker_a2;
                // q31_t ip_b1, ip_b2;

                // ker_a = read_and_pad(ker_a, &ker_a1, &ker_a2);

                // ip_b1 = arm_nn_read_q15x2_ia(&ip_as_col);
                // sum = __SMLAD(ker_a1, ip_b1, sum);
                // ip_b2 = arm_nn_read_q15x2_ia(&ip_as_col);
                // sum = __SMLAD(ker_a2, ip_b2, sum);

                col_count--;
            }
            /* Handle left over mac */
            col_count = input_ch * kernel_y * kernel_x & 0x3;
            while (col_count) {
                q7_t ker_a1 = *ker_a++;
                q15_t ip_b1 = *ip_as_col++;
                sum += ker_a1 * ip_b1;
                col_count--;
            }

            sum = (float)sum * scales[i];
            sum += output_offset;
            sum = TN_MAX(sum, output_activation_min);
            sum = TN_MIN(sum, output_activation_max);
            *out++ = (q7_t)sum;
        }
    }

    /* Return to application */
    return STATE_SUCCESS;
}
