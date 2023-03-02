#include "reference.h"

#include <math.h>
#include <stdlib.h>

namespace reference {
q7_t kernel::get_pixel(int h, int w, int c, int input_h, int input_w, int input_c, const q7_t *input) {
    if (h < 0 || h >= input_h) return 0;
    if (w < 0 || w >= input_w) return 0;

    return input[(h * input_w + w) * input_c + c];
}

void kernel::randomize_q7_vector(q7_t *vector, int length) {
    for (int i = 0; i < length; i++) {
        vector[i] = (rand() % 256) - 128;
    }
}

void kernel::randomize_fp_vector(float *vector, int length, float scale) {
    for (int i = 0; i < length; i++) {
        vector[i] = (float)(rand()) / (float)(RAND_MAX);
        vector[i] *= scale;
    }
}

void kernel::randomize_int_vector(int *vector, int length) {
    for (int i = 0; i < length; i++) {
        vector[i] = rand();
    }
}

void kernel::naive_conv2d_q7_fpreq(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                   const uint16_t input_ch, const q7_t *kernel, const uint16_t kernel_x,
                                   const uint16_t kernel_y, const uint16_t stride, const int32_t *bias,
                                   const float *scales, const int32_t out_offset, const int32_t input_offset,
                                   const int32_t out_activation_min, const int32_t out_activation_max, q7_t *output,
                                   const uint16_t output_x, const uint16_t output_y, const uint16_t output_ch,
                                   q15_t *runtime_buf) {
    for (int h = 0; h < output_y; h++) {
        for (int w = 0; w < output_x; w++) {
            for (int o = 0; o < output_ch; o++) {
                int32_t acc = 0;

                for (int k_h = 0; k_h < kernel_y; k_h++) {
                    for (int k_w = 0; k_w < kernel_x; k_w++) {
                        for (int i_ch = 0; i_ch < input_ch; i_ch++) {
                            int start_y = h * stride;
                            int start_x = w * stride;

                            q7_t pixel = this->get_pixel(start_x + k_h - kernel_y / 2, start_y + k_w - kernel_x / 2,
                                                         i_ch, input_y, input_x, input_ch, input);
                            q15_t offset_pixel = pixel + input_offset;
                            // assume weights are in the HWIO format
                            int weight_idx = ((k_h * kernel_x + k_w) * input_ch + i_ch) * output_ch + o;
                            q7_t kernel_v = kernel[weight_idx];
                            acc += offset_pixel * kernel_v;
                        }
                    }
                }

                // requantize
                acc = (q31_t)((float)acc * scales[o]);
                acc += out_offset + bias[o];
                acc = MAX(acc, out_activation_min);
                acc = MIN(acc, out_activation_max);

                output[(h * output_x + w) * output_ch + o] = (q7_t)acc;
            }
        }
    }
}
}  // namespace reference
