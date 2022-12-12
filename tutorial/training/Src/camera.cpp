/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   camera.cpp
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
#include "camera.h"
#include "camera_i2c.h"
#include "camera_spi.h"
#include "ov2640_regs.h"
#include <JPEGDecoder.h>
#include <assert.h>
#include <stdio.h>

uint8_t imgBuf[5120];
uint32_t imgLength;
bool is_header = false;
static byte m_fmt = JPEG;

void flush_fifo(void) { camWriteReg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK); }

void clear_fifo_flag(void) { camWriteReg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK); }

void start_capture(void) { camWriteReg(ARDUCHIP_FIFO, FIFO_START_MASK); }

void set_format(byte fmt) { m_fmt = fmt; }

int initCamera() {
  int camrror = camSPISetup();
  camrror += camI2CSetup();

  wrSensorReg8_8(0xff, 0x01);

  wrSensorReg8_8(0x12, 0x80);

  HAL_Delay(100);
  if (m_fmt == JPEG) {
    wrSensorRegs8_8(OV2640_JPEG_INIT);
    wrSensorRegs8_8(OV2640_YUV422);
    wrSensorRegs8_8(OV2640_JPEG);
    wrSensorReg8_8(0xff, 0x01);
    wrSensorReg8_8(0x15, 0x00);
    wrSensorRegs8_8(OV2640_320x240_JPEG);
    HAL_Delay(100);

  } else {
    wrSensorRegs8_8(OV2640_QVGA);
  }

  OV2640_set_JPEG_size(OV2640_160x120);

  HAL_Delay(100);
  return camrror;
}

void OV2640_set_JPEG_size(uint8_t size) {
  switch (size) {
  case OV2640_160x120:
    wrSensorRegs8_8(OV2640_160x120_JPEG);
    break;
  case OV2640_176x144:
    wrSensorRegs8_8(OV2640_176x144_JPEG);
    break;
  case OV2640_320x240:
    wrSensorRegs8_8(OV2640_320x240_JPEG);
    break;
  case OV2640_352x288:
    wrSensorRegs8_8(OV2640_352x288_JPEG);
    break;
  case OV2640_640x480:
    wrSensorRegs8_8(OV2640_640x480_JPEG);
    break;
  case OV2640_800x600:
    wrSensorRegs8_8(OV2640_800x600_JPEG);
    break;
  case OV2640_1024x768:
    wrSensorRegs8_8(OV2640_1024x768_JPEG);
    break;
  case OV2640_1280x1024:
    wrSensorRegs8_8(OV2640_1280x1024_JPEG);
    break;
  case OV2640_1600x1200:
    wrSensorRegs8_8(OV2640_1600x1200_JPEG);
    break;
  default:
    wrSensorRegs8_8(OV2640_320x240_JPEG);
    break;
  }
}

uint32_t read_fifo_length(void) {
  uint32_t len1, len2, len3, length = 0;
  len1 = camReadReg(FIFO_SIZE1);
  len2 = camReadReg(FIFO_SIZE2);
  len3 = camReadReg(FIFO_SIZE3) & 0x7f;
  length = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
  return length;
}

uint32_t getbufSize() { return imgLength; }

uint8_t getbufChar(int i) { return imgBuf[i]; }

