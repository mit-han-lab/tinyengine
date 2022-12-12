# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   InputResizer.py
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

import math


def _find_previous_info(layers, idx):
    for layer in layers:
        info = layer.get_layer_info()
        if info["output_idx"] == idx:
            return info


class InputResizer:
    def __init__(self, layer):
        self.layer = layer

    def inputResize(self, input_h, input_w):
        for i, layer in enumerate(self.layer):
            layer_info = layer.get_layer_info()

            previous_layer_info = _find_previous_info(self.layer, layer_info["input_idx"])
            # we need to handle different op
            op_code_str = layer_info["op"]
            if i == 0:
                layer_info["input_h"] = input_h
                layer_info["input_w"] = input_w
                _changeOPTensorSize(self.layer[i], "input", 0, layer_info["input_h"], layer_info["input_w"])
            else:
                if op_code_str == "SE_AVG_POOL_2D":
                    SEinput_h = previous_layer_info["output_h"]
                    SEinput_w = previous_layer_info["output_w"]
                    layer_info["input_h"] = SEinput_h
                    layer_info["input_w"] = SEinput_w
                    _changeOPTensorSize(self.layer[i], "input", 0, layer_info["input_h"], layer_info["input_w"])
                    layer_info["sample_h"] = SEinput_h
                    layer_info["sample_w"] = SEinput_w
                else:
                    layer_info["input_h"] = previous_layer_info["output_h"]
                    layer_info["input_w"] = previous_layer_info["output_w"]
                    layer_info["input_c"] = previous_layer_info["output_c"]
                    _changeOPTensorSize(self.layer[i], "input", 0, layer_info["input_h"], layer_info["input_w"])
                    if op_code_str == "AVERAGE_POOL_2D":
                        layer_info["filter_h"] = layer_info["input_h"]
                        layer_info["filter_w"] = layer_info["input_w"]
                        layer_info["filter_c"] = layer_info["input_c"]

                    # handle nodes for dag op
                    # find the previous node
                    if "dagop_input0_key" in layer_info:
                        for op in self.layer:
                            l_into = op.get_layer_info()
                            if (
                                "dagop_output_key" in l_into
                                and l_into["dagop_output_key"] == layer_info["dagop_input0_key"]
                            ):
                                layer_info["input_h"] = l_into["output_h"]
                                layer_info["input_w"] = l_into["output_w"]
                                layer_info["input_c"] = l_into["output_c"]
                    if "dagop_input1_key" in layer_info:
                        for op in self.layer:
                            l_into = op.get_layer_info()
                            if (
                                "dagop_output_key" in l_into
                                and l_into["dagop_output_key"] == layer_info["dagop_input1_key"]
                            ):
                                layer_info["input_h"] = l_into["output_h"]
                                layer_info["input_w"] = l_into["output_w"]
                                layer_info["input_c"] = l_into["output_c"]

            if op_code_str == "CONV_2D" or op_code_str == "DEPTHWISE_CONV_2D":
                layer_info["output_h"] = math.ceil(layer_info["input_h"] / layer_info["stride_h"])
                layer_info["output_w"] = math.ceil(layer_info["input_w"] / layer_info["stride_w"])
                _changeOPTensorSize(self.layer[i], "output", 0, layer_info["output_h"], layer_info["output_w"])
            elif op_code_str == "ADD":
                layer_info["output_h"] = layer_info["input_h"]
                layer_info["output_w"] = layer_info["input_w"]
                layer_info["output_c"] = layer_info["input_c"]
                _changeOPTensorSize(self.layer[i], "output", 0, layer_info["output_h"], layer_info["output_w"])
                layer_info["input2_h"] = layer_info["input_h"]
                layer_info["input2_w"] = layer_info["input_w"]
                _changeOPTensorSize(self.layer[i], "input", 1, layer_info["input2_h"], layer_info["input_w"])
            elif op_code_str == "SE_ELEMENT_MULT_2D":
                layer_info["input2_h"] = SEinput_h
                layer_info["input2_w"] = SEinput_w
                _changeOPTensorSize(self.layer[i], "input", 1, layer_info["input2_h"], layer_info["input_w"])
                layer_info["output_h"] = SEinput_h
                layer_info["output_w"] = SEinput_w
                _changeOPTensorSize(self.layer[i], "output", 0, layer_info["output_h"], layer_info["output_w"])
            elif op_code_str == "UPSAMPLE":
                layer_info["output_h"] = layer_info["input_h"] * layer_info["factor"]
                layer_info["output_w"] = layer_info["input_w"] * layer_info["factor"]
                layer_info["output_c"] = layer_info["input_c"]
                _changeOPTensorSize(self.layer[i], "output", 0, layer_info["output_h"], layer_info["output_w"])
            elif op_code_str == "MAX_POOL_2D":
                layer_info["output_h"] = int(layer_info["input_h"] / layer_info["filter_h"])
                layer_info["output_w"] = int(layer_info["input_w"] / layer_info["filter_h"])
                layer_info["output_c"] = layer_info["input_c"]
                _changeOPTensorSize(self.layer[i], "output", 0, layer_info["output_h"], layer_info["output_w"])


