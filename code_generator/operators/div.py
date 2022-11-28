import warnings

from numpy import iterable

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["div"]

default_params = {
    # op related
    "op": "DIV",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_size": None,
    "input2": None,
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
    # input of scale from some conv2d
    "scale_from_add": None,
}


class div(basicOperator):
    div_const_cnt = 0

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
        string = ""

        if params["input_dtype"] == "float32":
            if self.params["scale_from_add"] is not None:
                scale_divisor = f"{self.params['scale_from_add']}"
                string += (
                    f"fptr3 = {self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])}; "
                    + f"fptr2 = {self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])};\n"
                )
                string += (
                    f"for(int i = 0; i < {self.params['input_size']}; i++) *fptr3++ = *fptr2++ / {scale_divisor};\n"
                )
                return string
            elif "constant" in self.params["input2_idx"]:
                string += f"const float fptr{div.div_const_cnt}[] = " + " {"
                if iterable(self.params["input2"]):
                    for v in self.params["input2"]:
                        string += f"{str(v)},"
                else:
                    string += f"{str(self.params['input2'])},"
                string += "};\n"
                input2_str = f"fptr{div.div_const_cnt}"
                div.div_const_cnt += 1
            else:
                input2_str = f"{self._getBufferstrCast(params['input2_buf_add'], params['input2_buf_add_offset'])}"

            string += (
                f"div_fp({self.params['input_size']},"
                + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                + f"{input2_str},"
                + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
            )
        else:
            raise NotImplementedError

        return string
