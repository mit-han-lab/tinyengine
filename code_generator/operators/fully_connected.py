# code_generator/operators/fully_connected.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["FullyConnectedOperator"]

default_params = {
    # Operator related
    "op": "FULLY_CONNECTED",
    # Tensor related
    "input_idx": None,
    "weight_idx": None,
    "bias_idx": None,
    "output_idx": None,
    "input_shape": None,
    "output_shape": None,
    "input_dtype": "float32",
    "weight_dtype": "float32",
    "output_dtype": "float32",
    # Quantization related
    "input_zero_point": None,
    "weight_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "weight_scale": None,
    "output_scale": None,
    "input_multiplier": None,
    "weight_multiplier": None,
    "output_multiplier": None,
    "input_shift": None,
    "weight_shift": None,
    "output_shift": None,
    # Additional parameters
    "activation": None,
    "use_bias": True,
}

class FullyConnectedOperator(basicOperator):
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

        if self.params["use_bias"]:
            self._add_input(
                self.params["bias_idx"],
                self.params["input_dtype"],
                1
            )

        if None in self.params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        input_str = self._getBufferstrCast(
            self.params['input_buf_add'], self.params['input_buf_add_offset'], dtype=self.params["input_dtype"]
        )
        weight_str = self._getBufferstrCast(
            self.params['weight_buf_add'], self.params['weight_buf_add_offset'], dtype=self.params["weight_dtype"]
        )
        output_str = self._getBufferstrCast(
            self.params['output_buf_add'], self.params['output_buf_add_offset'], dtype=self.params["output_dtype"]
        )
        
        if self.params["use_bias"]:
            bias_str = self._getBufferstrCast(
                self.params['bias_buf_add'], self.params['bias_buf_add_offset'], dtype=self.params["input_dtype"]
            )
            bias_str = f", {bias_str}"
        else:
            bias_str = ""
        
        string = (
            f"fully_connected({input_str}, {weight_str}{bias_str}, {output_str}, "
            f"{self.params['input_shape'][0]}, {self.params['input_shape'][1]}, "
            f"{self.params['output_shape'][1]});\n"
        )

        return string
