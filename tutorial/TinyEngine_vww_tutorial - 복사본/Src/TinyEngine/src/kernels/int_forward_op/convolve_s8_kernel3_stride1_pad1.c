/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   convolve_s8_kernel3_stride1_pad1.c
 * Description:  for 3x3 convolution with kernels, typically for image processing
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

tinyengine_status convolve_s8_kernel3_stride1_pad1(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
		const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
		const int32_t *output_shift, const int32_t *output_mult,
		const int32_t output_offset, const int32_t input_offset,
		const int32_t output_activation_min,
		const int32_t output_activation_max, q7_t *output,
		const uint16_t output_x, const uint16_t output_y,
		const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value) {
	if (input_ch % 4 != 0) {
		return PARAM_NO_SUPPORT;
	}

	int32_t i_element;
	(void) input_x;
	(void) input_y;

	/* Partial(two columns) im2col buffer */
	q15_t *two_column_buffer = runtime_buf;
	q7_t *out = output;
	const int channel_div4 = (input_ch >> 2);

	const int16_t inoff16 = input_offset;
	q31_t offset_q15x2 = __PKHBT(inoff16, inoff16, 16);
	q31_t pad_q15x2 = __PKHBT(pad_value, pad_value, 16);
	q31_t pad_out_q15x2 = __SADD16(pad_q15x2, offset_q15x2);
	int in_row_offset = input_ch * input_x;

	for (int i_out_y = 0; i_out_y < output_y; i_out_y++) {
		const int16_t base_idx_y = i_out_y - 1;
		for (int i_out_x = 0; i_out_x < output_x; i_out_x++) {
			const int16_t base_idx_x = i_out_x - 1;
			//Img2col for 3x3 kernel
			/* Used for SIMD instructions  */
			q31_t in_q7x4;
			q31_t in_q15x2_1;
			q31_t in_q15x2_2;
			q31_t out_q15x2_1;
			q31_t out_q15x2_2;
			int block_cnt;
			q15_t *col_buffer = &two_column_buffer[0];

			//TODO: swap these two if statement out to reduce overhead
			int ypad_cnt = 0; //no pad by default
			if (base_idx_y == -1) { //pad the first row
				q31_t *dst_31 = (q31_t*) &col_buffer[0];
				int block_cnt = channel_div4;//unroll by 2, 3 element
				while (block_cnt > 0) {//total: 16bit * input_ch * 3
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					block_cnt--;
				}
				ypad_cnt = 1;
			}
			else if (base_idx_y + 2 == input_y) { //pad the third row
				q31_t *dst_31 = (q31_t*) &col_buffer[input_ch * 6];
				int block_cnt = channel_div4;//unroll by 2, 3 element
				while (block_cnt > 0) {//total: 16bit * input_ch * 3
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					*dst_31++ = pad_out_q15x2;
					block_cnt--;
				}
				ypad_cnt = 2;
			}

			if (ypad_cnt == 0){ //filled all rows
				if (base_idx_x == -1) {
					/* use pad for the first 1 col */
					q31_t *dst_31 = (q31_t*) &col_buffer[0];
					q31_t *dst2_31 = (q31_t*) &col_buffer[input_ch * 3];
					q31_t *dst3_31 = (q31_t*) &col_buffer[input_ch * 6];

					pad_3row_1col(dst_31, dst2_31, dst3_31, pad_out_q15x2)

					/* load input to 2 col*/
					const q7_t *src = input + base_idx_y * input_x * input_ch;
					const q7_t *src2 = src + in_row_offset;
					const q7_t *src3 = src2 + in_row_offset;
					q15_t *dst = dst_31;
					q15_t *dst2 = dst2_31;
					q15_t *dst3 = dst3_31;

					load_3row_2col(src, src2, src3, dst, dst2, dst3)
				} else if (base_idx_x + 2 == input_x) {
					/* load 2 col */
					const q7_t *src = input	+ (base_idx_y * input_x + base_idx_x) * input_ch;
					const q7_t *src2 = src + in_row_offset;
					const q7_t *src3 = src2 + in_row_offset;
					q15_t *dst = (q31_t*) &col_buffer[0];;
					q15_t *dst2 = (q31_t*) &col_buffer[input_ch * 3];
					q15_t *dst3 = (q31_t*) &col_buffer[input_ch * 6];;

					load_3row_2col(src, src2, src3, dst, dst2, dst3)

					q31_t *dst_31 = (q31_t*) dst;
					q31_t *dst2_31 = (q31_t*) dst2;
					q31_t *dst3_31 = (q31_t*) dst3;

					/* use pad for the last 1 col*/
					pad_3row_1col(dst_31,dst2_31,dst3_31,pad_out_q15x2)
				} else {
					/* load 3 col */
					const q7_t *src = input	+ (base_idx_y * input_x + base_idx_x) * input_ch;
					const q7_t *src2 = src + in_row_offset;
					const q7_t *src3 = src2 + in_row_offset;
					q15_t *dst = (q31_t*) &col_buffer[0];;
					q15_t *dst2 = (q31_t*) &col_buffer[input_ch * 3];
					q15_t *dst3 = (q31_t*) &col_buffer[input_ch * 6];;

					load_3row_3col(src, src2, src3, dst, dst2, dst3)
				}
			}
			else if (ypad_cnt == 1){//filled the last two rows
				if (base_idx_x == -1){
					/* use pad for the first 1 col */
					q31_t *dst_31 = &col_buffer[input_ch * 3];
					q31_t *dst2_31 = &col_buffer[input_ch * 6];
					pad_2row_1col(dst_31, dst2_31, pad_out_q15x2)

					/* load input to 2 col*/
					const q7_t *src = input + 0;
					const q7_t *src2 = src + in_row_offset;
					q15_t *dst = dst_31;
					q15_t *dst2 = dst2_31;

					load_2row_2col(src, src2, dst, dst2)
				} else if (base_idx_x + 2 == input_x) {
					/* load 2 col*/
					q31_t *dst = &col_buffer[input_ch * 3];
					q31_t *dst2 = &col_buffer[input_ch * 6];
					const q7_t *src = input + base_idx_x * input_ch;
					const q7_t *src2 = src + in_row_offset;

					load_2row_2col(src, src2, dst, dst2)
					q31_t *dst_31 = (q31_t*) dst;
					q31_t *dst2_31 = (q31_t*) dst2;

					/* use pad for the last 1 col*/
					pad_2row_1col(dst_31,dst2_31,pad_out_q15x2)
				}
				else {
					/* load 3 col*/
					q15_t *dst = &col_buffer[input_ch * 3];
					q15_t *dst2 = &col_buffer[input_ch * 6];
					const q7_t *src = input + base_idx_x * input_ch;
					const q7_t *src2 = src + in_row_offset;

					load_2row_3col(src, src2, dst, dst2)
				}
			} else{ //filled the first two rows
				if (base_idx_x == -1) {
					/* use pad for the first 1 col*/
					q31_t *dst_31 = (q31_t*) &col_buffer[0];
					q31_t *dst2_31 = (q31_t*) &col_buffer[input_ch * 3];

					pad_2row_1col(dst_31, dst2_31, pad_out_q15x2)

					/* load input to 2 col*/
					const q7_t *src = input + (base_idx_y * input_x) * input_ch;
					const q7_t *src2 = src + in_row_offset;
					q15_t *dst = dst_31;
					q15_t *dst2 = dst2_31;

					load_2row_2col(src, src2, dst, dst2)
				} else if (base_idx_x + 2 == input_x) {
					/* load 2 col*/
					q15_t *dst = &col_buffer[input_ch * 0];
					q15_t *dst2  = &col_buffer[input_ch * 3];
					const q7_t *src = input	+ (base_idx_y * input_x + base_idx_x) * input_ch;
					const q7_t *src2 = src + in_row_offset;

					load_2row_2col(src, src2, dst, dst2)

					/* use pad for the last 1 col*/
					q31_t *dst_31 = (q31_t*) dst;
					q31_t *dst2_31 = (q31_t*) dst2;

					pad_2row_1col(dst_31,dst2_31,pad_out_q15x2)
				} else {
					/* load 3 col*/
					q15_t *dst = &col_buffer[input_ch * 0];
					q15_t *dst2  = &col_buffer[input_ch * 3];
					/* load input to 1 col*/
					const q7_t *src = input	+ (base_idx_y * input_x + base_idx_x) * input_ch;
					const q7_t *src2 = src + in_row_offset;

					load_2row_3col(src, src2, dst, dst2)
				}
			}

			two_column_buffer += input_ch * 9;

			/* Computation is filed for every 2 columns */
			if (two_column_buffer == runtime_buf + 2 * input_ch * 9)
			{
				out = mat_mult_kernel_s8_s16(kernel,
										  runtime_buf,
										  output_ch,
										  output_shift,
										  output_mult,
										  output_offset,
										  output_activation_min,
										  output_activation_max,
										  input_ch * 9,
										  bias,
										  out);
				/* counter reset */
				two_column_buffer = runtime_buf;
			}
		}
	}

	/* left-over because odd number of output pixels */
	if (two_column_buffer != runtime_buf)
	{
		const q7_t *ker_a = kernel;
		int i;

		for (i = 0; i < output_ch; i++)
		{
			/* Load the accumulator with bias first */
			q31_t sum = bias[i];

			/* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
			const q15_t *ip_as_col = runtime_buf;

			/* 4 multiply and accumulates are done in one loop. */
			uint16_t col_count = (input_ch * 9) >> 2;

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
			col_count = input_ch * 3 * 3 & 0x3;
			while (col_count)
			{
				q7_t ker_a1 = *ker_a++;
				q15_t ip_b1 = *ip_as_col++;
				sum += ker_a1 * ip_b1;
				col_count--;
			}

			sum = arm_nn_requantize(sum, output_mult[i], output_shift[i]);
			sum += output_offset;
			sum = MAX(sum, output_activation_min);
			sum = MIN(sum, output_activation_max);
			*out++ = (q7_t)sum;
		}
	}

	/* Return to application */
	return STATE_SUCCESS;
}

/**
 * @} end of NNConv group
 */
