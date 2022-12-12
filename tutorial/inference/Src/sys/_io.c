/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   _io.c
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
  
#include "stm32746g_discovery.h"

volatile int32_t ITM_RxBuffer;

int __io_putchar(int ch) { return ITM_SendChar(ch); }

int __io_getchar(void) { return ITM_ReceiveChar(); }
