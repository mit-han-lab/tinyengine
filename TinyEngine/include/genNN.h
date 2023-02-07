/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   genNN.h
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

#ifndef INC_GENNN_H_
#define INC_GENNN_H_

#include <stdint.h>

#include "yoloOutput.h"

signed char* getInput();
signed char* getOutput();
float* getOutput_fp();
int32_t* getOutput_int32();
static float lr __attribute__((unused)) = 0.0008;   // To suppress warning
static float blr __attribute__((unused)) = 0.0004;  // To suppress warning

void setupBuffer();
void invoke(float* labels);
void invoke_inf();
void getResult(uint8_t* P, uint8_t* NP);
int* getKbuffer();
void end2endinference();
void det_post_procesing(int* box_cnt, det_box** ret_box, float threshold);

#endif /* INC_GENNN_H_ */
