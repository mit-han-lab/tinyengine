import warnings

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["bias_update"]

default_params = {
    # op related
    "op": "BIAS_UPDATE",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_size": None,
    "input_buf_add": None,
    "input_buf_add_offset": None,
    "QAS": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
}


class bias_update(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["input_size"],
            1,
            1,
        )
        if None in default_params:
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        # generate string
        string = f"const float {self.params['output_idx']}_QAS[{self.params['input_size']}] = " + "{"
        for i in 1 / self.params["QAS"]:
            string += str(i) + ","
        string += "};\n"
        input_str = self._getBufferstr(params["input_buf_add"], params["input_buf_add_offset"])
        if params["input_dtype"] == "float32":
            string += (
                f"fptr = (float*){input_str};//gradients\n"
                + f"for(int i = 0; i < {self.params['input_size']}; i++) {self.params['output_idx']}[i] -="
                + f" (int32_t)(fptr[i] * blr * {self.params['output_idx']}_QAS[i]);\n"
            )
        elif params["input_dtype"] == "int32":
            string += (
                f"int32ptr = (int32_t*){input_str};//gradients\n"
                + f"for(int i = 0; i < {self.params['input_size']}; i++) {self.params['output_idx']}[i] -="
                + f" (int32_t)(int32ptr[i] * blr * {self.params['output_idx']}_QAS[i]);\n"
            )
        else:
            raise NotImplementedError

        return string
