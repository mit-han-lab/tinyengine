################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPClientTask.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPServerTask.c \
../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/main.c 

C_DEPS += \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPClientTask.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPServerTask.d \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/main.d 

OBJS += \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPClientTask.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPServerTask.o \
./Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/main.o 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPClientTask.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPClientTask.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPClientTask.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPServerTask.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPServerTask.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/SecureTCPServerTask.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/main.o: ../Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS-Plus/Demo/FreeRTOS_Plus_WolfSSL_Windows_Simulator/main.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

