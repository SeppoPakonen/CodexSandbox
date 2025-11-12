# Boats with Drone - C Implementation

This is a C implementation of the Shadertoy project "Boats with Drone" (MsdSRj.json). The implementation includes physics converted from the original GLSL code. The project provides both a simulation version (text-based) that works without graphics libraries and a graphics version with both old-school and shader-based rendering (when graphics libraries are available).

## Features

- **Physics Simulation**: Wave physics, boat physics, and drone movement based on the original Shadertoy GLSL code
- **Converted GLSL Functions**: Noise functions, fractal Brownian motion, wave dynamics implemented in C
- **Dual Versions**: Simulation-based version (always available) and graphics version (when libraries available)

## Requirements

### For Simulation (Always Available):
- A C compiler (GCC recommended)
- Math library (libm, usually included by default)

### For Graphics Version:
- OpenGL development libraries
- GLUT development libraries
- Binutils for building

For Ubuntu/Debian systems (for graphics version):
```bash
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

## Building

To build the project, use make:

```bash
make
```

This will build the simulation version by default.

To build the graphics version (if libraries are available):
```bash
make boats_game
```

## Running

To run the simulation:

```bash
make run
```

This will show the physics simulation with boats and drones moving according to the converted GLSL physics.

To run the graphics version (if built):
```bash
make run-graphics
```

## Project Structure

- `boats_simulation.c`: Physics simulation of boats and drones based on Shadertoy code
- `boats_game.c`: Main game loop with OpenGL rendering (requires graphics libraries)
- `boats_physics.h/c`: Physics calculations converted from GLSL
- `vertex_shader.glsl` / `fragment_shader.glsl`: GLSL shaders for modern rendering
- `Makefile`: Build configuration using binutils

## Implementation Details

The GLSL from the original Shadertoy has been carefully converted to C functions, including:

- Noise functions (noise_ff, noise_fv2, etc.)
- Fractal Brownian Motion (fbm) functions
- Wave height calculations with realistic wave physics
- Boat physics that accounts for water interaction
- Drone movement algorithms

The simulation version provides text-based output of the physics simulation, while the graphics version includes both an old-school renderer using immediate mode OpenGL and a modern renderer using programmable shaders.

## Binutils Usage

The Makefile is configured to show how binutils (as, ld, objcopy, etc.) can be used in the build process. While the primary build uses GCC, the Makefile includes examples of how to use binutils directly for compilation if needed.