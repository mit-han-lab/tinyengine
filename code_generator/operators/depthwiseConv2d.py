# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   depthwiseConv2d.py
#
# Reference papers:
#  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
#  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
#  - MCUNetV3: On-Device Training Under 256KB Memory, arXiv:2206.15472
# Contact authors:
#  - Wei-Ming Chen, wmchen@mit.edu
#  - Wei-Chen Wang, wweichen@mit.edu
#  - Ji Lin, jilin@mit.edu
#  - Ligeng Zhu, ligeng@mit.edu
#  - Song Han, songhan@mit.edu
#
# Target ISA:  ARMv7E-M
# ----------------------------------------------------------------------

import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["DepthwiseConv2d"]

default_params = {
    # op related
    "op": "DEPTHWISE_CONV_2D",
    "is_SEBlock": False,
    "padding": None,
    "stride_h": None,
    "stride_w": None,
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "output_dim": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "dtypte": "int8",
    "kernel_h": None,
    "kernel_w": None,
    "input_dtype": "int8",
    "output_dtype": "int8",
    # quantization related
    "weight_value": None,
    "weight_name": None,
    "bias": None,
    "bias_name": None,
    "effective_scale": None,
    "input_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "output_scale": None,
    "weight_scale": None,
    "multiplier": None,
    "shift": None,
}


class DepthwiseConv2d(basicOperator):
    def __init__(self, params: dict, USE_INPLACE: bool = True) -> None:
        self.params = deep_copy_dicts(default_params)
        self.USE_INPLACE = USE_INPLACE
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["input_c"],
            self.params["input_w"],
            self.params["input_h"],
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["output_c"],
            self.params["output_w"],
            self.params["output_h"],
        )

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def _op_hparam_info(self) -> str:
        string = (
            f" k{self.params['kernel_h']}x{self.params['kernel_w']}_r{self.params['input_h']}x"
            + f"{self.params['input_w']}x{self.params['input_c']}_{self.params['output_h']}x"
            + f"{self.params['output_w']}x{self.params['output_c']}"
        )

        return string

    def get_macs(self):
        p = self.params
        return p["output_h"] * p["output_w"] * p["kernel_h"] * p["kernel_w"] * p["input_c"]

    def get_activation_size(self) -> int:
        size = 0
        for t in self.input_tensors:
            size += t.len()
        return size

    def get_weights_size(self) -> int:
        p = self.params
        return p["kernel_h"] * p["kernel_w"] * p["input_c"]

    def get_bias_size(self) -> int:
        p = self.params
        return 4 * p["output_c"]

    def get_scale_size(self) -> int:
        p = self.params
        return 4 * p["output_c"]

    def get_sbuf_size(self) -> int:
        p = self.params
        if self.USE_INPLACE:
            return 2 * (p["input_h"] + 2 * p["padding"]) * (p["input_w"] + 2 * p["padding"])  # 2 x resolution
        else:
            return 2 * p["kernel_h"] * p["kernel_w"] * p["input_c"]  # 16 bit

    def get_kbuf_size(self) -> int:
        return 0

    def generate_inference_str(
        self,
        fp_requantize: bool = False,
    ):
        string = ""
        params = self.params
        # function name
        if "is_patch" in params and params["is_patch"]:
            function_name = "patchpadding_depthwise_kernel"
        else:
            function_name = "depthwise_kernel"

        function_name += (
            f"{str(params['kernel_h'])}x{str(params['kernel_w'])}_stride{str(params['stride_h'])}_inplace_CHW"
        )

        if fp_requantize:
            function_name += "_fpreq"

        # input tensor, shape, weight, bias
        string += f"{function_name}({self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
        string += f"{str(params['input_w'])},{str(params['input_h'])},{str(params['input_c'])},"
        parsed_idx = str(params["parsed_trainable"])
        string += f"(const q7_t*) CHWweight{parsed_idx},offsetBias{parsed_idx},offsetRBias{parsed_idx},"

        # scales or multiplier and shift
        if fp_requantize:
            string += f"scales{parsed_idx},"
        else:
            string += f"shift{parsed_idx},multiplier{parsed_idx},"

        # output: zero point, min, max, output tensor, shape
        string += f"{str(params['output_zero_point'])},{str(params['input_zero_point'] * -1)},-128,127,"
        string += (
            f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])},"
            + f"{str(params['output_w'])},{str(params['output_h'])},{str(params['output_c'])},"
        )

        # intemediate buffers
        string += "sbuf,"

        # padding value
        string += f"{str(params['input_zero_point'])}"

        # patch-based parameters
        if "is_patch" in params and params["is_patch"]:
            string += ", pad_t, pad_b, pad_l, pad_r);\n"
            if params["stride_h"] != 1:
                stride_string = str(params["stride_h"])
                string += (
                    f"pad_t /= {stride_string};pad_b /= {stride_string};"
                    + "pad_l /= {stride_string};pad_r /= {stride_string};\n"
                )
            else:
                string += (
                    "pad_t = MAX(0, pad_t-1);pad_b = MAX(0, pad_b-1);pad_l = MAX(0, pad_l-1);pad_r = MAX(0, pad_r-1);\n"
                )
        else:
            string += ");\n"

        return string
