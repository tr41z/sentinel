@echo off

:: Activate the virtual environment
call .\.venv\Scripts\activate

:: Define source and destination directories
set SRC_DIR=src
set BIN_DIR=bin
set HOOKS_DIR=%SRC_DIR%\hooks

:: Run PyInstaller to build the executable
pyinstaller --onefile ^
  --add-data "%SRC_DIR%\models;models" ^
  --add-data "%SRC_DIR%\config.py;." ^
  --add-data "%SRC_DIR%\database.py;." ^
  --add-data "%SRC_DIR%\scheduler.py;." ^
  --additional-hooks-dir="%HOOKS_DIR%" ^
  --hidden-import "sklearn" ^
  --hidden-import "numpy" ^
  --hidden-import "numpy.core.multiarray" ^
  %SRC_DIR%\main.py

:: Move the executable to the 'bin' directory
move dist\main.exe %BIN_DIR%\main.exe

:: Clean up build artifacts
rmdir /s /q dist
rmdir /s /q build
del main.spec

:: Deactivate virtual environment
deactivate

echo Build completed and executable is placed in '%BIN_DIR%'
