################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/GAP9MHSA/src/MHSA.c \
../Src/GAP9MHSA/src/dory.c \
../Src/GAP9MHSA/src/linearO_4x2_H.c \
../Src/GAP9MHSA/src/linearQK_4x2_H.c \
../Src/GAP9MHSA/src/linearV_4x2_H.c \
../Src/GAP9MHSA/src/matmulSoftmax_4x2_S.c \
../Src/GAP9MHSA/src/matmul_4x2_S.c 

CPP_SRCS += \
../Src/GAP9MHSA/src/iSoftmax.cpp 

C_DEPS += \
./Src/GAP9MHSA/src/MHSA.d \
./Src/GAP9MHSA/src/dory.d \
./Src/GAP9MHSA/src/linearO_4x2_H.d \
./Src/GAP9MHSA/src/linearQK_4x2_H.d \
./Src/GAP9MHSA/src/linearV_4x2_H.d \
./Src/GAP9MHSA/src/matmulSoftmax_4x2_S.d \
./Src/GAP9MHSA/src/matmul_4x2_S.d 

OBJS += \
./Src/GAP9MHSA/src/MHSA.o \
./Src/GAP9MHSA/src/dory.o \
./Src/GAP9MHSA/src/iSoftmax.o \
./Src/GAP9MHSA/src/linearO_4x2_H.o \
./Src/GAP9MHSA/src/linearQK_4x2_H.o \
./Src/GAP9MHSA/src/linearV_4x2_H.o \
./Src/GAP9MHSA/src/matmulSoftmax_4x2_S.o \
./Src/GAP9MHSA/src/matmul_4x2_S.o 

CPP_DEPS += \
./Src/GAP9MHSA/src/iSoftmax.d 


# Each subdirectory must supply rules for building sources it contributes
Src/GAP9MHSA/src/MHSA.o: ../Src/GAP9MHSA/src/MHSA.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/GAP9MHSA/src/MHSA.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/GAP9MHSA/src/dory.o: ../Src/GAP9MHSA/src/dory.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/GAP9MHSA/src/dory.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/GAP9MHSA/src/iSoftmax.o: ../Src/GAP9MHSA/src/iSoftmax.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I../Src -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/GAP9MHSA/src/iSoftmax.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/GAP9MHSA/src/linearO_4x2_H.o: ../Src/GAP9MHSA/src/linearO_4x2_H.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/GAP9MHSA/src/linearO_4x2_H.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/GAP9MHSA/src/linearQK_4x2_H.o: ../Src/GAP9MHSA/src/linearQK_4x2_H.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/GAP9MHSA/src/linearQK_4x2_H.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/GAP9MHSA/src/linearV_4x2_H.o: ../Src/GAP9MHSA/src/linearV_4x2_H.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/GAP9MHSA/src/linearV_4x2_H.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/GAP9MHSA/src/matmulSoftmax_4x2_S.o: ../Src/GAP9MHSA/src/matmulSoftmax_4x2_S.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/GAP9MHSA/src/matmulSoftmax_4x2_S.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/GAP9MHSA/src/matmul_4x2_S.o: ../Src/GAP9MHSA/src/matmul_4x2_S.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"C:/workspaces/tinyengine/tutorial/TinyEngine_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/GAP9MHSA/src/matmul_4x2_S.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

