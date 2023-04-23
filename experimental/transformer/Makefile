# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -O3  -mavx2 -mfma -pthread

# Executable and source files
TARGET = example_evaluate
SRC = main.cc OPTForCausalLM.cc Int8OPTDecoder.cc lib/matmul_imp.cc lib/utils.cc lib/matmul_int8.cc lib/matmul_avx_int8.cc utils.cc
TEST_SRC = tests/test_main.cc OPTForCausalLM.cc Int8OPTDecoder.cc lib/matmul_imp.cc lib/utils.cc lib/matmul_int8.cc lib/matmul_avx_int8.cc utils.cc
OPS =  ops/LayerNormQ.cc ops/linear.cc
SRC += $(OPS)
TEST_SRC += $(OPS)

# Default target
all: $(TARGET) test_main

# Linking
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

test_main: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o test_main $(TEST_SRC)


# Clean up
clean:
	rm -f $(TARGET) test_main