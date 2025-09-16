#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenBufferSyncTests() {
    Video::Screen s;
    s.SetNumBuffers(2);
    int a = s.GetActiveBufferId();
    s.SwapOnNextPresent();
    s.Present();
    ASSERT(s.GetActiveBufferId() == ((a + 1) % 2));

    RF::State rf;
    rf.SetBuffers(2);
    rf.MarkBlockDirty(0,0,1, rf.GetActiveBuffer());
    ASSERT(rf.IsBlockDirty(0,0, rf.GetActiveBuffer()) == 1);
    rf.SwitchBuffer();
    ASSERT(rf.IsBlockDirty(0,0, rf.GetActiveBuffer()) == 0);
}

END_UPP_NAMESPACE

