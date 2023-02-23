################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/TinyEngine/src/yoloOutput.c 

C_DEPS += \
./Src/TinyEngine/src/yoloOutput.d 

OBJS += \
./Src/TinyEngine/src/yoloOutput.o 


# Each subdirectory must supply rules for building sources it contributes
Src/TinyEngine/src/%.o Src/TinyEngine/src/%.su: ../Src/TinyEngine/src/%.c Src/TinyEngine/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-TinyEngine-2f-src

clean-Src-2f-TinyEngine-2f-src:
	-$(RM) ./Src/TinyEngine/src/yoloOutput.d ./Src/TinyEngine/src/yoloOutput.o ./Src/TinyEngine/src/yoloOutput.su

.PHONY: clean-Src-2f-TinyEngine-2f-src

