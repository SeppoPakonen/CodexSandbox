#include "Id.h"

NAMESPACE_UPP

namespace Input {

Device& Device::Default() {
    static Device dev;
    return dev;
}

void Device::Startup() {}
void Device::Shutdown() {}
void Device::Poll() {
    // Update prev edge states
    prev_ = keys_;
    // Apply queued events atomically this frame
    for (const auto& e : queue_)
        SetKey(e.sc, e.down);
    queue_.clear();
}

void Device::MapKey(Key k, ScanCode sc) {
    map_[(size_t)k] = sc;
}

ScanCode Device::GetMapping(Key k) const {
    return map_[(size_t)k];
}

bool Device::IsDown(Key k) const { return IsDown(GetMapping(k)); }

bool Device::WasPressed(Key k) const {
    ScanCode sc = GetMapping(k);
    return keys_.test(sc) && !prev_.test(sc);
}

bool Device::WasReleased(Key k) const {
    ScanCode sc = GetMapping(k);
    return !keys_.test(sc) && prev_.test(sc);
}

void Device::ClearEdgeStates() { prev_.reset(); }

void Device::PushEvent(ScanCode sc, bool down) { queue_.push_back({sc, down}); }

} // namespace Input

END_UPP_NAMESPACE
