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

import logging
import math

import numpy as np

import code_generator.converters.tflite_parser as TF_Parser

from .constant import SKIP_OPs
from .tflite import Model
from .tflite.BuiltinOperator import BuiltinOperator
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
                    logging.info("found SE block")
                    skip_next_ops = 2
                    #         -> MEAN -> MEAN -> PWCONV -> PWCONV -> | ADD -> MUL ->     |
                    #  DWCONV                                        |            -> MUL |
                    #                                                |   SEelementmult   |
                    SEelementmult_op = TF_Parser.parse_SEelement(three_op_sequence, self.model, self.layer)

                    self.layer.append(SEelementmult_op)
                    continue

            # parse the op
            self._handleOperator(op)

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

    # handle one op and parse it into layers[] for supported operators
    def _handleOperator(self, op):
        op_code_str = self._getOpCodeStr(op)
        if op_code_str == "CONV_2D":
            self.layer.append(TF_Parser.parse_conv2d(op, self.model, self.tmpPADIndice))
            self.tmpPADIndice = None
        elif op_code_str == "ADD":
            self.layer.append(TF_Parser.parse_add(op, self.model))
        elif op_code_str == "AVERAGE_POOL_2D":
            self.layer.append(TF_Parser.parse_avgpool(op, self.model))
        elif op_code_str == "DEPTHWISE_CONV_2D":
            self.layer.append(TF_Parser.parse_conv2d(op, self.model, self.tmpPADIndice))
            self.tmpPADIndice = None
        elif op_code_str == "PAD":
            self._convert_PAD(op)
        elif op_code_str == "RESIZE_NEAREST_NEIGHBOR":
            self.layer.append(TF_Parser.parse_upsample(op, self.model))
        elif op_code_str == "MAX_POOL_2D":
            self.layer.append(TF_Parser.parse_maxpool(op, self.model))
        elif op_code_str in "MEAN":
            ret_op = TF_Parser.parse_mead1dto2d(op, self.model, self.average_1D_to_2D_holder)
            if ret_op is not None:
                # TODO: This only handle a specific graph: TRANSPOSE -> MEAN -> MEANS
                if self.skip_transpose is not None:
                    ret_op.params["input_idx"] = self.skip_transpose.input_idx
                    ret_op.input_tensors[0].graph_idx = self.skip_transpose.input_idx
                self.layer.append(ret_op)
        elif op_code_str == "TRANSPOSE":
            self._convert_TRANSPOSE(op)
        elif op_code_str in "FULLY_CONNECTED":
            self.layer.append(TF_Parser.parse_fc(op, self.model))
        elif op_code_str in SKIP_OPs:
            pass
        else:
            raise NotImplementedError(f"Unsupported {op_code_str}")

    def _get_np_from_wrapper(self, wrapper):
        if wrapper.tensor.Type() == TensorType.INT8:
            dtype = np.int8
        elif wrapper.tensor.Type() == TensorType.INT32:
            dtype = np.int32
        elif wrapper.tensor.Type() == TensorType.FLOAT32:
            dtype = np.float32
            logging.warn("Support of floating-point tensors are experimental.")
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

            qparams_to_tensor_wrapper = None
            if tflite_qparams:
                scale = tflite_qparams.ScaleAsNumpy()
                zero_point = tflite_qparams.ZeroPointAsNumpy()
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
            else:
                logging.warn(
                    "Quantization parameters not found in the model! Floating-point opeartos are experimental."
                )

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


def get_nhwc_from_shape(shape):
    n = 1
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
        w = shape[1]
        c = shape[0]
    elif len(shape) == 1:
        c = shape[0]
    return n, h, w, c
