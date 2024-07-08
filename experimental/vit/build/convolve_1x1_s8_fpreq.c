/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   convolve_1x1_s8_fpreq.c
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

// #include "arm_nnfunctions.h"
// #include "img2col_element.h"
#include <smmintrin.h>

#include "tinyengine_function.h"

#define DIM_KER_X (1U)
#define DIM_KER_Y (1U)

#ifdef MULTITHREADING
#include <pthread.h>
struct thread_args {
    const q7_t *input, *kernel;
    q7_t *output;
    uint16_t input_x, input_y, input_ch, output_x, output_y, output_ch, start_h, end_h;
    const int32_t *bias;
    int32_t out_offset, input_offset;
    const float *scales;
    q15_t *runtime_buf;
};
void *thread_func(void *_args) {
    struct thread_args *args = (struct thread_args *)_args;

    for (int h = args->start_h; h < args->end_h; h++) {
        for (int w = 0; w < args->output_x; w++) {
            for (int o = 0; o < args->output_ch; o++) {
                int32_t acc = args->bias[o];

                for (int i_ch = 0; i_ch < args->input_ch; i_ch++) {
                    int start_y = h;
                    int start_x = w;

                    q7_t pixel =
                        get_pixel(start_y, start_x, i_ch, args->input_y, args->input_x, args->input_ch, args->input);
                    q15_t offset_pixel = (q15_t)pixel + args->input_offset;
                    // assume weights are in the OHWI format
                    int weight_idx = o * args->input_ch + i_ch;
                    q7_t kernel_v = args->kernel[weight_idx];
                    acc += offset_pixel * kernel_v;
                }

                // requantize
                acc = (q31_t)((float)acc * args->scales[o]);
                acc += (q31_t)args->out_offset;
                acc = TN_MAX(acc, -128);
                acc = TN_MIN(acc, 127);

                args->output[(h * args->output_x + w) * args->output_ch + o] = (q7_t)acc;
            }
        }
    }
}
#define NUM_THREAD 4
pthread_t thread_pool[NUM_THREAD];
struct thread_args threads_args[NUM_THREAD];
#endif
#ifdef OPT
#include <pthread.h>
struct thread_args {
    const q7_t *input, *kernel;
    q7_t *output;
    uint16_t input_x, input_y, input_ch, output_x, output_y, output_ch, start_h, end_h;
    const int32_t *bias;
    int32_t out_offset, input_offset;
    const float *scales;
    q15_t *runtime_buf;
};
void *thread_func(void *_args) {
    struct thread_args *args = (struct thread_args *)_args;

    int32_t input_offset_128[4] = {args->input_offset, args->input_offset, args->input_offset, args->input_offset};
    __m128i *input_offset128 = (__m128i *)input_offset_128;
    int32_t input_int32[args->input_ch];

    for (int h = args->start_h; h < args->end_h; h++) {
        for (int w = 0; w < args->output_x; w++) {
            __m128i *input128_ptr = (__m128i *)input_int32;
            const q7_t *input = &args->input[(h * args->input_x + w) * args->input_ch];
            // Load 8bit input to the int32 buffer
            for (int ic = 0; ic < args->input_ch / 16; ic++) {
                __m128i input_vec = _mm_loadu_si128(reinterpret_cast<__m128i *>((int8_t *)input));
                input += 16;

                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(input_vec), *input_offset128);
                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 4)), *input_offset128);
                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 8)), *input_offset128);
                *input128_ptr++ = _mm_add_epi32(_mm_cvtepi8_epi32(_mm_srli_si128(input_vec, 12)), *input_offset128);
            }
            // left over
            if (args->input_ch % 16) {
                for (int ic = args->input_ch / 16 * 16; ic < args->input_ch; ic++) {
                    input_int32[ic] = *input++ + args->input_offset;
                }
            }

            const q7_t *filter = args->kernel;
            for (int o = 0; o < args->output_ch; o++) {
                int32_t acc = args->bias[o];
                int accumulators32[4] = {0, 0, 0, 0};
                __m128i *accumulators = (__m128i *)accumulators32;
                input128_ptr = (__m128i *)input_int32;

                for (int ic = 0; ic < args->input_ch / 16; ic++) {
                    __m128i filter_vec = _mm_loadu_si128(reinterpret_cast<__m128i *>((int8_t *)filter));
                    filter += 16;

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
                if (args->input_ch % 16) {
                    for (int ic = args->input_ch / 16 * 16; ic < args->input_ch; ic += 4) {
                        acc += (input_int32[ic]) * filter[0];
                        acc += (input_int32[ic + 1]) * filter[1];
                        acc += (input_int32[ic + 2]) * filter[2];
                        acc += (input_int32[ic + 3]) * filter[3];
                        filter += 4;
                    }
                }

                // requantize
                acc += accumulators32[0] + accumulators32[1] + accumulators32[2] + accumulators32[3];
                acc = (q31_t)((float)acc * args->scales[o]);
                acc += (q31_t)args->out_offset;
                acc = TN_MAX(acc, -128);
                acc = TN_MIN(acc, 127);

                args->output[(h * args->output_x + w) * args->output_ch + o] = (q7_t)acc;
            }
        }
    }
}
#define NUM_THREAD 4
pthread_t thread_pool[NUM_THREAD];
struct thread_args threads_args[NUM_THREAD];
#endif

