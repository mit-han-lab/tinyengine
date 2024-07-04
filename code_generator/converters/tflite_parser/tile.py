# code_generator/converters/tflite_parser/tile.py

from code_generator.operators.tile import tile
from .utils import get_input_tensors, get_output_tensors, getTensorTypeStr
import tflite

def parse_tile(op, model):
    input_tensors = get_input_tensors(op, model)
    output_tensors = get_output_tensors(op, model)
    
    multiples_tensor = input_tensors[1].tensor
    buffer_idx = multiples_tensor.Buffer()
    buffer = model.Buffers(buffer_idx)
    
    # Extract the multiples directly from the buffer
    multiples = buffer.DataAsNumpy()
    
    # if multiples is a scalar, reps_size is 1
    # int has no shape, so we need to check if it is a scalar
    # check variable is integer
    reps_size = 1
    if isinstance(multiples, int):
        reps_size = 1
    else:
        reps_size = len(multiples)

    input_idx = input_tensors[0].tensor_idx
    output_idx = output_tensors[0].tensor_idx
    input_dtype = getTensorTypeStr(input_tensors[0].tensor.Type())
    output_dtype = getTensorTypeStr(output_tensors[0].tensor.Type())
    input_shape = input_tensors[0].tensor.ShapeAsNumpy().tolist()
    output_shape = output_tensors[0].tensor.ShapeAsNumpy().tolist()
    
    params = {
        "op": "TILE",
        "input_idx": input_idx,
        "output_idx": output_idx,
        "input_dtype": input_dtype,
        "output_dtype": output_dtype,
        "input_dim": len(input_shape),
        "input_h": input_shape[0],
        "input_w": input_shape[1],
        "input_c": input_shape[2],
        "output_dim": len(output_shape),
        "output_h": output_shape[0],
        "output_w": output_shape[1],
        "output_c": output_shape[2],
        "reps_size": reps_size,
        "reps": multiples,
    }
    
    return tile(params)
