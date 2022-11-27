/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   nnfunctions.h
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *  - Chuang Gan, ganchuang@csail.mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#include "arm_nnsupportfunctions.h"
#include <stdlib.h>

/* START: MAC Functions for Pointwise Conv */
static inline void mac_4row_4col_IOHW_forint8w(q31_t* sum, const q7_t* input_0, const q7_t* input_1, const q7_t* input_2, const q7_t* input_3,
                      const q7_t* filter_0, const q7_t* filter_1, const q7_t* filter_2, const q7_t* filter_3) {
  *sum += *input_0++ * *filter_0;
  *sum += *input_0++ * *filter_1;
  *sum += *input_0++ * *filter_2;
  *sum++ += *input_0++ * *filter_3;

  *sum += *input_1++ * *filter_0;
  *sum += *input_1++ * *filter_1;
  *sum += *input_1++ * *filter_2;
  *sum++ += *input_1++ * *filter_3;

  *sum += *input_2++ * *filter_0;
  *sum += *input_2++ * *filter_1;
  *sum += *input_2++ * *filter_2;
  *sum++ += *input_2++ * *filter_3;

  *sum += *input_3++ * *filter_0;
  *sum += *input_3++ * *filter_1;
  *sum += *input_3++ * *filter_2;
  *sum++ += *input_3++ * *filter_3;
}

static inline void mac_1row_4col_IOHW_forint8w(q31_t* sum, const q7_t* input_0,
                      const q7_t* filter_0, const q7_t* filter_1, const q7_t* filter_2, const q7_t* filter_3) {
  *sum += *input_0++ * *filter_0;
  *sum += *input_0++ * *filter_1;
  *sum += *input_0++ * *filter_2;
  *sum += *input_0++ * *filter_3;
}
/* END: MAC Functions for Pointwise Conv */


