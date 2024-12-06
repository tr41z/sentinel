# Tauri App
run-tauri:
	@cd src-tauri && cargo test
	@npm run tauri dev

test-tauri:
	@cd src-tauri && cargo test

fmt:
	@cd src-tauri && cargo fmt
	@cd src-tauri && cargo test

clippy:
	@cd src-tauri && cargo clippy

# Python ML Module
run-ml:
	@ml-mod/_venv/bin/python3 ml/main.py

build-exec:
	@cd ml-mod && pyinstaller --onefile --add-data "models/ml/recondet_model.joblib:models/ml" --hidden-import sklearn --hidden-import sklearn.ensemble._forest --hidden-import numpy --hidden-import scipy main.py
	@rm -rf ml-mod/build ml-mod/dist ml-mod/main.spec

# C++ Sniffer
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

SNIFFER_DIR = sniffer-mod
SRC_DIR = src
HEADER_DIR = include
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = sniffer 

# Default target to build
all: $(EXEC)

# Change directory and link objects to create the executable
$(EXEC):
	@cd $(SNIFFER_DIR)/$(SRC_DIR) && $(CXX) $(SOURCES) -I $(SNIFFER_DIR)/$(SRC_DIR)/$(HEADER_DIR) -o ../$(EXEC)

# Compile the source files into object files in sniffer directory
%.o: %.cpp
	@cd $(SNIFFER_DIR)/$(SRC_DIR) && $(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable in sniffer directory
clean:
	@cd $(SNIFFER_DIR) && rm -f $(OBJECTS) $(EXEC)

# Run the program from sniffer directory
run-sniffer: $(EXEC)
	@cd $(SNIFFER_DIR) && ./$(EXEC)
