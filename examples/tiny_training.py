# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   tiny_training.py
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
# Example commands:
# cd examples
# python examples/tiny_training.py -f assets/49kb-int8-graph.json -D assets/full-int8-params.pkl -QAS \
# assets/scale.json -m -g -d -FR
#

import argparse
import json

from code_generator.CodeGenerator import CodeGenerator
from code_generator.GeneralMemoryScheduler import GeneralMemoryScheduler
from code_generator.TTEParser import TTEParser


def main():
    parser = argparse.ArgumentParser(description="TinyEngine code generation")
    parser.add_argument("-f", "--model_path", required=True, type=str, help="path of the model")
    parser.add_argument("-D", "--data_path", default=None, type=str, help="path of the param pkl")
    parser.add_argument("-m", "--memory_profile", action="store_true", help="show memory scheduling details")
    parser.add_argument("-d", "--detailed_profile", action="store_true", help="show detailed memory info")
    parser.add_argument("-g", "--code_gen", action="store_true", help="code generation")
    parser.add_argument("-B", "--profile", action="store_true", help="generate profiling code for benchmarking")
    parser.add_argument("-FR", "--fp_requantize", action="store_true", help="using floating point to requantize")
    parser.add_argument("-QAS", "--scale_path", default=None, type=str, help="path of the scale params")
    parser.add_argument(
        "-MV",
        "--memory_visualization",
        default="./lifecycle.png",
        type=str,
        help="path of the gernerated figure",
    )
    args = parser.parse_args()

    target_file = args.model_path
    fp_requantize = args.fp_requantize
    memory_profile = args.memory_profile
    code_gen = args.code_gen
    profile_mode = args.profile
    mv_path = args.memory_visualization
    dummy_address = False
    tflite_op = False
    unsigned_input = False
    patch_params = None
    BIT = 8

    # Load model
    if args.scale_path is None:
        scale_params = None
    else:
        with open(args.scale_path, "r") as fp:
            scale_params = json.load(fp)

    assert args.data_path is not None, "please provide data path for the weights of the model"
    parser = TTEParser(target_file, args.data_path, scale_params=scale_params)
    parser.loadModel()
    layer = parser.layer

    # Generate code
    outTable = parser.outputTables if hasattr(parser, "outputTables") else None
    VisaulizeTrainable = False if code_gen else True
    memory_scheduler = GeneralMemoryScheduler(
        layer,
        tflite_op,
        dummy_address,
        outputTables=outTable,
        mem_visual_path=mv_path,
        VisaulizeTrainable=VisaulizeTrainable,
        sort_by_lifetime=True,
    )
    memory_scheduler.allocateMemory()

    if memory_profile:
        memory_scheduler.dumpLayerMem()
    else:
        print(f"peak SRAM: {memory_scheduler.peakmem}")

    if code_gen:
        outTable = parser.outputTables if hasattr(parser, "outputTables") else None
        code_generator = CodeGenerator(
            memory_scheduler,
            memory_scheduler.USE_INPLACE,
            BIT,
            unsigned_input,
            patch_params,
            False,
            profile_mode,
            fp_requantize,
            tflite_op,
            dummy_address,
            outputTables=outTable,
            is_training=True,
        )
        # set detection outputs before codegen if any
        code_generator.codeGeneration()


if __name__ == "__main__":
    main()
