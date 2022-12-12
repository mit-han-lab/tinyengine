# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   TfliteConvertor.py
#
# Reference papers:
#  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
#  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
#  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
# Contact authors:
#  - Wei-Ming Chen, wmchen@mit.edu
#  - Wei-Chen Wang, wweichen@mit.edu
#  - Ji Lin, jilin@mit.edu
#  - Ligeng Zhu, ligeng@mit.edu
#  - Song Han, songhan@mit.edu
#
# Target ISA:  ARMv7E-M
# ----------------------------------------------------------------------

import math

import numpy as np

from .constant import SKIP_OPs
from .operators import add, avgpool2d, conv2d, depthwiseConv2d, maxpool2d, se_element_mult, upsample
from .tflite import Model
from .tflite.BuiltinOperator import BuiltinOperator
from .tflite.BuiltinOptions import BuiltinOptions
from .tflite.Conv2DOptions import Conv2DOptions
from .tflite.DepthwiseConv2DOptions import DepthwiseConv2DOptions
from .tflite.Padding import Padding
from .tflite.Pool2DOptions import Pool2DOptions
from .tflite.TensorType import TensorType


# Parse tflite model into TinyEngine IR format
class TfliteConvertor(object):
    def __init__(self, filepath):
        # path to the tflite file
        self.filepath = filepath
        self.model = self.loadTFmodel(filepath)
        self.subgraph = self.model.Subgraphs(0)
        self.builtin_op_code = self._build_str_map(BuiltinOperator())
        self.layer = []
        self.tmpPADIndice = None
        self.skip_transpose = None
        self.average_1D_to_2D_holder = MEAN2D()

    # public functions
    def loadTFmodel(self, filepath):
        buf = open(filepath, "rb").read()
        return Model.Model.GetRootAsModel(buf, 0)

    def dumpModelInfo(self):
        version = self.model.Version()
        print("Model version:", version)
        description = self.model.Description().decode("utf-8")
        print("Description:", description)
        subgraph_len = self.model.SubgraphsLength()
        print("Subgraph length:", subgraph_len)

        self.dumpLayerInfo()

    def dumpLayerInfo(self):
        print("Layer length:", len(self.layer))

        # print brief info about each layer
        for i, layer in enumerate(self.layer):
            if self.layer[i]["op"] == "ADD":
                print(
                    "op:",
                    layer["op"],
                    ",input_idx:",
                    layer["input_idx"],
                    ",input2_idx:",
                    layer["input2_idx"],
                    "output_idx:",
                    layer["output_idx"],
                )
            else:
                print(
                    "op:",
                    layer["op"],
                    ",input_idx:",
                    layer["input_idx"],
                    "output_idx:",
                    layer["output_idx"],
                )

    def parseOperatorInfo(self):
        operators_len = self.subgraph.OperatorsLength()

        skip_next_ops = 0
        for i in range(operators_len):
            if skip_next_ops > 0:
                skip_next_ops -= 1
                continue

            op = self.subgraph.Operators(i)
            if i + 2 < operators_len - 2:
                next_op = self.subgraph.Operators(i + 1)
                next_next_op = self.subgraph.Operators(i + 2)
                three_op_sequence = [op, next_op, next_next_op]

                if self.checkIfRequireSEelementmult(three_op_sequence):
                    print("found SE block")
                    skip_next_ops = 2

                    SEelementmult_op = self.construct_SEelementmult(three_op_sequence)

                    self.layer.append(SEelementmult_op)
                    continue

            # parse the op
            self._handleOperator(op)

    #         -> MEAN -> MEAN -> PWCONV -> PWCONV -> | ADD -> MUL ->     |
    #  DWCONV                                        |            -> MUL |
    #                                                |   SEelementmult   |
    def construct_SEelementmult(self, three_op_sequence):
        add_op = three_op_sequence[0]
        _ = three_op_sequence[1]  # mul1_op
        mul2_op = three_op_sequence[2]

        add_op_input_tensors = self._get_input_tensors(add_op)

        assert len(add_op_input_tensors) == 2, "Unexpected number of tensors."
        # Expect another tensor to be a scaler
        if len(add_op_input_tensors[0].tensor.ShapeAsNumpy()) > len(add_op_input_tensors[1].tensor.ShapeAsNumpy()):
            pwconv_output_tensor = add_op_input_tensors[0]
        elif len(add_op_input_tensors[0].tensor.ShapeAsNumpy()) < len(add_op_input_tensors[1].tensor.ShapeAsNumpy()):
            pwconv_output_tensor = add_op_input_tensors[1]
        else:
            raise NotImplementedError("unexpected tensor shapes for SE block")

        assert (
            self.layer[-1].params["output_idx"] == pwconv_output_tensor.tensor_idx
        ), "Cannot link to the PWCONV ooutput tensor."

        mul2_op_input_tensors = self._get_input_tensors(mul2_op)
        dwcov_input_tensor = None
        # Find out the tensor link to DWCONV
        for each_layer in self.layer:
            for tensor in mul2_op_input_tensors:
                if str(each_layer.params["output_idx"]) == str(tensor.tensor_idx):
                    dwcov_input_tensor = tensor

            if dwcov_input_tensor:
                break

        assert dwcov_input_tensor, "Cannot link to the dw conv layer"

        mul2_op_output_tensor = self._get_output_tensors(mul2_op)[0]

        _, input_h, input_w, input_c = dwcov_input_tensor.tensor.ShapeAsNumpy()
        _, input2_h, input2_w, input2_c = pwconv_output_tensor.tensor.ShapeAsNumpy()
        _, output_h, output_w, output_c = mul2_op_output_tensor.tensor.ShapeAsNumpy()

        input_zero_point = dwcov_input_tensor.qnn_params["zero_point"]
        input2_zero_point = pwconv_output_tensor.qnn_params["zero_point"]
        output_zero_point = mul2_op_output_tensor.qnn_params["zero_point"]

        input_scale = dwcov_input_tensor.qnn_params["scale"]
        input2_scale = pwconv_output_tensor.qnn_params["scale"]
        output_scale = mul2_op_output_tensor.qnn_params["scale"]
        effective_scale = np.double(input_scale) * np.double(input2_scale) / np.double(output_scale)
        multiplier, shift = self._getLayerMultiplierShift(effective_scale)

        params = {
            # op related
            "op": "SE_ELEMENT_MULT_2D",
            "input_idx": dwcov_input_tensor.tensor_idx,
            "input2_idx": pwconv_output_tensor.tensor_idx,
            "output_idx": mul2_op_output_tensor.tensor_idx,
            # tensor related
            "input_dim": 3,
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input2_dim": 3,
            "input2_h": input2_h,
            "input2_w": input2_w,
            "input2_c": input2_c,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": "int8",
            "input2_dtype": "int8",
            "output_dtype": "int8",
            # quantization related
            "input_zero_point": input_zero_point,
            "input2_zero_point": input2_zero_point,
            "output_zero_point": output_zero_point,
            "output_multiplier": multiplier,
            "output_effective_scale": effective_scale,
            "output_shift": shift,
        }

        op = se_element_mult.SEelementmult(params)

        return op

    #         -> MEAN -> MEAN -> PWCONV -> PWCONV -> | ADD -> MUL ->     |
    #  DWCONV                                        |            -> MUL |
    #                                                |    Fuse Target    |
    def checkIfRequireSEelementmult(self, three_op_sequence):
        if (
            self._getOpCodeStr(three_op_sequence[0]) == "ADD"
            and self._getOpCodeStr(three_op_sequence[1]) == "MUL"
            and self._getOpCodeStr(three_op_sequence[2]) == "MUL"
        ):
            return True
        return False

    # private functions
    def _build_str_map(self, obj):
        ret = {}
        for field_name in dir(obj):
            if not field_name.startswith("_"):
                field_value = getattr(obj, field_name)
                if isinstance(field_value, int):
                    ret[field_value] = field_name
        return ret

    def _getOpCodeStr(self, op):
        op_code_list_idx = op.OpcodeIndex()
        op_code_id = self.model.OperatorCodes(op_code_list_idx).DeprecatedBuiltinCode()
        return self.builtin_op_code[op_code_id]

    def _getTensorTypeStr(self, type):
        if TensorType.INT8 == type:
            return "int8"
        if TensorType.UINT8 == type:
            return "uint8"
        if TensorType.FLOAT32 == type:
            return "float32"

    def _getMultiplierShift(self, effective_scale):
        significand = np.zeros(len(effective_scale), dtype="int32")
        shift = np.zeros(len(effective_scale), dtype="int32")

        for i, s in enumerate(effective_scale):
            if s == 0:
                significand[i] = 0
                shift[i] = 0
            else:
                sig, shi = math.frexp(s)
                sig = int(round(sig * 2**31))

                if sig == 2**31:
                    sig /= 2
                    shi += 1
                if shi < -31:
                    shi = 0
                    sig = 0

                significand[i] = sig
                shift[i] = shi

        return significand, shift

    def _getSigShift(self, s):
        sig, shi = math.frexp(s)
        sig = int(round(sig * 2**31))
        if sig == 2**31:
            sig /= 2
            shi += 1
        if shi < -31:
            shi = 0
            sig = 0

        return sig, shi

    def _getADDMultiplierShift(self, input_scale, input2_scale, output_scale):
        left_shift = 20

        twice_max_input_scale = 2 * np.double(max(input_scale, input2_scale))
        real_input1_multiplier = np.double(input_scale / twice_max_input_scale)
        real_input2_multiplier = np.double(input2_scale / twice_max_input_scale)
        real_output_multiplier = np.double(twice_max_input_scale / ((1 << left_shift) * output_scale))

        input_multiplier, input_shift = self._getSigShift(real_input1_multiplier)
        input2_multiplier, input2_shift = self._getSigShift(real_input2_multiplier)
        output_multiplier, output_shift = self._getSigShift(real_output_multiplier)

        return (
            left_shift,
            input_multiplier,
            input_shift,
            input2_multiplier,
            input2_shift,
            output_multiplier,
            output_shift,
        )

    def _preprocessSoftmaxScaling(self, beta, input_scale, input_integer_bits):

        input_beta_real_multiplier = min(beta * input_scale * (1 << (31 - input_integer_bits)), (1 << 31) - 1.0)

        multiplier, shift = self._getSigShift(input_beta_real_multiplier)

        return multiplier, shift

    # follow TFlite implementation
    def _calculateInputRadius(self, input_integer_bits, input_left_shift, total_signed_bits=31):
        max_input_rescaled = (
            1.0
            * ((1 << input_integer_bits) - 1)
            * (1 << (total_signed_bits - input_integer_bits))
            / (1 << input_left_shift)
        )
        return math.floor(max_input_rescaled)

    # converting tflite fuctions
    def _convert_convolution(self, op):
        # operator
        op_code_str = self._getOpCodeStr(op)

        # get input, weight, and output tensors
        input_tensors = self._get_input_tensors(op)
        input_tensor_count = len(input_tensors)
        assert input_tensor_count >= 2, "input tensors length should be >= 2"

        input_tensor = input_tensors[0]
        weight_tensor = input_tensors[1]

        output_tensors = self._get_output_tensors(op)
        assert len(output_tensors) == 1, "output tensors length should be 1"
        output_tensor = output_tensors[0]

        # conv_2d options
        if op_code_str == "CONV_2D":
            assert op.BuiltinOptionsType() == BuiltinOptions.Conv2DOptions
            op_options = op.BuiltinOptions()
            conv_options = Conv2DOptions()
            conv_options.Init(op_options.Bytes, op_options.Pos)
        if op_code_str == "DEPTHWISE_CONV_2D":
            assert op.BuiltinOptionsType() == BuiltinOptions.DepthwiseConv2DOptions
            op_options = op.BuiltinOptions()
            conv_options = DepthwiseConv2DOptions()
            conv_options.Init(op_options.Bytes, op_options.Pos)

        # conv parameters
        stride_h = conv_options.StrideH()
        stride_w = conv_options.StrideW()

        # shapes
        _, input_h, input_w, input_c = input_tensor.tensor.ShapeAsNumpy()
        if op_code_str == "CONV_2D":
            output_c, kernel_h, kernel_w, _ = weight_tensor.tensor.ShapeAsNumpy()
        elif op_code_str == "DEPTHWISE_CONV_2D":
            _, kernel_h, kernel_w, output_c = weight_tensor.tensor.ShapeAsNumpy()
        _, output_h, output_w, output_c_dual = output_tensor.tensor.ShapeAsNumpy()
        assert output_c_dual == output_c, "output channels not match"

        # tensor types
        input_type = self._getTensorTypeStr(input_tensor.tensor.Type())
        output_type = self._getTensorTypeStr(output_tensor.tensor.Type())
        weight_type = self._getTensorTypeStr(weight_tensor.tensor.Type())
        assert input_type == output_type == weight_type, "tensor type not consistent"

        # tensor value: weight, scalers
        weight_value = self._get_np_from_wrapper(weight_tensor)
        if input_tensor_count == 3:
            bias_tensor = input_tensors[2]
            # bias = self._get_np_from_wrapper(bias_tensor).astype('int') # forcely casting for testing latency
            bias = self._get_np_from_wrapper(bias_tensor)
        else:
            bias = None

        # quantized setting
        input_zero_point = input_tensor.qnn_params["zero_point"]
        output_zero_point = output_tensor.qnn_params["zero_point"]
        input_scale = input_tensor.qnn_params["scale"]
        weight_scale = weight_tensor.qnn_params["scale"]
        output_scale = output_tensor.qnn_params["scale"]
        effective_scale = np.double(input_scale) * np.double(weight_scale) / np.double(output_scale)

        # quantized inference, used for requantize
        multiplier, shift = self._getMultiplierShift(effective_scale)

        # find previous layer and redirct the index and fuse pad into conv
        if self.tmpPADIndice is not None:
            if self.tmpPADIndice.output_idx == input_tensor.tensor_idx:
                input_idx = self.tmpPADIndice.input_idx
                input_h = input_h - math.floor(kernel_h / 2) * 2
                input_w = input_w - math.floor(kernel_h / 2) * 2
            else:
                input_idx = input_tensor.tensor_idx
        else:
            input_idx = input_tensor.tensor_idx
        # clean the buffer
        self.tmpPADIndice = None

        params = {
            # operator
            "op": op_code_str,
            # conv
            "kernel_h": kernel_h,
            "kernel_w": kernel_w,
            "padding": math.floor(kernel_h / 2),
            "stride_h": stride_h,
            "stride_w": stride_w,
            # tensor
            "input_idx": input_idx,
            "output_idx": output_tensor.tensor_idx,
            "input_dim": 3,
            "output_dim": 3,
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "dtypte": input_type,
            # trainable parameters
            "weight_value": weight_value,
            "bias": bias,
            "effective_scale": effective_scale,
            "input_zero_point": input_zero_point,
            "output_zero_point": output_zero_point,
            "input_scale": input_scale,
            "weight_scale": weight_scale,
            "output_scale": output_scale,
            # quantized infernece
            "multiplier": multiplier,
            "shift": shift,
        }

        if op_code_str == "CONV_2D":
            op = conv2d.Conv2d(params)
        elif op_code_str == "DEPTHWISE_CONV_2D":
            op = depthwiseConv2d.DepthwiseConv2d(params)

        return op

    def _convert_ADD(self, op):
        # operator
        op_code_str = self._getOpCodeStr(op)

        # get input, weight, and output tensors
        input_tensors = self._get_input_tensors(op)
        input_tensor_count = len(input_tensors)
        assert input_tensor_count == 2, "input should be 2 tensors"

        input_tensor = input_tensors[0]
        input2_tensor = input_tensors[1]

        output_tensors = self._get_output_tensors(op)
        assert len(output_tensors) == 1, "output tensors length should be 1"
        output_tensor = output_tensors[0]

        # shapes
        _, input_h, input_w, input_c = input_tensor.tensor.ShapeAsNumpy()
        _, input2_h, input2_w, input2_c = input2_tensor.tensor.ShapeAsNumpy()
        _, output_h, output_w, output_c = output_tensor.tensor.ShapeAsNumpy()
        assert input_h == input2_h == output_h, "tensor shpae not consistent"
        assert input_w == input2_w == output_w, "tensor shpae not consistent"
        assert input_c == input2_c == output_c, "tensor shpae not consistent"

        # tensor types
        input_type = self._getTensorTypeStr(input_tensor.tensor.Type())
        input_type2 = self._getTensorTypeStr(input2_tensor.tensor.Type())
        output_type = self._getTensorTypeStr(output_tensor.tensor.Type())
        assert input_type == input_type2 == output_type, "tensor type not consistent"

        # quantized setting
        input_zero_point = input_tensor.qnn_params["zero_point"]
        input2_zero_point = input2_tensor.qnn_params["zero_point"]
        output_zero_point = output_tensor.qnn_params["zero_point"]
        input_scale = input_tensor.qnn_params["scale"]
        input2_scale = input2_tensor.qnn_params["scale"]
        output_scale = output_tensor.qnn_params["scale"]

        # get multipliers and shifts
        (
            left_shift,
            input_multiplier,
            input_shift,
            input2_multiplier,
            input2_shift,
            output_multiplier,
            output_shift,
        ) = self._getADDMultiplierShift(input_scale, input2_scale, output_scale)

        # assign params
        params = {
            # operator
            "op": op_code_str,
            # tensor
            "input_idx": input_tensor.tensor_idx,
            "input2_idx": input2_tensor.tensor_idx,
            "output_idx": output_tensor.tensor_idx,
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input2_h": input_h,
            "input2_w": input_w,
            "input2_c": input_c,
            "input_dim": 3,
            "input2_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "dtypte": input_type,
            # trainable parameters
            "input_zero_point": input_zero_point,
            "input2_zero_point": input2_zero_point,
            "output_zero_point": output_zero_point,
            "input_scale": input_scale,
            "input2_scale": input2_scale,
            "output_scale": output_scale,
            # quantized infernece
            "left_shift": left_shift,
            "input_multiplier": input_multiplier,
            "input2_multiplier": input2_multiplier,
            "input_shift": input_shift,
            "input2_shift": input2_shift,
            "output_multiplier": output_multiplier,
            "output_shift": output_shift,
        }
        op = add.Add(params)

        return op

    def _convert_AVERAGE_POOL_2D(self, op):
        # operator
        op_code_str = self._getOpCodeStr(op)

        # get input, weight, and output tensors
        input_tensors = self._get_input_tensors(op)
        input_tensor_count = len(input_tensors)
        assert input_tensor_count == 1, "input tensors length should be 1"

        input_tensor = input_tensors[0]

        output_tensors = self._get_output_tensors(op)
        assert len(output_tensors) == 1, "output tensors length should be 1"
        output_tensor = output_tensors[0]

        # shapes
        _, input_h, input_w, input_c = input_tensor.tensor.ShapeAsNumpy()
        _, output_h, output_w, output_c = output_tensor.tensor.ShapeAsNumpy()

        # tensor types
        input_type = self._getTensorTypeStr(input_tensor.tensor.Type())
        output_type = self._getTensorTypeStr(output_tensor.tensor.Type())
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

    def _convert_upsample(self, op):
        # Incase no params
        input_type = None
        input_zero_point = None
        output_zero_point = None
        input_scale = None
        output_scale = None

        # get input, weight, and output tensors
        input_tensors = self._get_input_tensors(op)
        input_tensor_count = len(input_tensors)
        assert input_tensor_count == 1, "input tensors length should be 1"

        input_tensor = input_tensors[0]

        output_tensors = self._get_output_tensors(op)
        assert len(output_tensors) == 1, "output tensors length should be 1"
        output_tensor = output_tensors[0]

        # shapes
        _, input_h, input_w, input_c = input_tensor.tensor.ShapeAsNumpy()
        _, output_h, output_w, output_c = output_tensor.tensor.ShapeAsNumpy()

        params = {
            # operator
            "op": "UPSAMPLE",
            # upsample parameters
            "factor": output_w / input_w,
            # tensor
            "input_idx": input_tensor.tensor_idx,
            "output_idx": output_tensor.tensor_idx,
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "dtype": input_type,
            # trainable parameters
            "input_zero_point": input_zero_point,
            "output_zero_point": output_zero_point,
            "input_scale": input_scale,
            "output_scale": output_scale,
            # quantized infernece
        }
        op = upsample.upSample(params)

        return op

    def _convert_PAD(self, op):
        # get input, weight, and output tensors
        input_tensors = self._get_input_tensors(op)
        input_tensor = input_tensors[0]

        output_tensors = self._get_output_tensors(op)
        assert len(output_tensors) == 1, "output tensors length should be 1"
        output_tensor = output_tensors[0]

        # fuse pad into conv
        self.tmpPADIndice = PAD_tensorIndice(input_tensor.tensor_idx, output_tensor.tensor_idx)

    def _convert_TRANSPOSE(self, op):
        # get input, weight, and output tensors
        input_tensors = self._get_input_tensors(op)
        input_tensor = input_tensors[0]

        output_tensors = self._get_output_tensors(op)
        assert len(output_tensors) == 1, "output tensors length should be 1"
        output_tensor = output_tensors[0]

        # fuse pad into conv
        self.skip_transpose = PAD_tensorIndice(input_tensor.tensor_idx, output_tensor.tensor_idx)

    def _convert_maxpool(self, op):
        # Incase no params
        input_type = None
        input_zero_point = None
        output_zero_point = None
        input_scale = None
        output_scale = None

        # get input, weight, and output tensors
        input_tensors = self._get_input_tensors(op)
        input_tensor_count = len(input_tensors)
        assert input_tensor_count == 1, "input tensors length should be 1"

        input_tensor = input_tensors[0]

        output_tensors = self._get_output_tensors(op)
        assert len(output_tensors) == 1, "output tensors length should be 1"
        output_tensor = output_tensors[0]

        # shapes
        _, input_h, input_w, input_c = input_tensor.tensor.ShapeAsNumpy()
        _, output_h, output_w, output_c = output_tensor.tensor.ShapeAsNumpy()

        # pool parameters
        assert op.BuiltinOptionsType() == BuiltinOptions.Pool2DOptions
        op_options = op.BuiltinOptions()
        pool2d_options = Pool2DOptions()
        pool2d_options.Init(op_options.Bytes, op_options.Pos)
        stride_h = pool2d_options.StrideH()
        stride_w = pool2d_options.StrideW()
        # padding = pool2d_options.Padding()
        filter_h = pool2d_options.FilterHeight()
        filter_w = pool2d_options.FilterWidth()
        # fused_activation_fn = pool2d_options.FusedActivationFunction()

        pool_params = {
            # operator
            "op": "MAX_POOL_2D",
            # pool parameters
            "filter_h": filter_h,
            "filter_w": filter_w,
            "stride_h": stride_h,
            "stride_w": stride_w,
            "pad_h": 0,
            "pad_w": 0,
            # tensor
            "input_idx": input_tensor.tensor_idx,
            "output_idx": output_tensor.tensor_idx,
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "dtype": input_type,
            # trainable parameters
            "input_zero_point": input_zero_point,
            "output_zero_point": output_zero_point,
            "input_scale": input_scale,
            "output_scale": output_scale,
            # quantized infernece
        }
        op = maxpool2d.maxPool2d(pool_params)

        return op

    def _convert_mean1D(self, op, MEAN2Dholder):
        # Incase no params
        input_type = None

        # get input, weight, and output tensors
        input_tensors = self._get_input_tensors(op)
        input_tensor_count = len(input_tensors)
        assert input_tensor_count == 1, "input tensors length should be 1"

        input_tensor = input_tensors[0]

        output_tensors = self._get_output_tensors(op)
        assert len(output_tensors) == 1, "output tensors length should be 1"
        output_tensor = output_tensors[0]

        # shapes
        input_shape = input_tensor.tensor.ShapeAsNumpy()
        output_shape = output_tensor.tensor.ShapeAsNumpy()

        input_h, input_w, input_c = get_hwc_from_chwshape(input_shape)
        output_h, output_w, output_c = get_hwc_from_chwshape(output_shape)
        input_type = self._getTensorTypeStr(input_tensor.tensor.Type())

        if not MEAN2Dholder.has_first_1D:
            MEAN2Dholder.add_first_1D_op(input_tensor.tensor_idx, output_tensor.tensor_idx, input_h, input_w, input_c)
            return None
        elif not MEAN2Dholder.has_second_1D:
            MEAN2Dholder.add_second_1D_op(
                input_tensor.tensor_idx, output_tensor.tensor_idx, output_h, output_w, output_c
            )
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

    def _convert_FULLY_CONNECTED(self, op):
        # get input, weight, and output tensors
        input_tensors = self._get_input_tensors(op)
        input_tensor_count = len(input_tensors)
        assert input_tensor_count == 3, "input tensors length should be 3"

        input_tensor = input_tensors[0]
        weight_tensor = input_tensors[1]
        bias_tensor = input_tensors[2]
        weight = self._get_np_from_wrapper(weight_tensor)
        bias = self._get_np_from_wrapper(bias_tensor)

        output_tensors = self._get_output_tensors(op)
        assert len(output_tensors) == 1, "output tensors length should be 1"
        output_tensor = output_tensors[0]

        # shapes
        if input_tensor.tensor.ShapeAsNumpy().shape[0] == 2:
            input_w, input_c = input_tensor.tensor.ShapeAsNumpy()
            input_h = 1
        elif input_tensor.tensor.ShapeAsNumpy().shape[0] == 4:
            _, input_h, input_w, input_c = input_tensor.tensor.ShapeAsNumpy()
        output_c, input_c_dual = weight_tensor.tensor.ShapeAsNumpy()
        output_h, output_c_dual = output_tensor.tensor.ShapeAsNumpy()
        assert input_c_dual == input_c, "channels not match"
        assert output_c_dual == output_c, "channels not match"

        # tensor types
        input_type = self._getTensorTypeStr(input_tensor.tensor.Type())
        output_type = self._getTensorTypeStr(output_tensor.tensor.Type())
        assert input_type == output_type, "tensor type not consistent"

        # quantized setting
        input_zero_point = input_tensor.qnn_params["zero_point"]
        output_zero_point = output_tensor.qnn_params["zero_point"]
        input_scale = input_tensor.qnn_params["scale"]
        weight_scale = weight_tensor.qnn_params["scale"]
        bias_scale = bias_tensor.qnn_params["scale"]
        output_scale = output_tensor.qnn_params["scale"]
        # We support per channel in the CONV2D operator
        if isinstance(bias_scale, float) and isinstance(weight_scale, float):
            np_ones = np.ones(output_c)
            bias_scale = np_ones * bias_scale
            np_ones = np.ones(output_c)
            output_scale = np_ones * output_scale
        effective_scale = np.double(input_scale) * np.double(weight_scale) / np.double(output_scale)

        # follows tensorflow lite micro
        multiplier, shift = self._getMultiplierShift(effective_scale)

        params = {
            # operator
            "op": "CONV_2D",
            # tensor
            "input_idx": input_tensor.tensor_idx,
            "output_idx": output_tensor.tensor_idx,
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 2,
            "output_h": output_h,
            "output_w": 1,
            "output_c": output_c,
            "dtypte": input_type,
            "kernel_h": 1,
            "kernel_w": 1,
            # trainable parameters
            "weight_value": weight,
            "bias": bias,
            "effective_scale": effective_scale,
            "input_zero_point": input_zero_point,
            "output_zero_point": output_zero_point,
            "input_scale": input_scale,
            "output_scale": output_scale,
            # quantized infernece
            "multiplier": multiplier,
            "shift": shift,
        }

        op = conv2d.Conv2d(params)

        return op

    # handle one op and parse it into layers[] for supported operators
    def _handleOperator(self, op):
        op_code_str = self._getOpCodeStr(op)
        if op_code_str == "CONV_2D":
            self.layer.append(self._convert_convolution(op))
        elif op_code_str == "ADD":
            self.layer.append(self._convert_ADD(op))
        elif op_code_str == "AVERAGE_POOL_2D":
            self.layer.append(self._convert_AVERAGE_POOL_2D(op))
        elif op_code_str == "DEPTHWISE_CONV_2D":
            self.layer.append(self._convert_convolution(op))
        elif op_code_str == "PAD":
            self._convert_PAD(op)
        elif op_code_str == "RESIZE_NEAREST_NEIGHBOR":
            self.layer.append(self._convert_upsample(op))
        elif op_code_str == "MAX_POOL_2D":
            self.layer.append(self._convert_maxpool(op))
        elif op_code_str in "MEAN":
            ret_op = self._convert_mean1D(op, self.average_1D_to_2D_holder)
            if ret_op is not None:
                # TODO: This only handle a specific graph: TRANSPOSE -> MEAN -> MEANS
                if self.skip_transpose is not None:
                    ret_op.params["input_idx"] = self.skip_transpose.input_idx
                    ret_op.input_tensors[0].graph_idx = self.skip_transpose.input_idx
                self.layer.append(ret_op)
        elif op_code_str == "TRANSPOSE":
            self._convert_TRANSPOSE(op)
        elif op_code_str in "FULLY_CONNECTED":
            self.layer.append(self._convert_FULLY_CONNECTED(op))
        elif op_code_str in SKIP_OPs:
            pass
        else:
            raise NotImplementedError(f"Unsupported {op_code_str}")

    def _get_np_from_wrapper(self, wrapper):
        if wrapper.tensor.Type() == TensorType.INT8:
            dtype = np.int8
        elif wrapper.tensor.Type() == TensorType.INT32:
            dtype = np.int32
        else:
            raise NotImplementedError("Current implementation only supports int8 and int32")

        data = wrapper.buffer.DataAsNumpy()
        shape = wrapper.tensor.ShapeAsNumpy() if wrapper.tensor.ShapeLength() != 0 else []

        return np.frombuffer(data, dtype=dtype).reshape(shape)

    def _get_tensor_type_str(self, tensor_type):
        if tensor_type == TensorType.INT8:
            return "int8"
        raise NotImplementedError(f"Tensor type: {tensor_type} is not supported yet.")

    def _get_input_tensors(self, op):
        return self._get_wrapper_tensors(op.InputsAsNumpy())

    def _get_output_tensors(self, op):
        return self._get_wrapper_tensors(op.OutputsAsNumpy())

    def _get_wrapper_tensors(self, tensor_index_list):
        ret = []
        for idx in tensor_index_list:
            tensor = self.subgraph.Tensors(idx)
            buffer_idx = tensor.Buffer()
            buffer = self.model.Buffers(buffer_idx)

            tflite_qparams = tensor.Quantization()

            if tflite_qparams is None:
                continue
            assert tflite_qparams, "Quantization parameters not found in the model!"

            scale = tflite_qparams.ScaleAsNumpy()
            zero_point = tflite_qparams.ZeroPointAsNumpy()
            qparams_to_tensor_wrapper = None

            if isinstance(zero_point, np.ndarray):
                # Per-channel quantization
                if scale.size != 1 and zero_point.size != 1:
                    qparams_to_tensor_wrapper = {"scale": scale, "zero_point": zero_point}
                # Per-tensor quantization
                elif scale.size == 1 and zero_point.size == 1:
                    qparams_to_tensor_wrapper = {"scale": float(scale[0]), "zero_point": int(zero_point[0])}
                else:
                    raise NotImplementedError
            elif scale == zero_point == 0:
                pass

            ret.append(TFLiteTensorWrpper(idx, tensor, buffer, qparams_to_tensor_wrapper))
        return ret

    def _getLayerMultiplierShift(self, effective_scale):
        if effective_scale == 0:
            significand = 0
            shift = 0
        else:
            sig, shi = math.frexp(effective_scale)
            sig = int(round(sig * 2**31))

            if sig == 2**31:
                sig /= 2
                shi += 1
            if shi < -31:
                shi = 0
                sig = 0

            significand = sig
            shift = shi

        return significand, shift


class PAD_tensorIndice(object):
    def __init__(self, input_idx, output_idx):
        self.input_idx = input_idx
        self.output_idx = output_idx


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


class TFLiteTensorWrpper:
    def __init__(self, tensor_idx, tensor, buffer, qnn_params):
        self.tensor_idx = tensor_idx
        self.tensor = tensor
        self.buffer = buffer
        self.qnn_params = qnn_params


def get_hwc_from_chwshape(shape):
    h = 1
    w = 1
    c = 1
    if len(shape) == 4:
        c = shape[1]
        h = shape[2]
        w = shape[3]
    elif len(shape) == 3:
        c = shape[1]
        h = shape[2]
    elif len(shape) == 2:
        c = shape[1]
    return h, w, c


def get_hwc_from_shape(shape):
    h = 1
    w = 1
    c = 1
    if len(shape) == 4:
        c = shape[3]
        h = shape[2]
        w = shape[1]
    elif len(shape) == 3:
        c = shape[2]
        h = shape[1]
    elif len(shape) == 2:
        c = shape[1]
    elif len(shape) == 1:
        c = shape[0]
    return h, w, c
