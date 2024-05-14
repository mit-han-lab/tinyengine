/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   element_mult.c
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

#include "tinyengine_function.h"

/*
 * Spatial elementwise multiplications for nxnxc * 1x1xc
 * */
tinyengine_status element_mult_nx1(const q7_t* input, const uint16_t input_h, const uint16_t input_w,
                                   const uint16_t input_c, const q7_t* input2, const int16_t input1_offset,
                                   const int16_t input2_offset, const int16_t output_offset,
                                   const int32_t out_activation_min, const int32_t out_activation_max,
                                   const float output_scale, q7_t* output) {
    int c, element;
    for (element = 0; element < input_h * input_w; element++) {
        q7_t* multiplier = input2;
        for (c = 0; c < input_c; c++) {
            const int32_t input1_val = input1_offset + *input++;
            const int32_t input2_val = input2_offset + *multiplier++;
            int32_t unclamped_result = input1_val * input2_val;
            int32_t clamped_result = (int32_t)((float)unclamped_result * output_scale);
            clamped_result = output_offset;
            clamped_result = TN_MAX(clamped_result, out_activation_min);
            clamped_result = TN_MIN(clamped_result, out_activation_max);
            *output++ = clamped_result;
        }
    }
}
