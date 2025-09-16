#include "Id.h"


NAMESPACE_UPP

namespace AppTime {

void Clock::DelayMs(int ms) {
    if (ms > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

uint32_t Clock::NowMs() {
    using namespace std::chrono;
    auto now = time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch();
    return (uint32_t)now.count();
}

int FrameRate::ComputeTicks(uint32_t elapsedMs) const {
    if (base_ms <= 0) return min_t;
    int ticks = (int)(elapsedMs / (uint32_t)base_ms);
    if (ticks < min_t) ticks = min_t;
    if (ticks > max_t) ticks = max_t;
    return ticks;
}

}

END_UPP_NAMESPACE
