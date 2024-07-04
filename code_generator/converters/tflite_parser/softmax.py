# code_generator/converters/tflite_parser/softmax.py

from code_generator.operators.softmax import SoftmaxOperator
from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr
from code_generator.tflite.SoftmaxOptions import SoftmaxOptions

def parse_softmax(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    input_tensor = input_tensors[0]
    output_tensor = output_tensors[0]

    input_shape = input_tensor.tensor.ShapeAsNumpy().tolist()
    output_shape = output_tensor.tensor.ShapeAsNumpy().tolist()

    options = SoftmaxOptions()
    options.Init(op.BuiltinOptions().Bytes, op.BuiltinOptions().Pos)
    beta = options.Beta()

    params = {
        "op": "SOFTMAX",
        "input_idx": input_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        "input_shape": input_shape,
        "output_shape": output_shape,
        "input_dtype": getTensorTypeStr(input_tensor.tensor.Type()),
        "output_dtype": getTensorTypeStr(output_tensor.tensor.Type()),
        "beta": beta
    }

    return SoftmaxOperator(params)
