#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenDemoRecordTests() {
    Game::StateMachine sm; sm.Reset();
    Input::Device& in = Input::Device::Default();
    in.MapKey(Input::Device::Key::Down, 70);
    in.MapKey(Input::Device::Key::Right, 71);
    ASSERT(!sm.IsDemo() && !sm.IsRecording());
    in.PushEvent(70, true); in.Poll(); sm.Step(in);
    ASSERT(sm.IsDemo());
    in.PushEvent(71, true); in.Poll(); sm.Step(in);
    ASSERT(sm.IsRecording());
}

END_UPP_NAMESPACE

