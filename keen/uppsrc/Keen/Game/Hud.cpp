#include "Game.h"

NAMESPACE_UPP

namespace Game {

void Hud::Update(Input::Device& in, int ticks) {
    if (in.GetMapping(Input::Device::Key::Fire) == 0)
        in.MapKey(Input::Device::Key::Fire, 0x1D);
    if (in.WasPressed(Input::Device::Key::Fire))
        shots++;
    elapsed += ticks;
    if (elapsed >= 35) { blink = !blink; elapsed = 0; }
}

void Hud::Draw(Video::Screen& s) const {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "S:%d N:%d", score, shots);
    s.DrawText5x7(2, 10, buf, 0xffffffff);
    if (blink)
        s.DrawText5x7(2, 18, "!", 0xffffffff);
}

}

END_UPP_NAMESPACE

