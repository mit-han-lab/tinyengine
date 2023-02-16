from code_generator.operators import concat
from code_generator.tflite import Model

from .utils import (
    get_input_tensors,
    get_nhwc_from_shape,
    get_np_from_wrapper,
    get_output_tensors,
    getOpCodeStr,
    getTensorTypeStr,
)


def parse_concat(op, model: Model.Model):
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
    _, input2_h, input2_w, input2_c = get_nhwc_from_shape(input2_tensor.tensor.ShapeAsNumpy())
    _, output_h, output_w, output_c = get_nhwc_from_shape(output_tensor.tensor.ShapeAsNumpy())

    assert input_h == output_h, "tensor shape not consistent"
    assert input_w == output_w, "tensor shape not consistent"
    assert input_c == output_c, "tensor shape not consistent"

    # tensor types
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    input_type2 = getTensorTypeStr(input2_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert input_type == input_type2 == output_type, "tensor type not consistent"

    # Check if the divisor is constant, e.g., a scale value or tensor
    input2_idx = input2_tensor.tensor_idx
    try:
        input2 = get_np_from_wrapper(input2_tensor)
        input2_idx = "constant"
    except Exception:
        input2 = None

    # initialize quantized parameters as None for floating-pointer ops
    input_zero_point = None
    input_scale = None
    input2_zero_point = None
    input2_scale = None
    output_zero_point = None
    output_scale = None

    if "float32" not in [output_type, input_type, input_type2]:
        raise NotImplementedError("only support floating point for now.")

    # assign params
    params = {
        # operator
        "op": op_code_str,
        # tensor
        "input_idx": input_tensor.tensor_idx,
        "input_size": input_h * input_w * input_c,
        "input2_idx": input2_idx,
        "input2": input2,
        "output_idx": output_tensor.tensor_idx,
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "input2_h": input2_h,
        "input2_w": input2_w,
        "input2_c": input2_c,
        "input_dim": 3,
        "input2_dim": 3,
        "output_dim": 3,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "dtypte": input_type,
        "input_dtype": input_type,
        "input2_dtype": input_type2,
        "output_dtype": output_type,
        # trainable parameters
        "input_zero_point": input_zero_point,
        "input2_zero_point": input2_zero_point,
        "output_zero_point": output_zero_point,
        "input_scale": input_scale,
        "input2_scale": input2_scale,
        "output_scale": output_scale,
    }
    op = concat.concat(params)

    return op
