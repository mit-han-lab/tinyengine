# example/test_operator_pack.py

import sys
import os
import numpy as np

# Add the path to the tinyengine directory
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..', 'tinyengine')))

# Add the path to the code_generator directory
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'code_generator')))

from converters.tflite_parser.pack import parse_pack
from operators.pack import PackOperator
from tflite.TensorType import TensorType

class MockTensor:
    def __init__(self, tensor_idx, dtype, size, buffer_data=None):
        self.tensor_idx = tensor_idx
        self.dtype = dtype
        self.size = size
        self.buffer_data = buffer_data

    def Buffer(self):
        return 0

    def Quantization(self):
        return None

    def Type(self):
        return self.dtype

    def ShapeAsNumpy(self):
        return np.array(self.size)

    def DataAsNumpy(self):
        if self.buffer_data is not None:
            return np.array(self.buffer_data)
        return np.zeros(self.size)

class MockOperator:
    def InputsAsNumpy(self):
        return [0, 1, 2, 3]
    
    def OutputsAsNumpy(self):
        return [4]

    def BuiltinOptions(self):
        class Options:
            def Axis(self):
                return 1

        return Options()

class MockSubgraph:
    def __init__(self):
        self.tensors = [
            MockTensor(0, TensorType.INT8, [1, 2, 3, 3]),  # Ensure 4D tensor
            MockTensor(1, TensorType.INT8, [1, 2, 3, 3]), 
            MockTensor(2, TensorType.INT8, [1, 2, 3, 3]),
            MockTensor(3, TensorType.INT8, [1, 2, 3, 3]),
            MockTensor(4, TensorType.INT8, [4, 2, 3, 3])  # Ensure 4D tensor
        ]

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

# Parse the pack operator
pack_op = parse_pack(op, model)

# Output the result
print("Parsed Pack Operator Params:", pack_op.params)
