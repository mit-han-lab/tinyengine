# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   vww_to_c.py
#
# Reference papers:
#  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
#  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
#  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
# Contact authors:
#  - Wei-Ming Chen, wmchen@mit.edu
#  - Wei-Chen Wang, wweichen@mit.edu
#  - Ji Lin, jilin@mit.edu
#  - Ligeng Zhu, ligeng@mit.edu
#  - Song Han, songhan@mit.edu
#
# Target ISA:  ARMv7E-M
# ----------------------------------------------------------------------
import sys
print("PYTHONPATH:", sys.path)

import sys
import os

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../code_generator')))


from code_generator.CodegenUtilTFlite import GenerateSourceFilesFromTFlite
# from mcunet.mcunet.model_zoo import download_tflite

# 1: Let's first build our pretrained VWW model
# 2: To deploy the model on MCU, we need to first convert the model to an Intermediate Representation (IR) and
# get the weight parameters and scale parameters.
# tflite_path = download_tflite(net_id="mcunet-vww1")
tflite_path = "D:\\workspace\\tinyengine\\examples\\deit_tiny_patch16_224_int8.tflite"

# 3. Let's generate source code for on-device deployment
peakmem = GenerateSourceFilesFromTFlite(
    tflite_path,
    life_cycle_path="./lifecycle.png",
)
print(f"Peak memory: {peakmem} bytes")
