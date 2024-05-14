/*
 * profile.h
 *
 *  Created on: Apr 20, 2024
 *      Author: sm5pyo
 */

#ifndef VIT_00_PROFILE_H_
#define VIT_00_PROFILE_H_


/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   profile.h
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
//#include "stm32746g_discovery.h"
#include "stm32f7xx_hal.h"
#include <stdio.h>
#include <string.h>
static UART_HandleTypeDef UART;
#define RUNS 1
//static int profile_i;
//static int start, end;
//static char buf[100];
void printLog(const char *s);
void recieveChar(char *s);





#endif /* VIT_00_PROFILE_H_ */
