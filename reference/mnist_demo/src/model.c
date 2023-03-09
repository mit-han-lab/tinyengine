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
                     const int32_t output_offset, const int32_t multiplier, const int32_t shift, const float scale) {
    for (int oc = 0; oc < output_ch; oc++) {
        int accumulator = bias[oc];
        for (int ic = 0; ic < input_ch; ic++) {
            accumulator += ((int)input[ic] + input_offset) * (int)filter[oc * input_ch + ic];
        }
        // per-tensor quantization
        // accumulator = requantize_single_rounding(accumulator, multiplier, shift);
        accumulator = (int32_t)((float)accumulator * scale);
        accumulator += output_offset;
        accumulator = accumulator > OUTPUT_MAX ? OUTPUT_MAX : accumulator;
        accumulator = accumulator < OUTPUT_MIN ? OUTPUT_MIN : accumulator;
        output[oc] = (int8_t)(accumulator);
    }
}

signed char* getInput() { return &buffer[0]; }
signed char* getOutput() { return NNoutput; }
void inference() {
    /* layer 0:CONV_2D */
    fully_connected(&buffer[0], 784, weight0, &buffer[784], 300, bias0, 128, -128, 1714425232, -10,
                    0.0007796303326164419);
    // convolve_1x1_s8(&buffer[0],1,1,784,(const q7_t*)
    // weight0,bias0,shift0,multiplier0,-128,128,-128,127,&buffer[784],1,1,300,sbuf);
    /* layer 1:CONV_2D */
    fully_connected(&buffer[784], 300, weight1, &buffer[0], 100, bias1, 128, -128, 1490186346, -8,
                    0.0027106331722312913);
    // convolve_1x1_s8(&buffer[784],1,1,300,(const q7_t*)
    // weight1,bias1,shift1,multiplier1,-128,128,-128,127,&buffer[0],1,1,100,sbuf);
    /* layer 2:CONV_2D */
    fully_connected(&buffer[0], 100, weight1, &buffer[100], 10, bias1, 128, -128, 1349101615, -9, 0.0012270007714017);
    // convolve_1x1_s8(&buffer[0],1,1,100,(const q7_t*)
    // weight2,bias2,shift2,multiplier2,2,128,-128,127,&buffer[100],1,1,10,sbuf);
}
