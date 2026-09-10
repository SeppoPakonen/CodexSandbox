#!/bin/bash

# Build script using binutils for the FEZ game

# Create build directory
mkdir -p build

echo "Compiling C game..."

# Compile the source files with binutils (using as for assembly if needed, but gcc handles that)
gcc -c src/main.c -o build/main.o -Iinclude -std=c99 -Wall -O2 -DGL_GLEXT_LEGACY
gcc -c src/math_world.c -o build/math_world.o -Iinclude -std=c99 -Wall -O2
gcc -c src/software_renderer.c -o build/software_renderer.o -Iinclude -std=c99 -Wall -O2
gcc -c src/shader_renderer.c -o build/shader_renderer.o -Iinclude -std=c99 -Wall -O2

# Link the objects into the final executable
gcc build/main.o build/math_world.o build/software_renderer.o build/shader_renderer.o -o build/fez_game -lGL -lGLU -lSDL2 -lSDL2main -lm

echo "Build complete! Executable: build/fez_game"

# Verify the executable with binutils tools
echo "Checking symbols with nm:"
nm -D build/fez_game | grep -E "(render|update|main)" | head -10

echo "Checking with objdump:"
objdump -t build/main.o | head -10

echo "Binary size information:"
size build/fez_game

echo "You can now run the game with: ./build/fez_game"