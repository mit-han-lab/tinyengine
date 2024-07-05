# code_generator/converters/tflite_parser/reduce_prod.py

from code_generator.operators.reduce_prod import ReduceProdOperator
from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr
import numpy as np

def parse_reduce_prod(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    input_tensor = input_tensors[0]
    output_tensor = output_tensors[0]

    # Handling the case where the reduction axes are provided as a tensor
    if len(input_tensors) > 1:
        reduction_axes_tensor = input_tensors[1]
        reduction_axes = reduction_axes_tensor.buffer.DataAsNumpy().tolist()
    else:
        # Default reduction axes (e.g., reducing over all dimensions)
        reduction_axes = list(range(len(input_tensor.tensor.ShapeAsNumpy())))

    params = {
        "op": "REDUCE_PROD",
        "input_idx": input_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        "input_dtype": getTensorTypeStr(input_tensor.tensor.Type()),
        "output_dtype": getTensorTypeStr(output_tensor.tensor.Type()),
        "input_size": np.prod(input_tensor.tensor.ShapeAsNumpy()),
        "output_size": np.prod(output_tensor.tensor.ShapeAsNumpy()),
        "reduction_axes": reduction_axes
    }

    return ReduceProdOperator(params)
