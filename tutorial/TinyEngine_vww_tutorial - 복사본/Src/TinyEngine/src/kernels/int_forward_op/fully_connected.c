/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   fully_connected.c
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

#include "tinyengine_function.h"

tinyengine_status fully_connected_fp(
		const float *input, const uint16_t input_x, const uint16_t input_y,
		const uint16_t input_ch, const uint16_t output_ch, const float *bias,
		const float *weights, float *output)
{
	int h, w, out_c, in_c;
	for (h = 0; h < input_y; h++){
		for (w = 0; w < input_x; w++){
			int pixel_cnt = w + input_x * h;
			for (out_c = 0; out_c < output_ch; out_c++){
				float intermediate = bias[out_c];
				float *start_weight = weights + out_c * input_ch;
				float *start_input = input + input_ch * pixel_cnt;
				float *start_out = output + output_ch * pixel_cnt;
				for (in_c = 0; in_c < input_ch; in_c++){
					intermediate += start_weight[in_c] * start_input[in_c];
				}
				start_out[out_c] = intermediate;
			}
		}
	}

}
