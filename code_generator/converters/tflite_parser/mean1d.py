from code_generator.operators import mean1d
from code_generator.tflite import Model

from .utils import get_hwc_from_chwshape, get_input_tensors, get_output_tensors, getTensorTypeStr


def parse_mean1d(op, model: Model.Model):
    # get input, weight, and output tensors
    input_tensors = get_input_tensors(op, model)
    input_tensor = input_tensors[0]

    output_tensors = get_output_tensors(op, model)
    assert len(output_tensors) == 1, "output tensors length should be 1"
    output_tensor = output_tensors[0]

    # shapes
    input_shape = input_tensor.tensor.ShapeAsNumpy()
    output_shape = output_tensor.tensor.ShapeAsNumpy()

    input_h, input_w, input_c = get_hwc_from_chwshape(input_shape)
    output_h, output_w, output_c = get_hwc_from_chwshape(output_shape)
    input_type = getTensorTypeStr(input_tensor.tensor.Type())
    output_type = getTensorTypeStr(output_tensor.tensor.Type())
    assert output_type == input_type

    filter_h = input_h - output_h + 1
    filter_w = input_w - output_w + 1
    params = {
        # operator
        "op": "MEAN_1D",
        # pool parameters
        "filter_h": filter_h,
        "filter_w": filter_w,
        "stride_h": 1,
        "stride_w": 1,
        "pad_h": 0,
        "pad_w": 0,
        # tensor related
        "input_idx": input_tensor.tensor_idx,
        "output_idx": output_tensor.tensor_idx,
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "input_dtype": input_type,
        "output_dtype": output_type,
    }

    op = mean1d.mean1d(params)

    return op


class MEAN2D(object):
    def __init__(self):
        self.reset_holder()

    def add_first_1D_op(self, input_idx, output_idx, input_h, input_w, input_c):
        self.first_1D_input_idx = input_idx
        self.first_1D_output_idx = output_idx
        self.input_h = input_h
        self.input_w = input_w
        self.input_c = input_c
        self.has_first_1D = True

    def add_second_1D_op(self, input_idx, output_idx, output_h, output_w, output_c):
        self.second_1D_input_idx = input_idx
        self.second_1D_output_idx = output_idx
        self.output_h = output_h
        self.output_w = output_w
        self.output_c = output_c
        self.has_second_1D = True

    def reset_holder(self):
        self.has_first_1D = False
        self.has_second_1D = False
