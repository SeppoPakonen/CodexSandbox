# TASKS.md - Commander Keen Standard C++ and Binutils Build System

## Overview
This project converts the original Commander Keen game from C to modern C++ using standard C++ features and binutils build system. The code avoids smart pointers in favor of RAII principles with raw C-pointers that are zeroed at definition. A single main-header will be used for all cpp files. The project is divided into shared library projects with preparation for modern graphics instead of bitmaps.

## Project Structure
- **stdsrc/** - Main C++ source code following standard C++ and RAII principles
- **stdsrc/core/** - Core engine components (memory, file system, etc.)
- **stdsrc/video/** - Video rendering components with OpenGL backend preparation
- **stdsrc/audio/** - Audio system components
- **stdsrc/input/** - Input handling components
- **stdsrc/game/** - Game logic and mechanics
- **stdsrc/graphics/** - Graphics extraction and management system
- **build.sh** - Build script using standard binutils (gcc/g++/ld/ar)

## Shared Library Architecture
1. **libkeencore.so** - Core engine functionality (memory management, file I/O, timing)
2. **libkeengfx.so** - Graphics and rendering system
3. **libkeenvideo.so** - Video output and OpenGL rendering
4. **libkeenaudio.so** - Audio system and sound playback
5. **libkeeninput.so** - Input handling and event processing
6. **libkeengame.so** - Game logic, objects, and mechanics

## TODO Tasks

### Core Infrastructure (stdsrc/core/)
- [ ] **CORE001** - Create main-header `Keen.h` for all cpp files
- [ ] **CORE002** - Implement RAII memory management with raw C-pointers, zeroed at definition
- [ ] **CORE003** - Convert memory manager from `id_mm.c` to modern C++ with RAII
- [ ] **CORE004** - Create file system abstractions using raw C-pointers with RAII
- [ ] **CORE005** - Implement configuration manager from `id_cfg.c` with RAII principles
- [ ] **CORE006** - Create cross-platform utilities from `ck_cross.c` with RAII
- [ ] **CORE007** - Develop string utilities with RAII and raw pointers (no smart_ptr)
- [ ] **CORE008** - Implement timing system with RAII principles
- [ ] **CORE009** - Create basic application entry point with proper resource cleanup

### Video System (stdsrc/video/)
- [ ] **VID001** - Prepare video hardware helpers from `id_vh.c` for OpenGL rendering
- [ ] **VID002** - Create video layer from `id_vl.c` with modern graphics preparation
- [ ] **VID003** - Design OpenGL renderer interface for modern graphics
- [ ] **VID004** - Implement basic OpenGL context management with RAII
- [ ] **VID005** - Create video backend abstractions for future platform expansion
- [ ] **VID006** - Implement frame management and timing with RAII

### Audio System (stdsrc/audio/)
- [ ] **AUD001** - Implement sound manager from `id_sd.c` with RAII
- [ ] **AUD002** - Create audio device abstractions using raw C-pointers
- [ ] **AUD003** - Design audio backend interface for future implementations
- [ ] **AUD004** - Implement music and sound effect systems with proper cleanup

### Input System (stdsrc/input/)
- [ ] **INP001** - Convert input manager from `id_in.c` to C++ with RAII
- [ ] **INP002** - Create input device abstractions with raw C-pointers
- [ ] **INP003** - Implement keyboard, mouse, and joystick handling
- [ ] **INP004** - Design input event processing with RAII cleanup

### Graphics System (stdsrc/graphics/)
- [ ] **GFX001** - Implement bitmap extraction system from original assets
- [ ] **GFX002** - Create texture management system for OpenGL rendering
- [ ] **GFX003** - Design graphics resource pooling with RAII
- [ ] **GFX004** - Implement sprite and animation systems for modern rendering
- [ ] **GFX005** - Create graphics pipeline for converting bitmaps to OpenGL textures
- [ ] **GFX006** - Implement graphics caching and loading systems with raw pointers

### Game Logic (stdsrc/game/)
- [ ] **GAME001** - Convert action manager from `ck_act.c` to C++ with RAII
- [ ] **GAME002** - Implement game manager from `ck_game.c` with modern C++
- [ ] **GAME003** - Create game interaction system from `ck_inter.c` with RAII
- [ ] **GAME004** - Convert Keen-specific logic from `ck_keen.c` to C++
- [ ] **GAME005** - Implement main game logic from `ck_main.c` with proper RAII
- [ ] **GAME006** - Create map handling from `ck_map.c` using RAII principles
- [ ] **GAME007** - Convert miscellaneous game objects from `ck_misc.c` to C++
- [ ] **GAME008** - Implement object system from `ck_obj.c` with raw pointers
- [ ] **GAME009** - Create physics engine from `ck_phys.c` with RAII
- [ ] **GAME010** - Implement gameplay loop from `ck_play.c` using RAII
- [ ] **GAME011** - Create quit game functions from `ck_quit.c` with proper cleanup
- [ ] **GAME012** - Implement text handling from `ck_text.c` with RAII

### Episode-Specific Content (stdsrc/game/ep4, ep5, ep6/)
- [ ] **EP401** - Convert Episode 4 map functions from `ck4_map.c` to C++
- [ ] **EP402** - Convert Episode 4 misc functions from `ck4_misc.c` to C++
- [ ] **EP403** - Convert Episode 4 objects part 1 from `ck4_obj1.c` to C++
- [ ] **EP404** - Convert Episode 4 objects part 2 from `ck4_obj2.c` to C++
- [ ] **EP405** - Convert Episode 4 objects part 3 from `ck4_obj3.c` to C++
- [ ] **EP501** - Convert Episode 5 map functions from `ck5_map.c` to C++
- [ ] **EP502** - Convert Episode 5 misc functions from `ck5_misc.c` to C++
- [ ] **EP503** - Convert Episode 5 objects part 1 from `ck5_obj1.c` to C++
- [ ] **EP504** - Convert Episode 5 objects part 2 from `ck5_obj2.c` to C++
- [ ] **EP505** - Convert Episode 5 objects part 3 from `ck5_obj3.c` to C++
- [ ] **EP601** - Convert Episode 6 map functions from `ck6_map.c` to C++
- [ ] **EP602** - Convert Episode 6 misc functions from `ck6_misc.c` to C++
- [ ] **EP603** - Convert Episode 6 objects part 1 from `ck6_obj1.c` to C++
- [ ] **EP604** - Convert Episode 6 objects part 2 from `ck6_obj2.c` to C++
- [ ] **EP605** - Convert Episode 6 objects part 3 from `ck6_obj3.c` to C++

### Build System (build.sh)
- [ ] **BUILD001** - Update `build.sh` to compile shared libraries with g++/gcc
- [ ] **BUILD002** - Implement proper linking of shared libraries
- [ ] **BUILD003** - Add dependency tracking and incremental builds
- [ ] **BUILD004** - Implement installation targets for libraries
- [ ] **BUILD005** - Add debugging and optimization build configurations
- [ ] **BUILD006** - Create static analysis and code quality checks

### Headers and Interfaces
- [ ] **HDR001** - Convert `ck_config.h`, `ck_def.h`, `ck_ep.h`, `ck_phys.h`, `ck_play.h`, `ck_text.h` to C++ headers
- [ ] **HDR002** - Convert `id_heads.h`, `id_ca.h`, `id_cfg.h`, `id_fs.h`, `id_in.h`, `id_mm.h`, `id_rf.h`, `id_sd.h`, `id_str.h`, `id_ti.h`, `id_us.h`, `id_vh.h`, `id_vl.h`, `id_vl_private.h` to C++ headers
- [ ] **HDR003** - Convert episode headers `ck4_ep.h`, `ck5_ep.h`, `ck6_ep.h` to C++ headers
- [ ] **HDR004** - Create unified main-header `Keen.h` with proper includes

## RAII and Raw Pointer Guidelines
All C++ code will follow these principles:
- Use raw C-pointers instead of smart pointers
- Initialize all pointer variables to nullptr at definition
- Implement proper destructors for automatic cleanup
- Use RAII (Resource Acquisition Is Initialization) patterns
- Ensure exception safety with proper cleanup in destructors
- Follow consistent naming and memory management patterns

## Modern Graphics Preparation
- Design abstract texture and rendering interfaces
- Implement bitmap extraction tooling for asset preparation
- Create OpenGL texture loading and management systems
- Prepare for shader-based rendering pipeline
- Implement sprite batching for efficient rendering

## Priority Order for Implementation
1. **Core Infrastructure** - Basic framework and main-header (CORE001-CORE009)
2. **Build System** - Update build scripts for shared libraries (BUILD001-BUILD006)
3. **Graphics System** - Asset extraction and OpenGL preparation (GFX001-GFX006)
4. **Game Logic** - Core game systems (GAME001-GAME012)
5. **Video System** - OpenGL rendering implementation (VID001-VID006)
6. **Audio System** - Sound and music implementation (AUD001-AUD004)
7. **Input System** - Control and event handling (INP001-INP004)
8. **Episode-Specific Content** - Game content (EP401-EP605)
9. **Headers and Interfaces** - Complete header conversions (HDR001-HDR004)

## Final Deliverables
- Standard C++ codebase with RAII and raw pointers (no smart_ptr)
- Shared library architecture (libkeencore.so, libkeengfx.so, etc.)
- Single main-header `Keen.h` used across all cpp files
- OpenGL-ready graphics system with bitmap extraction tools
- Proper build system using binutils (gcc/g++/ld/ar)
- Complete game functionality with modern graphics preparation