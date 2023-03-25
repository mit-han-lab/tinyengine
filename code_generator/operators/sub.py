from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["sub"]

default_params = {
    # op related
    "op": "SUB",
    "input_idx": None,
    "input2": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_size": None,
    "input2_size": None,
    "input_dtype": "float32",
    "input2_dtype": "float32",
    "output_dtype": "float32",
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


class sub(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(self.params["input_idx"], self.params["input_dtype"], self.params["input_size"], 1, 1)
        self._add_input(self.params["input2_idx"], self.params["input2_dtype"], self.params["input2_size"], 1, 1)
        # TODO: Refactor this
        if self.input_tensors[1].constant():
            self.input_tensors[1].set_data(self.params["input2"], self.params["input2_idx"])
        self._add_output(self.params["output_idx"], self.params["output_dtype"], self.params["input_size"], 1, 1)

    def generate_inference_str(self):
        params = self.params
        string = ""
        if isinstance(params["input2_idx"], str) and "constant" in params["input2_idx"]:
            t = self.input_tensors[1]
            assert t.data is not None
            # elementwise add
            if t.num_elements() == self.input_tensors[0].num_elements():
                string += (
                    f"sub_fp({str(int(params['input_size']))},"
                    + f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{t.graph_idx},"
                    + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                )
            # scaler or vector based
            elif t.num_elements() == 1:
                string += (
                    f"sub_scaler_fp({str(int(params['input_size']))},"
                    + f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
                    + f"{t.graph_idx},"
                    + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                )
            else:
                raise NotImplementedError("sub with scaler/vector constant support is still no ready.")
            return string
        if params["input_dtype"] == "float32":
            string = (
                f"sub({self.params['input_size']},"
                + f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
                + f"{self._getBufferstrCast(params['input2_buf_add'], params['input2_buf_add_offset'])},"
                + f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
            )
        else:
            raise NotImplementedError
        return string
