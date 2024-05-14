################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/crl.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/internal.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/io.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/keys.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ocsp.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/sniffer.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ssl.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/tls.c 

C_DEPS += \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/crl.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/internal.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/io.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/keys.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ocsp.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/sniffer.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ssl.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/tls.d 

OBJS += \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/crl.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/internal.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/io.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/keys.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ocsp.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/sniffer.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ssl.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/tls.o 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/crl.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/crl.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/crl.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/internal.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/internal.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/internal.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/io.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/io.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/io.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/keys.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/keys.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/keys.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ocsp.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ocsp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ocsp.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/sniffer.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/sniffer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/sniffer.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ssl.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ssl.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/ssl.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/tls.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/tls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Source/WolfSSL/src/tls.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

