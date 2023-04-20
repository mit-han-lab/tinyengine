# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -O3

# Executable and source files
TARGET = example_evaluate
SRC = main.cc OPTForCausalLM.cc Int8OPTDecoder.cc

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean up
clean:
	rm -f $(TARGET)