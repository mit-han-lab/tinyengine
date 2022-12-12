/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   concat_ch.c
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

#include "arm_nnfunctions.h"
#include "tinyengine_function.h"

tinyengine_status concat_ch(const q7_t *input1, const uint16_t input_x,
	const uint16_t input_y, const uint16_t input1_ch, const q7_t* input2, const uint16_t input2_ch, q7_t *output) {

	int elements = input_y * input_x;

	while(elements--){
		//place the first input
		memcpy(output, input1, input1_ch);
		input1 += input1_ch; output += input1_ch;

		//place the second input
		memcpy(output, input2, input2_ch);
		input2 += input2_ch; output += input2_ch;
	}

	return STATE_SUCCESS;
}





