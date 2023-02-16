import logging
import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["concat"]

default_params = {
    # op related
    "op": "CONCATENATION",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "input2": None,
    "input2_h": None,
    "input2_w": None,
    "input2_c": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "input_dtype": None,
    "input2_dtype": None,
    "output_dtype": None,
}


class concat(basicOperator):
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
        if "constant" not in self.params["input2_idx"]:
            self._add_input(
                self.params["input2_idx"],
                self.params["input2_dtype"],
                self.params["input2_h"],
                self.params["input2_w"],
                self.params["input2_c"],
            )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["output_h"],
            self.params["output_w"],
            self.params["output_c"],
        )

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        string = ""

        if params["input_dtype"] == "float32":
            logging.warn("CONCATENATION still needs implementation.")
        else:
            raise NotImplementedError

        return string
