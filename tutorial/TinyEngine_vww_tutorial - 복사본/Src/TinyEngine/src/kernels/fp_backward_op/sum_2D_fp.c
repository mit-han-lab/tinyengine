/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   sum_2D_fp.c
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

tinyengine_status_fp sum_2D(const float* input_data, const uint16_t matA_row,
                      const uint16_t matA_col, const uint16_t axis, float* output_data) {
  int i, j;
  float sum;

  if (axis == 0){
    for (i = 0; i < matA_row; ++i) {
      sum = 0;

      for (j = 0; j < matA_col; ++j) {
        sum += input_data[j + (i * matA_row)];
      }

      output_data[i] = sum;
    }
  }
  else{ /* axis == 1 */
    for (j = 0; j < matA_col; ++j) {
      sum = 0;

      for (i = 0; i < matA_row; ++i) {
        sum += input_data[j + (i * matA_row)];
      }

      output_data[j] = sum;
    }
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}