/* START: MAC Functions for Group Conv */
static inline void group_mac_kernel8_4row_uniweight_reuse_output_input(q31_t* sum_0, q31_t* sum_1, q31_t* sum_2, q31_t* sum_3,
           const q7_t* input_0, const q7_t* input_1, const q7_t* input_2, const q7_t* input_3,
           const q7_t* filter) {
    q31_t tmp;
    tmp = 0;
    tmp += input_0[0] * filter[0];
    tmp += input_0[1] * filter[1];
    tmp += input_0[2] * filter[2];
    tmp += input_0[3] * filter[3];
    tmp += input_0[4] * filter[4];
    tmp += input_0[5] * filter[5];
    tmp += input_0[6] * filter[6];
    tmp += input_0[7] * filter[7];
    tmp += input_0[8] * filter[8];
    tmp += input_0[9] * filter[9];
    tmp += input_0[10] * filter[10];
    tmp += input_0[11] * filter[11];
    tmp += input_0[12] * filter[12];
    tmp += input_0[13] * filter[13];
    tmp += input_0[14] * filter[14];
    tmp += input_0[15] * filter[15];
    tmp += input_0[16] * filter[16];
    tmp += input_0[17] * filter[17];
    tmp += input_0[18] * filter[18];
    tmp += input_0[19] * filter[19];
    tmp += input_0[20] * filter[20];
    tmp += input_0[21] * filter[21];
    tmp += input_0[22] * filter[22];
    tmp += input_0[23] * filter[23];
    tmp += input_0[24] * filter[24];
    tmp += input_0[25] * filter[25];
    tmp += input_0[26] * filter[26];
    tmp += input_0[27] * filter[27];
    tmp += input_0[28] * filter[28];
    tmp += input_0[29] * filter[29];
    tmp += input_0[30] * filter[30];
    tmp += input_0[31] * filter[31];
    tmp += input_0[32] * filter[32];
    tmp += input_0[33] * filter[33];
    tmp += input_0[34] * filter[34];
    tmp += input_0[35] * filter[35];
    tmp += input_0[36] * filter[36];
    tmp += input_0[37] * filter[37];
    tmp += input_0[38] * filter[38];
    tmp += input_0[39] * filter[39];
    tmp += input_0[40] * filter[40];
    tmp += input_0[41] * filter[41];
    tmp += input_0[42] * filter[42];
    tmp += input_0[43] * filter[43];
    tmp += input_0[44] * filter[44];
    tmp += input_0[45] * filter[45];
    tmp += input_0[46] * filter[46];
    tmp += input_0[47] * filter[47];
    tmp += input_0[48] * filter[48];
    tmp += input_0[49] * filter[49];
    tmp += input_0[50] * filter[50];
    tmp += input_0[51] * filter[51];
    tmp += input_0[52] * filter[52];
    tmp += input_0[53] * filter[53];
    tmp += input_0[54] * filter[54];
    tmp += input_0[55] * filter[55];
    tmp += input_0[56] * filter[56];
    tmp += input_0[57] * filter[57];
    tmp += input_0[58] * filter[58];
    tmp += input_0[59] * filter[59];
    tmp += input_0[60] * filter[60];
    tmp += input_0[61] * filter[61];
    tmp += input_0[62] * filter[62];
    tmp += input_0[63] * filter[63];
    *sum_0 += tmp;
    tmp = 0;
    tmp += input_1[0] * filter[0];
    tmp += input_1[1] * filter[1];
    tmp += input_1[2] * filter[2];
    tmp += input_1[3] * filter[3];
    tmp += input_1[4] * filter[4];
    tmp += input_1[5] * filter[5];
    tmp += input_1[6] * filter[6];
    tmp += input_1[7] * filter[7];
    tmp += input_1[8] * filter[8];
    tmp += input_1[9] * filter[9];
    tmp += input_1[10] * filter[10];
    tmp += input_1[11] * filter[11];
    tmp += input_1[12] * filter[12];
    tmp += input_1[13] * filter[13];
    tmp += input_1[14] * filter[14];
    tmp += input_1[15] * filter[15];
    tmp += input_1[16] * filter[16];
    tmp += input_1[17] * filter[17];
    tmp += input_1[18] * filter[18];
    tmp += input_1[19] * filter[19];
    tmp += input_1[20] * filter[20];
    tmp += input_1[21] * filter[21];
    tmp += input_1[22] * filter[22];
    tmp += input_1[23] * filter[23];
    tmp += input_1[24] * filter[24];
    tmp += input_1[25] * filter[25];
    tmp += input_1[26] * filter[26];
    tmp += input_1[27] * filter[27];
    tmp += input_1[28] * filter[28];
    tmp += input_1[29] * filter[29];
    tmp += input_1[30] * filter[30];
    tmp += input_1[31] * filter[31];
    tmp += input_1[32] * filter[32];
    tmp += input_1[33] * filter[33];
    tmp += input_1[34] * filter[34];
    tmp += input_1[35] * filter[35];
    tmp += input_1[36] * filter[36];
    tmp += input_1[37] * filter[37];
    tmp += input_1[38] * filter[38];
    tmp += input_1[39] * filter[39];
    tmp += input_1[40] * filter[40];
    tmp += input_1[41] * filter[41];
    tmp += input_1[42] * filter[42];
    tmp += input_1[43] * filter[43];
    tmp += input_1[44] * filter[44];
    tmp += input_1[45] * filter[45];
    tmp += input_1[46] * filter[46];
    tmp += input_1[47] * filter[47];
    tmp += input_1[48] * filter[48];
    tmp += input_1[49] * filter[49];
    tmp += input_1[50] * filter[50];
    tmp += input_1[51] * filter[51];
    tmp += input_1[52] * filter[52];
    tmp += input_1[53] * filter[53];
    tmp += input_1[54] * filter[54];
    tmp += input_1[55] * filter[55];
    tmp += input_1[56] * filter[56];
    tmp += input_1[57] * filter[57];
    tmp += input_1[58] * filter[58];
    tmp += input_1[59] * filter[59];
    tmp += input_1[60] * filter[60];
    tmp += input_1[61] * filter[61];
    tmp += input_1[62] * filter[62];
    tmp += input_1[63] * filter[63];
    *sum_1 += tmp;
    tmp = 0;
    tmp += input_2[0] * filter[0];
    tmp += input_2[1] * filter[1];
    tmp += input_2[2] * filter[2];
    tmp += input_2[3] * filter[3];
    tmp += input_2[4] * filter[4];
    tmp += input_2[5] * filter[5];
    tmp += input_2[6] * filter[6];
    tmp += input_2[7] * filter[7];
    tmp += input_2[8] * filter[8];
    tmp += input_2[9] * filter[9];
    tmp += input_2[10] * filter[10];
    tmp += input_2[11] * filter[11];
    tmp += input_2[12] * filter[12];
    tmp += input_2[13] * filter[13];
    tmp += input_2[14] * filter[14];
    tmp += input_2[15] * filter[15];
    tmp += input_2[16] * filter[16];
    tmp += input_2[17] * filter[17];
    tmp += input_2[18] * filter[18];
    tmp += input_2[19] * filter[19];
    tmp += input_2[20] * filter[20];
    tmp += input_2[21] * filter[21];
    tmp += input_2[22] * filter[22];
    tmp += input_2[23] * filter[23];
    tmp += input_2[24] * filter[24];
    tmp += input_2[25] * filter[25];
    tmp += input_2[26] * filter[26];
    tmp += input_2[27] * filter[27];
    tmp += input_2[28] * filter[28];
    tmp += input_2[29] * filter[29];
    tmp += input_2[30] * filter[30];
    tmp += input_2[31] * filter[31];
    tmp += input_2[32] * filter[32];
    tmp += input_2[33] * filter[33];
    tmp += input_2[34] * filter[34];
    tmp += input_2[35] * filter[35];
    tmp += input_2[36] * filter[36];
    tmp += input_2[37] * filter[37];
    tmp += input_2[38] * filter[38];
    tmp += input_2[39] * filter[39];
    tmp += input_2[40] * filter[40];
    tmp += input_2[41] * filter[41];
    tmp += input_2[42] * filter[42];
    tmp += input_2[43] * filter[43];
    tmp += input_2[44] * filter[44];
    tmp += input_2[45] * filter[45];
    tmp += input_2[46] * filter[46];
    tmp += input_2[47] * filter[47];
    tmp += input_2[48] * filter[48];
    tmp += input_2[49] * filter[49];
    tmp += input_2[50] * filter[50];
    tmp += input_2[51] * filter[51];
    tmp += input_2[52] * filter[52];
    tmp += input_2[53] * filter[53];
    tmp += input_2[54] * filter[54];
    tmp += input_2[55] * filter[55];
    tmp += input_2[56] * filter[56];
    tmp += input_2[57] * filter[57];
    tmp += input_2[58] * filter[58];
    tmp += input_2[59] * filter[59];
    tmp += input_2[60] * filter[60];
    tmp += input_2[61] * filter[61];
    tmp += input_2[62] * filter[62];
    tmp += input_2[63] * filter[63];
    *sum_2 += tmp;
    tmp = 0;
    tmp += input_3[0] * filter[0];
    tmp += input_3[1] * filter[1];
    tmp += input_3[2] * filter[2];
    tmp += input_3[3] * filter[3];
    tmp += input_3[4] * filter[4];
    tmp += input_3[5] * filter[5];
    tmp += input_3[6] * filter[6];
    tmp += input_3[7] * filter[7];
    tmp += input_3[8] * filter[8];
    tmp += input_3[9] * filter[9];
    tmp += input_3[10] * filter[10];
    tmp += input_3[11] * filter[11];
    tmp += input_3[12] * filter[12];
    tmp += input_3[13] * filter[13];
    tmp += input_3[14] * filter[14];
    tmp += input_3[15] * filter[15];
    tmp += input_3[16] * filter[16];
    tmp += input_3[17] * filter[17];
    tmp += input_3[18] * filter[18];
    tmp += input_3[19] * filter[19];
    tmp += input_3[20] * filter[20];
    tmp += input_3[21] * filter[21];
    tmp += input_3[22] * filter[22];
    tmp += input_3[23] * filter[23];
    tmp += input_3[24] * filter[24];
    tmp += input_3[25] * filter[25];
    tmp += input_3[26] * filter[26];
    tmp += input_3[27] * filter[27];
    tmp += input_3[28] * filter[28];
    tmp += input_3[29] * filter[29];
    tmp += input_3[30] * filter[30];
    tmp += input_3[31] * filter[31];
    tmp += input_3[32] * filter[32];
    tmp += input_3[33] * filter[33];
    tmp += input_3[34] * filter[34];
    tmp += input_3[35] * filter[35];
    tmp += input_3[36] * filter[36];
    tmp += input_3[37] * filter[37];
    tmp += input_3[38] * filter[38];
    tmp += input_3[39] * filter[39];
    tmp += input_3[40] * filter[40];
    tmp += input_3[41] * filter[41];
    tmp += input_3[42] * filter[42];
    tmp += input_3[43] * filter[43];
    tmp += input_3[44] * filter[44];
    tmp += input_3[45] * filter[45];
    tmp += input_3[46] * filter[46];
    tmp += input_3[47] * filter[47];
    tmp += input_3[48] * filter[48];
    tmp += input_3[49] * filter[49];
    tmp += input_3[50] * filter[50];
    tmp += input_3[51] * filter[51];
    tmp += input_3[52] * filter[52];
    tmp += input_3[53] * filter[53];
    tmp += input_3[54] * filter[54];
    tmp += input_3[55] * filter[55];
    tmp += input_3[56] * filter[56];
    tmp += input_3[57] * filter[57];
    tmp += input_3[58] * filter[58];
    tmp += input_3[59] * filter[59];
    tmp += input_3[60] * filter[60];
    tmp += input_3[61] * filter[61];
    tmp += input_3[62] * filter[62];
    tmp += input_3[63] * filter[63];
    *sum_3 += tmp;
}

