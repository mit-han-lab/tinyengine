/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   mat_mul_kernels_fpreq.c
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

#include "arm_nnsupportfunctions.h"
#include "tinyengine_lib.h"
#include "tinyengine_function.h"

q7_t* mat_mult_kernel_s8_s16_reordered_ch48_fpreq(const q7_t *input_a,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
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

		uint16_t col_count = num_col_a / 8;
		/* accumulate over the vector */
		q31_t a01, a02, a11, a12;
		q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
		q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

		//4
		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//8
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//12
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//16
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//20
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//24
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//28
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//32
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//36
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//40
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//44
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//48
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		ch_0_out_0 = MAX(ch_0_out_0, activation_min);
		ch_0_out_0 = MIN(ch_0_out_0, activation_max);
		*out_0++ = (q7_t) ch_0_out_0;

		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		ch_0_out_1 = MAX(ch_0_out_1, activation_min);
		ch_0_out_1 = MIN(ch_0_out_1, activation_max);
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		ch_1_out_0 = MAX(ch_1_out_0, activation_min);
		ch_1_out_0 = MIN(ch_1_out_0, activation_max);
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		ch_1_out_1 = MAX(ch_1_out_1, activation_min);
		ch_1_out_1 = MIN(ch_1_out_1, activation_max);
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		/* skip row */
		ip_a0 += num_col_a;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

q7_t* mat_mult_kernel_s8_s16_reordered_ch16_fpreq(const q7_t *input_a,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
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

		uint16_t col_count = num_col_a / 8;
		/* accumulate over the vector */
		q31_t a01, a02, a11, a12;
		q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
		q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

		//4
		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//8
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//12
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//16
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		ch_0_out_0 = MAX(ch_0_out_0, activation_min);
		ch_0_out_0 = MIN(ch_0_out_0, activation_max);
		*out_0++ = (q7_t) ch_0_out_0;

		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		ch_0_out_1 = MAX(ch_0_out_1, activation_min);
		ch_0_out_1 = MIN(ch_0_out_1, activation_max);
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		ch_1_out_0 = MAX(ch_1_out_0, activation_min);
		ch_1_out_0 = MIN(ch_1_out_0, activation_max);
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		ch_1_out_1 = MAX(ch_1_out_1, activation_min);
		ch_1_out_1 = MIN(ch_1_out_1, activation_max);
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		/* skip row */
		ip_a0 += num_col_a;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

q7_t* mat_mult_kernel_s8_s16_reordered_ch8_fpreq(const q7_t *input_a,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
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

		uint16_t col_count = num_col_a / 8;
		/* accumulate over the vector */
		q31_t a01, a02, a11, a12;
		q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
		q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

		//4
		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//8
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		ch_0_out_0 = MAX(ch_0_out_0, activation_min);
		ch_0_out_0 = MIN(ch_0_out_0, activation_max);
		*out_0++ = (q7_t) ch_0_out_0;

		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		ch_0_out_1 = MAX(ch_0_out_1, activation_min);
		ch_0_out_1 = MIN(ch_0_out_1, activation_max);
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		ch_1_out_0 = MAX(ch_1_out_0, activation_min);
		ch_1_out_0 = MIN(ch_1_out_0, activation_max);
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		ch_1_out_1 = MAX(ch_1_out_1, activation_min);
		ch_1_out_1 = MIN(ch_1_out_1, activation_max);
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		/* skip row */
		ip_a0 += num_col_a;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

q7_t* mat_mult_kernel_s8_s16_reordered_ch24_fpreq(const q7_t *input_a,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
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

		uint16_t col_count = num_col_a / 8;
		/* accumulate over the vector */
		q31_t a01, a02, a11, a12;
		q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
		q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

		//4
		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//8
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//12
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//16
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//20
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		//24
		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

		ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
		ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
		ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

		b1 = arm_nn_read_q15x2_ia(&ip_b1);

		ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
		ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
		ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
		b0 = arm_nn_read_q15x2_ia(&ip_b0);
		ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		ch_0_out_0 = MAX(ch_0_out_0, activation_min);
		ch_0_out_0 = MIN(ch_0_out_0, activation_max);
		*out_0++ = (q7_t) ch_0_out_0;

		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		ch_0_out_1 = MAX(ch_0_out_1, activation_min);
		ch_0_out_1 = MIN(ch_0_out_1, activation_max);
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		ch_1_out_0 = MAX(ch_1_out_0, activation_min);
		ch_1_out_0 = MIN(ch_1_out_0, activation_max);
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		ch_1_out_1 = MAX(ch_1_out_1, activation_min);
		ch_1_out_1 = MIN(ch_1_out_1, activation_max);
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		/* skip row */
		ip_a0 += num_col_a;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

q7_t* mat_mult_kernel_s8_s16_reordered_fpreq(const q7_t *input_a,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
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
			q31_t a01, a02, a11, a12;
			q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
			q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

			ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
			ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
			ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
			b0 = arm_nn_read_q15x2_ia(&ip_b0);
			ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

			b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
			ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
			ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

			col_count--;
		} /* while over col_count */

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		ch_0_out_0 = MAX(ch_0_out_0, activation_min);
		ch_0_out_0 = MIN(ch_0_out_0, activation_max);
		*out_0++ = (q7_t) ch_0_out_0;

		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		ch_0_out_1 = MAX(ch_0_out_1, activation_min);
		ch_0_out_1 = MIN(ch_0_out_1, activation_max);
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		ch_1_out_0 = MAX(ch_1_out_0, activation_min);
		ch_1_out_0 = MIN(ch_1_out_0, activation_max);
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		ch_1_out_1 = MAX(ch_1_out_1, activation_min);
		ch_1_out_1 = MIN(ch_1_out_1, activation_max);
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		/* skip row */
		ip_a0 += num_col_a;
		row_count--;
	}

	if (output_ch & 1) {
		/* setup pointers for B */
		const q15_t *ip_b0 = input_b;
		const q15_t *ip_b1 = ip_b0 + num_col_a;

		/* Init accumulator with bias for channel N + 1 */
		q31_t ch_0_out_0 = *bias;
		q31_t ch_0_out_1 = ch_0_out_0;

		int32_t col_count = num_col_a / 4;
		while (col_count) {
			q31_t a01, a02;
			q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
			q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

			ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
			ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);

			b0 = arm_nn_read_q15x2_ia(&ip_b0);
			b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
			ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);

			col_count--;
		} /* while over col_count */

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * *scales);
		ch_0_out_0 += out_offset;
		ch_0_out_0 = MAX(ch_0_out_0, activation_min);
		ch_0_out_0 = MIN(ch_0_out_0, activation_max);
		*out_0++ = (q7_t) ch_0_out_0;

		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * *scales);
		ch_0_out_1 += out_offset;
		ch_0_out_1 = MAX(ch_0_out_1, activation_min);
		ch_0_out_1 = MIN(ch_0_out_1, activation_max);
		*out_1++ = (q7_t) ch_0_out_1;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

