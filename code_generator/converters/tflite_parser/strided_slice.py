# code_generator/converters/tflite_parser/strided_slice.py

from code_generator.operators.strided_slice_v2 import StridedSliceOperator
from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr
import tflite

def parse_strided_slice(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    input_shapes = [tensor.tensor.ShapeAsNumpy().tolist() for tensor in input_tensors]
    output_shape = output_tensors[0].tensor.ShapeAsNumpy().tolist()

    # Ensure the shapes have 4 dimensions
    input_shapes = [shape + [1] * (4 - len(shape)) for shape in input_shapes]
    if len(output_shape) < 4:
        output_shape += [1] * (4 - len(output_shape))

    # Parse options
    options = tflite.StridedSliceOptions()
    options.Init(op.BuiltinOptions().Bytes, op.BuiltinOptions().Pos)

    begin_mask = options.BeginMask()
    end_mask = options.EndMask()
    shrink_axis_mask = options.ShrinkAxisMask()

    # Manually extract begin, end, and strides
    n = model.BuffersLength()
    begin = [model.Buffers(i).DataAsNumpy() for i in range(n)]
    end = [model.Buffers(i).DataAsNumpy() for i in range(n)]
    strides = [model.Buffers(i).DataAsNumpy() for i in range(n)]

    params = {
        "op": "STRIDED_SLICE",
        "input_idx": input_tensors[0].tensor_idx,
        "output_idx": output_tensors[0].tensor_idx,
        "input_dtype": getTensorTypeStr(input_tensors[0].tensor.Type()),
        "output_dtype": getTensorTypeStr(output_tensors[0].tensor.Type()),
        "input_shape": input_shapes[0],
        "output_shape": output_shape,
        "d1": input_shapes[0][0],
        "d2": input_shapes[0][1],
        "d3": input_shapes[0][2],
        "d4": input_shapes[0][3],
        "o_d1": output_shape[0],
        "o_d2": output_shape[1],
        "o_d3": output_shape[2],
        "o_d4": output_shape[3],
        "begin": begin,
        "end": end,
        "strides": strides,
        "begin_mask": begin_mask,
        "end_mask": end_mask,
        "shrink_axis_mask": shrink_axis_mask
    }

    return StridedSliceOperator(params)
