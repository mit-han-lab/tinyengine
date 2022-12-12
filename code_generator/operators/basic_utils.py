# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   basic_utils.py
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
from copy import deepcopy

import numpy as np

__all__ = [
    "basicOperator",
    "tensor",
    "overwrite_dicts",
    "deep_copy_dicts",
    "isweightstr",
    "isconstanttstr",
    "islabelstr",
    "isParamstr",
]
FORCE_DTYPE = None


class basicOperator:
    """Abstrct of operators.

    Child opeartors should
    1. Maintain hyperparameters, trainable parameters of the operator
    2. Generate inference string for this operator
    3. Be easy to add support of different precision levels
    4. Generate the corresponding kernel code (optional)
    """

    def __init__(self) -> None:
        self.input_tensors = []
        self.output_tensors = []
        if not hasattr(self, "params"):
            self.params = {}

    def generate_inference_str(self) -> str:
        raise NotImplementedError

    def _op_hparam_info(self):
        string = ""
        if "input_c" in self.params:
            string += f"r{self.params['input_h']}x{self.params['input_w']}x{self.params['input_c']}"
        elif "input_size" in self.params:
            string += f"size_{self.params['input_size']}"
        elif "matA_row" in self.params:
            string += f"r{self.params['matA_row']}x{self.params['matA_col']}"
        if "output_c" in self.params:
            string += f"_{self.params['output_h']}x{self.params['output_w']}x{self.params['output_c']}"
        elif "matB_row" in self.params and "matB_col" in self.params:
            string += f"_{self.params['matB_row']}x{self.params['matB_col']}"

        if len(string) == 0:
            string = "no_info"

        return string

    def _op_string(self) -> str:
        return self.params["op"]

    def get_macs(self) -> int:
        return 0

    def get_weights_size(self) -> int:
        return 0

    def get_bias_size(self) -> int:
        return 0

    def get_scale_size(self) -> int:
        return 0

    def get_activation_size(self) -> int:
        size = 0
        for t in self.input_tensors:
            size += t.len()
        for t in self.output_tensors:
            size += t.len()
        return size

    def get_sbuf_size(self) -> int:
        return 0

    def get_kbuf_size(self) -> int:
        return 0

    def generate_profiling_str(self, *args) -> str:
        if len(args) == 0:
            inf_str = self.generate_inference_str()
        else:
            inf_str = self.generate_inference_str(*args)
        op_str = self._op_string() + " " + self._op_hparam_info() + " " + str(self.get_macs()) + " "
        string = (
            """start = HAL_GetTick();
for(profile_i = 0; profile_i < RUNS; profile_i++) {
    """
            + inf_str
            + """}
end = HAL_GetTick();
sprintf(buf, \""""
            + op_str
            + """%.2f\\r\\n", (float)(end - start)/(float)RUNS);printLog(buf);
"""
        )
        return string

    def _add_input(self, graph_idx, dtype, *dims):
        self.input_tensors.append(tensor(graph_idx, dtype, dims))

    def _add_output(self, graph_idx, dtype, *dims):
        self.output_tensors.append(tensor(graph_idx, dtype, dims))

    def get_layer_info(self):
        return self.params

    def change_output_tensor_idx(self, idx):
        if hasattr(self, "params"):
            self.params["output_idx"] = idx
        self.output_tensors[0].graph_idx = idx

    def _getBufferstr(self, location, offset):
        if location == "front":
            return f"&buffer0[{offset}]"
        elif location == "end":
            return f"&buffer0[{offset}]"
        elif location == "residual":
            return f"&buffer1[{offset}]"
        elif location.startswith("stage"):
            return f"&buffer{location}[{offset}]"
        elif location.startswith("dagop"):
            return f"&buffer{location}[{offset}]"
        else:
            raise NotImplementedError

    def _getBufferstrCast(self, location, offset, dtype="float32"):
        ret = ""

        cast_type = "float"
        if dtype == "int8":
            cast_type = "signed char"
        elif dtype == "bool":
            cast_type = "bool"
        elif dtype == "int32":
            cast_type = "int32_t"
        cast_str = f"({cast_type}*)"

        ret += cast_str

        if location == "front":
            ret += f"&buffer0[{offset}]"
        elif location == "end":
            ret += f"&buffer0[{offset}]"
        elif location == "residual":
            ret += f"&buffer1[{offset}]"
        elif location.startswith("stage"):
            ret += f"&buffer{location}[{offset}]"
        elif location.startswith("dagop"):
            ret += f"&buffer{location}[{offset}]"
        elif "weight" in location:
            ret += f"{location}"
        elif "bias" in location:
            ret += f"{location}"
        else:
            raise NotImplementedError

        return ret


class tensor:
    byte_size = {
        "bool": 1,
        "int8": 1,
        "int32": 4,
        "fp32": 4,
        "float32": 4,
    }

    def __init__(self, graph_idx, dtype, dims) -> None:
        size_list = []
        for dim in dims:
            try:
                dim = int(dim)
            except ValueError:
                raise ValueError("dimension should be int and positive")
            if not isinstance(dim, int) or dim <= 0:
                raise ValueError("dimension should be int and positive")
            size_list.append(dim)
        self.size = tuple(size_list)
        self.dtype = dtype
        if FORCE_DTYPE is not None:
            self.dtype = FORCE_DTYPE
        self.buffer_placement = None
        self.buffer_name = None
        self.buffer_address = None
        self.allocator_idx = None
        self.graph_idx = str(graph_idx)

    def input_c(self):
        return self.size[0]

    def input_w(self):
        return self.size[1]

    def input_h(self):
        return self.size[2]

    def set_input_w(self, w):
        self.size = (self.size[0], w, self.size[2])

    def set_input_h(self, h):
        self.size = (self.size[0], self.size[1], h)

    def len(self):
        byte_cnt = math.ceil(np.prod(self.size) * self.byte_size[self.dtype])
        # align the memory to 4
        byte_cnt = math.ceil(byte_cnt / 4) * 4
        return byte_cnt


def overwrite_dicts(src, dst):
    for k in dst:
        if k in src:
            src[k] = dst[k]
        else:
            pass
            # warnings.warn(f"given key:{k} is not used in src dict")


def deep_copy_dicts(dst):
    return deepcopy(dst)


def get_dtype(tensor_info):
    if "dtype" in tensor_info:
        return tensor_info["dtype"]
    else:
        return "int8"


def isweightstr(str):
    if "weight" in str:
        return True
    return False


def isconstanttstr(str):
    if "constant" in str:
        return True
    return False


def islabelstr(str):
    if "label" in str:
        return True
    return False


def isParamstr(str):
    if "scale" in str or "weight" in str or "bias" in str:
        return True
    return False
