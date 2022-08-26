# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   GeneralMemoryScheduler.py
#
# Reference papers:
#  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
#  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
#  - MCUNetV3: On-Device Training Under 256KB Memory, arXiv:2206.15472
# Contact authors:
#  - Wei-Ming Chen, wmchen@mit.edu
#  - Wei-Chen Wang, wweichen@mit.edu
#  - Ji Lin, jilin@mit.edu
#  - Ligeng Zhu, ligeng@mit.edu
#  - Song Han, songhan@mit.edu
#
# Target ISA:  ARMv7E-M
# ----------------------------------------------------------------------

from .allocator.firstFit import FirstFit
from .constant import TTYPE_INFERNECE


class GeneralMemoryScheduler:
    def __init__(
        self,
        layer,
        tflite_op=False,
        dummy_address=False,
        memory_limit=10 * 1024 * 1024,
        inplace=True,
        outputTables=None,
        mem_visual_path="codegen/allocation.png",
        VisaulizeTrainable=True,
    ):
        self.layer = layer
        self.heads = 0
        self.buffers = {
            "input_output": 0,
            "residual": 0,
            "im2col": 0,
            "kernel": 0,
            "feature": 0,
            "trainable": 0,
        }  # for feature pyramid
        # overall memory info
        self.peakmem = 0
        self.flash = 0
        self.bias = 0
        self.scale = 0
        self.code = 0
        self.allocator = FirstFit(memory_limit)
        self.outputTables = outputTables
        self.USE_INPLACE = inplace
        self.mem_visual_path = mem_visual_path
        self.tflite_op = tflite_op
        self.dummy_address = dummy_address
        self.VisaulizeTrainable = VisaulizeTrainable

        # for showing layer-wise memory usage
        self.layermem = []

    def _isTranable(self, name):
        for o in self.outputTables:
            if isinstance(name, str) and o.name in name:
                return True
        return False

    def allocateMemory(self):
        # assign the same graph index for inplace operations
        # note: we need to handle stride == 2 for int8 depthwise to save memory
        if self.USE_INPLACE:
            for i, op in enumerate(self.layer):
                if op.params["op"] == "DEPTHWISE_CONV_2D" and op.params["input_dtype"] == "int8" and not self.tflite_op:
                    # set the idx of output and next layer input
                    previous_output_idx = op.output_tensors[0].graph_idx
                    op.output_tensors[0].graph_idx = op.input_tensors[0].graph_idx
                    if (
                        i + 1 < len(self.layer)
                        and len(self.layer[i + 1].input_tensors) > 0
                        and str(self.layer[i + 1].input_tensors[0].graph_idx) == str(previous_output_idx)
                    ):
                        self.layer[i + 1].input_tensors[0].graph_idx = op.input_tensors[0].graph_idx
                    # update following ops' tensors
                    for following_idx in range(i, len(self.layer)):
                        for cnt, inp_tensor in enumerate(self.layer[following_idx].input_tensors):
                            if str(inp_tensor.graph_idx) == str(previous_output_idx):
                                inp_tensor.graph_idx = op.input_tensors[0].graph_idx

        num_layers = len(self.layer)
        # go through all tensors in the model
        for i, op in enumerate(self.layer):
            # get all unallocated tensors for this layer
            unallocated_tensors = []
            for t in op.input_tensors:
                if t.allocator_idx is None:
                    unallocated_tensors.append(t)
            for cnt, t in enumerate(op.output_tensors):
                if cnt == 0 and not (
                    self.USE_INPLACE
                    and op.params["op"] == "DEPTHWISE_CONV_2D"
                    and op.params["input_dtype"] == "int8"
                    and not self.tflite_op
                ):
                    if t.allocator_idx is None:
                        unallocated_tensors.append(t)
                # assume seocnd outputs will not be inplace updated
                else:
                    if t.allocator_idx is None:
                        unallocated_tensors.append(t)

            # add each tensor
            for cnt, t in enumerate(unallocated_tensors):
                start_idx = i
                end_idx = i + 1 if i == 0 else num_layers
                for idx in range(start_idx + 1, num_layers):
                    for input_t in self.layer[idx].input_tensors:
                        if str(t.graph_idx) == str(input_t.graph_idx):
                            end_idx = idx + 1
                # check if this is output
                ttype = TTYPE_INFERNECE
                # add the tensor
                t.allocator_idx = self.allocator.addTensor(start_idx, end_idx, t.len(), name=t.graph_idx, type=ttype)
                # propagate the allocation to tensors with the same idx
                for j in range(i + 1, num_layers):
                    opp = self.layer[j]
                    for tt in opp.input_tensors:
                        if str(t.graph_idx) == str(tt.graph_idx):
                            tt.allocator_idx = t.allocator_idx
                    # not inplace update
                    for tt in opp.output_tensors:
                        if str(t.graph_idx) == str(tt.graph_idx):
                            tt.allocator_idx = t.allocator_idx

            # for detailed memory
            layermem = {}

            layermem["MAC"] = op.get_macs()
            layermem["activation"] = op.get_activation_size()
            layermem["scale"] = op.get_scale_size()
            layermem["runtime"] = op.get_sbuf_size()
            layermem["kernel"] = op.get_kbuf_size()
            self._enlargeBuffer("im2col", layermem["runtime"])
            self._enlargeBuffer("kernel", layermem["kernel"])

            if (
                "weight_name" in op.params
                and self._isTranable(op.params["weight_name"])
                and op.params["op"] != "TRANSPOSE_CONV_2D"
            ):
                size = int(op.get_weights_size())
                self.buffers["trainable"] += size
                layermem["trainable"] = size
                layermem["weight"] = 0
            else:
                layermem["weight"] = int(op.get_weights_size())
            if "bias_name" in op.params and self._isTranable(op.params["bias_name"]):
                size = int(op.get_bias_size())
                self.buffers["trainable"] += size
                if "trainable" in layermem:
                    layermem["trainable"] += size
                else:
                    layermem["trainable"] = size
                layermem["bias"] = 0
            else:
                layermem["bias"] = int(op.get_bias_size())
            # if it is float32 op, then their wegiths/bias should from SRAM buffers
            if op.params["input_dtype"] != "int8":
                layermem["scale"] = 0
                layermem["bias"] = 0
                layermem["weight"] = 0
            self.__increaseFlash(layermem["weight"])
            self.__increaseFlash(layermem["bias"])
            self.__increaseFlash(layermem["scale"])

            self.layermem.append(layermem)

        # find out int8 inplace depthwise conv and stride == 2
        for i, op in enumerate(self.layer):
            if (
                op.params["op"] == "DEPTHWISE_CONV_2D"
                and op.params["input_dtype"] == "int8"
                and op.params["stride_h"] == op.params["stride_w"] == 2
            ):
                if op.input_tensors[0].allocator_idx == op.output_tensors[0].allocator_idx:
                    self.allocator.rectangles[op.input_tensors[0].allocator_idx]["stride2_inplace_idx"] = i

        # Reorder the rectangles to decide which tensor needs to be scheduled first
        self.allocator.sortSize()
        self.allocator.allocate()
        self.allocator.visualize(self.mem_visual_path)
        self._enlargeBuffer("input_output", self.allocator.get_peak())

        # sanity check, see if all tensors have been allocated
        for i, op in enumerate(self.layer):
            # get all unallocated tensors for this layer
            for cnt, t in enumerate(op.input_tensors):
                assert t.allocator_idx is not None
            for cnt, t in enumerate(op.output_tensors):
                assert t.allocator_idx is not None

        # assign the address according to placement
        for i, op in enumerate(self.layer):
            # get all unallocated tensors for this layer
            for cnt, t in enumerate(op.input_tensors):
                if cnt == 0:
                    op.params["input_buf_add_offset"] = self.allocator.getIdxAddress(t.allocator_idx)
                    op.params["input_buf_add"] = "front"
                elif cnt == 1:
                    op.params["input2_buf_add_offset"] = self.allocator.getIdxAddress(t.allocator_idx)
                    op.params["input2_buf_add"] = "front"
                elif cnt == 2:
                    op.params["input3_buf_add_offset"] = self.allocator.getIdxAddress(t.allocator_idx)
                    op.params["input3_buf_add"] = "front"
                op.input_tensors[cnt].buffer_name = "buffer0"
                op.input_tensors[cnt].buffer_address = self.allocator.getIdxAddress(t.allocator_idx)
            for cnt, t in enumerate(op.output_tensors):
                if cnt == 0:
                    op.params["output_buf_add_offset"] = self.allocator.getIdxAddress(t.allocator_idx)
                    op.params["output_buf_add"] = "front"
                    op.output_tensors[cnt].buffer_name = "buffer0"
                    op.output_tensors[cnt].buffer_address = self.allocator.getIdxAddress(t.allocator_idx)
                if cnt == 1:
                    op.params["output2_buf_add_offset"] = self.allocator.getIdxAddress(t.allocator_idx)
                    op.params["output2_buf_add"] = "front"
                    op.output_tensors[cnt].buffer_name = "buffer0"
                    op.output_tensors[cnt].buffer_address = self.allocator.getIdxAddress(t.allocator_idx)

        # calculate peak mem
        self.peakmem = (
            self.allocator.get_peak() + self.buffers["im2col"] + self.buffers["kernel"]  # + self.buffers["trainable"]
        )

    def dumpLayerIndex(self):
        # header
        print("-" * 14 + " Tensor Allocation Details " + "-" * 14)
        print(" #op |   operator type   | input index | output index |")
        for cnt, l in enumerate(self.layer):
            operator_num = "#" + str(cnt)
            type = str(l.params["op"])
            input_tensor = ""
            for cnt_inp, inp in enumerate(l.input_tensors):
                input_tensor += str(inp.allocator_idx)
                if cnt_inp < len(l.input_tensors) - 1:
                    input_tensor += ","
            output_tensor = str(l.output_tensors[0].allocator_idx)
            string = (
                operator_num.ljust(5)
                + "|"
                + type.ljust(19)
                + "|"
                + input_tensor.ljust(13)
                + "|"
                + output_tensor.ljust(14)
                + "|"
            )
            print(string)

    def dumpLayerMem(self):
        # header
        print(
            "----------------------------------------------------  Schedule Details ----------------------------------------------------------------"  # noqa: E501
        )
        print(
            "----------------------|                      SRAM                      ||                     Flash                      |             |"  # noqa: E501
        )
        print(
            "----------------------|  activation  |  runtime  | trainable  |  sum   ||   weight   |   bias   |  scale   |     sum     |     MAC     |"  # noqa: E501
        )

        layermem = self.layermem
        self.__dumpMemInfo(layermem)

    def __dumpMemInfo(self, layermem):
        string = "-------Schedule-------|"
        maxActive = self.buffers["input_output"]
        maxRuntime = self.buffers["im2col"] + self.buffers["kernel"]
        maxTrainable = self.buffers["trainable"]
        totalWeight = self.__sumKey(layermem, "weight")
        totalBias = self.__sumKey(layermem, "bias")
        totalScale = self.__sumKey(layermem, "scale")
        totalMAC = self.__sumKey(layermem, "MAC")
        string += str(maxActive).ljust(14) + "|"
        string += str(maxRuntime).ljust(11) + "|"
        string += str(maxTrainable).ljust(12) + "|"
        string += str(maxActive + maxRuntime + maxTrainable).ljust(8) + "||"
        string += str(totalWeight).ljust(12) + "|"
        string += str(totalBias).ljust(10) + "|"
        string += str(totalScale).ljust(10) + "|"
        string += str(totalWeight + totalBias + totalScale).ljust(13) + "|"
        string += str(totalMAC).ljust(13) + "|"
        print(string)
        for i, _ in enumerate(layermem):
            layer_info = self.layer[i].get_layer_info()
            string = ""
            string += str(i) + ":" + layer_info["op"]
            string = string.ljust(22) + "|"
            SRAM = 0
            if "activation" in layermem[i]:
                substr = (
                    str(layermem[i]["activation"]) + " (" + "{:.0%}".format(layermem[i]["activation"] / maxActive) + ")"
                )
                string += substr.ljust(14) + "|"
                SRAM += layermem[i]["activation"]
            if "runtime" in layermem[i]:
                sbuf = layermem[i]["runtime"] + layermem[i]["kernel"]
                substr = str(sbuf) + " (" + "{:.0%}".format(sbuf / maxRuntime) + ")"
                string += substr.ljust(11) + "|"
                SRAM += sbuf
            else:
                string = string.ljust(49) + "|"
            if "trainable" in layermem[i]:
                substr = (
                    str(layermem[i]["trainable"])
                    + " ("
                    + "{:.0%}".format(layermem[i]["trainable"] / maxTrainable)
                    + ")"
                )
                string += substr.ljust(12) + "|"
                SRAM += layermem[i]["trainable"]
            else:
                string = string.ljust(62) + "|"

            # SRAM end
            string += str(SRAM)
            string = string.ljust(71) + "||"
            flash = 0
            if "weight" in layermem[i]:
                substr = (
                    str(layermem[i]["weight"])
                    + " ("
                    + "{:.0%}".format(layermem[i]["weight"] / (totalWeight + 0.0001))
                    + ")"
                )
                string += str(substr).ljust(12) + "|"
                flash += layermem[i]["weight"]
            if "bias" in layermem[i]:
                substr = (
                    str(layermem[i]["bias"]) + " (" + "{:.0%}".format(layermem[i]["bias"] / (totalBias + 0.0001)) + ")"
                )
                string += str(substr).ljust(10) + "|"
                flash += layermem[i]["bias"]
            if "scale" in layermem[i]:
                substr = (
                    str(layermem[i]["scale"]) + " (" + "{:.0%}".format(layermem[i]["scale"] / totalScale + 0.0001) + ")"
                )
                string += str(substr).ljust(10) + "|"
                flash += layermem[i]["scale"]

                if flash > 0:
                    string += (
                        str(flash)
                        + " ("
                        + "{:.0%}".format(flash / (totalWeight + totalBias + totalScale + 0.0001))
                        + ")"
                    )
                    string = string.ljust(121) + "|"
            # flash end
            if "MAC" in layermem[i]:
                substr = str(layermem[i]["MAC"]) + " (" + "{:.0%}".format(layermem[i]["MAC"] / totalMAC) + ")"
                string += str(substr).ljust(13) + "|"
            print(string)

    def __sumKey(self, layers, key):
        result = 0
        for _, layer in enumerate(layers):
            if key in layer:
                result += layer[key]

        return result

    def getBuffers(self):
        return self.buffers

    # Maximum binary size: This should be updated if any change in the inference side
    # TODO: Combine with code generation to get more accurate result
    def profileResult(self):
        return self.peakmem, self.flash + self.bias + self.scale + int(self.code * 1024)

    def __increaseFlash(self, size):
        self.flash += int(size)

    def _enlargeBuffer(self, buf_str, size):
        if buf_str == "input_output" or buf_str == "residual":
            self.buffers[buf_str] = max(self.buffers[buf_str], int(size))
        else:
            if buf_str not in self.buffers:
                self.buffers[buf_str] = size
            else:
                self.buffers[buf_str] = max(self.buffers[buf_str], size)
