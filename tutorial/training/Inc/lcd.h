/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   lcd.h
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

#ifndef LCD_H_
#define LCD_H_
#include <stdio.h>

void loadRGB565LCD(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
                   uint16_t *src, uint8_t resize);
void loadgrayscale2LCD(uint8_t *src);
void lcdsetup();

#define USENEW
#ifdef USENEW
void detectResponse(int person, float ms, int training_mode, int pred,
                    int label);
#else
void detectResponse(int person, float ms);
#endif

void displaystring(char *buf, int x, int y);

#endif