int DecodeandProcessRGB565(int image_width, int image_height,
                           uint16_t *image_data) {

  JpegDec.decodeArray(imgBuf, imgLength);

  const int keep_x_mcus = image_width / JpegDec.MCUWidth;
  const int keep_y_mcus = image_height / JpegDec.MCUHeight;

  const int skip_x_mcus = JpegDec.MCUSPerRow - keep_x_mcus;

  const int skip_start_x_mcus = skip_x_mcus / 2;

  const int skip_end_x_mcu_index = skip_start_x_mcus + keep_x_mcus;

  const int skip_y_mcus = JpegDec.MCUSPerCol - keep_y_mcus;
  const int skip_start_y_mcus = skip_y_mcus / 2;
  const int skip_end_y_mcu_index = skip_start_y_mcus + keep_y_mcus;

  uint16_t *pImg;

  uint16_t color;

  while (JpegDec.read()) {

    if (JpegDec.MCUy < skip_start_y_mcus) {
      continue;
    }

    if (JpegDec.MCUx < skip_start_x_mcus ||
        JpegDec.MCUx >= skip_end_x_mcu_index) {
      continue;
    }

    if (JpegDec.MCUy >= skip_end_y_mcu_index) {
      continue;
    }

    pImg = JpegDec.pImage;

    int relative_mcu_x = JpegDec.MCUx - skip_start_x_mcus;
    int relative_mcu_y = JpegDec.MCUy - skip_start_y_mcus;

    int x_origin = relative_mcu_x * JpegDec.MCUWidth;
    int y_origin = relative_mcu_y * JpegDec.MCUHeight;

    for (int mcu_row = 0; mcu_row < JpegDec.MCUHeight; mcu_row++) {

      int current_y = y_origin + mcu_row;
      for (int mcu_col = 0; mcu_col < JpegDec.MCUWidth; mcu_col++) {

        color = *pImg++;

        int current_x = x_origin + mcu_col;

        int index = (current_y * image_width) + current_x;
        image_data[index] = color;
      }
    }
  }
}

int DecodeandProcessAndRGB(int image_width, int image_height,
                           int8_t *image_data, uint16_t *lcd_data,
                           int scale_factor) {

  JpegDec.decodeArray(imgBuf, imgLength);

  const int keep_x_mcus = image_width / JpegDec.MCUWidth;
  const int keep_y_mcus = image_height / JpegDec.MCUHeight;

  const int skip_x_mcus = JpegDec.MCUSPerRow - keep_x_mcus;

  const int skip_start_x_mcus = skip_x_mcus / 2;

  const int skip_end_x_mcu_index = skip_start_x_mcus + keep_x_mcus;

  const int skip_y_mcus = JpegDec.MCUSPerCol - keep_y_mcus;
  const int skip_start_y_mcus = skip_y_mcus / 2;
  const int skip_end_y_mcu_index = skip_start_y_mcus + keep_y_mcus;

  uint16_t *pImg;

  uint16_t color;

  for (int i = 0;
       i < (image_height / scale_factor) * (image_width / scale_factor) * 3;
       i++)
    image_data[i] = -128;

  while (JpegDec.read()) {

    if (JpegDec.MCUy < skip_start_y_mcus) {
      continue;
    }

    if (JpegDec.MCUx < skip_start_x_mcus ||
        JpegDec.MCUx >= skip_end_x_mcu_index) {
      continue;
    }

    if (JpegDec.MCUy >= skip_end_y_mcu_index) {
      continue;
    }

    pImg = JpegDec.pImage;

    int relative_mcu_x = JpegDec.MCUx - skip_start_x_mcus;
    int relative_mcu_y = JpegDec.MCUy - skip_start_y_mcus;

    int x_origin = relative_mcu_x * JpegDec.MCUWidth;
    int y_origin = relative_mcu_y * JpegDec.MCUHeight;

    for (int mcu_row = 0; mcu_row < JpegDec.MCUHeight; mcu_row++) {

      int current_y = y_origin + mcu_row;
      for (int mcu_col = 0; mcu_col < JpegDec.MCUWidth; mcu_col++) {

        color = *pImg++;

        uint8_t r, g, b;
        r = ((color & 0xF800) >> 11) * 8;
        g = ((color & 0x07E0) >> 5) * 4;
        b = ((color & 0x001F) >> 0) * 8;

        int current_x = x_origin + mcu_col;

        int index = (current_y * image_width) + current_x;
        int a_index = index * 3;

        if (current_y >= 120)
          continue;

        lcd_data[index] = color;
        if (scale_factor == 1) {
          image_data[index * 3] = r - 128;
          image_data[index * 3 + 1] = g - 128;
          image_data[index * 3 + 2] = b - 128;
          lcd_data[index] = color;
        }
        if (scale_factor != 1 &&
            (current_y % scale_factor != 0 || current_x % scale_factor != 0))
          continue;

        if (image_width % scale_factor != 0)
          continue;
        int width = image_width;
        if (width > 120)
          width = 120;
        int Iindex = (current_y / scale_factor) * (image_width / scale_factor) +
                     current_x / scale_factor;

        image_data[Iindex * 3] = r - 128;
        image_data[Iindex * 3 + 1] = g - 128;
        image_data[Iindex * 3 + 2] = b - 128;
      }
    }
  }
}

