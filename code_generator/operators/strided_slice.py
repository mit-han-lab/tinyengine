import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["stridedSlice"]

default_params = {
    # op related
    "op": "STRIDED_SLICE",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "d1": None,
    "d2": None,
    "d3": None,
    "d4": None,
    "output_dim": None,
    "o_d1": None,
    "o_d2": None,
    "o_d3": None,
    "o_d4": None,
    "begin": None,
    "end": None,
    "strides": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
}


class stridedSlice(basicOperator):
    ss_cnt = 0

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
            self.params["o_d1"],
            self.params["o_d2"],
            self.params["o_d3"] * self.params["o_d4"],
        )

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        if params["input_dtype"] == "float32":
            function_name = "strided_slice_4Dto4D"
        elif params["input_dtype"] == "int8":
            function_name = "strided_slice_4Dto4D_int8"
        else:
            raise NotImplementedError

        string = (
            f"const uint16_t begin{stridedSlice.ss_cnt}[] = "
            + "{"
            + str(params["begin"]).replace("[", "").replace("]", "").replace("'", "")
            + "};\n"
        )
        string += (
            f"const uint16_t end{stridedSlice.ss_cnt}[] = "
            + "{"
            + str(params["end"]).replace("[", "").replace("]", "").replace("'", "")
            + "};\n"
        )
        string += (
            f"const uint16_t strides{stridedSlice.ss_cnt}[] = "
            + "{"
            + str(params["strides"]).replace("[", "").replace("]", "").replace("'", "")
            + "};\n"
        )
        input_str = self._getBufferstrCast(
            params["input_buf_add"], params["input_buf_add_offset"], dtype=params["input_dtype"]
        )
        output_str = self._getBufferstrCast(
            params["output_buf_add"], params["output_buf_add_offset"], dtype=params["output_dtype"]
        )
        string += (
            f"{function_name}({input_str},{str(params['d1'])},{str(params['d2'])},"
            + f"{str(params['d3'])},{str(params['d4'])},"
            + f"begin{stridedSlice.ss_cnt},end{stridedSlice.ss_cnt},strides{stridedSlice.ss_cnt},"
            + f"{output_str},{str(params['o_d1'])},{str(params['o_d2'])},{str(params['o_d3'])},"
            + f"{str(params['o_d4'])});\n"
        )
        stridedSlice.ss_cnt += 1

        return string
