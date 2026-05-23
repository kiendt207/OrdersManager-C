CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
TEST_DIR = tests

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = orders_manager

# Objects needed for testing (excludes main.o to avoid duplicate main function)
TEST_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c, %, $(TEST_SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Target to compile unit tests
test: $(OBJS) $(TEST_TARGETS)
	@echo "All unit tests compiled successfully!"

# Compile specific unit test linking with internal code blocks
%: $(TEST_DIR)/%.c $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TARGET).exe $(TEST_TARGETS) *.exe

.PHONY: all clean test
