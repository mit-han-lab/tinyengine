/*
 * Copyright (C) 2010-2022 Arm Limited or its affiliates.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * This file is MODIFIED from Arm CMSIS NN Library.
 *
 * Project: TinyEngine
 * Title:   arm_nnfunctions_modified.h
 * Description:  Public header file for TinyEngine.
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
 * Original Project:      CMSIS NN Library
 * Original Title:        arm_nnfunctions.h
 *
 * Target Processor:  Cortex-M CPUs
 * -------------------------------------------------------------------- */

/**
   \mainpage CMSIS NN Software Library
   *
   * Introduction
   * ------------
   *
   * This user manual describes the CMSIS NN software library,
   * a collection of efficient neural network kernels developed to maximize the
   * performance and minimize the memory footprint of neural networks on Cortex-M processor cores.
   *
   * The library is divided into a number of functions each covering a specific category:
   * - Convolution Functions
   * - Activation Functions
   * - Fully-connected Layer Functions
   * - SVDF Layer Functions
   * - Pooling Functions
   * - Softmax Functions
   * - Basic math Functions
   *
   * The library has separate functions for operating on different weight and activation data
   * types including 8-bit integers (q7_t) and 16-bit integers (q15_t). The descrition of the
   * kernels are included in the function description. The implementation details are also
   * described in this paper [1].
   *
   * Function Classification
   * --------
   * The functions can be classified into two segments
   * - Legacy functions supporting ARM's internal symmetric quantization(8 bits).
   * - Functions that support TensorFlow Lite framework with symmetric quantization(8 bits).
   *
   * The legacy functions can be identified with their suffix of _q7 or _q15 and are no new development is done there.
   * The article in [2] describes in detail how to run a network using the legacy functions.
   *
   * The functions supporting TensorFlow Lite framework is identified by the _s8 suffix and can be invoked from TFL
   * micro. The functions are bit exact to TensorFlow Lite. Refer to the TensorFlow's documentation in [3] on how to run
   * a TensorFlow Lite model using optimized CMSIS-NN kernels.
   *
   * Block Diagram
   * --------
   * \image html CMSIS-NN-OVERVIEW.PNG
   *
   * Examples
   * --------
   *
   * The library ships with a number of examples which demonstrate how to use the library functions.
   *
   * Pre-processor Macros
   * ------------
   *
   * Each library project have different pre-processor macros.
   *
   * - ARM_MATH_DSP:
   *
   * Define macro ARM_MATH_DSP, If the silicon supports DSP instructions(DSP extension).
   *
   * - ARM_MATH_MVEI:
   *
   * Define macro ARM_MATH_MVEI, If the silicon supports M-Profile Vector Extension.

   * - ARM_MATH_AUTOVECTORIZE
   *  Used in conjucture with ARM_MATH_MVEI to let the compiler auto vectorize for the functions that uses inline
   *  assembly. It does not affect functions that use C or intrinsics.
   * - ARM_MATH_BIG_ENDIAN:
   *
   * Define macro ARM_MATH_BIG_ENDIAN to build the library for big endian targets. This is supported only for the legacy
   * functions i.e, functions targetted at TensorFlow Lite do not support big endianness. By default library builds for
   * little endian targets.
   *
   * - ARM_NN_TRUNCATE:
   *
   * Define macro ARM_NN_TRUNCATE to use floor instead of round-to-the-nearest-int for the computation.
   *
   *
   * Copyright Notice
   * ------------
   *
   * Copyright (C) 2010-2019 Arm Limited. All rights reserved.
   *
   * [1] CMSIS-NN: Efficient Neural Network Kernels for Arm Cortex-M CPUs https://arxiv.org/abs/1801.06601
   *
   * [2] Converting a Neural Network for Arm Cortex-M with CMSIS-NN
   *
   https://developer.arm.com/solutions/machine-learning-on-arm/developer-material/how-to-guides/converting-a-neural-network-for-arm-cortex-m-with-cmsis-nn/single-page
   * [3] https://www.tensorflow.org/lite/microcontrollers/library
   *
   * [4] https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/NN#legacy-vs-tfl-micro-compliant-apis
   */

