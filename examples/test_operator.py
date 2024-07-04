
# test_operator.py

import sys
import os

# Add the path to the tinyengine directory
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..', 'tinyengine')))

# Add the path to the code_generator directory
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'code_generator')))

# test_operator.py
from converters.tflite_parser.gather import parse_gather
from operators.gather import GatherOperator
from converters.tflite_parser.utils import getTensorTypeStr


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
        return self.size

class MockOperator:
    def InputsAsNumpy(self):
        return [0]
    
    def OutputsAsNumpy(self):
        return [1]

class MockSubgraph:
    def __init__(self):
        self.tensors = [MockTensor(0, "int32", [5]), MockTensor(1, "int32", [3])]

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

def get_input_tensors(op, model):
    return [MockTensor(0, "int32", [5])]

def get_output_tensors(op, model):
    return [MockTensor(1, "int32", [3])]

# Define the input data
input_data = [1, 2, 3, 4, 5]
indices = [0, 2, 4]

# Mock model and operation
model = MockModel()
op = MockOperator()

# Parse the gather operator
gather_op = parse_gather(op, model)

# Simulate the gather operation
def gather(input_data, indices):
    return [input_data[i] for i in indices]

# Run the gather operation
output_data = gather(input_data, indices)

# Output the result
print("Input Data:", input_data)
print("Indices:", indices)
print("Output Data:", output_data)
