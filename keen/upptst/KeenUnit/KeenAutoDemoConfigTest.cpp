#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenAutoDemoConfigTests() {
    AppTime::FrameRate fr(14, 2, 5, 3);
    int def_ticks = Game::ComputeAutoDemoThresholdTicks(fr);
    ASSERT(def_ticks >= 70);
    Config::Store::Instance().SetInt("auto_demo_ms", 140);
    int cfg_ticks = Game::ComputeAutoDemoThresholdTicks(fr);
    ASSERT(cfg_ticks <= 10);
}

END_UPP_NAMESPACE

