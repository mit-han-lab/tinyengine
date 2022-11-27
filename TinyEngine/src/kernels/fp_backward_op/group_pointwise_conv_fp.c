/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   group_pointwise_conv_fp.c
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
#include "nnfunctions_fp.h"
#define DIM_KER_X (1U)
#define DIM_KER_Y (1U)

tinyengine_status_fp group_pointwise_conv_fp_in1x1_out1x1_1row10col_uniweight_int8input_inplace(const int8_t* input_data, 
                 const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
                 const float* filter_data, const float* bias_data, 
                 int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
                 const float output_activation_min, const float output_activation_max,
                 float* im2col_data, const uint16_t batches, const uint16_t groups,
                 const float* scales, const float learning_rate) {
  (void) input_height;
  (void) input_width;

  int group;
  int output_depth_per_group = output_depth / groups;

  for (group = 0; group < groups; group++) {
    int i_ch_out;

    for (i_ch_out = 0; i_ch_out < output_depth_per_group; i_ch_out+=10) {
      /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
      const float input_0 = (float)input_data[group];
      const float filter[10] = {filter_data[i_ch_out], filter_data[i_ch_out + 1], filter_data[i_ch_out + 2], filter_data[i_ch_out + 3], filter_data[i_ch_out + 4], 
                      filter_data[i_ch_out + 5], filter_data[i_ch_out + 6], filter_data[i_ch_out + 7], filter_data[i_ch_out + 8], filter_data[i_ch_out + 9]};

      uint16_t col_count_div10 = (output_depth_per_group * DIM_KER_X * DIM_KER_Y) / 10;

      while (col_count_div10--) {
        // Assume bias_data as NULL
        float sum[10] = {};

        sum[0] += input_0 * filter[0];
        sum[1] += input_0 * filter[1];
        sum[2] += input_0 * filter[2];
        sum[3] += input_0 * filter[3];
        sum[4] += input_0 * filter[4];
        sum[5] += input_0 * filter[5];
        sum[6] += input_0 * filter[6];
        sum[7] += input_0 * filter[7];
        sum[8] += input_0 * filter[8];
        sum[9] += input_0 * filter[9];

        output_weight_data[i_ch_out + group] -= TN_MIN(TN_MAX(sum[0], output_activation_min), output_activation_max) * scales[i_ch_out] * learning_rate;
        output_weight_data[(i_ch_out + 1) * groups + group] -= TN_MIN(TN_MAX(sum[1], output_activation_min), output_activation_max) * scales[i_ch_out + 1] * learning_rate;
        output_weight_data[(i_ch_out + 2) * groups + group] -= TN_MIN(TN_MAX(sum[2], output_activation_min), output_activation_max) * scales[i_ch_out + 2] * learning_rate;
        output_weight_data[(i_ch_out + 3) * groups + group] -= TN_MIN(TN_MAX(sum[3], output_activation_min), output_activation_max) * scales[i_ch_out + 3] * learning_rate;
        output_weight_data[(i_ch_out + 4) * groups + group] -= TN_MIN(TN_MAX(sum[4], output_activation_min), output_activation_max) * scales[i_ch_out + 4] * learning_rate;
        output_weight_data[(i_ch_out + 5) * groups + group] -= TN_MIN(TN_MAX(sum[5], output_activation_min), output_activation_max) * scales[i_ch_out + 5] * learning_rate;
        output_weight_data[(i_ch_out + 6) * groups + group] -= TN_MIN(TN_MAX(sum[6], output_activation_min), output_activation_max) * scales[i_ch_out + 6] * learning_rate;
        output_weight_data[(i_ch_out + 7) * groups + group] -= TN_MIN(TN_MAX(sum[7], output_activation_min), output_activation_max) * scales[i_ch_out + 7] * learning_rate;
        output_weight_data[(i_ch_out + 8) * groups + group] -= TN_MIN(TN_MAX(sum[8], output_activation_min), output_activation_max) * scales[i_ch_out + 8] * learning_rate;
        output_weight_data[(i_ch_out + 9) * groups + group] -= TN_MIN(TN_MAX(sum[9], output_activation_min), output_activation_max) * scales[i_ch_out + 9] * learning_rate;
      }
    }
  }

  /* Return to application */
  return STATE_SUCCESS_fp;
}

