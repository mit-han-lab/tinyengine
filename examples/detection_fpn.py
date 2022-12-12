# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   detection_fpn.py
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

from code_generator.CodeGenerator import CodeGenerator
from code_generator.detection_utils import detectionUtils
from code_generator.GeneralMemoryScheduler import GeneralMemoryScheduler
from code_generator.InputResizer import InputResizer
from code_generator.TfliteConvertor import TfliteConvertor

tflite_path = "./assets/detection.tflite"
life_cycle_path = "./lifecycle.png"
detection_conf = "./assets/detection_config.json"
h = 128
w = 160

use_inplace = True
schedule_image_path = life_cycle_path

tf_convertor = TfliteConvertor(tflite_path)
tf_convertor.parseOperatorInfo()
layer = tf_convertor.layer
resizer = InputResizer(layer)
resizer.inputResize(int(h), int(w))
outTable = []
VisaulizeTrainable = False  # disable for code gen
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
memory_scheduler.dumpLayerIndex()

detection = detectionUtils(layer, detection_conf)

code_generator = CodeGenerator(
    memsche=memory_scheduler,
    inplace=memory_scheduler.USE_INPLACE,
    unsigned_input=False,
    patch_params=None,
    FP_output=False,
    profile_mode=False,
    fp_requantize=True,
    tflite_op=False,
    dummy_address=False,
    outputTables=outTable,
    detectionUtils=detection,
)
# set detection outputs before codegen if any
code_generator.codeGeneration()

print(memory_scheduler.buffers["input_output"])
