# example/test_operator_tile.py

import sys
import os
import numpy as np

# Add the path to the tinyengine directory
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..', 'tinyengine')))

# Add the path to the code_generator directory
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'code_generator')))

from converters.tflite_parser.tile import parse_tile
from operators.tile import tile
from tflite.TensorType import TensorType

class MockTensor:
    def __init__(self, tensor_idx, dtype, size):
        self.tensor_idx = tensor_idx
        self.dtype = dtype
        self.size = size

    def Buffer(self):
        return 0

    def Quantization(self):
        return None

    def Type(self):
        return self.dtype

    def ShapeAsNumpy(self):
        return np.array(self.size)

class MockOperator:
    def InputsAsNumpy(self):
        return [0]
    
    def OutputsAsNumpy(self):
        return [1]

    def BuiltinOptions(self):
        class Options:
            def MultiplesAsNumpy(self):
                return np.array([1, 2, 3])

        return Options()

class MockSubgraph:
    def __init__(self):
        self.tensors = [MockTensor(0, TensorType.INT8, [1, 2, 3]), MockTensor(1, TensorType.INT8, [1, 4, 9])]

    def Tensors(self, index):
        return self.tensors[index]

class MockBuffer:
    def __init__(self):
        self.data = [0]

class MockModel:
    def __init__(self):
        self.subgraphs = [MockSubgraph()]
        self.buffers = [MockBuffer()]

    def Subgraphs(self, index):
        return self.subgraphs[index]

    def Buffers(self, index):
        return self.buffers[index]

# Mock model and operation
model = MockModel()
op = MockOperator()

# Parse the tile operator
tile_op = parse_tile(op, model)

# Output the result
print("Parsed Tile Operator Params:", tile_op.params)
