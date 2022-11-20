import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["exp"]

default_params = {
    # op related
    "op": "EXP",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_size": None,
    "input_dtype": "float32",
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


class exp(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(self.params["input_idx"], self.params["input_dtype"], self.params["input_size"], 1, 1)
        self._add_output(self.params["output_idx"], self.params["output_dtype"], self.params["input_size"], 1, 1)

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        if params["input_dtype"] == "float32":
            string = (
                f"tte_exp({self.params['input_size']},"
                + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
            )
        else:
            raise NotImplementedError
        return string
