import warnings
from typing import Iterable

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["cast"]

default_params = {
    # op related
    "op": "ONES_LIKE",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "input_size": None,
    "output_dim": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
    "input_meta": None,
}


class cast(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        #     raise NotImplementedError
        # else:
        if (
            "constant" not in params["input_idx"]
            and "weight" not in params["input_idx"]
            and "bias" not in params["input_idx"]
        ):
            self._add_input(
                self.params["input_idx"],
                self.params["input_dtype"],
                self.params["input_size"],
                1,
                1,
            )

        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["input_size"],
            1,
            1,
        )

        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        # figure out input/output types
        if params["input_dtype"] == "float32":
            input_type_str = "float"
            input_ptr = "fptr"
        elif params["input_dtype"] == "int32" or params["input_dtype"] == "int8":
            input_type_str = "int8_t"
            input_ptr = "int8ptr"
        else:
            raise NotImplementedError
        if params["output_dtype"] == "float32":
            output_type_str = "float"
            output_ptr = "fptr"
        elif params["output_dtype"] == "int32" or params["output_dtype"] == "int8":
            output_type_str = "int8_t"
            output_ptr = "int8ptr"
        else:
            raise NotImplementedError
        # generate string
        if "constant" in params["input_idx"] and "data" in params["input_meta"]:
            array_name = params["input_idx"].replace("@", "").replace("-", "")
            array = "{"
            array_len = 0
            if isinstance(params["input_meta"]["data"], Iterable):
                for element in params["input_meta"]["data"]:
                    array += str(element) + ","
                array_len = len(params["input_meta"]["data"])
            else:
                array += str(params["input_meta"]["data"]) + ","
                array_len = 1
            array += "}"
            string = (
                f"const {input_type_str} {array_name}[] = {array};\n{output_ptr} = "
                + f"({output_type_str}*)"
                + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])};\n"
                + f"for(int i = 0; i < {array_len}; i++) {output_ptr}[i] = ({output_type_str}){array_name}[i];\n"
            )
        elif "weight" in params["input_idx"]:
            string = (
                f"{output_ptr} = ({output_type_str}*)"
                + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])};\n"
                + f"{input_ptr} = ({output_type_str}*){params['input_idx']};\n"
                f"for(int i = 0; i < {params['input_size']}; i++) "
                + f"{output_ptr}[i] = ({output_type_str}){input_ptr}[i];\n"
            )
        else:
            string = (
                f"{output_ptr} = ({output_type_str}*)"
                + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])};\n"
                + f"{input_ptr} = ({input_type_str}*)"
                + f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])};\n"
                + f"for(int i = 0; i < {params['input_size']}; i++) "
                + f"{output_ptr}[i] = ({output_type_str}){input_ptr}[i];\n"
            )

        return string
