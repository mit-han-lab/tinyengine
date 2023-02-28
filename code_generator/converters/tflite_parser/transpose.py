from code_generator.operators import transpose
from code_generator.tflite import Model

from .utils import (
    get_input_tensors,
    get_nhwc_from_shape,
    get_np_from_wrapper,
    get_output_tensors,
    getOpCodeStr,
    getTensorTypeStr,
)


def parse_transpose(op, model: Model.Model):
    # operator
    op_code_str = getOpCodeStr(op, model)

    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    input_tensor_count = len(input_tensors)
    assert input_tensor_count == 2, "input should be 2 tensors"

    input_tensor = input_tensors[0]
    input2_tensor = input_tensors[1]

    output_tensors = get_output_tensors(op, model)
    assert len(output_tensors) == 1, "output tensors length should be 1"
    output_tensor = output_tensors[0]

    # shapes
    _, input_h, input_w, input_c = get_nhwc_from_shape(input_tensor.tensor.ShapeAsNumpy())
    _, output_h, output_w, output_c = get_nhwc_from_shape(output_tensor.tensor.ShapeAsNumpy())

    # tensor types
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())

    # Check if any constant
    input2_idx = input2_tensor.tensor_idx
    try:
        input2 = get_np_from_wrapper(input2_tensor)
        input2_idx = "constant" + str(input2_idx)
    except Exception:
        input2 = None

    input_idx = input_tensor.tensor_idx
    assert input2[0] == 0 and len(input2) == 4

    # assign params
    params = {
        # operator
        "op": op_code_str,
        # tensor
        "input_idx": input_idx,
        "input_size": input_h * input_w * input_c,
        "input2_idx": input2_idx,
        "permute": input2,
        "output_idx": output_tensor.tensor_idx,
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "input_dim": 3,
        "output_dim": 3,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "dtypte": input_type,
        "input_dtype": input_type,
        "output_dtype": output_type,
    }
    op = transpose.transpose(params)

    return op
