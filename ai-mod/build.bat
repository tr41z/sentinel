@echo off

:: Ensure the script stops on errors
setlocal enabledelayedexpansion
set ERRORS=0

:: Check if the virtual environment exists, create it if not
if not exist ".venv" (
    echo Creating virtual environment...
    python -m venv .venv
)

:: Activate the virtual environment
call .\.venv\Scripts\activate

:: Ensure necessary dependencies are installed
pip install --upgrade pip
pip install pyinstaller scikit-learn numpy fastapi uvicorn xgboost

:: Define the source and destination directories
set SRC_DIR=src
set BIN_DIR=bin
set HOOKS_DIR=%SRC_DIR%\hooks

:: Ensure the bin directory exists
if not exist "%BIN_DIR%" (
    mkdir "%BIN_DIR%"
)

:: Find the xgboost shared library and VERSION file
for /f "delims=" %%i in ('python -c "import xgboost.libpath as lp; print(lp.find_lib_path()[0])"') do set XGBOOST_LIB=%%i
for /f "delims=" %%i in ('python -c "import xgboost; import os; print(os.path.join(os.path.dirname(xgboost.__file__), 'VERSION'))"') do set XGBOOST_VERSION=%%i

:: Run PyInstaller to build the executable with the necessary hidden imports
pyinstaller --onefile --name ai ^
  --add-data "%SRC_DIR%\models;models" ^
  --add-data "%SRC_DIR%\config.py;." ^
  --add-data "%SRC_DIR%\database.py;." ^
  --add-data "%SRC_DIR%\scheduler.py;." ^
  --add-data "%XGBOOST_LIB%;lib" ^
  --add-data "%XGBOOST_VERSION%;xgboost" ^
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
  --hidden-import xgboost ^
  "%SRC_DIR%\main.py"

:: Move the executable to the 'bin' directory
if exist dist\ai.exe (
    move /Y dist\ai.exe "%BIN_DIR%\ai.exe"
)

:: Clean up build artifacts
rmdir /s /q dist
rmdir /s /q build
if exist ai.spec del ai.spec

:: Deactivate virtual environment
deactivate

:: Handle errors if any occurred
if %ERRORS% neq 0 (
    echo Build failed.
    exit /b 1
)

echo Build completed and executable is placed in '%BIN_DIR%\ai.exe'

endlocal