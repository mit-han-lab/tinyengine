import logging
import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["rsqrt"]

default_params = {
    # op related
    "op": "RSQRT",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "input_size": None,
    "output_dim": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
    "input_meta": None,
}


class rsqrt(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["input_size"],
            1,
            1,
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["input_size"],
            1,
            1,
        )

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        # params = self.params
        string = ""

        logging.warn("RSQRT operator support is still no ready.")

        return string
