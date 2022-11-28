################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/sys/_io.c \
../Src/sys/stubs.c 

C_DEPS += \
./Src/sys/_io.d \
./Src/sys/stubs.d 

OBJS += \
./Src/sys/_io.o \
./Src/sys/stubs.o 


# Each subdirectory must supply rules for building sources it contributes
Src/sys/%.o Src/sys/%.su: ../Src/sys/%.c Src/sys/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-sys

clean-Src-2f-sys:
	-$(RM) ./Src/sys/_io.d ./Src/sys/_io.o ./Src/sys/_io.su ./Src/sys/stubs.d ./Src/sys/stubs.o ./Src/sys/stubs.su

.PHONY: clean-Src-2f-sys

