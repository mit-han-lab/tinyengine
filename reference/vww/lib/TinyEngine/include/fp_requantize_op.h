/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   fp_requantize_op.h
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
#ifndef TINYENGINE_INCLUDE_FP_REQUANTIZE_OP_H_
#define TINYENGINE_INCLUDE_FP_REQUANTIZE_OP_H_

tinyengine_status convolve_1x1_s8_fpreq(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                        const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
                                        const float *scales, const int32_t out_offset, const int32_t input_offset,
                                        const int32_t out_activation_min, const int32_t out_activation_max,
                                        q7_t *output, const uint16_t output_x, const uint16_t output_y,
                                        const uint16_t output_ch, q15_t *runtime_buf);

q7_t *mat_mult_kernel_s8_s16_fpreq(const q7_t *input_a, const q15_t *input_b, const uint16_t output_ch,
                                   const float *scales, const int32_t out_offset, const int16_t activation_min,
                                   const int16_t activation_max, const uint16_t num_col_a,
                                   const int32_t *const output_bias, q7_t *out_0);

q7_t *mat_mult_kernel_s8_s16_multithread_fpreq(const q7_t *input_a, const q15_t *input_b, const uint16_t output_ch,
                                               const float *scales, const int32_t out_offset,
                                               const int16_t activation_min, const int16_t activation_max,
                                               const uint16_t num_col_a, const int32_t *const output_bias, q7_t *out_0);

q7_t *mat_mult_kernel3_input3_s8_s16_fpreq(const q7_t *input_a, const q15_t *input_b, const uint16_t output_ch,
                                           const float *scales, const int32_t out_offset, const int16_t activation_min,
                                           const int16_t activation_max, const uint16_t num_col_a,
                                           const int32_t *const output_bias, q7_t *out_0, q15_t *kbuf);

#endif /* TINYENGINE_INCLUDE_FP_REQUANTIZE_OP_H_ */