q7_t* mat_mult_kernel_s8_s16_reordered_fpreq_bitmask(const q7_t *input_a,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
		const int16_t activation_max, const uint16_t num_col_a,
		const int32_t *const output_bias, q7_t *out_0, q7_t *mask) {
	/* set up the second output pointers */
	q7_t *out_1 = out_0 + output_ch;
	q7_t *mask1 = mask + output_ch / 8;
	const int32_t *bias = output_bias;

	uint16_t row_count = output_ch / 2;
	const q7_t *ip_a0 = input_a;
	int bit_starting_idx = 0;
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
			q31_t a01, a02, a11, a12;
			q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
			q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

			ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
			ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
			ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
			b0 = arm_nn_read_q15x2_ia(&ip_b0);
			ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

			b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
			ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
			ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

			col_count--;
		} /* while over col_count */

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		q7_t mask_value = 1;
		if (ch_0_out_0 < activation_min){
			ch_0_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_0 > activation_max){
			ch_0_out_0 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask, bit_starting_idx);
		else
			BIT_CLEAR(*mask, bit_starting_idx);
		*out_0++ = (q7_t) ch_0_out_0;


		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		mask_value = 1;
		if (ch_0_out_1 < activation_min){
			ch_0_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_1 > activation_max){
			ch_0_out_1 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask1, bit_starting_idx);
		else
			BIT_CLEAR(*mask1, bit_starting_idx);
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		mask_value = 1;
		if (ch_1_out_0 < activation_min){
			ch_1_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_0 > activation_max){
			ch_1_out_0 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask, bit_starting_idx+1);
		else
			BIT_CLEAR(*mask, bit_starting_idx+1);
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		mask_value = 1;
		if (ch_1_out_1 < activation_min){
			ch_1_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_1 > activation_max){
			ch_1_out_1 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask1, bit_starting_idx+1);
		else
			BIT_CLEAR(*mask1, bit_starting_idx+1);
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		bit_starting_idx += 2;
		if(bit_starting_idx == 8){
			bit_starting_idx = 0;
			mask++;
			mask1++;
		}

		/* skip row */
		ip_a0 += num_col_a;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

