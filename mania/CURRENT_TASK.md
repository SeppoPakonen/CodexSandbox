# Mania C# → C++/U++ Port — Plan and Status

Scope: Convert Unity C# sources in `sandbox/mania/csharp/` to C++ packages that follow U++ conventions. Target root for new packages: `sandbox/mania/uppsrc/`.

Status: Initial analysis complete. This document defines the package layout, mapping choices, and phased plan. Implementation has not started yet.

## Inventory (source to port)
- Location: `sandbox/mania/csharp/`
- Files: 362 C# files (Unity/Photon heavy; multiple subsystems)
- Notable areas identified during sampling:
  - Core utils and settings: `Base.cs`, `bs.cs`, `AutoQuality.cs`, `Awards*.cs`, `AuthenticationValues.cs`, `Server*`, `Settings`/`Res`/`ResLoader`
  - Geometry/algorithms: `Triangulator.cs`, `AlphanumComparatorFast.cs`
  - Spline system: `CurvySpline*.cs`, `Curvy.Utils/` (segments, interpolation, orientation)
  - Gameplay-specific: `Weapon*`, `Bullet.cs`, `Tank*`, `Car*`, `Skidmarks.cs`, `Damage*`
  - Networking (Photon): `Room*`, `ConnectAndJoinRandom.cs`, `ViewSynchronization.cs`, `SmoothSync*`
  - Audio/voice: `VoiceChat*`
  - FX/UI/Unity components: `WFX_*`, `CFX_*`, `Everyplay`, various MonoBehaviours

Observation: Much of the code depends on Unity (MonoBehaviour, Transform, Vector2/3, coroutines, Resources, WWW) and Photon. The initial port should focus on reusable algorithms and core logic, abstracting or stubbing engine-specific behavior.

## U++ Conventions to apply
- Package structure under `uppsrc/` with `.upp` manifests.
- Each package provides a single main header `PackageName.h`; all `.cpp/.icpp` begin with `#include "PackageName.h"`.
- Avoid intra‑package header includes from source files; only the main header aggregates. Keep aggregation wrapped in `NAMESPACE_UPP` via `NAMESPACE_UPP` macro.
- Subpackages are independent packages (no cross‑include of internals). Parent may include subpackage main headers only.
- Add `AGENTS.md` and `CURRENT_TASK.md` as the first entries in each package `.upp` file when packages are created.

## Proposed package layout (sandbox/mania/uppsrc)
- ManiaCore
  - Purpose: Core utilities replacing parts of `Base.cs`/`bs.cs` that are engine‑agnostic.
  - Components: Prefs (key/value persistent store), Logger, String/time utils, RNG helpers, Alphanum comparator.
  - HTTP: thin wrapper targeting `Upp::HttpRequest` to replace simple `WWW` use (deferred to phase C).
  - Obscured types: lightweight XOR/salt wrappers for `int/float/bool` if needed by Prefs.
- ManiaMath
  - Purpose: Minimal math types analogous to Unity’s `Vector2/3` and helpers used by geometry/splines.
  - Components: `Vec2f`, `Vec3f`, basic ops (dot, cross, length, normalize), angle helpers.
  - Note: Prefer U++/Geometry types if practical; otherwise provide compact local types to avoid wide dependencies initially.
- ManiaGeometry
  - Purpose: Algorithms like ear‑clipping triangulation.
  - Components: `Triangulator` (port of `Triangulator.cs`), utility predicates.
- ManiaCurvy
  - Purpose: Spline math without engine objects.
  - Components: `CurvySpline`, `CurvySplineSegment`, interpolation modes (Catmull‑Rom, TCB), orientation utilities.
  - Changes: Replace `Transform/GameObject/MonoBehaviour` with plain data structures; remove runtime scene management; provide API for sampling, moving along spline, approximations.
- ManiaNet (stub, optional later)
  - Purpose: Abstract interfaces for networking formerly implemented via Photon, can be ignored initially.
- ManiaFX/ManiaAudio (defer)
  - Unity‑dependent visual/audio features are out of scope for the first pass.

## Mapping and de‑Unity plan
- Unity types
  - `Vector2/Vector3` → `ManiaMath::Vec2f/Vec3f` (or U++ geometry types if we later align)
  - `Quaternion/Matrix4x4/Transform` → remove from core; only math required for tangents/orientation remains. Expose orientation as vectors; later add optional quaternion support.
- Coroutines/Timers
  - Replace `IEnumerator` coroutines (`AddMethod`, delayed actions) with U++ `SetTimeCallback*` utilities and/or a small scheduler in ManiaCore.
