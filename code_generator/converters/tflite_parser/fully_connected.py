# code_generator/converters/tflite_parser/fully_connected.py

from code_generator.operators.fully_connected import FullyConnectedOperator
from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr

def parse_fully_connected(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    # Extracting tensor details
    input_tensor = input_tensors[0]
    weight_tensor = input_tensors[1]
    bias_tensor = input_tensors[2] if len(input_tensors) > 2 else None
    output_tensor = output_tensors[0]

    input_shape = input_tensor.tensor.ShapeAsNumpy().tolist()
    output_shape = output_tensor.tensor.ShapeAsNumpy().tolist()

    # Create the operator parameters
    params = {
        "op": "FULLY_CONNECTED",
        "input_idx": input_tensor.tensor_idx,
        "weight_idx": weight_tensor.tensor_idx,
        "bias_idx": bias_tensor.tensor_idx if bias_tensor else -1,
        "output_idx": output_tensor.tensor_idx,
        "input_shape": input_shape,
        "output_shape": output_shape,
        "input_dtype": getTensorTypeStr(input_tensor.tensor.Type()),
        "weight_dtype": getTensorTypeStr(weight_tensor.tensor.Type()),
        "output_dtype": getTensorTypeStr(output_tensor.tensor.Type()),
        "input_zero_point": input_tensor.qnn_params["zero_point"] if input_tensor.qnn_params else 0,
        "weight_zero_point": weight_tensor.qnn_params["zero_point"] if weight_tensor.qnn_params else 0,
        "output_zero_point": output_tensor.qnn_params["zero_point"] if output_tensor.qnn_params else 0,
        "input_scale": input_tensor.qnn_params["scale"] if input_tensor.qnn_params else 1.0,
        "weight_scale": weight_tensor.qnn_params["scale"] if weight_tensor.qnn_params else 1.0,
        "output_scale": output_tensor.qnn_params["scale"] if output_tensor.qnn_params else 1.0,
        "use_bias": bias_tensor is not None,
    }

    return FullyConnectedOperator(params)
