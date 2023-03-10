#include "model.h"

#define OUTPUT_MAX 127
#define OUTPUT_MIN -128

int32_t requantize_single_rounding(const int32_t val, const int32_t multiplier, const int32_t shift) {
    const int64_t total_shift = 31 - shift;
    const int64_t new_val = val * (int64_t)multiplier;

    int32_t result = new_val >> (total_shift - 1);
    result = (result + 1) >> 1;

    return result;
}

void fully_connected(const int8_t* input, const uint16_t input_ch, const int8_t* filter, int8_t* output,
                     const uint16_t output_ch, const int32_t* bias, const int32_t input_offset,
                     const int32_t output_offset, const int32_t multiplier, const int32_t shift) {
    for (int oc = 0; oc < output_ch; oc++) {
        int accumulator = bias[oc];
        for (int ic = 0; ic < input_ch; ic++) {
            accumulator += ((int)input[ic] + input_offset) * (int)filter[oc * input_ch + ic];
        }
        // per-tensor quantization
        accumulator = requantize_single_rounding(accumulator, multiplier, shift);
        accumulator += output_offset;
        accumulator = accumulator > OUTPUT_MAX ? OUTPUT_MAX : accumulator;
        accumulator = accumulator < OUTPUT_MIN ? OUTPUT_MIN : accumulator;
        output[oc] = (int8_t)(accumulator);
    }
}

signed char* getInput() { return &buffer[0]; }
signed char* getOutput() { return NNoutput; }
void inference() {
    fully_connected(&buffer[0], 784, weight0, &buffer[784], 300, bias0, 128, -128, 1714425232, -10);
    fully_connected(&buffer[784], 300, weight1, &buffer[0], 100, bias1, 128, -128, 1490186346, -8);
    fully_connected(&buffer[0], 100, weight2, &buffer[100], 10, bias1, 128, 21, 1349101615, -9);
}
