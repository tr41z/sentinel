#!/bin/bash
set -e  # Exit on error
set -o pipefail  # Catch errors in piped commands

echo -e "🚀 Starting setup..."

# Step 1: Run scripts/config.sh or config.bat
echo -e "📂 Navigating to scripts..."
cd scripts || { echo -e "❌ Failed to enter 'scripts' directory!"; exit 1; }

if [[ -f "config.sh" ]]; then
    echo -e "🔧 Running config.sh..."
    chmod +x config.sh
    ./config.sh
elif [[ -f "config.bat" ]]; then
    echo -e "⚠️ config.bat detected, but running on Unix. Please run on Windows."
    exit 1
else
    echo -e "❌ No config script found!"
    exit 1
fi
cd ..

# Step 2: Run ai-mod/build.sh or build.bat
echo -e "📂 Navigating to ai-mod..."
cd ai-mod || { echo -e "❌ Failed to enter 'ai-mod' directory!"; exit 1; }

if [[ -f "build.sh" ]]; then
    echo -e "🔧 Running build.sh..."
    chmod +x build.sh
    ./build.sh
elif [[ -f "build.bat" ]]; then
    echo -e "⚠️ build.bat detected, but running on Unix. Please run on Windows."
    exit 1
else
    echo -e "❌ No build script found!"
    exit 1
fi
cd ..

# Step 3: Handle sniffer-mod if it exists
if [[ -d "sniffer-mod" ]]; then
    echo -e "📂 Navigating to sniffer-mod..."
    cd sniffer-mod || { echo -e "❌ Failed to enter 'sniffer-mod' directory!"; exit 1; }

    echo -e "🗑️ Removing old build directory..."
    rm -rf build
    mkdir build
    cd build || { echo -e "❌ Failed to create/enter 'build' directory!"; exit 1; }

    echo -e "🔨 Running CMake..."
    if [[ "$(uname)" == "Darwin" ]] || [[ "$(uname)" == "Linux" ]]; then
        cmake -G "Unix Makefiles" ..
        make
    else
        cmake ..
        make
    fi

    echo -e "✅ CMake setup completed!"
    cd ../..
fi

echo -e "🎉 Setup completed successfully!"
exit 0
