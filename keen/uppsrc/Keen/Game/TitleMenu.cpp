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
    uint32_t colorStart = (sel == 0 ? 0xffffff00 : 0xffffffff);
    uint32_t colorDemo  = (sel == 1 ? 0xffffff00 : 0xffffffff);
    s.DrawText5x7(60, 40, "START", colorStart);
    s.DrawText5x7(60, 52, "DEMO", colorDemo);
}

}

END_UPP_NAMESPACE

