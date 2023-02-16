from code_generator.operators import slice
from code_generator.tflite import Model

from .utils import get_input_tensors, get_nhwc_from_shape, get_output_tensors, getTensorTypeStr


def parse_slice(op, model: Model.Model):
    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    assert len(input_tensors) == 3, "intput tensors length should be 3"
    input_tensor = input_tensors[0]
    begin_tensor = input_tensors[1]
    end_tensor = input_tensors[1]

    output_tensors = get_output_tensors(op, model)
    assert len(output_tensors) == 1, "output tensors length should be 1"
    output_tensor = output_tensors[0]

    # shapes
    _, input_h, input_w, input_c = get_nhwc_from_shape(input_tensor.tensor.ShapeAsNumpy())
    _, output_h, output_w, output_c = get_nhwc_from_shape(output_tensor.tensor.ShapeAsNumpy())
    _, begin_h, begin_w, begin_c = get_nhwc_from_shape(begin_tensor.tensor.ShapeAsNumpy())
    _, end_h, end_w, end_c = get_nhwc_from_shape(end_tensor.tensor.ShapeAsNumpy())

    # tensor types
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert input_type == output_type, "tensor types inconsistent"

    # assign params
    params = {
        # op related
        "op": "SLICE",
        "input_idx": input_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        # tensor related
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "begin_h": begin_h,
        "begin_w": begin_w,
        "begin_c": begin_c,
        "end_h": end_h,
        "end_w": end_w,
        "end_c": end_c,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "input_dtype": input_type,
        "output_dtype": output_type,
    }
    op = slice.slice(params)

    return op
