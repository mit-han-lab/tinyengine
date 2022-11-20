import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    # op related
    "op": "PERMUTE_GROUPCONV_OUT",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "groups": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
}


class permute_groupconv_out(basicOperator):
    static_cnt = 0

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
            f"permute_groupconv_out{permute_groupconv_out.static_cnt}",
            self.params["output_dtype"],
            self.params["input_c"],
            self.params["input_h"],
            self.params["input_w"],
        )
        permute_groupconv_out.static_cnt += 1

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        if params["input_dtype"] == "float32":
            string = (
                "permute_groupconv_out("
                + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                + f"{params['input_h']},{params['input_w']},{params['input_c']},"
                + f"{int(params['input_c']/params['groups'])},{params['groups']},"
                + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
            )
        else:
            raise NotImplementedError

        return string
