#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenAutoDemoTimeoutTests() {
    int idle = 0;
    ASSERT(!Game::AutoDemoTimeout(idle, 100, 350));
    ASSERT(Game::AutoDemoTimeout(idle, 250, 350));
    ASSERT(!Game::AutoDemoTimeout(idle, 100, 350));
}

END_UPP_NAMESPACE

