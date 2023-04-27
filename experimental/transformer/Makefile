# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -mavx2 -mfma -pthread -g

# Executable and source files
TARGET = test_example_eval test_ops test_Int8OPTAttention

LIB_DIR = /home/wmchen/workspace/tinyengine/experimental/matmul_optimization/src
LIB_SRC = $(wildcard $(LIB_DIR)/lib/*.cc)
INCLUDE_DIRS = -I$(LIB_DIR)

$(info $(LIB_SRC))

SRC = OPTForCausalLM.cc Int8OPTDecoder.cc Int8OPTAttention.cc utils.cc
OPS =  $(wildcard ops/*.cc)
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

# Clean up
clean:
	rm -f $(TARGET)