- PlayerPrefs
  - Replace with `Prefs` using U++ config path (`GetConfigFile`) and simple INI or JSON storage (`Upp::LoadIniFile`/`SaveIniFile` or `JsonIO`). Keep an in‑memory cache mirroring the original pattern.
  - Support optional obfuscation (simple XOR with per‑user salt) to mirror `Obscured*` usage.
- HTTP/Downloads (`WWW`)
  - Replace with `Upp::HttpRequest`. Preserve simple GET/POST plus optional caching flag. This is non‑critical for geometry/splines and can be phased in later.
- Compression (Ionic.Zlib)
  - Use U++ Z library (`Z`), or fallback to system zlib if available. For now, gate features with compile‑time flag to allow building without compression.
- Photon networking / Unity scene graph / resources
  - Not ported. Provide no‑op or thin interfaces where needed so core packages compile independently.

## Deliverables and acceptance
- Buildable U++ packages under `sandbox/mania/uppsrc/` with `.upp` manifests and main headers.
- Compiles in TheIDE with only U++ dependencies (Core; optional Z/Http packages if used in later phases).
- Unit/demo program that exercises: Alphanum comparator, Triangulator with a simple polygon, Curvy spline interpolation and sampling.

## Phased plan
1) Scaffolding and groundwork
   - Create `ManiaCore`, `ManiaMath`, `ManiaGeometry`, `ManiaCurvy` packages with `.upp` manifests.
   - Add `AGENTS.md` and `CURRENT_TASK.md` to each package manifest first. Main header per package.
2) Utilities (ManiaCore)
   - Implement Prefs cache and persistent store (INI via U++).
   - Port `AlphanumComparatorFast` to a C++ comparator and tests.
   - Provide small logger helpers and time/format functions (`TimeToStr`, `Ordinal`).
3) Math and Geometry
   - Implement `Vec2f/Vec3f` and helpers used by algorithms.
   - Port `Triangulator` and validate on sample polygons.
4) Spline subsystem (ManiaCurvy)
   - Model `CurvySpline` data: control points, segments, granularity, interpolation.
   - Implement `Interpolate`, `GetTangent`, `MoveBy/MoveByFast`, TF/Distance mapping, approximation arrays. Remove Transform/GameObject coupling.
   - Provide tests/demos to sample spline points and lengths.
5) Optional HTTP/Compression (deferred)
   - Port simple download routines with cache using `HttpRequest`; wrap compression if present.
6) Documentation and cleanup
   - Update this CURRENT_TASK with progress, decisions, and next steps.

## Initial targets (first conversion wave)
- `AlphanumComparatorFast.cs` → `ManiaCore/AlphaNum.h/.cpp`
- `Triangulator.cs` → `ManiaGeometry/Triangulator.h/.cpp`
- `CurvySpline.cs`, `CurvySplineSegment.cs`, `CurvySpline2.cs`, `CurvyUtility.cs` → `ManiaCurvy/*` (math only)
- Selected small utilities from `Base.cs`/`bs.cs`: `TimeToStr`, `Ordinal`, modular arithmetic helpers; Prefs redesign as above.

Additional progress
- Added `ManiaCore/Hash` with MD5 helpers, and `ManiaCore/Http` wrapper for GET/POST with optional cache (U++ HttpRequest-backed).
- Added `ManiaGeometry/Poly` with `PolygonArea`, `IsCCW`, and `IsConvex` helpers.

## Risks and notes
- Many C# classes are Unity/Photon specific; full parity is not practical. We focus on reusable math and core logic.
- Spline orientation/quaternion code may need adaptation; we can initially support tangent vectors only.
- Preferences security/obfuscation should be pragmatic, not strong crypto.
- Zlib/HTTP availability depends on U++ build configuration; gate optional features.

## Next actions
- Create package scaffolds in `sandbox/mania/uppsrc/` per layout above. [DONE]
- Port and test `AlphanumComparatorFast` and `Triangulator` first. [DONE]
- Implement `CurvySpline` tangents, approximation, length and TF/Distance mapping. [DONE]
- Extend Curvy with segment-based distance tables and nearest-point utilities. [PENDING]

—
Log
- 2025‑09‑15: Analyzed `sandbox/mania/csharp` sources (362 files). Drafted package layout, mappings, and phased plan. No code changes yet.
- 2025‑09‑15: Scaffolded `ManiaCore`, `ManiaMath`, `ManiaGeometry` packages with `.upp` manifests, AGENTS, CURRENT_TASK, and main headers. Implemented AlphaNum comparator and Triangulator (ear‑clipping). Next: wire a simple demo/test and start `ManiaCurvy` stubs.
- 2025‑09‑15: Added `ManiaCurvy` with Catmull‑Rom interpolation, tangents, polyline approximation, approximate length, TF↔Distance mapping. Created `ManiaDemo` console app with tests for AlphaNum, Triangulator (including concave polygon), and Curvy length checks.
