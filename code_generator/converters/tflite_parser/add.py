import math

import numpy as np

from code_generator.operators import add
from code_generator.tflite import Model

from .utils import (
    get_input_tensors, 
    get_nhwc_from_shape, 
    get_output_tensors, 
    getOpCodeStr, 
    getTensorTypeStr,
    getMultiplierShift_,
    getSigShift,
    getADDMultiplierShift
)
    


def parse_add(op, model: Model.Model):
    # operator
    op_code_str = getOpCodeStr(op, model)

    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    assert len(input_tensors) == 2, "ADD operation requires 2 input tensors"
    assert len(output_tensors) == 1, "ADD operation requires 1 output tensor"

    input_tensor = input_tensors[0]
    input2_tensor = input_tensors[1]
    output_tensor = output_tensors[0]
    
    input_shape = np.array(input_tensor.tensor.ShapeAsNumpy())
    input2_shape = np.array(input2_tensor.tensor.ShapeAsNumpy())
    output_shape = np.array(output_tensor.tensor.ShapeAsNumpy())

    # Debug: Print shapes for verification
    print(f"Input Shape 1: {input_shape}")
    print(f"Input Shape 2: {input2_shape}")
    print(f"Output Shape: {output_shape}")
    
    # Handle broadcasting by checking if shapes are broadcastable
    try:
        broadcast_shape = np.broadcast_shapes(input_shape, input2_shape)
    except ValueError:
        raise AssertionError("tensor shape not consistent and not broadcastable")

    assert np.array_equal(broadcast_shape, output_shape), "output shape is not consistent with broadcast shape"


    input_idx = input_tensor.tensor_idx
    input2_idx = input2_tensor.tensor_idx
    output_idx = output_tensor.tensor_idx

    input_dtype = getTensorTypeStr(input_tensor.tensor.Type())
    output_dtype = getTensorTypeStr(output_tensor.tensor.Type())

    # Extract dimensions
    input_h, input_w, input_c = input_shape[-3:]
    output_h, output_w, output_c = output_shape[-3:]

    # Handle quantization parameters
    input_scale = input_tensor.qnn_params["scale"] if input_tensor.qnn_params else None
    input_zero_point = input_tensor.qnn_params["zero_point"] if input_tensor.qnn_params else None
    input2_scale = input2_tensor.qnn_params["scale"] if input2_tensor.qnn_params else None
    input2_zero_point = input2_tensor.qnn_params["zero_point"] if input2_tensor.qnn_params else None
    output_scale = output_tensor.qnn_params["scale"] if output_tensor.qnn_params else None
    output_zero_point = output_tensor.qnn_params["zero_point"] if output_tensor.qnn_params else None

    # Calculate multiplier and shift if quantized
    if input_scale and input2_scale and output_scale:
        left_shift = 20  # example value, can be adjusted based on needs
        input_multiplier, input_shift = getSigShift(input_scale, input_scale, output_scale)
        input2_multiplier, input2_shift = getSigShift(input2_scale, input2_scale, output_scale)
        output_multiplier, output_shift = getADDMultiplierShift(input_scale * input2_scale, output_scale)
    else:
        left_shift, input_multiplier, input_shift = None, None, None
        input2_multiplier, input2_shift, output_multiplier, output_shift = None, None, None, None

    # assign params
    params = {
        # operator
        "op": "ADD",
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
        "input_dtype": input_dtype,
        "input2_dtype": input_dtype,
        "output_dtype": output_dtype,
        
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

# def getMultiplierShift(real_multiplier):
#     if real_multiplier == 0:
#         return 0, 0
#     shift = 0
#     while real_multiplier < 0.5:
#         real_multiplier *= 2
#         shift += 1
#     multiplier = int(round(real_multiplier * (1 << 31)))
#     return multiplier, shift

# def getSigShift(input_scale, input_product_scale, output_scale):
#     product_scale = input_product_scale / output_scale
#     product_multiplier, product_shift = getMultiplierShift(product_scale)
#     input_multiplier, input_shift = getMultiplierShift(input_scale / output_scale)
#     return input_multiplier, input_shift

# def getADDMultiplierShift(input_product_scale, output_scale):
#     output_multiplier, output_shift = getMultiplierShift(input_product_scale / output_scale)
#     return output_multiplier, output_shift

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
