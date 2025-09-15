# Game Package Split Plan (Ultimate++ Packages)

This document proposes a modular split of the current monolithic `Game` package into multiple cohesive Ultimate++ subpackages (.upp) under `Game/`, to reduce build times, improve maintainability, and clarify dependencies. We avoid the `Game*` prefix in package names; instead, place each package under `uppsrc/Game/<Subpkg>/<Subpkg>.upp` (e.g., `uppsrc/Game/Graphics/Graphics.upp`).

## Goals

- Reduce compile time and link size via cohesion.
- Layer packages to avoid cycles; keep a small, stable core.
- Preserve existing external deps (Dolphin, SimpleMedia, Socket, Graphics).

## Layering Overview

Top depends on bottom. Avoid upward/circular dependencies.

1. Core
2. IO
3. Assets
4. Graphics / Audio
5. World
6. Physics
7. Components
8. ComponentsSkater / ComponentsVehicle
9. UI / Net / Modes
10. Platform (orthogonal glue) / All (aggregator)

Scripting sits near Assets/World; used by Modes and selective components.

## Proposed Packages (as subpackages of `Game`)

### Core
- Purpose: Platform-agnostic core types, containers, math, memory, and utilities.
- Contents: Atomic, Array, String, CString, Vector/Vector2, Matrix/Matrix2, Quat/Quat2, Rect, BBox, Crc, HashId, HashTable, Flags, Handle, ObjPtr, Lock, Thread, Timer, Pool/Poolable/PoolManager, Heap, Alloc, Mem*/MemPtr, Defines, Macros, Support/Support2, Checks, Assert/Debug, Profiler/Profile, TimestampedFlag, CompactPool, VectorPair, Types, System.
- Uses: none (keep Dolphin out if possible).

### IO
- Purpose: File and async I/O, compression, DMA/ARam, config.
- Contents: File/File2, AsyncFileSys/AsyncFilesys2, AsyncTypes, Dma, ARam, EzConfig/EzCommon/LibEzConfig, MemDebug, MemView.
- Uses: Game/Core, Dolphin.

### Assets
- Purpose: Asset definitions and managers, animation assets.
- Contents: Asset/RefAsset, AssetTypes, AssMan, ImageBasic, Anim/AnimAsset/AnimController/BaseAnimCtrl/SubAnimCtrl, Skeleton/BonedAnimTypes, Movies/MovieDetails, Texture/NxTexture, Material/Material2/MatMan, Model/ModelAppearance, Tex/TexMan, GfxMan/GfxUtils.
- Uses: Game/Core, Game/IO, Dolphin.

### Graphics
- Purpose: Rendering and the `Nx*` subsystem.
- Contents: Render/Render2, Scene/Scene2, Camera/Camera2, Light/Light2/NxLight*, Viewport/NxViewport*, Nx*, NxModel*, NxMesh*, NxGeom*, NxParticle*, NxImposter*, NxLoadScreen*, DebugGfx, VectorFont, Screenshot, Occlude, Prim/Triangle.
- Uses: Game/Core, Game/Assets, Dolphin, Graphics.
- Notes: Likely `noblitz;` (heavy).

### Audio
- Purpose: Audio integration and SFX/music.
- Contents: AUDSimpleAudio, AUDSimplePlayer, SoundFx, SoundComponent, Sounds, Music/Music2.
- Uses: Game/Core, SimpleMedia, Dolphin.

### Scripting
- Purpose: Scripting engine: tokens, symbols, runtime, native bindings.
- Contents: Script, ScriptCache, ScriptDebugger, Tokens, SymbolType, SymbolTable, CFuncs, FTables, McFuncs, SkFuncs, Import, Content.
- Uses: Game/Core, Game/Assets.
- Notes: Avoid dependence on Components to prevent cycles.

### World
- Purpose: World state, pathing, rails, triggers/regions.
- Contents: Region, PathMan, Rail/RailManager/RailEditor/EdRail, Proxim/ProximTrigger, Terrain, Records, Restart, Scene2 (world variant).
- Uses: Game/Core, Game/Assets, Game/Graphics.

### Physics
- Purpose: Collisions and physics helpers.
- Contents: Collision/Collision2/CollCache/CollEnums/CollTriData, Contact, Feeler/RectFeeler, MovingObject, RigidBodyComponent, CarPhysicsComponent, ProjectileCollisionComponent, Near/Avoid, Walk*Utils.
- Uses: Game/Core, Game/World.

### Components
- Purpose: Entity/component system and general-purpose components.
- Contents: BaseComponent, Component, Object/ObjectManager, CompositeObject, ObjectHook/ObjectHookManager, Module/Module2, Task/Task2, MotionComponent, Stream/SetDisplayMatrix/NodeArray/Near/RailManager/Empty.
- Uses: Game/Core, Game/World, Game/Graphics, Game/Audio, Game/Physics.

