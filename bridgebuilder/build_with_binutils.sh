#!/bin/bash

# This script demonstrates how to build the BridgeBuilder game
# using binutils for assembly and linking operations

set -e  # Exit on any error

echo "Building BridgeBuilder game..."

# Configuration
CC=gcc
CFLAGS="-Wall -Wextra -O2 -std=c11 -Iinclude"
LIBS="-lglfw -lGL -lGLEW -lm"
TARGET="BridgeBuilder"

# Source files
SOURCES="src/main.c src/physics.c src/renderer.c src/input.c src/bridge_builder.c src/shader_loader.c"

# Assembly output directory
ASM_DIR="asm_output"
mkdir -p $ASM_DIR

echo "Step 1: Compiling source files to assembly..."
for src in $SOURCES; do
    base_name=$(basename $src .c)
    echo "  Compiling $src to assembly..."
    $CC $CFLAGS -S -o $ASM_DIR/$base_name.s $src
done

echo "Step 2: Converting assembly to object files..."
for src in $SOURCES; do
    base_name=$(basename $src .c)
    echo "  Assembling $ASM_DIR/$base_name.s to object file..."
    as --64 $ASM_DIR/$base_name.s -o $ASM_DIR/$base_name.o
done

echo "Step 3: Linking with binutils..."
echo "  Creating final executable $TARGET..."
ld --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 \
   /usr/lib/x86_64-linux-gnu/crt1.o /usr/lib/x86_64-linux-gnu/crti.o \
   $ASM_DIR/*.o \
   -lglfw -lGL -lGLEW -lm \
   /usr/lib/x86_64-linux-gnu/crtn.o \
   -o $TARGET

echo "Step 4: Using objcopy to manipulate the binary..."
objcopy --only-keep-debug $TARGET $TARGET.debug
objcopy --strip-debug $TARGET
objcopy --add-gnu-debuglink=$TARGET.debug $TARGET

echo "Step 5: Using strip to remove symbol table (optional)..."
strip --strip-unneeded $TARGET

echo ""
echo "Build completed successfully!"
echo "Executable: $TARGET"
echo "Debug symbols: $TARGET.debug"
echo ""
echo "To run the game: ./BridgeBuilder"

# Verification
if [ -f "$TARGET" ]; then
    echo "Verification: Executable exists and is executable"
    file $TARGET
else
    echo "ERROR: Build failed - executable not found!"
    exit 1
fi