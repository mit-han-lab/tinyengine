#include "model.h"

#include <assert.h>
#include <smmintrin.h>
#include <stdio.h>

#define OUTPUT_MAX 127
#define OUTPUT_MIN -128
#define MAX_INPUT_CH 784
int32_t input_int32[MAX_INPUT_CH];

int32_t requantize_single_rounding(const int32_t val, const int32_t multiplier, const int32_t shift) {
    const int64_t total_shift = 31 - shift;
    const int64_t new_val = val * (int64_t)multiplier;

    int32_t result = new_val >> (total_shift - 1);
    result = (result + 1) >> 1;

    return result;
}

void fully_connected(const int8_t* input, const uint16_t input_ch, const int8_t* filter, int8_t* output,
                     const uint16_t output_ch, const int32_t input_offset, const int32_t output_offset,
                     const int32_t multiplier, const int32_t shift, int imp_choice) {
    if (imp_choice == unroll) {
        assert(input_ch % 4 == 0);
        for (int oc = 0; oc < output_ch; oc++) {
            int accumulator = 0;
            const int8_t* input_ptr = input;
            for (int ic = 0; ic < input_ch; ic += 4) {
                accumulator += ((int)input_ptr[0] + input_offset) * filter[0];
                accumulator += ((int)input_ptr[1] + input_offset) * filter[1];
                accumulator += ((int)input_ptr[2] + input_offset) * filter[2];
                accumulator += ((int)input_ptr[3] + input_offset) * filter[3];
                filter += 4;
                input_ptr += 4;
            }
            // per-tensor quantization
            accumulator = requantize_single_rounding(accumulator, multiplier, shift);
            accumulator += output_offset;
            accumulator = accumulator > OUTPUT_MAX ? OUTPUT_MAX : accumulator;
            accumulator = accumulator < OUTPUT_MIN ? OUTPUT_MIN : accumulator;
            output[oc] = (int8_t)(accumulator);
        }
    } else if (imp_choice == unroll_simd) {
        assert(input_ch % 4 == 0);

        __m128i* input128_ptr = (__m128i*)input_int32;
        int32_t input_offset_128[4] = {input_offset, input_offset, input_offset, input_offset};
        __m128i* input_offset128 = (__m128i*)input_offset_128;

        // Load 8bit input to the int32 buffer and
        for (int ic = 0; ic < input_ch / 16; ic++) {
            __m128i input_vec = _mm_load_si128(reinterpret_cast<__m128i*>((int8_t*)input));
            input += 16;

            *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(input_vec), *input_offset128);
            *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 4)), *input_offset128);
            *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 8)), *input_offset128);
            *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 12)), *input_offset128);
        }
        // left over
        if (input_ch % 16) {
            for (int ic = input_ch / 16 * 16; ic < input_ch; ic++) {
                input_int32[ic] = *input++ + input_offset;
            }
        }

        for (int oc = 0; oc < output_ch; oc++) {
            int accumulator = 0;
            int accumulators32[4] = {0, 0, 0, 0};
            __m128i* accumulators = (__m128i*)accumulators32;
            input128_ptr = (__m128i*)input_int32;

            for (int ic = 0; ic < input_ch / 16; ic++) {
                __m128i filter_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)filter));
                filter += 16;

                *accumulators =
                    _mm_add_epi32(_mm_mullo_epi32(_mm_cvtepi8_epi32(filter_vec), *input128_ptr++), *accumulators);
                *accumulators = _mm_add_epi32(
                    _mm_mullo_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 4)), *input128_ptr++), *accumulators);
                *accumulators = _mm_add_epi32(
                    _mm_mullo_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 8)), *input128_ptr++), *accumulators);
                *accumulators = _mm_add_epi32(
                    _mm_mullo_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 12)), *input128_ptr++), *accumulators);
            }
            if (input_ch % 16) {
                for (int ic = input_ch / 16 * 16; ic < input_ch; ic += 4) {
                    accumulator += (input_int32[ic]) * filter[0];
                    accumulator += (input_int32[ic + 1]) * filter[1];
                    accumulator += (input_int32[ic + 2]) * filter[2];
                    accumulator += (input_int32[ic + 3]) * filter[3];
                    filter += 4;
                }
            }
            // per-tensor quantization
            accumulator += accumulators32[0] + accumulators32[1] + accumulators32[2] + accumulators32[3];
            accumulator = requantize_single_rounding(accumulator, multiplier, shift);
            accumulator += output_offset;
            accumulator = accumulator > OUTPUT_MAX ? OUTPUT_MAX : accumulator;
            accumulator = accumulator < OUTPUT_MIN ? OUTPUT_MIN : accumulator;
            output[oc] = (int8_t)(accumulator);
        }
    } else if (imp_choice == naive)
        for (int oc = 0; oc < output_ch; oc++) {
            int accumulator = 0;
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
void inference(int imp_choice) {
    fully_connected(&buffer[0], 784, weight0, &buffer[784], 300, 128, -128, 1714425232, -10, imp_choice);
    fully_connected(&buffer[784], 300, weight1, &buffer[0], 100, 128, -128, 1490186346, -8, imp_choice);
    fully_connected(&buffer[0], 100, weight2, &buffer[100], 10, 128, 21, 1349101615, -9, imp_choice);
}
