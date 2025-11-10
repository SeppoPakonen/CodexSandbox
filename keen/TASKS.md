# TASKS.md - Commander Keen C to U++ C++ Conversion

## TODO (Unconverted C Files)

### Core ID Engine Files (Id/ subpackage)
- [ ] **ID001** - Convert `id_ca.c` - Cache Manager (Asset loading and resource management)
- [ ] **ID002** - Convert `id_cfg.c` - Configuration Manager (Already partially done in IdCfg.cpp)  
- [ ] **ID003** - Convert `id_fs.c` - File System (Already partially done in IdFs.cpp)
- [ ] **ID004** - Convert `id_in.c` - Input Manager (Already partially done in IdInput.cpp)
- [ ] **ID005** - Convert `id_mm.c` - Memory Manager (Already partially done in IdMm.cpp)
- [ ] **ID006** - Convert `id_rf.c` - Random/Frame Functions (Already partially done in IdRf.cpp)
- [ ] **ID007** - Convert `id_sd.c` - Sound Manager (Already partially done in IdAudio.cpp)
- [ ] **ID008** - Convert `id_str.c` - String utilities (Already partially done in IdStr.cpp)
- [ ] **ID009** - Convert `id_ti.c` - Time Manager (Already partially done in IdTime.cpp)
- [ ] **ID010** - Convert `id_vh.c` - Video Hardware helpers (Already partially done in IdVh.cpp)
- [ ] **ID011** - Convert `id_vl.c` - Video Layer (Already partially done in IdVideo.cpp)
- [ ] **ID012** - Convert `id_us_1.c` - User Interface 1
- [ ] **ID013** - Convert `id_us_2.c` - User Interface 2
- [ ] **ID014** - Convert `id_us_textscreen.c` - Text screen interface
- [ ] **ID015** - Convert `id_in_dos.c` - DOS Input Backend (Platform-specific, not needed)
- [ ] **ID016** - Convert `id_in_null.c` - Null Input Backend (Already done as base class)
- [ ] **ID017** - Convert `id_in_sdl.c` - SDL Input Backend (Platform-specific, not needed)
- [ ] **ID018** - Convert `id_in_sdl3.c` - SDL3 Input Backend (Platform-specific, not needed)
- [ ] **ID019** - Convert `id_sd_dos.c` - DOS Sound Backend (Platform-specific, not needed)
- [ ] **ID020** - Convert `id_sd_liboplhw.c` - Hardware OPL Backend (Platform-specific, not needed)
- [ ] **ID021** - Convert `id_sd_null.c` - Null Sound Backend (Platform-specific, not needed)
- [ ] **ID022** - Convert `id_sd_opl2.c` - OPL2 Sound Backend (Platform-specific, not needed)
- [ ] **ID023** - Convert `id_sd_opl2alsa.c` - OPL2 ALSA Backend (Platform-specific, not needed)
- [ ] **ID024** - Convert `id_sd_opl2lpt.c` - OPL2 LPT Backend (Platform-specific, not needed)
- [ ] **ID025** - Convert `id_sd_sdl.c` - SDL Sound Backend (Platform-specific, not needed)
- [ ] **ID026** - Convert `id_sd_sdl3.c` - SDL3 Sound Backend (Platform-specific, not needed)
- [ ] **ID027** - Convert `id_vl_dos.c` - DOS Video Backend (Platform-specific, not needed)
- [ ] **ID028** - Convert `id_vl_null.c` - Null Video Backend (Already done as base class)
- [ ] **ID029** - Convert `id_vl_sdl12.c` - SDL12 Video Backend (Platform-specific, not needed)
- [ ] **ID030** - Convert `id_vl_sdl2.c` - SDL2 Video Backend (Platform-specific, not needed)
- [ ] **ID031** - Convert `id_vl_sdl2gl.c` - SDL2 GL Video Backend (Platform-specific, not needed)
- [ ] **ID032** - Convert `id_vl_sdl2vk.c` - SDL2 Vulkan Video Backend (Platform-specific, not needed)
- [ ] **ID033** - Convert `id_vl_sdl3.c` - SDL3 Video Backend (Platform-specific, not needed)
- [ ] **ID034** - Convert `id_vl_sdl3gpu.c` - SDL3 GPU Video Backend (Platform-specific, not needed)

