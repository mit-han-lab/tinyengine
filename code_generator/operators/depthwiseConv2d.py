import warnings

from ..constant import USE_BIT_MASK, USE_TTE_INT8
from .basic_utils import basicOperator, deep_copy_dicts, isweightstr, overwrite_dicts

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
    # for fp implementation
    "padding_h": None,
    "padding_w": None,
    "dilation_h": None,
    "dilation_w": None,
    "float_max": "FLT_MAX",
    "float_min": "-FLT_MAX",
    # fof Q training
    "need_Bmask": False,
    "output2_h": None,
    "output2_w": None,
    "output2_c": None,
    "output2_idx": None,
    "output2_dtype": "int8",
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
        return (
            f" k{self.params['kernel_h']}x{self.params['kernel_w']}_r{self.params['input_h']}x"
            + f"{self.params['input_w']}x{self.params['input_c']}_{self.params['output_h']}x"
            + f"{self.params['output_w']}x{self.params['output_c']}"
        )

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

    def generate_inference_str(self, fp_requantize: bool = False, tflite_op: bool = False, dummy_address: bool = False):
        string = ""
        params = self.params
        # floating point implmenetation
        if params["input_dtype"] == params["output_dtype"] == "float32":
            string += f"dpconv_params.stride_height = {params['stride_h']};\n"
            string += f"dpconv_params.stride_width = {params['stride_w']};\n"
            string += f"dpconv_params.dilation_width_factor = {params['dilation_w']};\n"
            string += f"dpconv_params.dilation_height_factor = {params['dilation_h']};\n"
            string += f"dpconv_params.input_offset = {params['input_zero_point']};\n"
            string += f"dpconv_params.output_offset = {params['output_zero_point']};\n"
            string += f"dpconv_params.padding_values.width = {params['padding_w']};\n"
            string += f"dpconv_params.padding_values.height = {params['padding_h']};\n"
            string += "dpconv_params.quantized_activation_max = 127;\n"
            string += "dpconv_params.quantized_activation_min = -128;\n"
            string += "dpconv_params.depth_multiplier = 1;\n"
            string += f"dpconv_params.float_activation_min = {params['float_min']};\n"
            string += f"dpconv_params.float_activation_max = {params['float_max']};\n"

            if isinstance(params["weight_name"], str) and isweightstr(params["weight_name"]):
                weight_string = params["weight_name"]
            else:
                weight_string = f"weight_fp{params['parsed_trainable']}"
            string += (
                "DepthwiseConv(dpconv_params,"
                + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                + f"{weight_string},{params['kernel_h']},{params['kernel_w']},bias_fp{params['parsed_trainable']},"
                + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},(float*)sbuf,1);\n"
            )
        elif params["input_dtype"] == params["output_dtype"] == "int8" and tflite_op and (not USE_TTE_INT8):
            string += f"dpconv_params.stride_height = {params['stride_h']};\n"
            string += f"dpconv_params.stride_width = {params['stride_w']};\n"
            string += "dpconv_params.dilation_width_factor = 1;\n"
            string += "dpconv_params.dilation_height_factor = 1;\n"
            string += f"dpconv_params.input_offset = {params['input_zero_point']};\n"
            string += f"dpconv_params.output_offset = {params['output_zero_point']};\n"
            string += f"dpconv_params.padding_values.width = {params['padding_w']};\n"
            string += f"dpconv_params.padding_values.height = {params['padding_h']};\n"
            string += "dpconv_params.quantized_activation_min = -128;\n"
            string += "dpconv_params.quantized_activation_max = 127;\n"
            string += "dpconv_params.depth_multiplier = 1;\n"
            string += f"dpconv_params.float_activation_min = {params['float_min']};\n"
            string += f"dpconv_params.float_activation_max = {params['float_max']};\n"

            parsed_idx = str(params["parsed_trainable"])

            if dummy_address:
                string += (
                    f"TFLite_DepthwiseConv_int8_PerChannel(dpconv_params,multiplier{parsed_idx},shift{parsed_idx},"
                    + f"&buffer0[0],{params['input_h']},{params['input_w']},{params['input_c']},"
                    # TODO: bias data might be incorrect for now
                    + f"(const int8_t*) weight{parsed_idx},{params['kernel_h']},"
                    + f"{params['kernel_w']},offsetBias{parsed_idx},"
                    + "&buffer0[0],"
                    + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},1);\n"
                )
            else:
                string += (
                    f"TFLite_DepthwiseConv_int8_PerChannel(dpconv_params,multiplier{parsed_idx},shift{parsed_idx},"
                    + f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                    # TODO: bias data might be incorrect for now
                    + f"(const int8_t*) weight{parsed_idx},{params['kernel_h']},"
                    + f"{params['kernel_w']},offsetBias{parsed_idx},"
                    + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},1);\n"
                )
        else:
            # function name
            if "is_patch" in params and params["is_patch"]:
                function_name = "patchpadding_depthwise_kernel"
            else:
                function_name = "depthwise_kernel"

            function_name += (
                f"{str(params['kernel_h'])}x{str(params['kernel_w'])}_stride{str(params['stride_h'])}_inplace_CHW"
            )

            if fp_requantize and not ("is_patch" in params and params["is_patch"]):
                function_name += "_fpreq"

            if params["need_Bmask"]:
                if USE_BIT_MASK:
                    function_name += "_bitmask"
                else:
                    function_name += "_mask"

            # input tensor, shape, weight, bias
            string += f"{function_name}({self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
            string += f"{str(params['input_w'])},{str(params['input_h'])},{str(params['input_c'])},"
            parsed_idx = str(params["parsed_trainable"])
            string += f"(const q7_t*) CHWweight{parsed_idx},offsetBias{parsed_idx},offsetRBias{parsed_idx},"

            # scales or multiplier and shift
            if fp_requantize and not ("is_patch" in params and params["is_patch"]):
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
            string += "sbuf,"

            # padding value
            string += f"{str(params['input_zero_point'])}"

            # patch-based parameters
            if "is_patch" in params and params["is_patch"]:
                string += ", pad_t, pad_b, pad_l, pad_r);\n"
                if params["stride_h"] != 1:
                    stride_string = str(params["stride_h"])
                    string += (
                        f"pad_t /= {stride_string};pad_b /= {stride_string};pad_l /= {stride_string};"
                        + f"pad_r /= {stride_string};\n"
                    )
                else:
                    string += (
                        "pad_t = TN_MAX(0, pad_t-1);pad_b = TN_MAX(0, pad_b-1);pad_l = TN_MAX(0, pad_l-1);"
                        + "pad_r = TN_MAX(0, pad_r-1);\n"
                    )
            else:
                string += ");\n"

        return string
