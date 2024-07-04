# code_generator/operators/shape.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    "op": "SHAPE",
    "input_idx": None,
    "output_idx": None,
    "input_dtype": "int8",
    "output_dtype": "int8",
    "input_shape": [],  # Add default empty list for input_shape
}

class ShapeOperator(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        self._add_input(self.params["input_idx"], self.params["input_dtype"], 1)
        self._add_output(self.params["output_idx"], self.params["output_dtype"], len(self.params["input_shape"]))

        if None in self.params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        input_buffer = self._getBufferstr("front", 0)
        output_buffer = self._getBufferstr("end", 0)
        return f"shape({input_buffer}, {output_buffer}, {len(self.params['input_shape'])});"

    def get_macs(self):
        return 0

    def get_weights_size(self):
        return 0

    def get_bias_size(self):
        return 0

    def get_scale_size(self):
        return 0

    def get_activation_size(self):
        return super().get_activation_size()

    def get_sbuf_size(self):
        return super().get_sbuf_size()

    def get_kbuf_size(self):
        return super().get_kbuf_size()
