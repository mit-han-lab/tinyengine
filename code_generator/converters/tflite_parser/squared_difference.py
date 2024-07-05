# code_generator/converters/tflite_parser/squared_difference.py

from code_generator.operators.squared_difference import SquaredDifferenceOperator
from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr

def parse_squared_difference(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    input_tensor = input_tensors[0]
    input2_tensor = input_tensors[1]
    output_tensor = output_tensors[0]

    input_shape = input_tensor.tensor.ShapeAsNumpy().tolist()
    input2_shape = input2_tensor.tensor.ShapeAsNumpy().tolist()
    output_shape = output_tensor.tensor.ShapeAsNumpy().tolist()

    params = {
        "op": "SQUARED_DIFFERENCE",
        "input_idx": input_tensor.tensor_idx,
        "input2_idx": input2_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        "input_shape": input_shape,
        "input2_shape": input2_shape,
        "output_shape": output_shape,
        "input_dtype": getTensorTypeStr(input_tensor.tensor.Type()),
        "input2_dtype": getTensorTypeStr(input2_tensor.tensor.Type()),
        "output_dtype": getTensorTypeStr(output_tensor.tensor.Type())
    }

    return SquaredDifferenceOperator(params)
