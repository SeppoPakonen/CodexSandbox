#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenHudTests() {
    Game::Hud hud; hud.Reset();
    Input::Device& in = Input::Device::Default();
    in.MapKey(Input::Device::Key::Fire, 40);
    ASSERT(hud.Shots() == 0);
    in.PushEvent(40, true);
    in.Poll();
    hud.Update(in, 1);
    ASSERT(hud.Shots() == 1);
}

END_UPP_NAMESPACE

