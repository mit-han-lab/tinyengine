/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   kernel_element.h
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

#ifndef ARMNN_INCLUDE_KERNEL_ELEMENT_H_
#define ARMNN_INCLUDE_KERNEL_ELEMENT_H_

#include "mutable_function.h"
#include "precision_cnt.h"

#define loop_ele_ext()													\
sum = __SMLAD(col32[0], k_buf1[0], sum);								\
sum_2 = __SMLAD(col32[1], k_buf1[1], sum_2);					\
sum_3 = __SMLAD(col32[2], k_buf1[2], sum_3);				\
sum_4 = __SMLAD(col32[3], k_buf1[3], sum_4);				\
col32 += 4;\
k_buf1 += 4;																\

#define loop_ele()													\
op_a = arm_nn_read_q15x2(col_pos);									\
op_b = arm_nn_read_q15x2(col_pos + input_ch);						\
																	\
op_c = __PKHBT(op_b, op_a, 16);										\
op_a = __PKHTB(op_b, op_a, 16);										\
sum = __SMLAD(op_c, k_buf1[0], sum);								\
sum_2 = __SMLAD(op_a, k_buf1[q32_elements], sum_2);					\
																	\
op_a = arm_nn_read_q15x2(col_pos + 2);								\
op_b = arm_nn_read_q15x2(col_pos + input_ch + 2);					\
																	\
op_c = __PKHBT(op_b, op_a, 16);										\
op_a = __PKHTB(op_b, op_a, 16);										\
sum_3 = __SMLAD(op_c, k_buf1[q32_elements*2], sum_3);				\
sum_4 = __SMLAD(op_a, k_buf1[q32_elements*3], sum_4);				\
																	\
col_pos += two_inch;												\
k_buf1++;
/* end of loop_ele() */

#define prepare_loops()\
q7_t *out_1 = out + output_ch / output_scaler;\
const int32_t *out_shift = output_shift;\
const int32_t *out_mult = output_mult;\
const int32_t *obias = bias;\
uint16_t row_count = output_ch / 2;\
q31_t *ksrc = &kbuf[0];\
/* end of prepare_loops() */

#define conv_1stloop_ele()\
q31_t ch_0_out_0 = *obias;\
q31_t ch_0_out_1 = *obias++;\
q31_t ch_1_out_0 = *obias;\
q31_t ch_1_out_1 = *obias++;\
q31_t b0 = arm_nn_read_q15x2_ia((const q15_t **)&ip_b0);\
q31_t b1 = arm_nn_read_q15x2_ia((const q15_t **)&ip_b1);\
ch_0_out_0 = __SMLAD(*ksrc, b0, ch_0_out_0);\
ch_0_out_1 = __SMLAD(*ksrc++, b1, ch_0_out_1);\
ch_1_out_0 = __SMLAD(*ksrc2, b0, ch_1_out_0);\
b0 = arm_nn_read_q15x2_ia((const q15_t **)&ip_b0);\
ch_1_out_1 = __SMLAD(*ksrc2++, b1, ch_1_out_1);\
 /* end of conv_1stloop_ele */

#define conv_lastloop_ele()\
b1 = arm_nn_read_q15x2_ia((const q15_t **)&ip_b1);\
\
ch_0_out_0 = __SMLAD(*ksrc, b0, ch_0_out_0);\
ch_0_out_1 = __SMLAD(*ksrc++, b1, ch_0_out_1);\
ch_1_out_0 = __SMLAD(*ksrc2, b0, ch_1_out_0);\
ch_1_out_1 = __SMLAD(*ksrc2++, b1, ch_1_out_1);\
\
ksrc = ksrc2;\
 /* end of conv_lastloop_ele */

