# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -O3  -mavx2 -mfma -pthread

# Executable and source files
TARGET = test_example_eval test_ops

LIB_DIR = /NFS/home/wmchen/workspace/tinyengine/experimental/matmul_optimization/src
LIB_SRC = $(wildcard $(LIB_DIR)/lib/*.cc)
INCLUDE_DIRS = -I$(LIB_DIR)

$(info $(LIB_SRC))

SRC = OPTForCausalLM.cc Int8OPTDecoder.cc utils.cc
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


# Clean up
clean:
	rm -f $(TARGET)