#!/bin/bash
# Build script for Soccer2D using binutils directly

echo "Building Soccer2D using binutils..."

# Create build directory
mkdir -p build

# Compile all source files to object files
echo "Compiling source files..."

gcc -c src/soccer2d_physics.c -o build/soccer2d_physics.o -I./include -Wall -Wextra -std=c99
gcc -c src/soccer2d_physics_update.c -o build/soccer2d_physics_update.o -I./include -Wall -Wextra -std=c99
gcc -c src/soccer2d_render.c -o build/soccer2d_render.o -I./include -Wall -Wextra -std=c99
gcc -c src/main.c -o build/main.o -I./include -Wall -Wextra -std=c99

# Link the terminal-only version
echo "Linking terminal version..."
ld -o soccer2d_term build/soccer2d_physics.o build/soccer2d_physics_update.o build/soccer2d_render.o build/main.o -lc -lm

# For the OpenGL version, we need additional libraries
echo "Linking OpenGL version (if libraries available)..."
if pkg-config --exists glfw3 glew; then
    # Get the proper flags from pkg-config
    CFLAGS=$(pkg-config --cflags glfw3 glew)
    LIBS=$(pkg-config --libs glfw3 glew)
    
    # Recompile with OpenGL flags
    gcc -DUSE_OPENGL -c src/soccer2d_physics.c -o build/soccer2d_physics_gl.o -I./include -Wall -Wextra -std=c99
    gcc -DUSE_OPENGL -c src/soccer2d_physics_update.c -o build/soccer2d_physics_update_gl.o -I./include -Wall -Wextra -std=c99
    gcc -DUSE_OPENGL -c src/soccer2d_render.c -o build/soccer2d_render_gl.o -I./include -Wall -Wextra -std=c99
    gcc -DUSE_OPENGL -c src/main.c -o build/main_gl.o -I./include -Wall -Wextra -std=c99
    
    # Link OpenGL version
    gcc -o soccer2d_gl build/soccer2d_physics_gl.o build/soccer2d_physics_update_gl.o build/soccer2d_render_gl.o build/main_gl.o -lGL -lGLU $LIBS -lm
else
    echo "Warning: OpenGL libraries not found. Building terminal version only."
fi

echo "Build complete!"
echo ""
echo "Run the terminal version with: ./soccer2d_term"
echo "Run the OpenGL version with: ./soccer2d_gl --shader"