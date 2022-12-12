/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   mutable_function.h
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

#ifndef TINYENGINE_SOURCE_CONVOLUTIONFUNCTIONS_MIX_MUTABLE_FUNCTION_H_
#define TINYENGINE_SOURCE_CONVOLUTIONFUNCTIONS_MIX_MUTABLE_FUNCTION_H_

/* mutable functions */
#if KERNEL_PRE == 4
#define mix_read_and_pad_reordered b4_read_and_pad_reordered
#define mix_nn_read_q7x4 b4_nn_read_q7x4
#define mix_read_and_pad b4_read_and_pad
#elif KERNEL_PRE == 2
#define mix_read_and_pad_reordered b2_read_and_pad_reordered
#define mix_nn_read_q7x4 b2_nn_read_q7x4
#define mix_read_and_pad b2_read_and_pad
#else
#define mix_read_and_pad_reordered read_and_pad_reordered
#define mix_nn_read_q7x4 arm_nn_read_q7x4
#define mix_read_and_pad read_and_pad
#endif

#if INPUT_PRE == 4
#define mix_q7_q15_offset_ele b4_q7_q15_offset_ele
#elif INPUT_PRE == 2
#define mix_q7_q15_offset_ele b2_q7_q15_offset_ele
#else
#define mix_q7_q15_offset_ele q7_q15_offset_ele
#endif

