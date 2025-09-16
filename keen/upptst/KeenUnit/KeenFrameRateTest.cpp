#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenFrameRateTests() {
    AppTime::FrameRate fr(14, 2, 5, 3);
    ASSERT(fr.ComputeTicks(1) == 2);
    int t = fr.ComputeTicks(14);
    ASSERT(t >= 2 && t <= 5);
    ASSERT(fr.ComputeTicks(1000) == 5);
    ASSERT(fr.ComputeTicks(28) <= 5);
}

END_UPP_NAMESPACE