### Game Logic Files (Game/ subpackage)
- [x] **GAME001** - Convert `ck_act.c` - Action Manager (Actor animations and behaviors)
- [x] **GAME002** - Convert `ck_cross.c` - Cross-platform utilities
- [x] **GAME003** - Convert `ck_game.c` - Game Manager (Main game loop, save/load, level loading) - PRIORITY
- [x] **GAME004** - Convert `ck_inter.c` - Game Interactions (Title screen, help screens)
- [x] **GAME005** - Convert `ck_keen.c` - Keen-specific game logic
- [x] **GAME006** - Convert `ck_main.c` - Main entry point (Entry point and initialization) - PRIORITY
- [x] **GAME007** - Convert `ck_map.c` - Map handling and spawning
- [x] **GAME008** - Convert `ck_misc.c` - Miscellaneous game objects
- [x] **GAME009** - Convert `ck_obj.c` - Object spawning and utilities
- [x] **GAME010** - Convert `ck_phys.c` - Physics engine
- [x] **GAME011** - Convert `ck_play.c` - Play loop (Core gameplay loop, collision, object processing) - PRIORITY
- [x] **GAME012** - Convert `ck_quit.c` - Quit game functions
- [x] **GAME013** - Convert `ck_text.c` - Text handling and UI

### Episode 4 Specific Files (Game/ subpackage)
- [ ] **EP401** - Convert `ck4_map.c` - Episode 4 specific map functions
- [ ] **EP402** - Convert `ck4_misc.c` - Episode 4 specific miscellaneous functions
- [ ] **EP403** - Convert `ck4_obj1.c` - Episode 4 objects part 1
- [ ] **EP404** - Convert `ck4_obj2.c` - Episode 4 objects part 2
- [ ] **EP405** - Convert `ck4_obj3.c` - Episode 4 objects part 3

### Episode 5 Specific Files (Game/ subpackage)
- [ ] **EP501** - Convert `ck5_map.c` - Episode 5 specific map functions
- [ ] **EP502** - Convert `ck5_misc.c` - Episode 5 specific miscellaneous functions
- [ ] **EP503** - Convert `ck5_obj1.c` - Episode 5 objects part 1
- [ ] **EP504** - Convert `ck5_obj2.c` - Episode 5 objects part 2
- [ ] **EP505** - Convert `ck5_obj3.c` - Episode 5 objects part 3

### Episode 6 Specific Files (Game/ subpackage)
- [ ] **EP601** - Convert `ck6_map.c` - Episode 6 specific map functions
- [ ] **EP602** - Convert `ck6_misc.c` - Episode 6 specific miscellaneous functions
- [ ] **EP603** - Convert `ck6_obj1.c` - Episode 6 objects part 1
- [ ] **EP604** - Convert `ck6_obj2.c` - Episode 6 objects part 2
- [ ] **EP605** - Convert `ck6_obj3.c` - Episode 6 objects part 3

### OPL Emulator Files (Opl/ subpackage)
- [ ] **OPL001** - Convert `opl/dbopl.c` - DOSBox OPL emulator
- [ ] **OPL002** - Convert `opl/nuked_opl3.c` - Nuked OPL3 emulator

### Other Files
- [ ] **MISC001** - Convert `icon.c` - Application icon (Platform-specific)
- [ ] **MISC002** - Handle `ck_config.h`, `ck_def.h`, `ck_ep.h`, `ck_phys.h`, `ck_play.h`, `ck_text.h` - Headers that need U++ adaptation
- [ ] **MISC003** - Handle `id_heads.h`, `id_ca.h`, `id_cfg.h`, `id_fs.h`, `id_in.h`, `id_mm.h`, `id_rf.h`, `id_sd.h`, `id_str.h`, `id_ti.h`, `id_us.h`, `id_vh.h`, `id_vl.h`, `id_vl_private.h` - Headers that need U++ adaptation
- [ ] **MISC004** - Handle episode headers `ck4_ep.h`, `ck5_ep.h`, `ck6_ep.h` - Episode-specific headers
- [ ] **MISC005** - Handle `ck_cross.h`, `ck_act.h` - Additional headers

