/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   convolve_s8_kernel3x2_inputch3_stride2_pad1.c
 * Description:  for 3x3 convolution with 3 input channels, typically for image processing
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

#include "arm_math.h"
#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"
#include "img2col_element.h"
#include "tinyengine_function.h"

tinyengine_status convolve_s8_kernel3x2_inputch3_stride2_pad1(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
		const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
		const int32_t *output_shift, const int32_t *output_mult,
		const int32_t output_offset, const int32_t input_offset,
		const int32_t output_activation_min,
		const int32_t output_activation_max, q7_t *output,
		const uint16_t output_x, const uint16_t output_y,
		const uint16_t output_ch, q15_t *runtime_buf, q15_t *kbuf, q7_t pad_value) {
	const int kernel_y = 3;
	const int kernel_x = 2;

	//check this during code gen for better performance
	if(input_x % 2 != 0 || input_y % 2 != 0){
		return PARAM_NO_SUPPORT;
	}

	int16_t i_out_y, i_out_x, i_ker_y, i_ker_x;

	/* Generate two columns from the input tensor a GEMM computation */
	q15_t *two_column_buf = runtime_buf;
	q7_t *out = output;

	q15_t pad16 = pad_value;
	const int16_t inoff16 = input_offset;
	q15_t pad_out = pad16 + inoff16;
	q31_t pad_out_q15x2 = __PKHBT(pad_out, pad_out, 16);
	q31_t offset_q15x2 = __PKHBT(inoff16, inoff16, 16);

	const q7_t *ip_a0 = kernel;

	for (int i = 0; i < output_ch; i += 2) {
		q15_t *dst1 = &kbuf[i * 18]; //each q31_t store 2 elements
		q15_t *dst2 = dst1 + 18;

		const q7_t *ip_a1 = ip_a0 + 18;

		//27 for each output_ch
		q31_t *dst1_31 = dst1;
		q31_t *dst2_31 = dst2;
		ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
		ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
		dst1_31 += 2;
		dst2_31 += 2;

		ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
		ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
		dst1_31 += 2;
		dst2_31 += 2;

		ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
		ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
		dst1_31 += 2;
		dst2_31 += 2;

		ip_a0 = read_and_pad(ip_a0, &dst1_31[0], &dst1_31[1]);
		ip_a1 = read_and_pad(ip_a1, &dst2_31[0], &dst2_31[1]);
		dst1_31 += 2;
		dst2_31 += 2;

		//17, 18
		dst1 = dst1_31;
		dst2 = dst2_31;
		dst1[0] = *ip_a0++;
		dst1[1] = *ip_a0++;
		dst2[0] = *ip_a1++;
		dst2[1] = *ip_a1++;

		/* skip row */
		ip_a0 += 27;
	}

	for (i_out_y = 0; i_out_y < output_y; i_out_y++) {
		for (i_out_x = 0; i_out_x < output_x; i_out_x++) {
			/* This part implements the im2col function */
			const int16_t base_idx_y = (i_out_y * 2) - 1;
			const int16_t base_idx_x = (i_out_x * 2) - 1;
			const q15_t *col_buffer = two_column_buf;

			//use variables
			q31_t in_q7x4;
			q31_t in_q15x2_1;
			q31_t in_q15x2_2;
			q31_t out_q15x2_1;
			q31_t out_q15x2_2;

			/* load address:8bit */
			q7_t *src;
			q7_t *src2;
			q7_t *src3;

			/* buffer for load:16bit */
			q15_t *dst;
			q15_t *dst2;
			q15_t *dst3;

			int input_row_offset = 3 * input_x;
			dst = col_buffer;
			dst2 = dst + 6;
			dst3 = dst2 + 6;
			if (base_idx_y != -1) {
				if (base_idx_x != -1) {
					//load all for now and unroll all
					//3x3 = 9 elements
					src = input	+ (base_idx_y * input_x + base_idx_x) * input_ch;
					src2 = src + input_row_offset;
					src3 = src2 + input_row_offset;

					//3 * 2 = 6
					q7_q15_offset_ele(src, dst)
					*dst++ = *src++ + input_offset;
					*dst++ = *src++ + input_offset;
					//
					q7_q15_offset_ele(src2, dst2)
					*dst2++ = *src2++ + input_offset;
					*dst2++ = *src2++ + input_offset;
					//
					q7_q15_offset_ele(src3, dst3)
					*dst3++ = *src3++ + input_offset;
					*dst3++ = *src3++ + input_offset;
				} else {
					src = input + (base_idx_y * input_x) * input_ch;
					src2 = src + input_row_offset;
					src3 = src2 + input_row_offset;

					//pad the first col: 1x3 = 3
					*dst++ = pad_out;
					*dst++ = pad_out;
					*dst++ = pad_out;
					*dst2++ = pad_out;
					*dst2++ = pad_out;
					*dst2++ = pad_out;
					*dst3++ = pad_out;
					*dst3++ = pad_out;
					*dst3++ = pad_out;
					//load 3 elements
					*dst++ = *src++ + input_offset;
					*dst++ = *src++ + input_offset;
					*dst++ = *src++ + input_offset;
					//
					*dst2++ = *src2++ + input_offset;
					*dst2++ = *src2++ + input_offset;
					*dst2++ = *src2++ + input_offset;
					//
					*dst3++ = *src3++ + input_offset;
					*dst3++ = *src3++ + input_offset;
					*dst3++ = *src3++ + input_offset;
				}
			} else {
				//Padding the first row
				//3x2 = 6 elements
				q31_t *dst_31 = dst;
				*dst_31++ = pad_out_q15x2;
				*dst_31++ = pad_out_q15x2;
				*dst_31++ = pad_out_q15x2;
				if (base_idx_x != -1) {
					//3x3 = 9 elements
					src2 = input + (base_idx_x) * input_ch;
					src3 = src2 + input_row_offset;

					//3 * 2 = 6 = 4 * 1 + 2
					q7_q15_offset_ele(src2, dst2)
					*dst2++ = *src2++ + input_offset;
					*dst2++ = *src2++ + input_offset;
					//
					q7_q15_offset_ele(src3, dst3)
					*dst3++ = *src3++ + input_offset;
					*dst3++ = *src3++ + input_offset;
				} else {
					src2 = input;
					src3 = src2 + input_row_offset;

					//pad the first one: 1x3 = 3
					*dst2++ = pad_out;
					*dst2++ = pad_out;
					*dst2++ = pad_out;
					*dst3++ = pad_out;
					*dst3++ = pad_out;
					*dst3++ = pad_out;
					//load 3 elements
					*dst2++ = *src2++ + input_offset;
					*dst2++ = *src2++ + input_offset;
					*dst2++ = *src2++ + input_offset;
					//
					*dst3++ = *src3++ + input_offset;
					*dst3++ = *src3++ + input_offset;
					*dst3++ = *src3++ + input_offset;
				}
			}

			two_column_buf += 18;
			/* Computation is filed for every 2 columns */
			if (two_column_buf == runtime_buf + 2 * 18) {

				out = mat_mult_unloop18_s8_s16(kernel,
						runtime_buf, output_ch, output_shift, output_mult,
						output_offset, output_activation_min, output_activation_max,
						input_ch * kernel_y * kernel_x, bias, out, kbuf);

				/* counter reset */
				two_column_buf = runtime_buf;
			}
		}
	}

	/* Return to application */
	return STATE_SUCCESS;
}
