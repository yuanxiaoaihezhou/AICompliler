# SYSY Compiler Makefile
# Target platform: x86_64 Ubuntu 22.04

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = 

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin
TEST_DIR = tests
EXAMPLES_DIR = examples

# Source files
LEXER_SRCS = $(SRC_DIR)/lexer/token.cpp $(SRC_DIR)/lexer/lexer.cpp
PARSER_SRCS = $(SRC_DIR)/parser/ast.cpp $(SRC_DIR)/parser/parser.cpp
IR_SRCS = $(SRC_DIR)/ir/ir.cpp $(SRC_DIR)/ir/ir_generator.cpp
CODEGEN_SRCS = $(SRC_DIR)/codegen/codegen.cpp
MAIN_SRC = $(SRC_DIR)/main.cpp

ALL_SRCS = $(LEXER_SRCS) $(PARSER_SRCS) $(IR_SRCS) $(CODEGEN_SRCS) $(MAIN_SRC)

# Object files
OBJS = $(ALL_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Target executable
TARGET = $(BIN_DIR)/sysyc

# Test files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_BINS = $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(BIN_DIR)/test_%)

.PHONY: all clean test examples install

all: $(TARGET)

# Create necessary directories
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $(BUILD_DIR)/lexer $(BUILD_DIR)/parser $(BUILD_DIR)/ir $(BUILD_DIR)/codegen
	mkdir -p $(BIN_DIR)

# Link executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build tests
test: $(TARGET) $(TEST_BINS)
	@echo "Running tests..."
	@for test in $(TEST_BINS); do \
		echo "Running $$test..."; \
		$$test || exit 1; \
	done
	@echo "All tests passed!"

$(BIN_DIR)/test_%: $(TEST_DIR)/%.cpp $(filter-out $(BUILD_DIR)/main.o,$(OBJS)) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Build and run examples
examples: $(TARGET)
	@echo "Building examples..."
	@for example in $(EXAMPLES_DIR)/*.sy; do \
		if [ -f "$$example" ]; then \
			echo "Compiling $$example..."; \
			$(TARGET) $$example -o $${example%.sy}.s; \
			as $${example%.sy}.s -o $${example%.sy}.o; \
			ld $${example%.sy}.o -o $${example%.sy}; \
		fi \
	done

# Install compiler
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	rm -f $(EXAMPLES_DIR)/*.s $(EXAMPLES_DIR)/*.o $(EXAMPLES_DIR)/*.out

# Display help
help:
	@echo "SYSY Compiler Makefile"
	@echo "======================"
	@echo "Targets:"
	@echo "  all       - Build the compiler (default)"
	@echo "  test      - Build and run tests"
	@echo "  examples  - Build and run example programs"
	@echo "  clean     - Remove build artifacts"
	@echo "  install   - Install compiler to /usr/local/bin/"
	@echo "  help      - Display this help message"
