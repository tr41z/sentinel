@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

:: Detect Windows architecture
echo Detecting Windows architecture...
wmic os get osarchitecture | findstr /C:"64-bit" >nul 2>nul
if %ERRORLEVEL% EQU 0 (
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

:: Install Node.js and NPM if not installed
where node >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing Node.js and NPM using Chocolatey...
    choco install nodejs -y
    refreshenv
) else (
    echo Node.js is already installed.
)

:: Verify Node.js and NPM installation
node --version >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Node.js installation failed. Please install manually.
    pause
    exit /b 1
)
npm --version >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo NPM installation failed. Please install manually.
    pause
    exit /b 1
)
echo Node.js and NPM installation verified.

:: Install CMake if not installed
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing CMake using Chocolatey...
    choco install cmake -y
    refreshenv
) else (
    echo CMake is already installed.
)

:: Install Visual Studio Build Tools if not installed
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing Visual Studio Build Tools using Chocolatey...
    choco install visualstudio2022buildtools -y --package-parameters "--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --includeOptional"
    refreshenv
) else (
    echo Visual Studio Build Tools are already installed.
)

:: Verify CMake and C++ compiler installation
cmake --version >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo CMake installation failed. Please install manually.
    pause
    exit /b 1
)
cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Visual Studio C++ Compiler not found. Please install manually.
    pause
    exit /b 1
)
echo CMake and C++ compiler installation verified.

ENDLOCAL
pause
exit /b 0
