import warnings

from .basic_utils import basicOperator, deep_copy_dicts, isweightstr, overwrite_dicts
from .conv2d import Conv2d

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
    "norm_buffer": None,
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
    "groups": 1,
    # for inplace SGD
    "inplace_weight_name": None,
    "inplace_int8_input": False,
    "QAS": None,
    # for int8/fp implementation
    "float32_input2": None,
}


class groupConv2d(basicOperator):
    conv_params_declared = False

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
        return (
            p["output_h"] * p["output_w"] * p["kernel_h"] * p["kernel_w"] * p["input_c"] * p["output_c"] / p["groups"]
        )

    def get_weights_size(self) -> int:
        p = self.params
        if p["input_dtype"] == "float32" or "fp32":
            size = 4
        else:
            size = 1
        return p["kernel_h"] * p["kernel_w"] * p["input_c"] * p["output_c"] * size / (p["groups"] * p["groups"])

    def get_bias_size(self) -> int:
        p = self.params
        return 4 * p["output_c"]

    def get_scale_size(self) -> int:
        p = self.params
        return 4 * p["output_c"]

    def get_sbuf_size(self) -> int:
        p = self.params
        if p["output_c"] == p["input_c"] and p["output_c"] == p["groups"]:
            return int((p["input_h"] + p["padding_h"] * 2) * (p["input_w"] + p["padding_w"] * 2) * 4) + int(
                p["kernel_h"] * p["kernel_w"] * p["input_c"] * 4
            )  # Same like depthwise conv; brutally transform filter_data from HWC -> CHW
        elif (p["output_c"] / p["groups"]) % 16 == 0:
            return int(p["kernel_h"] * p["kernel_w"] * 4 * 4) + int(
                p["kernel_h"] * p["kernel_w"] * 16 * 4
            )  # group_conv 4row16col, im2col for both input and weight, and floating point
        elif (p["output_c"] / p["groups"]) % 8 == 0:
            return int(p["kernel_h"] * p["kernel_w"] * 4 * 4) + int(
                p["kernel_h"] * p["kernel_w"] * 8 * 4
            )  # group_conv 4row8col, im2col for both input and weight, and floating point
        else:
            return 0

    def get_kbuf_size(self) -> int:
        return 0

    def add_int32_buffer_tensor(self):
        params = self.params
        if (params["output_c"] / params["groups"]) % 16 == 0:
            self._add_input(
                params["output_idx"] + "_buffer",
                "int32",
                16,
                params["groups"],
                1,
            )
        elif (params["output_c"] / params["groups"]) % 8 == 0:
            self._add_input(
                params["output_idx"] + "_buffer",
                "int32",
                8,
                params["groups"],
                1,
            )
        else:
            raise NotImplementedError
        params["norm_buffer"] = len(self.input_tensors) - 1

    def generate_inference_str(
        self,
        tflite_op: bool = False,
        dummy_address: bool = False,
    ):
        string = ""
        params = self.params
        # floating point implmenetation
        if tflite_op:
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

            function_name = "group_conv"
            if params["input_dtype"] == "int8":
                function_name += "_int8input"

            if not params["float32_input2"]:
                function_name += "_int8weight"

            if params["inplace_weight_name"] is not None:
                if self.params["QAS"] is not None:
                    QAS_cnt = int(self.params["output_c"] / self.params["input_c"])
                    if QAS_cnt == 1:
                        QAS_cnt = len(self.params["QAS"].flatten())
                    string += f"const float {self.params['inplace_weight_name']}_QAS[{QAS_cnt}] = " + "{"
                    QAS = self.params["QAS"].flatten()
                    for i in 1 / QAS:
                        string += str(i) + ","
                    string += "};\n"

                    if dummy_address:
                        string += (
                            f"{function_name}_inplace(conv_params,{params['groups']},&buffer0[0],"
                            + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                            + f"{weight_string},{params['kernel_h']},{params['kernel_w']},NULL,"
                            + f"{params['inplace_weight_name']},"
                            + f"{str(params['output_h'])},{str(params['output_w'])},"
                            + f"{str(params['output_c'])},(float*)sbuf,1, "
                            + f"{self.params['inplace_weight_name']}_QAS, lr);\n"
                        )
                    else:
                        string += (
                            f"{function_name}_inplace(conv_params,{params['groups']},"
                            + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                            + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                            + f"{weight_string},{params['kernel_h']},{params['kernel_w']},NULL,"
                            + f"{params['inplace_weight_name']},"
                            + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                            + f"(float*)sbuf,1, {self.params['inplace_weight_name']}_QAS, lr);\n"
                        )
            else:
                string += (
                    f"{function_name}(conv_params,{params['groups']},"
                    + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                    + f"{weight_string},{params['kernel_h']},{params['kernel_w']},NULL,"
                    + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{str(params['output_h'])},{str(params['output_w'])},"
                    + f"{str(params['output_c'])},(float*)sbuf,1);\n"
                )
        elif not tflite_op:
            # function name
            if (
                params["kernel_h"] == 1
                and params["kernel_w"] == 1
                and params["input_h"] == 1
                and params["input_w"] == 1
                and params["output_h"] == 1
                and params["output_w"] == 1
                and params["output_c"] / params["input_c"] == 10
            ):  # group pointwise conv
                function_name = (
                    "group_pointwise_conv_in1x1_out1x1_1row10col_uniweight"
                    if not params["float32_input2"]
                    else "group_pointwise_conv_fp_in1x1_out1x1_1row10col_uniweight"
                )
            elif (
                params["output_c"] == params["input_c"] and params["output_c"] == params["groups"]
            ):  # Same like depthwise conv
                function_name = "depthwise_conv_kernel" if not params["float32_input2"] else "depthwise_conv_fp_kernel"
                function_name += (
                    f"{str(params['kernel_h'])}_stride{str(params['stride_h'])}_pad{str(params['padding_h'])}"
                    + f"_in{str(params['input_h'])}x{str(params['input_w'])}_out{str(params['output_h'])}x"
                    + f"{str(params['output_w'])}_uniweight_1row1col"
                )
            elif (params["output_c"] / params["groups"]) % 16 == 0:
                function_name = "group_conv_kernel" if not params["float32_input2"] else "group_conv_fp_kernel"
                function_name += (
                    f"{str(params['kernel_h'])}_stride{str(params['stride_h'])}_pad{str(params['padding_h'])}_in"
                    + f"{str(params['input_h'])}x{str(params['input_w'])}_out{str(params['output_h'])}x"
                    + f"{str(params['output_w'])}_uniweight_4row16col"
                )
            elif (params["output_c"] / params["groups"]) % 8 == 0:
                function_name = "group_conv_kernel" if not params["float32_input2"] else "group_conv_fp_kernel"
                function_name += (
                    f"{str(params['kernel_h'])}_stride{str(params['stride_h'])}_pad{str(params['padding_h'])}_in"
                    + f"{str(params['input_h'])}x{str(params['input_w'])}_out{str(params['output_h'])}x"
                    + f"{str(params['output_w'])}_uniweight_4row8col"
                )
            else:
                raise NotImplementedError

            # int8 input for inplace cast
            if params["input_dtype"] == "int8":
                function_name += "_int8input"

            if not params["float32_input2"]:
                function_name += "_int8weight"

            if (
                (params["output_c"] / params["groups"]) % 16 == 0
                or (params["output_c"] / params["groups"]) % 8 == 0
                or params["output_c"] / params["input_c"] == 10
            ):
                function_name += "_inplace"
            else:
                raise NotImplementedError

            # weight name
            if isinstance(params["weight_name"], str) and isweightstr(params["weight_name"]):
                weight_string = params["weight_name"]
            else:
                weight_string = f"weight_fp{params['parsed_trainable']}"

            # require int32 output buffer
            norm_buffer_add = None
            if params["norm_buffer"]:
                norm_tensor = self.input_tensors[params["norm_buffer"]]
                norm_buffer_add = f"&{norm_tensor.buffer_name}[{norm_tensor.buffer_address}]"

            if params["inplace_weight_name"] is not None:
                if self.params["QAS"] is not None:
                    QAS_cnt = int(self.params["output_c"] / self.params["input_c"])
                    if QAS_cnt == 1:
                        QAS_cnt = len(self.params["QAS"].flatten())
                    string += f"const float {self.params['inplace_weight_name']}_QAS[{QAS_cnt}] = " + "{"
                    QAS = self.params["QAS"].flatten()
                    for i in 1 / QAS:
                        string += str(i) + ","
                    string += "};\n"

                    string += (
                        f"{function_name}"
                        + f"({self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                        + f"{weight_string},NULL,"
                        + f"{params['inplace_weight_name']},"
                        + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                        + f"{params['float_min']},{params['float_max']},"
                    )
                    if not params["float32_input2"]:
                        string += (
                            (
                                f"(float*)sbuf, NULL, 1,{params['groups']}, "
                                + f"{self.params['inplace_weight_name']}_QAS, lr);\n"
                            )
                            if not norm_buffer_add
                            else (
                                f"(float*)sbuf, {norm_buffer_add}, 1, {params['groups']}, "
                                + f"{self.params['inplace_weight_name']}_QAS, lr);\n"
                            )
                        )
                    else:
                        string += f"(float*)sbuf,1,{params['groups']}, {self.params['inplace_weight_name']}_QAS, lr);\n"
                else:
                    string += (
                        f"{function_name}"
                        + f"({self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                        + f"{weight_string},NULL,"
                        + f"{params['inplace_weight_name']},"
                        + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                        + f"{params['float_min']},{params['float_max']},"
                    )
                    if not params["float32_input2"]:
                        string += (
                            f"(float*)sbuf, NULL, 1,{params['groups']});\n"
                            if not norm_buffer_add
                            else f"(float*)sbuf, {norm_buffer_add}, 1, {params['groups']});\n"
                        )
                    else:
                        string += f"(float*)sbuf,1,{params['groups']});\n"
            else:
                string += (
                    f"{function_name}"
                    + f"({self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                    + f"{weight_string},NULL,"
                    + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])},"
                    + f"{str(params['output_h'])},{str(params['output_w'])},{str(params['output_c'])},"
                    + f"{params['float_min']},{params['float_max']},"
                )
                if not params["float32_input2"]:
                    string += (
                        f"(float*)sbuf,NULL,1,{params['groups']});\n"
                        if not norm_buffer_add
                        else f"(float*)sbuf, {norm_buffer_add}, 1, {params['groups']});\n"
                    )
                else:
                    string += f"(float*)sbuf,1,{params['groups']});\n"

        return string
