################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/TinyEngine/src/kernels/fp_requantize_op/add_fpreq.c \
../Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch16_fpreq.c \
../Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch24_fpreq.c \
../Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch48_fpreq.c \
../Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch8_fpreq.c \
../Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq.c \
../Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask.c \
../Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask_partialCH.c \
../Src/TinyEngine/src/kernels/fp_requantize_op/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.c \
../Src/TinyEngine/src/kernels/fp_requantize_op/mat_mul_kernels_fpreq.c 

C_DEPS += \
./Src/TinyEngine/src/kernels/fp_requantize_op/add_fpreq.d \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch16_fpreq.d \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch24_fpreq.d \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch48_fpreq.d \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch8_fpreq.d \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq.d \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask.d \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask_partialCH.d \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.d \
./Src/TinyEngine/src/kernels/fp_requantize_op/mat_mul_kernels_fpreq.d 

OBJS += \
./Src/TinyEngine/src/kernels/fp_requantize_op/add_fpreq.o \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch16_fpreq.o \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch24_fpreq.o \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch48_fpreq.o \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch8_fpreq.o \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq.o \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask.o \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask_partialCH.o \
./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.o \
./Src/TinyEngine/src/kernels/fp_requantize_op/mat_mul_kernels_fpreq.o 


# Each subdirectory must supply rules for building sources it contributes
Src/TinyEngine/src/kernels/fp_requantize_op/%.o Src/TinyEngine/src/kernels/fp_requantize_op/%.su: ../Src/TinyEngine/src/kernels/fp_requantize_op/%.c Src/TinyEngine/src/kernels/fp_requantize_op/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"/Users/raymondwang/STM32CubeIDE/workspace_1.9.0/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-TinyEngine-2f-src-2f-kernels-2f-fp_requantize_op

clean-Src-2f-TinyEngine-2f-src-2f-kernels-2f-fp_requantize_op:
	-$(RM) ./Src/TinyEngine/src/kernels/fp_requantize_op/add_fpreq.d ./Src/TinyEngine/src/kernels/fp_requantize_op/add_fpreq.o ./Src/TinyEngine/src/kernels/fp_requantize_op/add_fpreq.su ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch16_fpreq.d ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch16_fpreq.o ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch16_fpreq.su ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch24_fpreq.d ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch24_fpreq.o ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch24_fpreq.su ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch48_fpreq.d ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch48_fpreq.o ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch48_fpreq.su ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch8_fpreq.d ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch8_fpreq.o ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_ch8_fpreq.su ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq.d ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq.o ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq.su ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask.d ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask.o ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask.su ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask_partialCH.d ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask_partialCH.o ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_1x1_s8_fpreq_mask_partialCH.su ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.d ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.o ./Src/TinyEngine/src/kernels/fp_requantize_op/convolve_s8_kernel3_inputch3_stride2_pad1_fpreq.su ./Src/TinyEngine/src/kernels/fp_requantize_op/mat_mul_kernels_fpreq.d ./Src/TinyEngine/src/kernels/fp_requantize_op/mat_mul_kernels_fpreq.o ./Src/TinyEngine/src/kernels/fp_requantize_op/mat_mul_kernels_fpreq.su

.PHONY: clean-Src-2f-TinyEngine-2f-src-2f-kernels-2f-fp_requantize_op

