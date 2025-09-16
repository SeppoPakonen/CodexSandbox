# ManiaCore AGENTS

- Follows repository Header Include Policy (U++ BLITZ): all `.cpp/.icpp` include only `ManiaCore.h` first.
- `ManiaCore.h` aggregates internal headers and wraps them in `NAMESPACE_UPP`.
- Do not add `#include` lines to internal headers; keep them standalone. Use forward declarations when possible.
- Keep this package engine-agnostic; no Unity/Photon references.

Contents
- AlphaNum: natural/alphanumeric string comparator.
- Utils: small helpers (`TimeToStr`, `Ordinal`, modular arithmetic).
- Prefs/Logger: placeholders, minimal stubs for now.

