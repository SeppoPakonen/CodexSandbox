Package: Game/Core/Math

Purpose
- Hosts glm-like math code (vectors, matrices, quaternions, geometry, bounds) previously in Game/Core.

Conventions
- Main header is `Math.h`. All `.cpp` files in this package must include `"Math.h"` first.
- Internal header previously named `Math.h` (math utilities namespace Mth) is renamed to `MathCore.h` to avoid collision with the package main header.
- This package depends on `Game/Core` for basic types and debug macros. The main header includes the needed parent headers (`Types.h`, `Defines.h`, `Checks.h`, `Debug.h`).
- Follow the existing code style; do not wrap these headers with U++ `NAMESPACE_UPP` as math symbols are in their own namespaces and widely referenced.

Notes
- If new math components are added, list them in `Math.upp` and aggregate them in `Math.h`.

