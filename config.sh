#!/bin/bash

set -e

# Function to install CMake on Debian-based systems
install_cmake_debian() {
    echo "Installing CMake on Debian-based system..."
    sudo apt update
    sudo apt install -y cmake g++
}

# Function to install CMake on Red Hat-based systems
install_cmake_redhat() {
    echo "Installing CMake on Red Hat-based system..."
    if command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y cmake gcc-c++
    else
        sudo yum install -y cmake gcc-c++
    fi
}

# Function to install CMake on macOS
install_cmake_macos() {
    echo "Installing CMake on macOS..."
    if ! command -v brew >/dev/null 2>&1; then
        echo "Homebrew not found. Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        touch /Users/$(whoami)/.zprofile
        echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> /Users/$(whoami)/.zprofile
        eval "$(/opt/homebrew/bin/brew shellenv)"
    fi
    brew install cmake gcc
}

# Function to install .NET 9 SDK on Linux
install_dotnet9() {
    echo "Installing .NET 9 SDK..."

    # Add Microsoft package repository
    wget https://packages.microsoft.com/config/ubuntu/22.04/prod.list
    sudo mv prod.list /etc/apt/sources.list.d/microsoft-prod.list

    # Import Microsoft GPG key
    wget -q https://packages.microsoft.com/keys/microsoft.asc -O- | sudo apt-key add -

    # Update apt and install .NET 9 SDK
    sudo apt update
    sudo apt install -y dotnet-sdk-9.0
}

# Main script logic
echo "Detecting platform..."
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    if command -v apt >/dev/null 2>&1; then
        install_cmake_debian
        install_dotnet9
    elif command -v yum >/dev/null 2>&1 || command -v dnf >/dev/null 2>&1; then
        install_cmake_redhat
    else
        echo "Unsupported Linux distribution. Please install CMake manually."
        exit 1
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    install_cmake_macos
else
    echo "Unsupported platform: $OSTYPE. This script only supports macOS and Linux."
    exit 1
fi

# Verify installations
echo "Verifying installations..."
if command -v cmake >/dev/null 2>&1 && command -v g++ >/dev/null 2>&1; then
    cmake --version
    g++ --version
else
    echo "CMake or C++ compiler installation failed. Please check the log and install manually."
    exit 1
fi

if command -v dotnet >/dev/null 2>&1; then
    dotnet --version
else
    echo ".NET SDK installation failed. Please check the log and install manually."
    exit 1
fi