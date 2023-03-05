#ifndef TINYENGINE_INCLUDE_REFERENCE_H_
#define TINYENGINE_INCLUDE_REFERENCE_H_

#include <stdint.h>
typedef int8_t q7_t;
typedef uint8_t q8_t;
typedef int16_t q15_t;
typedef uint16_t q16_t;
typedef int32_t q31_t;
typedef uint32_t q32_t;

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

namespace reference {
class kernel {
   public:
    void randomize_q7_vector(q7_t *vector, int length);
    void randomize_fp_vector(float *vector, int length, float scale);
    void randomize_int_vector(int32_t *vector, int length, int max);
    void naive_conv2d_q7_fpreq(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                               const uint16_t input_ch, const q7_t *kernel, const uint16_t kernel_y,
                               const uint16_t kernel_x, const uint16_t stride, const int32_t *bias, const float *scales,
                               const int32_t out_offset, const int32_t input_offset, const int32_t out_activation_min,
                               const int32_t out_activation_max, q7_t *output, const uint16_t output_x,
                               const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf);

   private:
    q7_t get_pixel(int h, int w, int c, int input_h, int input_w, int input_c, const q7_t *input);
};
}  // namespace reference

#endif
