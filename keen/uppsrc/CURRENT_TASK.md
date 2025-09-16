Commander Keen (Omnispeak C) → U++ C++ Port

Scope
- Convert C sources in `sandbox/keen/c` into U++-style C++ packages under `sandbox/keen/uppsrc`.
- Follow AGENTS and Header Include Policy: each package has a main header, implementation files include only that header first, use `NAMESPACE_UPP`.
- Start with core utilities and compile incrementally; keep platform backends stubbed where needed.

Thinking Log (design/state)
- We will progressively introduce C++ classes that wrap legacy C APIs, without breaking call sites yet. Pattern: keep C entry points for now, add thin class wrappers with RAII and methods. New code uses classes; legacy ports can be gradually migrated.
- Filesystem refactor:
  - Added `namespace FS { class File }` with `Read/Write/GetSize/PrintF/Close` and static `OpenUser/CreateUser`.
  - Removed C APIs: `FS_Read`, `FS_OpenUserFile`, `FS_CreateUserFile`, `FS_CloseFile`, `FS_PrintF`, `FS_GetFileSize`, `FS_IsFileValid` from our C++ port; replaced with native `FS::File` methods.
  - Introduced `FS::ReadAllUser(const char*, mm_ptr_t*, int*)` helper for loading whole files; updated config loader to use it.
- Str wrappers scaffolded: `Str::Parser`, `Str::Token`, `Str::Table` thin wrappers over `STR_*` in `IdStr.h`.
- Config refactor: Introduced `Config::Store` with typed get/set and load/save; existing `CFG_*` functions now forward to the singleton for compatibility.
- Video facade: Added `Video::Screen` with `Startup/Shutdown` and no-op `Clear/Present` in place of previous `VL_*` null stubs.
- Input facade: Added `Input::Device` with `Startup/Shutdown/Poll`, basic key state; replaces `IN_*` stubs in this port.
- Audio facade: Added `Audio::Device` with `Startup/Shutdown` and basic mode/sound methods; replaces `SD_*` stubs in this port.
- Video primitives: `DrawRect`, `Blit`, `BlitPAL8`, `SetPalette`, and tiny diagnostics `DrawText5x7` (digits + `:`) on `Video::Screen`.
 - Asset helper: `Assets::LoadUserImageARGB32` loads a trivial ARGB image and `Game::Bootstrap()` blits it if present.
 - Sub-rect blits: `BlitSubrect` and `BlitPAL8SubrectMask` added for sprite sheets and tiles.
 - Sprite queue: added `Video::Queue` + `Sprite` to batch blits (currently used indirectly via resources).
- Resources: `Game::Resources` with `LoadTilesPal8` and `DrawTile()` leveraging `FS::Reader` and `Video::Screen`.
 - Video buffering: `Video::Screen` now supports ring buffers (SetNumBuffers, GetActiveBufferId, SwapOnNextPresent).
 - RF integration: `RF::State` has buffer tracking and switching; loop marks dirty tiles around player and switches buffers each present.
- Game loop: `Game::RunDemoLoop` advances input/state/player with `Time::FrameRate`, renders overlay with state + ticks + pos.
- HUD: Minimal `Game::Hud` that tracks score, shots, and a blink indicator; integrates with input (Fire pressed) and draws textual overlay. Added tests.
 - Demo mode: `Game::InjectDemo` injects Jump pulses and alternates Left/Right movement periodically; loop calls it when demo flag is on. Added auto-demo tests (jump and movement).
- Timing facade: Added `Time::Clock` with `DelayMs()` and `NowMs()`; will be used by gameplay loop and tick logic.
- Binary IO helpers: Added `FS::Reader` and `FS::Writer` with `Read/Write(Int8/Int16LE/Int32LE/Bool16LE)` to replace FS_ReadIntXX/Write paths during future migrations.

