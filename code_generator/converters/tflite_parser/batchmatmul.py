from code_generator.operators import batchmatmul
from code_generator.tflite import Model

from .utils import get_input_tensors, get_nhwc_from_shape, get_output_tensors, getTensorTypeStr


def parse_batchmatmul(op, model: Model.Model):
    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    input_tensor_count = len(input_tensors)
    assert input_tensor_count == 2, "input tensors length should be 2"

    input_tensor = input_tensors[0]
    input2_tensor = input_tensors[1]

    output_tensors = get_output_tensors(op, model)
    assert len(output_tensors) == 1, "output tensors length should be 1"
    output_tensor = output_tensors[0]

    # shapes
    _, i_batch_size, input_row, input_col = get_nhwc_from_shape(input_tensor.tensor.ShapeAsNumpy())
    _, i2_batch_size, input2_row, input2_col = get_nhwc_from_shape(input2_tensor.tensor.ShapeAsNumpy())
    _, o_batch_size, output_row, output_col = get_nhwc_from_shape(output_tensor.tensor.ShapeAsNumpy())
    assert i_batch_size == i2_batch_size == o_batch_size, "batch size not match"
    assert input_col == input2_row, "matric dimension not match"
    assert input2_col == output_col, "matric dimension not match"
    assert input_row == output_row, "matric dimension not match"

    # tensor types
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    input2_type = getTensorTypeStr(input2_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert input_type == output_type == input2_type, "tensor type not consistent"

    params = {
        # op related
        "op": "BATCH_MATMUL",
        "batch_size": i_batch_size,
        "input_idx": input_tensor.tensor_idx,
        "input2_idx": input2_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        # tensor related
        "input_col": input_col,
        "input_row": input_row,
        "input2": None,
        "input2_col": input2_col,
        "input2_row": input2_row,
        "output_col": output_col,
        "output_row": output_row,
        "input_dtype": "fp32",
        "input2_dtype": "fp32",
        "output_dtype": "fp32",
    }
    op = batchmatmul.batchmatmul(params)

    return op
