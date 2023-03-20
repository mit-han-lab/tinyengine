#include <gtest/gtest.h>

#include <tuple>

#include "reference.h"
extern "C" {
#include "tinyengine_function.h"
}

using namespace reference;

// (input_y, input_x, input_ch, input_offset, kernel_y, kernel_x, stride,
// output_y, ouyput_x, output_ch, output_offset)
class Conv1x1 : public ::testing::TestWithParam<std::tuple<uint16_t, uint16_t, uint16_t, int16_t, uint16_t, uint16_t,
                                                           uint16_t, uint16_t, uint16_t, uint16_t, int16_t>> {};

TEST_P(Conv1x1, Tinyegnine) {
    reference::kernel op = reference::kernel();

    const uint16_t input_y = std::get<0>(GetParam()), input_x = std::get<1>(GetParam()),
                   input_ch = std::get<2>(GetParam());
    const uint16_t kernel_y = std::get<4>(GetParam()), kernel_x = std::get<5>(GetParam()),
                   stride = std::get<6>(GetParam());
    const uint16_t output_y = std::get<7>(GetParam()), output_x = std::get<8>(GetParam()),
                   output_ch = std::get<9>(GetParam());
    const q15_t output_offset = std::get<10>(GetParam()), input_offset = std::get<3>(GetParam());
    ;

    q7_t input[input_y * input_x * input_ch];
    q7_t kernel[kernel_x * kernel_y * input_ch * output_ch];
    q7_t reference_output[output_y * output_x * output_ch];
    q7_t output[output_y * output_x * output_ch];
    float scales[output_ch];
    int32_t bias[output_ch];
    q15_t runtime_buf[2 * input_ch * kernel_x * kernel_y];

    op.randomize_q7_vector(input, input_x * input_y * input_ch);
    op.randomize_q7_vector(kernel, kernel_y * kernel_x * input_ch * output_ch);

    op.randomize_fp_vector(scales, output_ch, 2);
    op.randomize_int_vector(bias, output_ch, 1);

    op.naive_conv2d_q7_fpreq(input, input_x, input_y, input_ch, kernel, kernel_x, kernel_y, stride, bias, scales,
                             output_offset, input_offset, -128, 127, reference_output, output_x, output_y, output_ch,
                             runtime_buf);

    convolve_1x1_s8_fpreq(input, input_x, input_y, input_ch, kernel, bias, scales, output_offset, input_offset, -128,
                          127, output, output_x, output_y, output_ch, runtime_buf);

    for (int i = 0; i < output_y * output_x * output_ch; i++) {
        EXPECT_EQ(reference_output[i], output[i]) << "i:" << i << std::endl;
    }
}

// Define the sets of parameters for the parameterized test
// (input_y, input_x, input_ch, input_offset, kernel_y, kernel_x, stride,
// output_y, ouyput_x, output_ch, output_offset)
INSTANTIATE_TEST_SUITE_P(Conv1x1s, Conv1x1,
                         ::testing::Values(std::make_tuple(1, 2, 4, -10, 1, 1, 1, 1, 1, 2, 10),
                                           std::make_tuple(1, 5, 4, 0, 1, 1, 1, 1, 5, 4, 0),
                                           std::make_tuple(10, 10, 20, -10, 1, 1, 1, 10, 10, 10, 10),
                                           std::make_tuple(1, 5, 4, 10, 1, 1, 1, 1, 5, 9, 10),
                                           std::make_tuple(5, 5, 4, 10, 1, 1, 1, 5, 5, 9, 10),
                                           // channel specific
                                           std::make_tuple(5, 5, 4, 8, 1, 1, 1, 5, 5, 9, 10),
                                           std::make_tuple(10, 10, 8, -10, 1, 1, 1, 10, 10, 10, 10),
                                           std::make_tuple(5, 5, 4, 24, 1, 1, 1, 5, 5, 9, 10),
                                           std::make_tuple(10, 10, 24, -10, 1, 1, 1, 10, 10, 10, 10),
                                           std::make_tuple(5, 5, 4, 48, 1, 1, 1, 5, 5, 9, 10),
                                           std::make_tuple(10, 10, 48, -10, 1, 1, 1, 10, 10, 10, 10)));