#if INPUT_PRE == 4
#define	mix_q7_q15_offset_reordered_ele b4_q15_offset_reordered_ele
#define mix_load_1row_1col b4_load_1row_1col
#define mix_load_1row_2col b4_load_1row_2col
#define mix_load_1row_3col b4_load_1row_3col
#define mix_load_1row_4col b4_load_1row_4col
#define mix_load_1row_5col b4_load_1row_5col
#define mix_load_1row_6col b4_load_1row_6col
#define mix_load_1row_7col b4_load_1row_7col
#define mix_load_2row_1col b4_load_2row_1col
#define mix_load_2row_2col b4_load_2row_2col
#define mix_load_2row_3col b4_load_2row_3col
#define mix_load_2row_4col b4_load_2row_4col
#define mix_load_2row_5col b4_load_2row_5col
#define mix_load_2row_6col b4_load_2row_6col
#define mix_load_2row_7col b4_load_2row_7col
#define mix_load_3row_1col b4_load_3row_1col
#define mix_load_3row_2col b4_load_3row_2col
#define mix_load_3row_3col b4_load_3row_3col
#define mix_load_3row_4col b4_load_3row_4col
#define mix_load_3row_5col b4_load_3row_5col
#define mix_load_3row_6col b4_load_3row_6col
#define mix_load_3row_7col b4_load_3row_7col
#define mix_load_4row_1col b4_load_4row_1col
#define mix_load_4row_2col b4_load_4row_2col
#define mix_load_4row_3col b4_load_4row_3col
#define mix_load_4row_4col b4_load_4row_4col
#define mix_load_4row_5col b4_load_4row_5col
#define mix_load_4row_6col b4_load_4row_6col
#define mix_load_4row_7col b4_load_4row_7col
#define mix_load_5row_1col b4_load_5row_1col
#define mix_load_5row_2col b4_load_5row_2col
#define mix_load_5row_3col b4_load_5row_3col
#define mix_load_5row_4col b4_load_5row_4col
#define mix_load_5row_5col b4_load_5row_5col
#define mix_load_5row_6col b4_load_5row_6col
#define mix_load_5row_7col b4_load_5row_7col
#define mix_load_6row_1col b4_load_6row_1col
#define mix_load_6row_2col b4_load_6row_2col
#define mix_load_6row_3col b4_load_6row_3col
#define mix_load_6row_4col b4_load_6row_4col
#define mix_load_6row_5col b4_load_6row_5col
#define mix_load_6row_6col b4_load_6row_6col
#define mix_load_6row_7col b4_load_6row_7col
#define mix_load_7row_1col b4_load_7row_1col
#define mix_load_7row_2col b4_load_7row_2col
#define mix_load_7row_3col b4_load_7row_3col
#define mix_load_7row_4col b4_load_7row_4col
#define mix_load_7row_5col b4_load_7row_5col
#define mix_load_7row_6col b4_load_7row_6col
#define mix_load_7row_7col b4_load_7row_7col
#elif INPUT_PRE == 2
#define	mix_q7_q15_offset_reordered_ele b2_q15_offset_reordered_ele
#define mix_load_1row_1col b2_load_1row_1col
#define mix_load_1row_2col b2_load_1row_2col
#define mix_load_1row_3col b2_load_1row_3col
#define mix_load_1row_4col b2_load_1row_4col
#define mix_load_1row_5col b2_load_1row_5col
#define mix_load_1row_6col b2_load_1row_6col
#define mix_load_1row_7col b2_load_1row_7col
#define mix_load_2row_1col b2_load_2row_1col
#define mix_load_2row_2col b2_load_2row_2col
#define mix_load_2row_3col b2_load_2row_3col
#define mix_load_2row_4col b2_load_2row_4col
#define mix_load_2row_5col b2_load_2row_5col
#define mix_load_2row_6col b2_load_2row_6col
#define mix_load_2row_7col b2_load_2row_7col
#define mix_load_3row_1col b2_load_3row_1col
#define mix_load_3row_2col b2_load_3row_2col
#define mix_load_3row_3col b2_load_3row_3col
#define mix_load_3row_4col b2_load_3row_4col
#define mix_load_3row_5col b2_load_3row_5col
#define mix_load_3row_6col b2_load_3row_6col
#define mix_load_3row_7col b2_load_3row_7col
#define mix_load_4row_1col b2_load_4row_1col
#define mix_load_4row_2col b2_load_4row_2col
#define mix_load_4row_3col b2_load_4row_3col
#define mix_load_4row_4col b2_load_4row_4col
#define mix_load_4row_5col b2_load_4row_5col
#define mix_load_4row_6col b2_load_4row_6col
#define mix_load_4row_7col b2_load_4row_7col
#define mix_load_5row_1col b2_load_5row_1col
#define mix_load_5row_2col b2_load_5row_2col
#define mix_load_5row_3col b2_load_5row_3col
#define mix_load_5row_4col b2_load_5row_4col
#define mix_load_5row_5col b2_load_5row_5col
#define mix_load_5row_6col b2_load_5row_6col
#define mix_load_5row_7col b2_load_5row_7col
#define mix_load_6row_1col b2_load_6row_1col
#define mix_load_6row_2col b2_load_6row_2col
#define mix_load_6row_3col b2_load_6row_3col
#define mix_load_6row_4col b2_load_6row_4col
#define mix_load_6row_5col b2_load_6row_5col
#define mix_load_6row_6col b2_load_6row_6col
#define mix_load_6row_7col b2_load_6row_7col
#define mix_load_7row_1col b2_load_7row_1col
#define mix_load_7row_2col b2_load_7row_2col
#define mix_load_7row_3col b2_load_7row_3col
#define mix_load_7row_4col b2_load_7row_4col
#define mix_load_7row_5col b2_load_7row_5col
#define mix_load_7row_6col b2_load_7row_6col
#define mix_load_7row_7col b2_load_7row_7col
#else
#define mix_q7_q15_offset_reordered_ele q7_q15_offset_reordered_ele
#define mix_load_1row_1col load_1row_1col
#define mix_load_1row_2col load_1row_2col
#define mix_load_1row_3col load_1row_3col
#define mix_load_1row_4col load_1row_4col
#define mix_load_1row_5col load_1row_5col
#define mix_load_1row_6col load_1row_6col
#define mix_load_1row_7col load_1row_7col
#define mix_load_2row_1col load_2row_1col
#define mix_load_2row_2col load_2row_2col
#define mix_load_2row_3col load_2row_3col
#define mix_load_2row_4col load_2row_4col
#define mix_load_2row_5col load_2row_5col
#define mix_load_2row_6col load_2row_6col
#define mix_load_2row_7col load_2row_7col
#define mix_load_3row_1col load_3row_1col
#define mix_load_3row_2col load_3row_2col
#define mix_load_3row_3col load_3row_3col
#define mix_load_3row_4col load_3row_4col
#define mix_load_3row_5col load_3row_5col
#define mix_load_3row_6col load_3row_6col
#define mix_load_3row_7col load_3row_7col
#define mix_load_4row_1col load_4row_1col
#define mix_load_4row_2col load_4row_2col
#define mix_load_4row_3col load_4row_3col
#define mix_load_4row_4col load_4row_4col
#define mix_load_4row_5col load_4row_5col
#define mix_load_4row_6col load_4row_6col
#define mix_load_4row_7col load_4row_7col
#define mix_load_5row_1col load_5row_1col
#define mix_load_5row_2col load_5row_2col
#define mix_load_5row_3col load_5row_3col
#define mix_load_5row_4col load_5row_4col
#define mix_load_5row_5col load_5row_5col
#define mix_load_5row_6col load_5row_6col
#define mix_load_5row_7col load_5row_7col
#define mix_load_6row_1col load_6row_1col
#define mix_load_6row_2col load_6row_2col
#define mix_load_6row_3col load_6row_3col
#define mix_load_6row_4col load_6row_4col
#define mix_load_6row_5col load_6row_5col
#define mix_load_6row_6col load_6row_6col
#define mix_load_6row_7col load_6row_7col
#define mix_load_7row_1col load_7row_1col
#define mix_load_7row_2col load_7row_2col
#define mix_load_7row_3col load_7row_3col
#define mix_load_7row_4col load_7row_4col
#define mix_load_7row_5col load_7row_5col
#define mix_load_7row_6col load_7row_6col
#define mix_load_7row_7col load_7row_7col
#endif

