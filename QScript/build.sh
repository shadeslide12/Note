#!/bin/bash

# CFD Solver GUI Build Script for WSL/Linux
# This script builds the project using CMake and Qt5

set -e  # Exit on error

# Configuration
QT_PATH="/mnt/f/Linux_Files/Qt/5.15.2/gcc_64"
BUILD_DIR="build"

echo "=== CFD Solver GUI Build Script ==="
echo "Qt Path: $QT_PATH"
echo "Build Directory: $BUILD_DIR"
echo ""

# Clean previous build if requested
if [ "$1" == "clean" ]; then
    echo "Cleaning previous build..."
    rm -rf "$BUILD_DIR"
    echo "Clean complete."
    echo ""
fi

# Create build directory
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# Configure with CMake
echo "Configuring project with CMake..."
cmake -DCMAKE_PREFIX_PATH="$QT_PATH" -S . -B "$BUILD_DIR"
echo ""

# Build
echo "Building project..."
cmake --build "$BUILD_DIR" -j$(nproc)
echo ""

echo "=== Build Complete ==="
echo "Executable: $BUILD_DIR/CFDSolverGUI"
echo ""
echo "To run the application:"
echo "  ./$BUILD_DIR/CFDSolverGUI"
echo ""
echo "To run tests:"
echo "  cd $BUILD_DIR && ctest --output-on-failure"
echo ""
