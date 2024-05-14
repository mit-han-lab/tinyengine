################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/NetworkInterface.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/uncached_memory.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_dma.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_hw.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_physpeed.c 

C_DEPS += \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/NetworkInterface.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/uncached_memory.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_dma.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_hw.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_physpeed.d 

OBJS += \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/NetworkInterface.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/uncached_memory.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_dma.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_hw.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_physpeed.o 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/NetworkInterface.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/NetworkInterface.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/NetworkInterface.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/uncached_memory.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/uncached_memory.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/uncached_memory.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_dma.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_dma.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_hw.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_hw.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_hw.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_physpeed.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_physpeed.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/NetworkInterface/Zynq/x_emacpsif_physpeed.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