### ComponentsSkater
- Purpose: Skater-specific components and logic.
- Contents: Skater*, SkaterCam/SkaterCameraComponent/WalkCameraComponent/CameraLookAround, SkaterPhysics*/SkaterAdjust*/Finalize*/Proximity, SkaterTricks, SkaterScore/RunTimer/StancePanel/FloatingName/LoopingSound/Sound, SkaterProfile/Career/Pad.
- Uses: Game/Components, Game/Audio, Game/Physics, Game/World, Game/Scripting.

### ComponentsVehicle
- Purpose: Vehicle-specific components and logic.
- Contents: VehicleComponent, VehicleCameraComponent, VehicleSoundComponent, SkitchComponent.
- Uses: Game/Components, Game/Audio, Game/Physics.

### UI
- Purpose: UI/Frontend, menus, screen elements, viewers, window mgmt.
- Contents: FrontEnd, Menu2/ScrollingMenu, ScreenElement2/ScreenElemMan, Viewer/ViewerObj, VectorFont, Window, Win32Functions, ModelBuilder.
- Uses: Game/Core, Game/Graphics, Game/Components.

### Net
- Purpose: Networking stack and gameplay integration.
- Contents: Net*, NetServer/NetClnt/NetDispatch/NetConnect/NetApp/NetConfig, Lobby, ServerList, NetGoal.
- Uses: Game/Core, Socket, Dolphin.
- Notes: Keep gameplay-specific hooks at edges to avoid pulling in Components.

### Modes
- Purpose: Game flow and goal systems.
- Contents: GameFlow, GameMode, Competition, Goals (FindGapsGoal, HorseGoal, RaceGoal, CATGoal, Minigame, NetGoal), Skate.h, Crown, BettingGuy, Horse, VictoryCond.
- Uses: Game/Core, Game/Scripting, Game/World, Game/Components, Game/UI.

### Platform
- Purpose: Platform integration and low-level I/O not in IO.
- Contents: SioDev/SioMan, Gx.h bridge if needed, Win32 utils.
- Uses: Game/Core, Dolphin.

### All (optional aggregator)
- Purpose: Transitional package that `uses` all subpackages for compatibility.
- Uses: all of the above.

## .upp Skeletons (examples)

Game/Core/Core.upp

```upp
noblitz;

file
	Atomic.h,
	Atomic.cpp,
	Array.h,
	Array.cpp,
	String.h,
	String.cpp,
	Vector.h,
	Vector.inl,
	Vector2.h,
	Matrix.h,
	Matrix.inl,
	Matrix2.h,
	Quat.h,
	Quat.inl,
	Quat2.h,
	Rect.h,
	BBox.h,
	BBox.cpp,
	Crc.h,
	HashId.h,
	HashId.cpp,
	HashTable.h,
	Flags.h,
	Handle.h,
	ObjPtr.h,
	Lock.h,
	Thread.h,
	Thread.cpp,
	Timer.h,
	Pool.h,
	Pool.cpp,
	Poolable.h,
	Poolable.cpp,
	PoolManager.h,
	PoolManager.cpp,
	Heap.h,
	Alloc.h,
	MemPtr.h,
	Mem_stat.h,
	MemView.h,
	MemDebug.h,
	Checks.h,
	Assert.h,
	Assert.cpp,
	Debug.h,
	Profiler.h,
	Profiler.cpp,
	Profile.h,
	Profile.cpp,
	TimestampedFlag.h,
	Defines.h,
	Macros.h,
	Support.h,
	Support2.h,
	Types.h,
	System.h,
	System.cpp;
```

Game/Graphics/Graphics.upp

```upp
noblitz;

uses
	Dolphin,
	Graphics,
	Game/Core,
	Game/Assets;

file
	Render.h,
	Render.cpp,
	Render2.h,
	Render2.cpp,
	Scene.h,
	Scene.cpp,
	Scene2.h,
	Scene2.cpp,
	Camera.h,
	Camera.cpp,
	Camera2.h,
	Camera2.cpp,
	Light.h,
	Light.cpp,
	Light2.h,
	Light2.cpp,
	Nx.h,
	Nx.cpp,
	Nx2.cpp,
	Nx* (other Nx*.h/.cpp files),
	NxViewport.h,
	NxViewport.cpp,
	NxViewport2.h,
	NxViewport2.cpp,
	NxLoadScreen.h,
	NxLoadScreen.cpp,
	VectorFont.h,
	VectorFont.cpp,
	Screenshot.h,
	Screenshot.cpp,
	Occlude.h,
	Occlude.cpp,
	Prim.h,
	Prim.cpp,
	Triangle.h,
	Triangle.cpp;
```

