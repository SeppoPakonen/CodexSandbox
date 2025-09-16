Keen/Game Package

- Ports gameplay code (`ck_*`), organized by core and per-episode pieces.
- Main header: `Game.h`. Implementations must start with `#include "Game.h"`.
- `Game.h` wraps internal headers under `NAMESPACE_UPP` and should only include headers from this subpackage or earlier dependencies (e.g., `Keen/Id`).

