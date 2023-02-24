from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["slice"]

default_params = {
    # op related
    "op": "SLICE",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "begin_h": None,
    "begin_w": None,
    "begin_c": None,
    "end_h": None,
    "end_w": None,
    "end_c": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "input_dtype": "fp32",
    "output_dtype": "fp32",
}


class slice(basicOperator):
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
        begin_h = f"{str(params['begin_h'])}"
        begin_w = f"{str(params['begin_w'])}"
        begin_c = f"{str(params['begin_c'])}"
        output = f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])}"
        output_h = f"{str(params['output_h'])}"
        output_w = f"{str(params['output_w'])}"
        output_c = f"{str(params['output_c'])}"

        if params["input_dtype"] == "float32":
            if params["input_dtype"] == "float32":
                function_name = "slice3d_fp"
            string += (
                f"{function_name}({input},{input_h},{input_w},{input_c},"
                + f"{begin_h},{begin_w},{begin_c},{output},{output_h},{output_w},{output_c});\n"
            )
        else:
            raise NotImplementedError

        return string
