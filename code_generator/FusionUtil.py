from .constant import op_name_translation
from .GraphReorder import find_following_link_op, find_previous_link_op

__all__ = [
    "_accessTrainable",
    "_findKeyinTensors",
    "_removeLayers",
    "_findTargetWeightforGconv",
    "_findBinMaskPattern",
    "_findBinMaskPatternint8",
    "_castisFusable",
    "_castisFusable_for_gconv",
    "_castSliceisFusable",
    "_fileTileRepAsWeights",
    "_updateOutputDtype",
    "_findConv2dwithScaleName",
    "_updateIdxParameter",
    "_updateIdx",
    "_findPartialConv",
    "_findWhereTensorFrom",
    "_findWhereTheTensorIdxFrom",
    "_findTransposeMaxDivideDivide",
    "_findNextOpTakeInputName",
    "_findAbsMaxDivideDivide",
    "_findTransposeMultiplyAbsMaxDivide",
    "_findMultiplyAbsMaxDivide",
]


def _findWhereTensorFrom(layers, tensor_idx):
    for op in layers:
        if op.params["output_idx"] == tensor_idx:
            return op
    return None


def _findPartialConv(layers, weight_name):
    for op in layers:
        if "weight_name" in op.params and op.params["input_dtype"] == "int8":
            if op.params["weight_name"] == weight_name:
                return op
    raise AssertionError


def _findKeyinTensors(inputs, key):
    for _, inp in enumerate(inputs):
        if key in inp["name"]:
            return inp
    raise KeyError


def _accessTrainable(table, name):
    in_table = False
    for t in table:
        if t.name is name:
            in_table = True
            t.access_cnt += 1
            break

    if not in_table:
        table.append(trainableTensor(name, 0))


def _removeLayers(layers, target_dict):
    for k in target_dict:
        layers.remove(target_dict[k])
    return layers


