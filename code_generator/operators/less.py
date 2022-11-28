import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    # op related
    "op": "LESS",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_size": None,
    "output_size": None,
    "input_dtype": "float32",
    "input2_dtype": "float32",
    "output_dtype": "float32",
    # quantization related
    "weight_value": None,
    "bias": None,
    "input_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "output_scale": None,
    "multiplier": None,
    "shift": None,
}


class less(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(self.params["input_idx"], self.params["input_dtype"], self.params["input_size"], 1, 1)
        self._add_input(self.params["input2_idx"], self.params["input2_dtype"], self.params["input_size"], 1, 1)
        self._add_output(self.params["output_idx"], self.params["output_dtype"], self.params["input_size"], 1, 1)

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        input1_str = self._getBufferstrCast(
            params["input_buf_add"], params["input_buf_add_offset"], dtype=self.input_tensors[0].dtype
        )
        input2_str = self._getBufferstrCast(
            params["input2_buf_add"], params["input2_buf_add_offset"], dtype=self.input_tensors[1].dtype
        )
        output_str = self._getBufferstrCast(
            params["output_buf_add"], params["output_buf_add_offset"], dtype=self.output_tensors[0].dtype
        )
        if params["input_dtype"] == "float32":
            string = f"less({input1_str},{input2_str},{output_str});\n"
        else:
            raise NotImplementedError

        return string
