from code_generator.operators import softmax
from code_generator.tflite import Model

from .utils import get_input_tensors, get_nhwc_from_shape, get_output_tensors, getTensorTypeStr


def parse_softmax(op, model: Model.Model):
    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    input_tensor = input_tensors[0]
    assert len(input_tensors) == 1, "input tensors length should be 1"

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
    assert input_type == output_type, "tensor types inconsistent"

    # assign params
    params = {
        # op related
        "op": "SOFTMAX",
        "input_idx": input_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        # tensor related
        "input_size": input_h * input_w * input_c,
        "input_dtype": input_type,
        "output_dtype": output_type,
        "input_meta": None,
    }
    op = softmax.softmax(params)

    return op
