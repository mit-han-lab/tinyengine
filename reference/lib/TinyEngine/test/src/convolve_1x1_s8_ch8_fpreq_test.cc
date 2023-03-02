#include <gtest/gtest.h>

#include "reference.h"
#include "tinyengine_function.h"

using namespace reference;

TEST(conv_1x1_s8_fpreq, Tinyegnine) {
    reference::kernel op = reference::kernel();

    const uint16_t input_y = 10, input_x = 10, input_ch = 20, input_offset = -10;
    const uint16_t kernel_y = 1, kernel_x = 1, stride = 2;
    const uint16_t output_y = 5, output_x = 5, output_ch = 40, output_offset = 10;

    q7_t input[input_y * input_x * input_ch];
    q7_t kernel[kernel_x * kernel_y * input_ch * output_ch];
    q7_t reference_output[output_y * output_x * output_ch];
    q7_t output[output_y * output_x * output_ch];
    float scales[output_ch];
    int32_t bias[output_ch];
    q15_t runtime_buf[2 * input_ch * kernel_x * kernel_y];

    op.randomize_q7_vector(input, input_x * input_y * input_ch);
    op.randomize_q7_vector(kernel, kernel_y * kernel_x * input_ch * output_ch);
    op.randomize_q7_vector(output, output_y * output_x * output_ch);
    op.randomize_q7_vector(reference_output, output_y * output_x * output_ch);

    op.randomize_fp_vector(scales, output_ch, 1);
    op.randomize_int_vector(bias, output_ch);

    op.naive_conv2d_q7_fpreq(input, input_x, input_y, input_ch, kernel, kernel_x, kernel_y, stride, bias, scales,
                             output_offset, input_offset, -128, 127, reference_output, output_x, output_y, output_ch,
                             runtime_buf);

    convolve_1x1_s8_fpreq(input, input_x, input_y, input_ch, kernel, bias, scales, output_offset, input_offset, -128,
                          127, output, output_x, output_y, output_ch, runtime_buf);

    for (int i = 0; i < output_y * output_x * output_ch; i++) {
        EXPECT_EQ(reference_output[i], output[i]);
    }
}
