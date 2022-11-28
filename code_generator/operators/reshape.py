import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    # op related
    "op": "RESHAPE",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "output_dim": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
}


class reshape(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["input_c"],
            self.params["input_w"],
            self.params["input_h"],
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["output_c"],
            self.params["output_w"],
            self.params["output_h"],
        )
        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        if params["input_dtype"] == "float32":
            if params["output_w"] == params["output_h"] == 1:
                string = (
                    f"reshape_3dto1d({self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                )
                string += f"{params['input_h']},{params['input_w']},{params['input_c']},"
                string += f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
            else:
                raise NotImplementedError
        else:
            raise NotImplementedError
        return string
