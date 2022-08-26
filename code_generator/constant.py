# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   constant.py
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

TTYPE_INFERNECE = "inference"

FIGURE_CONFIG = {
    "INFERENCE_COLOR": "#eab11f",  # yellow
    "FIGURE_W_INCH": 19,
    "FIGURE_H_INCH": 7,
    "DPI": 800,
    "FONT_SIZE": 32,
    "INDEX_FONT_SIZE": 24,
    "Y_STEP": 64,
    "Y_MAX": 256,
    "X_STEP": 5,
    "X_MAX": 80,
    "SHOW_INDEX": True,
}

op_name_translation = {
    "nn.conv2d": ["CONV_2D", "DEPTHWISE_CONV_2D", "GROUP_CONV"],
    "nn.mcuconv2d": ["CONV_2D", "DEPTHWISE_CONV_2D", "GROUP_CONV"],
    "nn.mcuadd": "ADD",
    "add": "ADD",
    "ResizeNearestNeighbor": "UPSAMPLE",
}

SKIP_OPs = {"QUANTIZE", "DEQUANTIZE", "RESHAPE"}  # TODO: Handle RESHAPE during codegen
