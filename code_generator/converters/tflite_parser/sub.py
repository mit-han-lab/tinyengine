# code_generator/converters/tflite_parser/sub.py

from code_generator.operators.sub import sub
from .utils import (
    get_input_tensors,
    get_output_tensors,
    getTensorTypeStr,    
)
import numpy as np

def parse_sub(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    assert len(input_tensors) == 2, "SUB operation requires 2 input tensors"
    assert len(output_tensors) == 1, "SUB operation requires 1 output tensor"

    input_tensor = input_tensors[0]
    input2_tensor = input_tensors[1]
    output_tensor = output_tensors[0]

    input_shape = input_tensor.tensor.ShapeAsNumpy().tolist()
    output_shape = output_tensor.tensor.ShapeAsNumpy().tolist()

    input_idx = input_tensor.tensor_idx
    input2_idx = input2_tensor.tensor_idx
    output_idx = output_tensor.tensor_idx

    input_dtype = getTensorTypeStr(input_tensor.tensor.Type())
    input2_dtype = getTensorTypeStr(input2_tensor.tensor.Type())
    output_dtype = getTensorTypeStr(output_tensor.tensor.Type())

    input_size = np.prod(input_shape)

    output_size = np.prod(output_shape)


    # assign params
    params = {
        # operator
        "op": "SUB",
        # tensor
        "input_idx": input_idx,
        "input2_idx": input2_idx,
        "output_idx": output_idx,
        
        "input_size": input_size,
        # "input2_size": input2_size,
        "output_size": output_size,
        "input_dtype": input_dtype,
        "input2_dtype": input2_dtype,
        "output_dtype": output_dtype,

    }

    return sub(params)
