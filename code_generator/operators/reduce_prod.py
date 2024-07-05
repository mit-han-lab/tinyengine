# code_generator/operators/reduce_prod.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    "op": "REDUCE_PROD",
    "input_idx": None,
    "output_idx": None,
    "input_size": 1,
    "output_size": 1,
    "input_dtype": "float32",
    "output_dtype": "float32",
    "reduction_axes": [],
}

class ReduceProdOperator(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        self._add_input(self.params["input_idx"], self.params["input_dtype"], self.params["input_size"])
        self._add_output(self.params["output_idx"], self.params["output_dtype"], self.params["output_size"])

        if None in default_params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        input_str = self._getBufferstrCast(params["input_buf_add"], params["input_buf_add_offset"], dtype=params["input_dtype"])
        output_str = self._getBufferstrCast(params["output_buf_add"], params["output_buf_add_offset"], dtype=params["output_dtype"])
        
        reduction_axes_str = ', '.join(map(str, params["reduction_axes"]))

        if params["input_dtype"] == "float32":
            function_name = "reduce_prod"
        elif params["input_dtype"] == "int8":
            function_name = "reduce_prod_int8"
        elif params["input_dtype"] == "int32":
            function_name = "reduce_prod_int32"
        else:
            raise NotImplementedError(f"Data type {params['input_dtype']} is not implemented for reduce_prod operator.")

        string = (
            f"{function_name}({input_str}, {output_str}, {params['input_size']}, "
            + f"{params['output_size']}, {{{reduction_axes_str}}}, {len(params['reduction_axes'])});\n"
        )

        return string
