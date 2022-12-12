/*
 * Copyright (C) 2010-2020 Arm Limited or its affiliates. All rights reserved.
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
 * Title:   arm_nn_mat_mult_kernel3_input3_s8_s16.c
 * Description:  Matrix-multiplication function for convolution (input channel = 3 and kernel size = 3).
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
 * Original Title:        arm_nn_mat_mult_kernel_s8_s16.c
 *
 * Target Processor:  Cortex-M cores
 * -------------------------------------------------------------------- */

#include "arm_math.h"
#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"

/*
   * Matrix-multiplication function for convolution with per-channel requantization.
   *
   * Refer header file for details.
   *
   */

q7_t *arm_nn_mat_mult_kernel3_input3_s8_s16(const q7_t *input_a,
        const q15_t *input_b,
        const uint16_t output_ch,
        const int32_t *out_shift,
        const int32_t *out_mult,
        const int32_t out_offset,
        const int16_t activation_min,
        const int16_t activation_max,
        const uint16_t num_col_a,
        const int32_t *const output_bias,
        q7_t *out_0,
		q15_t *kbuf)
{
    /* set up the second output pointers */
    q7_t *out_1 = out_0 + output_ch;
    const int32_t *bias = output_bias;

    uint16_t row_count = output_ch / 2;
    const q15_t *ksrc = &kbuf[0];
    /* this loop over rows in A */
    while (row_count)
    {
        /* setup pointers for B */
        const q15_t *ip_b0 = input_b;
        const q15_t *ip_b1 = ip_b0 + num_col_a;
        const q31_t *ip31_b0 = ip_b0;
        const q31_t *ip31_b1 = ip_b1;

        /* align the second pointer for A */
        const q15_t *ksrc2 = ksrc + 27;
        q31_t *ksrc_31 = ksrc;
        q31_t *ksrc2_31 = ksrc2;

        /* Init accumulator with bias for channel N and N + 1 */
        q31_t ch_0_out_0 = *bias;
        q31_t ch_0_out_1 = *bias++;
        q31_t ch_1_out_0 = *bias;
        q31_t ch_1_out_1 = *bias++;

		//------------------4
		q31_t a01, a02, a11, a12;
		q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
		q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(ksrc_31[0], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[0], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[0], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[0], b1, ch_1_out_1);

		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(ksrc_31[1], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[1], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[1], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[1], b1, ch_1_out_1);

		//------------------8
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(ksrc_31[2], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[2], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[2], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[2], b1, ch_1_out_1);

		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(ksrc_31[3], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[3], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[3], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[3], b1, ch_1_out_1);

		//------------------12
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(ksrc_31[4], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[4], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[4], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[4], b1, ch_1_out_1);

		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(ksrc_31[5], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[5], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[5], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[5], b1, ch_1_out_1);

		//------------------16
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);
		ch_0_out_0 = __SMLAD(ksrc_31[6], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[6], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[6], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[6], b1, ch_1_out_1);

		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(ksrc_31[7], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[7], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[7], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[7], b1, ch_1_out_1);

		//------------------20
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);
		ch_0_out_0 = __SMLAD(ksrc_31[8], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[8], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[8], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[8], b1, ch_1_out_1);

		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(ksrc_31[9], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[9], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[9], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[9], b1, ch_1_out_1);

		//------------------24
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);
		ch_0_out_0 = __SMLAD(ksrc_31[10], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[10], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[10], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[10], b1, ch_1_out_1);

		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(ksrc_31[11], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[11], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[11], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[11], b1, ch_1_out_1);

		//------------------25,26,27
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		b1 = arm_nn_read_q15x2_ia(&ip_b1);
		ch_0_out_0 = __SMLAD(ksrc_31[12], b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(ksrc_31[12], b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(ksrc2_31[12], b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(ksrc2_31[12], b1, ch_1_out_1);
		q15_t _b0 = *ip_b0++;
		q15_t _b1 = *ip_b1++;

		ch_0_out_0 += ksrc[26] * _b0;
		ch_0_out_1 += ksrc[26] * _b1;
		ch_1_out_0 += ksrc2[26] * _b0;
		ch_1_out_1 += ksrc2[26] * _b1;

        ch_0_out_0 = arm_nn_requantize(ch_0_out_0, *out_mult, *out_shift);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = MIN(ch_0_out_0, activation_max);
        *out_0++ = (q7_t)ch_0_out_0;

        ch_0_out_1 = arm_nn_requantize(ch_0_out_1, *out_mult, *out_shift);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = MIN(ch_0_out_1, activation_max);
        *out_1++ = (q7_t)ch_0_out_1;
        out_mult++;
        out_shift++;

        ch_1_out_0 = arm_nn_requantize(ch_1_out_0, *out_mult, *out_shift);
        ch_1_out_0 += out_offset;
        ch_1_out_0 = MAX(ch_1_out_0, activation_min);
        ch_1_out_0 = MIN(ch_1_out_0, activation_max);
        *out_0++ = (q7_t)ch_1_out_0;

        ch_1_out_1 = arm_nn_requantize(ch_1_out_1, *out_mult, *out_shift);
        ch_1_out_1 += out_offset;
        ch_1_out_1 = MAX(ch_1_out_1, activation_min);
        ch_1_out_1 = MIN(ch_1_out_1, activation_max);
        *out_1++ = (q7_t)ch_1_out_1;
        out_mult++;
        out_shift++;

        /* skip row */
        ksrc += 54;
        row_count--;
    }

    out_0 += output_ch;

    /* return the new output pointer with offset */
    return out_0;
}
