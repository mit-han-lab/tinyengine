import warnings

from .basic_utils import basicOperator, deep_copy_dicts, isweightstr, overwrite_dicts

__all__ = ["transposeConv2d"]

# USE_FP_REQ = True

default_params = {
    # op related
    "op": "TRANSPOSE_CONV_2D",
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
    "input_dtype": "float32",
    "input2_dtype": "float32",
    "output_dtype": "int8",
    "norm_buffer": None,
    "float_to_int8": False,
    # quantization related
    "weight_value": None,
    "weight_name": None,
    "bias": None,
    "effective_scale": None,
    "input_zero_point": 0,
    "output_zero_point": 0,
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
    "kernel_layout": "OIHW",
    "group": None,
    # for partial channel
    "first_k_channel": None,
}


class transposeConv2d(basicOperator):
    conv_params_declared = False

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
            f"{self.params['input_w']}x{self.params['input_c']}_{self.params['output_h']}x"
            f"{self.params['output_w']}x{self.params['output_c']}"
        )

    def set_input_zero_point(self, zero_x):
        p = self.params
        p["input_zero_point"] = zero_x

    def set_output_zero_point(self, zero_y):
        p = self.params
        p["input_zero_point"] = zero_y

    def get_macs(self):
        p = self.params
        if p["group"] == p["input_c"] and p["group"] == p["output_c"]:
            return (
                p["input_h"] * p["input_w"] * p["kernel_h"] * p["kernel_w"] * p["input_c"] * p["output_c"] / p["group"]
            )
        else:
            return p["input_h"] * p["input_w"] * p["kernel_h"] * p["kernel_w"] * p["input_c"] * p["output_c"]

    def get_weights_size(self) -> int:
        p = self.params
        if p["input_dtype"] == "float32" or "fp32":
            size = 4
        else:
            size = 1
        if p["group"] == p["input_c"] and p["group"] == p["output_c"]:
            return p["kernel_h"] * p["kernel_w"] * p["input_c"] * size
        elif p["group"] == 1:
            return p["kernel_h"] * p["kernel_w"] * p["input_c"] * p["output_c"] * size
        else:
            raise NotImplementedError

    def get_bias_size(self) -> int:
        p = self.params
        return 4 * p["output_c"]

    def get_scale_size(self) -> int:
        p = self.params
        return 4 * p["output_c"]

    def get_sbuf_size(self) -> int:
        p = self.params
        if p["group"] == p["input_c"] and p["group"] == p["output_c"]:
            return (p["output_h"] + p["kernel_h"] - 1) * (p["output_w"] + p["kernel_w"] - 1) * 4  # floating point
        else:
            return 0

    def get_kbuf_size(self) -> int:
        return 0  # 16bit

    def add_int32_buffer_tensor(self):
        params = self.params
        if params["group"] == params["input_c"] and params["group"] == params["output_c"]:  # depthwise
            self._add_input(
                params["output_idx"] + "_buffer", "int32", params["output_c"], params["output_w"], params["output_h"]
            )
        else:
            self._add_input(params["output_idx"] + "_buffer", "int32", params["output_c"], 4, 4)

        params["norm_buffer"] = len(self.input_tensors) - 1

    def generate_inference_str(
        self,
        tflite_op: bool = False,
        dummy_address: bool = False,
    ):
        string = ""
        params = self.params
        # require int32 output buffer
        norm_buffer_add = None
        if params["norm_buffer"]:
            norm_tensor = self.input_tensors[params["norm_buffer"]]
            norm_buffer_add = f"&{norm_tensor.buffer_name}[{norm_tensor.buffer_address}]"
        if params["group"] == 1 and tflite_op:
            string += f"conv_params.stride_height = {params['stride_h']};\n"
            string += f"conv_params.stride_width = {params['stride_w']};\n"
            string += "conv_params.dilation_width_factor = 0;\n"
            string += "conv_params.dilation_height_factor = 0;\n"
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
            if params["kernel_layout"] == "OIHW":
                function_name = "TFLite_TransposeConv"
            elif params["kernel_layout"] == "IOHW":
                function_name = "TFLite_TransposeConv_IOHW"

            if dummy_address:
                input_address_string = "&buffer0[0]"
                output_address_string = "&buffer0[0]"
            else:
                input_address_string = (
                    f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])}"
                )
                output_address_string = (
                    f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])}"
                )

            if params["input2_dtype"] == "int8" and params["input_dtype"] in ["float32", "int8"]:
                if params["first_k_channel"] is None:
                    string += (
                        f"{function_name}_int8weight(conv_params,{input_address_string},"
                        + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                        + f"(q7_t*){weight_string},{params['kernel_h']},{params['kernel_w']},NULL,"
                        + f"{output_address_string},"
                        + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                        + "(float*)sbuf,1);\n"
                    )
                else:
                    if params["first_k_channel"] % 8 == 0:
                        string += (
                            f"{function_name}_int8weight_partialCH_8innercol(conv_params,{input_address_string},"
                            + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                            + f"(q7_t*){weight_string},(q7_t*){weight_string}Flash,{params['first_k_channel']},"
                            + f"{params['kernel_h']},{params['kernel_w']},NULL,"
                            + f"{output_address_string},"
                            + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                            + "(float*)sbuf,1);\n"
                        )
                    elif params["first_k_channel"] % 4 == 0:
                        string += (
                            f"{function_name}_int8weight_partialCH_4innercol(conv_params,{input_address_string},"
                            + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                            + f"(q7_t*){weight_string},(q7_t*){weight_string}Flash,{params['first_k_channel']},"
                            + f"{params['kernel_h']},{params['kernel_w']},NULL,"
                            + f"{output_address_string},"
                            + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                            + "(float*)sbuf,1);\n"
                        )
                    else:
                        raise NotImplementedError
            else:
                string += (
                    f"{function_name}(conv_params,{input_address_string},"
                    + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                    + f"{weight_string},{params['kernel_h']},{params['kernel_w']},NULL,"
                    + f"{output_address_string},"
                    + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                    + "(float*)sbuf,1);\n"
                )
        elif params["group"] == 1 and not tflite_op:
            # function name
            if (
                params["kernel_h"] == 1
                and params["kernel_w"] == 1
                and params["input_h"] == 1
                and params["input_w"] == 1
                and params["output_h"] == 1
                and params["output_w"] == 1
                and params["input_c"] / 10 == 1
                and params["output_c"] % 8 == 0
            ):
                if params["input_dtype"] == "int8":
                    function_name = "pointwise_conv_1row10col_10inputdepth"
                else:
                    function_name = "pointwise_conv_fp_1row10col_10inputdepth"
            elif (
                params["kernel_h"] == 1
                and params["kernel_w"] == 1
                and params["input_h"] * params["input_w"] >= 4
                and params["output_c"] % 4 == 0
            ):
                if params["input_dtype"] == "int8":
                    function_name = "pointwise_conv_4row4col"
                else:
                    function_name = "pointwise_conv_fp_4row4col"
            else:
                raise NotImplementedError

            if params["kernel_layout"] == "IOHW":
                function_name += "_IOHW"

            # int8 bp support
            if params["input_dtype"] == "int8":
                function_name += "_int8input"
            elif params["output_dtype"] == "int8":
                function_name += "_int8output"

            # weight name
            if isinstance(params["weight_name"], str) and isweightstr(params["weight_name"]):
                weight_string = params["weight_name"]
            else:
                weight_string = f"weight_fp{params['parsed_trainable']}"

            if params["input2_dtype"] == "int8" and params["input_dtype"] in ["float32", "int8"]:
                if params["first_k_channel"] is None:
                    string += (
                        f"{function_name}_int8weight("
                        + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                        + f"(q7_t*){weight_string},NULL,"
                        + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                        + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                        + f"{params['float_min']},{params['float_max']},"
                    )
                    # int8 bp support
                    if params["output_dtype"] == "int8":
                        string += (
                            "(float*)sbuf, NULL, 1);\n"
                            if not norm_buffer_add
                            else f"(float*)sbuf, {norm_buffer_add}, 1);\n"
                        )
                    else:
                        string += "(float*)sbuf, 1);\n"
                
                else:
                    function_name += "_int8weight_partialCH"
                    if params["first_k_channel"] % 8 == 0:
                        function_name += "_8innercol"
                    elif params["first_k_channel"] % 4 == 0:
                        function_name += "_4innercol"
                    else:
                        raise NotImplementedError

                    string += (
                        f"{function_name}("
                        + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                        + f"(q7_t*){weight_string},(q7_t*){weight_string}Flash,{params['first_k_channel']},NULL,"
                        + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                        + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                        + f"{params['float_min']},{params['float_max']},"
                    )
                    # int8 bp support
                    if params["output_dtype"] == "int8":
                        string += (
                            "(float*)sbuf, NULL, 1);\n"
                            if not norm_buffer_add
                            else f"(float*)sbuf, {norm_buffer_add}, 1);\n"
                        )
                    else:
                        string += "(float*)sbuf, 1);\n"
            else:
                string += (
                    f"{function_name}("
                    + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                    + f"{weight_string},NULL,"
                    + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                    + f"{params['float_min']},{params['float_max']},"
                )
                # int8 bp support
                if params["output_dtype"] == "int8":
                    string += (
                        "(float*)sbuf, NULL, 1);\n"
                        if not norm_buffer_add
                        else f"(float*)sbuf, {norm_buffer_add}, 1);\n"
                    )
                else:
                    string += "(float*)sbuf,1);\n"

        elif params["group"] == params["input_c"] and params["group"] == params["output_c"] and tflite_op:
            string += f"conv_params.stride_height = {params['stride_h']};\n"
            string += f"conv_params.stride_width = {params['stride_w']};\n"
            string += "conv_params.dilation_width_factor = 0;\n"
            string += "conv_params.dilation_height_factor = 0;\n"
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
            if params["kernel_layout"] == "OIHW":
                function_name = "TFLite_TransposeDepthwiseConv"
            elif params["kernel_layout"] == "IOHW":
                function_name = "TFLite_TransposeDepthwiseConv"

            if params["input2_dtype"] == "int8" and params["input_dtype"] == "float32":
                string += (
                    f"{function_name}_int8weight(conv_params,"
                    + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                    + f"{weight_string},{params['kernel_h']},{params['kernel_w']},NULL,"
                    + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                )
                string += "(float*)sbuf,1);\n"
            else:
                string += (
                    f"{function_name}(conv_params,"
                    + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                    + f"{weight_string},{params['kernel_h']},{params['kernel_w']},NULL,"
                    + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                )
                string += "(float*)sbuf,1);\n"
        elif params["group"] == params["input_c"] and params["group"] == params["output_c"] and not tflite_op:
            # function name
            function_name = "transpose_depthwise_conv_fp_kernel"
            if params["stride_h"] == 1:
                outpad = 0
            elif params["stride_h"] == 2:
                outpad = params["output_h"] - (
                    (params["input_h"] - 1) * params["stride_h"] - 2 * params["padding_h"] + params["kernel_h"]
                )
            else:
                raise NotImplementedError
            function_name += (
                f"{str(params['kernel_h'])}_stride{str(params['stride_h'])}_"
                + f"inpad{str(params['padding_h'])}_outpad{str(outpad)}"
            )

            if params["kernel_layout"] == "IOHW":
                function_name += "_IOHW"

            # weight name
            if isinstance(params["weight_name"], str) and isweightstr(params["weight_name"]):
                weight_string = params["weight_name"]
            else:
                weight_string = f"weight_fp{params['parsed_trainable']}"

            if params["input2_dtype"] == "int8" and params["input_dtype"] == "float32":
                string += (
                    f"{function_name}_int8weight("
                    + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                    + f"{weight_string},NULL,"
                    + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                    + f"{params['float_min']},{params['float_max']},"
                )
                # Assume padding value is 0.
                if params["output_dtype"] == "int8":
                    string += (
                        "(float*)sbuf, NULL, 1,0);\n"
                        if not norm_buffer_add
                        else f"(float*)sbuf, {norm_buffer_add}, 1, 0);\n"
                    )
                else:
                    string += "(float*)sbuf,1,0);\n"
            else:
                string += (
                    f"{function_name}("
                    + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                    + f"{weight_string},NULL,"
                    + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                    + f"{params['float_min']},{params['float_max']},"
                )
                # Assume padding value is 0.
                if params["output_dtype"] == "int8":
                    string += (
                        "(float*)sbuf, NULL, 1,0);\n"
                        if not norm_buffer_add
                        else f"(float*)sbuf, {norm_buffer_add}, 1, 0);\n"
                    )
                else:
                    string += "(float*)sbuf, 1,0);\n"
        else:
            raise NotImplementedError

        return string
