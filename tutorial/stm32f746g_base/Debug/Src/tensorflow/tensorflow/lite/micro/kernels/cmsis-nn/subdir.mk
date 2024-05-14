################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/add.cc \
../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/conv.cc \
../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/depthwise_conv.cc \
../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/fully_connected.cc \
../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/mul.cc \
../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/pooling.cc \
../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/softmax.cc 

CC_DEPS += \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/add.d \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/conv.d \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/depthwise_conv.d \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/fully_connected.d \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/mul.d \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/pooling.d \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/softmax.d 

OBJS += \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/add.o \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/conv.o \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/depthwise_conv.o \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/fully_connected.o \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/mul.o \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/pooling.o \
./Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/softmax.o 


# Each subdirectory must supply rules for building sources it contributes
Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/add.o: ../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/add.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/add.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/conv.o: ../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/conv.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/conv.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/depthwise_conv.o: ../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/depthwise_conv.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/depthwise_conv.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/fully_connected.o: ../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/fully_connected.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/fully_connected.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/mul.o: ../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/mul.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/mul.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/pooling.o: ../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/pooling.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/pooling.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/softmax.o: ../Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/softmax.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/kernels/cmsis-nn/softmax.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

