# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   GeneralMemoryScheduler.py
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

from enum import Enum
from collections import defaultdict
from .allocator.base_allocator import AllocatorType
from .allocator.firstFit import FirstFit
from .right_inplace_gap import right_inplace_gap_dense_ext

from .operators.basic_utils import align_byte_to_n
from .constant import (
    FUSE_SGD_UPDATE_STR,
    FUSHION_CONFIG,
    INFERECE_WEIGHT_SIZE,
    TTYPE_INFERNECE,
    TTYPE_STATIC_WEIGHT,
    TTYPE_TRAINING_ACTIVATION,
    TTYPE_TRAINING_GRADIENT,
    TTYPE_TRAINING_WEIGHTS,
)

from .operators import add, avgpool2d, conv2d, depthwiseConv2d, maxpool2d, upsample, se_element_mult
from .operators.basic_utils import op_inplace_type, tensor_io, InplaceMode, RightShift

# NOTE: Currently only supports ops a single output
INPLACE_OPS_INPUT_GROUPING = {
    0: (),
    1: (
        avgpool2d.AvgPool2d,
        conv2d.Conv2d,
        depthwiseConv2d.DepthwiseConv2d,
        maxpool2d.maxPool2d,
        upsample.upSample
    ),
    2: (
        add.Add,
        se_element_mult.SEelementmult
    )
}

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
        allocator_type=AllocatorType.first_fit,
        inplace_mode=InplaceMode.depthwise_only,
        optimize_right_shift=RightShift.flexible_right_shift,
        optimize_inplace_flexible=True,
        right_shift_cost = 1e-4,
        align_to_n_bytes=4,
        model_name="mcunet_model"
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

        if allocator_type == AllocatorType.MILP:
            # imported here, so pyomo and a solver are only needed when the MILP is asked for
            from .allocator.milp_allocator import MILPAllocator

            self.allocator = MILPAllocator(
                memory_limit,
                model_name=model_name,
                optimize_right_shift=optimize_right_shift,
                optimize_inplace_flexible = optimize_inplace_flexible,
                RIGHT_SHIFT_COST=right_shift_cost,
                align_to_n_bytes=align_to_n_bytes
            )
        elif allocator_type == AllocatorType.first_fit:
            self.allocator = FirstFit(
                memory_limit,
                model_name=model_name,
                align_to_n_bytes=align_to_n_bytes
            )
        else:
            raise ValueError(
                f"Invalid allocator type, currently only supports {[t.name for t in AllocatorType]}"
            )
        
        self.inplace_mode = inplace_mode
        self.outputTables = outputTables
        self.USE_INPLACE = inplace
        self.mem_visual_path = mem_visual_path
        self.tflite_op = tflite_op
        self.dummy_address = dummy_address
        self.VisaulizeTrainable = VisaulizeTrainable
        self.align_to_n_bytes=align_to_n_bytes

        # for showing layer-wise memory usage
        self.layermem = []

    def _isTranable(self, name):
        for o in self.outputTables:
            if isinstance(name, str) and o.name in name:
                return True
        return False

    def allocateMemory(self):
        # NOTE: I am not sure what this section of code does, but it is never activate as
        # self.outputTables is always None in my test, so I will leave it as is for now.
        assert self.outputTables is None or (isinstance(self.outputTables, list) and len(self.outputTables) == 0), "outputTables is must be None or a list for this version of the code"
        num_layers = len(self.layer)
        # add all trainable tensors as one tensor
        length_model = len(self.layer)
        trainable = 0
        weight_size = 0
        bias_size = 0
        for out_t in self.outputTables:
            if "bias" in out_t.name:
                dtype_multiplier = 4
                trainable += int(out_t.len * dtype_multiplier)
                bias_size += int(out_t.len * dtype_multiplier)
            elif "weight" in out_t.name:
                dtype_multiplier = INFERECE_WEIGHT_SIZE
                # find the conv2d owning the tensor
                conv_2d_op = None
                for lay in self.layer:
                    if "weight_name" in lay.params and out_t.name in lay.params["weight_name"]:
                        conv_2d_op = lay
                        break
                assert conv_2d_op is not None
                # check if it is partial
                if "first_k_channel" in conv_2d_op.params and conv_2d_op.params["first_k_channel"] is not None:
                    trainable += int(
                        out_t.len
                        * dtype_multiplier
                        * conv_2d_op.params["first_k_channel"]
                        / conv_2d_op.params["input_c"]
                    )
                    weight_size += int(
                        out_t.len
                        * dtype_multiplier
                        * conv_2d_op.params["first_k_channel"]
                        / conv_2d_op.params["input_c"]
                    )
                else:
                    trainable += int(out_t.len * dtype_multiplier)
                    weight_size += int(out_t.len * dtype_multiplier)
            else:
                pass
        assert not self.VisaulizeTrainable, f"Training is currently not supported this disables that."
        if self.VisaulizeTrainable:
            self.allocator.addTensor(0, length_model, trainable, type=TTYPE_STATIC_WEIGHT)

        def is_layer_last_consumer_of_tensor(l_idx, t):
            """Returns True if not succeding layer of l consumes tensor t"""
            for idx in range(l_idx+1, len(self.layer)):
                for input_t in self.layer[idx].input_tensors:
                    if str(t.graph_idx) == str(input_t.graph_idx):
                        return False
            return True

        def tensor_inplace_parameters_in(t, op, gap=0):
            """
            set the 3 parameters:
                - its inplace type
                - its gap
                - its outputs which can `potentially` overwrite, an output can have multiple inputs
                  like add so only one is choosen to be overwritten
            needed by input tensor for inplace operations and memory scheduling

            A tensor another layer still reads cannot be overwritten, so it gets no
            gap either. Codegen emits this straight into the kernel call, and the
            kernel shifts on any nonzero gap once it is handed one pointer for both
            input and output, so a gap left on a tensor that was never going to be
            aliased is a memmove waiting for the addresses to line up.
            """
            if t.is_last_consumed and self.inplace_mode == InplaceMode.full:
                inplace = op.params["inplace"]
            else: inplace = op_inplace_type.force_not_inplace

            # only a flexible_inplace tensors can be modified, if an op has special requirements like
            # depthwise conv, which has to be inplace, it sets it before here
            if t.inplace is None or t.inplace == op_inplace_type.flexible_inplace:
                t.inplace = inplace

            # Determines if the allocator should factor in the right_shift of ignore it
            # it ignores it for most layers except conv currently
            t.op_needs_right_shift = op.params["op_needs_right_shift"]

            # aligning the memory so the memmove operation is not fragmented which speeds it up
            t.gap = 0 if t.inplace == op_inplace_type.force_not_inplace else \
                    align_byte_to_n(gap, self.align_to_n_bytes)
            assert len(op.output_tensors) == 1, (
                "the current implementation only support single output operations but received "
                f"{op.__class__.__name__} with {len(op.output_tensors)}"
            )
            t.inplace_tensor_out = None if t.inplace == op_inplace_type.force_not_inplace else op.output_tensors[0]

        # Classify each op's inputs inplace type conditioned on if it is the last consumers of the tensor,
        # hence it can be overwritten. The op declares what its kernel can do through params["inplace"],
        # but a tensor a later layer still reads cannot be given away regardless of what the kernel supports.

        # NOTE: This also acts as the switch to turn off the inplace operation for all tensors and exclude
        # those with a default force inplace operation like depthwise covn, this can also be used to all
        # tensor fully dual-buffer that will need all kernel to support that

        for i, op in enumerate(self.layer):
            if isinstance(op, INPLACE_OPS_INPUT_GROUPING[1]):
                t = op.input_tensors[0]
                gap = 0
                t.is_last_consumed = is_layer_last_consumer_of_tensor(i, t)
                if isinstance(op, conv2d.Conv2d):
                    # adding gap to the output tensor only for conv2d,
                    # to cater for the inplace convolution head room
                    params = op.params

                    # A patch holds its padding as rows/columns inside its own buffer, which the
                    # kernel skips via pad_t/pad_b/pad_l/pad_r instead of reading past the edge,
                    # so every tap lands in the buffer: padding 0 relative to it.
                    padding = 0 if params.get("is_patch", False) else params["padding"]

                    # dilation is not set in the current implementation
                    gap = right_inplace_gap_dense_ext(
                        cin=params["input_c"], cout=params["output_c"],
                        k_h=params["kernel_h"], k_w=params["kernel_w"],
                        hin=params["input_h"], win=params["input_w"],
                        s_h=params["stride_h"], s_w=params["stride_w"],
                        d_h=1, d_w=1,
                        p_h=padding, p_w=padding,
                        load_input_receptive_field=True
                    )
                    # if it is not the last consumed, it must be preserved, hence not in place
                elif isinstance(op, depthwiseConv2d.DepthwiseConv2d):
                    assert t.is_last_consumed, (
                        f"only the inplace variant of {op.__class__.__name__} has been implemented but "
                        f"its input tensor {t.graph_idx} is still needed by a future layer, so inplace "
                        "is not possible"
                    )
                    # For a naive/dual-buffer comparison schedule only
                    if self.inplace_mode == InplaceMode.depthwise_only:
                        t.inplace = op.params["inplace"]
                elif isinstance(op, upsample.upSample):
                    assert op.params["factor"].is_integer(), \
                        f"Upsample only supports integer factors, received {op.params["factor"]}"
                    output_h = op.params["output_h"]
                    output_w = op.params["output_w"]
                    output_c = op.params["output_c"]
                    input_h = op.params["input_h"]
                    input_w = op.params["input_w"]
                    input_c = op.params["input_c"]
                    gap = (output_h * output_w * output_c) - (input_h * input_w * input_c)

                tensor_inplace_parameters_in(t, op, gap)

            elif isinstance(op, INPLACE_OPS_INPUT_GROUPING[2]):
                t1, t2 = op.input_tensors
                gap1 = gap2 = 0
                t1.is_last_consumed = is_layer_last_consumer_of_tensor(i, t1)
                t2.is_last_consumed = is_layer_last_consumer_of_tensor(i, t2)
                tensor_inplace_parameters_in(t1, op, gap1)
                tensor_inplace_parameters_in(t2, op, gap2)

            else:
                raise AttributeError(f"Unaccounted Layerr {op} {op.params['op']}")

        all_t_size = 0
        graph_idx_register = defaultdict(list)
        graph_idx_allocated_idx = dict()
        # go through all tensors in the model
        for i, op in enumerate(self.layer):
            # get all unallocated tensors for this layer
            unallocated_tensors = []
            for t in op.input_tensors:
                if t.allocator_idx is None and t.graph_idx not in graph_idx_register:
                    unallocated_tensors.append((t, tensor_io.in_))
                graph_idx_register[t.graph_idx].append(t)
            for t in op.output_tensors:
                if t.allocator_idx is None and t.graph_idx not in graph_idx_register:
                    unallocated_tensors.append((t, tensor_io.out_))
                graph_idx_register[t.graph_idx].append(t)
            # add each tensor
            training_start_idx = _find_training_idx(layers=self.layer)
            assert training_start_idx == len(self.layer), \
                f"The current implementation does not support traning yet, this is a guard for that"
            for cnt, (t, tio) in enumerate(unallocated_tensors):
                t.align_to_n_bytes = self.align_to_n_bytes
                start_idx = i
                # a tensor no later layer reads dies with this layer, unless it is a model output; a patch
                # stage output is copied into the assembled buffer after each patch, so it is not one
                if tio == tensor_io.out_ and not op.params.get("is_patch", False):
                    end_idx = num_layers
                else:
                    end_idx = i + 1
                for idx in range(start_idx + 1, num_layers):
                    for input_t in self.layer[idx].input_tensors:
                        if str(t.graph_idx) == str(input_t.graph_idx):
                            end_idx = idx + 1
                assert isinstance(end_idx, int), f"end_idx is not set for tensor {t.graph_idx} in layer {i}"
                # check if this is output
                ttype = TTYPE_INFERNECE
                if self.outputTables is not None and not FUSHION_CONFIG[FUSE_SGD_UPDATE_STR]:
                    for o in self.outputTables:
                        if o.idx in t.graph_idx:
                            end_idx = len(self.layer)
                            all_t_size += o.len
                            ttype = TTYPE_TRAINING_GRADIENT

                if (
                    "is_start_of_normal_inference_block" in op.params
                    and op.params["is_start_of_normal_inference_block"]
                ):
                    # pin the input for the start of normal inference block to exist from the start
                    if t in op.input_tensors:
                        start_idx = 0
                # add the tensor
                graph_idx_allocated_idx[t.graph_idx] = self.allocator.addTensor(
                    start_idx, end_idx, t.len(), op=op, name=t.graph_idx,
                    type=ttype, inplace=t.inplace, tio=tio
                )

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

        # assign data dtype for each tensor for visualization
        # we need to figure out training_weight and training_activation here
        # for training_weight, it should contain weights of "transpose conv"
        # then, other tensors in training can be categorized as training activation
        training_start_idx = _find_training_idx(self.layer)
        # assign every tenosrs labeled as TTYPE_INFERNECE after the index as TTYPE_TRAINING_ACTIVATION
        for r in self.allocator.rectangles:
            if r["type"] == TTYPE_INFERNECE and r["end"] > training_start_idx:
                r["type"] = TTYPE_TRAINING_ACTIVATION
        # for each tranpose conv, find it
        for i, op in enumerate(self.layer):
            if op.params["op"] == "TRANSPOSE_CONV_2D":
                # if any tenosr used by this layer
                for r in self.allocator.rectangles:
                    if r["end"] <= training_start_idx:
                        continue
                    if r["name"] == op.params["weight_name"]:
                        r["type"] = TTYPE_TRAINING_WEIGHTS

        # propagate the allocation to tensors with the same graph_idx
        # the last consumed tensor (basically, the time this tensor is the input to a layer, its value has to be preserved till then)
        # with the same idx overwrites the others, and rejects:
        #       - an intermidate tensor which is forced inplace, but an later consumer input, which is not inplace as the changes the address
        #         that the later layer will read from
        #       - an intermidate tensor which is flexible inplace by a force not inplace, the same reason as the offer
        assert len(graph_idx_register) == len(graph_idx_allocated_idx), f"graph tree malformed"
        for (graph_idx, tensors), (graph_idx_, allocated_idx) in zip(graph_idx_register.items(), graph_idx_allocated_idx.items()):
            assert graph_idx == graph_idx_, f"graph tree is malformed"

            last_consumed_t = [t for t in tensors if t.is_last_consumed]
            assert len(last_consumed_t) <= 1, "Only one tensor should be the last, graph malformed"
            try: last_consumed_t = last_consumed_t[0]
            except IndexError: last_consumed_t = None
            for t in tensors:
                t.allocator_idx = allocated_idx
                if last_consumed_t is not None:
                    # the rejection assignment is an extra sanity check, to assure no graph order corruption
                    t.force_change_inplace(last_consumed_t.inplace, reject=(
                        # dst, src
                        (op_inplace_type.force_inplace, None),
                        (op_inplace_type.flexible_inplace, op_inplace_type.force_not_inplace)
                    ))
                    t.gap = last_consumed_t.gap
                    t.inplace_tensor_out = last_consumed_t.inplace_tensor_out
                    t.op_needs_right_shift = last_consumed_t.op_needs_right_shift

        # Now that every rectangle exists, hand the allocator each tensor that is
        # can be overwritten and the output that can overwrite it.
        # NOTE: For actual output tensor, which are tensor, that no layer takes them as input the
        #       inplace is set to None and not a op_inplace_type
        for i, op in enumerate(self.layer):
            for t in op.input_tensors:
                self.allocator.markPotentialInplace(t)

        # Allocating a memory location for each tensor/rectangle
        self.allocator.allocate()

        # Only a shifted input keeps its gap; the rest have the output placed below.
        for op in self.layer:
            for t in op.input_tensors:
                rec = self.allocator.rectangles[t.allocator_idx]
                t.gap = rec["gap"]
                t.right_shift_decision = rec["right_shift_decision"]

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
                # elif cnt == 2:
                #     op.params["input3_buf_add_offset"] = self.allocator.getIdxAddress(t.allocator_idx)
                #     op.params["input3_buf_add"] = "front"
                else:
                    raise RuntimeError(
                        f"Unexpected number of arguments {cnt} for {op}. "
                        "We currently only support ops with at most 2 arguments."
                    )
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


def _find_training_idx(layers):
    idx = len(layers)
    for cnt, l in enumerate(layers):
        if l.params["op"] in ["CAST"]:
            return cnt
    return idx