Game/Audio/Audio.upp

```upp
uses
	Dolphin,
	SimpleMedia,
	Game/Core;

file
	AUDSimpleAudio.h,
	AUDSimpleAudio.cpp,
	AUDSimplePlayer.h,
	AUDSimplePlayer.cpp,
	SoundFx.h,
	SoundFx.cpp,
	SoundComponent.h,
	SoundComponent.cpp,
	Sounds.h,
	Sounds.cpp,
	Music.h,
	Music.cpp,
	Music2.h,
	Music2.cpp;
```

Game/Scripting/Scripting.upp

```upp
uses
	Game/Core,
	Game/Assets;

file
	Script.h,
	Script.cpp,
	ScriptCache.h,
	ScriptCache.cpp,
	ScriptDebugger.h,
	Tokens.h,
	Tokens.cpp,
	SymbolType.h,
	SymbolType.cpp,
	SymbolTable.h,
	SymbolTable.cpp,
	CFuncs.h,
	CFuncs.cpp,
	FTables.h,
	FTables.cpp,
	McFuncs.h,
	McFuncs.cpp,
	SkFuncs.h,
	SkFuncs.cpp,
	Import.h,
	Import.cpp,
	Content.h,
	Content.cpp;
```

Game/Net/Net.upp

```upp
uses
	Dolphin,
	Socket,
	Game/Core;

file
	Net.h,
	Net.cpp,
	NetServer.h,
	NetServer.cpp,
	NetClnt.h,
	NetClnt.cpp,
	NetDispatch.cpp,
	NetConnect.cpp,
	NetApp.cpp,
	NetConfigIf.h,
	NetConfig.cpp,
	Lobby.h,
	Lobby.cpp,
	ServerList.cpp,
	NetGoal.h,
	NetGoal.cpp;
```

Create similar .upps for: Game/World, Game/Physics, Game/Components, Game/ComponentsSkater, Game/ComponentsVehicle, Game/UI, Game/Modes, Game/Platform, and Game/All (aggregator that only has a `uses` section).

## Incremental Migration Steps

1. Establish base:
   - Create Game/Core with files that have zero external deps; adjust includes to package-qualified forms (e.g., `#include <Game/Core/Vector.h>`).
2. Extract I/O and assets:
   - Move File/Async/Compression/ARam to Game/IO; Anim/Asset/Mat/Tex/Model managers to Game/Assets. Fix includes.
3. Split rendering and audio:
   - Move Nx*/Render/Scene/Viewport/VectorFont/Screenshot to Game/Graphics; AUD*/Music/Sounds to Game/Audio. Mark heavy packages `noblitz;`.
4. Move scripting:
   - Extract the script system into Game/Scripting; ensure no dependency on Components (use interfaces/events if needed).
5. World and physics:
   - Move Rail/PathMan/Proxim/Region/Terrain to Game/World; Collision/Contact/Feeler/Physics to Game/Physics.
6. Components:
   - Create Game/Components (base ECS), then Game/ComponentsSkater and Game/ComponentsVehicle. Make features depend on Game/Components and specializations.
7. UI and modes:
   - Move FrontEnd/Menu/ScreenElement*/Viewer/Window into Game/UI; GameFlow/GameMode/Goals into Game/Modes.
8. Networking and platform:
   - Move Net*/Lobby/ServerList to Game/Net; Sio*/Win32Functions to Game/Platform.
9. Aggregator:
   - Add Game/All.upp with `uses` referencing all new packages for transitional builds.
10. Clean up Game:
   - Deprecate monolithic `Game` or keep a thin façade that `uses` the new packages and re-exports umbrella headers.

## Dependency Rules

- Never depend upward; keep linear layering to avoid cycles.
- Keep Scripting free of Components; bridge via callbacks/interfaces.
- Keep Core free of Dolphin; if unavoidable, wrap via minimal headers.

## Practical Notes

- Include hygiene: migrate to package-qualified includes to avoid collisions (e.g., duplicate `List.h`). For subpackages, prefer includes like `#include <Game/Core/Vector.h>` from dependents.
- Build flags: retain `noblitz;` for very large packages; small ones can use blitz.
- Options: propagate `options(BUILDER_OPTION) NOWARNINGS` where needed per package.
- Naming: subpackages live under `uppsrc/Game/<Subpkg>/` and contain `<Subpkg>.upp` and sources; avoid `Game*`-prefixed package names.
- Validation: after each extraction, build dependents to catch missing includes early.

