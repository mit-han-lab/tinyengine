import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    # op related
    "op": "ADD1D",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_size": None,
    "input_dtype": "int8",
    "input2_dtype": "int8",
    "output_dtype": "int8",
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


class add1d(basicOperator):
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

    def get_macs(self) -> int:
        p = self.params
        return p["input_size"]

    def generate_inference_str(self):
        params = self.params
        if params["input_dtype"] == "float32":
            string = (
                f"add_fp({self.params['input_size']},"
                + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                + f"{self._getBufferstrCast(params['input2_buf_add'], params['input2_buf_add_offset'])},"
                + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
            )
        else:
            raise NotImplementedError

        return string
