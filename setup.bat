@echo off
setlocal enabledelayedexpansion
chcp 65001 > nul

echo 🚀 Starting setup...

:: Step 1: Run scripts\config.bat or config.sh
echo 📂 Navigating to scripts...
cd scripts || (echo ❌ Failed to enter 'scripts' directory! & exit /b 1)

if exist config.bat (
    echo 🔧 Running config.bat...
    call config.bat
) else if exist config.sh (
    echo ⚠️ config.sh detected, but running on Windows. Please run on Unix.
    exit /b 1
) else (
    echo ❌ No config script found!
    exit /b 1
)
cd ..

:: Load MinGW environment variables
echo 🔧 Loading MinGW environment variables...
set PATH=%PATH%;C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin
set CXX=g++
set CC=gcc
echo ✅ MinGW environment variables loaded.

:: Step 2: Run ai-mod\build.bat or build.sh
echo 📂 Navigating to ai-mod...
cd ai-mod || (echo ❌ Failed to enter 'ai-mod' directory! & exit /b 1)

if exist build.bat (
    echo 🔧 Running build.bat...
    call build.bat
) else if exist build.sh (
    echo ⚠️ build.sh detected, but running on Windows. Please run on Unix.
    exit /b 1
) else (
    echo ❌ No build script found!
    exit /b 1
)
cd ..

:: Step 3: Handle sniffer-mod if it exists
if exist sniffer-mod (
    echo 📂 Navigating to sniffer-mod...
    cd sniffer-mod || (echo ❌ Failed to enter 'sniffer-mod' directory! & exit /b 1)

    echo 🗑️  Removing old build directory...
    rmdir /s /q build 2> nul
    mkdir build
    cd build || (echo ❌ Failed to create/enter 'build' directory! & exit /b 1)

    echo 🔨 Running CMake...
    cmake -G "Unix Makefiles" ..
    if %ERRORLEVEL% NEQ 0 (
        echo ❌ CMake failed! Check logs.
        exit /b 1
    )

    echo 🔨 Running Make...
    make
    if %ERRORLEVEL% NEQ 0 (
        echo ❌ Make failed! Check logs.
        exit /b 1
    )

    echo ✅ CMake setup completed!
    cd ../..
)

echo 🎉 Setup completed successfully!
exit /b 0
