# code_generator/operators/concatenation.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    "op": "CONCATENATION",
    "input_indices": None,
    "output_idx": None,
    "axis": None,
    "input_dtype": None,
    "output_dtype": None,
}

class ConcatenationOperator(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()

        if self.params["input_indices"] is not None:
            for idx in self.params["input_indices"]:
                self._add_input(idx, self.params["input_dtype"])

        if self.params["output_idx"] is not None:
            self._add_output(self.params["output_idx"], self.params["output_dtype"])

        if None in default_params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        input_str = ", ".join([self._getBufferstrCast(f"input{idx}", 0, dtype=params["input_dtype"]) for idx in params["input_indices"]])
        output_str = self._getBufferstrCast(f"output{params['output_idx']}", 0, dtype=params["output_dtype"])
        
        string = (
            f"concatenate({input_str}, {output_str}, {params['axis']});\n"
        )

        return string
