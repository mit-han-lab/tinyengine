################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/port.c 

S_UPPER_SRCS += \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/portASM.S 

C_DEPS += \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/port.d 

OBJS += \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/port.o \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/portASM.o 

S_UPPER_DEPS += \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/portASM.d 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/port.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/port.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/port.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/portASM.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/portASM.S
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/GCC/ARM_CRx_No_GIC/portASM.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

