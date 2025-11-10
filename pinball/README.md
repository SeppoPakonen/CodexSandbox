# Pinball Game with Three Renderer Systems

This project implements a pinball game with support for three different rendering systems:

1. **Simple Shader Renderer** - Based on the MdyGWG Shadertoy shader
2. **Advanced Shader Renderer** - Based on the more complex MdyGDz Shadertoy shader
3. **Simple Fixed-Function Renderer** - Old-school OpenGL using fixed-function pipeline with C++ physics

## Features

- Three renderer system allowing real-time switching between visual styles
- Full OpenGL 3.3+ based rendering for shader modes
- Old-school OpenGL fixed-function pipeline for lightweight rendering
- Proper conversion of Shadertoy shaders to standard OpenGL fragment shaders
- Support for all Shadertoy uniforms (iTime, iResolution, iMouse, iFrame, iDate)
- Interactive 3D pinball gameplay with C++ physics simulation
- Camera system with 3D view controls

## Project Structure

```
Pinball/
├── CMakeLists.txt              # Build configuration
├── src/                        # Source code files
│   ├── main.cpp                # Application entry point
│   ├── game.cpp                # Game logic and main loop
│   ├── renderer.cpp            # Shader-based renderer implementation
│   ├── simple_renderer.cpp     # Fixed-function pipeline renderer
│   ├── shader.cpp              # Shader loading and compilation
│   ├── camera.cpp              # Camera implementation
│   ├── input.cpp               # Input handling
│   ├── ball.cpp                # Ball physics
│   └── paddle.cpp              # Paddle controls
├── include/                    # Header files
├── shaders/                    # Converted GLSL shaders
│   ├── screen_quad.vert        # Fullscreen quad vertex shader
│   ├── simple_pinball.frag     # Converted MdyGWG shader
│   ├── advanced_pinball.frag   # Converted MdyGDz shader
│   └── various extracted shaders
└── build/                      # Build directory
```

## Building

1. Create build directory:
   ```bash
   mkdir build && cd build
   ```

2. Generate build files:
   ```bash
   cmake ..
   ```

3. Compile the project:
   ```bash
   make
   ```

4. Run the game:
   ```bash
   ./pinball_game
   ```

## Running

**Important**: This application requires a display environment (GUI) to run, as it uses OpenGL for rendering. 
If you get an error about XDG_RUNTIME_DIR or GLFW initialization, you're likely in a headless environment.
To run on a server, you may need:
- An X server running
- Forward X11 connections (ssh -X)
- A virtual framebuffer like Xvfb

## Controls

- `ESC` - Exit the game
- `R` - Cycle between Simple Shader, Advanced Shader, and Fixed-Function renderers
- Arrow keys - Control the paddles (in game)

## Technical Details

The project successfully converts the original Shadertoy shaders:
- MdyGWG.json (simpler 2D pinball simulation)
- MdyGDz.json (advanced 3D raymarched pinball with lighting & reflections)

It also includes a new lightweight renderer using fixed-function OpenGL pipeline with complete C++ physics simulation for better performance and simplicity.