tinyengine_status_fp group_pointwise_conv_fp_in1x1_out1x1_1row10col_uniweight_inplace(const float* input_data, 
                 const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
                 const float* filter_data, const float* bias_data, 
                 int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
                 const float output_activation_min, const float output_activation_max,
                 float* im2col_data, const uint16_t batches, const uint16_t groups,
                 const float* scales, const float learning_rate) {
  (void) input_height;
  (void) input_width;

  int group;
  int output_depth_per_group = output_depth / groups;

  for(group = 0; group < groups; group++) {
    int i_ch_out;

    for (i_ch_out = 0; i_ch_out < output_depth_per_group; i_ch_out+=10) {
      /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
      const float input_0 = input_data[group];
      const float filter[10] = {filter_data[i_ch_out], filter_data[i_ch_out + 1], filter_data[i_ch_out + 2], filter_data[i_ch_out + 3], filter_data[i_ch_out + 4], 
                      filter_data[i_ch_out + 5], filter_data[i_ch_out + 6], filter_data[i_ch_out + 7], filter_data[i_ch_out + 8], filter_data[i_ch_out + 9]};

      uint16_t col_count_div10 = (output_depth_per_group * DIM_KER_X * DIM_KER_Y) / 10;

      while (col_count_div10--) {
        // Assume bias_data as NULL
        float sum[10] = {};

        sum[0] += input_0 * filter[0];
        sum[1] += input_0 * filter[1];
        sum[2] += input_0 * filter[2];
        sum[3] += input_0 * filter[3];
        sum[4] += input_0 * filter[4];
        sum[5] += input_0 * filter[5];
        sum[6] += input_0 * filter[6];
        sum[7] += input_0 * filter[7];
        sum[8] += input_0 * filter[8];
        sum[9] += input_0 * filter[9];

        output_weight_data[i_ch_out + group] -= TN_MIN(TN_MAX(sum[0], output_activation_min), output_activation_max) * scales[i_ch_out] * learning_rate;
        output_weight_data[(i_ch_out + 1) * groups + group] -= TN_MIN(TN_MAX(sum[1], output_activation_min), output_activation_max) * scales[i_ch_out + 1] * learning_rate;
        output_weight_data[(i_ch_out + 2) * groups + group] -= TN_MIN(TN_MAX(sum[2], output_activation_min), output_activation_max) * scales[i_ch_out + 2] * learning_rate;
        output_weight_data[(i_ch_out + 3) * groups + group] -= TN_MIN(TN_MAX(sum[3], output_activation_min), output_activation_max) * scales[i_ch_out + 3] * learning_rate;
        output_weight_data[(i_ch_out + 4) * groups + group] -= TN_MIN(TN_MAX(sum[4], output_activation_min), output_activation_max) * scales[i_ch_out + 4] * learning_rate;
        output_weight_data[(i_ch_out + 5) * groups + group] -= TN_MIN(TN_MAX(sum[5], output_activation_min), output_activation_max) * scales[i_ch_out + 5] * learning_rate;
        output_weight_data[(i_ch_out + 6) * groups + group] -= TN_MIN(TN_MAX(sum[6], output_activation_min), output_activation_max) * scales[i_ch_out + 6] * learning_rate;
        output_weight_data[(i_ch_out + 7) * groups + group] -= TN_MIN(TN_MAX(sum[7], output_activation_min), output_activation_max) * scales[i_ch_out + 7] * learning_rate;
        output_weight_data[(i_ch_out + 8) * groups + group] -= TN_MIN(TN_MAX(sum[8], output_activation_min), output_activation_max) * scales[i_ch_out + 8] * learning_rate;
        output_weight_data[(i_ch_out + 9) * groups + group] -= TN_MIN(TN_MAX(sum[9], output_activation_min), output_activation_max) * scales[i_ch_out + 9] * learning_rate;
      }
    }
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}
