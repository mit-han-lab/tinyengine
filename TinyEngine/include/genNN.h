/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   genNN.h
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, arXiv:2206.15472
 * Contact authors:
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#ifndef INC_GENNN_H_
#define INC_GENNN_H_

#include <stdint.h>

signed char* getInput();
signed char* getOutput();
float* getOutput_fp();
int32_t* getOutput_int32();

void setupBuffer();
void invoke(float* labels);
void getResult(uint8_t *P, uint8_t *NP);
int* getKbuffer();
void end2endinference();

#endif /* INC_GENNN_H_ */