static inline void group_mac_kernel4_4row_uniweight_reuse_output_input(q31_t* sum_0, q31_t* sum_1, q31_t* sum_2, q31_t* sum_3,
           const q7_t* input_0, const q7_t* input_1, const q7_t* input_2, const q7_t* input_3,
           const q7_t* filter) {
    q31_t tmp;
    tmp = 0;
    tmp += input_0[0] * filter[0];
    tmp += input_0[1] * filter[1];
    tmp += input_0[2] * filter[2];
    tmp += input_0[3] * filter[3];
    tmp += input_0[4] * filter[4];
    tmp += input_0[5] * filter[5];
    tmp += input_0[6] * filter[6];
    tmp += input_0[7] * filter[7];
    tmp += input_0[8] * filter[8];
    tmp += input_0[9] * filter[9];
    tmp += input_0[10] * filter[10];
    tmp += input_0[11] * filter[11];
    tmp += input_0[12] * filter[12];
    tmp += input_0[13] * filter[13];
    tmp += input_0[14] * filter[14];
    tmp += input_0[15] * filter[15];
    *sum_0 += tmp;
    tmp = 0;
    tmp += input_1[0] * filter[0];
    tmp += input_1[1] * filter[1];
    tmp += input_1[2] * filter[2];
    tmp += input_1[3] * filter[3];
    tmp += input_1[4] * filter[4];
    tmp += input_1[5] * filter[5];
    tmp += input_1[6] * filter[6];
    tmp += input_1[7] * filter[7];
    tmp += input_1[8] * filter[8];
    tmp += input_1[9] * filter[9];
    tmp += input_1[10] * filter[10];
    tmp += input_1[11] * filter[11];
    tmp += input_1[12] * filter[12];
    tmp += input_1[13] * filter[13];
    tmp += input_1[14] * filter[14];
    tmp += input_1[15] * filter[15];
    *sum_1 += tmp;
    tmp = 0;
    tmp += input_2[0] * filter[0];
    tmp += input_2[1] * filter[1];
    tmp += input_2[2] * filter[2];
    tmp += input_2[3] * filter[3];
    tmp += input_2[4] * filter[4];
    tmp += input_2[5] * filter[5];
    tmp += input_2[6] * filter[6];
    tmp += input_2[7] * filter[7];
    tmp += input_2[8] * filter[8];
    tmp += input_2[9] * filter[9];
    tmp += input_2[10] * filter[10];
    tmp += input_2[11] * filter[11];
    tmp += input_2[12] * filter[12];
    tmp += input_2[13] * filter[13];
    tmp += input_2[14] * filter[14];
    tmp += input_2[15] * filter[15];
    *sum_2 += tmp;
    tmp = 0;
    tmp += input_3[0] * filter[0];
    tmp += input_3[1] * filter[1];
    tmp += input_3[2] * filter[2];
    tmp += input_3[3] * filter[3];
    tmp += input_3[4] * filter[4];
    tmp += input_3[5] * filter[5];
    tmp += input_3[6] * filter[6];
    tmp += input_3[7] * filter[7];
    tmp += input_3[8] * filter[8];
    tmp += input_3[9] * filter[9];
    tmp += input_3[10] * filter[10];
    tmp += input_3[11] * filter[11];
    tmp += input_3[12] * filter[12];
    tmp += input_3[13] * filter[13];
    tmp += input_3[14] * filter[14];
    tmp += input_3[15] * filter[15];
    *sum_3 += tmp;
}
/* END: MAC Functions for Group Conv */


