################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/port.c \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portclib.c \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_init.c \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr.c \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_overlay_os_hook.c 

S_UPPER_SRCS += \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portasm.S \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_context.S \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr_asm.S \
../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_vectors.S 

C_DEPS += \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/port.d \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portclib.d \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_init.d \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr.d \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_overlay_os_hook.d 

OBJS += \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/port.o \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portasm.o \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portclib.o \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_context.o \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_init.o \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr.o \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr_asm.o \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_overlay_os_hook.o \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_vectors.o 

S_UPPER_DEPS += \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portasm.d \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_context.d \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr_asm.d \
./Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_vectors.d 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/port.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/port.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/port.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portasm.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portasm.S
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portasm.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portclib.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portclib.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/portclib.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_context.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_context.S
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_context.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_init.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_init.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_init.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr_asm.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr_asm.S
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_intr_asm.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_overlay_os_hook.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_overlay_os_hook.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src/tensorflow/tensorflow/lite/kernels/internal/reference" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Inc" -I"C:/Users/sm5py/STM32CubeIDE/workspace_1.5.0/stm32f746g_base/Src" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_overlay_os_hook.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_vectors.o: ../Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_vectors.S
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/ViT_00/libs/freertos/FreeRTOS/Source/portable/ThirdParty/XCC/Xtensa/xtensa_vectors.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

