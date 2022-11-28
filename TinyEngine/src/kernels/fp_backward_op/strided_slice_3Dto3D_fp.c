/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   strided_slice_3Dto3D_fp.c
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

tinyengine_status_fp strided_slice_3Dto3D(const float* input, const uint16_t input_h, const uint16_t input_w, const uint16_t input_c,
                                          const uint16_t* begin, const uint16_t* end, const uint16_t* stride, 
                                          float* output, const uint16_t output_h, const uint16_t output_w, const uint16_t output_c) {
  int h, w, c;
  
  for(c = begin[2]; c < end[2]; c += stride[2]){
    for(h = begin[1]; h < end[1]; h += stride[1]){
      for(w = begin[0]; w < end[0]; w += stride[0]){
        output[(w + h * output_w) * output_c + c] = input[(w + h * input_w) * input_c + c];
      }
    }
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}
