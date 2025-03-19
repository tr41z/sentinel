#!/bin/bash

# Activate the virtual environment
source .venv/bin/activate

# Define the source and destination directories
SRC_DIR="src"
BIN_DIR="bin"
HOOKS_DIR="$SRC_DIR/hooks"

# Run PyInstaller to build the executable
pyinstaller --onefile \
  --add-data $SRC_DIR/"models:models" \
  --add-data $SRC_DIR/"config.py:." \
  --add-data $SRC_DIR/"database.py:." \
  --add-data $SRC_DIR/"scheduler.py:." \
  --additional-hooks-dir="$HOOKS_DIR" \
  --hidden-import "sklearn" \
  --hidden-import "numpy" \
  --hidden-import "numpy.core.multiarray" \
  $SRC_DIR/main.py

# Move the executable to the 'bin' directory
mv dist/main $BIN_DIR/main

# Clean up build artifacts
rm -rf dist build main.spec

# Deactivate virtual environment
deactivate

echo "Build completed and executable is placed in '$BIN_DIR'"
