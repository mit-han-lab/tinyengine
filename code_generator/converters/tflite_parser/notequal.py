from code_generator.operators import notequal
from code_generator.tflite import Model

from .utils import get_input_tensors, get_nhwc_from_shape, get_np_from_wrapper, get_output_tensors, getTensorTypeStr


def parse_notequal(op, model: Model.Model):
    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)

    output_tensors = get_output_tensors(op, model)
    output_tensor = output_tensors[0]

    # tensor types
    input_type = getTensorTypeStr(input_tensors[0].tensor.Type())
    input2_type = getTensorTypeStr(input_tensors[1].tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert input_type == input2_type, "tensor type not consistent"

    # shapes
    _, input_h, input_w, input_c = get_nhwc_from_shape(input_tensors[0].tensor.ShapeAsNumpy())
    _, input2_h, input2_w, input2_c = get_nhwc_from_shape(input_tensors[1].tensor.ShapeAsNumpy())
    _, output_h, output_w, output_c = get_nhwc_from_shape(output_tensor.tensor.ShapeAsNumpy())

    # Check if the divisor is constant, e.g., a scale value or tensor
    input2_idx = input_tensors[1].tensor_idx
    try:
        input2 = get_np_from_wrapper(input_tensors[1])
        input2_idx = "constant"
    except Exception:
        input2 = None

    params = {
        # op related
        "op": "NOT_EQUAL",
        "input_idx": input_tensors[0].tensor_idx,
        "input2_idx": input2_idx,
        "output_idx": output_tensors[0].tensor_idx,
        # tensor related
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "input2": input2,
        "input2_h": input2_h,
        "input2_w": input2_w,
        "input2_c": input2_c,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "input_dtype": input_type,
        "input2_dtype": input2_type,
        "output_dtype": output_type,
    }

    return notequal.notequal(params)
