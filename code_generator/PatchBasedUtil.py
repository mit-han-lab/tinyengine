# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   PatchBasedUtil.py
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

def getPatchParams(layers, split_idx, n_patch):
    patch_params = {}

    feat_stride = 8
    patch_params["n_patch"] = n_patch
    patch_params["layer_cnt"] = split_idx

    resolution = max(layers[0].get_layer_info()["input_h"], layers[0].get_layer_info()["input_w"])
    layer_cnt = layers[patch_params["layer_cnt"]].get_layer_info()
    out_shape = max(layer_cnt["input_h"], layer_cnt["input_w"])
    feat_stride = resolution // out_shape
    grain_size = out_shape // n_patch

    patch_params["single_rf"] = compute_receptive_field(layers, patch_params["layer_cnt"], 1)
    patch_params["output_c"] = layer_cnt["input_c"]
    patch_params["output_h"] = layer_cnt["output_h"]
    patch_params["output_w"] = layer_cnt["output_w"]
    patch_params["grain_rf"] = compute_receptive_field(layers, patch_params["layer_cnt"], grain_size)
    patch_params["grain_rf_height"] = compute_receptive_field(
        layers, patch_params["layer_cnt"], layer_cnt["input_h"] // n_patch
    )
    print("receptive field: single {} all {}".format(patch_params["single_rf"], patch_params["grain_rf"]))

    # now generate the padding for each layer (two side)
    patch_params["pad_l"] = patch_params["single_rf"] // 2
    patch_params["pad_r"] = max(
        0,
        patch_params["grain_rf"]
        + feat_stride * grain_size * (n_patch - 1)
        - patch_params["single_rf"] // 2
        - resolution,
    )

    return patch_params


def get_recompute_layer(model, split_idx):
    layer_cnt = 1  # first conv

    for i in range(split_idx):
        block = model["blocks"][i]
        if "pointwise1" in block and block["pointwise1"] is not None:
            layer_cnt += 1
        if "depthwise" in block and block["depthwise"] is not None:
            layer_cnt += 1
        if "pointwise2" in block and block["pointwise2"] is not None:
            layer_cnt += 1

    return layer_cnt


def compute_receptive_field(layers, layer_cnt, grain=1):
    for i in range(layer_cnt):
        op = layers[(layer_cnt - 1) - i]  # trace in a backward manner
        layer_info = op.get_layer_info()
        if layer_info["op"] == "CONV_2D" or layer_info["op"] == "DEPTHWISE_CONV_2D":  # receptive field will increase
            stride = layer_info["stride_h"]
            kernel_size = max(layer_info["kernel_h"], layer_info["kernel_w"])
            if stride in [1, 2]:
                if stride == 1:
                    grain += kernel_size - 1
                else:
                    grain = (grain - 1) * 2 + kernel_size
        else:
            pass

    return grain
