# code_generator/operators/mul.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts, islabelstr, isParamstr

default_params = {
    "op": "MUL",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    "input_size": None,
    "input2_size": None,
    "output_size": None,
    "input_dtype": "float32",
    "input2_dtype": "float32",
    "output_dtype": "float32",
    "weight_value": None,
    "bias": None,
    "input_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "output_scale": None,
    "multiplier": None,
    "shift": None,
    "scale_conv_2d_op": None,
    "scale_from_add": None,
    "constant": None,
    "inplace": False,
}

class mul(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
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
            function_name = "mul"
        elif params["input_dtype"] == "int8":
            function_name = "mul_int8"
        else:
            raise NotImplementedError(f"Data type {params['input_dtype']} is not implemented for mul operator.")

        input_str = f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'], dtype=params['input_dtype'])}"
        input2_str = f"{self._getBufferstrCast(params['input2_buf_add'], params['input2_buf_add_offset'], dtype=params['input2_dtype'])}"
        output_str = f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'], dtype=params['output_dtype'])}"

        if params["input_size"] != params["input2_size"]:
            if self.params["input_size"] > self.params["input2_size"]:
                input_array_ptr = input_str
                scaler = input2_str
                input_size = self.params["input_size"]
                scaler_size = self.params["input2_size"]
            else:
                input_array_ptr = input2_str
                scaler = input_str
                input_size = self.params["input2_size"]
                scaler_size = self.params["input_size"]

            HW_count = int(input_size / scaler_size)

            if scaler_size > 1:
                if HW_count > 1:
                    if self.params["inplace"]:
                        string = (
                            f"fptr = {input_array_ptr};\n"
                            + f"fptr2 = {scaler};\n"
                            + f"for(int hw = 0; hw < {HW_count}; hw++)"
                            + "{\n"
                            + f"for(int i = 0; i < {scaler_size}; i++)"
                            + "{float f = *fptr; *fptr++ = fptr2[i] * f;};\n"
                            + "}\n"
                        )
                    else:
                        string = (
                            f"fptr = {input_array_ptr};\n"
                            + "fptr3 = (float*)"
                            + f"{output_str};"
                            + f"fptr2 = {scaler};\n"
                            + f"for(int hw = 0; hw < {HW_count}; hw++)"
                            + "{\n"
                            + f"for(int i = 0; i < {scaler_size}; i++) *fptr3++ = fptr2[i] * *fptr++;\n"
                            + "}\n"
                        )
                else:
                    string = f"fptr = (float*){input_array_ptr};\n"
                    string += f"fptr3 = (float*){output_str};\n"
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
                        string += f"fptr2 = {scaler};\n"
                        string += (
                            f"for(int i = 0; i < {self.params['output_size']}; i++) fptr3[i] = *fptr2 * fptr[i];\n"
                        )
            else:
                string = f"fptr = (float*){input_array_ptr};\n"
                string += f"fptr3 = (float*){output_str};\n"
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
                    string += f"fptr2 = {scaler};\n"
                    string += (
                        f"for(int i = 0; i < {self.params['output_size']}; i++) fptr3[i] = *fptr2 * fptr[i];\n"
                    )
        else:
            string = (
                f"{function_name}({self.params['output_size']},"
                + f"{input_str},"
                + f"{input2_str},"
                + f"{output_str});\n"
            )

        return string
