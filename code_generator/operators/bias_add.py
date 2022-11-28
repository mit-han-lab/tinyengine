import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    # op related
    "op": "BIAS_ADD",
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
    "input_dtype": "float32",
    "output_dtype": "float32",
    # quantization related
    "bias": None,
    "bias_name": None,
    "input_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "output_scale": None,
    "multiplier": None,
    "shift": None,
}


class biasAdd(basicOperator):
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

    def get_macs(self) -> int:
        p = self.params
        return p["output_h"] * p["output_w"] * p["output_c"]

    def get_bias_size(self) -> int:
        p = self.params
        return 4 * p["output_c"]

    def generate_inference_str(self):
        params = self.params

        if params["input_dtype"] == "float32":
            string = (
                f"bias_add_3D({self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                + f"{str(params['input_h'])},{str(params['input_w'])},"
                + f"{str(params['input_c'])},bias_fp{params['parsed_trainable']},"
                + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
            )
        else:
            raise NotImplementedError

        return string
