# Minigolf 3D Game

A 3D minigolf game based on a Shadertoy project, implemented in pure C with both software and OpenGL rendering options.

## Features

- 3D minigolf gameplay with realistic ball physics
- Two rendering modes: OpenGL (modern) and software rendering (classic)
- Procedurally generated terrain based on Shadertoy algorithm
- Interactive gameplay with mouse-based aiming and shooting
- Collision detection and response system

## Build Requirements

- GCC (GNU Compiler Collection)
- CMake or Make
- GLFW3 development libraries
- OpenGL development libraries
- GLU development libraries
- Math library (libm)

To install dependencies on Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev
```

## Building

### Using Make
```bash
cd minigolf3D
make
```

### Using build script
```bash
cd minigolf3D
./build.sh
```

## Running

### OpenGL Renderer (default)
```bash
./minigolf3D
```

### Software Renderer
```bash
./minigolf3D --software
```

## Controls

- Click and drag to aim the golf shot
- Release to hit the ball
- The longer you drag, the more powerful the shot
- ESC to quit the game

## Game Mechanics

- The ball is affected by gravity and rolls realistically on the terrain
- Collision with the ground causes the ball to bounce with damping
- The goal is to get the ball into the hole
- After scoring, the ball resets and a new hole position is generated

## Project Structure

- `src/` - Source code files
- `include/` - Header files
- `shaders/` - GLSL shader files converted from Shadertoy
- `build.sh` - Build script using binutils
- `Makefile` - Make build file

## Implementation Details

The game was converted from a Shadertoy project, with the GLSL shader code for terrain generation and physics converted to pure C functions. The renderer supports both:
1. Modern OpenGL rendering with shaders
2. Classic software rendering with a frame buffer

The physics system handles gravity, collisions, and ball movement with realistic bouncing and friction.