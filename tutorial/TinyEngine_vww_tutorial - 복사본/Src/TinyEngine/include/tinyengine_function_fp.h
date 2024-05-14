/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   tinyengine_function_fp.h
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

#include <stdint.h>
#include <complex.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

typedef enum {
	STATE_SUCCESS_fp = 0, /* No error */
	PARAM_NO_SUPPORT_fp = 1, /* Unsupported parameters */
} tinyengine_status_fp;

#define TN_MAX(A,B) ((A) > (B) ? (A) : (B))
#define TN_MIN(A,B) ((A) < (B) ? (A) : (B))

tinyengine_status_fp add_fp(const uint16_t size, const float* input1_data,
		const float* input2_data, float* output_data);

tinyengine_status_fp div_fp(const uint16_t size, const float* input1_data,
		const float* input2_data, float* output_data);

tinyengine_status_fp less(const uint16_t size, const float* input1_data,
		const float* input2_data, bool* output_data);

tinyengine_status_fp LogSoftmax(const float* input_data, const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth);

tinyengine_status_fp mul(const uint16_t size, const float* input1_data,
		const float* input2_data, float* output_data);

tinyengine_status_fp negative(const uint16_t size, const float* input1_data, bool* output_data);

tinyengine_status_fp nll_loss(const float* input_data, const uint16_t input_dim, const uint16_t input_depth, 
		const float* target, const uint16_t target_size, float* output_data);

tinyengine_status_fp strided_slice_3Dto3D(const float* input, const uint16_t input_h, const uint16_t input_w, const uint16_t input_c,
		const uint16_t* begin, const uint16_t* end, const uint16_t* stride, 
		float* output, const uint16_t output_h, const uint16_t output_w, const uint16_t output_c);

tinyengine_status_fp strided_slice_4Dto4D(const float* input, const uint16_t inn, const uint16_t inc, const uint16_t inh, const uint16_t inw,
		const uint16_t* begin, const uint16_t* end, const uint16_t* stride,
		float* output, const uint16_t on, const uint16_t oc, const uint16_t oh, const uint16_t ow);

tinyengine_status_fp sub(const uint16_t size, const float* input1_data,
		const float* input2_data, float* output_data);

tinyengine_status_fp sum_2D(const float* input_data, const uint16_t matA_row,
		const uint16_t matA_col, const uint16_t axis, float* output_data);

tinyengine_status_fp sum_3D(const float* input_data, const uint16_t input_w, const uint16_t input_h,
		const uint16_t input_c, const uint16_t axis, float* output_data);

tinyengine_status_fp sum_4D_exclude(const float* input_data, const uint16_t d1, const uint16_t d2,
		const uint16_t d3, const uint16_t d4, const uint16_t axis, float* output_data);

tinyengine_status_fp tte_exp(const uint16_t size, const float* input_data, float* output_data);

tinyengine_status_fp where(const bool* inMask, const uint16_t size, const float* input1_data,
		const float* input2_data, float* output_data);

tinyengine_status_fp where_zeros(const bool* inMask, const uint16_t size, const float* input1_data, float* output_data);

tinyengine_status_fp where_zeros_inplace(const bool* inMask, const uint16_t size, float* input1_data);

tinyengine_status_fp where_zeros_inplace_bit(const unsigned char* inMask, const uint16_t size, float* input1_data);

tinyengine_status_fp group_pointwise_conv_fp_in1x1_out1x1_1row10col_uniweight_int8input_inplace(const int8_t* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const float* filter_data, const float* bias_data, 
		int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const uint16_t groups,
		const float* scales, const float learning_rate);

tinyengine_status_fp group_pointwise_conv_fp_in1x1_out1x1_1row10col_uniweight_inplace(const float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const float* filter_data, const float* bias_data, 
		int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const uint16_t groups,
		const float* scales, const float learning_rate);

tinyengine_status_fp group_conv_fp_kernel4_stride1_pad0_in4x4_out1x1_uniweight_4row16col_inplace(const float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const float* filter_data, const float* bias_data, 
		int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const uint16_t groups,
		const float* scales, const float learning_rate);

tinyengine_status_fp group_conv_fp_kernel4_stride1_pad0_in4x4_out1x1_uniweight_4row8col_inplace(const float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const float* filter_data, const float* bias_data, 
		int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const uint16_t groups,
		const float* scales, const float learning_rate);

tinyengine_status_fp group_conv_fp_kernel8_stride1_pad0_in8x8_out1x1_uniweight_4row16col_inplace(const float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const float* filter_data, const float* bias_data, 
		int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const uint16_t groups,
		const float* scales, const float learning_rate);

tinyengine_status_fp group_conv_fp_kernel8_stride1_pad0_in8x8_out1x1_uniweight_4row8col_inplace(const float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const float* filter_data, const float* bias_data, 
		int8_t* output_weight_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const uint16_t groups,
		const float* scales, const float learning_rate);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel3_stride1_inpad1_outpad0_IOHW_int8weight_partialCH(float* input_output_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel3_stride1_inpad1_outpad0_IOHW_int8weight(float* input_output_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_data, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel3_stride2_inpad1_outpad1_IOHW_int8weight_partialCH(float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel3_stride2_inpad1_outpad1_IOHW_int8weight(float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_data, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel5_stride1_inpad2_outpad0_IOHW_int8weight_partialCH(float* input_output_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel5_stride1_inpad2_outpad0_IOHW_int8weight(float* input_output_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_data, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel5_stride2_inpad2_outpad1_IOHW_int8weight_partialCH(float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel5_stride2_inpad2_outpad1_IOHW_int8weight(float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_data, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel7_stride1_inpad3_outpad0_IOHW_int8weight_partialCH(float* input_output_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel7_stride1_inpad3_outpad0_IOHW_int8weight(float* input_output_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_data, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel7_stride2_inpad3_outpad1_IOHW_int8weight_partialCH(float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp transpose_depthwise_conv_fp_kernel7_stride2_inpad3_outpad1_IOHW_int8weight(float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_data, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches, const int pad_value);

tinyengine_status_fp pointwise_conv_fp_1row10col_10inputdepth_IOHW_int8weight(const float* input_data, const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_data, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches);

tinyengine_status_fp pointwise_conv_fp_4row4col_IOHW_int8weight(const float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_data, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches);

tinyengine_status_fp pointwise_conv_fp_4row4col_IOHW_int8weight_partialCH_8innercol(const float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches);

tinyengine_status_fp pointwise_conv_fp_4row4col_IOHW_int8weight_partialCH_4innercol(const float* input_data, 
		const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
		const int8_t* filter_sram, const int8_t* filter_flash, const uint16_t first_k_channel, const float* bias_data, 
		float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth, 
		const float output_activation_min, const float output_activation_max,
		float* im2col_data, const uint16_t batches);
