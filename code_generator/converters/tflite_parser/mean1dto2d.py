from code_generator.operators import avgpool2d
from code_generator.tflite import Model

from .utils import get_hwc_from_chwshape, get_input_tensors, get_output_tensors, getTensorTypeStr


def parse_mead1dto2d(op, model: Model.Model, MEAN2Dholder=None):
    # Incase no params
    input_type = None

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

    if not MEAN2Dholder.has_first_1D:
        MEAN2Dholder.add_first_1D_op(input_tensor.tensor_idx, output_tensor.tensor_idx, input_h, input_w, input_c)
        return None
    elif not MEAN2Dholder.has_second_1D:
        MEAN2Dholder.add_second_1D_op(input_tensor.tensor_idx, output_tensor.tensor_idx, output_h, output_w, output_c)
        filter_h = input_h - output_h + 1
        filter_w = input_w - output_w + 1
        params = {
            # operator
            "op": "AVERAGE_POOL_2D",
            # pool parameters
            "filter_h": filter_h,
            "filter_w": filter_w,
            "stride_h": 1,
            "stride_w": 1,
            "pad_h": 0,
            "pad_w": 0,
            # tensor
            "input_idx": MEAN2Dholder.first_1D_input_idx,
            "output_idx": MEAN2Dholder.second_1D_output_idx,
            "input_h": MEAN2Dholder.input_h,
            "input_w": MEAN2Dholder.input_w,
            "input_c": MEAN2Dholder.input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": MEAN2Dholder.output_h,
            "output_w": MEAN2Dholder.output_w,
            "output_c": MEAN2Dholder.output_c,
            "dtypte": input_type,
        }

        op = avgpool2d.AvgPool2d(params)

        # reset MEAN2Dholder
        MEAN2Dholder.reset_holder()

        return op
    else:
        raise NotImplementedError


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
