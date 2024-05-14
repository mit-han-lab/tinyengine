/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   lcd.cpp
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

#include "lcd.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32f7xx_hal.h"

#define TRANS 128

void loadRGB565LCD(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
                   uint16_t *src, uint8_t resize) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

      uint16_t color = src[i * width + j];

      for (int ti = 0; ti < resize; ti++) {
        for (int tj = 0; tj < resize; tj++) {
          BSP_LCD_DrawPixel(x + j * resize + tj, y + i * resize + ti, color);
        }
      }
    }
  }
}

void drawRedBackground(int x1, int x2, int y1, int y2) {
  uint16_t red = 63488;

  for (int i = x1 - 1; i < x2; i++)
    for (int j = y1 - 1; j < y2; j++) {
      BSP_LCD_DrawPixel(i, j, red);
    }
}

void drawGreenBackground(int x1, int x2, int y1, int y2) {
  uint16_t green = 2016;

  for (int i = x1 - 1; i < x2; i++)
    for (int j = y1 - 1; j < y2; j++) {
      BSP_LCD_DrawPixel(i, j, green);
    }
}

void drawBlueBackground(int x1, int x2, int y1, int y2) {
  uint16_t blue = 2016 + 63488;

  for (int i = x1 - 1; i < x2; i++)
    for (int j = y1 - 1; j < y2; j++) {
      BSP_LCD_DrawPixel(i, j, blue);
    }
}

void drawBlackBackground(int x1, int x2, int y1, int y2) {
  uint16_t black = 0;

  for (int i = x1 - 1; i < x2; i++)
    for (int j = y1 - 1; j < y2; j++) {
      BSP_LCD_DrawPixel(i, j, black);
    }
}

void displaystring(char *buf, int x, int y) {
  BSP_LCD_DisplayStringAt(x, y, buf, LEFT_MODE);
}

#define USENEW
int unper_cnt = 0;

#ifdef USENEW
void detectResponse(int person, float ms, int training_mode, int pred,
                    int label) {
  char buf[20];
  if (person) {
    unper_cnt = 0;
    if (training_mode) {
      drawGreenBackground(270, 480, 40, 100);
      drawGreenBackground(270, 480, 125, 180);
      drawGreenBackground(270, 480, 205, 250);
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      sprintf(buf, " Prediction:");
      BSP_LCD_DisplayStringAt(273, 80, buf, LEFT_MODE);
      sprintf(buf, "  class %d  ", pred);
      BSP_LCD_DisplayStringAt(273, 100, buf, LEFT_MODE);
      sprintf(buf, "Ground True:");
      BSP_LCD_DisplayStringAt(273, 120, buf, LEFT_MODE);
      sprintf(buf, "  class %d   ", label);
      BSP_LCD_DisplayStringAt(273, 140, buf, LEFT_MODE);
    } else {
      drawBlueBackground(270, 480, 40, 100);
      drawBlueBackground(270, 480, 125, 180);
      drawBlueBackground(270, 480, 205, 250);
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_DisplayStringAt(273, 100, "   Person   ", LEFT_MODE);
    }
  } else {
    if (training_mode) {
      drawRedBackground(270, 480, 40, 100);
      drawRedBackground(270, 480, 125, 180);
      drawRedBackground(270, 480, 205, 250);
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      sprintf(buf, " Prediction:");
      BSP_LCD_DisplayStringAt(273, 80, buf, LEFT_MODE);
      sprintf(buf, "  class %d   ", pred);
      BSP_LCD_DisplayStringAt(273, 100, buf, LEFT_MODE);
      sprintf(buf, "Ground-Truth");
      BSP_LCD_DisplayStringAt(273, 120, buf, LEFT_MODE);
      sprintf(buf, "  class %d   ", label);
      BSP_LCD_DisplayStringAt(273, 140, buf, LEFT_MODE);
    } else {
      drawBlackBackground(270, 480, 40, 100);
      drawBlackBackground(270, 480, 125, 180);
      drawBlackBackground(270, 480, 205, 250);
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_DisplayStringAt(273, 100, "  No Person ", LEFT_MODE);
    }
  }

  if (ms == 0)
    return;
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  volatile float rate = 1000 / ms;
  volatile int decimal = (int)rate;
  volatile int floating = (int)((rate - (float)decimal) * 1000);
  sprintf(buf, "  fps:%d.%03d ", decimal, floating);
  BSP_LCD_DisplayStringAt(273, 180, buf, LEFT_MODE);
}

#else
void detectResponse(int person, float ms) {
  if (person) {
    unper_cnt = 0;
    drawRedBackground(270, 480, 40, 100);
    drawRedBackground(270, 480, 125, 180);
    drawRedBackground(270, 480, 205, 250);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(273, 100, "  No Person ", LEFT_MODE);
  } else {
    unper_cnt++;

    drawGreenBackground(270, 480, 40, 100);
    drawGreenBackground(270, 480, 125, 180);
    drawGreenBackground(270, 480, 205, 250);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(273, 100, "   Person   ", LEFT_MODE);
  }

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  char buf[20];
  volatile float rate = 1000 / ms;
  volatile int decimal = (int)rate;
  volatile int floating = (int)((rate - (float)decimal) * 1000);
  sprintf(buf, "  fps:%d.%03d ", decimal, floating);
  BSP_LCD_DisplayStringAt(273, 180, buf, LEFT_MODE);
}
#endif

void lcdsetup() {
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  BSP_LCD_Init();

  BSP_LCD_LayerRgb565Init(0, LCD_FB_START_ADDRESS);
  BSP_LCD_LayerRgb565Init(1, LCD_FB_START_ADDRESS +
                                 (BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4));

  BSP_LCD_DisplayOn();

  BSP_LCD_SelectLayer(0);

  BSP_LCD_Clear(LCD_COLOR_BLACK);

  BSP_LCD_SelectLayer(1);

  BSP_LCD_Clear(LCD_COLOR_BLACK);

  BSP_LCD_SetTransparency(0, 0);
  BSP_LCD_SetTransparency(1, 100);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
}
