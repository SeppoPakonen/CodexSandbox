# Soccer2D Game

A 2D soccer game converted from Shadertoy, with both old-school terminal rendering and modern OpenGL shader rendering.

## Features

- Physics system based on the original Shadertoy soccer2d implementation
- Two rendering modes:
  - Old school terminal-based renderer
  - Modern OpenGL shader renderer
- Complete soccer game mechanics (kicking, passing, scoring, throw-ins, corners)

## Building

### Prerequisites

- GCC compiler
- Make (optional, for Makefile)
- For OpenGL version: GLFW3, GLEW, OpenGL libraries

### Installation of Dependencies

For Ubuntu/Debian:
```bash
sudo apt-get install build-essential libglfw3-dev libglew-dev
```

For Fedora/RHEL:
```bash
sudo dnf install gcc make glfw-devel glew-devel
```

For Arch Linux:
```bash
sudo pacman -S gcc make glfw-x11 glew
```

### Building the Game

Using Makefile:
```bash
make                    # Build terminal version
make soccer2d_shader    # Build OpenGL version
make both              # Build both versions
```

Using build script:
```bash
./build.sh
```

## Running

For terminal version:
```bash
./soccer2d
```

For OpenGL shader version:
```bash
./soccer2d --shader
```

## Controls

- A: Shot
- S: Pass
- E: Toggle demo mode
- C: Toggle auto shot (not fully implemented)
- Mouse: Control player (in OpenGL version)
- Q: Quit

## Architecture

- `src/soccer2d_physics.c` - Physics implementation
- `src/soccer2d_physics_update.c` - Game logic and player behavior
- `src/soccer2d_render.c` - Rendering functions
- `src/main.c` - Main game loop
- `include/soccer2d.h` - Game header
- `shaders/` - GLSL shader files

## Credits

Based on "Soccermania" by Kastorp from Shadertoy (https://www.shadertoy.com/view/4sXGzn)