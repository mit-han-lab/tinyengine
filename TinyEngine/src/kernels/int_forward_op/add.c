/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   add.c
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#include <math.h>
#include "arm_math.h"
#include "tinyengine_function.h"

int32_t Add(int32_t a, int32_t b) {
  return a + b;
}
int32_t ShiftRight(int32_t a, int offset) {
  return a >> offset;
}
int32_t BitAnd(int32_t a, int32_t b) {
  return a & b;
}
int32_t BitNot(int32_t a) {
  return ~a;
}
int32_t MaskIfNonZero(int32_t a) {
  static const int32_t zero = 0;
  return a ? BitNot(zero) : zero;
}
int32_t MaskIfGreaterThan(int32_t a, int32_t b) {
  return MaskIfNonZero(a > b);
}
int32_t MaskIfLessThan(int32_t a, int32_t b) {
  return MaskIfNonZero(a < b);
}

static inline int32_t SaturatingRoundingDoublingHighMul(int32_t a, int32_t b) {
  int64_t a_64 = a;
  int64_t b_64 = b;
  int64_t ab_64 = a_64 * b_64;
  int32_t nudge = ab_64 >= 0 ? (1 << 30) : (1 - (1 << 30));
  int32_t ab_x2_high32 = (int32_t)((ab_64 + nudge) / (1ll << 31));
  return a == b && a == -2147483648 ? 2147483647 : ab_x2_high32;
}

static inline  int32_t RoundingDivideByPOT(int32_t x, int exponent) {
  const int32_t mask = ((1ll << exponent) - 1);
  const int32_t zero = (0);
  const int32_t one = (1);
  const int32_t remainder = BitAnd(x, mask);
  const int32_t threshold = Add(ShiftRight(mask, 1), BitAnd(MaskIfLessThan(x, zero), one));
  return Add(ShiftRight(x, exponent), BitAnd(MaskIfGreaterThan(remainder, threshold), one));
}

static inline int32_t MultiplyByQuantizedMultiplierSmallerThanOneExp(
		int32_t x, int32_t quantized_multiplier, int left_shift) {
  return RoundingDivideByPOT(
      SaturatingRoundingDoublingHighMul(x, quantized_multiplier), -left_shift);
}

tinyengine_status add(int size, ADD_params* params, const int8_t* input1_data,
			const int8_t* input2_data, int8_t* output_data) {
  for (int i = 0; i < size; ++i) {
    const int32_t input1_val = params->input1_offset + input1_data[i];
    const int32_t input2_val = params->input2_offset + input2_data[i];
    const int32_t shifted_input1_val = input1_val * (1 << params->left_shift);
    const int32_t shifted_input2_val = input2_val * (1 << params->left_shift);
    const int32_t scaled_input1_val =
        MultiplyByQuantizedMultiplierSmallerThanOneExp(
            shifted_input1_val, params->input1_multiplier, params->input1_shift);
    const int32_t scaled_input2_val =
        MultiplyByQuantizedMultiplierSmallerThanOneExp(
            shifted_input2_val, params->input2_multiplier, params->input2_shift);
    const int32_t raw_sum = scaled_input1_val + scaled_input2_val;
    const int32_t raw_output =
        MultiplyByQuantizedMultiplierSmallerThanOneExp(
            raw_sum, params->output_multiplier, params->output_shift) +
        params->output_offset;
    const int32_t clamped_output = TN_MIN(params->quantized_activation_max,
    		TN_MAX(params->quantized_activation_min, raw_output));
    output_data[i] = (int8_t)(clamped_output);
  }
}