Source Survey (from sandbox/keen/c)
- Core engine (id_*): asset/cache (`id_ca.*`), config (`id_cfg.*`), filesystem (`id_fs.*`), input (`id_in.*` + backends: dos/null/sdl/sdl3), memory manager (`id_mm.*`), renderer/viewport (`id_vl.*` + backends: dos/null/sdl12/sdl2/sdl2gl/sdl2vk/sdl3/sdl3gpu + vk shaders), video helpers (`id_vh.*`), sound (`id_sd.*` + backends: dos/null/opl2/opl2alsa/opl2lpt/sdl/sdl3/liboplhw), random/file (`id_rf.*`), strings/tokenizer (`id_str.*`), UI/system (`id_us.*`, `id_us_textscreen.c`), timing (`id_ti.*`).
- Game logic (ck_*): entry/main (`ck_main.c`), orchestrator (`ck_keen.c`, `ck_game.*`, `ck_play.*`), actors/physics/map (`ck_act.*`, `ck_phys.*`, `ck_map.*`, `ck_obj.*`, `ck_misc.*`, `ck_inter.c`, `ck_text.*`, `ck_quit.c`).
- Episode specifics: `ck4_*`, `ck5_*`, `ck6_*` buckets.
- OPL emulation: `opl/dbopl.*`, `opl/nuked_opl3.*` and license.

U++ Package Plan (subpackage independence)
- Keen (root package)
  - Purpose: Aggregate public headers; minimal glue. Main header: `Keen.h` (wraps subpackage headers under `NAMESPACE_UPP`).
  - Depends on: Keen/Id, Keen/Game.
- Keen/Id
  - Ports `id_*` modules except platform backends. Split platform backends behind interfaces; initial stubs: Null backends to get compiling.
  - Main header: `Id.h` aggregates internal headers (`IdCa.h`, `IdCfg.h`, `IdFs.h`, `IdIn.h`, `IdMm.h`, `IdTi.h`, `IdUs.h`, `IdVl.h`, `IdVh.h`, `IdSd.h`, `IdStr.h`, `IdRf.h`).
- Keen/Game
  - Ports `ck_*` (core game, maps, physics, text) and episode chunks (`ck4_*`, `ck5_*`, `ck6_*`).
  - Main header: `Game.h` aggregates game headers; episode headers grouped per episode.
- Keen/Opl
  - Wraps OPL emulators behind a simple interface used by `Id/Sound`. Initial: compile `dbopl` path only; add `nuked_opl3` once the core builds.

Header/Source Conventions (required)
- Every implementation file starts with only its package’s main header, e.g. `#include "Id.h"` then rare file-specific includes.
- Only main headers include other headers; non-main headers avoid `#include` except truly inline companions.
- Wrap public headers in `NAMESPACE_UPP` via `Keen.h`, `Id.h`, `Game.h`, `Opl.h`.
- Prefer U++ types: `String`, `Vector`, `Array`, `VectorMap`, `Index`, `uint8/16/32`, etc. Avoid raw malloc/free; use RAII.
- Keep C API shapes temporarily when useful for a clean compile; refactor to idiomatic C++ as a second pass.

Initial Port Targets (build nucleus first)
1) id_str: tokenizer + small string table. Swap ad‑hoc table for `Index<String>` or keep original API with safer wrappers. Keep void* mapping for now.
2) id_mm: Replace custom allocator with thin wrappers to U++ containers and `UniquePtr`; keep API surface to unblock users.
3) id_cfg/id_fs: File and config reading using U++ `FileIn`, `LoadFile`, `ParseCmdLine` style utilities (scoped wrappers to match expected functions).
4) id_vl (null) + id_sd (null) + id_in (null): Provide compile-time selectable null backends to remove SDL dependency initially.
5) Wire `id_us`, `id_ti`, `id_rf` as needed by the parser and core game init.
6) ck_main/ck_game minimal loop behind a feature flag, no rendering/audio at first.

Porting Approach
- Phase A: Scaffolding
  - Create packages: `Keen.upp`, `Keen/Id/Id.upp`, `Keen/Game/Game.upp`, `Keen/Opl/Opl.upp`.
  - Add `AGENTS.md` to each new package. Add `CURRENT_TASK.md` to each `.upp` file’s `file` list when manifests exist.
