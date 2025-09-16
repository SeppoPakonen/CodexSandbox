// Minimal demo main loop
#pragma once

namespace Game {

void RunDemoLoop(int frames = 60);

// Helper used by loop and tests: inject demo input pulses when demo mode is active
void InjectDemo(Input::Device& in, StateMachine& sm, int& demo_accum, int ticks);

// Helper to accumulate title idle ticks and signal auto-demo start
bool AutoDemoTimeout(int& idle_accum, int ticks, int thresholdTicks = 350);
// Reads auto-demo threshold from config (auto_demo_ms, default 5000) and returns ticks for the given framerate
int ComputeAutoDemoThresholdTicks(const AppTime::FrameRate& fr);

}
