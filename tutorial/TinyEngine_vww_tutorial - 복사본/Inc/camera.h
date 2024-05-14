/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   camera.h
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

#ifndef CAMERA_H_
#define CAMERA_H_

#define byte uint8_t
#define BMP 0
#define JPEG 1

#define OV2640_160x120 0
#define OV2640_176x144 1
#define OV2640_320x240 2
#define OV2640_352x288 3
#define OV2640_640x480 4
#define OV2640_800x600 5
#define OV2640_1024x768 6
#define OV2640_1280x1024 7
#define OV2640_1600x1200 8

#define ARDUCHIP_FIFO 0x04
#define FIFO_CLEAR_MASK 0x01
#define FIFO_START_MASK 0x02
#define FIFO_RDPTR_RST_MASK 0x10
#define FIFO_WRPTR_RST_MASK 0x20

#define ARDUCHIP_TRIG 0x41
#define CAP_DONE_MASK 0x08

#define BURST_FIFO_READ 0x3C
#define FIFO_SIZE1 0x42
#define FIFO_SIZE2 0x43
#define FIFO_SIZE3 0x44

#define MAX_FIFO_SIZE 0x5FFFF

uint8_t getbufChar(int i);
uint32_t getbufSize();
int initCamera();
uint8_t read_fifo_burst();
void OV2640_set_JPEG_size(uint8_t size);
int PerformCapture();
int ReadCapture();
int StartCapture();
int DecodeandProcessAndRGB(int image_width, int image_height,
                           int8_t *image_data, uint16_t *lcd_data,
                           int scale_factor);
int DecodeandProcess(int image_width, int image_height, uint8_t *image_data);
int DecodeandProcessRGB565(int image_width, int image_height,
                           uint16_t *image_data);

#endif
