/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   group_conv_fp_kernel4_stride1_pad0.c
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
#include "nnfunctions_fp.h"
#define DIM_KER_X (4U)
#define DIM_KER_Y (4U)

tinyengine_status_fp group_conv_fp_kernel4_stride1_pad0_in4x4_out1x1_uniweight_4row16col_inplace(const float* input_data, 
                 const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
                 const float* filter_data, const float* bias_data, 
                 int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
                 const float output_activation_min, const float output_activation_max,
                 float* im2col_data, const uint16_t batches, const uint16_t groups,
                 const float* scales, const float learning_rate) {
  int i_output_depth, i , j;
  int output_depth_per_group = output_depth / groups;

  for (i_output_depth = 0; i_output_depth < output_depth_per_group; i_output_depth += 16) {
    /* Alter the data format of filter_data from IHWO to OHWI and put it into im2col_data buffer */
    float* two_column_buffer_0 = im2col_data; float* two_column_buffer_1 = &im2col_data[DIM_KER_X * DIM_KER_Y];
    float* two_column_buffer_2 = &im2col_data[DIM_KER_X * DIM_KER_Y * 2]; float* two_column_buffer_3 = &im2col_data[DIM_KER_X * DIM_KER_Y * 3];
    float* two_column_buffer_4 = &im2col_data[DIM_KER_X * DIM_KER_Y * 4]; float* two_column_buffer_5 = &im2col_data[DIM_KER_X * DIM_KER_Y * 5];
    float* two_column_buffer_6 = &im2col_data[DIM_KER_X * DIM_KER_Y * 6]; float* two_column_buffer_7 = &im2col_data[DIM_KER_X * DIM_KER_Y * 7];
    float* two_column_buffer_8 = &im2col_data[DIM_KER_X * DIM_KER_Y * 8]; float* two_column_buffer_9 = &im2col_data[DIM_KER_X * DIM_KER_Y * 9];
    float* two_column_buffer_10 = &im2col_data[DIM_KER_X * DIM_KER_Y * 10]; float* two_column_buffer_11 = &im2col_data[DIM_KER_X * DIM_KER_Y * 11];
    float* two_column_buffer_12 = &im2col_data[DIM_KER_X * DIM_KER_Y * 12]; float* two_column_buffer_13 = &im2col_data[DIM_KER_X * DIM_KER_Y * 13];
    float* two_column_buffer_14 = &im2col_data[DIM_KER_X * DIM_KER_Y * 14]; float* two_column_buffer_15 = &im2col_data[DIM_KER_X * DIM_KER_Y * 15];
    const float* src_0 = filter_data++; const float* src_1 = filter_data++; const float* src_2 = filter_data++; const float* src_3 = filter_data++;
    const float* src_4 = filter_data++; const float* src_5 = filter_data++; const float* src_6 = filter_data++; const float* src_7 = filter_data++;
    const float* src_8 = filter_data++; const float* src_9 = filter_data++; const float* src_10 = filter_data++; const float* src_11 = filter_data++;
    const float* src_12 = filter_data++; const float* src_13 = filter_data++; const float* src_14 = filter_data++; const float* src_15 = filter_data++;

    for (i = 0; i < DIM_KER_X; i++) {
      for (j = 0; j < DIM_KER_Y; j++) {
        *two_column_buffer_0++ = *src_0; src_0 += output_depth_per_group;
        *two_column_buffer_1++ = *src_1; src_1 += output_depth_per_group;
        *two_column_buffer_2++ = *src_2; src_2 += output_depth_per_group;
        *two_column_buffer_3++ = *src_3; src_3 += output_depth_per_group;
        *two_column_buffer_4++ = *src_4; src_4 += output_depth_per_group;
        *two_column_buffer_5++ = *src_5; src_5 += output_depth_per_group;
        *two_column_buffer_6++ = *src_6; src_6 += output_depth_per_group;
        *two_column_buffer_7++ = *src_7; src_7 += output_depth_per_group;
        *two_column_buffer_8++ = *src_8; src_8 += output_depth_per_group;
        *two_column_buffer_9++ = *src_9; src_9 += output_depth_per_group;
        *two_column_buffer_10++ = *src_10; src_10 += output_depth_per_group;
        *two_column_buffer_11++ = *src_11; src_11 += output_depth_per_group;
        *two_column_buffer_12++ = *src_12; src_12 += output_depth_per_group;
        *two_column_buffer_13++ = *src_13; src_13 += output_depth_per_group;
        *two_column_buffer_14++ = *src_14; src_14 += output_depth_per_group;
        *two_column_buffer_15++ = *src_15; src_15 += output_depth_per_group;
      }
    }

    /* Setup output_weight_data */
    int8_t* out_0 = &output_weight_data[i_output_depth * groups]; int8_t* out_1 = &output_weight_data[(i_output_depth + 1) * groups];
    int8_t* out_2 = &output_weight_data[(i_output_depth + 2) * groups]; int8_t* out_3 = &output_weight_data[(i_output_depth + 3) * groups];
    int8_t* out_4 = &output_weight_data[(i_output_depth + 4) * groups]; int8_t* out_5 = &output_weight_data[(i_output_depth + 5) * groups];
    int8_t* out_6 = &output_weight_data[(i_output_depth + 6) * groups]; int8_t* out_7 = &output_weight_data[(i_output_depth + 7) * groups];
    int8_t* out_8 = &output_weight_data[(i_output_depth + 8) * groups]; int8_t* out_9 = &output_weight_data[(i_output_depth + 9) * groups];
    int8_t* out_10 = &output_weight_data[(i_output_depth + 10) * groups]; int8_t* out_11 = &output_weight_data[(i_output_depth + 11) * groups];
    int8_t* out_12 = &output_weight_data[(i_output_depth + 12) * groups]; int8_t* out_13 = &output_weight_data[(i_output_depth + 13) * groups];
    int8_t* out_14 = &output_weight_data[(i_output_depth + 14) * groups]; int8_t* out_15 = &output_weight_data[(i_output_depth + 15) * groups];

    const float* input = input_data;

    /* Calculate 4 rows(input channels) at a time */
    uint16_t group_cnt = groups >> 2;
    while (group_cnt--) {
      /* Alter the data format of input_data from HWC to CHW and put it into im2col_data buffer */
      two_column_buffer_0 = &im2col_data[DIM_KER_X * DIM_KER_Y * 16];
      two_column_buffer_1 = &im2col_data[DIM_KER_X * DIM_KER_Y * 17];
      two_column_buffer_2 = &im2col_data[DIM_KER_X * DIM_KER_Y * 18];
      two_column_buffer_3 = &im2col_data[DIM_KER_X * DIM_KER_Y * 19];
      src_0 = input++;
      src_1 = input++;
      src_2 = input++;
      src_3 = input++;

      for (i = 0; i < input_height; i++) {
        for (j = 0; j < input_width; j++) {
          *two_column_buffer_0++ = *src_0;
          src_0 += input_depth;
          *two_column_buffer_1++ = *src_1;
          src_1 += input_depth;
          *two_column_buffer_2++ = *src_2;
          src_2 += input_depth;
          *two_column_buffer_3++ = *src_3;
          src_3 += input_depth;
        }
      }

      /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
      const float* input_0 = &im2col_data[DIM_KER_X * DIM_KER_Y * 16];
      const float* input_1 = &im2col_data[DIM_KER_X * DIM_KER_Y * 17];
      const float* input_2 = &im2col_data[DIM_KER_X * DIM_KER_Y * 18];
      const float* input_3 = &im2col_data[DIM_KER_X * DIM_KER_Y * 19];

      const float* filter = im2col_data;

      // We assume bias_data as zeros.
      float sum_0[16] = {};
      float sum_1[16] = {};
      float sum_2[16] = {};
      float sum_3[16] = {};
      
      /* Group Conv Computation */
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[0], &sum_1[0], &sum_2[0], &sum_3[0], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[1], &sum_1[1], &sum_2[1], &sum_3[1], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[2], &sum_1[2], &sum_2[2], &sum_3[2], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[3], &sum_1[3], &sum_2[3], &sum_3[3], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[4], &sum_1[4], &sum_2[4], &sum_3[4], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[5], &sum_1[5], &sum_2[5], &sum_3[5], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[6], &sum_1[6], &sum_2[6], &sum_3[6], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[7], &sum_1[7], &sum_2[7], &sum_3[7], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[8], &sum_1[8], &sum_2[8], &sum_3[8], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[9], &sum_1[9], &sum_2[9], &sum_3[9], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[10], &sum_1[10], &sum_2[10], &sum_3[10], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[11], &sum_1[11], &sum_2[11], &sum_3[11], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[12], &sum_1[12], &sum_2[12], &sum_3[12], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[13], &sum_1[13], &sum_2[13], &sum_3[13], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[14], &sum_1[14], &sum_2[14], &sum_3[14], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[15], &sum_1[15], &sum_2[15], &sum_3[15], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;

      /* Calculate outputs */      
      assign_sum_to_group_output_4row16col(out_0, out_1, out_2, out_3, out_4, out_5, out_6, out_7, out_8, out_9, out_10, out_11, out_12, out_13, out_14, out_15, 
                                    sum_0, sum_1, sum_2, sum_3, output_activation_min, output_activation_max, scales, learning_rate, i_output_depth);
      out_0 += 4; out_1 += 4; out_2 += 4; out_3 += 4; out_4 += 4; out_5 += 4; out_6 += 4; out_7 += 4; 
      out_8 += 4; out_9 += 4; out_10 += 4; out_11 += 4; out_12 += 4; out_13 += 4; out_14 += 4; out_15 += 4; 
    }
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}

tinyengine_status_fp group_conv_fp_kernel4_stride1_pad0_in4x4_out1x1_uniweight_4row8col_inplace(const float* input_data, 
                 const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
                 const float* filter_data, const float* bias_data, 
                 int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
                 const float output_activation_min, const float output_activation_max,
                 float* im2col_data, const uint16_t batches, const uint16_t groups,
                 const float* scales, const float learning_rate) {
  int i_output_depth, i , j;
  int output_depth_per_group = output_depth / groups;

  for (i_output_depth = 0; i_output_depth < output_depth_per_group; i_output_depth += 8) {
    /* Alter the data format of filter_data from IHWO to OHWI and put it into im2col_data buffer */
    float* two_column_buffer_0 = im2col_data; float* two_column_buffer_1 = &im2col_data[DIM_KER_X * DIM_KER_Y];
    float* two_column_buffer_2 = &im2col_data[DIM_KER_X * DIM_KER_Y * 2]; float* two_column_buffer_3 = &im2col_data[DIM_KER_X * DIM_KER_Y * 3];
    float* two_column_buffer_4 = &im2col_data[DIM_KER_X * DIM_KER_Y * 4]; float* two_column_buffer_5 = &im2col_data[DIM_KER_X * DIM_KER_Y * 5];
    float* two_column_buffer_6 = &im2col_data[DIM_KER_X * DIM_KER_Y * 6]; float* two_column_buffer_7 = &im2col_data[DIM_KER_X * DIM_KER_Y * 7];
    const float* src_0 = filter_data++; const float* src_1 = filter_data++; const float* src_2 = filter_data++; const float* src_3 = filter_data++;
    const float* src_4 = filter_data++; const float* src_5 = filter_data++; const float* src_6 = filter_data++; const float* src_7 = filter_data++;

    for (i = 0; i < DIM_KER_X; i++) {
      for (j = 0; j < DIM_KER_Y; j++) {
        *two_column_buffer_0++ = *src_0; src_0 += output_depth_per_group;
        *two_column_buffer_1++ = *src_1; src_1 += output_depth_per_group;
        *two_column_buffer_2++ = *src_2; src_2 += output_depth_per_group;
        *two_column_buffer_3++ = *src_3; src_3 += output_depth_per_group;
        *two_column_buffer_4++ = *src_4; src_4 += output_depth_per_group;
        *two_column_buffer_5++ = *src_5; src_5 += output_depth_per_group;
        *two_column_buffer_6++ = *src_6; src_6 += output_depth_per_group;
        *two_column_buffer_7++ = *src_7; src_7 += output_depth_per_group;
      }
    }

    /* Setup output_weight_data */
    int8_t* out_0 = &output_weight_data[i_output_depth * groups]; int8_t* out_1 = &output_weight_data[(i_output_depth + 1) * groups];
    int8_t* out_2 = &output_weight_data[(i_output_depth + 2) * groups]; int8_t* out_3 = &output_weight_data[(i_output_depth + 3) * groups];
    int8_t* out_4 = &output_weight_data[(i_output_depth + 4) * groups]; int8_t* out_5 = &output_weight_data[(i_output_depth + 5) * groups];
    int8_t* out_6 = &output_weight_data[(i_output_depth + 6) * groups]; int8_t* out_7 = &output_weight_data[(i_output_depth + 7) * groups];

    const float* input = input_data;

    /* Calculate 4 rows(input channels) at a time */
    uint16_t group_cnt = groups >> 2;
    while (group_cnt--) {
      /* Alter the data format of input_data from HWC to CHW and put it into im2col_data buffer */
      two_column_buffer_0 = &im2col_data[DIM_KER_X * DIM_KER_Y * 8];
      two_column_buffer_1 = &im2col_data[DIM_KER_X * DIM_KER_Y * 9];
      two_column_buffer_2 = &im2col_data[DIM_KER_X * DIM_KER_Y * 10];
      two_column_buffer_3 = &im2col_data[DIM_KER_X * DIM_KER_Y * 11];
      src_0 = input++;
      src_1 = input++;
      src_2 = input++;
      src_3 = input++;

      for (i = 0; i < input_height; i++) {
        for (j = 0; j < input_width; j++) {
          *two_column_buffer_0++ = *src_0;
          src_0 += input_depth;
          *two_column_buffer_1++ = *src_1;
          src_1 += input_depth;
          *two_column_buffer_2++ = *src_2;
          src_2 += input_depth;
          *two_column_buffer_3++ = *src_3;
          src_3 += input_depth;
        }
      }

      /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
      const float* input_0 = &im2col_data[DIM_KER_X * DIM_KER_Y * 8];
      const float* input_1 = &im2col_data[DIM_KER_X * DIM_KER_Y * 9];
      const float* input_2 = &im2col_data[DIM_KER_X * DIM_KER_Y * 10];
      const float* input_3 = &im2col_data[DIM_KER_X * DIM_KER_Y * 11];

      const float* filter = im2col_data;

      // We assume bias_data as zeros.
      float sum_0[8] = {};
      float sum_1[8] = {};
      float sum_2[8] = {};
      float sum_3[8] = {};
      
      /* Group Conv Computation */
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[0], &sum_1[0], &sum_2[0], &sum_3[0], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[1], &sum_1[1], &sum_2[1], &sum_3[1], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[2], &sum_1[2], &sum_2[2], &sum_3[2], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[3], &sum_1[3], &sum_2[3], &sum_3[3], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[4], &sum_1[4], &sum_2[4], &sum_3[4], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[5], &sum_1[5], &sum_2[5], &sum_3[5], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[6], &sum_1[6], &sum_2[6], &sum_3[6], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;
      group_mac_kernel4_4row_fp_uniweight_reuse_output_input(&sum_0[7], &sum_1[7], &sum_2[7], &sum_3[7], input_0, input_1, input_2, input_3, filter);
      filter += DIM_KER_X * DIM_KER_Y;

      /* Calculate outputs */      
      assign_sum_to_group_output_4row8col(out_0, out_1, out_2, out_3, out_4, out_5, out_6, out_7, sum_0, sum_1, sum_2, sum_3, 
                                    output_activation_min, output_activation_max, scales, learning_rate, i_output_depth);
      out_0 += 4; out_1 += 4; out_2 += 4; out_3 += 4; out_4 += 4; out_5 += 4; out_6 += 4; out_7 += 4; 
    }
  }

  /* Return to application */
  return STATE_SUCCESS_fp;
}
