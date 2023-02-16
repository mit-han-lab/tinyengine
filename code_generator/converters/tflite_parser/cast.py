from code_generator.operators import cast
from code_generator.tflite import Model

from .utils import get_input_tensors, get_nhwc_from_shape, get_output_tensors, getTensorTypeStr


def parse_cast(op, model: Model.Model):
    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    input_tensor = input_tensors[0]

    output_tensors = get_output_tensors(op, model)
    assert len(output_tensors) == 1, "output tensors length should be 1"
    output_tensor = output_tensors[0]

    # shapes
    _, input_h, input_w, input_c = get_nhwc_from_shape(input_tensor.tensor.ShapeAsNumpy())
    _, output_h, output_w, output_c = get_nhwc_from_shape(output_tensor.tensor.ShapeAsNumpy())

    assert input_h == output_h, "tensor shape not consistent"
    assert input_w == output_w, "tensor shape not consistent"
    assert input_c == output_c, "tensor shape not consistent"

    # tensor types
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert input_type != output_type, "expect tensor types to be inconsistent"

    # assign params
    params = {
        # op related
        "op": "CAST",
        "input_idx": input_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        # tensor related
        "input_size": input_h * input_w * input_c,
        "input_dtype": input_type,
        "output_dtype": output_type,
        "input_meta": None,
    }
    op = cast.cast(params)

    return op
