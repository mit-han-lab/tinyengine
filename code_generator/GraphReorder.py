def find_previous_link_op(model, target_op):
    tensor_name = target_op["inputs"][0]["name"]
    for idx, previous_op in enumerate(model):
        if previous_op["outputs"][0]["name"] == tensor_name:
            return idx, previous_op


def find_previous_link_op_input2(model, target_op):
    tensor_name = target_op["inputs"][1]["name"]
    for idx, previous_op in enumerate(model):
        if previous_op["outputs"][0]["name"] == tensor_name:
            return idx, previous_op


def find_following_link_op(model, target_op):
    tensor_name = target_op["outputs"][0]["name"]
    for idx, following_op in enumerate(model):
        for input_t in following_op["inputs"]:
            if input_t["name"] == tensor_name:
                return idx, following_op
    return None, None


def reorderGroupConv_TransponseConv(model):
    global_index = 0
    # compact the group conv op ordering
    # cast -> reshape -> (... which we want to skip) -> tile -> reshape -> nn.conv2d -> reshape -> sum ->
    # transpose -> [max -> divide -> divide (int8 bp)]
    for _, op in enumerate(model):
        if op["type"] == "cast":
            resshape_idx, reshape = find_following_link_op(model, op)
            if not reshape:
                continue
            if reshape["type"] != "reshape":
                continue
            conv2d_idx, conv2d = find_following_link_op(model, reshape)
            if reshape["type"] == "reshape" and conv2d["type"] == "nn.conv2d":
                resshape2_idx, reshape2 = find_previous_link_op_input2(model, conv2d)
                tile_idx, tile = find_previous_link_op(model, reshape2)
                if not (tile["type"] == "tile" and reshape2["type"] == "reshape"):
                    continue
                model.remove(reshape)
                model.remove(op)
                model.insert(tile_idx - 2, op)
                model.insert(tile_idx - 1, reshape)
    # compact the transpose conv
    while global_index < len(model):
        # find cast - > reshape -> tile ...-> conv2d(group) -> ... -> transpose (wiht 'weight' in 'output_info)
        conv2d_set_start_idx = None
        conv2d_set_end_idx = None
        transpose_conv_idx = None
        for cnt in range(global_index, len(model)):
            op = model[cnt]
            if op["type"] == "transpose" and "meta" in op["outputs"][0] and "output_info" in op["outputs"][0]["meta"]:
                conv2d_set_end_idx = cnt
                # back trace to the conv2d
                for back_inx in range(global_index, cnt):
                    back_op = model[back_inx]
                    if back_op["type"] == "nn.conv2d":
                        groups = back_op["attrs"]["groups"]
                        input_ch = back_op["inputs"][0]["shape"][1]
                        output_ch = back_op["outputs"][0]["shape"][1]
                        if not (input_ch == groups == output_ch):  # pylint: disable=C0325
                            conv2d_set_start_idx = back_inx
                            break
                if conv2d_set_start_idx is not None:
                    # find the closest cast
                    conv2d_set_start_idx, cast_op = find_previous_link_op(model, model[conv2d_set_start_idx])
                    while cast_op["type"] != "cast":
                        conv2d_set_start_idx, cast_op = find_previous_link_op(model, model[conv2d_set_start_idx])
                    break
        if conv2d_set_end_idx is None:
            break
        # find the closest transpose conv 2d -> ... -> sum after transpose
        for cnt in range(conv2d_set_end_idx, len(model)):
            if model[cnt]["type"] == "nn.conv2d_transpose":
                transpose_conv_idx = cnt
                # find the closest sum
                transpose_conv_idx, sum_op = find_following_link_op(model, model[transpose_conv_idx])
                # case 1. reaching the sum, this means the calculation cycle of this transpose conv is finished
                while sum_op["type"] != "sum":
                    transpose_conv_idx, sum_op = find_following_link_op(model, model[transpose_conv_idx])
                break
        # no more subgraphs to reroder
        if None in [conv2d_set_start_idx, conv2d_set_end_idx, transpose_conv_idx]:
            break
        # update the global index
        # global_index = cnt
        # reoder these two parts
        if not (None in [conv2d_set_start_idx, conv2d_set_end_idx, transpose_conv_idx]):
            new_model_first = model[0:conv2d_set_start_idx]
            new_model_second = model[conv2d_set_start_idx : conv2d_set_end_idx + 1]
            new_model_thrid = model[conv2d_set_end_idx + 1 : transpose_conv_idx + 1]
            new_model_final = model[transpose_conv_idx + 1 :]

            model = []
            model += new_model_first
            model += new_model_thrid
            model += new_model_second
            global_index = len(model)
            model += new_model_final

    return model


