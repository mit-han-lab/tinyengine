/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   pointwise_conv_fp.c
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

tinyengine_status_fp pointwise_conv_fp_1row10col_10inputdepth_IOHW_int8weight(const float* input_data, 
                 const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
                 const int8_t* filter_data, const float* bias_data, 
                 float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
                 const float output_activation_min, const float output_activation_max,
                 float* im2col_data, const uint16_t batches) {
  (void) input_height;
  (void) input_width;

  float* out = output_data;

  int i_element;
  const int num_elements = output_height * output_width;

  for (i_element = 0; i_element < num_elements; i_element++) {
    int i_ch_in;

    for (i_ch_in = 0; i_ch_in < input_depth; i_ch_in+=10) {
      /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
      const float* input_0 = &input_data[i_element * input_depth];

      const int8_t* filter_0_int8 = &filter_data[i_ch_in * output_depth];
      const int8_t* filter_1_int8 = &filter_data[(i_ch_in + 1) * output_depth];
      const int8_t* filter_2_int8 = &filter_data[(i_ch_in + 2) * output_depth];
      const int8_t* filter_3_int8 = &filter_data[(i_ch_in + 3) * output_depth];
      const int8_t* filter_4_int8 = &filter_data[(i_ch_in + 4) * output_depth];
      const int8_t* filter_5_int8 = &filter_data[(i_ch_in + 5) * output_depth];
      const int8_t* filter_6_int8 = &filter_data[(i_ch_in + 6) * output_depth];
      const int8_t* filter_7_int8 = &filter_data[(i_ch_in + 7) * output_depth];
      const int8_t* filter_8_int8 = &filter_data[(i_ch_in + 8) * output_depth];
      const int8_t* filter_9_int8 = &filter_data[(i_ch_in + 9) * output_depth];
      float filter_0, filter_1, filter_2, filter_3, filter_4, filter_5, filter_6, filter_7, filter_8, filter_9;

      uint16_t col_count_div8 = (output_depth * DIM_KER_X * DIM_KER_Y) >> 3;

      while (col_count_div8--) {
        float sum[8] = {};

        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        filter_4 = (float)*filter_4_int8++; filter_5 = (float)*filter_5_int8++; filter_6 = (float)*filter_6_int8++; filter_7 = (float)*filter_7_int8++;
        filter_8 = (float)*filter_8_int8++; filter_9 = (float)*filter_9_int8++;
        mac_1row_10col_fp_IOHW_forint8w(&sum[0], input_0, filter_0, filter_1, filter_2, filter_3, filter_4, filter_5, filter_6, filter_7, filter_8, filter_9);

        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        filter_4 = (float)*filter_4_int8++; filter_5 = (float)*filter_5_int8++; filter_6 = (float)*filter_6_int8++; filter_7 = (float)*filter_7_int8++;
        filter_8 = (float)*filter_8_int8++; filter_9 = (float)*filter_9_int8++;
        mac_1row_10col_fp_IOHW_forint8w(&sum[1], input_0, filter_0, filter_1, filter_2, filter_3, filter_4, filter_5, filter_6, filter_7, filter_8, filter_9);

        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        filter_4 = (float)*filter_4_int8++; filter_5 = (float)*filter_5_int8++; filter_6 = (float)*filter_6_int8++; filter_7 = (float)*filter_7_int8++;
        filter_8 = (float)*filter_8_int8++; filter_9 = (float)*filter_9_int8++;
        mac_1row_10col_fp_IOHW_forint8w(&sum[2], input_0, filter_0, filter_1, filter_2, filter_3, filter_4, filter_5, filter_6, filter_7, filter_8, filter_9);

        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        filter_4 = (float)*filter_4_int8++; filter_5 = (float)*filter_5_int8++; filter_6 = (float)*filter_6_int8++; filter_7 = (float)*filter_7_int8++;
        filter_8 = (float)*filter_8_int8++; filter_9 = (float)*filter_9_int8++;
        mac_1row_10col_fp_IOHW_forint8w(&sum[3], input_0, filter_0, filter_1, filter_2, filter_3, filter_4, filter_5, filter_6, filter_7, filter_8, filter_9);

        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        filter_4 = (float)*filter_4_int8++; filter_5 = (float)*filter_5_int8++; filter_6 = (float)*filter_6_int8++; filter_7 = (float)*filter_7_int8++;
        filter_8 = (float)*filter_8_int8++; filter_9 = (float)*filter_9_int8++;
        mac_1row_10col_fp_IOHW_forint8w(&sum[4], input_0, filter_0, filter_1, filter_2, filter_3, filter_4, filter_5, filter_6, filter_7, filter_8, filter_9);

        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        filter_4 = (float)*filter_4_int8++; filter_5 = (float)*filter_5_int8++; filter_6 = (float)*filter_6_int8++; filter_7 = (float)*filter_7_int8++;
        filter_8 = (float)*filter_8_int8++; filter_9 = (float)*filter_9_int8++;
        mac_1row_10col_fp_IOHW_forint8w(&sum[5], input_0, filter_0, filter_1, filter_2, filter_3, filter_4, filter_5, filter_6, filter_7, filter_8, filter_9);

        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        filter_4 = (float)*filter_4_int8++; filter_5 = (float)*filter_5_int8++; filter_6 = (float)*filter_6_int8++; filter_7 = (float)*filter_7_int8++;
        filter_8 = (float)*filter_8_int8++; filter_9 = (float)*filter_9_int8++;
        mac_1row_10col_fp_IOHW_forint8w(&sum[6], input_0, filter_0, filter_1, filter_2, filter_3, filter_4, filter_5, filter_6, filter_7, filter_8, filter_9);

        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        filter_4 = (float)*filter_4_int8++; filter_5 = (float)*filter_5_int8++; filter_6 = (float)*filter_6_int8++; filter_7 = (float)*filter_7_int8++;
        filter_8 = (float)*filter_8_int8++; filter_9 = (float)*filter_9_int8++;
        mac_1row_10col_fp_IOHW_forint8w(&sum[7], input_0, filter_0, filter_1, filter_2, filter_3, filter_4, filter_5, filter_6, filter_7, filter_8, filter_9);

        *out++ = TN_MIN(TN_MAX(sum[0], output_activation_min), output_activation_max);
        *out++ = TN_MIN(TN_MAX(sum[1], output_activation_min), output_activation_max);
        *out++ = TN_MIN(TN_MAX(sum[2], output_activation_min), output_activation_max);
        *out++ = TN_MIN(TN_MAX(sum[3], output_activation_min), output_activation_max);
        *out++ = TN_MIN(TN_MAX(sum[4], output_activation_min), output_activation_max);
        *out++ = TN_MIN(TN_MAX(sum[5], output_activation_min), output_activation_max);
        *out++ = TN_MIN(TN_MAX(sum[6], output_activation_min), output_activation_max);
        *out++ = TN_MIN(TN_MAX(sum[7], output_activation_min), output_activation_max);
      }
    }
  }
}