/* START: MAC Functions for Transpose Depthwise Conv */
/* START: For 3x3 kernel size*/
static inline void transpose_depthwise_mac_kernel3_2row_uniweight(q31_t* sum_0, q31_t* sum_1, 
                      const q7_t* im2col_buffer, const q7_t* ksrc_transposed, const uint16_t input_width, 
                      const uint16_t STRIDE, const uint16_t IN_PAD, const uint16_t OUT_PAD) {
    *sum_0 += im2col_buffer[0] * ksrc_transposed[0];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[0];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[1];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[1];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[2];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[2];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[3];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[3];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[4];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[4];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[5];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[5];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[6];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[6];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[7];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[7];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[8];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[8];
}

static inline void transpose_depthwise_mac_kernel3_1row_uniweight(q31_t* sum_0, 
                      const q7_t* im2col_buffer, const q7_t* ksrc_transposed, const uint16_t input_width, 
                      const uint16_t STRIDE, const uint16_t IN_PAD, const uint16_t OUT_PAD) {
    *sum_0 += im2col_buffer[0] * ksrc_transposed[0];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[1];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[2];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[3];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[4];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[5];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[6];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[7];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[8];
}
/* END: For 3x3 kernel size*/


/* START: For 5x5 kernel size*/
static inline void transpose_depthwise_mac_kernel5_2row_uniweight(q31_t* sum_0, q31_t* sum_1, 
                      const q7_t* im2col_buffer, const q7_t* ksrc_transposed, const uint16_t input_width, 
                      const uint16_t STRIDE, const uint16_t IN_PAD, const uint16_t OUT_PAD) {
    *sum_0 += im2col_buffer[0] * ksrc_transposed[0];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[0];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[1];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[1];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[2];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[2];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[3];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[3];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[4];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[4];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[5];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[5];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[6];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[6];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[7];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[7];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[8];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[8];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[9];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[9];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[10];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[10];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[11];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[11];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[12];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[12];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[13];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[13];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[14];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[14];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[15];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[15];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[16];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[16];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[17];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[17];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[18];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[18];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[19];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[19];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[20];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[20];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[21];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[21];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[22];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[22];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[23];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[23];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[24];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[24];
}

static inline void transpose_depthwise_mac_kernel5_1row_uniweight(q31_t* sum_0, 
                      const q7_t* im2col_buffer, const q7_t* ksrc_transposed, const uint16_t input_width, 
                      const uint16_t STRIDE, const uint16_t IN_PAD, const uint16_t OUT_PAD) {
    *sum_0 += im2col_buffer[0] * ksrc_transposed[0];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[1];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[2];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[3];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[4];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[5];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[6];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[7];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[8];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[9];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[10];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[11];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[12];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[13];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[14];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[15];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[16];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[17];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[18];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[19];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[20];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[21];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[22];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[23];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[24];
}
/* END: For 5x5 kernel size*/


