import warnings

from ..constant import USE_BIT_MASK, USE_TTE_INT8
from .basic_utils import basicOperator, deep_copy_dicts, isweightstr, overwrite_dicts

__all__ = ["Conv2d"]

# USE_FP_REQ = True

default_params = {
    # op related
    "op": "CONV_2D",
    "is_SEBlock": False,
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
    # for fp implementation
    "float_max": "FLT_MAX",
    "float_min": "-FLT_MAX",
    "padding": None,
    "padding_h": None,
    "padding_w": None,
    "dilation_h": None,
    "dilation_w": None,
    # fof Q training
    "need_Bmask": False,
    "output2_h": None,
    "output2_w": None,
    "output2_c": None,
    "output2_idx": None,
    "output2_dtype": "int8",
    # for partial channel update
    "first_k_channel": None,
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
        return (
            f" k{self.params['kernel_h']}x{self.params['kernel_w']}_r{self.params['input_h']}x"
            + f"{self.params['input_w']}x{self.params['input_c']}_{self.params['output_h']}x"
            + f"{self.params['output_w']}x{self.params['output_c']}"
        )

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
        if params["input_dtype"] == params["output_dtype"] == "float32":
            string += f"conv_params.stride_height = {params['stride_h']};\n"
            string += f"conv_params.stride_width = {params['stride_w']};\n"
            string += f"conv_params.dilation_width_factor = {params['dilation_w']};\n"
            string += f"conv_params.dilation_height_factor = {params['dilation_h']};\n"
            string += f"conv_params.input_offset = {params['input_zero_point']};\n"
            string += f"conv_params.output_offset = {params['output_zero_point']};\n"
            string += f"conv_params.padding_values.width = {params['padding_w']};\n"
            string += f"conv_params.padding_values.height = {params['padding_h']};\n"
            string += "conv_params.quantized_activation_min = -128;\n"
            string += "conv_params.quantized_activation_max = 127;\n"
            string += f"conv_params.float_activation_min = {params['float_min']};\n"
            string += f"conv_params.float_activation_max = {params['float_max']};\n"

            if isinstance(params["weight_name"], str) and isweightstr(params["weight_name"]):
                weight_string = params["weight_name"]
            else:
                weight_string = f"weight_fp{params['parsed_trainable']}"

            string += (
                "TFLite_Conv_fp(conv_params,"
                + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                + f"{weight_string},{params['kernel_h']},{params['kernel_w']},{params['input_c']},NULL,"
                + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},(float*)sbuf,1);\n"
            )
        elif params["input_dtype"] == params["output_dtype"] == "int8" and tflite_op and (not USE_TTE_INT8):
            string += f"conv_params.stride_height = {params['stride_h']};\n"
            string += f"conv_params.stride_width = {params['stride_w']};\n"
            string += "conv_params.dilation_width_factor = 1;\n"
            string += "conv_params.dilation_height_factor = 1;\n"
            string += f"conv_params.input_offset = {params['input_zero_point']};\n"
            string += f"conv_params.output_offset = {params['output_zero_point']};\n"
            string += f"conv_params.padding_values.width = {params['padding_w']};\n"
            string += f"conv_params.padding_values.height = {params['padding_h']};\n"
            string += "conv_params.quantized_activation_min = -128;\n"
            string += "conv_params.quantized_activation_max = 127;\n"
            string += f"conv_params.float_activation_min = {params['float_min']};\n"
            string += f"conv_params.float_activation_max = {params['float_max']};\n"

            parsed_idx = str(params["parsed_trainable"])

            function_name = "TFLite_Conv_int8_PerChannel"
            if params["first_k_channel"] is not None:  # partial channels in SRAM,
                function_name += "_partialCH"
                weight_string = (
                    f"(const q7_t*)weight{parsed_idx},(const q7_t*)weight{parsed_idx}Flash,{params['first_k_channel']}"
                )
            else:
                weight_string = f"(const q7_t*) weight{parsed_idx}"

            if dummy_address:
                input_address_string = "&buffer0[0]"
                output_address_string = "&buffer0[0]"
            else:
                input_address_string = f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])}"
                output_address_string = (
                    f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])}"
                )

            string += (
                f"{function_name}(conv_params,multiplier{parsed_idx},shift{parsed_idx},"
                + f"{params['input_h']},{params['input_w']},{params['input_c']},{input_address_string},"
                + f"{weight_string},{params['kernel_h']},{params['kernel_w']},{params['input_c']},bias{parsed_idx},"
                + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                + f"{output_address_string},1);\n"
            )
        else:
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
                    if use_aggressive_unroll and params["input_c"] in [8, 16, 24, 48] and not params["need_Bmask"]:
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

            if fp_requantize and not ("is_patch" in params and params["is_patch"] and kernel_h > 1):
                function_name += "_fpreq"

            if params["need_Bmask"]:
                if USE_BIT_MASK:
                    function_name += "_bitmask"
                else:
                    function_name += "_mask"

            if params["first_k_channel"] is not None:
                function_name += "_partialCH"

            # input tensor, shape, weight, bias
            if unsigned_input:
                string += (
                    f"{function_name}((const q8_t *)"
                    + f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
                )
            else:
                string += (
                    f"{function_name}({self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
                )
            string += f"{str(params['input_w'])},{str(params['input_h'])},{str(params['input_c'])},"
            parsed_idx = str(params["parsed_trainable"])
            if params["first_k_channel"] is not None:  # partial channels in SRAM,
                string += (
                    f"(const q7_t*)weight{parsed_idx},"
                    + f"(const q7_t*)weight{parsed_idx}Flash,{params['first_k_channel']},bias{parsed_idx},"
                )
            else:
                string += f"(const q7_t*) weight{parsed_idx},bias{parsed_idx},"

            # scales or multiplier and shift
            if (
                fp_requantize
                and not ("is_patch" in params and params["is_patch"] and kernel_h > 1)
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
            if params["need_Bmask"]:
                string += (
                    f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{self._getBufferstr(params['output2_buf_add'], params['output2_buf_add_offset'])},"
                    + f"{str(params['output_w'])},{str(params['output_h'])},{str(params['output_c'])},"
                )
            else:
                string += (
                    f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{str(params['output_w'])},{str(params['output_h'])},{str(params['output_c'])},"
                )

            # intemediate buffers
            string += "sbuf"
            if kernel_h == 3 and params["stride_h"] == 2 and params["padding"] == 1:
                string += ",kbuf"

            # pad value for kernel size > 1
            if kernel_h > 1:
                string += f",{str(params['input_zero_point'])}"

            # patch-based parameters
            if "is_patch" in params and params["is_patch"] and (params["kernel_h"] > 1 or params["kernel_w"] > 1):
                string += ", pad_t, pad_b, pad_l, pad_r);\n"
                stride_string = str(params["stride_h"])
                string += (
                    f"pad_t /= {stride_string};pad_b /= {stride_string};"
                    + f"pad_l /= {stride_string};pad_r /= {stride_string};\n"
                )
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
                    + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])}, "
                    + "output_shape, "
                    + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                )

        return string
