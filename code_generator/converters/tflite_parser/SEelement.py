import numpy as np

from code_generator.operators import se_element_mult
from code_generator.tflite import Model

from .utils import get_input_tensors, get_output_tensors, getLayerMultiplierShift


#         -> MEAN -> MEAN -> PWCONV -> PWCONV -> | ADD -> MUL ->     |
#  DWCONV                                        |            -> MUL |
#                                                |   SEelementmult   |
def parse_SEelement(three_op_sequence, model: Model.Model, layer):
    add_op = three_op_sequence[0]
    _ = three_op_sequence[1]  # mul1_op
    mul2_op = three_op_sequence[2]

    add_op_input_tensors = get_input_tensors(add_op, model)

    assert len(add_op_input_tensors) == 2, "Unexpected number of tensors."
    # Expect another tensor to be a scaler
    if len(add_op_input_tensors[0].tensor.ShapeAsNumpy()) > len(add_op_input_tensors[1].tensor.ShapeAsNumpy()):
        pwconv_output_tensor = add_op_input_tensors[0]
    elif len(add_op_input_tensors[0].tensor.ShapeAsNumpy()) < len(add_op_input_tensors[1].tensor.ShapeAsNumpy()):
        pwconv_output_tensor = add_op_input_tensors[1]
    else:
        raise NotImplementedError("unexpected tensor shapes for SE block")

    assert (
        layer[-1].params["output_idx"] == pwconv_output_tensor.tensor_idx
    ), "Cannot link to the PWCONV ooutput tensor."

    mul2_op_input_tensors = get_input_tensors(mul2_op, model)
    dwcov_input_tensor = None
    # Find out the tensor link to DWCONV
    for each_layer in layer:
        for tensor in mul2_op_input_tensors:
            if str(each_layer.params["output_idx"]) == str(tensor.tensor_idx):
                dwcov_input_tensor = tensor

        if dwcov_input_tensor:
            break

    assert dwcov_input_tensor, "Cannot link to the dw conv layer"

    mul2_op_output_tensor = get_output_tensors(mul2_op, model)[0]

    _, input_h, input_w, input_c = dwcov_input_tensor.tensor.ShapeAsNumpy()
    _, input2_h, input2_w, input2_c = pwconv_output_tensor.tensor.ShapeAsNumpy()
    _, output_h, output_w, output_c = mul2_op_output_tensor.tensor.ShapeAsNumpy()

    input_zero_point = dwcov_input_tensor.qnn_params["zero_point"]
    input2_zero_point = pwconv_output_tensor.qnn_params["zero_point"]
    output_zero_point = mul2_op_output_tensor.qnn_params["zero_point"]

    input_scale = dwcov_input_tensor.qnn_params["scale"]
    input2_scale = pwconv_output_tensor.qnn_params["scale"]
    output_scale = mul2_op_output_tensor.qnn_params["scale"]
    effective_scale = np.double(input_scale) * np.double(input2_scale) / np.double(output_scale)
    multiplier, shift = getLayerMultiplierShift(effective_scale)

    params = {
        # op related
        "op": "SE_ELEMENT_MULT_2D",
        "input_idx": dwcov_input_tensor.tensor_idx,
        "input2_idx": pwconv_output_tensor.tensor_idx,
        "output_idx": mul2_op_output_tensor.tensor_idx,
        # tensor related
        "input_dim": 3,
        "input_h": input_h,
        "input_w": input_w,
        "input_c": input_c,
        "input2_dim": 3,
        "input2_h": input2_h,
        "input2_w": input2_w,
        "input2_c": input2_c,
        "output_dim": 3,
        "output_h": output_h,
        "output_w": output_w,
        "output_c": output_c,
        "input_dtype": "int8",
        "input2_dtype": "int8",
        "output_dtype": "int8",
        # quantization related
        "input_zero_point": input_zero_point,
        "input2_zero_point": input2_zero_point,
        "output_zero_point": output_zero_point,
        "output_multiplier": multiplier,
        "output_effective_scale": effective_scale,
        "output_shift": shift,
    }

    op = se_element_mult.SEelementmult(params)

    return op
