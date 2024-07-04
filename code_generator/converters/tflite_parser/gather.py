import math

import numpy as np
from code_generator.operators.gather import GatherOperator
from code_generator.tflite import Model
from code_generator.tflite.BuiltinOperator import BuiltinOperator
from code_generator.tflite.TensorType import TensorType

from .utils import (
    get_input_tensors,
    get_output_tensors,
    getTensorTypeStr,
)
def parse_gather(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)
    
    # print(input_tensors[0])
    
    params = {
        "op": "GATHER",
        "input_idx": input_tensors[0].tensor_idx,
        "output_idx": output_tensors[0].tensor_idx,
        "input_dtype": getTensorTypeStr(input_tensors[0].tensor.Type()),
        "output_dtype": getTensorTypeStr(output_tensors[0].tensor.Type()),
        "input_shape": input_tensors[0].tensor.ShapeAsNumpy(),
        "output_shape": output_tensors[0].tensor.ShapeAsNumpy()
    }
    return GatherOperator(params)
    