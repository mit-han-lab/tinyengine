from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["squarddiff"]

default_params = {
    # op related
    "op": "SQUARED_DIFFERENCE",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_size": None,
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "input2_h": None,
    "input2_w": None,
    "input2_c": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "input2": None,
    "input_dtype": "fp32",
    "input2_dtype": "fp32",
    "output_dtype": "fp32",
}


class squarddiff(basicOperator):
    div_const_cnt = 0

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
        if not (isinstance(self.params["input2_idx"], str) and "constant" not in self.params["input2_idx"]):
            self._add_input(
                self.params["input2_idx"],
                self.params["input2_dtype"],
                self.params["input2_h"],
                self.params["input2_w"],
                self.params["input2_c"],
            )
            # TODO: Refactor this
            if self.input_tensors[1].constant():
                self.input_tensors[1].set_data(self.params["input2"], self.params["input2_idx"])
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["output_h"],
            self.params["output_w"],
            self.params["output_c"],
        )

    def generate_inference_str(self):
        params = self.params
        string = ""
        input = f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])}"
        input_h = f"{str(params['input_h'])}"
        input_w = f"{str(params['input_w'])}"
        input_c = f"{str(params['input_c'])}"
        if self.input_tensors[1].constant():
            input2 = f"{self.input_tensors[1].graph_idx}"
        else:
            input2 = f"{self._getBufferstrCast(params['input2_buf_add'], params['input2_buf_add_offset'])}"
        input2_h = f"{str(params['input2_h'])}"
        input2_w = f"{str(params['input2_w'])}"
        input2_c = f"{str(params['input2_c'])}"
        output = f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])}"
        output_h = f"{str(params['output_h'])}"
        output_w = f"{str(params['output_w'])}"
        output_c = f"{str(params['output_c'])}"

        if params["input_dtype"] == "float32":
            if params["input_dtype"] == "float32":
                function_name = "squarddiff_fp"
            string += (
                f"{function_name}({input},{input_h},{input_w},{input_c},"
                + f"{input2},{input2_h},{input2_w},{input2_c},{output},{output_h},{output_w},{output_c});\n"
            )
        else:
            raise NotImplementedError
        return string
