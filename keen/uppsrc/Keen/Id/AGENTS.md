Keen/Id Package

- Ports core ID-engine modules (`id_*`).
- Main header: `Id.h`. All implementation files must start with `#include "Id.h"`.
- `Id.h` wraps internal headers under `NAMESPACE_UPP`.
- Non-main headers (e.g. `IdStr.h`) must not declare a namespace and should avoid including third-party/system headers unless truly local.
- Prefer U++ containers and RAII over custom memory managers; keep C APIs during the initial compile pass.

