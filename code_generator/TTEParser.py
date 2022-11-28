import json
import math
import pickle
import warnings

import numpy as np

from .constant import (
    FUSE_INT8CAST_STR,
    FUSE_SGD_UPDATE_STR,
    FUSE_TILE_STR,
    FUSE_WHERE_ZEROSSTR,
    FUSHION_CONFIG,
    INPLACE_MUL_STR,
    INPLACE_WHERE_STR,
    REORDER_STR,
    USE_BIT_MASK,
    op_name_translation,
)
from .FusionUtil import (
    _accessTrainable,
    _castisFusable,
    _castisFusable_for_gconv,
    _fileTileRepAsWeights,
    _findBinMaskPattern,
    _findBinMaskPatternint8,
    _findConv2dwithScaleName,
    _findKeyinTensors,
    _findMultiplyAbsMaxDivide,
    _findPartialConv,
    _findTargetWeightforGconv,
    _findTransposeMultiplyAbsMaxDivide,
    _findWhereTensorFrom,
    _removeLayers,
    _updateIdx,
    _updateIdxParameter,
    _updateOutputDtype,
)
from .GraphReorder import reorderGroupConv_TransponseConv, reorderGroupConv_TransponseConv_int8
from .operators import (
    add,
    add1d,
    avgpool2d,
    bias_add,
    bias_update,
    cast,
    collapse_sum_like,
    conv2d,
    dense,
    depthwiseConv2d,
    div,
    exp,
    greater,
    group_conv2d,
    less,
    log_softmax,
    mat_mul,
    mul,
    negative,
    nll_loss,
    ones_like,
    permute_4D_3012,
    permute_groupconv_out,
    relu,
    reshape,
    reshape_like,
    strided_slice,
    sub,
    sum,
    tile,
    transpose,
    transpose_conv2d,
    where,
    zeros_like,
)
from .operators.basic_utils import isconstanttstr
from .QAS_util import get_effective_scalename_with_input_key, get_QAS

MAX_DAGOP_OUTPUTS = 5

fused_op = {"clip", "nn.batch_flatten", "squeeze", "reshape", "reshape_like"}


class outputInfo:
    def __init__(self, name, idx, len, dtype):
        self.name = name
        self.idx = idx
        self.len = len
        self.dtype = dtype


