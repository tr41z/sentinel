@echo off

:: Activate the virtual environment
call .\.venv\Scripts\activate

:: Define source and destination directories
set SRC_DIR=src
set BIN_DIR=bin
set HOOKS_DIR=%SRC_DIR%\hooks

:: Run PyInstaller to build the executable with the desired name
pyinstaller --onefile --name ai ^
  --add-data "%SRC_DIR%\models;models" ^
  --add-data "%SRC_DIR%\config.py;." ^
  --add-data "%SRC_DIR%\database.py;." ^
  --add-data "%SRC_DIR%\scheduler.py;." ^
  --additional-hooks-dir "%HOOKS_DIR%" ^
  --hidden-import sklearn ^
  --hidden-import sklearn.pipeline ^
  --hidden-import sklearn.utils._heap ^
  --hidden-import sklearn.utils._typedefs ^
  --hidden-import sklearn.utils._cython_blas ^
  --hidden-import sklearn.neighbors._partition_nodes ^
  --hidden-import sklearn.tree._utils ^
  --hidden-import sklearn.ensemble._forest ^
  --hidden-import sklearn.ensemble._gb ^
  --hidden-import sklearn.ensemble._gradient_boosting ^
  --hidden-import sklearn.ensemble._hist_gradient_boosting ^
  --hidden-import numpy ^
  --hidden-import numpy.core.multiarray ^
  "%SRC_DIR%\main.py"

:: Move the executable to the 'bin' directory
if exist dist\ai.exe move /Y dist\ai.exe %BIN_DIR%\ai.exe

:: Clean up build artifacts
rmdir /s /q dist
rmdir /s /q build
if exist ai.spec del ai.spec

:: Deactivate virtual environment
deactivate

echo Build completed and executable is placed in '%BIN_DIR%\ai.exe'