/* START: For 7x7 kernel size*/
static inline void transpose_depthwise_mac_kernel7_2row_uniweight(q31_t* sum_0, q31_t* sum_1, 
                      const q7_t* im2col_buffer, const q7_t* ksrc_transposed, const uint16_t input_width, 
                      const uint16_t STRIDE, const uint16_t IN_PAD, const uint16_t OUT_PAD) {
    *sum_0 += im2col_buffer[0] * ksrc_transposed[0];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[0];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[1];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[1];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[2];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[2];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[3];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[3];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[4];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[4];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[5];
    *sum_1 += im2col_buffer[6] * ksrc_transposed[5];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[6];
    *sum_1 += im2col_buffer[7] * ksrc_transposed[6];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[7];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[7];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[8];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[8];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[9];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[9];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[10];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[10];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[11];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[11];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[12];
    *sum_1 += im2col_buffer[6] * ksrc_transposed[12];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[13];
    *sum_1 += im2col_buffer[7] * ksrc_transposed[13];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[14];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[14];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[15];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[15];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[16];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[16];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[17];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[17];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[18];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[18];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[19];
    *sum_1 += im2col_buffer[6] * ksrc_transposed[19];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[20];
    *sum_1 += im2col_buffer[7] * ksrc_transposed[20];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[21];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[21];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[22];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[22];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[23];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[23];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[24];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[24];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[25];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[25];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[26];
    *sum_1 += im2col_buffer[6] * ksrc_transposed[26];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[27];
    *sum_1 += im2col_buffer[7] * ksrc_transposed[27];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[28];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[28];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[29];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[29];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[30];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[30];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[31];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[31];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[32];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[32];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[33];
    *sum_1 += im2col_buffer[6] * ksrc_transposed[33];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[34];
    *sum_1 += im2col_buffer[7] * ksrc_transposed[34];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[35];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[35];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[36];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[36];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[37];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[37];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[38];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[38];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[39];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[39];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[40];
    *sum_1 += im2col_buffer[6] * ksrc_transposed[40];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[41];
    *sum_1 += im2col_buffer[7] * ksrc_transposed[41];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[42];
    *sum_1 += im2col_buffer[1] * ksrc_transposed[42];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[43];
    *sum_1 += im2col_buffer[2] * ksrc_transposed[43];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[44];
    *sum_1 += im2col_buffer[3] * ksrc_transposed[44];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[45];
    *sum_1 += im2col_buffer[4] * ksrc_transposed[45];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[46];
    *sum_1 += im2col_buffer[5] * ksrc_transposed[46];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[47];
    *sum_1 += im2col_buffer[6] * ksrc_transposed[47];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[48];
    *sum_1 += im2col_buffer[7] * ksrc_transposed[48];
}

static inline void transpose_depthwise_mac_kernel7_1row_uniweight(q31_t* sum_0, 
                      const q7_t* im2col_buffer, const q7_t* ksrc_transposed, const uint16_t input_width, 
                      const uint16_t STRIDE, const uint16_t IN_PAD, const uint16_t OUT_PAD) {
    *sum_0 += im2col_buffer[0] * ksrc_transposed[0];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[1];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[2];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[3];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[4];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[5];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[6];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[7];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[8];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[9];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[10];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[11];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[12];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[13];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;
    
    *sum_0 += im2col_buffer[0] * ksrc_transposed[14];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[15];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[16];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[17];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[18];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[19];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[20];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[21];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[22];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[23];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[24];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[25];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[26];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[27];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[28];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[29];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[30];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[31];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[32];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[33];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[34];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[35];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[36];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[37];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[38];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[39];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[40];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[41];
    im2col_buffer += (input_width - 1) * STRIDE + 1 + IN_PAD * 2 + OUT_PAD;

    *sum_0 += im2col_buffer[0] * ksrc_transposed[42];
    *sum_0 += im2col_buffer[1] * ksrc_transposed[43];
    *sum_0 += im2col_buffer[2] * ksrc_transposed[44];
    *sum_0 += im2col_buffer[3] * ksrc_transposed[45];
    *sum_0 += im2col_buffer[4] * ksrc_transposed[46];
    *sum_0 += im2col_buffer[5] * ksrc_transposed[47];
    *sum_0 += im2col_buffer[6] * ksrc_transposed[48];
}
/* END: For 7x7 kernel size*/
/* END: MAC Functions for Transpose Depthwise Conv */


/* START: Assign Output Functions */
/* START: For Pointwise Conv */
static inline void assign_sum_to_pointwise_tmp_output_buffer_4row8col_int8(q31_t* out_0, q31_t* out_1, q31_t* out_2, q31_t* out_3, 
                      const q31_t* sum) {
    *out_0++ += sum[0];
    *out_1++ += sum[1];
    *out_2++ += sum[2];
    *out_3++ += sum[3];
    *out_0++ += sum[4];
    *out_1++ += sum[5];
    *out_2++ += sum[6];
    *out_3++ += sum[7];
    *out_0++ += sum[8];
    *out_1++ += sum[9];
    *out_2++ += sum[10];
    *out_3++ += sum[11];
    *out_0++ += sum[12];
    *out_1++ += sum[13];
    *out_2++ += sum[14];
    *out_3++ += sum[15];
    *out_0++ += sum[16];
    *out_1++ += sum[17];
    *out_2++ += sum[18];
    *out_3++ += sum[19];
    *out_0++ += sum[20];
    *out_1++ += sum[21];
    *out_2++ += sum[22];
    *out_3++ += sum[23];
    *out_0++ += sum[24];
    *out_1++ += sum[25];
    *out_2++ += sum[26];
    *out_3++ += sum[27];
    *out_0++ += sum[28];
    *out_1++ += sum[29];
    *out_2++ += sum[30];
    *out_3++ += sum[31];
}

static inline void assign_sum_to_pointwise_tmp_output_buffer_1row8col_int8(q31_t* out_0, const q31_t* sum) {
    *out_0++ += sum[0];
    *out_0++ += sum[1];
    *out_0++ += sum[2];
    *out_0++ += sum[3];
    *out_0++ += sum[4];
    *out_0++ += sum[5];
    *out_0++ += sum[6];
    *out_0++ += sum[7];
}

