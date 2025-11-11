# City Game

A C implementation of the Shadertoy "City Game II" simulation with both old school and modern rendering approaches.

## Overview

This project implements a city-building simulation based on the Shadertoy project "City Game II" by kastorp. It features:

- City terrain generation with roads, buildings, roundabouts, semaphores, and overpasses
- Vehicle physics with pathfinding and traffic rules
- Dual rendering system (old school software rendering and modern OpenGL shader rendering)
- Game mechanics for connecting roads to the city center and managing traffic

## Features

- Procedural city generation with terrain height mapping
- Vehicle simulation with realistic traffic behavior
- Road connection logic and roundabout/semaphore management
- Building placement and city planning mechanics
- Dual rendering modes for comparison

## Build Requirements

- GCC compiler
- GNU Make
- Math library (libm)

## Build Instructions

```bash
make
```

This will compile the project and create the executable at `bin/citygame`.

## Run Instructions

```bash
./bin/citygame
```

The game will run for 10 seconds (100 frames at 10 FPS) and then exit automatically.

## Architecture

- `include/types.h` - Basic data types and structures
- `include/physics.h` - Physics and vehicle simulation
- `include/renderer.h` - Rendering system interface
- `include/game.h` - Game mechanics and state management
- `src/` - Implementation files
- `shaders/` - GLSL shader files for modern renderer

## Renderers

The project includes two rendering systems:
1. **Old School Renderer**: Software-based rendering using basic drawing functions
2. **Modern Renderer**: OpenGL-based rendering with custom shaders (implemented with placeholders in this simplified version)

## Controls

In the full version (with SDL support), controls would include:
- Mouse click/drag: Place selected block type
- Keyboard: Select block types, reset game, view mode toggle
- Space: Reset map and show demo mode
- Z: Switch between 2D and isometric views

## License

This project is created for educational purposes.