# find gconv -> reshape -> sum -> transpose (weight_idx)
def _findTargetWeightforGconv(layers, output_idx):
    children = 0
    reshape_op = 0
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == output_idx:
                children += 1
                reshape_op = op
    # see if we have for reshape
    if not (children == 1 and reshape_op["type"] == "reshape"):
        return None
    # reshape -> sum
    sum_op = None
    children = 0
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == reshape_op["outputs"][0]["name"]:
                children += 1
                sum_op = op
    if not (children == 1 and sum_op["type"] == "sum"):
        return None
    # sum -> transpose
    transpose_op = None
    children = 0
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == sum_op["outputs"][0]["name"]:
                children += 1
                transpose_op = op
    if not (children == 1 and transpose_op["type"] == "transpose"):
        return None
    if transpose_op["outputs"][0]["meta"]["children"] == 0:
        if transpose_op["outputs"][0]["meta"]["output_info"][0] == "v":
            return transpose_op["outputs"][0]["meta"]["output_info"]
        return "v" + transpose_op["outputs"][0]["meta"]["output_info"]
    else:
        # for int8 bp we may need abs -> max -> divide (127) -> divide (output activation) -> cast to cliping
        last_op = transpose_op
        op_start = _findNextOpTakeInputName(layers, last_op["outputs"][0]["name"])
        # if abs["type"] == "abs":
        #     op_start = _findNextOpTakeInputName(layers, abs["outputs"][0]["name"])
        #     if op0["type"] == "max":
        #         op1 =  _findNextOpTakeInputName(layers, op0["outputs"][0]["name"])
        #         if op1["type"] == "divide":
        #             op2 =  _findNextOpTakeInputName(layers, op1["outputs"][0]["name"])
        #             if op2["type"] == "divide":
        #                 cast =  _findNextOpTakeInputName(layers, op2["outputs"][0]["name"])
        #                 if cast["type"] == "cast":
        #                     if cast["outputs"][0]["meta"]["output_info"][0] == "v":
        #                         return cast["outputs"][0]["meta"]["output_info"]
        #                     return "v" + cast["outputs"][0]["meta"]["output_info"]
        #                 else:
        #                     last_op = cast
        # for int8 bp we may have the following pattern
        #                -> (cast1) -> multiply
        #  from op  ->                       -> divide -> cast
        #                -> abs -> max -> (cast2)
        if op_start["type"] == "multiply":
            op0 = _findNextOpTakeInputName(layers, op_start["outputs"][0]["name"])
            if op0["type"] == "divide":
                cast = _findNextOpTakeInputName(layers, op0["outputs"][0]["name"])
                if cast["type"] == "cast":
                    if cast["outputs"][0]["meta"]["output_info"][0] == "v":
                        return cast["outputs"][0]["meta"]["output_info"]
                    return "v" + cast["outputs"][0]["meta"]["output_info"]
                else:
                    last_op = cast
        elif op_start["type"] == "cast":
            op_0 = _findNextOpTakeInputName(layers, op_start["outputs"][0]["name"])
            if op_0["type"] == "multiply":
                op1 = _findNextOpTakeInputName(layers, op_0["outputs"][0]["name"])
                if op1["type"] == "divide":
                    cast = _findNextOpTakeInputName(layers, op1["outputs"][0]["name"])
                    if cast["type"] == "cast":
                        if cast["outputs"][0]["meta"]["output_info"][0] == "v":
                            return cast["outputs"][0]["meta"]["output_info"]
                        return "v" + cast["outputs"][0]["meta"]["output_info"]
                    else:
                        last_op = cast
        elif op_start["type"] == "abs":
            op0 = _findNextOpTakeInputName(layers, op_start["outputs"][0]["name"])
            if op0["type"] == "max":
                op1 = _findNextOpTakeInputName(layers, op0["outputs"][0]["name"])
                if op1["type"] == "divide":
                    cast = _findNextOpTakeInputName(layers, op1["outputs"][0]["name"])
                    if cast["type"] == "cast":
                        if cast["outputs"][0]["meta"]["output_info"][0] == "v":
                            return cast["outputs"][0]["meta"]["output_info"]
                        return "v" + cast["outputs"][0]["meta"]["output_info"]
                    else:
                        last_op = cast
        elif op_start["type"] == "abs":
            op0 = _findNextOpTakeInputName(layers, op_start["outputs"][0]["name"])
            if op0["type"] == "max":
                op1 = _findNextOpTakeInputName(layers, op0["outputs"][0]["name"])
                if op1["type"] == "cast":
                    op2 = _findNextOpTakeInputName(layers, op1["outputs"][0]["name"])
                    if op2["type"] == "divide":
                        cast = _findNextOpTakeInputName(layers, op2["outputs"][0]["name"])
                        if cast["type"] == "cast":
                            if cast["outputs"][0]["meta"]["output_info"][0] == "v":
                                return cast["outputs"][0]["meta"]["output_info"]
                            return "v" + cast["outputs"][0]["meta"]["output_info"]
                        else:
                            last_op = cast

        # we may have slice op
        slice_op = None
        for op in layers:
            for input_tensor in op["inputs"]:
                if input_tensor["name"] == last_op["outputs"][0]["name"]:
                    children += 1
                    slice_op = op
                    assert slice_op["outputs"][0]["meta"]["children"] == 0
                    if slice_op["outputs"][0]["meta"]["output_info"][0] == "v":
                        return slice_op["outputs"][0]["meta"]["output_info"]
                    return "v" + slice_op["outputs"][0]["meta"]["output_info"]


def _findNextOpTakeInputName(layers, input_name):
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == input_name:
                return op
    return None


def _findNextOpListTakeInputName(layers, input_name):
    ops = []
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == input_name:
                ops.append(op)
    return ops


# check if we need to have binary mask for this conv2d
# conv2d (int32) -> cast -> greater/less -> multiply -> binary mask
def _findBinMaskPattern(layers, output_idx):
    children = 0
    f_ops = []
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == output_idx:
                children += 1
                f_ops.append(op)
    # see if we have cast op
    cast_op = greater_op = less_op = multiply_op = where_op = None
    if children >= 2:
        for op in f_ops:
            if op["type"] == "cast":
                cast_op = op
                break
    if None in [cast_op]:
        return False, {}
    # find greater/less op
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == cast_op["outputs"][0]["name"]:
                if op["type"] in {"greater", "greater_equal"}:
                    greater_op = op
                elif op["type"] in {"less", "less_equal"}:
                    less_op = op
    if None in [greater_op, less_op]:
        return False, {}
    # find multiply
    for op in layers:
        if op["type"] == "multiply":
            if op["inputs"][0]["name"] in [greater_op["outputs"][0]["name"], less_op["outputs"][0]["name"]] and op[
                "inputs"
            ][1]["name"] in [
                greater_op["outputs"][0]["name"],
                less_op["outputs"][0]["name"],
            ]:
                multiply_op = op
                break
    if None in [multiply_op]:
        return False, {}
    # find where
    for op in layers:
        if op["type"] == "where":
            if op["inputs"][0]["name"] in multiply_op["outputs"][0]["name"]:
                where_op = op
                break
    if where_op is None:
        return False, {}
    op_dict = {
        "cast": cast_op,
        "greater": greater_op,
        "less": less_op,
        "multiply": multiply_op,
        # "where": where_op,
        # "zeros": zeros_op,
        # "ones": ones_op,
    }
    return True, op_dict


