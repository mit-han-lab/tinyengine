import logging
import math

import numpy as np

from code_generator.tflite import Model
from code_generator.tflite.BuiltinOperator import BuiltinOperator
from code_generator.tflite.TensorType import TensorType


class TFLiteTensorWrpper:
    def __init__(self, tensor_idx, tensor, buffer, qnn_params):
        self.tensor_idx = tensor_idx
        self.tensor = tensor
        self.buffer = buffer
        self.qnn_params = qnn_params


def getOpCodeStr(op, model: Model.Model):
    op_code_list_idx = op.OpcodeIndex()
    op_code_id = model.OperatorCodes(op_code_list_idx).DeprecatedBuiltinCode()

    def _build_str_map(obj):
        ret = {}
        for field_name in dir(obj):
            if not field_name.startswith("_"):
                field_value = getattr(obj, field_name)
                if isinstance(field_value, int):
                    ret[field_value] = field_name
        return ret

    builtin_op_code = _build_str_map(BuiltinOperator())

    return builtin_op_code[op_code_id]


def get_np_from_wrapper(wrapper):
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


def get_input_tensors(op, model: Model.Model):
    return _get_wrapper_tensors(op.InputsAsNumpy(), model)


def get_output_tensors(op, model: Model.Model):
    return _get_wrapper_tensors(op.OutputsAsNumpy(), model)


def _get_wrapper_tensors(tensor_index_list, model: Model.Model):
    ret = []
    subgraph = model.Subgraphs(0)
    for idx in tensor_index_list:
        tensor = subgraph.Tensors(idx)
        buffer_idx = tensor.Buffer()
        buffer = model.Buffers(buffer_idx)

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
            logging.warn("Quantization parameters not found in the model! Floating-point opeartos are experimental.")

        ret.append(TFLiteTensorWrpper(idx, tensor, buffer, qparams_to_tensor_wrapper))
    return ret


def getLayerMultiplierShift(effective_scale):
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


def getMultiplierShift(effective_scale):
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


def getTensorTypeStr(type):
    if TensorType.INT8 == type:
        return "int8"
    if TensorType.UINT8 == type:
        return "uint8"
    if TensorType.FLOAT32 == type:
        return "float32"


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


def get_nhwc_from_shape(shape):
    n = 1
    h = 1
    w = 1
    c = 1
    if len(shape) == 4:
        c = shape[3]
        w = shape[2]
        h = shape[1]
    elif len(shape) == 3:
        c = shape[2]
        w = shape[1]
        h = shape[0]
    elif len(shape) == 2:
        c = shape[1]
        w = shape[0]
    elif len(shape) == 1:
        c = shape[0]
    return n, h, w, c
