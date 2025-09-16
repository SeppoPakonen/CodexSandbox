#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenPlayerTests() {
    Game::Player p;
    p.Reset();
    Input::Device& in = Input::Device::Default();
    in.MapKey(Input::Device::Key::Right, 20);
    in.MapKey(Input::Device::Key::Jump, 21);
    in.PushEvent(20, true);
    in.PushEvent(21, true);
    in.Poll();
    p.Step(in, 1);
    ASSERT(!p.OnGround());
    in.PushEvent(21, false);
    in.Poll();
    p.Step(in, 10);
    ASSERT(p.X() > 0);
    p.Step(in, 100);
    ASSERT(p.OnGround());
}

END_UPP_NAMESPACE