def reorderGroupConv_TransponseConv_int8(model):
    global_index = 0
    # compact the group conv op ordering
    # compact the transpose conv

    while global_index < len(model):
        conv2d_set_start_idx = None
        conv2d_set_end_idx = None
        transpose_conv_idx = None
        for cnt in range(global_index, len(model)):
            op = model[cnt]
            # Group conv: reshape -> (... which we want to skip) -> tile -> reshape ->
            #  nn.conv2d -> reshape -> sum -> transpose ->
            # [abs -> max -> divide -> divide -> cast (int8 bp)]
            if op["type"] == "cast" and "meta" in op["outputs"][0] and "output_info" in op["outputs"][0]["meta"]:
                conv2d_set_end_idx = cnt
                # back trace to the conv2d/transpose conv2d
                for back_inx in range(global_index, cnt):
                    back_op = model[back_inx]
                    # if back_op["type"] == "nn.conv2d_transpose":
                    # raise NotImplementedError
                    if back_op["type"] == "nn.conv2d":
                        groups = back_op["attrs"]["groups"]
                        input_ch = back_op["inputs"][0]["shape"][1]
                        output_ch = back_op["outputs"][0]["shape"][1]
                        if not (input_ch == groups == output_ch):  # pylint: disable=C0325
                            conv2d_set_start_idx = back_inx
                            break
                if conv2d_set_start_idx is not None:
                    # find the closest reshape
                    conv2d_set_start_idx, reshape_op = find_previous_link_op(model, model[conv2d_set_start_idx])
                    while reshape_op["type"] != "reshape":
                        conv2d_set_start_idx, reshape_op = find_previous_link_op(model, model[conv2d_set_start_idx])
                    break
        if conv2d_set_end_idx is None:
            break
        # find the closest transpose conv 2d -> ... -> sum after transpose
        for cnt in range(conv2d_set_end_idx, len(model)):
            if model[cnt]["type"] == "nn.conv2d_transpose":
                transpose_conv_idx = cnt
                # find the closest sum
                transpose_conv_idx, sum_op = find_following_link_op(model, model[transpose_conv_idx])
                # case 1. reaching the sum, this means the calculation cycle of this transpose conv is finished
                while sum_op["type"] != "sum":
                    transpose_conv_idx, sum_op = find_following_link_op(model, model[transpose_conv_idx])
                break
        # no more subgraphs to reroder
        if None in [conv2d_set_start_idx, conv2d_set_end_idx, transpose_conv_idx]:
            break
        # update the global index
        # global_index = cnt
        # reoder these two parts
        if not (None in [conv2d_set_start_idx, conv2d_set_end_idx, transpose_conv_idx]):
            new_model_first = model[0:conv2d_set_start_idx]
            new_model_second = model[conv2d_set_start_idx : conv2d_set_end_idx + 1]
            new_model_thrid = model[conv2d_set_end_idx + 1 : transpose_conv_idx + 1]
            new_model_final = model[transpose_conv_idx + 1 :]

            model = []
            model += new_model_first
            model += new_model_thrid
            model += new_model_second
            global_index = len(model)
            model += new_model_final

    return model
