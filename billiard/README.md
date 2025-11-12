# Billiard Simulation

A C implementation of the "Quasi Billiards" Shadertoy project with both old-school software rendering and modern OpenGL rendering.

## Features

- Physics simulation of billiard balls with collision detection
- Cue stick mechanics with swing and shot mechanics
- Two rendering modes:
  - Software rendering (rasterization)
  - OpenGL rendering with shaders
- Complete conversion of GLSL physics to C

## Project Structure

```
billiard/
├── src/                    # Source code
│   ├── billiard.h          # Game state and physics structures
│   ├── billiard.c          # Physics simulation implementation
│   ├── raster_renderer.h   # Software renderer header
│   ├── raster_renderer.c   # Software renderer implementation
│   ├── opengl_renderer.h   # OpenGL renderer header
│   ├── opengl_renderer.c   # OpenGL renderer implementation
│   └── main.c              # Main game loop
├── shaders/                # GLSL shader files
│   ├── billiard.vert       # Vertex shader
│   ├── billiard.frag       # Fragment shader
│   ├── billiard_main.frag  # Original Shadertoy main shader
│   └── billiard_physics.frag # Original Shadertoy physics shader
├── Makefile                # Build system
└── README.md               # This file
```

## Build Instructions

### Prerequisites

For Ubuntu/Debian:
```bash
sudo apt-get install libglfw3-dev libglew-dev libgl1-mesa-dev libglu1-mesa-dev
```

For macOS with Homebrew:
```bash
brew install glfw glew
```

### Building

```bash
make
```

### Running

With OpenGL rendering (default):
```bash
./billiard
```

With software rendering:
```bash
./billiard --software
```

## Physics Conversion

The original GLSL physics code was converted to C, preserving the same behavior:
- Ball collision physics with repulsion forces
- Friction simulation
- Quaternion-based ball rotation
- Pocket detection
- Game state management (waiting to shoot to ball motion)

## Rendering Modes

### Software Rendering
- Implements a simple software rasterizer
- Draws billiard table, balls, and cue stick
- Shows the basic game mechanics

### OpenGL Rendering
- Uses modern OpenGL with programmable shaders
- Implements 3D rendering of the billiard scene
- Includes lighting and shading effects

## Controls (OpenGL Mode)
- Close the window to exit
- The game follows the original Shadertoy logic:
  - Automatically starts in waiting mode
  - Cue swings automatically
  - Click to shoot in original Shadertoy (not implemented in this conversion)

## License

The original Shadertoy code is licensed under Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.