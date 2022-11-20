import warnings

from .basic_utils import basicOperator, deep_copy_dicts, islabelstr, isParamstr, overwrite_dicts

__all__ = ["mul"]

default_params = {
    # op related
    "op": "MUL",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_size": None,
    "input2_size": None,
    "output_size": None,
    "input_dtype": "float32",
    "input2_dtype": "float32",
    "output_dtype": "float32",
    # quantization related
    "weight_value": None,
    "bias": None,
    "input_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "output_scale": None,
    "multiplier": None,
    "shift": None,
    # input of scale from some conv2d
    "scale_conv_2d_op": None,
    "scale_from_add": None,
    "constant": None,
    # inplace
    "inplace": False,
}


class mul(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(self.params["input_idx"], self.params["input_dtype"], self.params["input_size"], 1, 1)
        if not (isParamstr(self.params["input2_idx"]) or islabelstr(self.params["input2_idx"])):
            self._add_input(self.params["input2_idx"], self.params["input2_dtype"], self.params["output_size"], 1, 1)
        self._add_output(self.params["output_idx"], self.params["output_dtype"], self.params["output_size"], 1, 1)

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def get_macs(self):
        p = self.params
        return p["input_size"]

    def generate_inference_str(self):
        params = self.params

        if params["input_dtype"] == "float32":
            if self.params["input_size"] != self.params["input2_size"]:
                if not islabelstr(self.params["input_idx"]):
                    input0_ptr = f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])}"
                else:
                    input0_ptr = "labels"
                if isParamstr(self.params["input2_idx"]):
                    if "add" not in self.params["input2_idx"] and "scale" in self.params["input2_idx"]:
                        input2_ptr = f"scales{self.params['scale_conv_2d_op'].params['parsed_trainable']}"
                    else:
                        input2_ptr = None  # we don't
                elif not islabelstr(self.params["input2_idx"]):
                    input2_ptr = f"{self._getBufferstr(params['input2_buf_add'], params['input2_buf_add_offset'])}"
                else:
                    input2_ptr = "labels"

                if self.params["input_size"] > self.params["input2_size"]:
                    input_array_ptr = input0_ptr
                    scaler = input2_ptr
                    input_size = self.params["input_size"]
                    scaler_size = self.params["input2_size"]
                else:
                    input_array_ptr = input2_ptr
                    scaler = input0_ptr
                    input_size = self.params["input2_size"]
                    scaler_size = self.params["input_size"]

                if scaler_size > 1:
                    # we need loop over HW dimensions
                    HW_cout = int(input_size / scaler_size)
                    assert HW_cout > 1
                    if self.params["inplace"]:
                        string = (
                            f"fptr = {input_array_ptr};\n"
                            + f"fptr2 = {scaler};\n"
                            + f"for(int hw = 0; hw < {HW_cout}; hw++)"
                            + "{\n"
                            + (
                                f"for(int i = 0; i < {scaler_size}; i++)"
                                + "{float f = *fptr; *fptr++ = fptr2[i] * f;};\n"
                            )
                            + "}\n"
                        )
                    else:
                        string = (
                            f"fptr = {input_array_ptr};\n"
                            + "fptr3 = (float*)"
                            + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])};"
                            + f"fptr2 = {scaler};\n"
                            + f"for(int hw = 0; hw < {HW_cout}; hw++)"
                            + "{\n"
                            + f"for(int i = 0; i < {scaler_size}; i++) *fptr3++ = fptr2[i] * *fptr++;\n"
                            + "}\n"
                        )
                else:
                    string = f"fptr = (float*){input_array_ptr};"
                    string += (
                        "fptr3 = (float*)"
                        + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])};"
                    )
                    # if it is from parameter
                    if self.params["scale_from_add"] is not None:
                        string += (
                            f"for(int i = 0; i < {self.params['output_size']}; i++) fptr3[i] = "
                            + f"{self.params['scale_from_add']} * fptr[i];\n"
                        )
                    elif isinstance(self.params["constant"], float):
                        string += (
                            f"for(int i = 0; i < {self.params['output_size']}; i++) fptr3[i] = "
                            + f"{self.params['constant']} * fptr[i];\n"
                        )
                    else:
                        string += f"fptr2 = {scaler};"
                        string += (
                            f"for(int i = 0; i < {self.params['output_size']}; i++) fptr3[i] = *fptr2 * fptr[i];\n"
                        )
            else:
                if isParamstr(self.params["input2_idx"]):
                    assert self.params["scale_conv_2d_op"] is not None
                    string = (
                        f"mul({self.params['output_size']},"
                        + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + f"scales{self.params['scale_conv_2d_op'].params['parsed_trainable']},"
                        + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                    )
                elif islabelstr(self.params["input2_idx"]):
                    string = (
                        f"mul({self.params['output_size']},"
                        + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + "labels,"
                        + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                    )
                else:
                    string = (
                        f"mul({self.params['output_size']},"
                        + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + f"{self._getBufferstrCast(params['input2_buf_add'], params['input2_buf_add_offset'])},"
                        + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                    )
        else:
            raise NotImplementedError

        return string
