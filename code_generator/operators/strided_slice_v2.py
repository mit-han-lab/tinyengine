import warnings
from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts
import numpy as np
__all__ = ["stridedSlice"]

default_params = {
    # op related
    "op": "STRIDED_SLICE",
    "input_idx": None,
    "output_idx": None,
    # tensor related
    "input_dim": None,
    "d1": None,
    "d2": None,
    "d3": None,
    "d4": None,
    "output_dim": None,
    "o_d1": None,
    "o_d2": None,
    "o_d3": None,
    "o_d4": None,
    "begin": None,
    "end": None,
    "strides": None,
    "input_dtype": "float32",
    "output_dtype": "float32",
}

class StridedSliceOperator(basicOperator):
    ss_cnt = 0

    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        # handle input/output tensors in HWC format
        self._add_input(
            self.params["input_idx"],
            self.params["input_dtype"],
            self.params["d1"],
            self.params["d2"],
            self.params["d3"] * self.params["d4"],
        )
        self._add_output(
            self.params["output_idx"],
            self.params["output_dtype"],
            self.params["o_d1"],
            self.params["o_d2"],
            self.params["o_d3"] * self.params["o_d4"],
        )

        if None in default_params.values():
            warnings.warn(f"parameters are not all set for op {self.params['op']}")

    def generate_inference_str(self):
        params = self.params
        if params["input_dtype"] == "float32":
            function_name = "strided_slice_4Dto4D"
        elif params["input_dtype"] == "int8":
            function_name = "strided_slice_4Dto4D_int8"
        elif params["input_dtype"] == "int32":
            function_name = "strided_slice_4Dto4D_int32"
        else:
            raise NotImplementedError

        #Flatten the arrays and format them correctly
        def flatten_and_format_array(array_data):
            formatted_array = []
            for item in array_data:                
                if isinstance(item, (list, np.ndarray)):  # array 처리 조건 추가
                    formatted_array.extend(flatten_and_format_array(item))
                elif isinstance(item, (int, np.int8, np.int32, np.uint8)):
                    formatted_array.append(item)
                else:
                    print(type(item))
                    raise ValueError("Array contains non-integer elements")
            return formatted_array

        def format_array(array_name, array_data):
            flattened_array = flatten_and_format_array(array_data)
            formatted_array_str = ", ".join(map(str, flattened_array))
            return f"const uint16_t {array_name}{StridedSliceOperator.ss_cnt}[] = {{{formatted_array_str}}};\n"

        begin_str = format_array("begin", params["begin"])
        end_str = format_array("end", params["end"])
        strides_str = format_array("strides", params["strides"])
        
        # 이 부분에서 실제로 출력하여 확인
        print("begin_str:", begin_str)
        print("end_str:", end_str)
        print("strides_str:", strides_str)

        input_str = self._getBufferstrCast(
            params["input_buf_add"], params["input_buf_add_offset"], dtype=params["input_dtype"]
        )
        output_str = self._getBufferstrCast(
            params["output_buf_add"], params["output_buf_add_offset"], dtype=params["output_dtype"]
        )

        inference_str = (
            f"{begin_str}"
            f"{end_str}"
            f"{strides_str}"
            f"{function_name}({input_str}, {params['d1']}, {params['d2']}, {params['d3']}, {params['d4']}, "
            f"begin{StridedSliceOperator.ss_cnt}, end{StridedSliceOperator.ss_cnt}, strides{StridedSliceOperator.ss_cnt}, "
            f"{output_str}, {params['o_d1']}, {params['o_d2']}, {params['o_d3']}, {params['o_d4']});\n"
        )
        
        StridedSliceOperator.ss_cnt += 1
        return inference_str
