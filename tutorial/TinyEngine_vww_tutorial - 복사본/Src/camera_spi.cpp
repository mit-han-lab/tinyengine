/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   camera_spi.cpp
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

 #include "camera_spi.h"

struct sensor_reg {
  uint8_t reg;
  uint8_t val;
};

uint8_t core[][2] = {
    {0x00, 0x00}, {0x01, 0x00}, {0x03, 0x00}, {0x04, 0x00}, {0x05, 0x00},
    {0x06, 0x00}, {0x07, 0x00}, {0x3C, 0x00}, {0x3D, 0x00}, {0x40, 0x00},
    {0x41, 0x00}, {0x42, 0x00}, {0x43, 0x00}, {0x44, 0x00}, {0x45, 0x00},
    {0x46, 0x00}, {0x47, 0x00}, {0x48, 0x00}, {0xFF, 0xFF},
};

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

void delayUS(uint32_t us) {
#define CLOCK_CYCLE_PER_INSTRUCTION 1
#define CLOCK_FREQ 216

  volatile int cycleCount = us * CLOCK_FREQ / CLOCK_CYCLE_PER_INSTRUCTION;

  while (cycleCount--)
    ;
}

void setBit(uint8_t addr, uint8_t bit) {
  uint8_t temp;
  temp = camReadReg(addr);
  camWriteReg(addr, temp | bit);
}

void clearBit(uint8_t addr, uint8_t bit) {
  uint8_t temp;
  temp = camReadReg(addr);
  camWriteReg(addr, temp & (~bit));
}

uint8_t getBit(uint8_t addr, uint8_t bit) {
  uint8_t temp;
  temp = camReadReg(addr);
  temp = temp & bit;
  return temp;
}

static SPI_HandleTypeDef SpiHandle;
int camSPISetup() {
  int error = 0;

  SpiHandle.Instance = SPIx;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
  SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
  SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
  SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SpiHandle.Init.CRCPolynomial = 7;
  SpiHandle.Init.NSS = SPI_NSS_SOFT;
  SpiHandle.Init.Mode = SPI_MODE_MASTER;

  if (HAL_SPI_Init(&SpiHandle) != HAL_OK) {

    error++;
  }

  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  camSPIReset();
  error += camSPIErrorTest();
  return error;
}

int camSPIErrorTest() {
  uint8_t val = 0x55;
  volatile uint8_t rval;
  int error = 0;

  for (int i = 0; i < 10; i++) {
    val++;
    camWriteReg(0x00, val);
    rval = camReadReg(0x00);
    if (rval != val)
      error++;
  }

  return error;
}

void camSPIReset() {
  uint8_t cmd1[2] = {0x07, 0x80};
  uint8_t cmd2[2] = {0x07, 0x00};

  camWriteReg(cmd1[0], cmd1[1]);
  HAL_Delay(100);
  camWriteReg(cmd2[0], cmd2[1]);
  HAL_Delay(100);
}

void camWriteReg(const uint8_t reg, const uint8_t val) {
  uint8_t buff[2] = {reg | 0x80, val};
  ARDUCAM_CS_LOW;
  HAL_SPI_Transmit(&SpiHandle, (uint8_t *)&buff, 2, 100);
  ARDUCAM_CS_HIGH;
  HAL_Delay(1);
}

uint8_t camTransfer(const uint8_t val) {
  uint8_t ret;
  HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t *)&val, (uint8_t *)&ret, 1, 100);

  return ret;
}

#define DummyLength 8096
#define DummyVal 0x00
uint8_t dummy[DummyLength];
static int dummyinit = 0;

uint8_t camTransfers(uint8_t *buf, const uint32_t length) {
  if (!dummyinit) {
    for (int i = 0; i < DummyLength; i++)
      dummy[i] = DummyVal;
    dummyinit = 1;
  }
  uint8_t ret;
  HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t *)dummy, (uint8_t *)buf, length,
                          100);

  return ret;
}

uint8_t camReadReg(const uint8_t reg) {
  uint8_t buff[2] = {reg, 0x00};
  uint8_t rbuff[2];
  ARDUCAM_CS_LOW;
  HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t *)&buff, (uint8_t *)&rbuff, 2,
                          100);
  ARDUCAM_CS_HIGH;

  return rbuff[1];
}

uint8_t camReadRegBit(uint8_t addr, uint8_t bit) {
  uint8_t temp;
  temp = camReadReg(addr);
  temp = temp & bit;
  return temp;
}

#define DEBUG

void camCoreDump() {
#ifdef DEBUG
  int index = 0;
  while (core[index][0] != 0xFF) {
    core[index][1] = camReadReg(core[index][0]);
    index++;
  }
#endif
}
