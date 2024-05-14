################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/ViT_00/vit.cpp 

OBJS += \
./Src/ViT_00/vit.o 

CPP_DEPS += \
./Src/ViT_00/vit.d 


# Each subdirectory must supply rules for building sources it contributes
Src/ViT_00/vit.o: ../Src/ViT_00/vit.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I../Src -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/ViT_00/vit.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

