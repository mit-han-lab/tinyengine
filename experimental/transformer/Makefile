# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -O3  -mavx2 -mfma -pthread

# Executable and source files
TARGET = example_evaluate

LIB_DIR = /NFS/home/wmchen/workspace/tinyengine/experimental/matmul_optimization/src
LIB_SRC = $(wildcard $(LIB_DIR)/lib/*.cc)
INCLUDE_DIRS = -I$(LIB_DIR)

$(info $(LIB_SRC))

SRC = main.cc OPTForCausalLM.cc Int8OPTDecoder.cc utils.cc
TEST_SRC = tests/test_main.cc OPTForCausalLM.cc Int8OPTDecoder.cc utils.cc
OPS =  ops/LayerNormQ.cc ops/linear.cc ops/W8A8B8O8LinearReLU.cc
SRC += $(OPS)
SRC += $(LIB_SRC)
TEST_SRC += $(OPS)
TEST_SRC += $(LIB_SRC)

# Default target
all: $(TARGET) test_main

# Linking
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o $(TARGET) $(SRC)

test_main: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o test_main $(TEST_SRC)


# Clean up
clean:
	rm -f $(TARGET) test_main