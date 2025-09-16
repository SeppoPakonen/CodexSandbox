#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenStateTests() {
    Game::StateMachine sm;
    sm.Reset();
    Input::Device& in = Input::Device::Default();
    in.MapKey(Input::Device::Key::Jump, 30);
    in.PushEvent(30, true);
    in.Poll();
    sm.Step(in);
    ASSERT(String(sm.Name()) == "PLAY");
    in.MapKey(Input::Device::Key::Up, 31);
    in.PushEvent(31, true);
    in.Poll();
    sm.Step(in);
    ASSERT(String(sm.Name()) == "PAUSE");
    in.PushEvent(31, true);
    in.Poll();
    sm.Step(in);
    ASSERT(String(sm.Name()) == "PLAY");
}

END_UPP_NAMESPACE

