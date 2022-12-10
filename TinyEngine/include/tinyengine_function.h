/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   tinyengine_function.h
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

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int8_t q7_t;
typedef uint8_t q8_t;
typedef int16_t q15_t;
typedef uint16_t q16_t;
typedef int32_t q31_t;
typedef uint32_t q32_t;

typedef enum {
    STATE_SUCCESS = 0,    /* No error */
    PARAM_NO_SUPPORT = 1, /* Unsupported parameters */
} tinyengine_status;

typedef struct add_params {
    int input_h, input_w, input_c, left_shift;
    int input1_offset, input1_multiplier, input1_shift;
    int input2_offset, input2_multiplier, input2_shift;
    int output_offset, output_multiplier, output_shift;
    int quantized_activation_max, quantized_activation_min;

} ADD_params;

#define TN_MAX(A, B) ((A) > (B) ? (A) : (B))
#define TN_MIN(A, B) ((A) < (B) ? (A) : (B))

// bit assignment and check
#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL << (b))))  // '!!' to make sure this returns 0 or 1

#define BITMASK_SET(x, mask) ((x) |= (mask))
#define BITMASK_CLEAR(x, mask) ((x) &= (~(mask)))
#define BITMASK_FLIP(x, mask) ((x) ^= (mask))
#define BITMASK_CHECK_ALL(x, mask) (!(~(x) & (mask)))
#define BITMASK_CHECK_ANY(x, mask) ((x) & (mask))

tinyengine_status convolve_1x1_s8(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                  const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
                                  const int32_t *output_shift, const int32_t *output_mult, const int32_t out_offset,
                                  const int32_t input_offset, const int32_t out_activation_min,
                                  const int32_t out_activation_max, q7_t *output, const uint16_t output_x,
                                  const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf);

tinyengine_status convolve_1x1_s8_ch8(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                      const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
                                      const int32_t *output_shift, const int32_t *output_mult, const int32_t out_offset,
                                      const int32_t input_offset, const int32_t out_activation_min,
                                      const int32_t out_activation_max, q7_t *output, const uint16_t output_x,
                                      const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf);

tinyengine_status convolve_1x1_s8_ch16(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                       const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
                                       const int32_t *output_shift, const int32_t *output_mult,
                                       const int32_t out_offset, const int32_t input_offset,
                                       const int32_t out_activation_min, const int32_t out_activation_max, q7_t *output,
                                       const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch,
                                       q15_t *runtime_buf);

tinyengine_status convolve_1x1_s8_ch24(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                       const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
                                       const int32_t *output_shift, const int32_t *output_mult,
                                       const int32_t out_offset, const int32_t input_offset,
                                       const int32_t out_activation_min, const int32_t out_activation_max, q7_t *output,
                                       const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch,
                                       q15_t *runtime_buf);

tinyengine_status convolve_1x1_s8_ch48(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                       const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
                                       const int32_t *output_shift, const int32_t *output_mult,
                                       const int32_t out_offset, const int32_t input_offset,
                                       const int32_t out_activation_min, const int32_t out_activation_max, q7_t *output,
                                       const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch,
                                       q15_t *runtime_buf);

tinyengine_status convolve_s8_kernel3_inputch3_stride2_pad1(
    const q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *output_shift, const int32_t *output_mult, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q15_t *kbuf,
    q7_t pad_value);

tinyengine_status add(int size, ADD_params *params, const int8_t *input1_data, const int8_t *input2_data,
                      int8_t *output_data);

tinyengine_status avg_pooling(const q7_t *input, const uint16_t input_h, const uint16_t input_w, const uint16_t input_c,
                              const uint16_t sample_h, const uint16_t sample_w, const uint16_t output_h,
                              const uint16_t output_w, const int32_t out_activation_min,
                              const int32_t out_activation_max, q7_t *output);

tinyengine_status fully_connected_fp(const float *input, const uint16_t input_x, const uint16_t input_y,
                                     const uint16_t input_ch, const uint16_t output_ch, const float *bias,
                                     const float *weights, float *output);

