################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.c \
../Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.c \
../Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.c \
../Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.c 

C_DEPS += \
./Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.d \
./Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.d \
./Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.d \
./Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.d 

OBJS += \
./Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.o \
./Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.o \
./Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.o \
./Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.o 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.o: ../Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.o: ../Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.o: ../Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.o: ../Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

