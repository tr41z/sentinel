# Tauri App
run-tauri:
	@cd src-tauri && cargo test
	@npm run tauri dev

test-tauri:
	@cd src-tauri && cargo test

fmt:
	@cd src-tauri && cargo fmt && cargo test

clippy:
	@cd src-tauri && cargo clippy

# Python ML Module
run-ml:
	@ml-mod/_venv/bin/python3 ml/main.py

build-exec:
	@cd ml-mod && pyinstaller --onefile --add-data "models/ml/recondet_model.joblib:models/ml" --hidden-import sklearn --hidden-import sklearn.ensemble._forest --hidden-import numpy --hidden-import scipy main.py
	@rm -rf ml-mod/build ml-mod/dist ml-mod/main.spec

# C Sniffer
# Compiler and flags
CC = gcc
CXX = g++  # Compiler for C++ files
CFLAGS = -std=c11 -Wall -g
CXXFLAGS = -std=c++11 -Wall -g  # C++ flags
LDFLAGS = -lpthread -lcunit -lsqlite3

# Platform-specific flags
ifeq ($(OS), Windows_NT)
    # Windows-specific flags for MinGW and npcap
    CFLAGS += -I$(INCLUDE)
    CXXFLAGS += -I$(INCLUDE)
    LDFLAGS += -L$(LIB) -lwpcap
else ifeq ($(shell uname), Darwin)
    # macOS-specific flags
    CFLAGS += -I/opt/homebrew/include -I/opt/homebrew/include/CUnit
    CXXFLAGS += -I/opt/homebrew/include -I/opt/homebrew/include/CUnit
    LDFLAGS += -L/opt/homebrew/lib -lpcap
else
    # Linux-specific flags
    CFLAGS += -I/usr/include
    CXXFLAGS += -I/usr/include
    LDFLAGS += -L/usr/lib -lpcap
endif

# Directories
SNIFFER_DIR = sniffer-mod
SRC_DIR = src
HEADER_DIR = include
OBJ_DIR = $(SNIFFER_DIR)/_obj
TESTS_DIR = $(SNIFFER_DIR)/$(SRC_DIR)/tests

# Sniffer source and object files (including C++ .cpp files)
SOURCES = $(wildcard $(SNIFFER_DIR)/$(SRC_DIR)/*.c) $(wildcard $(SNIFFER_DIR)/$(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SNIFFER_DIR)/$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJECTS := $(OBJECTS:$(SNIFFER_DIR)/$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXEC = sniffer

# Test source and object files (including C++ .cpp files)
TEST_SOURCES = $(wildcard $(TESTS_DIR)/*.c) $(wildcard $(TESTS_DIR)/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTS_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_OBJECTS := $(TEST_OBJECTS:$(TESTS_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_EXEC = test

# Default target to build sniffer and run tests
all: $(EXEC) $(TEST_EXEC)

# Create the _obj directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compile sniffer executable
$(EXEC): $(OBJECTS) $(OBJ_DIR)
	$(CXX) $(OBJECTS) $(CXXFLAGS) $(LDFLAGS) -o $(SNIFFER_DIR)/$(EXEC)

# Compile test executable
$(TEST_EXEC): $(TEST_OBJECTS) $(OBJ_DIR)/packet.o $(OBJ_DIR)/ip.o
	$(CXX) $(TEST_OBJECTS) $(OBJ_DIR)/db.o $(OBJ_DIR)/sniffer.o $(OBJ_DIR)/packet.o $(OBJ_DIR)/ip.o $(CXXFLAGS) $(LDFLAGS) -o $(SNIFFER_DIR)/$(TEST_EXEC)

# Compile sniffer source files into object files (handle both C and C++)
$(OBJ_DIR)/%.o: $(SNIFFER_DIR)/$(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SNIFFER_DIR)/$(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test source files into object files (handle both C and C++)
$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files, executables, and the _obj directory
clean:
	rm -rf $(OBJ_DIR) $(SNIFFER_DIR)/$(SRC_DIR)/*.o $(SNIFFER_DIR)/$(TESTS_DIR)/*.o $(SNIFFER_DIR)/$(EXEC) $(SNIFFER_DIR)/$(TEST_EXEC)
	rm -rf ml-mod/build ml-mod/dist ml-mod/main.spec
	rm -rf src-tauri/target

# Run the sniffer program
run-sniffer: $(EXEC)
	@cd $(SNIFFER_DIR) && ./$(EXEC)

# Run tests
run-tests: $(TEST_EXEC)
	@cd $(SNIFFER_DIR) && ./$(TEST_EXEC)

# Check for memory leaks
mem-safe:
	@cd $(SNIFFER_DIR) && leaks -atExit -- ./sniffer

# Check for memory leaks for tests
mem-safe-tests:
	@cd $(SNIFFER_DIR) && leaks -atExit -- ./test