# this is for int8
# check if we need to have binary mask for this conv2d
# conv2d (int32) -> greater/less -> multiply -> binary mask
def _findBinMaskPatternint8(layers, output_idx):
    children = 0
    f_ops = []
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == output_idx:
                children += 1
                f_ops.append(op)
    # see if we have cast op
    greater_op = less_op = multiply_op = where_op = None
    if children >= 2:
        for op in f_ops:
            if op["type"] in {"greater", "greater_equal"}:
                greater_op = op
            elif op["type"] in {"less", "less_equal"}:
                less_op = op
    if None in [greater_op, less_op]:
        return False, {}
    # find multiply
    for op in layers:
        if op["type"] == "multiply":
            if op["inputs"][0]["name"] in [greater_op["outputs"][0]["name"], less_op["outputs"][0]["name"]] and op[
                "inputs"
            ][1]["name"] in [
                greater_op["outputs"][0]["name"],
                less_op["outputs"][0]["name"],
            ]:
                multiply_op = op
                break
    if None in [multiply_op]:
        return False, {}
    # find where
    for op in layers:
        if op["type"] == "where":
            if op["inputs"][0]["name"] in multiply_op["outputs"][0]["name"]:
                where_op = op
                break
    if where_op is None:
        return False, {}
    # packing return ops
    op_dict = {
        "greater": greater_op,
        "less": less_op,
        "multiply": multiply_op,
    }
    return True, op_dict


# check the graph and see if we can fuse the cast into transpose conv2d
def _castisFusable(layers, cast_op):
    # find the output
    fp_output_idx = cast_op["outputs"][0]["name"]
    children = 0
    following_op = None
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == fp_output_idx:
                children += 1
                following_op = op
    # checking the graph matches the patten
    # int8 -> fp32
    # only used as the weight of some transpose conv
    if (
        cast_op["inputs"][0]["dtype"] == "int8"
        and cast_op["outputs"][0]["dtype"] == "float32"
        and children == 1
        and following_op["type"] == "nn.conv2d_transpose"
        and following_op["inputs"][1]["name"] == fp_output_idx
    ):
        return True, following_op
    return False, following_op


# check the graph and see if we can fuse the cast into group conv2d
# cast -> reshape -> group_conv
def _castisFusable_for_gconv(layers, cast_op):
    # find the output
    idx, f_op_1 = find_following_link_op(layers, cast_op)
    if f_op_1["type"] != "reshape":
        return False, None
    idx, f_op_2 = find_following_link_op(layers, f_op_1)
    if (
        f_op_2["type"] == "nn.conv2d"
        and cast_op["inputs"][0]["dtype"] == "int8"
        and cast_op["outputs"][0]["dtype"] == "float32"
        and f_op_2["inputs"][0]["name"] == f_op_1["outputs"][0]["name"]
    ):
        # remove reshape
        # layers.remove(f_op_1)
        return True, f_op_2
    return False, f_op_2


# check the graph and see if we can fuse the cast into sclice conv2d
# Note: It seems we don't need this
def _castSliceisFusable(layers, cast_op):
    # find the output
    fp_output_idx = cast_op["outputs"][0]["name"]
    children = 0
    slice_op = reshape_op = g_conv_op = None
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == fp_output_idx:
                children += 1
                slice_op = op
    # checking the graph matches the patten
    # cast -> slice -> reshape -> transponse_conv
    # find slice
    if not (children == 1 and slice_op["type"] == "strided_slice"):
        return False, slice_op, reshape_op, g_conv_op
    # find reshape conv
    children = 0
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == slice_op["outputs"][0]["name"] and op["type"] == "reshape":
                children += 1
                reshape_op = op
    if not (children == 1 and reshape_op["type"] == "reshape"):
        return False, slice_op, reshape_op, g_conv_op
    # find g conv
    children = 0
    for op in layers:
        if op["type"] == "nn.conv2d" and op["inputs"][0]["name"] == reshape_op["outputs"][0]["name"]:
            children += 1
            g_conv_op = op
    if not (
        children == 1
        and g_conv_op["attrs"]["groups"] > 1
        and cast_op["inputs"][0]["dtype"] == "int8"
        and cast_op["outputs"][0]["dtype"] == "float32"
    ):
        return False, slice_op, reshape_op, g_conv_op
    # only used as the weight of some transpose conv
    return True, slice_op, reshape_op, g_conv_op


