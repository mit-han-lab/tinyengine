# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   mcunetv2_se_large.py
#
# Reference papers:
#  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
#  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
#  - MCUNetV3: On-Device Training Under 256KB Memory, arXiv:2206.15472
# Contact authors:
#  - Wei-Ming Chen, wmchen@mit.edu
#  - Wei-Chen Wang, wweichen@mit.edu
#  - Ji Lin, jilin@mit.edu
#  - Ligeng Zhu, ligeng@mit.edu
#  - Song Han, songhan@mit.edu
#
# Target ISA:  ARMv7E-M
# ----------------------------------------------------------------------

import os
from tempfile import TemporaryDirectory

from code_generator.CodeGenerator import CodeGenerator
from code_generator.GeneralMemoryScheduler import GeneralMemoryScheduler
from code_generator.InputResizer import PatchResizer
from code_generator.PatchBasedUtil import getPatchParams
from code_generator.TfliteConvertor import TfliteConvertor

# 1: The model with SE blocks for testing purpose
tflite_path = "/home/wmchen/workspace/mcunet/tinyengine/assets/mcunetv2_large.tflite"
life_cycle_path = "./lifecycle.png"
use_inplace = True
n_patches = 4
split_index = 8

with TemporaryDirectory() as WORKING_DIR:
    if life_cycle_path is None:
        schedule_image_path = os.path.join(WORKING_DIR, "schedule.png")
    else:
        schedule_image_path = life_cycle_path

    tf_convertor = TfliteConvertor(tflite_path)
    tf_convertor.parseOperatorInfo()
    layer = tf_convertor.layer
    outTable = []
    VisaulizeTrainable = False  # disable for code gen
    # Patch-based
    if n_patches is not None:
        patch_params = getPatchParams(layer, split_index, n_patches)
        P_resizer = PatchResizer(layer)
        P_resizer.patchResize(patch_params["layer_cnt"], patch_params["grain_rf"], patch_params["grain_rf_height"])

    memory_scheduler = GeneralMemoryScheduler(
        layer,
        False,
        False,
        outputTables=outTable,
        inplace=use_inplace,
        mem_visual_path=schedule_image_path,
        VisaulizeTrainable=VisaulizeTrainable,
    )
    memory_scheduler.USE_INPLACE = use_inplace
    memory_scheduler.allocateMemory()
    memory_scheduler.dumpLayerMem()

    code_generator = CodeGenerator(
        memsche=memory_scheduler,
        inplace=memory_scheduler.USE_INPLACE,
        unsigned_input=False,
        patch_params=patch_params,
        FP_output=False,
        profile_mode=False,
        fp_requantize=True,
        tflite_op=False,
        dummy_address=False,
        outputTables=[],
    )
    # set detection outputs before codegen if any
    code_generator.codeGeneration()

    peakmem = memory_scheduler.buffers["input_output"]


print(f"Peak memory: {peakmem} bytes")
