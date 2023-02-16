import math

import numpy as np

from code_generator.operators import conv2d, depthwiseConv2d
from code_generator.tflite import Model
from code_generator.tflite.BuiltinOptions import BuiltinOptions
from code_generator.tflite.Conv2DOptions import Conv2DOptions
from code_generator.tflite.DepthwiseConv2DOptions import DepthwiseConv2DOptions

from .utils import (
    get_input_tensors,
    get_np_from_wrapper,
    get_output_tensors,
    getMultiplierShift,
    getOpCodeStr,
    getTensorTypeStr,
)


def parse_conv2d(op, model: Model.Model, tmpPADIndice=None):
    # operator
    op_code_str = getOpCodeStr(op, model)

    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    input_tensor_count = len(input_tensors)
    assert input_tensor_count >= 2, "input tensors length should be >= 2"

    input_tensor = input_tensors[0]
    weight_tensor = input_tensors[1]

    output_tensors = get_output_tensors(op, model)
    assert len(output_tensors) == 1, "output tensors length should be 1"
    output_tensor = output_tensors[0]

    # conv_2d options
    if op_code_str == "CONV_2D":
        assert op.BuiltinOptionsType() == BuiltinOptions.Conv2DOptions
        op_options = op.BuiltinOptions()
        conv_options = Conv2DOptions()
        conv_options.Init(op_options.Bytes, op_options.Pos)
    if op_code_str == "DEPTHWISE_CONV_2D":
        assert op.BuiltinOptionsType() == BuiltinOptions.DepthwiseConv2DOptions
        op_options = op.BuiltinOptions()
        conv_options = DepthwiseConv2DOptions()
        conv_options.Init(op_options.Bytes, op_options.Pos)

    # conv parameters
    stride_h = conv_options.StrideH()
    stride_w = conv_options.StrideW()

    # shapes
    _, input_h, input_w, input_c = input_tensor.tensor.ShapeAsNumpy()
    if op_code_str == "CONV_2D":
        output_c, kernel_h, kernel_w, _ = weight_tensor.tensor.ShapeAsNumpy()
    elif op_code_str == "DEPTHWISE_CONV_2D":
        _, kernel_h, kernel_w, output_c = weight_tensor.tensor.ShapeAsNumpy()
    _, output_h, output_w, output_c_dual = output_tensor.tensor.ShapeAsNumpy()
    assert output_c_dual == output_c, "output channels not match"

    # tensor types
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    weight_type = getTensorTypeStr(weight_tensor.tensor.Type())
    assert input_type == output_type == weight_type, "tensor type not consistent"

    # tensor value: weight, scalers
    weight_value = get_np_from_wrapper(weight_tensor)
    if input_tensor_count == 3:
        bias_tensor = input_tensors[2]
        # bias = self._get_np_from_wrapper(bias_tensor).astype('int') # forcely casting for testing latency
        bias = get_np_from_wrapper(bias_tensor)
    else:
        bias = None

    # quantized setting
    input_zero_point = input_tensor.qnn_params["zero_point"]
    output_zero_point = output_tensor.qnn_params["zero_point"]
    input_scale = input_tensor.qnn_params["scale"]
    weight_scale = weight_tensor.qnn_params["scale"]
    output_scale = output_tensor.qnn_params["scale"]
    effective_scale = np.double(input_scale) * np.double(weight_scale) / np.double(output_scale)

    # quantized inference, used for requantize
    multiplier, shift = getMultiplierShift(effective_scale)

    # find previous layer and redirct the index and fuse pad into conv
    if tmpPADIndice is not None:
        if tmpPADIndice.output_idx == input_tensor.tensor_idx:
            input_idx = tmpPADIndice.input_idx
            input_h = input_h - math.floor(kernel_h / 2) * 2
            input_w = input_w - math.floor(kernel_h / 2) * 2
        else:
            input_idx = input_tensor.tensor_idx
    else:
        input_idx = input_tensor.tensor_idx

    params = {
        # operator
        "op": op_code_str,
        # conv
        "kernel_h": kernel_h,
        "kernel_w": kernel_w,
        "padding": math.floor(kernel_h / 2),
        "stride_h": stride_h,
        "stride_w": stride_w,
        # tensor
        "input_idx": input_idx,
        "output_idx": output_tensor.tensor_idx,
        "input_dim": 3,
        "output_dim": 3,
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "dtypte": input_type,
        # trainable parameters
        "weight_value": weight_value,
        "bias": bias,
        "effective_scale": effective_scale,
        "input_zero_point": input_zero_point,
        "output_zero_point": output_zero_point,
        "input_scale": input_scale,
        "weight_scale": weight_scale,
        "output_scale": output_scale,
        # quantized infernece
        "multiplier": multiplier,
        "shift": shift,
    }

    if op_code_str == "CONV_2D":
        op = conv2d.Conv2d(params)
    elif op_code_str == "DEPTHWISE_CONV_2D":
        op = depthwiseConv2d.DepthwiseConv2d(params)

    return op