# check if we need to fuse ops for tile
# ##########   tile  -> reshape -> conv2d (which takes it as weights)
# fusion     | ------------------------|
def _fileTileRepAsWeights(layers, tile_op):
    reshape_op = conv2d_op = None
    children = 0
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == tile_op["outputs"][0]["name"]:
                children += 1
    assert children == 1, "output of tile is used by other layers"
    # find reshape op
    for op in layers:
        for input_tensor in op["inputs"]:
            if input_tensor["name"] == tile_op["outputs"][0]["name"]:
                if op["type"] == "reshape":
                    reshape_op = op
    if None in [reshape_op]:
        return False, {}
    # find conv2d
    for op in layers:
        if op["type"] == "nn.conv2d":
            if op["inputs"][1]["name"] == reshape_op["outputs"][0]["name"]:
                conv2d_op = op
                break
    if None in [conv2d_op]:
        return False, {}
    op_dict = {
        "tile": tile_op,
        "reshape": reshape_op,
        "conv2d": conv2d_op,
    }
    return True, op_dict


def _findWhereTheTensorIdxFrom(model, tensor_idx):
    for _, op in enumerate(model):
        if op["outputs"][0]["name"] == tensor_idx:
            return op
    return None


def _updateOutputDtype(layer, origin_idx, type):
    for _, prev_op in enumerate(layer):
        if prev_op.params["output_idx"] == origin_idx:
            prev_op.params["output_dtype"] = type
            prev_op.output_tensors[0].dtype = type


def _findConv2dwithScaleName(model, layers, scale_idx):
    for _, op in enumerate(model):
        if op["type"] == "nn.mcuconv2d":
            if _findKeyinTensors(op["inputs"], "scale")["name"] == scale_idx:
                # find the layer
                for layer in layers:
                    if layer.params["op"] in op_name_translation["nn.mcuconv2d"]:
                        if layer.params["weight_name"] == _findKeyinTensors(op["inputs"], "weight")["name"]:
                            return layer
    raise AssertionError


def _updateIdxParameter(json, origin_idx, new_idx):
    # update input json
    for _, op in enumerate(json):
        for t in op["inputs"]:
            if t["name"] == new_idx:
                t["name"] = origin_idx
    # comment this since we should not need to traceback to the tensor
    # for _, prev_op in enumerate(layer):
    #     # get all tensors for this layer
    #     for t_cnt, t in enumerate(prev_op.input_tensors):
    #         if t_cnt == 0 and prev_op.params["input_idx"] == new_idx:
    #             prev_op.params["input_idx"] = origin_idx
    #             t.graph_idx = origin_idx
    #         elif t_cnt == 1 and "input2_idx" in prev_op.params and prev_op.params["input2_idx"] == new_idx:
    #             prev_op.params["input2_idx"] = origin_idx
    #             t.graph_idx = origin_idx
    #         elif t_cnt == 2 and "input3_idx" in prev_op.params and prev_op.params["input3_idx"] == new_idx:
    #             prev_op.params["input3_idx"] = origin_idx
    #             t.graph_idx = origin_idx
    #     if prev_op.params["output_idx"] == new_idx:
    #         prev_op.params["output_idx"] = origin_idx
    #         prev_op.output_tensors[0].graph_idx = origin_idx
    #     if "weight_name" in prev_op.params and prev_op.params["weight_name"] == new_idx:
    #         prev_op.params["weight_name"] = origin_idx
    #     if "weight_value" in prev_op.params and prev_op.params["weight_value"] == new_idx:
    #         prev_op.params["weight_value"] = origin_idx


