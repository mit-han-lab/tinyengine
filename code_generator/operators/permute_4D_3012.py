import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["permute_4D_3012"]

default_params = {
    # op related
    "op": "permute_4D_3012",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "d1": None,
    "d2": None,
    "d3": None,
    "d4": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
}


class permute_4D_3012(basicOperator):
    static_cnt = 0

    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["d1"],
            self.params["d2"],
            self.params["d3"] * self.params["d4"],
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["d1"],
            self.params["d2"],
            self.params["d3"] * self.params["d4"],
        )
        permute_4D_3012.static_cnt += 1

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        if params["input_dtype"] == "float32":
            string = (
                f"permute4D_dim3012({self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
            )
            string += f"{params['d1']},{params['d3']},{params['d4']},{params['d2']},"  # OIHW -> OHWI
            string += f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
        else:
            raise NotImplementedError

        return string
