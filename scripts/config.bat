@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

:: Detect Windows architecture
echo Detecting Windows architecture...
for /f "tokens=*" %%A in ('powershell -NoProfile -Command "[Environment]::Is64BitOperatingSystem"') do set IS_64BIT=%%A
if /i "%IS_64BIT%"=="True" (
    set ARCH=64-bit
) else (
    set ARCH=32-bit
)
echo Detected: %ARCH%

:: Check if Chocolatey is installed
where choco >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Chocolatey is not installed. Installing Chocolatey...
    powershell -NoProfile -ExecutionPolicy Bypass -Command "Set-ExecutionPolicy Bypass -Scope Process; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))"
    IF %ERRORLEVEL% NEQ 0 (
        echo Failed to install Chocolatey. Please install it manually from https://chocolatey.org/install.
        pause
        exit /b 1
    )
    refreshenv
)
goto :check_golang

:check_golang
:: Install Golang if not installed
where go >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing Golang using Chocolatey...
    choco install golang -y
    refreshenv
) else (
    echo Golang is already installed.
)

:: Verify Golang installation
go version >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Golang installation failed. Please install manually.
    pause
    exit /b 1
)
echo Golang installation verified.
goto :check_cmake

:check_cmake
:: Install CMake if not installed
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing CMake using Chocolatey...
    choco install cmake -y
    refreshenv
) else (
    echo CMake is already installed.
)

:: Verify CMake and C++ compiler installation
cmake --version >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo CMake installation failed. Please install manually.
    pause
    exit /b 1
)

echo CMake and C++ compiler installation verified.
goto :check_python

:check_python
:: Install Python if not installed
where python >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing Python using Chocolatey...
    choco install python -y
    refreshenv
) else (
    echo Python is already installed.
)

:: Verify Python installation
python --version >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Python installation failed. Please install manually.
    pause
    exit /b 1
)
pip --version >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Pip installation failed. Please install manually.
    pause
    exit /b 1
)
echo Python installation verified.

ENDLOCAL
exit /b 0
