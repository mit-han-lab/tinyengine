# code_generator/operators/reduce_prod.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["ReduceProdOperator"]

default_params = {
    "op": "REDUCE_PROD",
    "input_idxs": [],
    "output_idx": None,
    "input_sizes": [],
    "output_size": None,
    "input_dtypes": [],
    "output_dtype": "float32",
    "input_zero_points": [],
    "output_zero_point": None,
    "input_scales": [],
    "output_scale": None,
    "reduction_axis": None,
}

class ReduceProdOperator(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()

        for input_idx, input_dtype, input_size in zip(self.params["input_idxs"], self.params["input_dtypes"], self.params["input_sizes"]):
            self._add_input(input_idx, input_dtype, input_size)
        self._add_output(self.params["output_idx"], self.params["output_dtype"], self.params["output_size"])

        if None in default_params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def get_macs(self):
        return sum(self.params["input_sizes"])

    def generate_inference_str(self):
        params = self.params
        input_strs = [self._getBufferstrCast(idx, 0, dtype=dtype) for idx, dtype in zip(params["input_idxs"], params["input_dtypes"])]
        output_str = self._getBufferstrCast(params["output_idx"], 0, dtype=params["output_dtype"])
        
        input_str_concat = ", ".join(input_strs)
        reduction_axis_str = ", ".join(map(str, params["reduction_axis"]))
        
        if all(params["input_scales"]) and params["output_scale"]:
            string = f"quantized_reduce_prod([{input_str_concat}], {output_str}, [{reduction_axis_str}]);\n"
        else:
            string = f"reduce_prod([{input_str_concat}], {output_str}, [{reduction_axis_str}]);\n"

        return string
