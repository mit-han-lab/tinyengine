# code_generator/converters/tflite_parser/reduce_prod.py
import numpy as np
from code_generator.operators.reduce_prod import ReduceProdOperator
from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr
from code_generator.tflite.ReducerOptions import ReducerOptions

def parse_reduce_prod(op, model):
    subgraph = model.Subgraphs(0)
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)

    # Log details about the tensors
    print(f"Number of input tensors: {len(input_tensors)}")
    for i, tensor in enumerate(input_tensors):
        print(f"Input tensor {i} details: Shape - {tensor.tensor.ShapeAsNumpy().tolist()}, Type - {getTensorTypeStr(tensor.tensor.Type())}")

    assert len(output_tensors) == 1, "REDUCE_PROD operation requires 1 output tensor"

    output_tensor = output_tensors[0]

    input_shapes = [tensor.tensor.ShapeAsNumpy().tolist() for tensor in input_tensors]
    output_shape = output_tensor.tensor.ShapeAsNumpy().tolist()

    input_idxs = [tensor.tensor_idx for tensor in input_tensors]
    output_idx = output_tensor.tensor_idx

    input_names = [subgraph.Tensors(idx).Name().decode('utf-8') for idx in input_idxs]
    output_name = subgraph.Tensors(output_idx).Name().decode('utf-8')

    input_dtypes = [getTensorTypeStr(tensor.tensor.Type()) for tensor in input_tensors]
    output_dtype = getTensorTypeStr(output_tensor.tensor.Type())

    input_sizes = [np.prod(shape) for shape in input_shapes]
    output_size = np.prod(output_shape)

    # Handle quantization parameters for each input tensor
    input_scales = [tensor.qnn_params["scale"] if tensor.qnn_params else None for tensor in input_tensors]
    input_zero_points = [tensor.qnn_params["zero_point"] if tensor.qnn_params else None for tensor in input_tensors]
    output_scale = output_tensor.qnn_params["scale"] if output_tensor.qnn_params else None
    output_zero_point = output_tensor.qnn_params["zero_point"] if output_tensor.qnn_params else None

    # Fetch the reduction axis from the options
    options = ReducerOptions()
    options.Init(op.BuiltinOptions().Bytes, op.BuiltinOptions().Pos)
    reduction_axis = options.Axis() if hasattr(options, 'Axis') else []

    # Assign params
    params = {
        "op": "REDUCE_PROD",
        "input_idxs": input_idxs,
        "output_idx": output_idx,
        "input_sizes": input_sizes,
        "output_size": output_size,
        "input_dtypes": input_dtypes,
        "output_dtype": output_dtype,
        "input_zero_points": input_zero_points,
        "output_zero_point": output_zero_point,
        "input_scales": input_scales,
        "output_scale": output_scale,
        "input_names": input_names,
        "output_name": output_name,
        "reduction_axis": reduction_axis
    }

    return ReduceProdOperator(params)
