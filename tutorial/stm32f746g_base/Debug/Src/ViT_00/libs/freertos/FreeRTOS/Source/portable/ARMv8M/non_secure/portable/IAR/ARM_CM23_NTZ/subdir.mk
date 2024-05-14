################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ARMv8M/non_secure/portable/IAR/ARM_CM23_NTZ/portasm.s 

S_DEPS += \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ARMv8M/non_secure/portable/IAR/ARM_CM23_NTZ/portasm.d 

OBJS += \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ARMv8M/non_secure/portable/IAR/ARM_CM23_NTZ/portasm.o 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ARMv8M/non_secure/portable/IAR/ARM_CM23_NTZ/portasm.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ARMv8M/non_secure/portable/IAR/ARM_CM23_NTZ/portasm.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ARMv8M/non_secure/portable/IAR/ARM_CM23_NTZ/portasm.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

