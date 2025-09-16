// Minimal demo main loop
#pragma once

namespace Game {

void RunDemoLoop(int frames = 60);

// Helper used by loop and tests: inject demo input pulses when demo mode is active
void InjectDemo(Input::Device& in, StateMachine& sm, int& demo_accum, int ticks);

}
