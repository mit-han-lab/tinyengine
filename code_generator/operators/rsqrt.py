# code_generator/operators/rsqrt.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["RsqrtOperator"]

default_params = {
    # Operator related
    "op": "RSQRT",
    # Tensor related
    "input_idx": None,
    "output_idx": None,
    "input_shape": None,
    "output_shape": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
}

class RsqrtOperator(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()

        # Handle input/output tensors
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            *self.params["input_shape"]
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            *self.params["output_shape"]
        )

        if None in self.params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        input_str = self._getBufferstrCast(
            self.params['input_buf_add'], self.params['input_buf_add_offset'], dtype=self.params["input_dtype"]
        )
        output_str = self._getBufferstrCast(
            self.params['output_buf_add'], self.params['output_buf_add_offset'], dtype=self.params["output_dtype"]
        )

        string = (
            f"rsqrt({input_str}, {output_str}, "
            f"{self.params['input_shape'][0]}, {self.params['input_shape'][1]});\n"
        )

        return string