#define conv_midloop_ele(k_index)													\
b1 = arm_nn_read_q15x2_ia(&ip_b1);\
ch_0_out_0 = __SMLAD(ksrc[k_index], b0, ch_0_out_0);\
ch_0_out_1 = __SMLAD(ksrc[k_index], b1, ch_0_out_1);\
ch_1_out_0 = __SMLAD(ksrc2[k_index], b0, ch_1_out_0);\
b0 = arm_nn_read_q15x2_ia(&ip_b0);\
ch_1_out_1 = __SMLAD(ksrc2[k_index], b1, ch_1_out_1);\
 /* end of conv_midloop_ele */

#define conv_midloop_ptrele()													\
b1 = arm_nn_read_q15x2_ia((const q15_t **)&ip_b1);\
ch_0_out_0 = __SMLAD(*ksrc, b0, ch_0_out_0);\
ch_0_out_1 = __SMLAD(*ksrc++, b1, ch_0_out_1);\
ch_1_out_0 = __SMLAD(*ksrc2, b0, ch_1_out_0);\
b0 = arm_nn_read_q15x2_ia((const q15_t **)&ip_b0);\
ch_1_out_1 = __SMLAD(*ksrc2++, b1, ch_1_out_1);\
 /* end of conv_midloop_ele */

#define unroll_8inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 8;\
	q31_t *ksrc2 = ksrc + 4;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\

/* Specialized Loop Unrolling  */
//this can be selected for different models
#define unroll_8inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 8;\
	q31_t *ksrc2 = ksrc + 4;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\

#define unroll_12inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 12;\
	q31_t *ksrc2 = ksrc + 6;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\

#define unroll_16inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 16;\
	q31_t *ksrc2 = ksrc + 8;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\

#define unroll_20inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 20;\
	q31_t *ksrc2 = ksrc + 10;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\

#define unroll_24inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 24;\
	q31_t *ksrc2 = ksrc + 12;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\


#define unroll_32inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 32;\
	q31_t *ksrc2 = ksrc + 16;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\

#define unroll_36inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 36;\
	q31_t *ksrc2 = ksrc + 18;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\

#define unroll_40inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 40;\
	q31_t *ksrc2 = ksrc + 20;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\

#define unroll_48inch()\
prepare_loops();\
while (row_count) {\
	const q15_t *ip_b0 = two_column_buffer;\
	const q15_t *ip_b1 = ip_b0 + 48;\
	q31_t *ksrc2 = ksrc + 24;\
	conv_1stloop_ele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_midloop_ptrele()\
	conv_lastloop_ele()\
	mix_assign_requantize()\
	row_count--;\
}\


/* END: Specialized Loop Unrolling  */

#define b2_assign_requantize()	\
ch_0_out_0 = arm_nn_requantize(ch_0_out_0, *out_mult,*out_shift);\
ch_0_out_0 += out_offset;\
ch_0_out_0 = MAX(ch_0_out_0, out_activation_min);\
ch_0_out_0 = MIN(ch_0_out_0, out_activation_max);\
\
ch_0_out_1 = arm_nn_requantize(ch_0_out_1, *out_mult,*out_shift);\
ch_0_out_1 += out_offset;\
ch_0_out_1 = MAX(ch_0_out_1, out_activation_min);\
ch_0_out_1 = MIN(ch_0_out_1, out_activation_max);\
out_mult++;\
out_shift++;\
ch_1_out_0 = arm_nn_requantize(ch_1_out_0, *out_mult,*out_shift);\
ch_1_out_0 += out_offset;\
ch_1_out_0 = MAX(ch_1_out_0, out_activation_min);\
ch_1_out_0 = MIN(ch_1_out_0, out_activation_max);\
ch_1_out_1 = arm_nn_requantize(ch_1_out_1, *out_mult,*out_shift);\
ch_1_out_1 += out_offset;\
ch_1_out_1 = MAX(ch_1_out_1, out_activation_min);\
ch_1_out_1 = MIN(ch_1_out_1, out_activation_max);\
if(lower_bit == 1){\
	*out = (q7_t) ((ch_0_out_0 & 0x03) + ((ch_1_out_0 & 0x03) << 2));\
	*out_1 = (q7_t) ((ch_0_out_0 & 0x03) + ((ch_1_out_1 & 0x03) << 2));\
	lower_bit = 3;\
}\
else{\
	*out++ += (q7_t) (((ch_0_out_0 & 0x03) + ((ch_1_out_0 & 0x03) << 2)) << 4);\
	*out_1++ += (q7_t) (((ch_0_out_1 & 0x03) + ((ch_1_out_1 & 0x03) << 2)) << 4);\
	lower_bit = 1;\
}\
out_mult++;\
out_shift++;\

