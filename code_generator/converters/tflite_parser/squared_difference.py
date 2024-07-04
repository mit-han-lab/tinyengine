# code_generator/converters/tflite_parser/squared_difference.py

from code_generator.operators.squared_difference import SquaredDifferenceOperator
# from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr
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
    
import numpy as np

def parse_squared_difference(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    assert len(input_tensors) == 2, "SQUARED_DIFFERENCE operation requires 2 input tensors"
    assert len(output_tensors) == 1, "SQUARED_DIFFERENCE operation requires 1 output tensor"

    input_tensor = input_tensors[0]
    input2_tensor = input_tensors[1]
    output_tensor = output_tensors[0]

    input_shape = input_tensor.tensor.ShapeAsNumpy().tolist()
    input2_shape = input2_tensor.tensor.ShapeAsNumpy().tolist()
    output_shape = output_tensor.tensor.ShapeAsNumpy().tolist()

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

    # Handle quantization parameters
    input_scale = input_tensor.qnn_params["scale"] if input_tensor.qnn_params else None
    input_zero_point = input_tensor.qnn_params["zero_point"] if input_tensor.qnn_params else None
    input2_scale = input2_tensor.qnn_params["scale"] if input2_tensor.qnn_params else None
    input2_zero_point = input2_tensor.qnn_params["zero_point"] if input2_tensor.qnn_params else None
    output_scale = output_tensor.qnn_params["scale"] if output_tensor.qnn_params else None
    output_zero_point = output_tensor.qnn_params["zero_point"] if output_tensor.qnn_params else None

    # Calculate multiplier and shift if quantized
    if input_scale and input2_scale and output_scale:
        input_product_scale = input_scale * input2_scale
        left_shift = 20  # example value, can be adjusted based on needs
        input_multiplier, input_shift = getSigShift(input_scale, input_product_scale, output_scale)
        input2_multiplier, input2_shift = getSigShift(input2_scale, input_product_scale, output_scale)
        output_multiplier, output_shift = getADDMultiplierShift(input_product_scale, output_scale)
    else:
        left_shift, input_multiplier, input_shift = None, None, None
        input2_multiplier, input2_shift, output_multiplier, output_shift = None, None, None, None

    # assign params
    params = {
        # operator
        "op": "SQUARED_DIFFERENCE",
        # tensor
        "input_idx": input_idx,
        "input2_idx": input2_idx,
        "output_idx": output_idx,
        "input_shape": input_shape,
        "input2_shape": input2_shape,
        "output_shape": output_shape,
        "dtype": input_dtype,
        # trainable parameters
        "input_zero_point": input_zero_point,
        "input2_zero_point": input2_zero_point,
        "output_zero_point": output_zero_point,
        "input_scale": input_scale,
        "input2_scale": input2_scale,
        "output_scale": output_scale,
        # quantized inference
        "left_shift": left_shift,
        "input_multiplier": input_multiplier,
        "input2_multiplier": input2_multiplier,
        "input_shift": input_shift,
        "input2_shift": input2_shift,
        "output_multiplier": output_multiplier,
        "output_shift": output_shift,
    }

    return SquaredDifferenceOperator(params)