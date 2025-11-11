#!/bin/bash

# Commander Keen Standard C++ Run Script
# Sets up the environment and runs the Keen application

set -e  # Exit on any error

# Configuration
BUILD_DIR="build"
LIB_DIR="lib"
BIN_DIR="bin"
EXECUTABLE="keen"

# Check if the executable exists
if [ ! -f "$BIN_DIR/$EXECUTABLE" ]; then
    echo "Error: Executable $BIN_DIR/$EXECUTABLE does not exist."
    echo "Please build the project first using build.sh"
    exit 1
fi

# Check if the library directory exists
if [ ! -d "$LIB_DIR" ]; then
    echo "Error: Library directory $LIB_DIR does not exist."
    echo "Please build the project first using build.sh"
    exit 1
fi

echo "Running Commander Keen Standard C++ Application..."
echo "Executable: $BIN_DIR/$EXECUTABLE"
echo "Libraries: $LIB_DIR/"
echo ""

# Run with proper library path
LD_LIBRARY_PATH="$LIB_DIR:$LD_LIBRARY_PATH" ./"$BIN_DIR/$EXECUTABLE" "$@"

# Capture the exit code
exit_code=$?

echo ""
echo "Application exited with code: $exit_code"

exit $exit_code