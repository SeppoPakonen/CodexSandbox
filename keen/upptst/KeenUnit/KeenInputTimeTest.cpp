#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenInputTimeTests() {
    // Input mapping and edges
    Input::Device& in = Input::Device::Default();
    in.MapKey(Input::Device::Key::Jump, 10);
    in.PushEvent(10, true);
    in.Poll();
    ASSERT(in.IsDown(Input::Device::Key::Jump));
    ASSERT(in.WasPressed(Input::Device::Key::Jump));
    in.Poll();
    ASSERT(!in.WasPressed(Input::Device::Key::Jump));
    in.PushEvent(10, false);
    in.Poll();
    ASSERT(!in.IsDown(Input::Device::Key::Jump));
    ASSERT(in.WasReleased(Input::Device::Key::Jump));

    // FrameRate ticks
    AppTime::FrameRate fr(14, 2, 5, 3);
    ASSERT(fr.ComputeTicks(0) == 2);
    ASSERT(fr.ComputeTicks(14) >= 2);
    ASSERT(fr.ComputeTicks(1000) <= 5);
}

END_UPP_NAMESPACE

