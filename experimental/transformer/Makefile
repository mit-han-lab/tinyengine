# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -mavx2 -mfma -pthread -O3

# Executable and source files
TARGET = test_ops test_Int8OPTAttention test_Int8OPTDecoderLayer test_Int8OPTDecoder test_OPTForCausalLM profile_OPTForCausalLM test_ops_layer5_1.3B

LIB_DIR = ../matmul_optimization/src
LIB_SRC = $(wildcard $(LIB_DIR)/lib/*.cc)
INCLUDE_DIRS = -I$(LIB_DIR) -I./include

$(info $(LIB_SRC))

SRC_DIR = src
SRC = $(wildcard src/*.cc)
OPS =  $(wildcard src/ops/*.cc)
SRC += $(OPS)
SRC += $(LIB_SRC)

# Default target
all: $(TARGET)

# Linking
test_ops:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_ops tests/test_ops.cc $(SRC)

test_Int8OPTAttention:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_Int8OPTAttention tests/test_Int8OPTAttention.cc $(SRC)

test_Int8OPTDecoderLayer:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_Int8OPTDecoderLayer tests/test_Int8OPTDecoderLayer.cc $(SRC)

test_Int8OPTDecoder:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_Int8OPTDecoder tests/test_Int8OPTDecoder.cc $(SRC)

test_OPTForCausalLM:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_OPTForCausalLM tests/test_OPTForCausalLM.cc $(SRC)

test_ops_layer5_1.3B:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_ops_layer5_1.3B tests/test_ops_layer5_1.3B.cc $(SRC)

profile_OPTForCausalLM:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -D PROFILER -o profile_OPTForCausalLM tests/test_OPTForCausalLM.cc $(SRC)


# Clean up
clean:
	rm -f $(TARGET)