tinyengine_status statble_softmax_inplace(float *input, const uint16_t length);

tinyengine_status mat_mul_fp(const float *matA, const uint16_t matA_row, const uint16_t matA_col, const float *matB,
                             const uint16_t matB_col, float *output);

tinyengine_status convolve_s8_kernel3_inputch3_stride2_pad1_fpreq(
    const q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const float *scales, const int32_t output_offset, const int32_t input_offset,
    const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output, const uint16_t output_x,
    const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q15_t *kbuf, q7_t pad_value);

tinyengine_status add_fpreq(int size, const int8_t *input1_data, const float input1_scale, const float input1_zero,
                            const int8_t *input2_data, const float input2_scale, const float input2_zero,
                            const float output_scale, const float zero_y, int8_t *output_data);

tinyengine_status add_fpreq_mask(int size, const int8_t *input1_data, const float input1_scale, const float input1_zero,
                                 const int8_t *input2_data, const float input2_scale, const float input2_zero,
                                 const float output_scale, const float zero_y, int8_t *output_data,
                                 int8_t *output_mask);

tinyengine_status add_fpreq_bitmask(int size, const int8_t *input1_data, const float input1_scale,
                                    const float input1_zero, const int8_t *input2_data, const float input2_scale,
                                    const float input2_zero, const float output_scale, const float zero_y,
                                    int8_t *output_data, int8_t *output_mask);

tinyengine_status convolve_1x1_s8_fpreq_mask_partialCH(
    const q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel_sram,
    const q7_t *kernel_flash, const uint16_t first_k_channel, const int32_t *bias, const float *scales,
    const int32_t out_offset, const int32_t input_offset, const int32_t out_activation_min,
    const int32_t out_activation_max, q7_t *output, q7_t *mask, const uint16_t output_x, const uint16_t output_y,
    const uint16_t output_ch, q15_t *runtime_buf);

tinyengine_status patchpadding_convolve_s8_kernel3_inputch3_stride2(
    const q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *output_shift, const int32_t *output_mult, const int32_t output_offset,
    const int32_t input_offset, const int32_t output_activation_min, const int32_t output_activation_max, q7_t *output,
    const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf, q15_t *kbuf,
    q7_t pad_value, const uint16_t pad_t, const uint16_t pad_b, const uint16_t pad_l, const uint16_t pad_r);

tinyengine_status patchpadding_depthwise_kernel3x3_stride1_inplace_CHW(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const int32_t *output_shift, const int32_t *output_mult,
    const int32_t output_offset, const int32_t input_offset, const int32_t output_activation_min,
    const int32_t output_activation_max, q7_t *output, const uint16_t output_x, const uint16_t output_y,
    const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value, const uint16_t pad_t, const uint16_t pad_b,
    const uint16_t pad_l, const uint16_t pad_r);

tinyengine_status patchpadding_depthwise_kernel3x3_stride2_inplace_CHW(
    q7_t *input, const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch, const q7_t *kernel,
    const int32_t *bias, const int32_t *biasR, const int32_t *output_shift, const int32_t *output_mult,
    const int32_t output_offset, const int32_t input_offset, const int32_t output_activation_min,
    const int32_t output_activation_max, q7_t *output, const uint16_t output_x, const uint16_t output_y,
    const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value, const uint16_t pad_t, const uint16_t pad_b,
    const uint16_t pad_l, const uint16_t pad_r);

tinyengine_status element_mult_nx1(const q7_t *input, const uint16_t input_h, const uint16_t input_w,
                                   const uint16_t input_c, const q7_t *input2, const int16_t input1_offset,
                                   const int16_t input2_offset, const int16_t output_offset,
                                   const int32_t out_activation_min, const int32_t out_activation_max,
                                   const float output_scale, q7_t *output);

#include "fp_requantize_op.h"
#include "genInclude.h"
//#include "int8_bp_op.h"