class TTEParser(object):
    def __init__(self, model, data, scale_params=None):
        self.layer = []
        self.gout = []
        self.det_outputs = None
        with open(model, "r") as f:
            self.model = json.load(f)
        with open(data, "rb") as f:
            w_params = pickle.load(f)
            self.data = {}
            for k in w_params:
                if k[0] != "v":
                    self.data[f"v{k}"] = w_params[k]
                else:
                    self.data[k] = w_params[k]
        self.scale_params = scale_params

        self.layer = []
        self.trainedWeights = []  # key, weight_ip
        self.trainedBias = []  # key, weight_ip
        self.fusedInputTable = {}
        self.outputTables = []
        self.regularFunctionTable = {
            "cast": self._convert_cast,
            "cast_like": self._convert_cast,
            "exp": self._convert_exp,
            "transpose": self._convert_transpose,
            "where": self._convert_where,
            "nn.conv2d_transpose": self._convert_transpose_conv2d,
            "strided_slice": self._convert_strided_slice,
            "nn.bias_add": self._convert_bias_add,
            "nn.relu": self._convert_relu,
            "zeros_like": self._convert_zeros_like,
            "zeros": self._convert_zeros,
            "ones_like": self._convert_ones_like,
            "ones": self._convert_ones,
            "collapse_sum_like": self._convert_collapse_sum_like,
            "less": self._convert_less,
            "less_equal": self._convert_less,
            "nn.log_softmax": self._convert_log_softmax,
            "nn.cross_entropy_with_logits": self._convert_cross_entropy_with_logits,
            "divide": self._convert_div,
            "tile": self._convert_tile,
            "negative": self._convert_negative,
            "greater": self._convert_greater,
            "greater_equal": self._convert_greater,
            "multiply": self._convert_mul,
            "nn.matmul": self._convert_matmul,
            "nn.dense": self._convert_dense,
            "mcumean": self._convert_average_pool,
        }
        self.partialChannelList = {}  # "idx": first_k_channel

    def loadModel(self):
        last_op = None
        has_zero_x = False
        zero_x = None
        self.fusedInputTable[self.model[0]["inputs"][0]["name"]] = self.model[0]["inputs"][0]["name"]
        # reorder the group conv and transpose conv to calculate weight gradients first
        if FUSHION_CONFIG[REORDER_STR]:
            self.model = reorderGroupConv_TransponseConv(self.model)
            self.model = reorderGroupConv_TransponseConv_int8(self.model)
        for cnt, op in enumerate(self.model):
            op_type = op["type"]
            if op_type in {"nn.conv2d", "nn.mcuconv2d"}:
                last_op = self._convert_convolution(op)
                # Float bp fusion
                # check if we need to have binary mask for this conv2d
                # conv2d (int32) -> cast -> greater/less -> multiply -> where (which take the map)
                # fusion     | --------------------------------------|
                if op["outputs"][0]["dtype"] == "int32":
                    pattern_found, op_dict = _findBinMaskPattern(self.model, op["outputs"][0]["name"])
                    if pattern_found:
                        # add second output in the output tensors
                        b_mask_info = op_dict["multiply"]["outputs"][0]
                        if USE_BIT_MASK:
                            last_op._add_output(
                                b_mask_info["name"],
                                "bool",
                                int(math.ceil(last_op.params["output_c"] / 8)),
                                last_op.params["output_w"],
                                last_op.params["output_h"],
                            )
                        else:
                            last_op._add_output(
                                b_mask_info["name"],
                                b_mask_info["dtype"],
                                last_op.params["output_c"],
                                last_op.params["output_w"],
                                last_op.params["output_h"],
                            )
                        # update params in conv2d
                        last_op.params["need_Bmask"] = True
                        last_op.params["output2_h"] = last_op.params["output_h"]
                        last_op.params["output2_w"] = last_op.params["output_w"]
                        last_op.params["output2_c"] = last_op.params["output_c"]
                        last_op.params["output2_dtype"] = b_mask_info["dtype"]
                        last_op.params["output2_idx"] = b_mask_info["name"]
                        # remove fused ops in the graph
                        _removeLayers(self.model, op_dict)
                # int8 bp fusion
                # check if we need to have binary mask for this conv2d
                # conv2d (int32) -> greater/less -> multiply -> where (which take the map)
                # fusion     | ------------------------------|
                if op["outputs"][0]["dtype"] == "int32":
                    pattern_found, op_dict = _findBinMaskPatternint8(self.model, op["outputs"][0]["name"])
                    if pattern_found:
                        # add second output in the output tensors
                        b_mask_info = op_dict["multiply"]["outputs"][0]
                        if USE_BIT_MASK:
                            last_op._add_output(
                                b_mask_info["name"],
                                "bool",
                                int(math.ceil(last_op.params["output_c"] / 8)),
                                last_op.params["output_w"],
                                last_op.params["output_h"],
                            )
                        else:
                            last_op._add_output(
                                b_mask_info["name"],
                                b_mask_info["dtype"],
                                last_op.params["output_c"],
                                last_op.params["output_w"],
                                last_op.params["output_h"],
                            )
                        # update params in conv2d
                        last_op.params["need_Bmask"] = True
                        last_op.params["output2_h"] = last_op.params["output_h"]
                        last_op.params["output2_w"] = last_op.params["output_w"]
                        last_op.params["output2_c"] = last_op.params["output_c"]
                        last_op.params["output2_dtype"] = b_mask_info["dtype"]
                        last_op.params["output2_idx"] = b_mask_info["name"]
                        # remove fused ops in the graph
                        _removeLayers(self.model, op_dict)
                # we use hwc for computation, but in bp the 'c' may mean output channel for the training weights.
                # in this case, we need to insert an op to permute the weight tensor before running this conv2d op
                # TODO: make sure this is not longer needed after we optimize tile + group_conv2d
                # if len(self.model) > 0 and "weight" not in op["inputs"][1]["name"]:
                #     permute_params = {
                #         "input_idx": op["inputs"][1]["name"],
                #         "input_dim": 3,
                #         "input_h": op["inputs"][1]["shape"][-2],
                #         "input_w": op["inputs"][1]["shape"][-1],
                #         "input_c": op["inputs"][1]["shape"][-4],  # IOHW
                #     }
                #     permute_op = permute_3D_120.permute_3D_120(permute_params)
                #     self.layer.append(permute_op)
                if has_zero_x:
                    last_op.set_input_zero_point(zero_x)
                    has_zero_x = False
                    zero_x = None
                self.layer.append(last_op)
            elif op_type == "nn.mcuadd":
                # fp32
                pattern_found, op_dict = _findBinMaskPattern(self.model, op["outputs"][0]["name"])
                # try int8
                if not pattern_found:
                    pattern_found, op_dict = _findBinMaskPatternint8(self.model, op["outputs"][0]["name"])
                last_op = self._convert_qadd(op)
                if pattern_found:
                    # add second output in the output tensors
                    b_mask_info = op_dict["multiply"]["outputs"][0]
                    last_op._add_output(
                        b_mask_info["name"],
                        b_mask_info["dtype"],
                        last_op.params["output_c"],
                        last_op.params["output_w"],
                        last_op.params["output_h"],
                    )
                    # update params in conv2d
                    last_op.params["need_Bmask"] = True
                    last_op.params["output2_h"] = last_op.params["output_h"]
                    last_op.params["output2_w"] = last_op.params["output_w"]
                    last_op.params["output2_c"] = last_op.params["output_c"]
                    last_op.params["output2_dtype"] = b_mask_info["dtype"]
                    last_op.params["output2_idx"] = b_mask_info["name"]
                    # remove fused ops in the graph
                    _removeLayers(self.model, op_dict)
                self.layer.append(last_op)
            elif (
                op_type == "cast" and op["inputs"][0]["dtype"] == "int8" and op["outputs"][0]["dtype"] == "int32"
            ):  # int8 gradient for bias
                # skip this one
                _updateIdx(self.model, self.layer, op["inputs"][0]["name"], op["outputs"][0]["name"])
            elif op_type == "cast" and _castisFusable(self.model, op)[0] and FUSHION_CONFIG[FUSE_INT8CAST_STR]:
                _, transpose_conv_json = _castisFusable(self.model, op)
                transpose_conv_json["inputs"][1] = op["inputs"][0]  # pass the int8 input to transpose conv2d
            elif (
                op_type == "cast" and _castisFusable_for_gconv(self.model, op)[0] and FUSHION_CONFIG[FUSE_INT8CAST_STR]
            ):
                _, group_conv_json = _castisFusable_for_gconv(self.model, op)
                group_conv_json["inputs"][0] = op["inputs"][0]  # pass the int8 input to group conv2d
                group_conv_json["inplace_int8_input"] = True
            elif op_type == "tile" and FUSHION_CONFIG[FUSE_TILE_STR]:
                # check if we need to fuse ops for tile
                # ##########   tile  -> reshape -> conv2d (which takes it as weights)
                # fusion     | ------------------------|
                pattern_found, op_dict = _fileTileRepAsWeights(self.model, op)
                if pattern_found:
                    # remove reshape
                    _removeLayers(self.model, {"reshape": op_dict["reshape"]})
                    # redirect the input of tile to conv2d's weight
                    op_dict["conv2d"]["inputs"][1] = op_dict["tile"]["inputs"][0]
                else:
                    raise NotImplementedError
            elif op_type == "add":
                if len(op["inputs"][0]["shape"]) == 4 and op["inputs"][0]["dtype"] == "int8":
                    if "zero_y" in op["inputs"][1]["name"]:
                        zero_y = int(self.data[op["inputs"][1]["name"]])
                        last_op.set_output_zero_point(zero_y)
                        continue
                    last_op = self._convert_add(op)
                    self.layer.append(last_op)
                else:
                    last_op = self._convert_add1d(op)
                    self.layer.append(last_op)
            elif op_type == "nn.bias_add" and op["inputs"][1]["dtype"] == "int8":
                last_op.params["bias"] = self.data[op["inputs"][1]["name"]].astype(int)
                # redirect the index
                last_op.change_output_tensor_idx(op["outputs"][0]["name"])
            # fixing HWC -> CHW alginment
            elif (
                op_type == "reshape"
                and len(op["inputs"][0]["shape"]) == 4
                and (op["inputs"][0]["shape"][2] != 1 and op["inputs"][0]["shape"][3] != 1)
                and op["inputs"][0]["shape"][2] != op["outputs"][0]["shape"][2]
                and op["inputs"][0]["shape"][3] != op["outputs"][0]["shape"][3]
            ):
                last_op = self._convert_reshape(op)
                self.layer.append(last_op)
            # input might be parameters, we handle the inside ops since we only support for scales in `multiply`
            elif op_type == "reshape" and op["inputs"][0]["var_type"] == "parameter":
                # find out ops taking the output
                for other_op in self.model:
                    for input_tensor in other_op["inputs"]:
                        if input_tensor["name"] == op["outputs"][0]["name"]:
                            if other_op["type"] in {"multiply", "divide"}:
                                _updateIdxParameter(self.model, op["inputs"][0]["name"], op["outputs"][0]["name"])
                            else:
                                raise NotImplementedError
            # fixing CHW -> HWC alginment
            elif (
                op_type == "reshape_like"
                and len(op["inputs"][1]["shape"]) == 4
                and (op["inputs"][1]["shape"][2] != 1 and op["inputs"][1]["shape"][3] != 1)
                and op["inputs"][0]["shape"][2] != op["outputs"][0]["shape"][2]
                and op["inputs"][0]["shape"][3] != op["outputs"][0]["shape"][3]
            ):
                last_op = self._convert_reshape_like(op)
                self.layer.append(last_op)
            # bypass this layer by fusing it into the last layer, TODO: revisit this for clipping fp results
            elif op_type in fused_op and op:
                # update tensors
                _updateIdx(self.model, self.layer, op["inputs"][0]["name"], op["outputs"][0]["name"])
            elif op_type in "nn.mcutruncate":
                # update output dtype
                _updateOutputDtype(self.layer, op["inputs"][0]["name"], "int8")
                # update tensor idx
                _updateIdx(self.model, self.layer, op["inputs"][0]["name"], op["outputs"][0]["name"])
            elif op_type == "subtract":
                is_fuse = False
                for tensor in op["inputs"]:
                    if "zero_x" in tensor["name"]:
                        has_zero_x = True
                        zero_x = int(self.data[tensor["name"]])
                        is_fuse = True
                if not is_fuse:
                    last_op = self._convert_sub(op)
                    self.layer.append(last_op)
            elif op_type == "sum":
                input_length = np.prod(op["inputs"][0]["shape"])
                output_length = np.prod(op["outputs"][0]["shape"])
                if input_length != output_length:
                    last_op = self._convert_sum(op)
                    self.layer.append(last_op)
                    if op["outputs"][0] and "output_info" in op["outputs"][0]["meta"]:
                        if op["outputs"][0]["meta"]["output_info"][0] == "v":
                            key = op["outputs"][0]["meta"]["output_info"]
                        else:
                            key = "v" + op["outputs"][0]["meta"]["output_info"]
                        if self.scale_params is not None:
                            e_s_name = get_effective_scalename_with_input_key(key, self.model)
                            QAS = get_QAS(key, self.scale_params, self.data[e_s_name])
                        else:
                            QAS = np.zeros(int(output_length)) + 0.000000001  # avoid zero division
                        bias_update_params = {
                            "input_idx": last_op.params["output_idx"],
                            "output_idx": key,
                            # tensor related
                            "input_size": int(output_length),
                            "input_buf_add": None,
                            "input_buf_add_offset": None,
                            "QAS": QAS,
                            "input_dtype": last_op.params["input_dtype"],
                            "output_dtype": "float32",
                        }
                        bias_update_op = bias_update.bias_update(bias_update_params)
                        self.layer.append(bias_update_op)
                else:  # skip this, no need to do anything on the data
                    input_idx = op["inputs"][0]["name"]
                    output_idx = op["outputs"][0]["name"]
                    # update the bias
                    if op["outputs"][0] and "output_info" in op["outputs"][0]["meta"]:
                        if op["outputs"][0]["meta"]["output_info"][0] == "v":
                            key = op["outputs"][0]["meta"]["output_info"]
                        else:
                            key = "v" + op["outputs"][0]["meta"]["output_info"]
                        if self.scale_params is not None:
                            e_s_name = get_effective_scalename_with_input_key(key, self.model)
                            QAS = get_QAS(key, self.scale_params, self.data[e_s_name])
                        else:
                            QAS = np.zeros(int(output_length)) + 0.000000001  # avoid zero division
                        bias_update_params = {
                            "input_idx": last_op.params["output_idx"],
                            "output_idx": key,
                            # tensor related
                            "input_size": int(output_length),
                            "input_buf_add": None,
                            "input_buf_add_offset": None,
                            "QAS": QAS,
                            "input_dtype": "float32",
                            "output_dtype": "float32",
                        }
                        bias_update_op = bias_update.bias_update(bias_update_params)
                        self.layer.append(bias_update_op)
                    # # update tensors
                    _updateIdx(self.model, self.layer, input_idx, output_idx)

            # assume weights are updated once we obtain its gradient
            elif op_type == "transpose" and FUSHION_CONFIG[FUSE_SGD_UPDATE_STR]:
                fuseable, op_dict = _findTransposeMultiplyAbsMaxDivide(self.model, op)
                # old IR
                if op["outputs"][0]["meta"]["children"] == 0:
                    # update tensors
                    _updateIdx(self.model, self.layer, op["inputs"][0]["name"], op["outputs"][0]["name"])
                elif fuseable:
                    # fuse "transpose" -> [max -> divide -> divide (int8 bp)]
                    _updateIdx(self.model, self.layer, op["inputs"][0]["name"], op_dict["cast"]["outputs"][0]["name"])
                    # add the output to output table
                    name = op_dict["cast"]["outputs"][0]["meta"]["output_info"]
                    idx = op_dict["cast"]["outputs"][0]["name"]
                    length = np.prod(op_dict["cast"]["outputs"][0]["shape"])
                    dtype = op_dict["cast"]["outputs"][0]["dtype"]
                    self.outputTables.append(outputInfo(name, idx, int(length), dtype))
                    _removeLayers(self.model, op_dict)
                else:
                    raise NotImplementedError
            elif (
                FUSHION_CONFIG[FUSE_WHERE_ZEROSSTR]
                and op_type == "where"
                and (op["inputs"][2]["dtype"] in ["int8", "int32", "float32"])
                and _findWhereTensorFrom(self.layer, op["inputs"][2]["name"]) is not None
                and _findWhereTensorFrom(self.layer, op["inputs"][2]["name"]).params["op"]
                == "ZEROS"  # third input is from zeros
            ):
                zeros_op = _findWhereTensorFrom(self.layer, op["inputs"][2]["name"])
                # remove previous the zeros layer
                self.layer.remove(zeros_op)
                # parse the where but remove the third input and set "input3_is_zeros" in params
                last_op = self._convert_where(op)
                last_op.params["input3_is_zeros"] = True
                last_op.input_tensors.remove(last_op.input_tensors[2])
                # check where we can update input2 inplace
                # if input2 is not used by following ops
                # (1) make input2_inplace
                # (2) update the following op's input idx (normally it is MUL)
                can_be_inplace = None
                # check if the last_op["input2_idx"] == some_op["ouptuts"][0]
                for from_op in self.model:
                    if from_op["outputs"][0]["name"] == last_op.params["input2_idx"]:
                        if from_op["outputs"][0]["meta"]["children"] != 1:
                            can_be_inplace = False
                        else:
                            can_be_inplace = True
                assert can_be_inplace is not None
                if can_be_inplace and FUSHION_CONFIG[INPLACE_WHERE_STR]:
                    # find the where the output of where goes and link it to the second input of where
                    for following_op in self.model:
                        for inp in following_op["inputs"]:
                            if inp["name"] == op["outputs"][0]["name"]:
                                inp["name"] = op["inputs"][1]["name"]
                    # remove output tensor of where
                    last_op.output_tensors.remove(last_op.output_tensors[0])
                    # set where to inplace
                    last_op.params["inplace"] = True
                # add the op
                self.layer.append(last_op)
            elif op_type == "multiply" and FUSHION_CONFIG[INPLACE_MUL_STR]:
                last_op = self._convert_mul(op)
                last_op_input = last_op.params["input_idx"]
                last_op_output = last_op.params["output_idx"]
                if last_op.params["input2_size"] > 1 and last_op.params["input_size"] > last_op.params["input2_size"]:
                    # good to be updated inplace
                    last_op.params["inplace"] = True
                    last_op.output_tensors.remove(last_op.output_tensors[0])
                    # redirect the following op's input as the inplace input
                    for following_op in self.model:
                        # if following_op["type"] in {"sum", "nn.conv2d_transpose", "nn.conv2d"}:
                        for inp in following_op["inputs"]:
                            if inp["name"] == last_op_output:
                                inp["name"] = last_op_input
                    # _updateIdx(self.model, self.layer, last_op_input, last_op_output)
                    # replace the following
                self.layer.append(last_op)
            elif op_type in self.regularFunctionTable:
                last_op = self.regularFunctionTable[op_type](op)
                self.layer.append(last_op)
            elif op_type == "abs":
                if FUSHION_CONFIG[FUSE_SGD_UPDATE_STR]:
                    cliping_pattern, op_dict = _findMultiplyAbsMaxDivide(self.model, abs_op=op)
                    if cliping_pattern:
                        # For transpose conv2d, this could be float32 -> int8 if it connects to abs
                        previous_op = _findWhereTensorFrom(self.layer, op["inputs"][0]["name"])
                        if (
                            previous_op.params["op"] == "TRANSPOSE_CONV_2D"
                            and previous_op.params["output_dtype"] == "float32"
                        ):
                            previous_op.params["float_to_int8"] = True
                            previous_op.params["output_dtype"] = "int8"
                            previous_op.output_tensors[0].dtype = "int8"
                            previous_op.add_int32_buffer_tensor()

                        _updateIdx(
                            self.model, self.layer, op["inputs"][0]["name"], op_dict["cast"]["outputs"][0]["name"]
                        )
                        _removeLayers(self.model, op_dict)
                    else:
                        raise NotImplementedError
                else:
                    cliping_pattern, op_dict = _findMultiplyAbsMaxDivide(self.model, abs_op=op)
                    if cliping_pattern:
                        _updateIdx(
                            self.model, self.layer, op["inputs"][0]["name"], op_dict["cast"]["outputs"][0]["name"]
                        )
                        _removeLayers(self.model, op_dict)
                        # Baseline for int8 fp without graph optimization
                        # Adding outputTable for accurate trainable measuremnet
                        if "output_info" in op_dict["cast"]["outputs"][0]["meta"]:
                            name = op_dict["cast"]["outputs"][0]["meta"]["output_info"]
                            idx = op_dict["cast"]["outputs"][0]["name"]
                            length = np.prod(op_dict["cast"]["outputs"][0]["shape"])
                            dtype = "int8"
                            self.outputTables.append(outputInfo(name, idx, int(length), dtype))
                    else:
                        raise NotImplementedError

            else:
                warnings.warn("%s op is not `supported" % op_type)
                raise NotImplementedError

            # GROUP CONV
            if self.layer[-1].params["op"] == "GROUP_CONV":
                # for group conv the output is actually h, w, IxO, we need to permute it to OHWI
                if not FUSHION_CONFIG[FUSE_SGD_UPDATE_STR]:
                    params = {
                        # op related
                        "op": "PERMUTE_GROUPCONV_OUT",
                        "input_idx": last_op.params["output_idx"],
                        # tensor related
                        "input_dim": 3,
                        "input_h": last_op.params["output_h"],
                        "input_w": last_op.params["output_w"],
                        "input_c": last_op.params["output_c"],
                        "groups": last_op.params["groups"],
                        "input_dtype": "float32",
                        "output_dtype": "float32",
                    }
                    self.layer.append(permute_groupconv_out.permute_groupconv_out(params))
                # we inplace update the weights, for output stantionary group conv
                # here we need to
                #   (1) update the graph: remove gconv -> reshape -> sum -> transpose (done in "transpose" op)
                #       -> [max -> divide -> divide (int8 bp)]
                #   (2) remove the output tensor in gconv
                #   (3) replace the output address with int8 weight in SRAM
                # TODO: we also need to back trace the int8 conv and make it use wegiht in both SRAM and Flash
                elif len(self.layer[-1].output_tensors) > 0:
                    # find the target weigth
                    weight_idx = _findTargetWeightforGconv(self.model, self.layer[-1].output_tensors[0].graph_idx)
                    assert weight_idx is not None
                    self.layer[-1].params["inplace_weight_name"] = weight_idx
                    # back trace to the int8 conv
                    conv_partial_layer = _findPartialConv(self.layer, weight_idx)
                    conv_p = conv_partial_layer.params
                    gconv_output_len = np.prod(self.layer[-1].output_tensors[0].size)
                    conv_weight_size = conv_p["input_c"] * conv_p["output_c"] * conv_p["kernel_h"] * conv_p["kernel_w"]
                    if conv_weight_size != gconv_output_len:
                        # this is partial
                        # find the first k channel
                        fisrt_k_channel = int(conv_p["input_c"] * gconv_output_len / conv_weight_size)
                        conv_partial_layer.params["first_k_channel"] = fisrt_k_channel
                        self.partialChannelList[weight_idx] = fisrt_k_channel
                    #
                    if self.scale_params is not None:
                        key = weight_idx
                        e_s_name = get_effective_scalename_with_input_key(key, self.model)
                        QAS = get_QAS(key, self.scale_params, self.data[e_s_name])
                    else:
                        QAS = np.zeros(int(output_length)) + 0.000000001  # avoid zero division
                    self.layer[-1].params["QAS"] = QAS
                    # remove for inplace
                    self.layer[-1].output_tensors.remove(self.layer[-1].output_tensors[0])

            # add the gradient_output to table, we will use a custom layer to perform SGD
            if (
                "meta" in op["outputs"][0]
                and op["outputs"][0]["meta"]["children"] == 0
                and "output_info" in op["outputs"][0]["meta"]
            ):
                name = op["outputs"][0]["meta"]["output_info"]
                idx = op["outputs"][0]["name"]
                length = np.prod(op["outputs"][0]["shape"])
                dtype = op["outputs"][0]["dtype"]
                self.outputTables.append(outputInfo(name, idx, int(length), dtype))

        # loop over the graph and find transpose conv that use partial weights
        for layer in self.layer:
            if (
                layer.params["op"] == op_name_translation["nn.conv2d_transpose"]
                and layer.params["weight_name"] in self.partialChannelList
            ):
                layer.params["first_k_channel"] = self.partialChannelList[layer.params["weight_name"]]

    def _convert_cast(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        output_c = input_c = input_h = input_w = 1
        if len(input_shape) == 4:
            output_c, input_c, input_h, input_w = input_shape  # OIHW
        elif len(input_shape) == 2:
            input_h, input_w = input_shape
            input_c = 1
        elif len(input_shape) == 1:
            input_h = input_w = 1
            input_c = input_shape[0]
        else:
            raise NotImplementedError

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_size": output_c * input_c * input_h * input_w,
            "input_dim": 4,
            "output_dim": 4,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
            "input_meta": op["inputs"][0]["meta"],
        }

        op = cast.cast(params)

        return op

    def _convert_relu(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        if len(input_shape) == 4:
            _, input_c, input_h, input_w = input_shape
        elif len(input_shape) == 2:
            input_h, input_w = input_shape
            input_c = 1
        else:
            raise NotImplementedError

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_c = input_c
        output_h = input_h
        output_w = input_w

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }

        op = relu.relu(params)

        return op

    def _convert_bias_add(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        if len(input_shape) == 2:
            input_h = 1
            input_w = input_shape[0]
            input_c = input_shape[1]
        else:
            input_c, input_h, input_w = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        bias_name = op["inputs"][1]["name"]
        if bias_name not in self.data:
            bias_value = bias_name
        else:
            bias_value = self.data[bias_name]
        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": input_h,
            "output_w": input_w,
            "output_c": input_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
            "bias": bias_value,
            "bias_name": bias_name,
        }

        op = bias_add.biasAdd(params)

        return op

    def _convert_reshape(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }

        op = reshape.reshape(params)

        return op

    def _convert_reshape_like(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        input2_info = op["inputs"][1]
        input2_dtype = get_dtype(op["inputs"][1])
        input2_shape = input2_info["shape"]
        input2_c, input2_h, input2_w = get_chw_shape(input2_shape)

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input2_h": input2_h,
            "input2_w": input2_w,
            "input2_c": input2_c,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
        }

        op = reshape_like.reshape_like(params)

        return op

    def _convert_exp(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_size": input_h * input_w * input_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }

        op = exp.exp(params)

        return op

    def _convert_transpose(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "input_vartype": input_info["var_type"],
            "output_dtype": output_dtype,
        }

        if "axes" in op["attrs"] and op["attrs"]["axes"] is not None:
            if op["attrs"]["axes"] == [1, 0, 2, 3]:
                # torch: OIHW -> IOHW -> permute 1023
                # tinyengine: OHWI -> IOHW -> permute 3012
                params["d1"], params["d2"], params["d3"], params["d4"] = input_shape
                params["op"] = "permute_4D_3012"
                op = permute_4D_3012.permute_4D_3012(params)
            else:
                raise NotImplementedError
        else:
            op = transpose.transpose(params)
        return op

    def _convert_strided_slice(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]

        d1, d2, d3, d4 = input_shape  # OHWI

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        o_d1, o_d2, o_d3, o_d4 = output_shape  # OHWI

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        b_list = op["attrs"]["begin"]
        e_list = op["attrs"]["end"]
        begin = b_list  # [b_list[0], b_list[2], b_list[3], b_list[1]]
        end = e_list  # [e_list[0], e_list[2], e_list[3], e_list[1]]
        strides = op["attrs"]["strides"]

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "d1": d1,
            "d2": d2,
            "d3": d3,
            "d4": d4,
            "begin": begin,
            "end": end,
            "strides": strides,
            "input_dim": 4,
            "output_dim": 4,
            "o_d1": o_d1,
            "o_d2": o_d2,
            "o_d3": o_d3,
            "o_d4": o_d4,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }

        op = strided_slice.stridedSlice(params)

        return op

    def _convert_average_pool(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": "AVERAGE_POOL_2D",
            # pool parameters
            "filter_h": input_h,
            "filter_w": input_w,
            "stride_h": 1,
            "stride_w": 1,
            "pad_h": 0,
            "pad_w": 0,
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }

        op = avgpool2d.AvgPool2d(params)

        return op

    def _convert_zeros(self, op):
        # shape
        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "output_idx": output_info["name"],
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "output_dtype": output_dtype,
        }

        op = zeros_like.zeros_like(params)

        return op

    def _convert_ones(self, op):
        # shape
        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "output_idx": output_info["name"],
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "output_dtype": output_dtype,
        }

        op = ones_like.ones_like(params)

        return op

    def _convert_zeros_like(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_c = input_c
        output_h = input_h
        output_w = input_w

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }

        op = zeros_like.zeros_like(params)

        return op

    def _convert_ones_like(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }

        op = ones_like.ones_like(params)

        return op

    def _convert_collapse_sum_like(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }

        op = collapse_sum_like.collapseSumLike(params)

        return op

    def _convert_log_softmax(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_h, input_w, input_c = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_h, output_w, output_c = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }

        op = log_softmax.logSoftMax(params)

        return op

    def _convert_cross_entropy_with_logits(self, op):
        # shape
        input_info = op["inputs"][0]
        input_dtype = get_dtype(op["inputs"][0])
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        input2_info = op["inputs"][1]

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input2_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
        }

        op = nll_loss.nllLoss(params)

        return op

    def _convert_qadd(self, op):
        def _getSigShift(s):
            sig, shi = math.frexp(s)
            sig = int(round(sig * 2**31))
            if sig == 2**31:
                sig /= 2
                shi += 1
            if shi < -31:
                shi = 0
                sig = 0

            return sig, shi

        def _getADDMultiplierShift(input_scale, input2_scale, output_scale):
            left_shift = 20

            twice_max_input_scale = 2 * np.double(max(input_scale, input2_scale))
            real_input1_multiplier = np.double(input_scale / twice_max_input_scale)
            real_input2_multiplier = np.double(input2_scale / twice_max_input_scale)
            real_output_multiplier = np.double(twice_max_input_scale / ((1 << left_shift) * output_scale))

            input_multiplier, input_shift = _getSigShift(real_input1_multiplier)
            input2_multiplier, input2_shift = _getSigShift(real_input2_multiplier)
            output_multiplier, output_shift = _getSigShift(real_output_multiplier)

            return (
                left_shift,
                input_multiplier,
                input_shift,
                input2_multiplier,
                input2_shift,
                output_multiplier,
                output_shift,
            )

        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]
        output_info = op["outputs"][0]

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        assert len(op["inputs"]) == 8, "Unexpected number of inputs"
        input_zero_point = self.data[op["inputs"][2]["name"]][0]
        output_zero_point = self.data[op["inputs"][6]["name"]][0]
        input2_zero_point = self.data[op["inputs"][3]["name"]][0]
        input_scale = self.data[op["inputs"][4]["name"]][0]
        input2_scale = self.data[op["inputs"][5]["name"]][0]
        output_scale = self.data[op["inputs"][7]["name"]][0]
        (
            left_shift,
            input_multiplier,
            input_shift,
            input2_multiplier,
            input2_shift,
            output_multiplier,
            output_shift,
        ) = _getADDMultiplierShift(input_scale, input2_scale, output_scale)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "input_h": input0_h,
            "input_w": input0_w,
            "input_c": input0_c,
            "input2_h": input0_h,
            "input2_w": input0_w,
            "input2_c": input0_c,
            "input_dim": 3,
            "input2_dim": 3,
            "output_dim": 3,
            "output_h": input0_h,
            "output_w": input0_w,
            "output_c": input0_c,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
            # trainable parameters
            "input_zero_point": input_zero_point,
            "input2_zero_point": input2_zero_point,
            "output_zero_point": output_zero_point,
            "input_scale": input_scale,
            "input2_scale": input2_scale,
            "output_scale": output_scale,
            # quantized infernece
            "left_shift": left_shift,
            "input_multiplier": input_multiplier,
            "input2_multiplier": input2_multiplier,
            "input_shift": input_shift,
            "input2_shift": input2_shift,
            "output_multiplier": output_multiplier,
            "output_shift": output_shift,
        }
        op = add.Add(params)

        return op

    def _convert_add1d(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]
        output_info = op["outputs"][0]

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "input_size": input0_h * input0_w * input0_c,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
        }
        op = add1d.add1d(params)

        return op

    def _convert_sub(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]
        output_info = op["outputs"][0]

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "input_size": input0_h * input0_w * input0_c,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
        }
        op = sub.sub(params)

        return op

    def _convert_div(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]
        output_info = op["outputs"][0]

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        if "scale" in input2_info["name"]:
            scale_from_add = self.data[input2_info["name"]][0]
        else:
            scale_from_add = None

        input2 = input2_info["meta"]["data"] if isconstanttstr(input2_info["name"]) else None
        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "input_size": input0_h * input0_w * input0_c,
            "input_dtype": input_dtype,
            "input2": input2,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
            # input of scale from some conv2d
            "scale_from_add": scale_from_add,
        }
        op = div.div(params)

        return op

    def _convert_mul(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]
        input2_shape = input2_info["shape"]
        input2_c, input2_h, input2_w = get_chw_shape(input2_shape)
        output_info = op["outputs"][0]
        output_c, output_h, output_w = get_chw_shape(output_info["shape"])

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        scale_conv_2d_op = None
        scale_from_add = None

        if "constant" in input2_info["name"]:
            constant = input2_info["meta"]["data"]
        else:
            constant = None

        if "scale" in input2_info["name"]:
            # should be a scaler then
            if "qadd" in input2_info["name"]:
                scale_from_add = self.data[input2_info["name"]][0]
                constant = self.data[input2_info["name"]][0]
            else:
                scale_conv_2d_op = _findConv2dwithScaleName(self.model, self.layer, input2_info["name"])

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "output_size": output_c * output_h * output_w,
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "input_size": input0_h * input0_w * input0_c,
            "input2_size": input2_h * input2_w * input2_c,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
            # input of scale from some conv2d
            "scale_conv_2d_op": scale_conv_2d_op,
            "scale_from_add": scale_from_add,
            "constant": constant,
        }
        op = mul.mul(params)

        return op

    def _convert_less(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "input_size": input0_c * input0_h * input0_w,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
        }
        op = less.less(params)

        return op

    def _convert_greater(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "input_size": input0_c * input0_h * input0_w,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
        }
        op = greater.greater(params)

        return op

    def _convert_where(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]
        input3_info = op["inputs"][2]

        output_info = op["outputs"][0]
        output_dtype = get_dtype(op["outputs"][0])
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        input3_dtype = get_dtype(input3_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "input2_idx": input2_info["name"],
            "input3_idx": input3_info["name"],
            "output_idx": output_info["name"],
            "input_size": input0_c * input0_h * input0_w,
            "output_size": output_c * output_h * output_w,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "input3_dtype": input3_dtype,
            "output_dtype": output_dtype,
        }
        op = where.where(params)

        return op

    def _convert_negative(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input0_shape)

        output_info = op["outputs"][0]

        # dtype
        input_dtype = get_dtype(input0_info)
        output_dtype = get_dtype(output_info)

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "output_idx": output_info["name"],
            "input_size": input_c * input_h * input_w,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }
        op = negative.negative(params)

        return op

    def _convert_sum(self, op):
        # shape
        input_info = op["inputs"][0]
        input_shape = input_info["shape"]
        if len(input_shape) == 4:
            d1, d2, d3, d4 = input_shape
        else:
            d2, d3, d4 = get_chw_shape(input_shape)
            d1 = 1

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        if len(output_shape) == 4:
            od1, od2, od3, od4 = output_shape
        else:
            od2, od3, od4 = get_chw_shape(output_shape)
            od1 = 1

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        # TODO: update here after bugs in TTE IR is fixed (null)
        if op["attrs"]["axis"] is None:
            op["attrs"]["exclude"] = 1
            axis = 0
        elif op["attrs"]["axis"][0] < 0:
            if op["attrs"]["axis"][0] == -1:
                axis = 3
            elif op["attrs"]["axis"][0] == -2:
                axis = 2
            elif op["attrs"]["axis"][0] == -3:
                axis = 1
            elif op["attrs"]["axis"][0] == -4:
                axis = 0
        else:
            axis = op["attrs"]["axis"][0]

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "axis": axis,
            "d1": d1,
            "d2": d2,
            "d3": d3,
            "d4": d4,
            "input_dim": 4,
            "output_dim": 3,
            "od1": od1,
            "od2": od2,
            "od3": od3,
            "od4": od4,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
            "exclude": bool(op["attrs"]["exclude"]),
        }
        op = sum.sum(params)

        return op

    def _convert_tile(self, op):
        # shape
        input_info = op["inputs"][0]
        input_shape = input_info["shape"]
        input_c, input_h, input_w = get_chw_shape(input_shape)

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        output_c, output_h, output_w = get_chw_shape(output_shape)

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        reps_size = len(op["attrs"]["reps"])
        reps = op["attrs"]["reps"]

        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "reps_size": reps_size,
            "reps": reps,
            "input_h": input_h,
            "input_w": input_w,
            "input_c": input_c,
            "input_dim": 3,
            "output_dim": 3,
            "output_h": output_h,
            "output_w": output_w,
            "output_c": output_c,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
        }
        op = tile.tile(params)

        return op

    def _convert_convolution(self, op):
        weight_info = op["inputs"][1]
        if op["attrs"]["kernel_size"] is None:
            kh = op["inputs"][1]["shape"][-1]
            kw = op["inputs"][1]["shape"][-2]
        else:
            kh, kw = op["attrs"]["kernel_size"]
        padding = op["attrs"]["padding"][0]
        groups = op["attrs"]["groups"]
        sw, sh = op["attrs"]["strides"]

        # shape
        input_info = op["inputs"][0]
        input_shape = input_info["shape"]
        _, input_c, input_h, input_w = input_shape

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        _, output_c, output_h, output_w = output_shape

        # dtype
        input_dtype = get_dtype(input_info)
        output_dtype = get_dtype(output_info)

        weight_name = weight_info["name"]
        if weight_name not in self.data:
            weight_value = weight_name
        else:
            weight_value = self.data[weight_name].transpose(0, 2, 3, 1)  # OIHW -> OHWI
            if (
                groups == input_c == output_c
            ):  # For depthwise conv we assume the weights to be in HWC in code generation,
                # we revert the tensor to match the assumption
                weight_value = weight_value.transpose(3, 1, 2, 0)  # OHWI -> IHWO

        has_bias = False
        for inp in op["inputs"]:
            if "weight" in inp["name"]:
                _accessTrainable(self.trainedWeights, inp["name"])
            if "bias" in inp["name"]:
                bias_name = inp["name"]
                bias = self.data[bias_name]
                has_bias = True
                _accessTrainable(self.trainedBias, inp["name"])
        # for floating point implementation, we allow no bias data since the data might be in bias_add ops
        if op["type"] not in {"nn.mcuconv2d"} and not has_bias:
            bias = np.zeros(output_shape[1])
            bias_name = None
            has_bias = True

        assert has_bias, "no bias data found!"

        # normal conv2d, depthwise, group conv
        if groups == 1:
            op_name_idx = 0
        elif groups == input_c == output_c and input_dtype == "int8" and output_dtype == "int32":
            op_name_idx = 1
        else:
            op_name_idx = 2

        if input_dtype == "int8" and output_dtype == "int32" and op_name_idx != 2 and op["type"] != "nn.conv2d":
            input_zero_point = self.data[_findKeyinTensors(op["inputs"], "zero_x")["name"]][0]
            output_zero_point = self.data[_findKeyinTensors(op["inputs"], "zero_y")["name"]][0]
            # TODO: if the IR saperate these
            input_scale = np.ones(output_shape[1])
            output_scale = np.ones(output_shape[1])
            weight_scale = np.ones(output_shape[1])
            # effective scales
            effective_scale = self.data[_findKeyinTensors(op["inputs"], "scale")["name"]]
        else:
            input_zero_point = 0
            output_zero_point = 0
            input_scale = np.ones(output_shape[1])
            output_scale = np.ones(output_shape[1])
            weight_scale = np.ones(output_shape[1])
            effective_scale = np.ones(output_shape[1])

        multiplier, shift = _getMultiplierShift(effective_scale)

        params = {
            "op": op_name_translation[op["type"]][op_name_idx],
            # SE blocks
            "is_SEBlock": "se" in op and op["se"],
            "kernel_h": kh,
            "kernel_w": kw,
            "padding": padding,
            "padding_h": op["attrs"]["padding"][0],
            "padding_w": op["attrs"]["padding"][1],
            "dilation_h": op["attrs"]["dilation"][0],
            "dilation_w": op["attrs"]["dilation"][1],
            "groups": groups,
            "stride_h": sh,
            "stride_w": sw,
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_dim": 3,
            "input_c": input_c,
            "input_h": input_h,
            "input_w": input_w,
            "output_dim": 3,
            "output_c": output_c,
            "output_h": output_h,
            "output_w": output_w,
            "input_dtype": input_dtype,
            "output_dtype": output_dtype,
            "weight_value": weight_value,
            "weight_name": weight_name,
            "bias": bias.astype(int),
            "bias_name": bias_name,
            "effective_scale": effective_scale,
            "input_zero_point": input_zero_point,
            "output_zero_point": output_zero_point,
            "multiplier": multiplier.astype(int),
            "shift": shift.astype(int),
            "input_scale": input_scale,
            "output_scale": output_scale,
            "weight_scale": weight_scale,
        }

        # assert weight_value != "out_nn.conv2d_transpose@330"
        if groups == 1:
            op = conv2d.Conv2d(params)
        elif groups == input_c == output_c and input_dtype == "int8" and output_dtype == "int32":
            op = depthwiseConv2d.DepthwiseConv2d(params)
        else:
            if "inplace_int8_input" in op and op["inplace_int8_input"]:
                params["inplace_int8_input"] = True
            params["float32_input2"] = True if op["inputs"][1]["dtype"] == "float32" else False
            op = group_conv2d.groupConv2d(params)

        if isinstance(weight_value, str):
            w_c, w_w, w_h = (
                weight_info["shape"][1] * weight_info["shape"][2],
                weight_info["shape"][2],
                weight_info["shape"][3],
            )
            op._add_input(weight_name, input_dtype, w_h, w_w, w_c)

        # for int8 group conv
        if op_name_idx == 2 and "float32_input2" in params and not params["float32_input2"]:
            # we will use int8 for input and output, dynamic quantization will be inside ops
            for input_tensor in op.input_tensors:
                input_tensor.dtype = "int8"
            op.output_tensors[0].dtype = "int8"
            op.params["intput_dtype"] = "int8"
            op.params["intput2_dtype"] = "int8"
            op.params["output_dtype"] = "int8"
            # we need int32 intermediate buffer for normalization
            op.add_int32_buffer_tensor()

        return op

    def _convert_transpose_conv2d(self, op):
        assert op["attrs"]["kernel_size"] is not None
        kh, kw = op["attrs"]["kernel_size"]
        padding = op["attrs"]["padding"][0]
        groups = op["attrs"]["groups"]
        sw, sh = op["attrs"]["strides"]

        # shape
        input_info = op["inputs"][0]
        input2_info = op["inputs"][1]
        input_shape = input_info["shape"]
        _, input_c, input_h, input_w = input_shape

        output_info = op["outputs"][0]
        output_shape = output_info["shape"]
        _, output_c, output_h, output_w = output_shape

        # dtype
        input_dtype = get_dtype(input_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        weight_info = op["inputs"][-1]
        weight_name = weight_info["name"]
        for inp in op["inputs"]:
            if "weight" in inp["name"]:
                _accessTrainable(self.trainedWeights, inp["name"])
            if "bias" in inp["name"]:
                _accessTrainable(self.trainedBias, inp["name"])
        if weight_name not in self.data:
            weight_value = weight_name
        else:
            weight_value = self.data[weight_name]
        # currently ignore the bias
        bias = np.zeros(input_shape[1])

        params = {
            "op": op_name_translation[op["type"]],
            # SE blocks
            "is_SEBlock": "se" in op and op["se"],
            "kernel_h": kh,
            "kernel_w": kw,
            "padding": padding,
            "padding_h": op["attrs"]["padding"][0],
            "padding_w": op["attrs"]["padding"][1],
            "kernel_layout": op["attrs"]["kernel_layout"],
            "group": groups,
            "stride_h": sh,
            "stride_w": sw,
            "input_idx": input_info["name"],
            "output_idx": output_info["name"],
            "input_dim": 3,
            "input_c": input_shape[1],
            "input_h": input_shape[2],
            "input_w": input_shape[3],
            "output_dim": 3,
            "output_c": output_shape[1],
            "output_h": output_shape[2],
            "output_w": output_shape[3],
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
            "weight_value": weight_value,
            "weight_name": weight_name,
            "bias": bias.astype(int),
        }

        op_instance = transpose_conv2d.transposeConv2d(params)

        if isinstance(weight_value, str):
            w_c, w_w, w_h = (
                weight_info["shape"][0] * weight_info["shape"][1],
                weight_info["shape"][2],
                weight_info["shape"][3],
            )
            op_instance._add_input(weight_name, input_dtype, w_h, w_w, w_c)

        # for full int8 tranpose conv
        #  1. Cast dtype as we do requantize inside ops
        #  2. we need int32 intermediate buffer for normalization and
        #  3. update the output type as int8 as we fuze nomalization inside a single op
        if params["input2_dtype"] == params["input_dtype"] == "int8" and params["output_dtype"] == "int32":
            # we will use int8 for input and output, dynamic quantization will be inside ops
            for input_tensor in op_instance.input_tensors:
                input_tensor.dtype = "int8"
            op_instance.output_tensors[0].dtype = "int8"
            op_instance.params["intput_dtype"] = "int8"
            op_instance.params["intput2_dtype"] = "int8"
            op_instance.params["output_dtype"] = "int8"
            op_instance.add_int32_buffer_tensor()

        return op_instance

    def _convert_matmul(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]
        input2_shape = input2_info["shape"]
        output_info = op["outputs"][0]
        input2_c, input2_h, input2_w = get_chw_shape(input2_shape)

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        weight_info = op["inputs"][-1]
        weight_name = weight_info["name"]
        for inp in op["inputs"]:
            if "weight" in inp["name"]:
                _accessTrainable(self.trainedWeights, inp["name"])
            if "bias" in inp["name"]:
                _accessTrainable(self.trainedBias, inp["name"])
        if weight_name not in self.data:
            weight_value = weight_name
        else:
            weight_value = self.data[weight_name]
        assert input0_w == input2_h
        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "matA_row": input0_h,
            "matA_col": input0_w,
            "matB_row": input2_h,
            "matB_col": input2_w,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
            "weight_value": weight_value,
            "weight_name": weight_name,
        }
        op = mat_mul.mat_mul(params)

        if isinstance(weight_value, str):
            w_c, w_w, w_h = get_chw_shape(weight_info["shape"])
            op._add_input(weight_name, input_dtype, w_h, w_w, w_c)

        return op

    def _convert_dense(self, op):
        # shape
        input0_info = op["inputs"][0]
        input0_shape = input0_info["shape"]
        input0_c, input0_h, input0_w = get_chw_shape(input0_shape)

        input2_info = op["inputs"][1]
        input2_shape = input2_info["shape"]
        output_info = op["outputs"][0]
        input2_c, input2_h, input2_w = get_chw_shape(input2_shape)

        # dtype
        input_dtype = get_dtype(input0_info)
        input2_dtype = get_dtype(input2_info)
        output_dtype = get_dtype(output_info)

        weight_info = op["inputs"][-1]
        weight_name = weight_info["name"]
        for inp in op["inputs"]:
            if "weight" in inp["name"]:
                _accessTrainable(self.trainedWeights, inp["name"])
            if "bias" in inp["name"]:
                _accessTrainable(self.trainedBias, inp["name"])
        if weight_name not in self.data:
            weight_value = weight_name
        else:
            weight_value = self.data[weight_name]
        assert input0_w == input2_w
        params = {
            # operator
            "op": op_name_translation[op["type"]],
            # tensor
            "input_idx": input0_info["name"],
            "input2_idx": input2_info["name"],
            "output_idx": output_info["name"],
            "matA_row": input0_h,
            "matA_col": input0_w,
            "matB_row": input2_h,
            "matB_col": input2_w,
            "input_dtype": input_dtype,
            "input2_dtype": input2_dtype,
            "output_dtype": output_dtype,
            "weight_value": weight_value,
            "weight_name": weight_name,
        }
        op = dense.dense(params)

        if isinstance(weight_value, str):
            w_c, w_w, w_h = get_chw_shape(weight_info["shape"])
            op._add_input(weight_name, input_dtype, w_h, w_w, w_c)

        return op


