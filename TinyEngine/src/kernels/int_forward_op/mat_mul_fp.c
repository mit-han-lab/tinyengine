/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   mat_mul_fp.c
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

tinyengine_status mat_mul_fp(
				const float *matA, const uint16_t matA_row, const uint16_t matA_col,
				const float* matB, const uint16_t matB_col, float* output)
{
	int m, n, i;
	for (n = 0; n < matA_row; n++){
		for (m = 0; m < matB_col; m++){
			float sum = 0;
			for (i = 0; i < matA_col; i++){
				sum += matA[i + n * matA_col] * matB[m + i * matA_col];
			}
			output[m + n * matB_col] = sum;
		}
	}
}