tinyengine_status_fp pointwise_conv_fp_4row4col_IOHW_int8weight(const float* input_data, 
                 const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
                 const int8_t* filter_data, const float* bias_data, 
                 float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
                 const float output_activation_min, const float output_activation_max,
                 float* im2col_data, const uint16_t batches) {
  (void) input_height;
  (void) input_width;

  int i_element;
  const int num_elements = output_height * output_width;

  /* Initialize output data as 0 (assume bias == NULL) */
  for(i_element = 0; i_element < output_depth*num_elements; i_element++) {
    output_data[i_element] = 0;
  }

  for (i_element = 0; i_element/4 < num_elements/4; i_element+=4) {
    int i_ch_in;

    for (i_ch_in = 0; i_ch_in < input_depth; i_ch_in+=4) {
      float* out_0 = &output_data[i_element * output_depth];
      float* out_1 = &output_data[(i_element + 1) * output_depth];
      float* out_2 = &output_data[(i_element + 2) * output_depth];
      float* out_3 = &output_data[(i_element + 3) * output_depth];

      const float* input_0 = &input_data[i_element * input_depth + i_ch_in];
      const float* input_1 = &input_data[(i_element + 1) * input_depth + i_ch_in];
      const float* input_2 = &input_data[(i_element + 2) * input_depth + i_ch_in];
      const float* input_3 = &input_data[(i_element + 3) * input_depth + i_ch_in];

      const int8_t* filter_0_int8 = &filter_data[i_ch_in * output_depth];
      const int8_t* filter_1_int8 = &filter_data[(i_ch_in + 1) * output_depth];
      const int8_t* filter_2_int8 = &filter_data[(i_ch_in + 2) * output_depth];
      const int8_t* filter_3_int8 = &filter_data[(i_ch_in + 3) * output_depth];
      float filter_0, filter_1, filter_2, filter_3;

      uint16_t col_count_div8 = (output_depth * DIM_KER_X * DIM_KER_Y) >> 3;

      while (col_count_div8--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[32] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[0], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[4], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[8], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[12], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[16], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[20], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[24], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[28], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_4row8col(out_0, out_1, out_2, out_3, sum, output_activation_min, output_activation_max);
        out_0 += 8; out_1 += 8; out_2 += 8; out_3 += 8;
      }
    }
  }

  /* Handle left-over part */
  int leftover_elements = num_elements & 0x3;

  while (leftover_elements) {
    int i_ch_in;

    for (i_ch_in = 0; i_ch_in < input_depth; i_ch_in+=4) {
      float* out_0 = &output_data[(num_elements - leftover_elements) * output_depth];

      const float* input_0 = &input_data[(num_elements - leftover_elements) * input_depth + i_ch_in];

      const int8_t* filter_0_int8 = &filter_data[i_ch_in * output_depth];
      const int8_t* filter_1_int8 = &filter_data[(i_ch_in + 1) * output_depth];
      const int8_t* filter_2_int8 = &filter_data[(i_ch_in + 2) * output_depth];
      const int8_t* filter_3_int8 = &filter_data[(i_ch_in + 3) * output_depth];
      float filter_0, filter_1, filter_2, filter_3;

      uint16_t col_count_div8 = (output_depth * DIM_KER_X * DIM_KER_Y) >> 3;

      while (col_count_div8--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[8] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[0], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[1], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[2], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[3], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[4], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[5], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[6], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[7], input_0, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_1row8col(out_0, sum, output_activation_min, output_activation_max);
        out_0 += 8;
      }
    }

    leftover_elements--;
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}

tinyengine_status_fp pointwise_conv_fp_4row4col_IOHW_int8weight_partialCH_8innercol(const float* input_data, 
                 const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
                 const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
                 float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
                 const float output_activation_min, const float output_activation_max,
                 float* im2col_data, const uint16_t batches) {
  (void) input_height;
  (void) input_width;

  int i_element;
  const int num_elements = output_height * output_width;

  /* Initialize output data as 0 (assume bias == NULL) */
  for(i_element = 0; i_element < output_depth*num_elements; i_element++) {
    output_data[i_element] = 0;
  }

  for (i_element = 0; i_element/4 < num_elements/4; i_element+=4) {
    int i_ch_in;

    for (i_ch_in = 0; i_ch_in < input_depth; i_ch_in+=4) {
      float* out_0 = &output_data[i_element * output_depth];
      float* out_1 = &output_data[(i_element + 1) * output_depth];
      float* out_2 = &output_data[(i_element + 2) * output_depth];
      float* out_3 = &output_data[(i_element + 3) * output_depth];

      const float* input_0 = &input_data[i_element * input_depth + i_ch_in];
      const float* input_1 = &input_data[(i_element + 1) * input_depth + i_ch_in];
      const float* input_2 = &input_data[(i_element + 2) * input_depth + i_ch_in];
      const float* input_3 = &input_data[(i_element + 3) * input_depth + i_ch_in];

      const int8_t* filter_0_int8 = &filter_sram[i_ch_in * first_k_channel];
      const int8_t* filter_1_int8 = &filter_sram[(i_ch_in + 1) * first_k_channel];
      const int8_t* filter_2_int8 = &filter_sram[(i_ch_in + 2) * first_k_channel];
      const int8_t* filter_3_int8 = &filter_sram[(i_ch_in + 3) * first_k_channel];
      float filter_0, filter_1, filter_2, filter_3;

      /* Compute weights in SRAM */
      uint16_t col_count_div8 = (first_k_channel * DIM_KER_X * DIM_KER_Y) >> 3;
      while (col_count_div8--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[32] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[0], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[4], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[8], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[12], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[16], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[20], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[24], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[28], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_4row8col(out_0, out_1, out_2, out_3, sum, output_activation_min, output_activation_max);
        out_0 += 8; out_1 += 8; out_2 += 8; out_3 += 8;
      }

      filter_0_int8 = &filter_flash[i_ch_in * (output_depth - first_k_channel)];
      filter_1_int8 = &filter_flash[(i_ch_in + 1) * (output_depth - first_k_channel)];
      filter_2_int8 = &filter_flash[(i_ch_in + 2) * (output_depth - first_k_channel)];
      filter_3_int8 = &filter_flash[(i_ch_in + 3) * (output_depth - first_k_channel)];

      /* Compute weights in FLASH */
      col_count_div8 = ((output_depth - first_k_channel) * DIM_KER_X * DIM_KER_Y) >> 3;
      while (col_count_div8--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[32] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[0], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[4], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[8], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[12], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[16], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[20], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[24], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[28], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_4row8col(out_0, out_1, out_2, out_3, sum, output_activation_min, output_activation_max);
        out_0 += 8; out_1 += 8; out_2 += 8; out_3 += 8;
      }
    }
  }

  /* Handle left-over part */
  int leftover_elements = num_elements & 0x3;

  while (leftover_elements) {
    int i_ch_in;

    for (i_ch_in = 0; i_ch_in < input_depth; i_ch_in+=4) {
      float* out_0 = &output_data[(num_elements - leftover_elements) * output_depth];

      const float* input_0 = &input_data[(num_elements - leftover_elements) * input_depth + i_ch_in];

      const int8_t* filter_0_int8 = &filter_sram[i_ch_in * first_k_channel];
      const int8_t* filter_1_int8 = &filter_sram[(i_ch_in + 1) * first_k_channel];
      const int8_t* filter_2_int8 = &filter_sram[(i_ch_in + 2) * first_k_channel];
      const int8_t* filter_3_int8 = &filter_sram[(i_ch_in + 3) * first_k_channel];
      float filter_0, filter_1, filter_2, filter_3;

      /* Compute weights in SRAM */
      uint16_t col_count_div8 = (first_k_channel * DIM_KER_X * DIM_KER_Y) >> 3;
      while (col_count_div8--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[8] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[0], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[1], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[2], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[3], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[4], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[5], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[6], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[7], input_0, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_1row8col(out_0, sum, output_activation_min, output_activation_max);
        out_0 += 8;
      }

      filter_0_int8 = &filter_flash[i_ch_in * (output_depth - first_k_channel)];
      filter_1_int8 = &filter_flash[(i_ch_in + 1) * (output_depth - first_k_channel)];
      filter_2_int8 = &filter_flash[(i_ch_in + 2) * (output_depth - first_k_channel)];
      filter_3_int8 = &filter_flash[(i_ch_in + 3) * (output_depth - first_k_channel)];

      /* Compute weights in FLASH */
      col_count_div8 = ((output_depth - first_k_channel) * DIM_KER_X * DIM_KER_Y) >> 3;
      while (col_count_div8--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[8] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[0], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[1], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[2], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[3], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[4], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[5], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[6], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[7], input_0, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_1row8col(out_0, sum, output_activation_min, output_activation_max);
        out_0 += 8;
      }
    }

    leftover_elements--;
  }

  /* Return to application */
  return STATE_SUCCESS_fp;
}

tinyengine_status_fp pointwise_conv_fp_4row4col_IOHW_int8weight_partialCH_4innercol(const float* input_data, 
                 const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
                 const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
                 float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
                 const float output_activation_min, const float output_activation_max,
                 float* im2col_data, const uint16_t batches) {
  (void) input_height;
  (void) input_width;

  int i_element;
  const int num_elements = output_height * output_width;

  /* Initialize output data as 0 (assume bias == NULL) */
  for(i_element = 0; i_element < output_depth*num_elements; i_element++) {
    output_data[i_element] = 0;
  }

  for (i_element = 0; i_element/4 < num_elements/4; i_element+=4) {
    int i_ch_in;

    for (i_ch_in = 0; i_ch_in < input_depth; i_ch_in+=4) {
      float* out_0 = &output_data[i_element * output_depth];
      float* out_1 = &output_data[(i_element + 1) * output_depth];
      float* out_2 = &output_data[(i_element + 2) * output_depth];
      float* out_3 = &output_data[(i_element + 3) * output_depth];

      const float* input_0 = &input_data[i_element * input_depth + i_ch_in];
      const float* input_1 = &input_data[(i_element + 1) * input_depth + i_ch_in];
      const float* input_2 = &input_data[(i_element + 2) * input_depth + i_ch_in];
      const float* input_3 = &input_data[(i_element + 3) * input_depth + i_ch_in];

      const int8_t* filter_0_int8 = &filter_sram[i_ch_in * first_k_channel];
      const int8_t* filter_1_int8 = &filter_sram[(i_ch_in + 1) * first_k_channel];
      const int8_t* filter_2_int8 = &filter_sram[(i_ch_in + 2) * first_k_channel];
      const int8_t* filter_3_int8 = &filter_sram[(i_ch_in + 3) * first_k_channel];
      float filter_0, filter_1, filter_2, filter_3;

      /* Compute weights in SRAM */
      uint16_t col_count_div4 = (first_k_channel * DIM_KER_X * DIM_KER_Y) >> 2;
      while (col_count_div4--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[16] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[0], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[4], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[8], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[12], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_4row4col(out_0, out_1, out_2, out_3, sum, output_activation_min, output_activation_max);
        out_0 += 4; out_1 += 4; out_2 += 4; out_3 += 4;
      }

      filter_0_int8 = &filter_flash[i_ch_in * (output_depth - first_k_channel)];
      filter_1_int8 = &filter_flash[(i_ch_in + 1) * (output_depth - first_k_channel)];
      filter_2_int8 = &filter_flash[(i_ch_in + 2) * (output_depth - first_k_channel)];
      filter_3_int8 = &filter_flash[(i_ch_in + 3) * (output_depth - first_k_channel)];

      /* Compute weights in FLASH */
      col_count_div4 = ((output_depth - first_k_channel) * DIM_KER_X * DIM_KER_Y) >> 2;
      while (col_count_div4--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[16] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[0], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[4], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[8], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_4row_4col_fp_IOHW_forint8w(&sum[12], input_0, input_1, input_2, input_3, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_4row4col(out_0, out_1, out_2, out_3, sum, output_activation_min, output_activation_max);
        out_0 += 4; out_1 += 4; out_2 += 4; out_3 += 4;
      }
    }
  }

  /* Handle left-over part */
  int leftover_elements = num_elements & 0x3;

  while (leftover_elements) {
    int i_ch_in;

    for (i_ch_in = 0; i_ch_in < input_depth; i_ch_in+=4) {
      float* out_0 = &output_data[(num_elements - leftover_elements) * output_depth];

      const float* input_0 = &input_data[(num_elements - leftover_elements) * input_depth + i_ch_in];

      const int8_t* filter_0_int8 = &filter_sram[i_ch_in * first_k_channel];
      const int8_t* filter_1_int8 = &filter_sram[(i_ch_in + 1) * first_k_channel];
      const int8_t* filter_2_int8 = &filter_sram[(i_ch_in + 2) * first_k_channel];
      const int8_t* filter_3_int8 = &filter_sram[(i_ch_in + 3) * first_k_channel];
      float filter_0, filter_1, filter_2, filter_3;

      /* Compute weights in SRAM */
      uint16_t col_count_div4 = (first_k_channel * DIM_KER_X * DIM_KER_Y) >> 2;
      while (col_count_div4--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[4] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[0], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[1], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[2], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[3], input_0, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_1row4col(out_0, sum, output_activation_min, output_activation_max);
        out_0 += 4;
      }

      filter_0_int8 = &filter_flash[i_ch_in * (output_depth - first_k_channel)];
      filter_1_int8 = &filter_flash[(i_ch_in + 1) * (output_depth - first_k_channel)];
      filter_2_int8 = &filter_flash[(i_ch_in + 2) * (output_depth - first_k_channel)];
      filter_3_int8 = &filter_flash[(i_ch_in + 3) * (output_depth - first_k_channel)];

      /* Compute weights in FLASH */
      col_count_div4 = ((output_depth - first_k_channel) * DIM_KER_X * DIM_KER_Y) >> 2;
      while (col_count_div4--) {
        /* Initialize partial sum (assume bias == NULL) */
        float sum[4] = {};

        /* MAC computation */
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[0], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[1], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[2], input_0, filter_0, filter_1, filter_2, filter_3);
        filter_0 = (float)*filter_0_int8++; filter_1 = (float)*filter_1_int8++; filter_2 = (float)*filter_2_int8++; filter_3 = (float)*filter_3_int8++;
        mac_1row_4col_fp_IOHW_forint8w(&sum[3], input_0, filter_0, filter_1, filter_2, filter_3);

        /* Accumulate partial sum into output data */
        assign_sum_to_pointwise_output_1row4col(out_0, sum, output_activation_min, output_activation_max);
        out_0 += 4;
      }
    }

    leftover_elements--;
  }

  /* Return to application */
  return STATE_SUCCESS_fp;
}
