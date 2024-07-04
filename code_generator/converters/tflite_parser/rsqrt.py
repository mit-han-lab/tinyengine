# code_generator/converters/tflite_parser/rsqrt.py

from code_generator.operators.rsqrt import RsqrtOperator

from .utils import (
    get_input_tensors, 
    get_output_tensors, 
    getOpCodeStr, 
    getTensorTypeStr,    
    getSigShift,
    getADDMultiplierShift
)
    
import numpy as np

def parse_rsqrt(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    assert len(input_tensors) == 1, "RSQRT operation requires 1 input tensor"
    assert len(output_tensors) == 1, "RSQRT operation requires 1 output tensor"

    input_tensor = input_tensors[0]
    output_tensor = output_tensors[0]

    input_shape = input_tensor.tensor.ShapeAsNumpy().tolist()
    output_shape = output_tensor.tensor.ShapeAsNumpy().tolist()

    # Debug: Print shapes for verification
    print(f"Input Shape: {input_shape}")
    print(f"Output Shape: {output_shape}")

    input_idx = input_tensor.tensor_idx
    output_idx = output_tensor.tensor_idx

    input_dtype = getTensorTypeStr(input_tensor.tensor.Type())
    output_dtype = getTensorTypeStr(output_tensor.tensor.Type())

    # Handle quantization parameters
    input_scale = input_tensor.qnn_params["scale"] if input_tensor.qnn_params else None
    input_zero_point = input_tensor.qnn_params["zero_point"] if input_tensor.qnn_params else None
    output_scale = output_tensor.qnn_params["scale"] if output_tensor.qnn_params else None
    output_zero_point = output_tensor.qnn_params["zero_point"] if output_tensor.qnn_params else None

    # Calculate multiplier and shift if quantized
    if input_scale and output_scale:
        left_shift = 20  # example value, can be adjusted based on needs
        input_multiplier, input_shift = getSigShift(input_scale, input_scale, output_scale)
        output_multiplier, output_shift = getADDMultiplierShift(input_scale, output_scale)
    else:
        left_shift, input_multiplier, input_shift = None, None, None
        output_multiplier, output_shift = None, None

    # assign params
    params = {
        # operator
        "op": "RSQRT",
        # tensor
        "input_idx": input_idx,
        "output_idx": output_idx,
        "input_shape": input_shape,
        "output_shape": output_shape,
        "dtype": input_dtype,
        # trainable parameters
        "input_zero_point": input_zero_point,
        "output_zero_point": output_zero_point,
        "input_scale": input_scale,
        "output_scale": output_scale,
        # quantized inference
        "left_shift": left_shift,
        "input_multiplier": input_multiplier,
        "input_shift": input_shift,
        "output_multiplier": output_multiplier,
        "output_shift": output_shift,
    }

    return RsqrtOperator(params)
