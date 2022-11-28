import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["sum"]

default_params = {
    # op related
    "op": "SUM",
    "input_idx": None,
    "axis": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "output_dim": None,
    "d1": None,
    "d2": None,
    "d3": None,
    "d4": None,
    "od1": None,
    "od2": None,
    "od3": None,
    "od4": None,
    "output_size": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
    "exclude": False,
}


class sum(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["d2"] * self.params["d1"],
            self.params["d3"],
            self.params["d4"],
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["od2"] * self.params["od1"],
            self.params["od3"],
            self.params["od4"],
        )

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def get_macs(self):
        p = self.params
        return p["d1"] * p["d2"] * p["d3"] * p["d4"]

    def generate_inference_str(self):
        params = self.params
        # OIHW -> OHWI parameter conversion
        if params["input_dtype"] == "float32":
            if params["d1"] == 1 and not params["exclude"]:  # use 3D fallback
                mapping = {"0": None, "1": 2, "2": 0, "3": 1}
                string = f"sum_3D({self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                string += (
                    f"{self.params['d3']},{self.params['d4']},{self.params['d2']},{mapping[str(self.params['axis'])]},"
                )
                string += f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
            elif params["exclude"]:
                mapping = {"0": 0, "1": 3, "2": 1, "3": 2}
                string = (
                    f"sum_4D_exclude({self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                )
                string += (
                    f"{self.params['d1']},{self.params['d3']},{self.params['d4']},{self.params['d2']},"
                    + f"{mapping[str(self.params['axis'])]},"
                    + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                )
        else:
            # For int8 gradients
            if params["exclude"]:
                mapping = {"0": 0, "1": 3, "2": 1, "3": 2}
                string = (
                    "sum_4D_exclude_int8("
                    + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'], 'int8')},"
                    + f"{self.params['d1']},{self.params['d3']},{self.params['d4']},{self.params['d2']},"
                    + f"{mapping[str(self.params['axis'])]},"
                    + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'], 'int32')}"
                    + ");\n"
                )
            else:
                raise NotImplementedError
        return string
