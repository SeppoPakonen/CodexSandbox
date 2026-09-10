#!/bin/bash

# Build script for Minigolf 3D game
# This script uses binutils and gcc to build the project

set -e  # Exit on any error

echo "Building Minigolf 3D game..."

# Create build directory
mkdir -p build

# Compile each source file
echo "Compiling source files..."
gcc -c -Wall -Wextra -std=c99 -I./include src/main.c -o build/main.o
gcc -c -Wall -Wextra -std=c99 -I./include src/game.c -o build/game.o
gcc -c -Wall -Wextra -std=c99 -I./include src/renderer.c -o build/renderer.o
gcc -c -Wall -Wextra -std=c99 -I./include src/physics.c -o build/physics.o

# Link objects to create the executable
echo "Linking objects..."
gcc build/main.o build/game.o build/renderer.o build/physics.o -o build/minigolf3D -lglfw -lGL -lGLU -lm

# For the software renderer version
echo "Building software renderer version..."
gcc -D SOFTWARE_RENDERER -c -Wall -Wextra -std=c99 -I./include src/main.c -o build/main_sw.o
gcc -D SOFTWARE_RENDERER -c -Wall -Wextra -std=c99 -I./include src/game.c -o build/game_sw.o
gcc -D SOFTWARE_RENDERER -c -Wall -Wextra -std=c99 -I./include src/renderer.c -o build/renderer_sw.o
gcc -D SOFTWARE_RENDERER -c -Wall -Wextra -std=c99 -I./include src/physics.c -o build/physics_sw.o

# Link objects for software renderer
gcc build/main_sw.o build/game_sw.o build/renderer_sw.o build/physics_sw.o -o build/minigolf3D_sw -lglfw -lGL -lGLU -lm

echo "Build completed successfully!"
echo "Executables are in the 'build' directory:"
echo "  - minigolf3D: OpenGL renderer version"
echo "  - minigolf3D_sw: Software renderer version"
echo ""
echo "To run: cd build && ./minigolf3D"
echo "To run with software renderer: cd build && ./minigolf3D_sw"