tinyengine_status convolve_1x1_s8_fpreq(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                        const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
                                        const float *scales, const int32_t out_offset, const int32_t input_offset,
                                        const int32_t out_activation_min, const int32_t out_activation_max,
                                        q7_t *output, const uint16_t output_x, const uint16_t output_y,
                                        const uint16_t output_ch, q15_t *runtime_buf) {
    int32_t i_element;
    (void)input_x;
    (void)input_y;

#ifdef UNROLL_TILING
    /* Partial(two columns) im2col buffer */
    q15_t *two_column_buffer = runtime_buf;
    q7_t *out = output;
    const int32_t num_elements = output_x * output_y;
    const int channel_div4 = (input_ch >> 2);

    for (i_element = 0; i_element < num_elements / 2; i_element++) {
        /* Fill buffer for partial im2col - two columns at a time */
        const q7_t *src = &input[i_element * input_ch * 2];
        q15_t *dst = two_column_buffer;

        int cnt = channel_div4;  // two columns
        while (cnt > 0) {
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;

            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            cnt--;
        }

        out = mat_mult_kernel_s8_s16_fpreq(kernel, two_column_buffer, output_ch, scales, (q7_t)out_offset,
                                           out_activation_min, out_activation_max, input_ch * DIM_KER_Y * DIM_KER_X,
                                           bias, out);
    }

    /* check if there is an odd column left-over for computation */
    if (num_elements & 0x1) {
        int32_t ch_out;
        const q7_t *ker_a = kernel;
        const q7_t *src = &input[(num_elements - 1) * input_ch];
        q15_t *dst = two_column_buffer;

        int cnt = channel_div4;  // two * numof2col columns
        while (cnt > 0) {
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            *dst++ = *src++ + input_offset;
            cnt--;
        }

        for (ch_out = 0; ch_out < output_ch; ch_out++) {
            q31_t sum = bias[ch_out];

            /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
            const q15_t *ip_as_col = runtime_buf;
            uint16_t col_count = (input_ch * DIM_KER_X * DIM_KER_Y) >> 2;

            while (col_count) {
                sum += *ip_as_col++ * *ker_a++;
                sum += *ip_as_col++ * *ker_a++;
                sum += *ip_as_col++ * *ker_a++;
                sum += *ip_as_col++ * *ker_a++;
                col_count--;
            }

            sum = (float)sum * scales[ch_out];
            sum += out_offset;
            sum = TN_MAX(sum, out_activation_min);
            sum = TN_MIN(sum, out_activation_max);
            *out++ = (q7_t)sum;
        }
    }
#else  // UNROLL_TILING
#ifdef MULTITHREADING
    const int kernel_y = 1, kernel_x = 1, stride = 1;

    if (NUM_THREAD > output_y) {
        for (int h = 0; h < output_y; h++) {
            threads_args[h].input = input;
            threads_args[h].kernel = kernel;
            threads_args[h].output = output;

            threads_args[h].input_x = input_x;
            threads_args[h].input_y = input_y;
            threads_args[h].input_ch = input_ch;

            threads_args[h].output_x = output_x;
            threads_args[h].output_y = output_y;
            threads_args[h].output_ch = output_ch;

            threads_args[h].start_h = h;
            threads_args[h].end_h = h + 1;

            threads_args[h].bias = bias;
            threads_args[h].out_offset = out_offset;
            threads_args[h].input_offset = input_offset;

            threads_args[h].scales = scales;
            threads_args[h].runtime_buf = runtime_buf;

            pthread_create(&thread_pool[h], NULL, thread_func, &threads_args[h]);
        }

        for (int h = 0; h < output_y; h++) {
            pthread_join(thread_pool[h], NULL);
        }
    } else {
        for (int i = 0; i < NUM_THREAD; i++) {
            threads_args[i].input = input;
            threads_args[i].kernel = kernel;
            threads_args[i].output = output;

            threads_args[i].input_x = input_x;
            threads_args[i].input_y = input_y;
            threads_args[i].input_ch = input_ch;

            threads_args[i].output_x = output_x;
            threads_args[i].output_y = output_y;
            threads_args[i].output_ch = output_ch;

            threads_args[i].start_h = i * output_y / NUM_THREAD;
            if (i == NUM_THREAD - 1)
                threads_args[i].end_h = output_y;
            else
                threads_args[i].end_h = (i + 1) * output_y / NUM_THREAD;

            threads_args[i].bias = bias;
            threads_args[i].out_offset = out_offset;
            threads_args[i].input_offset = input_offset;

            threads_args[i].scales = scales;
            threads_args[i].runtime_buf = runtime_buf;

            pthread_create(&thread_pool[i], NULL, thread_func, &threads_args[i]);
        }
        for (int i = 0; i < output_y; i++) {
            pthread_join(thread_pool[i], NULL);
        }
    }

#else  // MULTITHREADING
#ifdef SIMD
    const int kernel_y = 1, kernel_x = 1, stride = 1;

    int32_t input_offset_128[4] = {input_offset, input_offset, input_offset, input_offset};
    __m128i *input_offset128 = (__m128i *)input_offset_128;
    int32_t *input_int32 = (int32_t *)runtime_buf;

    for (int h = 0; h < output_y; h++) {
        for (int w = 0; w < output_x; w++) {
            __m128i *input128_ptr = (__m128i *)input_int32;
            // Load 8bit input to the int32 buffer
            for (int ic = 0; ic < input_ch / 16; ic++) {
                __m128i input_vec = _mm_loadu_si128(reinterpret_cast<__m128i *>((int8_t *)input));
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

            const q7_t *filter = kernel;
            for (int o = 0; o < output_ch; o++) {
                int32_t acc = bias[o];
                int accumulators32[4] = {0, 0, 0, 0};
                __m128i *accumulators = (__m128i *)accumulators32;
                input128_ptr = (__m128i *)input_int32;

                for (int ic = 0; ic < input_ch / 16; ic++) {
                    __m128i filter_vec = _mm_loadu_si128(reinterpret_cast<__m128i *>((int8_t *)filter));
                    filter += 16;

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
                        acc += (input_int32[ic]) * filter[0];
                        acc += (input_int32[ic + 1]) * filter[1];
                        acc += (input_int32[ic + 2]) * filter[2];
                        acc += (input_int32[ic + 3]) * filter[3];
                        filter += 4;
                    }
                }

                // requantize
                acc += accumulators32[0] + accumulators32[1] + accumulators32[2] + accumulators32[3];
                acc = (q31_t)((float)acc * scales[o]);
                acc += (q31_t)out_offset;
                acc = TN_MAX(acc, out_activation_min);
                acc = TN_MIN(acc, out_activation_max);

                output[(h * output_x + w) * output_ch + o] = (q7_t)acc;
            }
        }
    }
#else  // SIMD
#ifdef OPT
    const int kernel_y = 1, kernel_x = 1, stride = 1;

    if (NUM_THREAD > output_y) {
        for (int h = 0; h < output_y; h++) {
            threads_args[h].input = input;
            threads_args[h].kernel = kernel;
            threads_args[h].output = output;

            threads_args[h].input_x = input_x;
            threads_args[h].input_y = input_y;
            threads_args[h].input_ch = input_ch;

            threads_args[h].output_x = output_x;
            threads_args[h].output_y = output_y;
            threads_args[h].output_ch = output_ch;

            threads_args[h].start_h = h;
            threads_args[h].end_h = h + 1;

            threads_args[h].bias = bias;
            threads_args[h].out_offset = out_offset;
            threads_args[h].input_offset = input_offset;

            threads_args[h].scales = scales;
            threads_args[h].runtime_buf = runtime_buf;

            pthread_create(&thread_pool[h], NULL, thread_func, &threads_args[h]);
        }

        for (int h = 0; h < output_y; h++) {
            pthread_join(thread_pool[h], NULL);
        }
    } else {
        for (int i = 0; i < NUM_THREAD; i++) {
            threads_args[i].input = input;
            threads_args[i].kernel = kernel;
            threads_args[i].output = output;

            threads_args[i].input_x = input_x;
            threads_args[i].input_y = input_y;
            threads_args[i].input_ch = input_ch;

            threads_args[i].output_x = output_x;
            threads_args[i].output_y = output_y;
            threads_args[i].output_ch = output_ch;

            threads_args[i].start_h = i * output_y / NUM_THREAD;
            if (i == NUM_THREAD - 1)
                threads_args[i].end_h = output_y;
            else
                threads_args[i].end_h = (i + 1) * output_y / NUM_THREAD;

            threads_args[i].bias = bias;
            threads_args[i].out_offset = out_offset;
            threads_args[i].input_offset = input_offset;

            threads_args[i].scales = scales;
            threads_args[i].runtime_buf = runtime_buf;

            pthread_create(&thread_pool[i], NULL, thread_func, &threads_args[i]);
        }
        for (int i = 0; i < output_y; i++) {
            pthread_join(thread_pool[i], NULL);
        }
    }
#else   // OPT
    const int kernel_y = 1, kernel_x = 1, stride = 1;
    for (int h = 0; h < output_y; h++) {
        for (int w = 0; w < output_x; w++) {
            for (int o = 0; o < output_ch; o++) {
                int32_t acc = bias[o];

                for (int k_h = 0; k_h < kernel_y; k_h++) {
                    for (int k_w = 0; k_w < kernel_x; k_w++) {
                        for (int i_ch = 0; i_ch < input_ch; i_ch++) {
                            int start_y = h * stride;
                            int start_x = w * stride;

                            q7_t pixel = get_pixel(start_y + k_h - kernel_y / 2, start_x + k_w - kernel_x / 2, i_ch,
                                                   input_y, input_x, input_ch, input);
                            q15_t offset_pixel = (q15_t)pixel + input_offset;
                            // assume weights are in the OHWI format
                            int weight_idx = ((o * kernel_y + k_h) * kernel_x + k_w) * input_ch + i_ch;
                            q7_t kernel_v = kernel[weight_idx];
                            acc += offset_pixel * kernel_v;
                        }
                    }
                }

                // requantize
                acc = (q31_t)((float)acc * scales[o]);
                acc += (q31_t)out_offset;
                acc = TN_MAX(acc, out_activation_min);
                acc = TN_MIN(acc, out_activation_max);

                output[(h * output_x + w) * output_ch + o] = (q7_t)acc;
            }
        }
    }
#endif  // OPT
#endif  // SIMD
#endif  // MULTITHREADING
#endif  // UNROLL_TILING

    /* Return to application */
    return STATE_SUCCESS;
}
