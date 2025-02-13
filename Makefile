
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -pthread


SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests
TEST_OBJ_DIR = $(OBJ_DIR)/tests


SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(TEST_OBJ_DIR)/%.o)
TEST_BINS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(TEST_OBJ_DIR)/%)


TARGET = wordcount-thws


all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR) $(TEST_OBJ_DIR):
	mkdir -p $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)


$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%: $(TEST_OBJ_DIR)/%.o $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	$(CC) $^ $(LDFLAGS) -o $@


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


clean:
	rm -rf $(OBJ_DIR) $(TARGET)


.PHONY: all clean test
