/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   camera_spi.h
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

#ifndef CAMERA_SPI_H_
#define CAMERA_SPI_H_

#include "stm32746g_discovery.h"

#define ARDUCAM_CS_PORT GPIOI
#define ARDUCAM_CS_PIN GPIO_PIN_0
#define RESET GPIO_PIN_RESET
#define SET GPIO_PIN_SET
#define ARDUCAM_SPI_TIMEOUT 100
#define ARDUCHIP_TEST1 0x00

#define ARDUCAM_CS_LOW                                                         \
  HAL_GPIO_WritePin(ARDUCAM_CS_PORT, ARDUCAM_CS_PIN, RESET);
#define ARDUCAM_CS_HIGH HAL_GPIO_WritePin(ARDUCAM_CS_PORT, ARDUCAM_CS_PIN, SET);
#if !defined(ARDUCAM_CS_PORT) || !defined(ARDUCAM_CS_PIN) ||                   \
    !defined(ARDUCAM_CS_LOW) || !defined(ARDUCAM_CS_HIGH)
#error please define Arducam SPI settings
#endif

int camSPISetup();
void camSPIReset();
void camWriteReg(const uint8_t reg, const uint8_t val);
int camSPIErrorTest();
void camCoreDump();
uint8_t camTransfer(const uint8_t val);
uint8_t camTransfers(uint8_t *buf, const uint32_t length);
uint8_t camReadReg(const uint8_t reg);
uint8_t camReadRegBit(uint8_t addr, uint8_t bit);
int camSPIErrorTest();

#define SPIx SPI2
#define SPIx_CLK_ENABLE() __HAL_RCC_SPI2_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define SPIx_FORCE_RESET() __HAL_RCC_SPI2_FORCE_RESET()
#define SPIx_RELEASE_RESET() __HAL_RCC_SPI2_RELEASE_RESET()

#define SPIx_SCK_PIN GPIO_PIN_1
#define SPIx_SCK_GPIO_PORT GPIOI
#define SPIx_SCK_AF GPIO_AF5_SPI2
#define SPIx_MISO_PIN GPIO_PIN_14
#define SPIx_MISO_GPIO_PORT GPIOB
#define SPIx_MISO_AF GPIO_AF5_SPI2
#define SPIx_MOSI_PIN GPIO_PIN_15
#define SPIx_MOSI_GPIO_PORT GPIOB
#define SPIx_MOSI_AF GPIO_AF5_SPI2

#define ARDUCAM_CS_PORT GPIOI
#define ARDUCAM_CS_PIN GPIO_PIN_0
#define RESET GPIO_PIN_RESET
#define SET GPIO_PIN_SET
#define ARDUCAM_SPI_TIMEOUT 100
#define ARDUCHIP_TEST1 0x00

#endif
