# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   vww_patchbased.py
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

import os
import argparse
from tempfile import TemporaryDirectory

from code_generator.CodeGenerator import CodeGenerator
from code_generator.GeneralMemoryScheduler import GeneralMemoryScheduler, AllocatorType, InplaceMode
from code_generator.InputResizer import PatchResizer
from code_generator.PatchBasedUtil import getPatchParams
from code_generator.TfliteConvertor import TfliteConvertor
from mcunet.mcunet.model_zoo import download_tflite

parser = argparse.ArgumentParser("vww_patchbased")
parser.add_argument(
    "-f", "--life_cycle_file", type=str, default="./lifecycle.png",
    help="the location to save the tensor lifecycle visualization"
)
parser.add_argument(
    "-a", "--allocator_type", type=str, default="first_fit", choices=[t.name for t in AllocatorType],
    help="selects the allocator type to use"
)
parser.add_argument(
    "-m", "--inplace_mode", default="depthwise_only", choices=[mode.name for mode in InplaceMode],
    help="selects the inplace mode",
)
parser.add_argument(
    "-p", "--n_patches", type=int, default=2,
    help="patches per side, so 2 runs the patch stage on a 2x2 grid"
)
parser.add_argument(
    "-s", "--split_index", type=int, default=5,
    help="the layer the patch stage ends before"
)
args = parser.parse_args()

# 1: Let's first build our pretrained VWW model
# 2: To deploy the model on MCU, we need to first convert the model to an Intermediate Representation (IR) and
# get the weight parameters and scale parameters.
tflite_path = download_tflite(net_id="mcunet-vww1")
life_cycle_path = args.life_cycle_file
# 3. Set up patchbased parameters
use_inplace = True
n_patches = args.n_patches  # n_patches x n_patches patches
split_index = args.split_index  # split at the split_index-th layer
# 4. Let's generate source code for on-device deployment

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
        allocator_type=AllocatorType(args.allocator_type),
        inplace_mode=InplaceMode(args.inplace_mode),
    )
    memory_scheduler.USE_INPLACE = use_inplace
    memory_scheduler.allocateMemory()
    memory_scheduler.dumpLayerMem()

    outTable = tf_convertor.outputTables if hasattr(tf_convertor, "outputTables") else []  # type: ignore
    code_generator = CodeGenerator(
        memsche=memory_scheduler,
        inplace=memory_scheduler.USE_INPLACE,
        unsigned_input=False,
        patch_params=patch_params,
        FP_output=False,
        profile_mode=False,
        fp_requantize=False,
        tflite_op=False,
        dummy_address=False,
        outputTables=outTable,
    )
    # set detection outputs before codegen if any
    code_generator.codeGeneration()

    peakmem = memory_scheduler.buffers["input_output"]


print(f"Peak memory: {peakmem} bytes")