q7_t* mat_mult_kernel_s8_s16_reordered_fpreq_mask(const q7_t *input_a,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
		const int16_t activation_max, const uint16_t num_col_a,
		const int32_t *const output_bias, q7_t *out_0, q7_t *mask) {
	/* set up the second output pointers */
	q7_t *out_1 = out_0 + output_ch;
	q7_t *mask1 = mask + output_ch;
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
			q31_t a01, a02, a11, a12;
			q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
			q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

			ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
			ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);
			ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
			b0 = arm_nn_read_q15x2_ia(&ip_b0);
			ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

			b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
			ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
			ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

			col_count--;
		} /* while over col_count */

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		q7_t mask_value = 1;
		if (ch_0_out_0 < activation_min){
			ch_0_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_0 > activation_max){
			ch_0_out_0 = activation_max;
			mask_value = 0;
		}
		*mask++ = (q7_t) mask_value;
		*out_0++ = (q7_t) ch_0_out_0;


		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		mask_value = 1;
		if (ch_0_out_1 < activation_min){
			ch_0_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_1 > activation_max){
			ch_0_out_1 = activation_max;
			mask_value = 0;
		}
		*mask1++ = (q7_t) mask_value;
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		mask_value = 1;
		if (ch_1_out_0 < activation_min){
			ch_1_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_0 > activation_max){
			ch_1_out_0 = activation_max;
			mask_value = 0;
		}
		*mask++ = (q7_t) mask_value;
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		mask_value = 1;
		if (ch_1_out_1 < activation_min){
			ch_1_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_1 > activation_max){
			ch_1_out_1 = activation_max;
			mask_value = 0;
		}
		*mask1++ = (q7_t) mask_value;
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		/* skip row */
		ip_a0 += num_col_a;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

