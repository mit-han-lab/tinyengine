/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   camera_i2c.h
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

#ifndef CAMERA_I2C_H_
#define CAMERA_I2C_H_

#include "stm32746g_discovery.h"
#include "stm32f7xx_hal.h"

struct sensor_reg {
  uint8_t reg;
  uint8_t val;
};

const struct sensor_reg testRegs[] = {
    {0xff, 0x0},
    {0x2c, 0xff},
};

#define I2Cx I2C1
#define RCC_PERIPHCLK_I2Cx RCC_PERIPHCLK_I2C1
#define RCC_I2CxCLKSOURCE_SYSCLK RCC_I2C1CLKSOURCE_PCLK1
#define I2Cx_CLK_ENABLE() __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define I2Cx_FORCE_RESET() __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET() __HAL_RCC_I2C1_RELEASE_RESET()

#define I2Cx_SCL_PIN GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT GPIOB
#define I2Cx_SDA_PIN GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT GPIOB
#define I2Cx_SCL_SDA_AF GPIO_AF4_I2C1

#define TXBUFFERSIZE (COUNTOF(aTxBuffer) - 1)

#define RXBUFFERSIZE TXBUFFERSIZE

#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

int camI2CSetup();
int camI2CErrorTest();

int wrSensorReg16_8(uint16_t regID, uint16_t regDat);
int wrSensorRegs16_8(const struct sensor_reg reglist[]);
int rdSensorReg16_8(uint16_t regID, uint8_t *regDat);
int wrSensorReg8_8(uint8_t regID, uint8_t regDat);
int wrSensorRegs8_8(const struct sensor_reg reglist[]);
int rdSensorReg8_8(uint8_t regID, uint8_t *regDat);

#endif
