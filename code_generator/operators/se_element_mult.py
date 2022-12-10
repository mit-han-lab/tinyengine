import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["SEelementmult"]

default_params = {
    # op related
    "op": "SE_ELEMENT_MULT_2D",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "input2_dim": None,
    "input2_h": None,
    "input2_w": None,
    "input2_c": None,
    "output_dim": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "input_dtype": "int8",
    "input2_dtype": "int8",
    "output_dtype": "int8",
    # quantization related
    "input_zero_point": None,
    "input2_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "input2_scale": None,
    "output_scale": None,
    "input_multiplier": None,
    "input2_multiplier": None,
    "output_multiplier": None,
    "input_effective_scale": None,
    "input2_effective_scale": None,
    "output_effective_scale": None,
    "input_shift": None,
    "input2_shift": None,
    "output_shift": None,
    "left_shift": None,
}


class SEelementmult(basicOperator):
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
        self._add_input(
            self.params["input2_idx"],
            self.params["input2_dtype"],
            self.params["input2_c"],
            self.params["input2_w"],
            self.params["input2_h"],
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

    def generate_inference_str(self):
        string = ""
        params = self.params
        string += (
            f"element_mult_nx1({self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
            + f"{str(int(params['input_h']))},"
            + f"{str(int(params['input_w']))},"
            f"{str(int(params['input_c']))},"
            + f"{self._getBufferstr(params['input2_buf_add'], params['input2_buf_add_offset'])},"
            + f"{str(params['input_zero_point'])},{str(params['input2_zero_point'])},"
            + f"{str(params['output_zero_point'])},-127,128,"
            + f"{str(params['output_effective_scale'])},"
            + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])});\n"
        )
        return string