int DecodeandProcess(int image_width, int image_height, uint8_t *image_data) {

  JpegDec.decodeArray(imgBuf, imgLength);

  const int keep_x_mcus = image_width / JpegDec.MCUWidth;
  const int keep_y_mcus = image_height / JpegDec.MCUHeight;

  const int skip_x_mcus = JpegDec.MCUSPerRow - keep_x_mcus;

  const int skip_start_x_mcus = skip_x_mcus / 2;

  const int skip_end_x_mcu_index = skip_start_x_mcus + keep_x_mcus;

  const int skip_y_mcus = JpegDec.MCUSPerCol - keep_y_mcus;
  const int skip_start_y_mcus = skip_y_mcus / 2;
  const int skip_end_y_mcu_index = skip_start_y_mcus + keep_y_mcus;

  uint16_t *pImg;

  uint16_t color;

  while (JpegDec.read()) {

    if (JpegDec.MCUy < skip_start_y_mcus) {
      continue;
    }

    if (JpegDec.MCUx < skip_start_x_mcus ||
        JpegDec.MCUx >= skip_end_x_mcu_index) {
      continue;
    }

    if (JpegDec.MCUy >= skip_end_y_mcu_index) {
      continue;
    }

    pImg = JpegDec.pImage;

    int relative_mcu_x = JpegDec.MCUx - skip_start_x_mcus;
    int relative_mcu_y = JpegDec.MCUy - skip_start_y_mcus;

    int x_origin = relative_mcu_x * JpegDec.MCUWidth;
    int y_origin = relative_mcu_y * JpegDec.MCUHeight;

    for (int mcu_row = 0; mcu_row < JpegDec.MCUHeight; mcu_row++) {

      int current_y = y_origin + mcu_row;
      for (int mcu_col = 0; mcu_col < JpegDec.MCUWidth; mcu_col++) {

        color = *pImg++;

        uint8_t r, g, b;
        r = ((color & 0xF800) >> 11) * 8;
        g = ((color & 0x07E0) >> 5) * 4;
        b = ((color & 0x001F) >> 0) * 8;

        float gray_value = (0.2126 * r) + (0.7152 * g) + (0.0722 * b);

        int current_x = x_origin + mcu_col;

        int index = (current_y * image_width) + current_x;
        image_data[index] = static_cast<uint8_t>(gray_value);
      }
    }
  }
}

int PerformCapture() {

  flush_fifo();
  HAL_Delay(1);
  clear_fifo_flag();
  HAL_Delay(1);

  start_capture();

  while (!camReadRegBit(ARDUCHIP_TRIG, CAP_DONE_MASK)) {
  }

  clear_fifo_flag();
  read_fifo_burst();

  return 0;
}

int StartCapture() {

  flush_fifo();
  HAL_Delay(1);
  clear_fifo_flag();
  HAL_Delay(1);

  start_capture();

  return 0;
}

int ReadCapture() {

  while (!camReadRegBit(ARDUCHIP_TRIG, CAP_DONE_MASK)) {
  }

  clear_fifo_flag();
  read_fifo_burst();

  return 0;
}

uint8_t read_fifo_burst() {
  uint32_t length = read_fifo_length();
  if (length >= MAX_FIFO_SIZE) {

    return 0;
  }
  if (length == 0) {

    return 0;
  }
  ARDUCAM_CS_LOW;
  volatile uint8_t burst = camTransfer(BURST_FIFO_READ);
  int index;

  camTransfers(imgBuf, length);

  for (index = length - 1; index >= 0; index--) {
    if (imgBuf[index] != 0) {
      break;
    }
  }

  imgLength = index + 1;

  ARDUCAM_CS_HIGH;

  is_header = false;
  return 1;
}
