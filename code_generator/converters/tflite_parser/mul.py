# code_generator/converters/tflite_parser/mul.py

from code_generator.operators.mul import mul
from .utils import (
    get_input_tensors,
    get_output_tensors,
    getOpCodeStr,
    getTensorTypeStr,
    getSigShift,
    getADDMultiplierShift
)
import numpy as np

def parse_mul(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    assert len(input_tensors) == 2, "MUL operation requires 2 input tensors"
    assert len(output_tensors) == 1, "MUL operation requires 1 output tensor"

    input_tensor = input_tensors[0]
    weight_tensor = input_tensors[1]
    output_tensor = output_tensors[0]

    input_shape = input_tensor.tensor.ShapeAsNumpy().tolist()
    weight_shape = weight_tensor.tensor.ShapeAsNumpy().tolist()
    output_shape = output_tensor.tensor.ShapeAsNumpy().tolist()

    # Debug: Print shapes for verification
    print(f"Input Shape: {input_shape}")
    print(f"Weight Shape: {weight_shape}")
    print(f"Output Shape: {output_shape}")

    # input_idx = input_tensor.tensor_idx
    # weight_idx = weight_tensor.tensor_idx
    # output_idx = output_tensor.tensor_idx
    
    subgraph = model.Subgraphs(0)
    input_idx = subgraph.Tensors(input_tensor.tensor_idx).Name().decode('utf-8')
    weight_idx = subgraph.Tensors(weight_tensor.tensor_idx).Name().decode('utf-8')
    output_idx = subgraph.Tensors(output_tensor.tensor_idx).Name().decode('utf-8')

    input_dtype = getTensorTypeStr(input_tensor.tensor.Type())
    weight_dtype = getTensorTypeStr(weight_tensor.tensor.Type())
    output_dtype = getTensorTypeStr(output_tensor.tensor.Type())

    input_size = np.prod(input_shape)
    weight_size = np.prod(weight_shape)
    output_size = np.prod(output_shape)

    # Handle quantization parameters
    input_scale = input_tensor.qnn_params["scale"] if input_tensor.qnn_params else None
    input_zero_point = input_tensor.qnn_params["zero_point"] if input_tensor.qnn_params else None
    weight_scale = weight_tensor.qnn_params["scale"] if weight_tensor.qnn_params else None
    weight_zero_point = weight_tensor.qnn_params["zero_point"] if weight_tensor.qnn_params else None
    output_scale = output_tensor.qnn_params["scale"] if output_tensor.qnn_params else None
    output_zero_point = output_tensor.qnn_params["zero_point"] if output_tensor.qnn_params else None

    # Initialize multipliers and shifts
    left_shift, input_multiplier, input_shift = None, None, None
    weight_multiplier, weight_shift, output_multiplier, output_shift = None, None, None, None

    # Calculate multiplier and shift if quantized
    if input_scale and weight_scale and output_scale:
        left_shift = 20  # example value, can be adjusted based on needs
        input_multiplier, input_shift = getSigShift(input_scale, input_scale * weight_scale, output_scale)
        weight_multiplier, weight_shift = getSigShift(weight_scale, input_scale * weight_scale, output_scale)
        output_multiplier, output_shift = getADDMultiplierShift(input_scale * weight_scale, output_scale)

    # assign params
    params = {
        # operator
        "op": "MUL",
        # tensor
        "input_idx": input_idx,
        "input2_idx": weight_idx,
        "output_idx": output_idx,
        "input_size": input_size,
        "input2_size": weight_size,
        "output_size": output_size,
        "input_dtype": input_dtype,
        "input2_dtype": weight_dtype,
        "output_dtype": output_dtype,
        # quantization
        "weight_value": weight_tensor.tensor.Buffer() if weight_tensor.tensor.Buffer() else None,
        "bias": None,  # assuming no bias for MUL
        
        "weight_zero_point": weight_zero_point,
        "weight_zero_point": weight_zero_point,
        "weight_multiplier": weight_multiplier,
        
        "input_zero_point": input_zero_point,
        "output_zero_point": output_zero_point,
        "input_scale": input_scale,
        "output_scale": output_scale,
        
        "left_shift": left_shift,
        "input_multiplier": input_multiplier,
        "input_shift": input_shift,
        "output_multiplier": output_multiplier,
        "output_shift": output_shift,
    }

    return mul(params)