#if OUTPUT_PRE == 4
#define	mix_assign_requantize()	b4_assign_requantize()
#elif OUTPUT_PRE == 2
#define	mix_assign_requantize()	b2_assign_requantize()
#else
#define	mix_assign_requantize()	assign_requantize()
#endif

#if KERNEL_PRE == 4
	#if OUTPUT_PRE == 4
#define mix_nn_mat_mult_kernel_s8_s16_reordered b44_nn_mat_mult_kernel_s8_s16_reordered
#define mix_nn_mat_mult_kernel_s8_s16_reordered_8mul b44_nn_mat_mult_kernel_s8_s16_reordered_8mul
	#elif OUTPUT_PRE == 2
#define mix_nn_mat_mult_kernel_s8_s16_reordered b42_nn_mat_mult_kernel_s8_s16_reordered
#define mix_nn_mat_mult_kernel_s8_s16_reordered_8mul b42_nn_mat_mult_kernel_s8_s16_reordered_8mul
	#else
#define mix_nn_mat_mult_kernel_s8_s16_reordered b48_nn_mat_mult_kernel_s8_s16_reordered
#define mix_nn_mat_mult_kernel_s8_s16_reordered_8mul b48_nn_mat_mult_kernel_s8_s16_reordered_8mul
	#endif//OUTPUT
#elif KERNEL_PRE == 2
	#if OUTPUT_PRE == 4
#define mix_nn_mat_mult_kernel_s8_s16_reordered b24_nn_mat_mult_kernel_s8_s16_reordered
#define mix_nn_mat_mult_kernel_s8_s16_reordered_8mul b24_nn_mat_mult_kernel_s8_s16_reordered_8mul
	#elif OUTPUT_PRE == 2
#define mix_nn_mat_mult_kernel_s8_s16_reordered b22_nn_mat_mult_kernel_s8_s16_reordered
#define mix_nn_mat_mult_kernel_s8_s16_reordered_8mul b22_nn_mat_mult_kernel_s8_s16_reordered_8mul
#else
#define mix_nn_mat_mult_kernel_s8_s16_reordered b28_nn_mat_mult_kernel_s8_s16_reordered
#define mix_nn_mat_mult_kernel_s8_s16_reordered_8mul b28_nn_mat_mult_kernel_s8_s16_reordered_8mul
	#endif//OUTPUT
#else
	#define mix_nn_mat_mult_kernel_s8_s16_reordered arm_nn_mat_mult_kernel_s8_s16_reordered
	#define mix_nn_mat_mult_kernel_s8_s16_reordered_8mul arm_nn_mat_mult_kernel_s8_s16_reordered_8mul
#endif


#endif /* TINYENGINE_SOURCE_CONVOLUTIONFUNCTIONS_MIX_MUTABLE_FUNCTION_H_ */
