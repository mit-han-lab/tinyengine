from code_generator.tflite import Model

from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr


class tensor_mapping:
    input_idx: str
    output_idx: str

    def __init__(self, input_idx, output_idx) -> None:
        self.input_idx = input_idx
        self.output_idx = output_idx


def parse_reshape_fuse_tensor_tuple(op, model: Model.Model):
    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)

    output_tensors = get_output_tensors(op, model)
    output_tensor = output_tensors[0]

    # tensor types
    input_type = getTensorTypeStr(input_tensors[0].tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert input_type == output_type, "tensor type not consistent"

    return tensor_mapping(input_tensors[0].tensor_idx, output_tensor.tensor_idx)
