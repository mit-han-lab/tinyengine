# code_generator/converters/tflite_parser/shape.py

from code_generator.operators.shape import ShapeOperator
from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr

def parse_shape(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    params = {
        "op": "SHAPE",
        "input_idx": input_tensors[0].tensor_idx,
        "output_idx": output_tensors[0].tensor_idx,
        "input_dtype": getTensorTypeStr(input_tensors[0].tensor.Type()),
        "output_dtype": getTensorTypeStr(output_tensors[0].tensor.Type()),
        "input_shape": input_tensors[0].tensor.ShapeAsNumpy().tolist(),  # Ensure input_shape is set
    }
    
    return ShapeOperator(params)