static inline void assign_sum_to_pointwise_tmp_output_buffer_4row4col_int8(q31_t* out_0, q31_t* out_1, q31_t* out_2, q31_t* out_3, 
                      const q31_t* sum) {
    *out_0++ += sum[0];
    *out_1++ += sum[1];
    *out_2++ += sum[2];
    *out_3++ += sum[3];
    *out_0++ += sum[4];
    *out_1++ += sum[5];
    *out_2++ += sum[6];
    *out_3++ += sum[7];
    *out_0++ += sum[8];
    *out_1++ += sum[9];
    *out_2++ += sum[10];
    *out_3++ += sum[11];
    *out_0++ += sum[12];
    *out_1++ += sum[13];
    *out_2++ += sum[14];
    *out_3++ += sum[15];
}

static inline void assign_sum_to_pointwise_tmp_output_buffer_1row4col_int8(q31_t* out_0, const q31_t* sum) {
    *out_0++ += sum[0];
    *out_0++ += sum[1];
    *out_0++ += sum[2];
    *out_0++ += sum[3];
}
/* END: For Pointwise Conv */


/* START: For Group Conv */
static inline void assign_sum_to_group_tmp_output_buffer_4row8col_int8(q31_t* out_0, q31_t* out_1, q31_t* out_2, q31_t* out_3, 
                      q31_t* out_4, q31_t* out_5, q31_t* out_6, q31_t* out_7,
                      const q31_t* sum_0, const q31_t* sum_1, const q31_t* sum_2, const q31_t* sum_3, 
                      q31_t* out_max_0, q31_t* out_max_1, q31_t* out_max_2, q31_t* out_max_3, 
                      q31_t* out_max_4, q31_t* out_max_5, q31_t* out_max_6, q31_t* out_max_7) {
  *out_0++ = sum_0[0];
  *out_max_0 = TN_MAX(abs(*out_max_0), abs(sum_0[0]));
  *out_1++ = sum_0[1];
  *out_max_1 = TN_MAX(abs(*out_max_1), abs(sum_0[1]));
  *out_2++ = sum_0[2];
  *out_max_2 = TN_MAX(abs(*out_max_2), abs(sum_0[2]));
  *out_3++ = sum_0[3];
  *out_max_3 = TN_MAX(abs(*out_max_3), abs(sum_0[3]));
  *out_4++ = sum_0[4];
  *out_max_4 = TN_MAX(abs(*out_max_4), abs(sum_0[4]));
  *out_5++ = sum_0[5];
  *out_max_5 = TN_MAX(abs(*out_max_5), abs(sum_0[5]));
  *out_6++ = sum_0[6];
  *out_max_6 = TN_MAX(abs(*out_max_6), abs(sum_0[6]));
  *out_7++ = sum_0[7];
  *out_max_7 = TN_MAX(abs(*out_max_7), abs(sum_0[7]));

  *out_0++ = sum_1[0];
  *out_max_0 = TN_MAX(abs(*out_max_0), abs(sum_1[0]));
  *out_1++ = sum_1[1];
  *out_max_1 = TN_MAX(abs(*out_max_1), abs(sum_1[1]));
  *out_2++ = sum_1[2];
  *out_max_2 = TN_MAX(abs(*out_max_2), abs(sum_1[2]));
  *out_3++ = sum_1[3];
  *out_max_3 = TN_MAX(abs(*out_max_3), abs(sum_1[3]));
  *out_4++ = sum_1[4];
  *out_max_4 = TN_MAX(abs(*out_max_4), abs(sum_1[4]));
  *out_5++ = sum_1[5];
  *out_max_5 = TN_MAX(abs(*out_max_5), abs(sum_1[5]));
  *out_6++ = sum_1[6];
  *out_max_6 = TN_MAX(abs(*out_max_6), abs(sum_1[6]));
  *out_7++ = sum_1[7];
  *out_max_7 = TN_MAX(abs(*out_max_7), abs(sum_1[7]));

  *out_0++ = sum_2[0];
  *out_max_0 = TN_MAX(abs(*out_max_0), abs(sum_2[0]));
  *out_1++ = sum_2[1];
  *out_max_1 = TN_MAX(abs(*out_max_1), abs(sum_2[1]));
  *out_2++ = sum_2[2];
  *out_max_2 = TN_MAX(abs(*out_max_2), abs(sum_2[2]));
  *out_3++ = sum_2[3];
  *out_max_3 = TN_MAX(abs(*out_max_3), abs(sum_2[3]));
  *out_4++ = sum_2[4];
  *out_max_4 = TN_MAX(abs(*out_max_4), abs(sum_2[4]));
  *out_5++ = sum_2[5];
  *out_max_5 = TN_MAX(abs(*out_max_5), abs(sum_2[5]));
  *out_6++ = sum_2[6];
  *out_max_6 = TN_MAX(abs(*out_max_6), abs(sum_2[6]));
  *out_7++ = sum_2[7];
  *out_max_7 = TN_MAX(abs(*out_max_7), abs(sum_2[7]));

  *out_0++ = sum_3[0];
  *out_max_0 = TN_MAX(abs(*out_max_0), abs(sum_3[0]));
  *out_1++ = sum_3[1];
  *out_max_1 = TN_MAX(abs(*out_max_1), abs(sum_3[1]));
  *out_2++ = sum_3[2];
  *out_max_2 = TN_MAX(abs(*out_max_2), abs(sum_3[2]));
  *out_3++ = sum_3[3];
  *out_max_3 = TN_MAX(abs(*out_max_3), abs(sum_3[3]));
  *out_4++ = sum_3[4];
  *out_max_4 = TN_MAX(abs(*out_max_4), abs(sum_3[4]));
  *out_5++ = sum_3[5];
  *out_max_5 = TN_MAX(abs(*out_max_5), abs(sum_3[5]));
  *out_6++ = sum_3[6];
  *out_max_6 = TN_MAX(abs(*out_max_6), abs(sum_3[6]));
  *out_7++ = sum_3[7];
  *out_max_7 = TN_MAX(abs(*out_max_7), abs(sum_3[7]));
}

