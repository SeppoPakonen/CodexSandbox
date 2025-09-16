#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenRfTests() {
    RF::State rf;
    rf.ForceRefresh();
    // Initially dirty after ForceRefresh
    ASSERT(rf.IsBlockDirty(0,0,0) == 1);
    // Mark (2,3) clean on all pages
    rf.MarkBlockDirty(2,3, 0, -1);
    ASSERT(rf.IsBlockDirty(2,3,0) == 0);
    // Reposition by +1 tile in X and +2 in Y causes wrap/offset change
    rf.Reposition(1<<8, 2<<8);
    // Mark (0,0) dirty on page 0 and check
    rf.MarkBlockDirty(0,0, 1, 0);
    ASSERT(rf.IsBlockDirty(0,0,0) == 1);
}

END_UPP_NAMESPACE