def _updateIdx(json, layer, origin_idx, new_idx):
    # update input json
    for _, op in enumerate(json):
        for t in op["inputs"]:
            if t["name"] == origin_idx:
                t["name"] = new_idx
        for t in op["outputs"]:
            if t["name"] == origin_idx:
                t["name"] = new_idx
        # update idx for trainable tensors
    # traceback to the tensor
    for _, prev_op in enumerate(layer):
        # get all tensors for this layer
        for t_cnt, t in enumerate(prev_op.input_tensors):
            if t_cnt == 0 and prev_op.params["input_idx"] == origin_idx:
                prev_op.params["input_idx"] = new_idx
                t.graph_idx = new_idx
            elif t_cnt == 1 and "input2_idx" in prev_op.params and prev_op.params["input2_idx"] == origin_idx:
                prev_op.params["input2_idx"] = new_idx
                t.graph_idx = new_idx
            elif t_cnt == 2 and "input3_idx" in prev_op.params and prev_op.params["input3_idx"] == origin_idx:
                prev_op.params["input3_idx"] = new_idx
                t.graph_idx = new_idx
        if prev_op.params["output_idx"] == origin_idx and len(prev_op.output_tensors) > 0:
            prev_op.params["output_idx"] = new_idx
            prev_op.output_tensors[0].graph_idx = new_idx
        if "weight_name" in prev_op.params and prev_op.params["weight_name"] == origin_idx:
            prev_op.params["weight_name"] = new_idx
        if (
            "weight_value" in prev_op.params
            and isinstance(prev_op.params["weight_value"], str)
            and prev_op.params["weight_value"] == origin_idx
        ):
            prev_op.params["weight_value"] = new_idx


# Purpose is tofuse "transpose" -> [abs -> max -> divide -> divide -> cast (int8 bp)]
def _findTransposeMaxDivideDivide(model, transpose_op):
    abs_op = None
    max_op = None
    divide0 = None
    divide1 = None
    cast_op = None

    abs_op = _findNextOpTakeInputName(model, transpose_op["outputs"][0]["name"])
    if not abs_op or abs_op["type"] != "abs":
        return False, {}
    max_op = _findNextOpTakeInputName(model, abs_op["outputs"][0]["name"])
    if not max_op or max_op["type"] != "max":
        return False, {}
    divide0 = _findNextOpTakeInputName(model, max_op["outputs"][0]["name"])
    if not divide0 or divide0["type"] != "divide":
        return False, {}
    divide1 = _findNextOpTakeInputName(model, divide0["outputs"][0]["name"])
    if not divide1 or divide1["type"] != "divide":
        return False, {}
    cast_op = _findNextOpTakeInputName(model, divide1["outputs"][0]["name"])
    if not cast_op or cast_op["type"] != "cast":
        return False, {}

    return True, {"abs": abs_op, "max": max_op, "divide_0": divide0, "divide_1": divide1, "cast": cast_op}


# Purpose is to fuse for int8 bp
#                -> (cast1) -> multiply
#  transpose ->                       -> divide -> cast
#                -> abs -> max -> (cast2)
def _findTransposeMultiplyAbsMaxDivide(model, transpose_op):
    multiply_op = None
    cast_1 = None
    abs_op = None
    max_op = None
    cast_2 = None
    divide_op = None
    cast_op = None

    ops = _findNextOpListTakeInputName(model, transpose_op["outputs"][0]["name"])

    if len(ops) == 2:
        if ops[0]["type"] == "abs" and ops[1]["type"] in ["multiply", "cast"]:
            abs_op = ops[0]
            # cast1
            if ops[1]["type"] == "cast":
                cast_1 = ops[1]
                multiply_op = _findNextOpTakeInputName(model, cast_1["outputs"][0]["name"])
            else:
                multiply_op = ops[1]
        else:
            abs_op = ops[1]
            if ops[0]["type"] == "cast":
                cast_1 = ops[0]
                multiply_op = _findNextOpTakeInputName(model, cast_1["outputs"][0]["name"])
            else:
                multiply_op = ops[0]
    else:
        return False, {}

    if abs_op["type"] == "abs":
        # max
        max_op = _findNextOpTakeInputName(model, abs_op["outputs"][0]["name"])
        if not max_op or max_op["type"] != "max":
            return False, {}
        next_of_max = _findNextOpTakeInputName(model, max_op["outputs"][0]["name"])
        # -> (cast2) -> divide or divide
        if next_of_max["type"] == "cast":
            cast_2 = next_of_max
            divide_op = _findNextOpTakeInputName(model, cast_2["outputs"][0]["name"])
        else:
            divide_op = _findNextOpTakeInputName(model, max_op["outputs"][0]["name"])
        if not divide_op or divide_op["type"] != "divide":
            return False, {}
        # -> cast
        cast_op = _findNextOpTakeInputName(model, divide_op["outputs"][0]["name"])
        if not cast_op or cast_op["type"] != "cast":
            return False, {}
    if multiply_op["type"] != "multiply":
        return False, {}

    op_dict = {
        "abs": abs_op,
        "multiply_op": multiply_op,
        "abs": abs_op,
        "max": max_op,
        "divide": divide_op,
        "cast": cast_op,
    }
    if cast_1:
        op_dict["cast_1"] = cast_1
    if cast_2:
        op_dict["cast_2"] = cast_2

    return True, op_dict