static inline void assign_sum_to_group_tmp_output_buffer_4row16col_int8(q31_t* out_0, q31_t* out_1, q31_t* out_2, q31_t* out_3, q31_t* out_4, q31_t* out_5, 
          q31_t* out_6, q31_t* out_7, q31_t* out_8, q31_t* out_9, q31_t* out_10, q31_t* out_11, q31_t* out_12, q31_t* out_13, q31_t* out_14, q31_t* out_15, 
          const q31_t* sum_0, const q31_t* sum_1, const q31_t* sum_2, const q31_t* sum_3, 
          q31_t* out_max_0, q31_t* out_max_1, q31_t* out_max_2, q31_t* out_max_3, q31_t* out_max_4, q31_t* out_max_5, q31_t* out_max_6, q31_t* out_max_7,
          q31_t* out_max_8, q31_t* out_max_9, q31_t* out_max_10, q31_t* out_max_11, q31_t* out_max_12, q31_t* out_max_13, q31_t* out_max_14, q31_t* out_max_15) {
  *out_0++ = sum_0[0];
  *out_max_0 = TN_MAX(abs(*out_max_0), abs(sum_0[0]));
  *out_1++ = sum_0[1];
  *out_max_1 = TN_MAX(abs(*out_max_1), abs(sum_0[1]));
  *out_2++ = sum_0[2];
  *out_max_2 = TN_MAX(abs(*out_max_2), abs(sum_0[2]));
  *out_3++ = sum_0[3];
  *out_max_3 = TN_MAX(abs(*out_max_3), abs(sum_0[3]));
  *out_4++ = sum_0[4];
  *out_max_4 = TN_MAX(abs(*out_max_4), abs(sum_0[4]));
  *out_5++ = sum_0[5];
  *out_max_5 = TN_MAX(abs(*out_max_5), abs(sum_0[5]));
  *out_6++ = sum_0[6];
  *out_max_6 = TN_MAX(abs(*out_max_6), abs(sum_0[6]));
  *out_7++ = sum_0[7];
  *out_max_7 = TN_MAX(abs(*out_max_7), abs(sum_0[7]));
  *out_8++ = sum_0[8];
  *out_max_8 = TN_MAX(abs(*out_max_8), abs(sum_0[8]));
  *out_9++ = sum_0[9];
  *out_max_9 = TN_MAX(abs(*out_max_9), abs(sum_0[9]));
  *out_10++ = sum_0[10];
  *out_max_10 = TN_MAX(abs(*out_max_10), abs(sum_0[10]));
  *out_11++ = sum_0[11];
  *out_max_11 = TN_MAX(abs(*out_max_11), abs(sum_0[11]));
  *out_12++ = sum_0[12];
  *out_max_12 = TN_MAX(abs(*out_max_12), abs(sum_0[12]));
  *out_13++ = sum_0[13];
  *out_max_13 = TN_MAX(abs(*out_max_13), abs(sum_0[13]));
  *out_14++ = sum_0[14];
  *out_max_14 = TN_MAX(abs(*out_max_14), abs(sum_0[14]));
  *out_15++ = sum_0[15];
  *out_max_15 = TN_MAX(abs(*out_max_15), abs(sum_0[15]));

  *out_0++ = sum_1[0];
  *out_max_0 = TN_MAX(abs(*out_max_0), abs(sum_1[0]));
  *out_1++ = sum_1[1];
  *out_max_1 = TN_MAX(abs(*out_max_1), abs(sum_1[1]));
  *out_2++ = sum_1[2];
  *out_max_2 = TN_MAX(abs(*out_max_2), abs(sum_1[2]));
  *out_3++ = sum_1[3];
  *out_max_3 = TN_MAX(abs(*out_max_3), abs(sum_1[3]));
  *out_4++ = sum_1[4];
  *out_max_4 = TN_MAX(abs(*out_max_4), abs(sum_1[4]));
  *out_5++ = sum_1[5];
  *out_max_5 = TN_MAX(abs(*out_max_5), abs(sum_1[5]));
  *out_6++ = sum_1[6];
  *out_max_6 = TN_MAX(abs(*out_max_6), abs(sum_1[6]));
  *out_7++ = sum_1[7];
  *out_max_7 = TN_MAX(abs(*out_max_7), abs(sum_1[7]));
  *out_8++ = sum_1[8];
  *out_max_8 = TN_MAX(abs(*out_max_8), abs(sum_1[8]));
  *out_9++ = sum_1[9];
  *out_max_9 = TN_MAX(abs(*out_max_9), abs(sum_1[9]));
  *out_10++ = sum_1[10];
  *out_max_10 = TN_MAX(abs(*out_max_10), abs(sum_1[10]));
  *out_11++ = sum_1[11];
  *out_max_11 = TN_MAX(abs(*out_max_11), abs(sum_1[11]));
  *out_12++ = sum_1[12];
  *out_max_12 = TN_MAX(abs(*out_max_12), abs(sum_1[12]));
  *out_13++ = sum_1[13];
  *out_max_13 = TN_MAX(abs(*out_max_13), abs(sum_1[13]));
  *out_14++ = sum_1[14];
  *out_max_14 = TN_MAX(abs(*out_max_14), abs(sum_1[14]));
  *out_15++ = sum_1[15];
  *out_max_15 = TN_MAX(abs(*out_max_15), abs(sum_1[15]));

  *out_0++ = sum_2[0];
  *out_max_0 = TN_MAX(abs(*out_max_0), abs(sum_2[0]));
  *out_1++ = sum_2[1];
  *out_max_1 = TN_MAX(abs(*out_max_1), abs(sum_2[1]));
  *out_2++ = sum_2[2];
  *out_max_2 = TN_MAX(abs(*out_max_2), abs(sum_2[2]));
  *out_3++ = sum_2[3];
  *out_max_3 = TN_MAX(abs(*out_max_3), abs(sum_2[3]));
  *out_4++ = sum_2[4];
  *out_max_4 = TN_MAX(abs(*out_max_4), abs(sum_2[4]));
  *out_5++ = sum_2[5];
  *out_max_5 = TN_MAX(abs(*out_max_5), abs(sum_2[5]));
  *out_6++ = sum_2[6];
  *out_max_6 = TN_MAX(abs(*out_max_6), abs(sum_2[6]));
  *out_7++ = sum_2[7];
  *out_max_7 = TN_MAX(abs(*out_max_7), abs(sum_2[7]));
  *out_8++ = sum_2[8];
  *out_max_8 = TN_MAX(abs(*out_max_8), abs(sum_2[8]));
  *out_9++ = sum_2[9];
  *out_max_9 = TN_MAX(abs(*out_max_9), abs(sum_2[9]));
  *out_10++ = sum_2[10];
  *out_max_10 = TN_MAX(abs(*out_max_10), abs(sum_2[10]));
  *out_11++ = sum_2[11];
  *out_max_11 = TN_MAX(abs(*out_max_11), abs(sum_2[11]));
  *out_12++ = sum_2[12];
  *out_max_12 = TN_MAX(abs(*out_max_12), abs(sum_2[12]));
  *out_13++ = sum_2[13];
  *out_max_13 = TN_MAX(abs(*out_max_13), abs(sum_2[13]));
  *out_14++ = sum_2[14];
  *out_max_14 = TN_MAX(abs(*out_max_14), abs(sum_2[14]));
  *out_15++ = sum_2[15];
  *out_max_15 = TN_MAX(abs(*out_max_15), abs(sum_2[15]));

  *out_0++ = sum_3[0];
  *out_max_0 = TN_MAX(abs(*out_max_0), abs(sum_3[0]));
  *out_1++ = sum_3[1];
  *out_max_1 = TN_MAX(abs(*out_max_1), abs(sum_3[1]));
  *out_2++ = sum_3[2];
  *out_max_2 = TN_MAX(abs(*out_max_2), abs(sum_3[2]));
  *out_3++ = sum_3[3];
  *out_max_3 = TN_MAX(abs(*out_max_3), abs(sum_3[3]));
  *out_4++ = sum_3[4];
  *out_max_4 = TN_MAX(abs(*out_max_4), abs(sum_3[4]));
  *out_5++ = sum_3[5];
  *out_max_5 = TN_MAX(abs(*out_max_5), abs(sum_3[5]));
  *out_6++ = sum_3[6];
  *out_max_6 = TN_MAX(abs(*out_max_6), abs(sum_3[6]));
  *out_7++ = sum_3[7];
  *out_max_7 = TN_MAX(abs(*out_max_7), abs(sum_3[7]));
  *out_8++ = sum_3[8];
  *out_max_8 = TN_MAX(abs(*out_max_8), abs(sum_3[8]));
  *out_9++ = sum_3[9];
  *out_max_9 = TN_MAX(abs(*out_max_9), abs(sum_3[9]));
  *out_10++ = sum_3[10];
  *out_max_10 = TN_MAX(abs(*out_max_10), abs(sum_3[10]));
  *out_11++ = sum_3[11];
  *out_max_11 = TN_MAX(abs(*out_max_11), abs(sum_3[11]));
  *out_12++ = sum_3[12];
  *out_max_12 = TN_MAX(abs(*out_max_12), abs(sum_3[12]));
  *out_13++ = sum_3[13];
  *out_max_13 = TN_MAX(abs(*out_max_13), abs(sum_3[13]));
  *out_14++ = sum_3[14];
  *out_max_14 = TN_MAX(abs(*out_max_14), abs(sum_3[14]));
  *out_15++ = sum_3[15];
  *out_max_15 = TN_MAX(abs(*out_max_15), abs(sum_3[15]));
}
/* END: For Group Conv */
/* END: Assign Output Functions */