// Note: this is intend for 1x1, others need to be further tested
q7_t* mat_mult_kernel_s8_s16_reordered_fpreq_mask_partialCH(const q7_t *kernel_sram,
		const q7_t *kernel_flash, const uint16_t first_k_channel,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
		const int16_t activation_max, const uint16_t num_col_a,
		const int32_t *const output_bias, q7_t *out_0, q7_t *mask) {
	/* set up the second output pointers */
	q7_t *out_1 = out_0 + output_ch;
	q7_t *mask1 = mask + output_ch;
	const int32_t *bias = output_bias;

	uint16_t row_count = output_ch / 2;
	const q7_t *ip_a0_sram = kernel_sram;
	const uint16_t num_col_sram = first_k_channel;
	const q7_t *ip_a0_flash = kernel_flash;
	const uint16_t num_col_flash = num_col_a - first_k_channel;
	/* this loop over rows in A */
	while (row_count) {
		/* setup pointers for B */
		const q15_t *ip_b0 = input_b;
		const q15_t *ip_b1 = ip_b0 + num_col_a;

		/* align the second pointer for A */
		const q7_t *ip_a1_sram = ip_a0_sram + num_col_sram;
		const q7_t *ip_a1_flash = ip_a0_flash + num_col_flash;
		const float scale_0 = scales[0];
		const float scale_1 = scales[1];

		/* Init accumulator with bias for channel N and N + 1 */
		q31_t ch_0_out_0 = *bias;
		q31_t ch_0_out_1 = *bias++;
		q31_t ch_1_out_0 = *bias;
		q31_t ch_1_out_1 = *bias++;

		// kernels in SRAM
		uint16_t col_count = num_col_sram / 4;
		/* accumulate over the vector */
		while (col_count) {
			q31_t a01, a02, a11, a12;
			q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
			q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ip_a0_sram = read_and_pad_reordered(ip_a0_sram, &a01, &a02);

			ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
			ip_a1_sram = read_and_pad_reordered(ip_a1_sram, &a11, &a12);
			ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
			b0 = arm_nn_read_q15x2_ia(&ip_b0);
			ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

			b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
			ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
			ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

			col_count--;
		} /* while over col_count */

		// kernels in Flash
		col_count = num_col_flash / 4;
		/* accumulate over the vector */
		while (col_count) {
			q31_t a01, a02, a11, a12;
			q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
			q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ip_a0_flash = read_and_pad_reordered(ip_a0_flash, &a01, &a02);

			ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
			ip_a1_flash = read_and_pad_reordered(ip_a1_flash, &a11, &a12);
			ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
			b0 = arm_nn_read_q15x2_ia(&ip_b0);
			ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

			b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
			ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
			ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

			col_count--;
		} /* while over col_count */

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		q7_t mask_value = 1;
		if (ch_0_out_0 < activation_min){
			ch_0_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_0 > activation_max){
			ch_0_out_0 = activation_max;
			mask_value = 0;
		}
		*mask++ = (q7_t) mask_value;
		*out_0++ = (q7_t) ch_0_out_0;


		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		mask_value = 1;
		if (ch_0_out_1 < activation_min){
			ch_0_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_1 > activation_max){
			ch_0_out_1 = activation_max;
			mask_value = 0;
		}
		*mask1++ = (q7_t) mask_value;
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		mask_value = 1;
		if (ch_1_out_0 < activation_min){
			ch_1_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_0 > activation_max){
			ch_1_out_0 = activation_max;
			mask_value = 0;
		}
		*mask++ = (q7_t) mask_value;
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		mask_value = 1;
		if (ch_1_out_1 < activation_min){
			ch_1_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_1 > activation_max){
			ch_1_out_1 = activation_max;
			mask_value = 0;
		}
		*mask1++ = (q7_t) mask_value;
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		/* skip row */
		ip_a0_sram += num_col_sram;
		ip_a0_flash += num_col_flash;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

// Note: this is intend for 1x1, others need to be further tested
q7_t* mat_mult_kernel_s8_s16_reordered_fpreq_bitmask_partialCH(const q7_t *kernel_sram,
		const q7_t *kernel_flash, const uint16_t first_k_channel,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
		const int16_t activation_max, const uint16_t num_col_a,
		const int32_t *const output_bias, q7_t *out_0, q7_t *mask) {
	/* set up the second output pointers */
	q7_t *out_1 = out_0 + output_ch;
	q7_t *mask1 = mask + output_ch / 8;
	const int32_t *bias = output_bias;

	uint16_t row_count = output_ch / 2;
	const q7_t *ip_a0_sram = kernel_sram;
	const uint16_t num_col_sram = first_k_channel;
	const q7_t *ip_a0_flash = kernel_flash;
	const uint16_t num_col_flash = num_col_a - first_k_channel;
	int bit_starting_idx = 0;
	/* this loop over rows in A */
	while (row_count) {
		/* setup pointers for B */
		const q15_t *ip_b0 = input_b;
		const q15_t *ip_b1 = ip_b0 + num_col_a;

		/* align the second pointer for A */
		const q7_t *ip_a1_sram = ip_a0_sram + num_col_sram;
		const q7_t *ip_a1_flash = ip_a0_flash + num_col_flash;
		const float scale_0 = scales[0];
		const float scale_1 = scales[1];

		/* Init accumulator with bias for channel N and N + 1 */
		q31_t ch_0_out_0 = *bias;
		q31_t ch_0_out_1 = *bias++;
		q31_t ch_1_out_0 = *bias;
		q31_t ch_1_out_1 = *bias++;

		// kernels in SRAM
		uint16_t col_count = num_col_sram / 4;
		/* accumulate over the vector */
		while (col_count) {
			q31_t a01, a02, a11, a12;
			q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
			q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ip_a0_sram = read_and_pad_reordered(ip_a0_sram, &a01, &a02);

			ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
			ip_a1_sram = read_and_pad_reordered(ip_a1_sram, &a11, &a12);
			ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
			b0 = arm_nn_read_q15x2_ia(&ip_b0);
			ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

			b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
			ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
			ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

			col_count--;
		} /* while over col_count */

		// kernels in Flash
		col_count = num_col_flash / 4;
		/* accumulate over the vector */
		while (col_count) {
			q31_t a01, a02, a11, a12;
			q31_t b0 = arm_nn_read_q15x2_ia(&ip_b0);
			q31_t b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ip_a0_flash = read_and_pad_reordered(ip_a0_flash, &a01, &a02);

			ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
			ip_a1_flash = read_and_pad_reordered(ip_a1_flash, &a11, &a12);
			ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
			b0 = arm_nn_read_q15x2_ia(&ip_b0);
			ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

			b1 = arm_nn_read_q15x2_ia(&ip_b1);

			ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
			ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
			ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
			ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);

			col_count--;
		} /* while over col_count */

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		q7_t mask_value = 1;
		if (ch_0_out_0 < activation_min){
			ch_0_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_0 > activation_max){
			ch_0_out_0 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask, bit_starting_idx);
		else
			BIT_CLEAR(*mask, bit_starting_idx);
		*out_0++ = (q7_t) ch_0_out_0;


		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		mask_value = 1;
		if (ch_0_out_1 < activation_min){
			ch_0_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_1 > activation_max){
			ch_0_out_1 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask1, bit_starting_idx);
		else
			BIT_CLEAR(*mask1, bit_starting_idx);
		*out_1++ = (q7_t) ch_0_out_1;


		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		mask_value = 1;
		if (ch_1_out_0 < activation_min){
			ch_1_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_0 > activation_max){
			ch_1_out_0 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask, bit_starting_idx+1);
		else
			BIT_CLEAR(*mask, bit_starting_idx+1);
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		mask_value = 1;
		if (ch_1_out_1 < activation_min){
			ch_1_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_1 > activation_max){
			ch_1_out_1 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask1, bit_starting_idx+1);
		else
			BIT_CLEAR(*mask1, bit_starting_idx+1);
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		bit_starting_idx += 2;
		if(bit_starting_idx == 8){
			bit_starting_idx = 0;
			mask++;
			mask1++;
		}

		/* skip row */
		ip_a0_sram += num_col_sram;
		ip_a0_flash += num_col_flash;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}


// Note: this is intend for 1x1, others need to be further tested
q7_t* mat_mult_kernel_s8_s16_reordered_fpreq_mask_partialCH_Multiple2(const q7_t *kernel_sram,
		const q7_t *kernel_flash, const uint16_t first_k_channel,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
		const int16_t activation_max, const uint16_t num_col_a,
		const int32_t *const output_bias, q7_t *out_0, q7_t *mask) {
	/* set up the second output pointers */
	q7_t *out_1 = out_0 + output_ch;
	q7_t *mask1 = mask + output_ch;
	const int32_t *bias = output_bias;

	uint16_t row_count = output_ch / 2;
	const q7_t *ip_a0_sram = kernel_sram;
	const uint16_t num_col_sram = first_k_channel;
	const q7_t *ip_a0_flash = kernel_flash;
	const uint16_t num_col_flash = num_col_a - first_k_channel;
	/* this loop over rows in A */
	while (row_count) {
		/* setup pointers for B */
		const q15_t *ip_b0 = input_b;
		const q15_t *ip_b1 = ip_b0 + num_col_a;

		/* align the second pointer for A */
		const q7_t *ip_a1_sram = ip_a0_sram + num_col_sram;
		const q7_t *ip_a1_flash = ip_a0_flash + num_col_flash;
		const float scale_0 = scales[0];
		const float scale_1 = scales[1];

		/* Init accumulator with bias for channel N and N + 1 */
		q31_t ch_0_out_0 = *bias;
		q31_t ch_0_out_1 = *bias++;
		q31_t ch_1_out_0 = *bias;
		q31_t ch_1_out_1 = *bias++;

		// kernels in SRAM
		uint16_t col_count = num_col_sram / 2;
		/* accumulate over the vector */
		while (col_count) {
			ch_0_out_0 += ip_a0_sram[0] * ip_b0[0];
			ch_0_out_0 += ip_a0_sram[1] * ip_b0[1];
			ch_0_out_1 += ip_a0_sram[0] * ip_b1[0];
			ch_0_out_1 += ip_a0_sram[1] * ip_b1[1];

			ch_1_out_0 += ip_a1_sram[0] * ip_b0[0];
			ch_1_out_0 += ip_a1_sram[1] * ip_b0[1];
			ch_1_out_1 += ip_a1_sram[0] * ip_b1[0];
			ch_1_out_1 += ip_a1_sram[1] * ip_b1[1];

			ip_a0_sram += 2;
			ip_a1_sram += 2;
			ip_b0 += 2;
			ip_b1 += 2;

			col_count--;
		} /* while over col_count */

		// kernels in Flash
		col_count = num_col_flash / 2;
		/* accumulate over the vector */
		while (col_count) {
			ch_0_out_0 += ip_a0_flash[0] * ip_b0[0];
			ch_0_out_0 += ip_a0_flash[1] * ip_b0[1];
			ch_0_out_1 += ip_a0_flash[0] * ip_b1[0];
			ch_0_out_1 += ip_a0_flash[1] * ip_b1[1];

			ch_1_out_0 += ip_a1_flash[0] * ip_b0[0];
			ch_1_out_0 += ip_a1_flash[1] * ip_b0[1];
			ch_1_out_1 += ip_a1_flash[0] * ip_b1[0];
			ch_1_out_1 += ip_a1_flash[1] * ip_b1[1];

			ip_a0_flash += 2;
			ip_a1_flash += 2;
			ip_b0 += 2;
			ip_b1 += 2;

			col_count--;
		} /* while over col_count */


		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		q7_t mask_value = 1;
		if (ch_0_out_0 < activation_min){
			ch_0_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_0 > activation_max){
			ch_0_out_0 = activation_max;
			mask_value = 0;
		}
		*mask++ = (q7_t) mask_value;
		*out_0++ = (q7_t) ch_0_out_0;


		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		mask_value = 1;
		if (ch_0_out_1 < activation_min){
			ch_0_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_1 > activation_max){
			ch_0_out_1 = activation_max;
			mask_value = 0;
		}
		*mask1++ = (q7_t) mask_value;
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		mask_value = 1;
		if (ch_1_out_0 < activation_min){
			ch_1_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_0 > activation_max){
			ch_1_out_0 = activation_max;
			mask_value = 0;
		}
		*mask++ = (q7_t) mask_value;
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		mask_value = 1;
		if (ch_1_out_1 < activation_min){
			ch_1_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_1 > activation_max){
			ch_1_out_1 = activation_max;
			mask_value = 0;
		}
		*mask1++ = (q7_t) mask_value;
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		/* skip row */
		ip_a0_sram += num_col_sram;
		ip_a0_flash += num_col_flash;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

// Note: this is intend for 1x1, others need to be further tested
q7_t* mat_mult_kernel_s8_s16_reordered_fpreq_bitmask_partialCH_Multiple2(const q7_t *kernel_sram,
		const q7_t *kernel_flash, const uint16_t first_k_channel,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
		const int16_t activation_max, const uint16_t num_col_a,
		const int32_t *const output_bias, q7_t *out_0, q7_t *mask) {
	/* set up the second output pointers */
	q7_t *out_1 = out_0 + output_ch;
	q7_t *mask1 = mask + output_ch / 8;
	const int32_t *bias = output_bias;

	uint16_t row_count = output_ch / 2;
	const q7_t *ip_a0_sram = kernel_sram;
	const uint16_t num_col_sram = first_k_channel;
	const q7_t *ip_a0_flash = kernel_flash;
	const uint16_t num_col_flash = num_col_a - first_k_channel;
	int bit_starting_idx = 0;
	/* this loop over rows in A */
	while (row_count) {
		/* setup pointers for B */
		const q15_t *ip_b0 = input_b;
		const q15_t *ip_b1 = ip_b0 + num_col_a;

		/* align the second pointer for A */
		const q7_t *ip_a1_sram = ip_a0_sram + num_col_sram;
		const q7_t *ip_a1_flash = ip_a0_flash + num_col_flash;
		const float scale_0 = scales[0];
		const float scale_1 = scales[1];

		/* Init accumulator with bias for channel N and N + 1 */
		q31_t ch_0_out_0 = *bias;
		q31_t ch_0_out_1 = *bias++;
		q31_t ch_1_out_0 = *bias;
		q31_t ch_1_out_1 = *bias++;

		// kernels in SRAM
		uint16_t col_count = num_col_sram / 2;
		/* accumulate over the vector */
		while (col_count) {
			ch_0_out_0 += ip_a0_sram[0] * ip_b0[0];
			ch_0_out_0 += ip_a0_sram[1] * ip_b0[1];
			ch_0_out_1 += ip_a0_sram[0] * ip_b1[0];
			ch_0_out_1 += ip_a0_sram[1] * ip_b1[1];

			ch_1_out_0 += ip_a1_sram[0] * ip_b0[0];
			ch_1_out_0 += ip_a1_sram[1] * ip_b0[1];
			ch_1_out_1 += ip_a1_sram[0] * ip_b1[0];
			ch_1_out_1 += ip_a1_sram[1] * ip_b1[1];

			ip_a0_sram += 2;
			ip_a1_sram += 2;
			ip_b0 += 2;
			ip_b1 += 2;

			col_count--;
		} /* while over col_count */

		// kernels in Flash
		col_count = num_col_flash / 2;
		/* accumulate over the vector */
		while (col_count) {
			ch_0_out_0 += ip_a0_flash[0] * ip_b0[0];
			ch_0_out_0 += ip_a0_flash[1] * ip_b0[1];
			ch_0_out_1 += ip_a0_flash[0] * ip_b1[0];
			ch_0_out_1 += ip_a0_flash[1] * ip_b1[1];

			ch_1_out_0 += ip_a1_flash[0] * ip_b0[0];
			ch_1_out_0 += ip_a1_flash[1] * ip_b0[1];
			ch_1_out_1 += ip_a1_flash[0] * ip_b1[0];
			ch_1_out_1 += ip_a1_flash[1] * ip_b1[1];

			ip_a0_flash += 2;
			ip_a1_flash += 2;
			ip_b0 += 2;
			ip_b1 += 2;

			col_count--;
		} /* while over col_count */


		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		q7_t mask_value = 1;
		if (ch_0_out_0 < activation_min){
			ch_0_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_0 > activation_max){
			ch_0_out_0 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask, bit_starting_idx);
		else
			BIT_CLEAR(*mask, bit_starting_idx);
		*out_0++ = (q7_t) ch_0_out_0;



		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		mask_value = 1;
		if (ch_0_out_1 < activation_min){
			ch_0_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_0_out_1 > activation_max){
			ch_0_out_1 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask1, bit_starting_idx);
		else
			BIT_CLEAR(*mask1, bit_starting_idx);
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		mask_value = 1;
		if (ch_1_out_0 < activation_min){
			ch_1_out_0 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_0 > activation_max){
			ch_1_out_0 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask, bit_starting_idx+1);
		else
			BIT_CLEAR(*mask, bit_starting_idx+1);
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		mask_value = 1;
		if (ch_1_out_1 < activation_min){
			ch_1_out_1 = activation_min;
			mask_value = 0;
		}
		if (ch_1_out_1 > activation_max){
			ch_1_out_1 = activation_max;
			mask_value = 0;
		}
		if (mask_value == 1)
			BIT_SET(*mask1, bit_starting_idx+1);
		else
			BIT_CLEAR(*mask1, bit_starting_idx+1);
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		bit_starting_idx += 2;
		if(bit_starting_idx == 8){
			bit_starting_idx = 0;
			mask++;
			mask1++;
		}

		/* skip row */
		ip_a0_sram += num_col_sram;
		ip_a0_flash += num_col_flash;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}


q7_t* mat_mult_kernel3_input3_s8_s16_fpreq(const q7_t *input_a,
		const q15_t *input_b, const uint16_t output_ch, const float *scales,
		const int32_t out_offset, const int16_t activation_min,
		const int16_t activation_max, const uint16_t num_col_a,
		const int32_t *const output_bias, q7_t *out_0, q15_t *kbuf) {
	/* set up the second output pointers */
	q7_t *out_1 = out_0 + output_ch;
	const int32_t *bias = output_bias;

	uint16_t row_count = output_ch / 2;
	const q15_t *ksrc = &kbuf[0];
	/* this loop over rows in A */
	while (row_count) {
		/* setup pointers for B */
		const q15_t *ip_b0 = input_b;
		const q15_t *ip_b1 = ip_b0 + num_col_a;
		const q31_t *ip31_b0 = ip_b0;
		const q31_t *ip31_b1 = ip_b1;
		const float scale_0 = scales[0];
		const float scale_1 = scales[1];

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

		ch_0_out_0 = (q31_t) ((float) ch_0_out_0 * scale_0);
		ch_0_out_0 += out_offset;
		ch_0_out_0 = MAX(ch_0_out_0, activation_min);
		ch_0_out_0 = MIN(ch_0_out_0, activation_max);
		*out_0++ = (q7_t) ch_0_out_0;

		ch_0_out_1 = (q31_t) ((float) ch_0_out_1 * scale_0);
		ch_0_out_1 += out_offset;
		ch_0_out_1 = MAX(ch_0_out_1, activation_min);
		ch_0_out_1 = MIN(ch_0_out_1, activation_max);
		*out_1++ = (q7_t) ch_0_out_1;

		ch_1_out_0 = (q31_t) ((float) ch_1_out_0 * scale_1);
		ch_1_out_0 += out_offset;
		ch_1_out_0 = MAX(ch_1_out_0, activation_min);
		ch_1_out_0 = MIN(ch_1_out_0, activation_max);
		*out_0++ = (q7_t) ch_1_out_0;

		ch_1_out_1 = (q31_t) ((float) ch_1_out_1 * scale_1);
		ch_1_out_1 += out_offset;
		ch_1_out_1 = MAX(ch_1_out_1, activation_min);
		ch_1_out_1 = MIN(ch_1_out_1, activation_max);
		*out_1++ = (q7_t) ch_1_out_1;
		scales += 2;

		/* skip row */
		ksrc += 54;
		row_count--;
	}

	out_0 += output_ch;

	/* return the new output pointer with offset */
	return out_0;
}

