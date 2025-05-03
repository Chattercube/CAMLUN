# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Wimplicit-fallthrough=3
LDFLAGS = -Llib
LIBS = -lcamlun

# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
LIB_DIR = lib
INCLUDE_DIR = include

# Find all source files recursively
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Library files
STATIC_LIB = $(LIB_DIR)/libcamlun.a
SHARED_LIB = $(LIB_DIR)/libcamlun.so
DEBUG_STATIC_LIB = $(LIB_DIR)/libcamlun-debug.a
DEBUG_SHARED_LIB = $(LIB_DIR)/libcamlun-debug.so

# Test files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_SRCS))
DEBUG_TEST_BINS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%-debug, $(TEST_SRCS))

# Default target
all: release

# Release build
release: CFLAGS += -O2
release: $(STATIC_LIB) $(SHARED_LIB)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(DEBUG_STATIC_LIB) $(DEBUG_SHARED_LIB) debug_tests

# Build static library
$(STATIC_LIB): $(OBJS)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Build shared library
$(SHARED_LIB): $(OBJS)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $^

# Build debug static library
$(DEBUG_STATIC_LIB): $(OBJS)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Build debug shared library
$(DEBUG_SHARED_LIB): $(OBJS)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $^

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Build tests
tests: $(TEST_BINS)

debug_tests: $(DEBUG_TEST_BINS)

$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(STATIC_LIB)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LIBS) -Wl,-rpath=lib

$(BUILD_DIR)/%-debug: $(TEST_DIR)/%.c $(DEBUG_STATIC_LIB)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -lcamlun-debug -Wl,-rpath=lib

# Run tests
test: tests
	@for test in $(TEST_BINS); do \
		echo "Running $$test..."; \
		$$test || exit 1; \
	done
	@echo "All tests passed!"

debug_test: debug_tests
	@for test in $(DEBUG_TEST_BINS); do \
		echo "Running $$test..."; \
		$$test || exit 1; \
	done
	@echo "All debug tests passed!"

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(TEST_BINS) $(DEBUG_TEST_BINS)

# Phony targets
.PHONY: all release debug tests test debug_tests debug_test clean