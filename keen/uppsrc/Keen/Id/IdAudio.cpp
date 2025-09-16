#include "Id.h"

NAMESPACE_UPP

namespace Audio {

Device& Device::Default() {
    static Device dev;
    return dev;
}

void Device::Startup() {}
void Device::Shutdown() {}

} // namespace Audio

END_UPP_NAMESPACE