/**
 * @defgroup groupNN Neural Network Functions
 * A collection of functions to perform basic operations for neural network layers. Functions with a _s8 suffix support
 * TensorFlow Lite framework.
 */

#ifndef _ARM_NNFUNCTIONS_H
#define _ARM_NNFUNCTIONS_H

#include "arm_nn_math_types.h"
#include "arm_nn_types.h"
#include "arm_nnsupportfunctions.h"

#define USE_INTRINSIC

//#define ARM_NN_TRUNCATE /* This config the rounding model to floor or round to the nearest int */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup NNConv Convolution Functions
 *
 * Collection of convolution, depthwise convolution functions and their variants.
 *
 * The convolution is implemented in 2 steps: im2col and GEMM
 *
 * im2col is a process of converting each patch of image data into
 * a column. After im2col, the convolution is computed as matrix-matrix
 * multiplication.
 *
 * To reduce the memory footprint, the im2col is performed partially.
 * Each iteration, only a few column (i.e., patches) are generated and
 * computed with GEMM kernels similar to CMSIS-DSP arm_mat_mult functions.
 *
 */

    arm_status arm_convolve_s8_4col(const q7_t *input,
                                    const uint16_t input_x,
                                    const uint16_t input_y,
                                    const uint16_t input_ch,
                                    const uint16_t input_batches,
                                    const q7_t *kernel,
                                    const uint16_t output_ch,
                                    const uint16_t kernel_x,
                                    const uint16_t kernel_y,
                                    const uint16_t pad_x,
                                    const uint16_t pad_y,
                                    const uint16_t stride_x,
                                    const uint16_t stride_y,
                                    const int32_t *bias,
                                    q7_t *output,
                                    const int32_t *output_shift,
                                    const int32_t *output_mult,
                                    const int32_t out_offset,
                                    const int32_t input_offset,
                                    const int32_t out_activation_min,
                                    const int32_t out_activation_max,
                                    const uint16_t output_x,
                                    const uint16_t output_y,
                                    q15_t *buffer_a);


    q7_t *arm_nn_mat_mult_kernel_s8_s16_reordered_oddch(const q7_t *input_a,
                                                        const q15_t *input_b,
                                                        const uint16_t output_ch,
                                                        const int32_t *out_shift,
                                                        const int32_t *out_mult,
                                                        const int32_t out_offset,
                                                        const int16_t activation_min,
                                                        const int16_t activation_max,
                                                        const uint16_t num_col_a,
                                                        const int32_t *const output_bias,
                                                        q7_t *out_0);

    q7_t *arm_nn_mat_mult_kernel_s8_s16_reordered_8mul(const q7_t *input_a,
                                                       const q15_t *input_b,
                                                       const uint16_t output_ch,
                                                       const int32_t *out_shift,
                                                       const int32_t *out_mult,
                                                       const int32_t out_offset,
                                                       const int16_t activation_min,
                                                       const int16_t activation_max,
                                                       const uint16_t num_col_a,
                                                       const int32_t *const output_bias,
                                                       q7_t *out_0);

    q7_t *arm_nn_mat_mult_kernel3_input3_s8_s16(const q7_t *input_a,
                                                const q15_t *input_b,
                                                const uint16_t output_ch,
                                                const int32_t *out_shift,
                                                const int32_t *out_mult,
                                                const int32_t out_offset,
                                                const int16_t activation_min,
                                                const int16_t activation_max,
                                                const uint16_t num_col_a,
                                                const int32_t *const output_bias,
                                                q7_t *out_0,
                                                q15_t *kbuf);

#ifdef __cplusplus
}
#endif

#endif
