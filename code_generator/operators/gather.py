# code_generator/operators/gather.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts, tensor

default_params = {
    # op related
    "op": "GATHER",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dtype": "int32",
    "output_dtype": "int32",
    "input_shape": [1],  # Default shape, modify as needed
    "output_shape": [1],  # Default shape, modify as needed
}

class GatherOperator(basicOperator):
    def __init__(self, params: dict) -> None:
        super().__init__()
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        # handle input/output tensors
        self._add_input(
            self.params["input_idx"], self.params["input_dtype"], *self.params["input_shape"]
        )
        self._add_output(
            self.params["output_idx"], self.params["output_dtype"], *self.params["output_shape"]
        )

        if None in self.params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        input_buffer = self._getBufferstr("front", 0)
        output_buffer = self._getBufferstr("end", 0)
        return f"gather({input_buffer}, {output_buffer});"

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
