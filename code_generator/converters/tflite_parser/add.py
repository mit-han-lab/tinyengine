import math

import numpy as np

from code_generator.operators import add
from code_generator.tflite import Model

from .utils import get_input_tensors, get_nhwc_from_shape, get_output_tensors, getOpCodeStr, getTensorTypeStr


def parse_add(op, model: Model.Model):
    # operator
    op_code_str = getOpCodeStr(op, model)

    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    input_tensor_count = len(input_tensors)
    assert input_tensor_count == 2, "input should be 2 tensors"

    input_tensor = input_tensors[0]
    input2_tensor = input_tensors[1]

    output_tensors = get_output_tensors(op, model)
    assert len(output_tensors) == 1, "output tensors length should be 1"
    output_tensor = output_tensors[0]

    # shapes
    _, input_h, input_w, input_c = get_nhwc_from_shape(input_tensor.tensor.ShapeAsNumpy())
    _, input2_h, input2_w, input2_c = get_nhwc_from_shape(input2_tensor.tensor.ShapeAsNumpy())
    _, output_h, output_w, output_c = get_nhwc_from_shape(output_tensor.tensor.ShapeAsNumpy())
    assert input_h == input2_h == output_h, "tensor shpae not consistent"
    assert input_w == input2_w == output_w, "tensor shpae not consistent"
    assert input_c == input2_c == output_c, "tensor shpae not consistent"

    # tensor types
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    input_type2 = getTensorTypeStr(input2_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert input_type == input_type2 == output_type, "tensor type not consistent"

    # initialize quantized parameters as None for floating-pointer ops
    input_zero_point = None
    input_scale = None
    input2_zero_point = None
    input2_scale = None
    output_zero_point = None
    output_scale = None

    left_shift = None
    input_multiplier = None
    input_shift = None
    input2_multiplier = None
    input2_shift = None
    output_multiplier = None
    output_shift = None

    # quantized setting
    if input_type != "float32":
        input_zero_point = input_tensor.qnn_params["zero_point"]
        input_scale = input_tensor.qnn_params["scale"]
    if input_type2 != "float32":
        input2_zero_point = input2_tensor.qnn_params["zero_point"]
        input2_scale = input2_tensor.qnn_params["scale"]
    if output_type != "float32":
        output_zero_point = output_tensor.qnn_params["zero_point"]
        output_scale = output_tensor.qnn_params["scale"]

    if "float32" not in [output_type, input_type, input_type2]:
        # get multipliers and shifts
        (
            left_shift,
            input_multiplier,
            input_shift,
            input2_multiplier,
            input2_shift,
            output_multiplier,
            output_shift,
        ) = _getADDMultiplierShift(input_scale, input2_scale, output_scale)

    # assign params
    params = {
        # operator
        "op": op_code_str,
        # tensor
        "input_idx": input_tensor.tensor_idx,
        "input2_idx": input2_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "input2_h": input_h,
        "input2_w": input_w,
        "input2_c": input_c,
        "input_dim": 3,
        "input2_dim": 3,
        "output_dim": 3,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "dtypte": input_type,
        # trainable parameters
        "input_zero_point": input_zero_point,
        "input2_zero_point": input2_zero_point,
        "output_zero_point": output_zero_point,
        "input_scale": input_scale,
        "input2_scale": input2_scale,
        "output_scale": output_scale,
        # quantized infernece
        "left_shift": left_shift,
        "input_multiplier": input_multiplier,
        "input2_multiplier": input2_multiplier,
        "input_shift": input_shift,
        "input2_shift": input2_shift,
        "output_multiplier": output_multiplier,
        "output_shift": output_shift,
    }
    op = add.Add(params)

    return op


def _getSigShift(s):
    sig, shi = math.frexp(s)
    sig = int(round(sig * 2**31))
    if sig == 2**31:
        sig /= 2
        shi += 1
    if shi < -31:
        shi = 0
        sig = 0

    return sig, shi


def _getADDMultiplierShift(input_scale, input2_scale, output_scale):
    left_shift = 20

    twice_max_input_scale = 2 * np.double(max(input_scale, input2_scale))
    real_input1_multiplier = np.double(input_scale / twice_max_input_scale)
    real_input2_multiplier = np.double(input2_scale / twice_max_input_scale)
    real_output_multiplier = np.double(twice_max_input_scale / ((1 << left_shift) * output_scale))

    input_multiplier, input_shift = _getSigShift(real_input1_multiplier)
    input2_multiplier, input2_shift = _getSigShift(real_input2_multiplier)
    output_multiplier, output_shift = _getSigShift(real_output_multiplier)

    return (
        left_shift,
        input_multiplier,
        input_shift,
        input2_multiplier,
        input2_shift,
        output_multiplier,
        output_shift,
    )
