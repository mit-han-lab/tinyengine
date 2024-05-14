################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Src/ViT_00/config/startup/startup_stm32f722xx.s \
../Src/ViT_00/config/startup/startup_stm32f723xx.s \
../Src/ViT_00/config/startup/startup_stm32f730xx.s \
../Src/ViT_00/config/startup/startup_stm32f732xx.s \
../Src/ViT_00/config/startup/startup_stm32f733xx.s \
../Src/ViT_00/config/startup/startup_stm32f745xx.s \
../Src/ViT_00/config/startup/startup_stm32f746xx.s \
../Src/ViT_00/config/startup/startup_stm32f750xx.s \
../Src/ViT_00/config/startup/startup_stm32f756xx.s \
../Src/ViT_00/config/startup/startup_stm32f765xx.s \
../Src/ViT_00/config/startup/startup_stm32f767xx.s \
../Src/ViT_00/config/startup/startup_stm32f769xx.s \
../Src/ViT_00/config/startup/startup_stm32f777xx.s \
../Src/ViT_00/config/startup/startup_stm32f779xx.s 

S_DEPS += \
./Src/ViT_00/config/startup/startup_stm32f722xx.d \
./Src/ViT_00/config/startup/startup_stm32f723xx.d \
./Src/ViT_00/config/startup/startup_stm32f730xx.d \
./Src/ViT_00/config/startup/startup_stm32f732xx.d \
./Src/ViT_00/config/startup/startup_stm32f733xx.d \
./Src/ViT_00/config/startup/startup_stm32f745xx.d \
./Src/ViT_00/config/startup/startup_stm32f746xx.d \
./Src/ViT_00/config/startup/startup_stm32f750xx.d \
./Src/ViT_00/config/startup/startup_stm32f756xx.d \
./Src/ViT_00/config/startup/startup_stm32f765xx.d \
./Src/ViT_00/config/startup/startup_stm32f767xx.d \
./Src/ViT_00/config/startup/startup_stm32f769xx.d \
./Src/ViT_00/config/startup/startup_stm32f777xx.d \
./Src/ViT_00/config/startup/startup_stm32f779xx.d 

OBJS += \
./Src/ViT_00/config/startup/startup_stm32f722xx.o \
./Src/ViT_00/config/startup/startup_stm32f723xx.o \
./Src/ViT_00/config/startup/startup_stm32f730xx.o \
./Src/ViT_00/config/startup/startup_stm32f732xx.o \
./Src/ViT_00/config/startup/startup_stm32f733xx.o \
./Src/ViT_00/config/startup/startup_stm32f745xx.o \
./Src/ViT_00/config/startup/startup_stm32f746xx.o \
./Src/ViT_00/config/startup/startup_stm32f750xx.o \
./Src/ViT_00/config/startup/startup_stm32f756xx.o \
./Src/ViT_00/config/startup/startup_stm32f765xx.o \
./Src/ViT_00/config/startup/startup_stm32f767xx.o \
./Src/ViT_00/config/startup/startup_stm32f769xx.o \
./Src/ViT_00/config/startup/startup_stm32f777xx.o \
./Src/ViT_00/config/startup/startup_stm32f779xx.o 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/config/startup/startup_stm32f722xx.o: ../Src/ViT_00/config/startup/startup_stm32f722xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f722xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f723xx.o: ../Src/ViT_00/config/startup/startup_stm32f723xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f723xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f730xx.o: ../Src/ViT_00/config/startup/startup_stm32f730xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f730xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f732xx.o: ../Src/ViT_00/config/startup/startup_stm32f732xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f732xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f733xx.o: ../Src/ViT_00/config/startup/startup_stm32f733xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f733xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f745xx.o: ../Src/ViT_00/config/startup/startup_stm32f745xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f745xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f746xx.o: ../Src/ViT_00/config/startup/startup_stm32f746xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f746xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f750xx.o: ../Src/ViT_00/config/startup/startup_stm32f750xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f750xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f756xx.o: ../Src/ViT_00/config/startup/startup_stm32f756xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f756xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f765xx.o: ../Src/ViT_00/config/startup/startup_stm32f765xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f765xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f767xx.o: ../Src/ViT_00/config/startup/startup_stm32f767xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f767xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f769xx.o: ../Src/ViT_00/config/startup/startup_stm32f769xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f769xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f777xx.o: ../Src/ViT_00/config/startup/startup_stm32f777xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f777xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/config/startup/startup_stm32f779xx.o: ../Src/ViT_00/config/startup/startup_stm32f779xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/config/startup/startup_stm32f779xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

