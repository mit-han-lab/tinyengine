# example/test_operator_concatenation.py

import sys
import os
import numpy as np

# Add the path to the tinyengine directory
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..', 'tinyengine')))

# Add the path to the code_generator directory
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'code_generator')))

from converters.tflite_parser.concatenation import parse_concatenation
from operators.concatenation import ConcatenationOperator
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
        return [0, 1]
    
    def OutputsAsNumpy(self):
        return [2]

    def BuiltinOptions(self):
        class Options:
            def Axis(self):
                return 1  # Example axis value
        return Options()

class MockSubgraph:
    def __init__(self):
        self.tensors = [
            MockTensor(0, TensorType.INT8, [1, 2, 3]), 
            MockTensor(1, TensorType.INT8, [1, 3, 3]), 
            MockTensor(2, TensorType.INT8, [1, 5, 3])
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

# Parse the concatenation operator
concatenation_op = parse_concatenation(op, model)

# Output the result
print("Parsed Concatenation Operator Params:", concatenation_op.params)
