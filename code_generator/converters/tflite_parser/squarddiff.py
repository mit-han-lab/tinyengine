from code_generator.operators import squarddiff
from code_generator.tflite import Model

from .utils import get_input_tensors, get_nhwc_from_shape, get_np_from_wrapper, get_output_tensors, getTensorTypeStr


def parse_squarddiff(op, model: Model.Model):
    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    input_tensor = input_tensors[0]
    input2_tensor = input_tensors[1]

    output_tensors = get_output_tensors(op, model)
    output_tensor = output_tensors[0]

    # shapes
    _, input_h, input_w, input_c = get_nhwc_from_shape(input_tensor.tensor.ShapeAsNumpy())
    _, input2_h, input2_w, input2_c = get_nhwc_from_shape(input2_tensor.tensor.ShapeAsNumpy())
    _, output_h, output_w, output_c = get_nhwc_from_shape(output_tensor.tensor.ShapeAsNumpy())

    # Find out which tensor is the main input, we assume the 1st input is the main one
    if input2_c == output_c and output_w == input2_w and output_h == input2_h:
        temp = input_tensor
        input2_tensor = input_tensor
        input_tensor = temp
        th, tw, tc = input_h, input_w, input_c
        input_h, input_w, input_c = input2_h, input2_w, input2_c
        input2_h, input2_w, input2_c = th, tw, tc

    assert input_h == output_h, "tensor shape not consistent"
    assert input_w == output_w, "tensor shape not consistent"
    assert input_c == output_c, "tensor shape not consistent"

    # tensor types
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert input_type == output_type, "tensor type is not consistent"

    # Check if the divisor is constant, e.g., a scale value or tensor
    input2_idx = input2_tensor.tensor_idx
    try:
        input2 = get_np_from_wrapper(input2_tensor)
        input2_idx = "constant" + str(input2_idx)
    except Exception:
        input2 = None

    # assign params
    params = {
        # op related
        "op": "SQUARED_DIFFERENCE",
        "input_idx": input_tensor.tensor_idx,
        "input2": input2,
        "input2_idx": input2_idx,
        "output_idx": output_tensor.tensor_idx,
        # tensor related
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "input2_h": input2_h,
        "input2_w": input2_w,
        "input2_c": input2_c,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "input_size": input_h * input_w * input_c,
        "input_dtype": input_type,
        "output_dtype": output_type,
        "input_meta": None,
    }
    op = squarddiff.squarddiff(params)

    return op
