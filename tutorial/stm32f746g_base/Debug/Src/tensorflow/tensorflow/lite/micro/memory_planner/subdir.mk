################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc \
../Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner_test.cc \
../Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner.cc \
../Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner_test.cc 

CC_DEPS += \
./Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner.d \
./Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner_test.d \
./Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner.d \
./Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner_test.d 

OBJS += \
./Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner.o \
./Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner_test.o \
./Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner.o \
./Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner_test.o 


# Each subdirectory must supply rules for building sources it contributes
Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner.o: ../Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner_test.o: ../Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner_test.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner_test.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner.o: ../Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner_test.o: ../Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner_test.cc
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner_test.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

