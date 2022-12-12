/*
 * Copyright (C) 2010-2022 Arm Limited or its affiliates.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * This file is MODIFIED from Arm CMSIS NN Library.
 *
 * Project: TinyEngine
 * Title:   arm_convolve_s8_4col.c
 * Description:  s8_4col version of convolution using symmetric quantization.
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
 * Original Project:      CMSIS NN Library
 * Original Title:        arm_convolve_s8.c
 *
 * Target Processor:  Cortex-M CPUs
 * -------------------------------------------------------------------- */

#include "arm_math.h"
#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup NNConv
 * @{
 */

/*
 * Basic s8_4col convolution function.
 *
 * Refer header file for details. Optimal use case for the DSP/MVE implementation is when input and output channels
 * are multiples of 4 or atleast greater than 4.
 *
 */

arm_status arm_convolve_s8_4col(const q7_t *input,
                           const uint16_t input_x,
                           const uint16_t input_y,
                           const uint16_t input_ch,
                           const uint16_t input_batches,
                           const q7_t *kernel,
                           const uint16_t output_ch,
                           const uint16_t kernel_x,
                           const uint16_t kernel_y,
                           const uint16_t pad_x,
                           const uint16_t pad_y,
                           const uint16_t stride_x,
                           const uint16_t stride_y,
                           const int32_t *bias,
                           q7_t *output,
                           const int32_t *output_shift,
                           const int32_t *output_mult,
                           const int32_t out_offset,
                           const int32_t input_offset,
                           const int32_t out_activation_min,
                           const int32_t out_activation_max,
                           const uint16_t output_x,
                           const uint16_t output_y,
                           q15_t *buffer_a)
{
    int i_batch;
    for (i_batch = 0; i_batch < input_batches; i_batch++)
    {
        input += i_batch * (input_x * input_y * input_ch);
        output += i_batch * (output_x * output_y * output_ch);

        int16_t i_out_y, i_out_x, i_ker_y, i_ker_x;

        /* Generate two columns from the input tensor a GEMM computation */
        q15_t *four_column_buf = buffer_a;

        q7_t *out = output;

        /* This part implements the im2col function */
        for (i_out_y = 0; i_out_y < output_y; i_out_y++)
        {
            for (i_out_x = 0; i_out_x < output_x; i_out_x++)
            {
                for (i_ker_y = i_out_y * stride_y - pad_y; i_ker_y < i_out_y * stride_y - pad_y + kernel_y; i_ker_y++)
                {
                    for (i_ker_x = i_out_x * stride_x - pad_x; i_ker_x < i_out_x * stride_x - pad_x + kernel_x; i_ker_x++)
                    {
                        if (i_ker_y < 0 || i_ker_y >= input_y || i_ker_x < 0 || i_ker_x >= input_x)
                        {
                            /* Filling 0 for out-of-bound paddings */
                            memset(four_column_buf, 0, sizeof(q15_t) * input_ch);
                        }
                        else
                        {
                            /* Copying the pixel data to column */
                            arm_q7_to_q15_with_offset(input + (i_ker_y * input_x + i_ker_x) * input_ch, four_column_buf, input_ch, input_offset);
                        }
                        four_column_buf += input_ch;
                    }
                }

                /* Computation is filed for every 2 columns */
                if (four_column_buf == buffer_a + 4 * input_ch * kernel_y * kernel_x)
                {
                    out =
                        arm_nn_mat_mult_kernel_s8_s16_4col(kernel,
                                                      buffer_a,
                                                      output_ch,
                                                      output_shift,
                                                      output_mult,
                                                      out_offset,
                                                      out_activation_min,
                                                      out_activation_max,
                                                      input_ch * kernel_y * kernel_x,
                                                      bias,
                                                      out);

                    /* counter reset */
                    four_column_buf = buffer_a;
                }
            }
        }

        q15_t *four_column_buf_mid = buffer_a;

        if (four_column_buf >= four_column_buf_mid + 2 * input_ch * kernel_y * kernel_x) {
            out =
                arm_nn_mat_mult_kernel_s8_s16(kernel,
                                              four_column_buf_mid,
                                              output_ch,
                                              output_shift,
                                              output_mult,
                                              out_offset,
                                              out_activation_min,
                                              out_activation_max,
                                              input_ch * kernel_y * kernel_x,
                                              bias,
                                              out);

            four_column_buf_mid = buffer_a + 2 * input_ch * kernel_y * kernel_x;

        }

        /* left-over because odd number of output pixels */
        if (four_column_buf != four_column_buf_mid)
        {
            const q7_t *ker_a = kernel;
            int i;

            for (i = 0; i < output_ch; i++)
            {
                /* Load the accumulator with bias first */
                q31_t sum = bias[i];

                /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
                const q15_t *ip_as_col = four_column_buf_mid;

                /* 4 multiply and accumulates are done in one loop. */
                uint16_t col_count = (input_ch * kernel_y * kernel_x) >> 2;

                while (col_count)
                {
                    q31_t ker_a1, ker_a2;
                    q31_t ip_b1, ip_b2;

                    ker_a = read_and_pad(ker_a, &ker_a1, &ker_a2);

                    ip_b1 = arm_nn_read_q15x2_ia(&ip_as_col);
                    sum = __SMLAD(ker_a1, ip_b1, sum);
                    ip_b2 = arm_nn_read_q15x2_ia(&ip_as_col);
                    sum = __SMLAD(ker_a2, ip_b2, sum);

                    col_count--;
                }
                /* Handle left over mac */
                col_count = input_ch * kernel_y * kernel_x & 0x3;
                while (col_count)
                {
                    q7_t ker_a1 = *ker_a++;
                    q15_t ip_b1 = *ip_as_col++;
                    sum += ker_a1 * ip_b1;
                    col_count--;
                }

                sum = arm_nn_requantize(sum, output_mult[i], output_shift[i]);
                sum += out_offset;
                sum = MAX(sum, out_activation_min);
                sum = MIN(sum, out_activation_max);
                *out++ = (q7_t)sum;
            }
        }
    }

    /* Return to application */
    return ARM_MATH_SUCCESS;
}

/**
 * @} end of NNConv group
 */