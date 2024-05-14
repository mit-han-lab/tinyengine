################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f32.c \
../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f32.c 

C_DEPS += \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f32.d \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f32.d 

OBJS += \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f32.o \
./Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f32.o 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f32.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f32.o: ../Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

