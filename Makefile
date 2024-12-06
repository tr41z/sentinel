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

# C Sniffer
CC = gcc
CFLAGS = -std=c11 -Wall -g
LDFLAGS = -lpcap -lpthread # Link the pcap and lpthread library

SNIFFER_DIR = sniffer-mod
SRC_DIR = src
HEADER_DIR = include
SOURCES = $(wildcard $(SNIFFER_DIR)/$(SRC_DIR)/*.c) # Automatically find all .c files
OBJECTS = $(SOURCES:.c=.o)
EXEC = sniffer

# Default target to build
all: $(EXEC)

# Change directory and link objects to create the executable
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -I $(SNIFFER_DIR)/$(HEADER_DIR) $(LDFLAGS) -o $(SNIFFER_DIR)/$(EXEC)

# Compile the source files into object files in the sniffer directory
$(SNIFFER_DIR)/$(SRC_DIR)/%.o: $(SNIFFER_DIR)/$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I $(SNIFFER_DIR)/$(HEADER_DIR) -c $< -o $@

# Clean up object files and executable in the sniffer directory
clean:
	rm -f $(SNIFFER_DIR)/$(SRC_DIR)/*.o $(SNIFFER_DIR)/$(EXEC)

# Run the program from the sniffer directory
run-sniffer: $(EXEC)
	@cd $(SNIFFER_DIR) && ./$(EXEC)
