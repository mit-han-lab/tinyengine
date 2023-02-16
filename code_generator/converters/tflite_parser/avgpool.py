from code_generator.operators import avgpool2d
from code_generator.tflite import Model
from code_generator.tflite.BuiltinOptions import BuiltinOptions
from code_generator.tflite.Padding import Padding
from code_generator.tflite.Pool2DOptions import Pool2DOptions

from .utils import get_input_tensors, get_output_tensors, getOpCodeStr, getTensorTypeStr


def parse_avgpool(op, model: Model.Model):
    # operator
    op_code_str = getOpCodeStr(op, model)

    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    input_tensor_count = len(input_tensors)
    assert input_tensor_count == 1, "input tensors length should be 1"

    input_tensor = input_tensors[0]

    output_tensors = get_output_tensors(op, model)
    assert len(output_tensors) == 1, "output tensors length should be 1"
    output_tensor = output_tensors[0]

    # shapes
    _, input_h, input_w, input_c = input_tensor.tensor.ShapeAsNumpy()
    _, output_h, output_w, output_c = output_tensor.tensor.ShapeAsNumpy()

    # tensor types
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert input_type == output_type, "tensor type not consistent"

    # pool parameters
    assert op.BuiltinOptionsType() == BuiltinOptions.Pool2DOptions
    op_options = op.BuiltinOptions()
    pool2d_options = Pool2DOptions()
    pool2d_options.Init(op_options.Bytes, op_options.Pos)
    stride_h = pool2d_options.StrideH()
    stride_w = pool2d_options.StrideW()
    padding = pool2d_options.Padding()
    filter_h = pool2d_options.FilterHeight()
    filter_w = pool2d_options.FilterWidth()

    # padding
    if padding == Padding.VALID:
        pad_h = 0
        pad_w = 0
    elif padding == Padding.SAME:
        pass  # no support for now

    # quantized setting
    input_zero_point = input_tensor.qnn_params["zero_point"]
    output_zero_point = output_tensor.qnn_params["zero_point"]
    input_scale = input_tensor.qnn_params["scale"]
    output_scale = output_tensor.qnn_params["scale"]

    params = {
        # operator
        "op": op_code_str,
        # pool parameters
        "filter_h": filter_h,
        "filter_w": filter_w,
        "stride_h": stride_h,
        "stride_w": stride_w,
        "pad_h": pad_h,
        "pad_w": pad_w,
        # tensor
        "input_idx": input_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "input_dim": input_tensor.tensor.ShapeAsNumpy().size,
        "output_dim": output_tensor.tensor.ShapeAsNumpy().size,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "dtypte": input_type,
        # trainable parameters
        "input_zero_point": input_zero_point,
        "output_zero_point": output_zero_point,
        "input_scale": input_scale,
        "output_scale": output_scale,
    }

    op = avgpool2d.AvgPool2d(params)

    return op