def get_dtype(tensor_info):
    if "dtype" in tensor_info:
        return tensor_info["dtype"]
    else:
        return "int8"


def get_chw_shape(shape):
    input_c = input_h = input_w = 0
    batch = 1
    if len(shape) == 5:
        _, batch, input_c, input_h, input_w = shape
    elif len(shape) == 4:
        batch, input_c, input_h, input_w = shape
    elif len(shape) == 2:
        input_h, input_w = shape
        input_c = 1
    elif len(shape) == 1:
        input_h = input_w = 1
        input_c = shape[0]
    else:
        raise NotImplementedError
    if batch != 1:
        # raise ValueError("batch size should be 1")
        warnings.warn("batch size should be 1")
    return input_c, input_h, input_w


def _getMultiplierShift(effective_scale):
    significand = np.zeros(len(effective_scale), dtype="int32")
    shift = np.zeros(len(effective_scale), dtype="int32")

    for i, s in enumerate(effective_scale):
        if s == 0:
            significand[i] = 0
            shift[i] = 0
        else:
            sig, shi = math.frexp(s)
            sig = int(round(sig * 2**31))

            if sig == 2**31:
                sig /= 2
                shi += 1
            if shi < -31:
                shi = 0
                sig = 0

            significand[i] = sig
            shift[i] = shi

    return significand, shift
