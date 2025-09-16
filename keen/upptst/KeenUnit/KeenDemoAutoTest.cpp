#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenDemoAutoTests() {
    Input::Device& in = Input::Device::Default();
    Game::StateMachine sm; sm.Reset();
    sm.SetDemo(true);
    Game::Player p; p.Reset();
    int demo_accum = 0;
    Game::InjectDemo(in, sm, demo_accum, 70);
    in.Poll();
    p.Step(in, 1);
    ASSERT(!p.OnGround());
}

END_UPP_NAMESPACE

