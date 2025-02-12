# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -pthread

# Directories
SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests
TEST_OBJ_DIR = $(OBJ_DIR)/tests

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Test files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(TEST_OBJ_DIR)/%.o)
TEST_BINS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(TEST_OBJ_DIR)/%)

# Target executable
TARGET = budgetWC

# Default target
all: $(OBJ_DIR) $(TARGET)

# Create object directories
$(OBJ_DIR) $(TEST_OBJ_DIR):
	mkdir -p $@

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Compile test files
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link test executables
$(TEST_OBJ_DIR)/%: $(TEST_OBJ_DIR)/%.o $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	$(CC) $^ $(LDFLAGS) -o $@

# Run all tests
.PHONY: test
test: $(TEST_OBJ_DIR) $(TEST_BINS)
	@echo "=== Running Tests ==="
	@for test in $(TEST_BINS); do \
		echo "\nRunning $$test:"; \
		$$test; \
	done
	@echo "\n=== Integration Tests ==="
	@echo "Testing basic file counting:"
	@./$(TARGET) -l -w -c $(TEST_DIR)/test_files/sample.txt
	@echo "\nTesting help flag:"
	@./$(TARGET) --help
	@echo "\nTesting multiple files:"
	@./$(TARGET) -l -w -c $(SRC_DIR)/*.c
	@echo "\n=== All Tests Completed ==="

# Run tests with memory check
.PHONY: test-memcheck
test-memcheck: $(TEST_OBJ_DIR) $(TEST_BINS)
	@echo "=== Running Tests with Valgrind ==="
	@for test in $(TEST_BINS); do \
		echo "\nChecking $$test:"; \
		valgrind --leak-check=full --error-exitcode=1 $$test; \
	done

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Phony targets
.PHONY: all clean test test-memcheck
