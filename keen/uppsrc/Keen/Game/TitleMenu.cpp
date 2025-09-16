#include "Game.h"

NAMESPACE_UPP

namespace Game {

TitleMenu::Action TitleMenu::Step(Input::Device& in) {
    if (in.GetMapping(Input::Device::Key::Up) == 0)
        in.MapKey(Input::Device::Key::Up, 0x48);
    if (in.GetMapping(Input::Device::Key::Down) == 0)
        in.MapKey(Input::Device::Key::Down, 0x50);
    if (in.GetMapping(Input::Device::Key::Jump) == 0)
        in.MapKey(Input::Device::Key::Jump, 0x39);

    if (in.WasPressed(Input::Device::Key::Up))
        sel = (sel + 1) % 2;
    if (in.WasPressed(Input::Device::Key::Down))
        sel = (sel + 1) % 2;
    if (in.WasPressed(Input::Device::Key::Jump))
        return sel == 0 ? Action::Start : Action::StartDemo;
    return Action::None;
}

void TitleMenu::Draw(Video::Screen& s) const {
    s.Clear(0xff000030);
    s.DrawText5x7(50, 20, "OMNISPEAK", 0xffffffff);
    uint32_t hl = blink ? 0xff00ffff : 0xffffff00;
    uint32_t colorStart = (sel == 0 ? hl : 0xffffffff);
    uint32_t colorDemo  = (sel == 1 ? hl : 0xffffffff);
    s.DrawText5x7(60, 40, "START", colorStart);
    s.DrawText5x7(60, 52, "DEMO", colorDemo);
}

void TitleMenu::Update(int ticks) {
    blink_accum += ticks;
    if (blink_accum >= 35) { blink = !blink; blink_accum = 0; }
}

}

END_UPP_NAMESPACE
