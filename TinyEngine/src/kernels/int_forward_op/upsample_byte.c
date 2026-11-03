/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   upsample_byte.c
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

tinyengine_status upsample_byte(const q7_t *input, const uint16_t input_x,
	const uint16_t input_y, const uint16_t input_ch, q7_t *output, const uint16_t sample_factor) {
	/*
	Does upsample inplace: To achieve this it does the sampling in reverse order.
	
	Upsampling makes the output grid larger than the input grid by a factor of s**2,
	We can exploit this to use the same memory or make it inplace for both the input
	and output without no cost by changing the upsampling order to reverse.

	Basically we start copy and writing to the output for the back this ensures that
	we never overwrite an input before it is needed.
	*/ 

	
	//get output resolution
	const uint16_t output_x = input_x * sample_factor, output_y = input_y * sample_factor , output_ch = input_ch;
	// pointer to each element in each of the grid
	const q7_t* reverse_input = input + ((input_x * input_y) * input_ch);
	q7_t* reverse_output = output + ((output_x * output_y) * output_ch);

	//upsample in a repeated and reversed manner
	for(int ih = input_y-1; ih >= 0; ih--){
		//place 1 row
		for(int iw = input_x-1; iw >= 0; iw--){
			reverse_input -= input_ch;
			for(int s = 0; s < sample_factor; s++){
				reverse_output -= input_ch;
				memcpy(reverse_output, reverse_input, input_ch);
			}
		}

		//copy the remaining rows
		q7_t* row_start = reverse_output;
		for(int s = 1; s < sample_factor; s++){
			// move it to the preceeding row
			reverse_output -= output_ch * output_x;
			memcpy(reverse_output, row_start, output_ch * output_x);
		}
	}
	return STATE_SUCCESS;
}


//ref: https://www.cs.toronto.edu/~guerzhoy/320/lec/upsampling.pdf
tinyengine_status upsample_byte_bilinear(const q7_t *input, const uint16_t input_x,
	const uint16_t input_y, const uint16_t input_ch, q7_t *output, const uint16_t sample_factor) {
	//get output resolution
	const uint16_t output_x = input_x * sample_factor, output_y = input_y * sample_factor , output_ch = input_ch;

//	//upsample in a repeated manner
//	for(int oh = 0; oh < input_y; oh++){
//		int ih = oh / sample_factor;
//		int rh = oh % sample_factor;
//
//		q7_t* out_head = output;
//		//place 1 row
//		for(int ow = 0; ow < onput_x; ow++){
//			int iw = iw / sample_factor;
//			int wh = wh % sample_factor;
//
//			//exact coordinate
//			q7_t* ori_input = input + input_ch * (input_x * ih + iw);
//			if(rh | wh == 0){
//				memcpy(output, ori_input, input_ch);
//				continue;
//			}
//
//			//interpolate
//			q7_t* topleft = ori_input;
//			q7_t* topright = ori_input + input_ch;
//			q7_t* bottomleft = topleft + input_ch * input_x;
//			q7_t* bottomright = topright + input_ch * input_x;
//		}
//	}
	return STATE_SUCCESS;
}