## DONE (Completed Conversions)

### ID Engine Components
- [x] **ID002** - `id_cfg.c` → `IdCfg.cpp` - Configuration Manager
- [x] **ID003** - `id_fs.c` → `IdFs.cpp` - File System Manager  
- [x] **ID004** - `id_in.c` → `IdInput.cpp` - Input Manager
- [x] **ID005** - `id_mm.c` → `IdMm.cpp` - Memory Manager
- [x] **ID006** - `id_rf.c` → `IdRf.cpp` - Random/Frame Functions
- [x] **ID007** - `id_sd.c` → `IdAudio.cpp` - Sound Manager
- [x] **ID008** - `id_str.c` → `IdStr.cpp` - String utilities
- [x] **ID009** - `id_ti.c` → `IdTime.cpp` - Time Manager
- [x] **ID010** - `id_vh.c` → `IdVh.cpp` - Video Hardware helpers
- [x] **ID011** - `id_vl.c` → `IdVideo.cpp` - Video Layer
- [x] **ID012** - `id_us_1.c` → Integrated in U++ fashion

### Game Components
- [x] **GAME003** - Basic Game bootstrap → `Game.cpp` - Initial game bootstrap with basic functionality
- [x] **GAME004** - HUD system → `Hud.cpp` - Player status display (score, lives, etc.)
- [x] **GAME005** - Game loop → `Loop.cpp` - Demo game loop implementation
- [x] **GAME006** - Game map → `Map.cpp` - Map loading and management
- [x] **GAME007** - Game state → `State.cpp` - Game state management
- [x] **GAME008** - Game title menu → `TitleMenu.cpp` - Title menu functionality
- [x] **GAME009** - Player logic → `Player.cpp` - Player movement and controls
- [x] **GAME010** - Game assets → `Assets.cpp` - Asset loading utilities
- [x] **GAME011** - Game resources → `Resources.cpp` - Resource management
- [x] **GAME012** - Core gameplay → `Game.cpp` - Bootstrap and entry point
- [x] **GAME013** - Convert `ck_game.c` → `Game/Game.cpp` - Game Manager (Main game loop, save/load, level loading) - PRIORITY
- [x] **GAME014** - Convert `ck_main.c` → `Game/Main.cpp` - Main entry point (Entry point and initialization) - PRIORITY
- [x] **GAME015** - Convert `ck_play.c` → `Game/Play.cpp` - Play loop (Core gameplay loop, collision, object processing) - PRIORITY
- [x] **GAME016** - Convert `ck_phys.c` → `Game/Physics.cpp` - Physics engine and clip system

### OPL Components
- [x] **OPL001** - OPL emulator → `Opl.cpp` - OPL audio emulation interface

### Package Structure
- [x] **PKG001** - Created Keen root package with `Keen.h` and `Keen.cpp`
- [x] **PKG002** - Created Keen/Id subpackage with main header `Id.h`
- [x] **PKG003** - Created Keen/Game subpackage with main header `Game.h`
- [x] **PKG004** - Created Keen/Opl subpackage with main header `Opl.h`

### Core Infrastructure
- [x] **INFRA001** - U++ namespace wrapping with `NAMESPACE_UPP` and proper headers
- [x] **INFRA002** - File system abstractions with `FS::File` class
- [x] **INFRA003** - Video abstractions with `Video::Screen` class
- [x] **INFRA004** - Audio abstractions with `Audio::Device` class
- [x] **INFRA005** - Input abstractions with `Input::Device` class
- [x] **INFRA006** - Timing abstractions with `Time::Clock` class
- [x] **INFRA007** - IO helpers with `FS::Reader` and `FS::Writer` classes
- [x] **INFRA008** - String utilities with `Str::Parser`, `Str::Token`, `Str::Table`

## Priority Order for Remaining Tasks
All main game logic C to C++ conversions have been completed!
1. All main game logic files (ck_act.c through ck_text.c) have been converted
2. All episode-specific files (4, 5, 6) should be converted in dependency order