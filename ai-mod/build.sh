#!/bin/bash

# Ensure the script stops on errors
set -e

# Check if the virtual environment exists, create it if not
if [ ! -d ".venv" ]; then
  echo "Creating virtual environment..."
  python3 -m venv .venv
fi

# Activate the virtual environment
source .venv/bin/activate

# Ensure necessary dependencies are installed
pip install --upgrade pip
pip install pyinstaller scikit-learn numpy fastapi pandas uvicorn

# Define the source and destination directories
SRC_DIR="src"
BIN_DIR="bin"
HOOKS_DIR="$SRC_DIR/hooks"

# Ensure the bin directory exists
mkdir -p "$BIN_DIR"

# Run PyInstaller to build the executable with all necessary hidden imports
pyinstaller --onefile --name ai \
  --add-data "$SRC_DIR/models:models" \
  --add-data "$SRC_DIR/config.py:." \
  --add-data "$SRC_DIR/database.py:." \
  --add-data "$SRC_DIR/scheduler.py:." \
  --additional-hooks-dir="$HOOKS_DIR" \
  --hidden-import "sklearn" \
  --hidden-import "sklearn.pipeline" \
  --hidden-import "sklearn.utils._heap" \
  --hidden-import "sklearn.utils._typedefs" \
  --hidden-import "sklearn.utils._cython_blas" \
  --hidden-import "sklearn.neighbors._partition_nodes" \
  --hidden-import "sklearn.tree._utils" \
  --hidden-import "sklearn.ensemble._forest" \
=  --hidden-import "numpy" \
  --hidden-import "numpy.core.multiarray" \
  "$SRC_DIR/main.py"

# Move the executable to the 'bin' directory
if [ -f dist/ai ]; then
  mv dist/ai "$BIN_DIR/ai"
fi

# Clean up build artifacts
rm -rf dist build ai.spec

# Deactivate virtual environment
deactivate

echo "Build completed and executable is placed in '$BIN_DIR/ai'"
