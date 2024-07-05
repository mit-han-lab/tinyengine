# code_generator/operators/sub.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    "op": "SUB",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    "input_size": 1,
    "input2_size": 1,
    "output_size": 1,
    "input_dtype": "float32",
    "input2_dtype": "float32",
    "output_dtype": "float32",
    "input_zero_point": 0,
    "input2_zero_point": 0,
    "output_zero_point": 0,
    "input_scale": 1.0,
    "input2_scale": 1.0,
    "output_scale": 1.0,
    "left_shift": 0,
    "input_multiplier": 1,
    "input2_multiplier": 1,
    "input_shift": 0,
    "input2_shift": 0,
    "output_multiplier": 1,
    "output_shift": 0,
}

class sub(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        self._add_input(self.params["input_idx"], self.params["input_dtype"], self.params["input_size"], 1, 1)
        self._add_input(self.params["input2_idx"], self.params["input2_dtype"], self.params["input2_size"], 1, 1)
        self._add_output(self.params["output_idx"], self.params["output_dtype"], self.params["output_size"], 1, 1)
        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def get_macs(self):
        return self.params["input_size"]

    def generate_inference_str(self):
        params = self.params

        if params["input_dtype"] == "float32":
            function_name = "sub"
        elif params["input_dtype"] == "int8":
            function_name = "sub_int8"
        else:
            raise NotImplementedError(f"Data type {params['input_dtype']} is not implemented for sub operator.")

        input_str = f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'], dtype=params['input_dtype'])}"
        input2_str = f"{self._getBufferstrCast(params['input2_buf_add'], params['input2_buf_add_offset'], dtype=params['input2_dtype'])}"
        output_str = f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'], dtype=params['output_dtype'])}"

        if params["input_dtype"] == "int8":
            input_zero_point = params["input_zero_point"]
            input2_zero_point = params["input2_zero_point"]
            output_zero_point = params["output_zero_point"]
            left_shift = params["left_shift"]
            input_multiplier = params["input_multiplier"]
            input2_multiplier = params["input2_multiplier"]
            input_shift = params["input_shift"]
            input2_shift = params["input2_shift"]
            output_multiplier = params["output_multiplier"]
            output_shift = params["output_shift"]

            string = (
                f"{function_name}({self.params['input_size']},"
                + f"{input_str}, {input_zero_point}, {input_multiplier}, {input_shift},"
                + f"{input2_str}, {input2_zero_point}, {input2_multiplier}, {input2_shift},"
                + f"{output_str}, {output_zero_point}, {output_multiplier}, {output_shift}, {left_shift});\n"
            )
        else:
            string = (
                f"{function_name}({self.params['output_size']},"
                + f"{input_str},"
                + f"{input2_str},"
                + f"{output_str});\n"
            )

        return string
