import warnings

from ..constant import USE_BIT_MASK
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["where"]

default_params = {
    # op related
    "op": "WHERE",
    "input_idx": None,
    "input2_idx": None,
    "input3_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "input_size": None,
    "output_dim": None,
    "output_size": None,
    "input_dtype": "float32",
    "input2_dtype": "float32",
    "input3_dtype": "float32",
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
    # fusion with zeros
    "input3_is_zeros": False,
    # inplace update
    "inplace": False,
}


class where(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(self.params["input_idx"], self.params["input_dtype"], self.params["input_size"], 1, 1)
        self._add_input(self.params["input2_idx"], self.params["input2_dtype"], self.params["input_size"], 1, 1)
        self._add_input(self.params["input3_idx"], self.params["input3_dtype"], self.params["input_size"], 1, 1)
        self._add_output(self.params["output_idx"], self.params["output_dtype"], self.params["output_size"], 1, 1)

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        # function_name
        input_str = self._getBufferstrCast(
            params["input_buf_add"], params["input_buf_add_offset"], dtype=self.input_tensors[0].dtype
        )
        input2_str = self._getBufferstrCast(
            params["input2_buf_add"], params["input2_buf_add_offset"], dtype=self.input_tensors[1].dtype
        )

        if params["input3_is_zeros"]:
            if params["output_dtype"] == "int8":
                function_name = "where_zeros_int8"
            elif params["output_dtype"] == "int32":
                function_name = "where_zeros_int32"
            elif params["output_dtype"] == "float32":
                function_name = "where_zeros"
            if params["inplace"]:
                function_name = f"{function_name}_inplace_bit" if USE_BIT_MASK else "function_name_inplace"
                string = f"{function_name}({input_str},{params['input_size']},{input2_str});\n"
            else:
                if USE_BIT_MASK:
                    raise NotImplementedError
                output_str = self._getBufferstrCast(
                    params["output_buf_add"], params["output_buf_add_offset"], dtype=self.output_tensors[0].dtype
                )
                string = f"{function_name}({input_str},{params['input_size']},{input2_str},{output_str});\n"
        else:
            if params["output_dtype"] != "float32":
                raise NotImplementedError

            function_name = "where"
            output_str = self._getBufferstrCast(
                params["output_buf_add"], params["output_buf_add_offset"], dtype=self.output_tensors[0].dtype
            )
            input3_str = self._getBufferstrCast(
                params["input3_buf_add"], params["input3_buf_add_offset"], dtype=self.input_tensors[2].dtype
            )
            string = f"{function_name}({input_str},{params['input_size']},"
            string += f"{input2_str},{input3_str},{output_str});\n"

        return string
