#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenStateCounterTests() {
    Game::StateMachine sm;
    sm.Reset();
    Input::Device& in = Input::Device::Default();
    in.MapKey(Input::Device::Key::Jump, 50);
    in.PushEvent(50, true);
    in.Poll();
    sm.Step(in);
    ASSERT(String(sm.Name()) == "PLAY");
    sm.Update(5);
    ASSERT(sm.Counter() >= 5);
    in.MapKey(Input::Device::Key::Up, 51);
    in.PushEvent(51, true);
    in.Poll();
    sm.Step(in);
    ASSERT(String(sm.Name()) == "PAUSE");
    int before = sm.Counter();
    sm.Update(10);
    ASSERT(sm.Counter() == before);
}

END_UPP_NAMESPACE

