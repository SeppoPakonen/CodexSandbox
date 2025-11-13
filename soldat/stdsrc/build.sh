#!/bin/bash

# Build script for soldat/stdsrc project

set -e  # Exit immediately if a command exits with a non-zero status

echo "Starting build process for soldat/stdsrc..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

# Change to build directory
cd build

# Run CMake to generate build files
echo "Running CMake to generate build files..."
cmake ..

# Build the project
echo "Building the project..."
make

echo "Build process completed!"
echo "Executables should be located in the build/bin directory if compilation was successful."