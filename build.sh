#!/bin/bash

# STL to EZ/NEC Converter Build Script
# Cross-platform build script for Linux, Windows, and macOS

set -e

echo "STL to EZ/NEC Converter Build Script"
echo "===================================="

# Detect operating system
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    OS="windows"
else
    echo "Unknown operating system: $OSTYPE"
    exit 1
fi

echo "Detected OS: $OS"

# Create build directory
if [ -d "build" ]; then
    echo "Removing existing build directory..."
    rm -rf build
fi

mkdir build
cd build

echo "Configuring with CMake..."

# Configure with CMake
if [ "$OS" == "windows" ]; then
    cmake .. -G "Visual Studio 16 2019" -A x64
    echo "Building with Visual Studio..."
    cmake --build . --config Release
else
    cmake .. -DCMAKE_BUILD_TYPE=Release
    echo "Building with make..."
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
fi

echo ""
echo "Build completed successfully!"
echo ""

# Check if executable was created
if [ -f "stl-to-eznec" ] || [ -f "stl-to-eznec.exe" ]; then
    echo "Executable created:"
    if [ -f "stl-to-eznec" ]; then
        ls -la stl-to-eznec
    else
        ls -la stl-to-eznec.exe
    fi
    echo ""
    echo "To run the program:"
    if [ -f "stl-to-eznec" ]; then
        echo "  ./stl-to-eznec"
    else
        echo "  ./stl-to-eznec.exe"
    fi
else
    echo "Error: Executable not found!"
    exit 1
fi

echo ""
echo "Build script completed successfully!"


