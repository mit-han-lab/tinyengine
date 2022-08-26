# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   conv2d.py
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

__all__ = ["Conv2d"]

default_params = {
    # op related
    "op": "CONV_2D",
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
    "kernel_h": None,
    "kernel_w": None,
    "input_dtype": "int8",
    "output_dtype": "int8",
    "padding": None,
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


class Conv2d(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
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
        string = f" k{self.params['kernel_h']}x{self.params['kernel_w']}_r{self.params['input_h']}"
        string += f"x{self.params['input_w']}x{self.params['input_c']}_{self.params['output_h']}"
        string += f"x{self.params['output_w']}x{self.params['output_c']}"
        return string

    def set_input_zero_point(self, zero_x):
        p = self.params
        p["input_zero_point"] = zero_x

    def set_output_zero_point(self, zero_y):
        p = self.params
        p["input_zero_point"] = zero_y

    def get_macs(self):
        p = self.params
        return p["output_h"] * p["output_w"] * p["kernel_h"] * p["kernel_w"] * p["input_c"] * p["output_c"]

    def get_weights_size(self) -> int:
        p = self.params
        if p["input_dtype"] in {"float32", "fp32"}:
            size = 4
        else:
            size = 1
        return p["kernel_h"] * p["kernel_w"] * p["input_c"] * p["output_c"] * size

    def get_bias_size(self) -> int:
        p = self.params
        return 4 * p["output_c"]

    def get_scale_size(self) -> int:
        p = self.params
        return 4 * p["output_c"]

    def get_sbuf_size(self) -> int:
        params = self.params
        if params["input_dtype"] == params["output_dtype"] == "int8":
            p = self.params
            return p["kernel_h"] * p["kernel_w"] * p["input_c"] * 2 * 2  # 2 col and 16 bit
        else:
            return 0

    def get_kbuf_size(self) -> int:
        p = self.params
        if p["kernel_h"] == 1:
            return 0
        else:
            return self.get_weights_size() * 2  # 16bit

    def generate_inference_str(
        self,
        unsigned_input: bool = False,
        FP_output: bool = False,
        use_aggressive_unroll: bool = False,
        use_hard_switsh: bool = False,
        fp_requantize: bool = False,
        tflite_op: bool = False,
        dummy_address: bool = False,
    ):
        string = ""
        params = self.params
        # floating point implmenetation
        kernel_h = params["kernel_h"]
        # function name
        if params["kernel_h"] == 1:
            # find the proper function
            if params["output_c"] % 2 != 0:
                if (
                    FP_output
                    and "effective_scale" in params
                    and params["output_scale"] is not None
                    and params["effective_scale"] is not None
                ):
                    function_name = "convolve_1x1_s8_oddch_fp"
                else:
                    function_name = "convolve_1x1_s8_oddch"
            else:
                if use_aggressive_unroll and params["input_c"] in [8, 16, 24, 48]:
                    function_name = f"convolve_1x1_s8_ch{str(params['input_c'])}"
                else:
                    if (
                        FP_output
                        and "effective_scale" in params
                        and params["output_scale"] is not None
                        and params["effective_scale"] is not None
                    ):
                        function_name = "convolve_1x1_s8_fp"
                    else:
                        function_name = "convolve_1x1_s8"
        elif kernel_h == 3 and params["stride_h"] == 2 and params["padding"] == 1:
            if unsigned_input:
                function_name = "convolve_u8_kernel3_inputch3_stride2_pad1"
            else:
                if "is_patch" in params and params["is_patch"]:
                    function_name = "patchpadding_convolve_s8_kernel3_inputch3_stride2"
                else:
                    function_name = "convolve_s8_kernel3_inputch3_stride2_pad1"
        elif kernel_h == 3 and params["stride_h"] == 1 and params["padding"] == 1:
            function_name = "convolve_s8_kernel3_stride1_pad1"
        else:
            raise NotImplementedError

        if fp_requantize:
            function_name += "_fpreq"

        # input tensor, shape, weight, bias
        if unsigned_input:
            string += f"{function_name}((const q8_t *)"
            string += f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
        else:
            string += f"{function_name}({self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
        string += f"{str(params['input_w'])},{str(params['input_h'])},{str(params['input_c'])},"
        parsed_idx = str(params["parsed_trainable"])
        string += f"(const q7_t*) weight{parsed_idx},bias{parsed_idx},"

        # scales or multiplier and shift
        if (
            fp_requantize
            or FP_output
            and "effective_scale" in params
            and params["output_scale"] is not None
            and params["effective_scale"] is not None
        ):
            string += f"scales{parsed_idx},"
        else:
            string += f"shift{parsed_idx},multiplier{parsed_idx},"

        # output: zero point, min, max, output tensor, shape
        string += f"{str(params['output_zero_point'])},{str(params['input_zero_point'] * -1)},-128,127,"
        string += f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])},"
        string += f"{str(params['output_w'])},{str(params['output_h'])},{str(params['output_c'])},"

        # intemediate buffers
        string += "sbuf"
        if (
            kernel_h == 3
            and params["stride_h"] == 2
            and params["padding"] == 1
            and not ("is_patch" in params and params["is_patch"])
        ):
            string += ",kbuf"

        # pad value for kernel size > 1
        if kernel_h > 1:
            string += f",{str(params['input_zero_point'])}"

        # patch-based parameters
        if "is_patch" in params and params["is_patch"] and (params["kernel_h"] > 1 or params["kernel_w"] > 1):
            string += ", pad_t, pad_b, pad_l, pad_r);\n"
            stride_string = str(params["stride_h"])
            string += f"pad_t /= {stride_string};pad_b /= {stride_string};pad_l /= {stride_string};"
            string += f"pad_r /= {stride_string};\n"
        else:
            string += ");\n"

        if use_hard_switsh:
            string = (
                "input_shape.Resize(4); ptr = input_shape.DimsData();\n"
                + f"ptr[0] = 1;ptr[1] = {str(params['output_h'])};ptr[2] = {str(params['output_w'])};"
                + f"ptr[3] = {str(params['output_c'])};\n"
            )
            string += (
                "output_shape.Resize(4); ptr = output_shape.DimsData();\n"
                + f"ptr[0] = 1;ptr[1] = {str(params['output_h'])};ptr[2] = {str(params['output_w'])};"
                + f"ptr[3] = {str(params['output_c'])};\n"
            )
            string += (
                "tflite::reference_ops::HardSwish<int8_t>(hs_op_params, input_shape, "
                + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])},"
                + f" output_shape, {self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])});\n"
            )

        return string
