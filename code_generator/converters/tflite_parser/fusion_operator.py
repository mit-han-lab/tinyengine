# code_generator/converters/tflite_parser/fusion.py

from code_generator.operators.fused_operator import FusedOperator
from .utils import (
    get_input_tensors,
    get_output_tensors,
    getOpCodeStr,
    getTensorTypeStr
)
import numpy as np

def parse_fusion(op, model, start_idx):
    # Extract the sequence of operators
    fusion_ops = [model.Subgraphs(0).Operators(start_idx + i) for i in range(5)]
    sequence = [getOpCodeStr(op, model) for op in fusion_ops]
    
    # Extract tensor information and create a fused operator
    reshape_op1, add_op, reshape_op2, transpose_op, fc_op = fusion_ops

    # Extract tensors for the sequence
    input_tensor = get_input_tensors(reshape_op1, model)[0]
    add_input_tensor = get_input_tensors(add_op, model)[1]
    weight_tensor = get_input_tensors(fc_op, model)[1]
    bias_tensor = get_input_tensors(fc_op, model)[2] if len(get_input_tensors(fc_op, model)) > 2 else None
    output_tensor = get_output_tensors(fc_op, model)[0]

    # Create the fused operator
    params = {
        "op": "FUSED_RESHAPE_ADD_TRANSPOSE_FC",
        "input_idx": input_tensor.tensor_idx,
        "add_input_idx": add_input_tensor.tensor_idx,
        "weight_idx": weight_tensor.tensor_idx,
        "bias_idx": bias_tensor.tensor_idx if bias_tensor else None,
        "output_idx": output_tensor.tensor_idx,
        "input_dtype": getTensorTypeStr(input_tensor.tensor.Type()),
        "add_input_dtype": getTensorTypeStr(add_input_tensor.tensor.Type()),
        "weight_dtype": getTensorTypeStr(weight_tensor.tensor.Type()),
        "output_dtype": getTensorTypeStr(output_tensor.tensor.Type()),
        "input_shape": input_tensor.tensor.ShapeAsNumpy().tolist(),
        "output_shape": output_tensor.tensor.ShapeAsNumpy().tolist()
    }

    return FusedOperator(params)
