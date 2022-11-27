/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   strided_slice_4Dto4D_fp.c
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *  - Chuang Gan, ganchuang@csail.mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#include "tinyengine_function_fp.h"

tinyengine_status_fp strided_slice_4Dto4D(const float* input, const uint16_t inn, const uint16_t inc, const uint16_t inh, const uint16_t inw,
                                          const uint16_t* begin, const uint16_t* end, const uint16_t* stride,
                                          float* output, const uint16_t on, const uint16_t oc, const uint16_t oh, const uint16_t ow) {
  int n, c, h, w;
  //begin and end are in [n, c, h, w]
  for(n = begin[0]; n < end[0]; n += stride[0]){
		for(h = begin[2]; h < end[2]; h += stride[0]){
		  for(w = begin[3]; w < end[3]; w += stride[0]){
				for(c = begin[1]; c < end[1]; c += stride[0]){
					output[((h + n * oh) * ow + w) * oc + c] = input[((h + n * inh) * inw + w) * inc + c];
				}
			}
	  }
	}
	
	/* Return to application */
	return STATE_SUCCESS_fp;
}
