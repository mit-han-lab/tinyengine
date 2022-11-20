import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    # op related
    "op": "COLLAPSE_SUM_LIKE",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "output_dim": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "input_dtype": "int8",
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


class collapseSumLike(basicOperator):
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

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        size = params["output_h"] * params["output_w"] * params["output_c"]

        if params["input_dtype"] == "float32":
            string = f"fptr = (float*){self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])};"
            string += f"fptr2 = (float*){self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])};"
            string += f"for(int i = 0; i < {size}; i++) fptr[i] = fptr2[i];\n"
        else:
            raise NotImplementedError

        return string
