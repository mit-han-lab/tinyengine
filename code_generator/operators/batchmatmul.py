import logging
import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["batchmatmul"]

default_params = {
    # op related
    "op": "BATCH_MATMUL",
    "batch_size": None,
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_col": None,
    "input_row": None,
    "input2": None,
    "input2_col": None,
    "input2_row": None,
    "output_col": None,
    "output_row": None,
    "input_dtype": "fp32",
    "input2_dtype": "fp32",
    "output_dtype": "fp32",
}


class batchmatmul(basicOperator):
    div_const_cnt = 0

    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["batch_size"],
            self.params["input_row"],
            self.params["input_col"],
        )
        self._add_input(
            self.params["input2_idx"],
            self.params["input2_dtype"],
            self.params["batch_size"],
            self.params["input2_row"],
            self.params["input2_col"],
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["batch_size"],
            self.params["output_row"],
            self.params["output_col"],
        )

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        string = ""

        if params["input_dtype"] == "float32":
            logging.warn("BATCHMAMUL still needs implementation.")
        else:
            raise NotImplementedError

        return string
