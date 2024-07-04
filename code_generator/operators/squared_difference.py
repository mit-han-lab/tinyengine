# code_generator/operators/squared_difference.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    "op": "SQUARED_DIFFERENCE",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    "input_dtype": None,
    "output_dtype": None,
    "input_shape": None,
    "input2_shape": None,
    "output_shape": None,
    "input_zero_point": None,
    "input2_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "input2_scale": None,
    "output_scale": None,
    "left_shift": None,
    "input_multiplier": None,
    "input2_multiplier": None,
    "input_shift": None,
    "input2_shift": None,
    "output_multiplier": None,
    "output_shift": None,
}

class SquaredDifferenceOperator(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()

        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            *self.params["input_shape"]
        )
        self._add_input(
            self.params["input2_idx"],
            self.params["input_dtype"],
            *self.params["input2_shape"]
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            *self.params["output_shape"]
        )

        if None in default_params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        input_str = self._getBufferstrCast(params["input_idx"], 0, dtype=params["input_dtype"])
        input2_str = self._getBufferstrCast(params["input2_idx"], 0, dtype=params["input_dtype"])
        output_str = self._getBufferstrCast(params["output_idx"], 0, dtype=params["output_dtype"])
        
        if params["input_scale"] and params["output_scale"]:
            string = (
                f"quantized_squared_difference({input_str}, {input2_str}, {output_str}, "
                f"{params['left_shift']}, {params['input_multiplier']}, {params['input_shift']}, "
                f"{params['input2_multiplier']}, {params['input2_shift']}, "
                f"{params['output_multiplier']}, {params['output_shift']});\n"
            )
        else:
            string = (
                f"squared_difference({input_str}, {input2_str}, {output_str});\n"
            )

        return string