# Find the following for int8 bp
#                -> (cast1) -> multiply
#  from op  ->                       -> divide -> cast
#                -> abs -> max -> (cast2)
def _findMultiplyAbsMaxDivide(model, abs_op):
    multiply_op = None
    max_op = None
    divide_op = None
    cast_op = None
    cast_1 = None
    cast_2 = None

    _, from_op = find_previous_link_op(model, abs_op)
    ops = _findNextOpListTakeInputName(model, from_op["outputs"][0]["name"])

    if len(ops) == 2:
        if ops[0]["type"] == "abs" and ops[1]["type"] in ["multiply", "cast"]:
            abs_op = ops[0]
            # cast1
            if ops[1]["type"] == "cast":
                cast_1 = ops[1]
                multiply_op = _findNextOpTakeInputName(model, cast_1["outputs"][0]["name"])
            else:
                multiply_op = ops[1]
        else:
            abs_op = ops[1]
            if ops[0]["type"] == "cast":
                cast_1 = ops[0]
                multiply_op = _findNextOpTakeInputName(model, cast_1["outputs"][0]["name"])
            else:
                multiply_op = ops[0]
    else:
        return False, {}

    if abs_op["type"] == "abs":
        # max
        max_op = _findNextOpTakeInputName(model, abs_op["outputs"][0]["name"])
        if not max_op or max_op["type"] != "max":
            return False, {}
        next_of_max = _findNextOpTakeInputName(model, max_op["outputs"][0]["name"])
        # -> (cast2) -> divide or divide
        if next_of_max["type"] == "cast":
            cast_2 = next_of_max
            divide_op = _findNextOpTakeInputName(model, cast_2["outputs"][0]["name"])
        else:
            divide_op = _findNextOpTakeInputName(model, max_op["outputs"][0]["name"])
        if not divide_op or divide_op["type"] != "divide":
            return False, {}
        # -> cast
        cast_op = _findNextOpTakeInputName(model, divide_op["outputs"][0]["name"])
        if not cast_op or cast_op["type"] != "cast":
            return False, {}
    if multiply_op["type"] != "multiply":
        return False, {}

    op_dict = {"multiply_op": multiply_op, "abs": abs_op, "max": max_op, "divide": divide_op, "cast": cast_op}
    if cast_1:
        op_dict["cast_1"] = cast_1
    if cast_2:
        op_dict["cast_2"] = cast_2

    return True, op_dict


# find [abs -> max -> divide -> divide (int8 bp)]
def _findAbsMaxDivideDivide(model, abs_op):
    max_op = None
    divide0 = None
    divide1 = None
    cast_op = None

    max_op = _findNextOpTakeInputName(model, abs_op["outputs"][0]["name"])
    if not max_op or max_op["type"] != "max":
        return False, {}
    divide0 = _findNextOpTakeInputName(model, max_op["outputs"][0]["name"])
    if not divide0 or divide0["type"] != "divide":
        return False, {}
    divide1 = _findNextOpTakeInputName(model, divide0["outputs"][0]["name"])
    if not divide1 or divide1["type"] != "divide":
        return False, {}
    cast_op = _findNextOpTakeInputName(model, divide1["outputs"][0]["name"])
    if not cast_op or cast_op["type"] != "cast":
        return False, {}

    return True, {"max": max_op, "divide_0": divide0, "divide_1": divide1, "cast": cast_op}


class trainableTensor:
    def __init__(self, name=None, access_cnt=None) -> None:
        self.name = name
        self.access_cnt = access_cnt
        self.allocated_name = None
