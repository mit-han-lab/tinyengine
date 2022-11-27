/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   sum_3D_fp.c
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

tinyengine_status_fp sum_3D(const float* input_data, const uint16_t input_w, const uint16_t input_h,
                      const uint16_t input_c, const uint16_t axis, float* output_data) {
  int c, h, w, i;
  float sum;

  if (axis == 2){
    for (c = 0; c < input_c; ++c) {
      for (h = 0; h < input_h; ++h) {
        sum = 0;

        for (w = 0; w < input_w; ++w) {
          sum += input_data[(w + h * input_w) * input_c + c];
        }

        output_data[c + (h * input_c)] = sum;
      }
    }
  }
  else if (axis == 1){
    for (c = 0; c < input_c; ++c) {
      for (w = 0; w < input_w; ++w) {
        sum = 0;

        for (h = 0; h < input_h; ++h) {
          sum += input_data[(w + h * input_w) * input_c + c];
        }

        output_data[c + (w * input_c)] = sum;
      }
    }
  }
  else{ /* axis == 0 */
    for (h = 0; h < input_h; ++h) {
      for (w = 0; w < input_w; ++w) {
        sum = 0;

        for (c = 0; c < input_c; ++c) {
          sum += input_data[(w + h * input_w) * input_c + c];
      }

        output_data[w + (h * input_w)] = sum;
      }
    }
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}
