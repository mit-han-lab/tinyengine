/* ----------------------------------------------------------------------
 * Name: log_softmax_fp.c
 * Project: TinyEngine, MCUNetV3
 * Contact author: Wei-Chen Wang, wweichen@mit.edu
 * -------------------------------------------------------------------- */

#include "tinyengine_function_fp.h"
#include "tinyengine_function.h"

tinyengine_status_fp LogSoftmax(const float* input_data, const uint16_t input_height, const uint16_t input_width, const uint16_t input_depth, 
                       float* output_data, const uint16_t output_height, const uint16_t output_width, const uint16_t output_depth) {
  const int outer_size = input_height * input_width;
  const int depth = TN_MIN(input_depth, output_depth);

  for (int i = 0; i < outer_size; ++i) {
    float max = FLT_MIN;
    for (int c = 0; c < depth; ++c) {
      max = TN_MAX(max, input_data[i * depth + c]);
    }

    float sum = 0.f;
    for (int c = 0; c < depth; ++c) {
      sum += exp(input_data[i * depth + c] - max);
    }

    const float log_sum = log(sum);
    for (int c = 0; c < depth; ++c) {
      output_data[i * depth + c] = input_data[i * depth + c] - max - log_sum;
    }
  }
  
  /* Return to application */
  return STATE_SUCCESS_fp;
}
