from ..constant import USE_BIT_MASK
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["Add"]

default_params = {
    # op related
    "op": "ADD",
    "input_idx": None,
    "input2_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "input2": None,
    "input2_dim": None,
    "input2_h": None,
    "input2_w": None,
    "input2_c": None,
    "output_dim": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "input_dtype": "int8",
    "input2_dtype": "int8",
    "output_dtype": "int8",
    # quantization related
    "input_zero_point": None,
    "input2_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "input2_scale": None,
    "output_scale": None,
    "input_multiplier": None,
    "input2_multiplier": None,
    "output_multiplier": None,
    "input_effective_scale": None,
    "input2_effective_scale": None,
    "output_effective_scale": None,
    "input_shift": None,
    "input2_shift": None,
    "output_shift": None,
    "left_shift": None,
    # fof Q training
    "need_Bmask": False,
    "output2_h": None,
    "output2_w": None,
    "output2_c": None,
    "output2_idx": None,
    "output2_dtype": "int8",
}


class Add(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["input_c"],
            self.params["input_w"],
            self.params["input_h"],
        )
        self._add_input(
            self.params["input2_idx"],
            self.params["input2_dtype"],
            self.params["input2_c"],
            self.params["input2_w"],
            self.params["input2_h"],
        )
        # TODO: Refactor this
        if self.input_tensors[1].constant():
            self.input_tensors[1].set_data(self.params["input2"], self.params["input2_idx"])
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["output_c"],
            self.params["output_w"],
            self.params["output_h"],
        )

    def get_macs(self) -> int:
        p = self.params
        return p["output_h"] * p["output_w"] * p["output_c"]

    def generate_inference_str(self):
        string = ""
        params = self.params
        if params["need_Bmask"]:
            if USE_BIT_MASK:
                string += (
                    f"add_fpreq_bitmask({str(int(params['input_h']*params['input_w']*params['input_c']))}, "
                    + f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
                )
            else:
                string += f"add_fpreq_mask({str(int(params['input_h']*params['input_w']*params['input_c']))}, "
                +f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
            string += (
                f"{str(params['input_scale'])},{str(params['input_zero_point'])},"
                + f"{self._getBufferstr(params['input2_buf_add'], params['input2_buf_add_offset'])},"
                + f"{str(params['input2_scale'])},{str(params['input2_zero_point'])},"
                + f"{str(params['output_scale'])},{str(params['output_zero_point'])},"
                + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])},"
                + f"{self._getBufferstr(params['output2_buf_add'], params['output2_buf_add_offset'])});\n"
            )
        else:
            if isinstance(params["input2_idx"], str) and "constant" in params["input2_idx"]:
                t = self.input_tensors[1]
                assert t.data is not None
                # elementwise add
                if t.num_elements() == self.input_tensors[0].num_elements():
                    string += (
                        f"add_fp({str(int(params['input_h']*params['input_w']*params['input_c']))}, "
                        + f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + f"{t.graph_idx},"
                        + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                    )
                # scaler or vector based
                else:
                    raise NotImplementedError("add with scaler/vector constant support is still no ready.")
            else:
                if params["input_dtype"] == "int8":
                    string += (
                        f"add_fpreq({str(int(params['input_h']*params['input_w']*params['input_c']))}, "
                        + f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + f"{str(params['input_scale'])},{str(params['input_zero_point'])},"
                        + f"{self._getBufferstr(params['input2_buf_add'], params['input2_buf_add_offset'])},"
                        + f"{str(params['input2_scale'])},{str(params['input2_zero_point'])},"
                        + f"{str(params['output_scale'])},{str(params['output_zero_point'])},"
                        + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                    )
                elif params["input_dtype"] == "float32":
                    string += (
                        f"add_fp({str(int(params['input_h']*params['input_w']*params['input_c']))}, "
                        + f"{self._getBufferstr(params['input_buf_add'], params['input_buf_add_offset'])},"
                        + f"{self._getBufferstr(params['input2_buf_add'], params['input2_buf_add_offset'])},"
                        + f"{self._getBufferstr(params['output_buf_add'], params['output_buf_add_offset'])});\n"
                    )
        return string
