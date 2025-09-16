Keen Package (umbrella)

- Purpose: Aggregate subpackages `Id` and `Game` for the Commander Keen port.
- Follow Header Include Policy:
  - Source files start with `#include "Keen.h"` only.
  - `Keen.h` is the main header and wraps aggregated headers in `NAMESPACE_UPP`.
  - Do not introduce namespaces in non-main headers included by `Keen.h`.