def _changeOPTensorSize(layer, tensor_type: str, tensor_idx: int, input_h: int, input_w: int):
    if tensor_type == "input":
        if hasattr(layer, "input_tensors") and len(layer.input_tensors) > tensor_idx:
            layer.input_tensors[tensor_idx].set_input_w(input_w)
            layer.input_tensors[tensor_idx].set_input_h(input_h)
    elif tensor_type == "output":
        if hasattr(layer, "output_tensors"):
            layer.output_tensors[tensor_idx].set_input_w(input_w)
            layer.output_tensors[tensor_idx].set_input_h(input_h)


class PatchResizer:
    def __init__(self, layer):
        self.layer = layer

    # manually setting these variables for now
    def patchResize(self, PatchLayers, PatchSize, PatchSize_height):
        for i, layer in enumerate(self.layer):
            layer_info = layer.get_layer_info()
            if i < PatchLayers:
                layer_info["is_patch"] = True
                op_code_str = layer_info["op"]
                if i == 0:
                    layer_info["input_h"] = PatchSize_height
                    layer_info["input_w"] = PatchSize
                    _changeOPTensorSize(self.layer[i], "input", 0, PatchSize_height, PatchSize)
                else:
                    prev_layer_info = self.layer[i - 1].get_layer_info()
                    layer_info["input_h"] = prev_layer_info["output_h"]
                    layer_info["input_w"] = prev_layer_info["output_w"]
                    _changeOPTensorSize(
                        self.layer[i], "input", 0, prev_layer_info["output_h"], prev_layer_info["output_w"]
                    )

                if op_code_str == "CONV_2D" or op_code_str == "DEPTHWISE_CONV_2D":
                    layer_info["output_h"] = math.ceil(
                        (layer_info["input_h"] - layer_info["kernel_h"] + 1) / layer_info["stride_h"]
                    )
                    layer_info["output_w"] = math.ceil(
                        (layer_info["input_w"] - layer_info["kernel_w"] + 1) / layer_info["stride_w"]
                    )
                    _changeOPTensorSize(self.layer[i], "output", 0, layer_info["output_h"], layer_info["output_w"])
                elif op_code_str == "ADD":
                    layer_info["output_h"] = layer_info["input_h"]
                    layer_info["output_w"] = layer_info["input_w"]
                    layer_info["input2_h"] = layer_info["input_h"]
                    layer_info["input2_w"] = layer_info["input_w"]
                    _changeOPTensorSize(self.layer[i], "input", 0, layer_info["input_h"], layer_info["input_w"])
                    _changeOPTensorSize(self.layer[i], "input", 1, layer_info["input_h"], layer_info["input_w"])
            else:
                layer_info["is_patch"] = False

        # We need to:
        # 1. cut off the link between patch blocks and normal inference blocks,
        # 2. set the lifetime of the input tensor of the first layer in the second stage to start from begging
        # so the memory buffers can be allocated successfully
        if PatchLayers > 0:
            self.layer[PatchLayers].params["input_idx"] = (
                str(self.layer[PatchLayers].params["input_idx"]) + "_start_normal_infernece_block"
            )
            self.layer[PatchLayers].input_tensors[0].graph_idx = self.layer[PatchLayers].params["input_idx"]
            self.layer[PatchLayers].params["is_start_of_normal_inference_block"] = True
