# code_generator/operators/fully_connected.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    "op": "FULLY_CONNECTED",
    "input_idx": None,
    "weight_idx": None,
    "bias_idx": None,
    "output_idx": None,
    "input_shape": None,
    "output_shape": None,
    "input_dtype": None,
    "weight_dtype": None,
    "output_dtype": None,
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
    "activation": None,
    "use_bias": None,
    "input_buf_add": None,
    "input_buf_add_offset": 0,
    "input2_buf_add": None,  # Renamed to input2_buf_add
    "input2_buf_add_offset": 0,  # Renamed to input2_buf_add_offset
    "input3_buf_add": None,  # Renamed to input3_buf_add for bias
    "input3_buf_add_offset": 0,  # Renamed to input3_buf_add_offset for bias
    "output_buf_add": None,
    "output_buf_add_offset": 0,
}

class FullyConnectedOperator(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors
        self._add_input(self.params["input_idx"], self.params["input_dtype"])
        self._add_input(self.params["weight_idx"], self.params["weight_dtype"])
        if self.params["use_bias"]:
            self._add_input(self.params["bias_idx"], self.params["output_dtype"])
        self._add_output(self.params["output_idx"], self.params["output_dtype"])

        if None in default_params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params

        # Ensure buffer addresses are not None
        if params["input_buf_add"] is None:
            raise ValueError("input_buf_add must not be None")
        if params["input2_buf_add"] is None:
            raise ValueError("input2_buf_add must not be None")
        if params["output_buf_add"] is None:
            raise ValueError("output_buf_add must not be None")
        if self.params["use_bias"] and params["input3_buf_add"] is None:
            raise ValueError("input3_buf_add must not be None if use_bias is True")

        input_str = self._getBufferstrCast(params["input_buf_add"], params["input_buf_add_offset"], dtype=params["input_dtype"])
        weight_str = self._getBufferstrCast(params["input2_buf_add"], params["input2_buf_add_offset"], dtype=params["weight_dtype"])
        output_str = self._getBufferstrCast(params["output_buf_add"], params["output_buf_add_offset"], dtype=params["output_dtype"])
        bias_str = "NULL"
        if self.params["use_bias"] and params["bias_idx"] != -1:
            bias_str = self._getBufferstrCast(params["input3_buf_add"], params["input3_buf_add_offset"], dtype=params["output_dtype"])

        input_zero_point = params["input_zero_point"] if params["input_zero_point"] is not None else 0
        weight_zero_point = params["weight_zero_point"] if params["weight_zero_point"] is not None else 0
        output_zero_point = params["output_zero_point"] if params["output_zero_point"] is not None else 0
        input_scale = params["input_scale"] if params["input_scale"] is not None else 0.0
        weight_scale = params["weight_scale"] if params["weight_scale"] is not None else 0.0
        output_scale = params["output_scale"] if params["output_scale"] is not None else 0.0
        input_multiplier = params["input_multiplier"] if params["input_multiplier"] is not None else 0
        weight_multiplier = params["weight_multiplier"] if params["weight_multiplier"] is not None else 0
        output_multiplier = params["output_multiplier"] if params["output_multiplier"] is not None else 0
        input_shift = params["input_shift"] if params["input_shift"] is not None else 0
        weight_shift = params["weight_shift"] if params["weight_shift"] is not None else 0
        output_shift = params["output_shift"] if params["output_shift"] is not None else 0
        activation = params["activation"] if params["activation"] is not None else 0
        
        return (f"fully_connected({input_str}, {weight_str}, {bias_str}, {output_str}, "
                f"{input_zero_point}, {weight_zero_point}, {output_zero_point}, "
                f"{input_scale}, {weight_scale}, {output_scale}, "
                f"{input_multiplier}, {weight_multiplier}, {output_multiplier}, "
                f"{input_shift}, {weight_shift}, {output_shift}, "
                f"{activation});")
