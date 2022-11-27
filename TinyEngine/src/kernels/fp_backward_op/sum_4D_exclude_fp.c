/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   sum_4D_exclude_fp.c
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

tinyengine_status_fp sum_4D_exclude(const float* input_data, const uint16_t d1, const uint16_t d2,
                      const uint16_t d3, const uint16_t d4, const uint16_t axis, float* output_data) {
  int i, j, m, n;

  if (axis == 0){
    for (i = 0; i < d1; i++){
      float sum = 0;
      for (j = 0; j < d2; j++){
        for (m = 0; m < d3; m++) {
          for (n = 0; n < d4; n++){
            sum += input_data[((i * d2 + j) * d3 + m) * d4 + n];
          }
        }
      }
      output_data[i] = sum;
    }
  }
  else if (axis == 1){
    for (j = 0; j < d2; j++){
      float sum = 0;
      for (i = 0; i < d1; i++){
        for (m = 0; m < d3; m++) {
          for (n = 0; n < d4; n++){
            sum += input_data[((i * d2 + j) * d3 + m) * d4 + n];
          }
        }
      }
      output_data[j] = sum;
    }
  }
  else if (axis == 2){
    for (m = 0; m < d3; m++) {
      float sum = 0;
        for (i = 0; i < d1; i++){
          for (j = 0; j < d2; j++){
          for (n = 0; n < d4; n++){
            sum += input_data[((i * d2 + j) * d3 + m) * d4 + n];
          }
        }
      }
      output_data[m] = sum;
    }
  }
  else if (axis == 3){
    for (n = 0; n < d4; n++){
      float sum = 0;
        for (i = 0; i < d1; i++){
          for (j = 0; j < d2; j++){
            for (m = 0; m < d3; m++) {
            sum += input_data[((i * d2 + j) * d3 + m) * d4 + n];
          }
        }
      }
      output_data[n] = sum;
    }
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}