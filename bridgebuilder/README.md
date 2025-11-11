# Bridge Builder Game

A physics-based bridge building game converted from GLSL Shadertoy to pure C with both software and shader-based rendering options.

## Features

- Physics simulation using verlet integration and constraint solving
- Bridge building mechanics with nodes and segments
- Two rendering modes:
  - Shader-based renderer (modern OpenGL)
  - Software renderer fallback
- Interactive editing and simulation modes
- Keyboard controls for building and manipulating structures

## Controls

- SPACE: Toggle between edit and simulation modes
- Mouse: Select and move nodes in edit mode
- A: Vertex mode (add/remove nodes)
- S: Edge mode (add/remove segments)
- D: Pin/unpin nodes
- R: Reset the simulation

## Building

### Prerequisites
- CMake 3.10 or higher
- OpenGL development libraries
- GLFW3
- GLEW

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Alternative Build with Binutils

```bash
./build_with_binutils.sh
```

## Project Structure

```
├── CMakeLists.txt           # CMake build configuration
├── Makefile               # Make-based build alternative
├── build_with_binutils.sh # Direct binutils build script
├── .gitignore            # Git ignore patterns
├── buffer_A.glsl         # Original shader source
├── common.glsl           # Common shader utilities
├── image.glsl            # Image rendering shader
├── include/              # Header files
│   ├── physics.h
│   ├── renderer.h
│   ├── input.h
│   ├── bridge_builder.h
│   └── utils.h
├── shaders/              # Shader files
│   ├── vertex_shader.glsl
│   └── fragment_shader.glsl
└── src/                  # Source files
    ├── main.c
    ├── physics.c
    ├── renderer.c
    ├── input.c
    ├── bridge_builder.c
    └── shader_loader.c
```

## Technical Details

The game implements a complete physics simulation system based on the original GLSL Shadertoy implementation. Key features include:

- Verlet integration for stable physics simulation
- Constraint solving for structural integrity
- Interactive node and segment manipulation
- Edit/Simulate mode switching
- Collision detection and response

The software renderer provides a fallback when OpenGL shaders are not available, though with reduced visual quality.