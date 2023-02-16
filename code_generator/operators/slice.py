import logging

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["slice"]

default_params = {
    # op related
    "op": "SLICE",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "begin_h": None,
    "begin_w": None,
    "begin_c": None,
    "end_h": None,
    "end_w": None,
    "end_c": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "input_dtype": "fp32",
    "output_dtype": "fp32",
}


class slice(basicOperator):
    div_const_cnt = 0

    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["input_h"],
            self.params["input_w"],
            self.params["input_c"],
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["output_h"],
            self.params["output_w"],
            self.params["output_c"],
        )

    def generate_inference_str(self):
        # params = self.params
        string = ""

        logging.warn("slice operator is still no ready.")

        return string
