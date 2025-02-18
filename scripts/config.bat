@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

echo Detecting Windows version...

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
)

:: Install Golang using Chocolatey
echo Installing Golang using Chocolatey...
choco install golang -y

:: Verify Golang installation
echo Verifying Golang installation...
where go >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    go version
    echo Golang installation complete.
) else (
    echo Golang installation failed. Please install manually.
    pause
    exit /b 1
)

:: Install Node.js and NPM using Chocolatey
echo Installing Node.js and NPM using Chocolatey...
choco install nodejs -y

:: Verify Node.js and NPM installation
echo Verifying Node.js and NPM installation...
where node >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    node --version
    npm --version
    echo Node.js and NPM installation complete.
) else (
    echo Node.js and NPM installation failed. Please install manually.
    pause
    exit /b 1
)

:: Install CMake and Visual Studio Build Tools using Chocolatey
echo Installing CMake and Visual Studio Build Tools using Chocolatey...
choco install cmake -y
choco install visualstudio2019buildtools -y --package-parameters "--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --includeOptional"

:: Verify installation
echo Verifying CMake and C++ compiler installation...
where cmake >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    cmake --version
    cl
    echo CMake and C++ compiler installation complete.
    pause
) else (
    echo CMake or C++ compiler installation failed. Please install manually.
    pause
    exit /b 1
)

ENDLOCAL
pause
exit /b 0