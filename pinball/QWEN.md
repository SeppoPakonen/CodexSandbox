# Qwen AI Assistant Memory File

This file is used to store persistent information and preferences for the Qwen AI Assistant session.

## Project Overview
- Project: C++ 3D Game Conversion from Shadertoy
- Shadertoy Options: MdyGWG.json (simpler shader) and MdyGDz.json (more complex shader)
- Goal: Implement both shaders as selectable renderers in the same 3D game engine
- Approach: Create a flexible renderer system that can switch between both shaders

## Key Tasks
- Converting GLSL shader code to work with standard OpenGL pipeline
- Setting up 3D rendering engine with camera, lighting, and geometry systems
- Mapping Shadertoy uniforms (iTime, iResolution, iMouse, etc.) to game equivalents
- Implementing game mechanics that incorporate the visual effects from the shader
- Ensuring performance and cross-platform compatibility

## Technical Stack
- C++ programming language
- OpenGL for graphics rendering
- GLFW for window management
- GLM for mathematics
- CMake for build system
- Possible audio libraries if needed (OpenAL, etc.)

## Project Structure
- src/: Source code files
- include/: Header files
- assets/: Game assets (textures, models, audio)
- shaders/: GLSL shader files
- cmake/: CMake modules

## User Preferences
- User is working on Linux OS
- Current project directory: /home/sblo/Dev/Pinball
- Previous work with shaders and 3D graphics

## Project Status
- Initial planning phase
- Tasks documented in tasks.md
- Ready to begin implementation phase