#define b4_assign_requantize()	\
ch_0_out_0 = arm_nn_requantize(ch_0_out_0, *out_mult,*out_shift);\
ch_0_out_0 += out_offset;\
ch_0_out_0 = MAX(ch_0_out_0, out_activation_min);\
ch_0_out_0 = MIN(ch_0_out_0, out_activation_max);\
\
ch_0_out_1 = arm_nn_requantize(ch_0_out_1, *out_mult,*out_shift);\
ch_0_out_1 += out_offset;\
ch_0_out_1 = MAX(ch_0_out_1, out_activation_min);\
ch_0_out_1 = MIN(ch_0_out_1, out_activation_max);\
out_mult++;\
out_shift++;\
ch_1_out_0 = arm_nn_requantize(ch_1_out_0, *out_mult,*out_shift);\
ch_1_out_0 += out_offset;\
ch_1_out_0 = MAX(ch_1_out_0, out_activation_min);\
ch_1_out_0 = MIN(ch_1_out_0, out_activation_max);\
*out++ = (q7_t) ((ch_0_out_0 & 0x0F) + ((ch_1_out_0 & 0x0F) << 4));\
ch_1_out_1 = arm_nn_requantize(ch_1_out_1, *out_mult,*out_shift);\
ch_1_out_1 += out_offset;\
ch_1_out_1 = MAX(ch_1_out_1, out_activation_min);\
ch_1_out_1 = MIN(ch_1_out_1, out_activation_max);\
*out_1++ = (q7_t) ((ch_0_out_1 & 0x0F) + ((ch_1_out_1 & 0x0F) << 4));\
out_mult++;\
out_shift++;\

#define assign_requantize()	\
ch_0_out_0 = arm_nn_requantize(ch_0_out_0, *out_mult,*out_shift);\
ch_0_out_0 += out_offset;\
ch_0_out_0 = MAX(ch_0_out_0, out_activation_min);\
ch_0_out_0 = MIN(ch_0_out_0, out_activation_max);\
*out++ = (q7_t) ch_0_out_0;\
\
ch_0_out_1 = arm_nn_requantize(ch_0_out_1, *out_mult,*out_shift);\
ch_0_out_1 += out_offset;\
ch_0_out_1 = MAX(ch_0_out_1, out_activation_min);\
ch_0_out_1 = MIN(ch_0_out_1, out_activation_max);\
*out_1++ = (q7_t) ch_0_out_1;\
out_mult++;\
out_shift++;\
ch_1_out_0 = arm_nn_requantize(ch_1_out_0, *out_mult,*out_shift);\
ch_1_out_0 += out_offset;\
ch_1_out_0 = MAX(ch_1_out_0, out_activation_min);\
ch_1_out_0 = MIN(ch_1_out_0, out_activation_max);\
*out++ = (q7_t) ch_1_out_0;\
\
ch_1_out_1 = arm_nn_requantize(ch_1_out_1, *out_mult,*out_shift);\
ch_1_out_1 += out_offset;\
ch_1_out_1 = MAX(ch_1_out_1, out_activation_min);\
ch_1_out_1 = MIN(ch_1_out_1, out_activation_max);\
*out_1++ = (q7_t) ch_1_out_1;\
out_mult++;\
out_shift++;\
 /* end of assign_requantize */

#endif /* ARMNN_INCLUDE_KERNEL_ELEMENT_H_ */
