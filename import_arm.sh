#!/bin/bash
mkdir ./TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis
cp -r ./TinyEngine/third_party/CMSIS/CMSIS/NN/Include/*.h ./TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis
cp -r ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/dsp ./TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis
cp ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/arm_common_tables.h ./TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis
cp ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/arm_math_memory.h ./TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis
cp ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/arm_math_types.h ./TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis
cp ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/arm_math.h ./TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis
cp -r ./TinyEngine/third_party/CMSIS/CMSIS/Core/Include ./TinyEngine_vww_tutorial/Drivers/CMSIS