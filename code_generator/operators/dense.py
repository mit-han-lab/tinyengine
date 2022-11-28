import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

default_params = {
    # op related
    "op": "DENSE",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "matA_row": None,
    "matA_col": None,
    "matB_row": None,
    "matB_col": None,
    "weight_value": None,
    "weight_name": None,
    "input_dtype": "int8",
    "input2_dtype": "int8",
    "output_dtype": "int8",
}


class dense(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"], self.params["input_dtype"], self.params["matA_row"], self.params["matA_col"], 1
        )
        self._add_output(
            self.params["output_idx"], self.params["output_dtype"], self.params["matA_row"], self.params["matB_row"], 1
        )

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def get_macs(self) -> int:
        p = self.params
        return p["matA_row"] * p["matB_row"] * p["matA_col"]

    def get_weights_size(self) -> int:
        p = self.params
        if p["input_dtype"] in {"float32", "fp32"}:
            size = 4
        else:
            size = 1
        return p["matB_row"] * p["matB_col"] * size

    def generate_inference_str(self):
        params = self.params
        if params["input_dtype"] == "float32":
            string = f"dense({self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])},"
            string += f"{str(params['matA_row'])},{str(params['matA_col'])},"
            string += f"weight_fp{params['parsed_trainable']},{str(params['matB_row'])},"
            string += f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])});\n"
        else:
            raise NotImplementedError

        return string