- Phase B: Core utilities
  - Port `id_str.*` 1:1 (C++), ensure unit-usage compiles. Replace `MM_GetPtr/SetLock` with standard allocation.
  - Stub `id_mm` to provide symbols used by `id_str` and others.
- Phase C: Files/config + null backends
  - Port `id_cfg`, `id_fs`, and add null backends for `id_vl`, `id_in`, `id_sd` to satisfy linkage.
- Phase D: Game bootstrap
  - Port `ck_main`, `ck_keen`, `ck_game` minimal init; disable platform calls behind interfaces.
- Phase E: Feature bring‑up
  - Incrementally replace null backends with real ones (start with U++ Draw for software blit and a minimal audio path).

Key Decisions
- Keep C naming during Phase A to minimize churn; introduce C++ classes/namespaces during stabilization.
- Use `Upp::` containers for new code; keep POD layouts in gameplay structs until logic is validated.
- Avoid SDL as a dependency initially; backends are abstracted and optional.

Risks / Notes
- OPL emulation is sizable; isolate in `Keen/Opl` to keep main build fast. Licenses included in `opl/` must be preserved.
- Vulkan/GL backends can be deferred; start with null/software.
- The original memory manager relies on manual purging/locking; replicate behavior only where semantically required (resource lifetime), otherwise prefer RAII.

Done
- Scanned and grouped C sources; defined U++ packaging and port sequence.
- Scaffolded packages: `Keen`, `Keen/Id`, `Keen/Game`, `Keen/Opl` with main headers and `.upp` manifests.
- Ported `id_str` to C++ (`Keen/Id/IdStr.*`).
- Implemented minimal memory and filesystem shims (`IdMm.*`, `IdFs.*`).
- Ported config manager minimally (`IdCfg.*`) using `IdStr` + `IdFs`.
- Introduced C++ FS wrapper: `namespace FS { class File }`. Removed `FS_Read` and related C APIs; updated loaders to use `File::Read` and `FS::ReadAllUser` helper.
- Replaced `VL/IN/SD` null C stubs with class facades: `Video::Screen`, `Input::Device`, `Audio::Device`.
- Added `Game::Bootstrap()` to exercise startup of Config/Video/Input/Audio.
 - Added `Time::Clock` and exercised a tiny 3-frame loop in `Game::Bootstrap()`.
- Added `FS::Reader/Writer` for endian-aware IO.

Next Actions
- Add more `id_fs` API surface if other modules need it (Keen data paths, endian helpers).
- Use `Game::Bootstrap()` to exercise facades and config flow; expand with a simple tick loop and state transitions.
- Expand `Video::Screen` (dimensions, buffers) and `Audio::Device` (modes, playbacks) as needed by upcoming ports.
- Begin mapping episode-specific headers for later inclusion.
 - Start migrating any new FS_ReadIntXX/WriteXX and similar call sites in future ports to `FS::Reader/Writer`.

Planned Class Conversions (queued)
- Str::Table, Str::Parser, Str::Token: wrappers over id_str.
- Video::Screen: initial facade done; expand functions as needed.
- Audio::Device: initial facade done; expand functions as needed.
- Input::Device: initial facade done; expand functions as needed.

Migration Rules (for continuation)
- When encountering a C function with a clear module prefix (e.g., FS_, STR_, CFG_, VL_, SD_, IN_), identify the owning class and add or extend a thin C++ wrapper with methods for that function group.
- Update all in-repo usages to call the class methods instead of the C function, then remove the C function from our `uppsrc` port to avoid drift. Keep the function name and behavior as method names where natural.
- Maintain compatibility temporarily via small helper functions in the C++ namespace (e.g., `FS::ReadAllUser`) only when useful to reduce boilerplate, but prefer moving logic into class methods (`File::ReadAll` etc.) as the code stabilizes.


TBD (after core compiles)
- Replace ad-hoc string table with `Index<String>`/`VectorMap<String, void*>` or a typed template.
- Implement a Draw-based renderer matching `id_vl` needs; introduce timing/audio via U++ or minimal SDL shims if necessary.
