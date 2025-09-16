#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenDemoPauseTests() {
    Input::Device& in = Input::Device::Default();
    Game::StateMachine sm; sm.Reset(); sm.SetDemo(true);
    in.MapKey(Input::Device::Key::Jump, 60);
    in.MapKey(Input::Device::Key::Up, 61);
    int demo_accum = 0;
    Game::InjectDemo(in, sm, demo_accum, 70);
    in.Poll();
    sm.Step(in);
    ASSERT(String(sm.Name()) == "PLAY");
    Game::InjectDemo(in, sm, demo_accum, 280);
    in.Poll();
    sm.Step(in);
    ASSERT(String(sm.Name()) == "PAUSE");
}

END_UPP_NAMESPACE

