#include "batch_model.h"

#include <assert.h>
#include <smmintrin.h>
#include <stdio.h>

#define OUTPUT_MAX 127
#define OUTPUT_MIN -128

int32_t requantize_single_rounding(const int32_t val, const int32_t multiplier, const int32_t shift) {
    const int64_t total_shift = 31 - shift;
    const int64_t new_val = val * (int64_t)multiplier;

    int32_t result = new_val >> (total_shift - 1);
    result = (result + 1) >> 1;

    return result;
}

void batch_fully_connected(const uint16_t batch_size, const int8_t* input, const uint16_t input_ch,
                           const int8_t* filter, int8_t* output, const uint16_t output_ch, const int32_t input_offset,
                           const int32_t output_offset, const int32_t multiplier, const int32_t shift, int imp_choice) {
    if (imp_choice == unroll) {
        for (int b = 0; b < batch_size; b++) {
            const int8_t* filter_ptr = filter;
            for (int oc = 0; oc < output_ch; oc++) {
                int accumulator = 0;
                const int8_t* input_ptr = &input[b * input_ch];
                for (int ic = 0; ic < input_ch; ic += 4) {
                    accumulator += ((int)input_ptr[0] + input_offset) * filter_ptr[0];
                    accumulator += ((int)input_ptr[1] + input_offset) * filter_ptr[1];
                    accumulator += ((int)input_ptr[2] + input_offset) * filter_ptr[2];
                    accumulator += ((int)input_ptr[3] + input_offset) * filter_ptr[3];
                    filter_ptr += 4;
                    input_ptr += 4;
                }
                // per-tensor quantization
                accumulator = requantize_single_rounding(accumulator, multiplier, shift);
                accumulator += output_offset;
                accumulator = accumulator > OUTPUT_MAX ? OUTPUT_MAX : accumulator;
                accumulator = accumulator < OUTPUT_MIN ? OUTPUT_MIN : accumulator;
                output[b * output_ch + oc] = (int8_t)(accumulator);
            }
        }
    } else if (imp_choice == unroll_simd_tiling) {
        assert(input_ch % 4 == 0);
        int b = 0;
        int32_t input_offset_128[4] = {input_offset, input_offset, input_offset, input_offset};
        __m128i* input_offset128 = (__m128i*)input_offset_128;
        int32_t b0_input_int32[input_ch], b1_input_int32[input_ch], b2_input_int32[input_ch],
            b3_input_int32[input_ch];  // b0, b1

        for (; b < batch_size - 3; b += 4) {
            __m128i *b0_input128_ptr = (__m128i*)b0_input_int32, *b1_input128_ptr = (__m128i*)b1_input_int32,
                    *b2_input128_ptr = (__m128i*)b2_input_int32, *b3_input128_ptr = (__m128i*)b3_input_int32;

            // Load 8bit input to the int32 buffer and
            const int8_t* b0_input_ptr = &input[b * input_ch];
            const int8_t* b1_input_ptr = &input[(b + 1) * input_ch];
            const int8_t* b2_input_ptr = &input[(b + 2) * input_ch];
            const int8_t* b3_input_ptr = &input[(b + 3) * input_ch];
            for (int ic = 0; ic < input_ch / 16; ic++) {
                __m128i b0_input_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)b0_input_ptr));
                __m128i b1_input_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)b1_input_ptr));
                __m128i b2_input_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)b2_input_ptr));
                __m128i b3_input_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)b3_input_ptr));
                b0_input_ptr += 16;
                b1_input_ptr += 16;
                b2_input_ptr += 16;
                b3_input_ptr += 16;

                *b0_input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(b0_input_vec), *input_offset128);
                *b0_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b0_input_vec, 4)), *input_offset128);
                *b0_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b0_input_vec, 8)), *input_offset128);
                *b0_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b0_input_vec, 12)), *input_offset128);

                *b1_input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(b1_input_vec), *input_offset128);
                *b1_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b1_input_vec, 4)), *input_offset128);
                *b1_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b1_input_vec, 8)), *input_offset128);
                *b1_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b1_input_vec, 12)), *input_offset128);

                *b2_input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(b2_input_vec), *input_offset128);
                *b2_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b2_input_vec, 4)), *input_offset128);
                *b2_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b2_input_vec, 8)), *input_offset128);
                *b2_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b2_input_vec, 12)), *input_offset128);

                *b3_input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(b3_input_vec), *input_offset128);
                *b3_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b3_input_vec, 4)), *input_offset128);
                *b3_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b3_input_vec, 8)), *input_offset128);
                *b3_input128_ptr++ =
                    _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(b3_input_vec, 12)), *input_offset128);
            }
            // left over
            if (input_ch % 16) {
                for (int ic = input_ch / 16 * 16; ic < input_ch; ic++) {
                    b0_input_int32[ic] = *b0_input_ptr++ + input_offset;
                    b1_input_int32[ic] = *b1_input_ptr++ + input_offset;
                    b2_input_int32[ic] = *b2_input_ptr++ + input_offset;
                    b3_input_int32[ic] = *b3_input_ptr++ + input_offset;
                }
            }

            int8_t* filter_ptr = (int8_t*)filter;
            for (int oc = 0; oc < output_ch; oc++) {
                int b0_accumulator = 0, b1_accumulator = 0, b2_accumulator = 0, b3_accumulator = 0;
                int b0_accumulators32[4] = {0, 0, 0, 0}, b1_accumulators32[4] = {0, 0, 0, 0},
                    b2_accumulators32[4] = {0, 0, 0, 0}, b3_accumulators32[4] = {0, 0, 0, 0};
                __m128i *b0_accumulators = (__m128i*)b0_accumulators32, *b1_accumulators = (__m128i*)b1_accumulators32,
                        *b2_accumulators = (__m128i*)b2_accumulators32, *b3_accumulators = (__m128i*)b3_accumulators32;
                b0_input128_ptr = (__m128i*)b0_input_int32;
                b1_input128_ptr = (__m128i*)b1_input_int32;
                b2_input128_ptr = (__m128i*)b2_input_int32;
                b3_input128_ptr = (__m128i*)b3_input_int32;

                for (int ic = 0; ic < input_ch / 16; ic++) {
                    __m128i filter_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)filter_ptr));
                    __m128i filter_val;
                    filter_ptr += 16;

                    filter_val = _mm_cvtepi8_epi32(filter_vec);
                    *b0_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b0_input128_ptr++), *b0_accumulators);
                    *b1_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b1_input128_ptr++), *b1_accumulators);
                    *b2_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b2_input128_ptr++), *b2_accumulators);
                    *b3_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b3_input128_ptr++), *b3_accumulators);
                    filter_val = _mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 4));
                    *b0_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b0_input128_ptr++), *b0_accumulators);
                    *b1_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b1_input128_ptr++), *b1_accumulators);
                    *b2_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b2_input128_ptr++), *b2_accumulators);
                    *b3_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b3_input128_ptr++), *b3_accumulators);
                    filter_val = _mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 8));
                    *b0_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b0_input128_ptr++), *b0_accumulators);
                    *b1_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b1_input128_ptr++), *b1_accumulators);
                    *b2_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b2_input128_ptr++), *b2_accumulators);
                    *b3_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b3_input128_ptr++), *b3_accumulators);
                    filter_val = _mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 12));
                    *b0_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b0_input128_ptr++), *b0_accumulators);
                    *b1_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b1_input128_ptr++), *b1_accumulators);
                    *b2_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b2_input128_ptr++), *b2_accumulators);
                    *b3_accumulators = _mm_add_epi32(_mm_mullo_epi32(filter_val, *b3_input128_ptr++), *b3_accumulators);
                }
                if (input_ch % 16) {
                    for (int ic = input_ch / 16 * 16; ic < input_ch; ic += 4) {
                        int8_t filter_val = filter_ptr[0];
                        b0_accumulator += (b0_input_int32[ic]) * filter_val;
                        b1_accumulator += (b1_input_int32[ic]) * filter_val;
                        b2_accumulator += (b2_input_int32[ic]) * filter_val;
                        b3_accumulator += (b3_input_int32[ic]) * filter_val;
                        filter_val = filter_ptr[1];
                        b0_accumulator += (b0_input_int32[ic + 1]) * filter_val;
                        b1_accumulator += (b1_input_int32[ic + 1]) * filter_val;
                        b2_accumulator += (b2_input_int32[ic + 1]) * filter_val;
                        b3_accumulator += (b3_input_int32[ic + 1]) * filter_val;
                        filter_val = filter_ptr[2];
                        b0_accumulator += (b0_input_int32[ic + 2]) * filter_val;
                        b1_accumulator += (b1_input_int32[ic + 2]) * filter_val;
                        b2_accumulator += (b2_input_int32[ic + 2]) * filter_val;
                        b3_accumulator += (b3_input_int32[ic + 2]) * filter_val;
                        filter_val = filter_ptr[3];
                        b0_accumulator += (b0_input_int32[ic + 3]) * filter_val;
                        b1_accumulator += (b1_input_int32[ic + 3]) * filter_val;
                        b2_accumulator += (b2_input_int32[ic + 3]) * filter_val;
                        b3_accumulator += (b3_input_int32[ic + 3]) * filter_val;
                        filter_ptr += 4;
                    }
                }
                // per-tensor quantization
                b0_accumulator +=
                    b0_accumulators32[0] + b0_accumulators32[1] + b0_accumulators32[2] + b0_accumulators32[3];
                b0_accumulator = requantize_single_rounding(b0_accumulator, multiplier, shift);
                b0_accumulator += output_offset;
                b0_accumulator = b0_accumulator > OUTPUT_MAX ? OUTPUT_MAX : b0_accumulator;
                b0_accumulator = b0_accumulator < OUTPUT_MIN ? OUTPUT_MIN : b0_accumulator;
                output[b * output_ch + oc] = (int8_t)(b0_accumulator);

                b1_accumulator +=
                    b1_accumulators32[0] + b1_accumulators32[1] + b1_accumulators32[2] + b1_accumulators32[3];
                b1_accumulator = requantize_single_rounding(b1_accumulator, multiplier, shift);
                b1_accumulator += output_offset;
                b1_accumulator = b1_accumulator > OUTPUT_MAX ? OUTPUT_MAX : b1_accumulator;
                b1_accumulator = b1_accumulator < OUTPUT_MIN ? OUTPUT_MIN : b1_accumulator;
                output[(b + 1) * output_ch + oc] = (int8_t)(b1_accumulator);

                b2_accumulator +=
                    b2_accumulators32[0] + b2_accumulators32[1] + b2_accumulators32[2] + b2_accumulators32[3];
                b2_accumulator = requantize_single_rounding(b2_accumulator, multiplier, shift);
                b2_accumulator += output_offset;
                b2_accumulator = b2_accumulator > OUTPUT_MAX ? OUTPUT_MAX : b2_accumulator;
                b2_accumulator = b2_accumulator < OUTPUT_MIN ? OUTPUT_MIN : b2_accumulator;
                output[(b + 2) * output_ch + oc] = (int8_t)(b2_accumulator);

                b3_accumulator +=
                    b3_accumulators32[0] + b3_accumulators32[1] + b3_accumulators32[2] + b3_accumulators32[3];
                b3_accumulator = requantize_single_rounding(b3_accumulator, multiplier, shift);
                b3_accumulator += output_offset;
                b3_accumulator = b3_accumulator > OUTPUT_MAX ? OUTPUT_MAX : b3_accumulator;
                b3_accumulator = b3_accumulator < OUTPUT_MIN ? OUTPUT_MIN : b3_accumulator;
                output[(b + 3) * output_ch + oc] = (int8_t)(b3_accumulator);
            }
        }

        // leftover
        for (; b < batch_size; b++) {
            int32_t input_int32[input_ch];
            __m128i* input128_ptr = (__m128i*)input_int32;
            int32_t input_offset_128[4] = {input_offset, input_offset, input_offset, input_offset};
            __m128i* input_offset128 = (__m128i*)input_offset_128;

            // Load 8bit input to the int32 buffer and
            const int8_t* input_ptr = &input[b * input_ch];
            for (int ic = 0; ic < input_ch / 16; ic++) {
                __m128i input_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)input_ptr));
                input_ptr += 16;

                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(input_vec), *input_offset128);
                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 4)), *input_offset128);
                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 8)), *input_offset128);
                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 12)), *input_offset128);
            }
            // left over
            if (input_ch % 16) {
                for (int ic = input_ch / 16 * 16; ic < input_ch; ic++) {
                    input_int32[ic] = *input_ptr++ + input_offset;
                }
            }

            int8_t* filter_ptr = (int8_t*)filter;
            for (int oc = 0; oc < output_ch; oc++) {
                int accumulator = 0;
                int accumulators32[4] = {0, 0, 0, 0};
                __m128i* accumulators = (__m128i*)accumulators32;
                input128_ptr = (__m128i*)input_int32;

                for (int ic = 0; ic < input_ch / 16; ic++) {
                    __m128i filter_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)filter_ptr));
                    filter_ptr += 16;

                    *accumulators =
                        _mm_add_epi32(_mm_mullo_epi32(_mm_cvtepi8_epi32(filter_vec), *input128_ptr++), *accumulators);
                    *accumulators = _mm_add_epi32(
                        _mm_mullo_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 4)), *input128_ptr++),
                        *accumulators);
                    *accumulators = _mm_add_epi32(
                        _mm_mullo_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 8)), *input128_ptr++),
                        *accumulators);
                    *accumulators = _mm_add_epi32(
                        _mm_mullo_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 12)), *input128_ptr++),
                        *accumulators);
                }
                if (input_ch % 16) {
                    for (int ic = input_ch / 16 * 16; ic < input_ch; ic += 4) {
                        accumulator += (input_int32[ic]) * filter_ptr[0];
                        accumulator += (input_int32[ic + 1]) * filter_ptr[1];
                        accumulator += (input_int32[ic + 2]) * filter_ptr[2];
                        accumulator += (input_int32[ic + 3]) * filter_ptr[3];
                        filter_ptr += 4;
                    }
                }
                // per-tensor quantization
                accumulator += accumulators32[0] + accumulators32[1] + accumulators32[2] + accumulators32[3];
                accumulator = requantize_single_rounding(accumulator, multiplier, shift);
                accumulator += output_offset;
                accumulator = accumulator > OUTPUT_MAX ? OUTPUT_MAX : accumulator;
                accumulator = accumulator < OUTPUT_MIN ? OUTPUT_MIN : accumulator;
                output[b * output_ch + oc] = (int8_t)(accumulator);
            }
        }
    } else if (imp_choice == unroll_simd) {
        assert(input_ch % 4 == 0);
        for (int b = 0; b < batch_size; b++) {
            int32_t input_int32[input_ch];
            __m128i* input128_ptr = (__m128i*)input_int32;
            int32_t input_offset_128[4] = {input_offset, input_offset, input_offset, input_offset};
            __m128i* input_offset128 = (__m128i*)input_offset_128;

            // Load 8bit input to the int32 buffer and
            const int8_t* input_ptr = &input[b * input_ch];
            for (int ic = 0; ic < input_ch / 16; ic++) {
                __m128i input_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)input_ptr));
                input_ptr += 16;

                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(input_vec), *input_offset128);
                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 4)), *input_offset128);
                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 8)), *input_offset128);
                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 12)), *input_offset128);
            }
            // left over
            if (input_ch % 16) {
                for (int ic = input_ch / 16 * 16; ic < input_ch; ic++) {
                    input_int32[ic] = *input_ptr++ + input_offset;
                }
            }

            int8_t* filter_ptr = (int8_t*)filter;
            for (int oc = 0; oc < output_ch; oc++) {
                int accumulator = 0;
                int accumulators32[4] = {0, 0, 0, 0};
                __m128i* accumulators = (__m128i*)accumulators32;
                input128_ptr = (__m128i*)input_int32;

                for (int ic = 0; ic < input_ch / 16; ic++) {
                    __m128i filter_vec = _mm_loadu_si128(reinterpret_cast<__m128i*>((int8_t*)filter_ptr));
                    filter_ptr += 16;

                    *accumulators =
                        _mm_add_epi32(_mm_mullo_epi32(_mm_cvtepi8_epi32(filter_vec), *input128_ptr++), *accumulators);
                    *accumulators = _mm_add_epi32(
                        _mm_mullo_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 4)), *input128_ptr++),
                        *accumulators);
                    *accumulators = _mm_add_epi32(
                        _mm_mullo_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 8)), *input128_ptr++),
                        *accumulators);
                    *accumulators = _mm_add_epi32(
                        _mm_mullo_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(filter_vec, 12)), *input128_ptr++),
                        *accumulators);
                }
                if (input_ch % 16) {
                    for (int ic = input_ch / 16 * 16; ic < input_ch; ic += 4) {
                        accumulator += (input_int32[ic]) * filter_ptr[0];
                        accumulator += (input_int32[ic + 1]) * filter_ptr[1];
                        accumulator += (input_int32[ic + 2]) * filter_ptr[2];
                        accumulator += (input_int32[ic + 3]) * filter_ptr[3];
                        filter_ptr += 4;
                    }
                }
                // per-tensor quantization
                accumulator += accumulators32[0] + accumulators32[1] + accumulators32[2] + accumulators32[3];
                accumulator = requantize_single_rounding(accumulator, multiplier, shift);
                accumulator += output_offset;
                accumulator = accumulator > OUTPUT_MAX ? OUTPUT_MAX : accumulator;
                accumulator = accumulator < OUTPUT_MIN ? OUTPUT_MIN : accumulator;
                output[b * output_ch + oc] = (int8_t)(accumulator);
            }
        }
    } else if (imp_choice == naive) {
        for (int b = 0; b < batch_size; b++) {
            const int8_t* input_ptr = &input[b * input_ch];
            for (int oc = 0; oc < output_ch; oc++) {
                int accumulator = 0;
                for (int ic = 0; ic < input_ch; ic++) {
                    accumulator += ((int)input_ptr[ic] + input_offset) * (int)filter[oc * input_ch + ic];
                }
                // per-tensor quantization
                accumulator = requantize_single_rounding(accumulator, multiplier, shift);
                accumulator += output_offset;
                accumulator = accumulator > OUTPUT_MAX ? OUTPUT_MAX : accumulator;
                accumulator = accumulator < OUTPUT_MIN ? OUTPUT_MIN : accumulator;
                output[b * output_ch + oc] = (int8_t)(accumulator);
            }
        }
    }
}

signed char* getInput() { return &buffer[0]; }
signed char* getOutput() { return NNoutput; }
void batch_inference(int batch_size, int imp_choice) {
    assert(MAX_BATCH_SIZE > batch_size);
    batch_fully_connected(batch_size, &buffer[0 * MAX_BATCH_SIZE], 784, weight0, &buffer[784 * MAX_BATCH_SIZE], 300,
                          128, -128, 1521392405, -10, imp_choice);
    batch_fully_connected(batch_size, &buffer[784 * MAX_BATCH_SIZE], 300, weight1, &buffer[0 * MAX_BATCH_SIZE], 100,
                          128, -128, 1594643846, -8, imp_choice);
    batch_fully_connected(batch_size, &buffer[0 * MAX_BATCH_SIZE], 100, weight2, &buffer[100 * MAX_BATCH_SIZE], 10, 128,
                          16, 1275591344, -9, imp_choice);
}
