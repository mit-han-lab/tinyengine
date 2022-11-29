# QAS related

__all__ = ["get_QAS", "get_effective_scalename_with_input_key"]


def get_QAS(k, scale_params, effective_scale):
    # perform QAS training
    if k.endswith("_weight"):
        xk = "_".join(
            k.split("_")[:-1]
            + [
                "x_scale",
            ]
        )
        yk = "_".join(
            k.split("_")[:-1]
            + [
                "y_scale",
            ]
        )
        _ = "_".join(
            k.split("_")[:-1]
            + [
                "scale",
            ]
        )
        x_scale = scale_params[xk]
        y_scale = scale_params[yk]
        w_scale = effective_scale * y_scale / x_scale
        return w_scale.reshape(-1, 1, 1, 1) ** 2
    elif k.endswith("_bias"):
        xk = "_".join(
            k.split("_")[:-1]
            + [
                "x_scale",
            ]
        )
        yk = "_".join(
            k.split("_")[:-1]
            + [
                "y_scale",
            ]
        )
        _ = "_".join(
            k.split("_")[:-1]
            + [
                "scale",
            ]
        )
        x_scale = scale_params[xk]
        y_scale = scale_params[yk]
        return (effective_scale * y_scale) ** 2
    else:
        raise NotImplementedError


def get_effective_scalename_with_input_key(k, model):
    for op in model:
        for inp in op["inputs"]:
            if inp["name"] == k:
                return op["inputs"][5]["name"]
