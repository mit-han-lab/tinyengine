# code_generator/converters/tflite_parser/rsqrt.py

from code_generator.operators.rsqrt import RsqrtOperator
from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr

def parse_rsqrt(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    input_tensor = input_tensors[0]
    output_tensor = output_tensors[0]

    input_shape = input_tensor.tensor.ShapeAsNumpy().tolist()
    output_shape = output_tensor.tensor.ShapeAsNumpy().tolist()

    params = {
        "op": "RSQRT",
        "input_idx": input_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        "input_shape": input_shape,
        "output_shape": output_shape,
        "input_dtype": getTensorTypeStr(input_tensor.tensor.Type()),
        "output_dtype": getTensorTypeStr(output_tensor.tensor.Type())
    }

    return RsqrtOperator(params)
