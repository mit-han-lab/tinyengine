/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   where_fp.c
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
#include "tinyengine_function.h"

tinyengine_status_fp where(const bool* inMask, const uint16_t size, const float* input1_data,
			               const float* input2_data, float* output_data) {
  int i;

  for (i = 0; i < size; ++i) {
    output_data[i] = inMask[i] > 0 ? input1_data[i] : input2_data[i];
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}

tinyengine_status_fp where_zeros(const bool* inMask, const uint16_t size, const float* input1_data, float* output_data) {
  int i;

  for (i = 0; i < size; ++i) {
    output_data[i] = inMask[i] > 0 ? input1_data[i] : 0;
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}

tinyengine_status_fp where_zeros_inplace(const bool* inMask, const uint16_t size, float* input1_data) {
  int i;

  for (i = 0; i < size; ++i) {
	  input1_data[i] = inMask[i] > 0 ? input1_data[i] : 0;
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}

tinyengine_status_fp where_zeros_inplace_bit(const unsigned char* inMask, const uint16_t size, float* input1_data) {
  int i;

  for (i = 0; i < size; ++i) {
	  int bit_starting_idx = i % 8;
	  int mask = BIT_CHECK(inMask[i/8], bit_starting_idx);
	  input1_data[i] = mask > 0 ? input1_data[i] : 0;
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}
