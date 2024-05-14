################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osassert.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osbdev.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osclock.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osmutex.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osoutput.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostask.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostimestamp.c 

C_DEPS += \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osassert.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osbdev.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osclock.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osmutex.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osoutput.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostask.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostimestamp.d 

OBJS += \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osassert.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osbdev.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osclock.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osmutex.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osoutput.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostask.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostimestamp.o 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osassert.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osassert.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osassert.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osbdev.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osbdev.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osbdev.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osclock.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osclock.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osclock.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osmutex.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osmutex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osmutex.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osoutput.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osoutput.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/osoutput.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostask.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostask.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostask.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostimestamp.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostimestamp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/Reliance-Edge/os/freertos/services/ostimestamp.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

