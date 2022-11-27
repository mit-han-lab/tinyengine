################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c \
../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.c 

C_DEPS += \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.d \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.d 

OBJS += \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.o \
./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F7xx_HAL_Driver/Src/%.o Drivers/STM32F7xx_HAL_Driver/Src/%.su: ../Drivers/STM32F7xx_HAL_Driver/Src/%.c Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32F7xx_HAL_Driver-2f-Src

clean-Drivers-2f-STM32F7xx_HAL_Driver-2f-Src:
	-$(RM) ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.su ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.d ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.o ./Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.su

.PHONY: clean-Drivers-2f-STM32F7xx_HAL_Driver-2f-Src

