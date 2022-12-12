/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   patchpadding_kbuf_convolve_s8_kernel3_inputch3_stride2.c
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

tinyengine_status patchpadding_kbuf_convolve_s8_kernel3_inputch3_stride2(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
		const uint16_t input_ch, const q7_t* kernel, const q31_t *kbuf, const int32_t *bias,
		const int32_t *output_shift, const int32_t *output_mult,
		const int32_t output_offset, const int32_t input_offset,
		const int32_t output_activation_min,
		const int32_t output_activation_max, q7_t *output,
		const uint16_t output_x, const uint16_t output_y,
		const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value,
		const uint16_t pad_t, const uint16_t pad_b, const uint16_t pad_l, const uint16_t pad_r) {
	const int kernel_y = 3;
	const int kernel_x = 3;

	int16_t i_out_y, i_out_x, i_ker_y, i_ker_x;

	/* Generate two columns from the input tensor a GEMM computation */
	q15_t *two_column_buf = runtime_buf;
	q7_t *out = output;

	q15_t pad16 = pad_value;
	const int16_t inoff16 = input_offset;

	for (i_out_y = 0; i_out_y < output_y; i_out_y++) {
		for (i_out_x = 0; i_out_x < output_x; i_out_x++) {
			/* This part implements the im2col function */
			const q15_t *col_buffer = two_column_buf;
			int16_t base_idx_y = (i_out_y * 2);
			int16_t base_idx_x = (i_out_x * 2);

			//use variables
			q31_t in_q7x4;
			q31_t in_q15x2_1;
			q31_t in_q15x2_2;
			q31_t out_q15x2_1;
			q31_t out_q15x2_2;

			/* load address:8bit */
			q7_t *src;

			/* buffer for im2col:16bit */
			q15_t *dst = col_buffer;

			int skip_top = pad_t - base_idx_y;
			int skip_bottom = MAX(0,(base_idx_y + 3) - (input_y - pad_b));//3x3

			int y_cnt = 3;//3 rows to load
			//fill zeros in the top regions
			while (y_cnt > 0 && skip_top-- > 0){
				*dst++ = 0; *dst++ = 0; *dst++ = 0;
				*dst++ = 0; *dst++ = 0; *dst++ = 0;
				*dst++ = 0; *dst++ = 0; *dst++ = 0;
				y_cnt--;
				base_idx_y++;
			}

			//fill in the middle
			int skip_left = MAX(0,pad_l - base_idx_x);
			int skip_right = MAX(0,(base_idx_x + 3) - (input_x - pad_r));//3x3
			//address of the first valid values
			int m;
			for (m = 0; m < y_cnt - skip_bottom; m++){
				src = input	+ ((base_idx_y+m) * input_x + base_idx_x + skip_left) * input_ch;
				int x_cnt = 3;//3 columns to load
				//fill zero for left regions
				int cnt = skip_left;
				while(x_cnt > 0 && cnt-- > 0){
					*dst++ = 0; *dst++ = 0; *dst++ = 0;//input_ch == 3
					x_cnt--;
				}

				//load the middle
				while(x_cnt > skip_right){
					*dst++ = *src++ + input_offset; *dst++ = *src++ + input_offset; *dst++ = *src++ + input_offset;
					x_cnt--;
				}

				//fill zero for right regions (for what's left)
				while(x_cnt > 0){
					*dst++ = 0; *dst++ = 0; *dst++ = 0;//input_ch == 3
					x_cnt--;
				}
			}
			y_cnt -= m;

			//fill zeros in the bottom regions
			while (y_cnt > 0){
				*dst++ = 0; *dst++ = 0; *dst++ = 0;
				*dst++ = 0; *dst++ = 0; *dst++ = 0;
				*dst++ = 0; *dst++ = 0; *dst++ = 0;
				y_cnt--;
			}

			two_column_buf += 27;
			/* Computation is filed for every 2 columns */
			if (two_column_buf == runtime_buf + 2 * 27) {

				out = mat_mult_s16(kernel,
						runtime_buf, output_ch, output_shift, output_mult,
						output_offset, output_activation_min, output_activation_max,
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
			while (col_count) {
				q7_t ker_a1 = *ker_a++;
				q15_t ip_b1 = *ip_as_col++;
				sum += ker_a1 * ip_b1;
				col_count--;
			}

			sum = arm_nn_requantize(sum, output_mult[i], output_shift[i]);
			sum += output_offset;
			sum = MAX(sum, output_activation_min);
			sum = MIN(sum, output_activation_max);
			*out++ = (q7_t) sum;
		}
	}

	/* Return to application */
	return STATE_SUCCESS;
}
