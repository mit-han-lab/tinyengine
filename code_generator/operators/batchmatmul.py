from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["batchmatmul"]

default_params = {
    # op related
    "op": "BATCH_MATMUL",
    "batch_size": None,
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_col": None,
    "input_row": None,
    "input2": None,
    "input2_col": None,
    "input2_row": None,
    "output_col": None,
    "output_row": None,
    "input_dtype": "fp32",
    "input2_dtype": "fp32",
    "output_dtype": "fp32",
}


class batchmatmul(basicOperator):
    div_const_cnt = 0

    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["batch_size"],
            self.params["input_row"],
            self.params["input_col"],
        )
        self._add_input(
            self.params["input2_idx"],
            self.params["input2_dtype"],
            self.params["batch_size"],
            self.params["input2_row"],
            self.params["input2_col"],
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["batch_size"],
            self.params["output_row"],
            self.params["output_col"],
        )

    def generate_inference_str(self):
        params = self.params
        string = ""
        input = self._getBufferstrCast(params["input_buf_add"], params["input_buf_add_offset"], params["input_dtype"])
        input = f"{input}"
        input_col = self.params["input_col"]
        input_row = self.params["input_row"]
        input2 = self._getBufferstrCast(
            params["input2_buf_add"], params["input2_buf_add_offset"], dtype=params["input2_dtype"]
        )
        input2 = f"{input2}"
        input2_col = self.params["input2_col"]
        input2_row = self.params["input2_row"]
        output = self._getBufferstrCast(
            params["output_buf_add"], params["output_buf_add_offset"], dtype=params["output_dtype"]
        )
        output = f"{output}"
        output_col = self.params["output_col"]
        output_row = self.params["output_row"]

        if params["input_dtype"] == "float32":
            string += (
                f"batchmatmul_fp({params['batch_size']},{input2},{input2_col},{input2_row},"
                f"{input},{input_col},{input_row},{output},{output_col},{output_row});\n"
            )
        else:
            raise NotImplementedError

        return string
