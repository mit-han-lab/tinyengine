# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   softmax.py
#
# Reference papers:
#  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
#  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
#  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
# Contact authors:
#  - Wei-Ming Chen, wmchen@mit.edu
#  - Wei-Chen Wang, wweichen@mit.edu
#  - Ji Lin, jilin@mit.edu
#  - Ligeng Zhu, ligeng@mit.edu
#  - Song Han, songhan@mit.edu
#
# Target ISA:  ARMv7E-M
# ----------------------------------------------------------------------

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts

__all__ = ["softmax"]


default_params = {
    # op related
    "op": "SOFTMAX",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "input_h": None,
    "input_w": None,
    "input_c": None,
    "output_dim": None,
    "output_h": None,
    "output_w": None,
    "output_c": None,
    "input_dtype": None,
    "output_dtype": None,
    # quantization related
    "input_zero_point": None,
    "output_zero_point": None,
    "input_scale": None,
    "output_scale": None,
}


class softmax(basicOperator):
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
        assert self.input_tensors[0].num_elements() == self.output_tensors[0].num_elements()

    def generate_inference_str(self):
        params = self.params
        string = ""
        input = f"{self._getBufferstrCast(params['input_buf_add'], params['input_buf_add_offset'])}"
        output = f"{self._getBufferstrCast(params['output_buf_add'], params['output_buf_add_offset'])}"

        if params["input_dtype"] == "float32":
            string += f"softmax_fp({self.input_tensors[0].num_elements()},{input},{output});\n"
        else:
            raise NotImplementedError

        return string
