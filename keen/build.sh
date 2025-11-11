#!/bin/bash

# Commander Keen Standard C++ Build System
# Compiles shared libraries and final executable using standard binutils (gcc/g++/ld/ar)

set -e  # Exit on any error

# Configuration
BUILD_TYPE=${1:-"debug"}  # Default to debug build if no argument provided
BUILD_DIR="build"
SRC_DIR="stdsrc"
LIB_DIR="lib"
BIN_DIR="bin"

# Compiler settings
CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra -fPIC"
LDFLAGS="-shared"

# Debug vs Release flags
if [ "$BUILD_TYPE" = "debug" ]; then
    CXXFLAGS="$CXXFLAGS -g -O0 -DDEBUG"
    echo "Building in DEBUG mode..."
else
    CXXFLAGS="$CXXFLAGS -O2 -DNDEBUG"
    echo "Building in RELEASE mode..."
fi

# Create necessary directories
mkdir -p $BUILD_DIR $LIB_DIR $BIN_DIR

# Define shared libraries to build
LIBS=(
    "libkeencore.so"
    "libkeengfx.so" 
    "libkeenvideo.so"
    "libkeenaudio.so"
    "libkeeninput.so"
    "libkeengame.so"
)

echo "Starting build process..."

# Build libkeencore.so (core infrastructure)
echo "Building libkeencore.so..."
CORE_SOURCES=(
    "$SRC_DIR/core/MemoryManager.cpp"
    "$SRC_DIR/core/ConfigManager.cpp"
    "$SRC_DIR/core/FileSystem.cpp"
    "$SRC_DIR/core/Timer.cpp"
    "$SRC_DIR/core/Application.cpp"
)
$CXX $CXXFLAGS $LDFLAGS -o "$LIB_DIR/libkeencore.so" "${CORE_SOURCES[@]}"
echo "Built libkeencore.so"

# Build libkeengfx.so (graphics system)
echo "Building libkeengfx.so..."
GFX_SOURCES=(
    "$SRC_DIR/graphics/GraphicsSystem.cpp"
    "$SRC_DIR/graphics/TextureManager.cpp"
    "$SRC_DIR/graphics/ResourcePool.cpp"
    "$SRC_DIR/graphics/SpriteAnimation.cpp"
)
$CXX $CXXFLAGS $LDFLAGS -o "$LIB_DIR/libkeengfx.so" "${GFX_SOURCES[@]}" -lGL
echo "Built libkeengfx.so"

# Build libkeengame.so (game logic system)
echo "Building libkeengame.so..."
GAME_SOURCES=(
    "$SRC_DIR/game/ActionManager.cpp"
    "$SRC_DIR/game/GameManager.cpp"
    "$SRC_DIR/game/InteractionManager.cpp"
    "$SRC_DIR/game/KeenSystem.cpp"
)
$CXX $CXXFLAGS $LDFLAGS -o "$LIB_DIR/libkeengame.so" "${GAME_SOURCES[@]}"
echo "Built libkeengame.so"

# Build other libraries (stub implementations for now)
echo "Building stub libraries..."
$CXX $CXXFLAGS $LDFLAGS -o "$LIB_DIR/libkeenvideo.so" -x c++ - <<EOF
// Stub implementation
extern "C" void libkeenvideo_init() {}
extern "C" void libkeenvideo_shutdown() {}
EOF

$CXX $CXXFLAGS $LDFLAGS -o "$LIB_DIR/libkeenaudio.so" -x c++ - <<EOF
// Stub implementation
extern "C" void libkeenaudio_init() {}
extern "C" void libkeenaudio_shutdown() {}
EOF

$CXX $CXXFLAGS $LDFLAGS -o "$LIB_DIR/libkeeninput.so" -x c++ - <<EOF
// Stub implementation
extern "C" void libkeeninput_init() {}
extern "C" void libkeeninput_shutdown() {}
EOF

echo "Built stub libraries"

# Build the main executable
echo "Building main executable..."
$CXX $CXXFLAGS -o "$BIN_DIR/keen" "$SRC_DIR/main.cpp" -L"$LIB_DIR" -lkeencore -lkeengfx -lkeenvideo -lkeenaudio -lkeeninput -lkeengame -lGL

echo "Build completed successfully!"
echo "Libraries are in: $LIB_DIR/"
echo "Executable is in: $BIN_DIR/"
echo "To run: LD_LIBRARY_PATH=$LIB_DIR ./$BIN_DIR/keen"
