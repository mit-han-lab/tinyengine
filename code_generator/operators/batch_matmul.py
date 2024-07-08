# code_generator/operators/batch_matmul.py

import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["BatchMatMulOperator"]

default_params = {
    # Operator related
    "op": "BATCH_MATMUL",
    # Tensor related
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    "input_shape": None,
    "input2_shape": None,
    "output_shape": None,
    "input_dtype": "float32",
    "input2_dtype": "float32",
    "output_dtype": "float32",
    # Additional parameters
    "adj_x": False,
    "adj_y": False,
}

class BatchMatMulOperator(basicOperator):
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
        self._add_input(
            self.params["input2_idx"],
            self.params["input2_dtype"],
            *self.params["input2_shape"]
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            *self.params["output_shape"]
        )

        if None in self.params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        input_str = self._getBufferstrCast(
            self.params['input_buf_add'], self.params['input_buf_add_offset'], dtype=self.params["input_dtype"]
        )
        input2_str = self._getBufferstrCast(
            self.params['input2_buf_add'], self.params['input2_buf_add_offset'], dtype=self.params["input2_dtype"]
        )
        output_str = self._getBufferstrCast(
            self.params['output_buf_add'], self.params['output_buf_add_offset'], dtype=self.params["output_dtype"]
        )

        # Convert boolean parameters to C++-style true/false
        adj_x = 'true' if self.params['adj_x'] else 'false'
        adj_y = 'true' if self.params['adj_y'] else 'false'

        string = (
            f"batch_matmul({input_str}, {input2_str}, {output_str}, "
            f"{self.params['input_shape'][0]}, {self.params['input_shape'][1]}, {self.params['input_shape'][2]}, "
            f"{self.params['input2_shape'][1]}, "
            f"{adj_x}, {adj_y});\n"
        )

        return string
