#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenDemoMoveTests() {
    Input::Device& in = Input::Device::Default();
    Game::StateMachine sm; sm.Reset(); sm.SetDemo(true);
    Game::Player p; p.Reset();
    in.MapKey(Input::Device::Key::Left, 80);
    in.MapKey(Input::Device::Key::Right, 81);
    int demo_accum = 0;
    Game::InjectDemo(in, sm, demo_accum, 70);
    in.Poll();
    p.Step(in, 10);
    int x1 = p.X();
    Game::InjectDemo(in, sm, demo_accum, 70);
    in.Poll();
    p.Step(in, 10);
    int x2 = p.X();
    ASSERT(x1 != x2);
}

END_UPP_NAMESPACE

