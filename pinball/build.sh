#!/bin/bash

# Pinball Game Build Script

set -e  # Exit on any error

echo "Pinball Game Build Script"
echo "========================="

BUILD_SYSTEM=${1:-cmake}  # Default to cmake, can override with autotools

if [ "$BUILD_SYSTEM" = "cmake" ]; then
    echo "Using CMake build system..."
    
    # Create build directory if it doesn't exist
    if [ ! -d "build" ]; then
        echo "Creating build directory..."
        mkdir build
    fi

    # Navigate to build directory
    cd build

    echo "Configuring project with CMake..."
    cmake ..

    echo "Building the project..."
    make -j$(nproc)
    
    echo ""
    echo "CMake build completed (may have failed due to missing dependencies)."
    echo "To run the game, use: ./pinball_game"
    echo ""
    
elif [ "$BUILD_SYSTEM" = "autotools" ]; then
    echo "Using Autotools build system..."
    
    # Generate autotools files if not present
    if [ ! -f "Makefile.am" ] || [ ! -f "configure.ac" ]; then
        echo "Error: Autotools files not found"
        exit 1
    fi
    
    # Check if autoconf is available
    if ! command -v autoreconf &> /dev/null; then
        echo "Error: autoreconf not found. Please install autoconf and automake."
        exit 1
    fi
    
    echo "Generating autotools build files..."
    autoreconf --install
    
    echo "Configuring project with autotools..."
    ./configure
    
    echo "Building the project..."
    make -j$(nproc)
    
    echo ""
    echo "Autotools build completed (may have failed due to missing dependencies)."
    echo "To run the game, use: ./pinball_game"
    echo ""
else
    echo "Usage: $0 [cmake|autotools]"
    echo "  cmake (default) - Use CMake build system"
    echo "  autotools      - Use Autotools build system"
    exit 1
fi