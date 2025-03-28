#!/bin/bash

set -e

# Function to install Golang on Debian-based systems
install_golang_debian() {
    echo "Installing Golang on Debian-based system..."
    sudo apt update
    sudo apt install -y golang
}

# Function to install Golang on Red Hat-based systems
install_golang_redhat() {
    echo "Installing Golang on Red Hat-based system..."
    if command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y golang
    else
        sudo yum install -y golang
    fi
}

# Function to install Golang on macOS
install_golang_macos() {
    echo "Installing Golang on macOS..."
    brew install go
}

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

# Function to install Python on Debian-based systems
install_python_debian() {
    echo "Installing Python on Debian-based system..."
    sudo apt update
    sudo apt install -y python3 python3-pip
}

# Function to install Python on Red Hat-based systems
install_python_redhat() {
    echo "Installing Python on Red Hat-based system..."
    if command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y python3 python3-pip
    else
        sudo yum install -y python3 python3-pip
    fi
}

# Function to install Python on macOS
install_python_macos() {
    echo "Installing Python on macOS..."
    brew install python
}

# Main script logic
echo "Detecting platform..."
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    if command -v apt >/dev/null 2>&1; then
        install_golang_debian
        install_cmake_debian
        install_python_debian
    elif command -v yum >/dev/null 2>&1 || command -v dnf >/dev/null 2>&1; then
        install_golang_redhat
        install_cmake_redhat
        install_python_redhat
    else
        echo "Unsupported Linux distribution. Please install Golang, CMake, and Python manually."
        exit 1
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    install_golang_macos
    install_cmake_macos
    install_python_macos
else
    echo "Unsupported platform: $OSTYPE. This script only supports macOS and Linux."
    exit 1
fi

# Verify installations
echo "Verifying installations..."
if command -v go >/dev/null 2>&1 && command -v cmake >/dev/null 2>&1 && command -v g++ >/dev/null 2>&1 && command -v python3 >/dev/null 2>&1 && command -v pip3 >/dev/null 2>&1; then
    go version
    cmake --version
    g++ --version
    python3 --version
    pip3 --version
else
    echo "One or more installations failed. Please check the log and install manually."
    exit 1
fi