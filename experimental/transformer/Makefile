# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -mavx2 -mfma -pthread -g

# Executable and source files
TARGET = test_example_eval test_ops test_Int8OPTAttention test_Int8OPTDecoderLayer

LIB_DIR = /home/wmchen/workspace/tinyengine/experimental/matmul_optimization/src
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
test_example_eval: $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_example_eval tests/test_example_eval.cc $(SRC)

test_ops: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_ops tests/test_ops.cc $(SRC)

test_Int8OPTAttention: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_Int8OPTAttention tests/test_Int8OPTAttention.cc $(SRC)

test_Int8OPTDecoderLayer: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_Int8OPTDecoderLayer tests/test_Int8OPTDecoderLayer.cc $(SRC)

# Clean up
clean:
	rm -f $(TARGET)