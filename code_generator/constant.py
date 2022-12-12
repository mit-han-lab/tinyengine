# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   constant.py
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

TTYPE_INFERNECE = "inference"

op_name_translation = {
    "nn.conv2d": ["CONV_2D", "DEPTHWISE_CONV_2D", "GROUP_CONV"],
    "nn.mcuconv2d": ["CONV_2D", "DEPTHWISE_CONV_2D", "GROUP_CONV"],
    "nn.mcuadd": "ADD",
    "add": "ADD",
    "ResizeNearestNeighbor": "UPSAMPLE",
}

SKIP_OPs = {"QUANTIZE", "DEQUANTIZE", "RESHAPE"}  # TODO: Handle RESHAPE during codegen

FUSE_TILE_STR = "FUSE_TILE"
FUSE_INT8CAST_STR = "FUSE_CAST"
FUSE_SGD_UPDATE_STR = "INPLACE_SGD"
FUSE_INT8CASTSLICE_STR = "FUSE_CAST_SLICE"
FUSE_WHERE_ZEROSSTR = "FUSE_WHERE_ZEROS"
INPLACE_WHERE_STR = "INPLACE_WHERE"
INPLACE_MUL_STR = "INPLACE_MUL"
INPLACE_DEPTHWISE_STRIDE2_STR = "INPLACE_DEPTHWISE_STRIDE2_MEM"
REORDER_STR = "REORDER_GCONV_TCONV"
FUSHION_CONFIG = {
    FUSE_TILE_STR: True,
    FUSE_INT8CAST_STR: True,
    FUSE_INT8CASTSLICE_STR: True,
    FUSE_SGD_UPDATE_STR: True,
    FUSE_WHERE_ZEROSSTR: True,
    INPLACE_WHERE_STR: True,
    INPLACE_MUL_STR: True,
    REORDER_STR: True,
    INPLACE_DEPTHWISE_STRIDE2_STR: True,
}
INFERECE_WEIGHT_SIZE = 1
INFERECE_INT8_SIZE = 1
USE_BIT_MASK = True
USE_TTE_INT8 = True
# end: for forcelly cast to fp32
TTYPE_STATIC_WEIGHT = "static_weights"
TTYPE_TRAINING_GRADIENT = "training_gradient"
TTYPE_TRAINING_ACTIVATION = "training_activation"
TTYPE_TRAINING_WEIGHTS = "training_weight"
TTYPE_INFERNECE = "inference"

# template 3
FIGURE_CONFIG = {
    "TRAIN_TENSOR_COLOR": "#68ac14",  # green
    "TRAIN_WEIGHT_COLOR": "#61a78f",  # light green:
    "TRAIN_ACTIVATION_COLOR": "#b9c8d8",  # light blue
    "TRAIN_GRADIENT_COLOR": "#35619f",  # blue
    "INFERENCE_COLOR": "#eab11f",  # yellow
    "FIGURE_W_INCH": 19,
    "FIGURE_H_INCH": 7,
    "DPI": 800,
    "Y_STEP": 96,
    "Y_MAX": 384,
    "X_STEP": 30,
    "X_MAX": 270,
    "SHOW_INDEX": False,
    "FONT_SIZE": 32,
    "INDEX_FONT_SIZE": 24,
}


op_name_translation = {
    "nn.conv2d": ["CONV_2D", "DEPTHWISE_CONV_2D", "GROUP_CONV"],
    "nn.mcuconv2d": ["CONV_2D", "DEPTHWISE_CONV_2D", "GROUP_CONV"],
    "nn.mcuadd": "ADD",
    "nn.dense": "DENSE",
    "nn.matmul": "MAT_MUL",
    "add": "ADD",
    "cast": "CAST",
    "cast_like": "CAST_LIKE",
    "divide": "DIV",
    "multiply": "MUL",
    "less": "LESS",
    "less_equal": "LESS",
    "greater": "GREATER",
    "greater_equal": "GREATER",
    "where": "WHERE",
    "exp": "EXP",
    "sum": "SUM",
    "subtract": "SUBTRACT",
    "tile": "TILE",
    "reshape": "RESHAPE",
    "reshape_like": "RESHAPE_LIKE",
    "nn.relu": "RELU",
    "strided_slice": "STRIDED_SLICE",
    "transpose": "TRANSPOSE",
    "nn.conv2d_transpose": "TRANSPOSE_CONV_2D",
    "nn.bias_add": "BIAS_ADD",
    "negative": "NEGATIVE",
    "zeros_like": "ZEROS_LIKE",
    "zeros": "ZEROS",
    "ones_like": "ONES_LIKE",
    "ones": "ONES",
    "collapse_sum_like": "COLLAPSE_SUM_LIKE",
    "nn.log_softmax": "LOG_SOFTMAX",
    "nn.cross_entropy_with_logits": "NLL_LOSS",
    "max": "MAX",
}
