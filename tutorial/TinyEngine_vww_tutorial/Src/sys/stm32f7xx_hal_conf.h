/**
  ******************************************************************************
  * @file    stm32f7xx_hal_conf.h
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   stm32f7xx_hal_conf.h
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

#ifndef __STM32F7xx_HAL_CONF_H
#define __STM32F7xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif
#define HAL_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_CAN_MODULE_ENABLED
#define HAL_CEC_MODULE_ENABLED
#define HAL_CRC_MODULE_ENABLED
#define HAL_CRYP_MODULE_ENABLED
#define HAL_DAC_MODULE_ENABLED
#define HAL_DCMI_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_DMA2D_MODULE_ENABLED
#define HAL_ETH_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_NAND_MODULE_ENABLED
#define HAL_NOR_MODULE_ENABLED
#define HAL_SRAM_MODULE_ENABLED
#define HAL_SDRAM_MODULE_ENABLED
#define HAL_HASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_I2S_MODULE_ENABLED
#define HAL_IWDG_MODULE_ENABLED
#define HAL_LPTIM_MODULE_ENABLED
#define HAL_LTDC_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_QSPI_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_RNG_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
#define HAL_SAI_MODULE_ENABLED
#define HAL_SD_MODULE_ENABLED
#define HAL_SPDIFRX_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_USART_MODULE_ENABLED
#define HAL_IRDA_MODULE_ENABLED
#define HAL_SMARTCARD_MODULE_ENABLED
#define HAL_WWDG_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_PCD_MODULE_ENABLED
#define HAL_HCD_MODULE_ENABLED

#define HAL_ACCURATE_TIMEOUT_ENABLED 0
#define HAL_TIMEOUT_VALUE 0x1FFFFFF

#if !defined(HSE_VALUE)
#define HSE_VALUE ((uint32_t)25000000)
#endif

#if !defined(HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT ((uint32_t)5000)
#endif

#if !defined(HSI_VALUE)
#define HSI_VALUE ((uint32_t)16000000)
#endif

#if !defined(LSI_VALUE)
#define LSI_VALUE ((uint32_t)32000)
#endif

#if !defined(LSE_VALUE)
#define LSE_VALUE ((uint32_t)32768)
#endif

#if !defined(EXTERNAL_CLOCK_VALUE)
#define EXTERNAL_CLOCK_VALUE ((uint32_t)12288000)
#endif
#define VDD_VALUE ((uint32_t)3300)
#define TICK_INT_PRIORITY ((uint32_t)0x0F)
#define USE_RTOS 0
#define PREFETCH_ENABLE 1
#define ART_ACCLERATOR_ENABLE 1
#define INSTRUCTION_CACHE_ENABLE 1
#define DATA_CACHE_ENABLE 1
#define MAC_ADDR0 2
#define MAC_ADDR1 1
#define MAC_ADDR2 0
#define MAC_ADDR3 0
#define MAC_ADDR4 0
#define MAC_ADDR5 0

#define ETH_RX_BUF_SIZE ETH_MAX_PACKET_SIZE
#define ETH_TX_BUF_SIZE ETH_MAX_PACKET_SIZE
#define ETH_RXBUFNB ((uint32_t)5)
#define ETH_TXBUFNB ((uint32_t)5)

#define DP83848_PHY_ADDRESS 0x01

#define LAN8742A_PHY_ADDRESS 0x00

#define PHY_RESET_DELAY ((uint32_t)0x00000FFF)

#define PHY_CONFIG_DELAY ((uint32_t)0x0000FFFF)

#define PHY_READ_TO ((uint32_t)0x0000FFFF)
#define PHY_WRITE_TO ((uint32_t)0x0000FFFF)

#define PHY_BCR ((uint16_t)0x00)
#define PHY_BSR ((uint16_t)0x01)

#define PHY_RESET ((uint16_t)0x8000)
#define PHY_LOOPBACK ((uint16_t)0x4000)
#define PHY_FULLDUPLEX_100M ((uint16_t)0x2100)
#define PHY_HALFDUPLEX_100M ((uint16_t)0x2000)
#define PHY_FULLDUPLEX_10M ((uint16_t)0x0100)
#define PHY_HALFDUPLEX_10M ((uint16_t)0x0000)
#define PHY_AUTONEGOTIATION ((uint16_t)0x1000)
#define PHY_RESTART_AUTONEGOTIATION ((uint16_t)0x0200)
#define PHY_POWERDOWN ((uint16_t)0x0800)
#define PHY_ISOLATE ((uint16_t)0x0400)

#define PHY_AUTONEGO_COMPLETE ((uint16_t)0x0020)
#define PHY_LINKED_STATUS ((uint16_t)0x0004)
#define PHY_JABBER_DETECTION ((uint16_t)0x0002)

#define PHY_SR ((uint16_t)0x10)
#define PHY_MICR ((uint16_t)0x11)
#define PHY_MISR ((uint16_t)0x12)

#define PHY_LINK_STATUS ((uint16_t)0x0001)
#define PHY_SPEED_STATUS ((uint16_t)0x0002)
#define PHY_DUPLEX_STATUS ((uint16_t)0x0004)

#define PHY_MICR_INT_EN ((uint16_t)0x0002)
#define PHY_MICR_INT_OE ((uint16_t)0x0001)

#define PHY_MISR_LINK_INT_EN ((uint16_t)0x0020)
#define PHY_LINK_INTERRUPT ((uint16_t)0x2000)

#ifdef HAL_RCC_MODULE_ENABLED
#include "stm32f7xx_hal_rcc.h"
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
#include "stm32f7xx_hal_gpio.h"
#endif

#ifdef HAL_DMA_MODULE_ENABLED
#include "stm32f7xx_hal_dma.h"
#endif

#ifdef HAL_CORTEX_MODULE_ENABLED
#include "stm32f7xx_hal_cortex.h"
#endif

#ifdef HAL_ADC_MODULE_ENABLED
#include "stm32f7xx_hal_adc.h"
#endif

#ifdef HAL_CAN_MODULE_ENABLED
#include "stm32f7xx_hal_can.h"
#endif

#ifdef HAL_CEC_MODULE_ENABLED
#include "stm32f7xx_hal_cec.h"
#endif

#ifdef HAL_CRC_MODULE_ENABLED
#include "stm32f7xx_hal_crc.h"
#endif

#ifdef HAL_CRYP_MODULE_ENABLED
#include "stm32f7xx_hal_cryp.h"
#endif

#ifdef HAL_DMA2D_MODULE_ENABLED
#include "stm32f7xx_hal_dma2d.h"
#endif

#ifdef HAL_DAC_MODULE_ENABLED
#include "stm32f7xx_hal_dac.h"
#endif

#ifdef HAL_DCMI_MODULE_ENABLED
#include "stm32f7xx_hal_dcmi.h"
#endif

#ifdef HAL_ETH_MODULE_ENABLED
#include "stm32f7xx_hal_eth.h"
#endif

#ifdef HAL_FLASH_MODULE_ENABLED
#include "stm32f7xx_hal_flash.h"
#endif

#ifdef HAL_SRAM_MODULE_ENABLED
#include "stm32f7xx_hal_sram.h"
#endif

#ifdef HAL_NOR_MODULE_ENABLED
#include "stm32f7xx_hal_nor.h"
#endif

#ifdef HAL_NAND_MODULE_ENABLED
#include "stm32f7xx_hal_nand.h"
#endif

#ifdef HAL_SDRAM_MODULE_ENABLED
#include "stm32f7xx_hal_sdram.h"
#endif

#ifdef HAL_HASH_MODULE_ENABLED
#include "stm32f7xx_hal_hash.h"
#endif

#ifdef HAL_I2C_MODULE_ENABLED
#include "stm32f7xx_hal_i2c.h"
#endif

#ifdef HAL_I2S_MODULE_ENABLED
#include "stm32f7xx_hal_i2s.h"
#endif

#ifdef HAL_IWDG_MODULE_ENABLED
#include "stm32f7xx_hal_iwdg.h"
#endif

#ifdef HAL_LPTIM_MODULE_ENABLED
#include "stm32f7xx_hal_lptim.h"
#endif

#ifdef HAL_LTDC_MODULE_ENABLED
#include "stm32f7xx_hal_ltdc.h"
#endif

#ifdef HAL_PWR_MODULE_ENABLED
#include "stm32f7xx_hal_pwr.h"
#endif

#ifdef HAL_QSPI_MODULE_ENABLED
#include "stm32f7xx_hal_qspi.h"
#endif

#ifdef HAL_RNG_MODULE_ENABLED
#include "stm32f7xx_hal_rng.h"
#endif

#ifdef HAL_RTC_MODULE_ENABLED
#include "stm32f7xx_hal_rtc.h"
#endif

#ifdef HAL_SAI_MODULE_ENABLED
#include "stm32f7xx_hal_sai.h"
#endif

#ifdef HAL_SD_MODULE_ENABLED
#include "stm32f7xx_hal_sd.h"
#endif

#ifdef HAL_SPDIFRX_MODULE_ENABLED
#include "stm32f7xx_hal_spdifrx.h"
#endif

#ifdef HAL_SPI_MODULE_ENABLED
#include "stm32f7xx_hal_spi.h"
#endif

#ifdef HAL_TIM_MODULE_ENABLED
#include "stm32f7xx_hal_tim.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
#include "stm32f7xx_hal_uart.h"
#endif

#ifdef HAL_USART_MODULE_ENABLED
#include "stm32f7xx_hal_usart.h"
#endif

#ifdef HAL_IRDA_MODULE_ENABLED
#include "stm32f7xx_hal_irda.h"
#endif

#ifdef HAL_SMARTCARD_MODULE_ENABLED
#include "stm32f7xx_hal_smartcard.h"
#endif

#ifdef HAL_WWDG_MODULE_ENABLED
#include "stm32f7xx_hal_wwdg.h"
#endif

#ifdef HAL_PCD_MODULE_ENABLED
#include "stm32f7xx_hal_pcd.h"
#endif

#ifdef HAL_HCD_MODULE_ENABLED
#include "stm32f7xx_hal_hcd.h"
#endif

#ifdef USE_FULL_ASSERT
#define assert_param(expr)                                                     \
  ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif
