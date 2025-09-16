#include "ManiaCore.h"

NAMESPACE_UPP

ManiaTaskScheduler Scheduler;

static inline int64 NowMs() { return msecs(); }

void ManiaTaskScheduler::PostDelayed(double seconds, Function<void()> fn)
{
    Item it; it.due_ms = NowMs() + (int64)(seconds * 1000.0); it.fn = pick(fn);
    q.Add(pick(it));
}

void ManiaTaskScheduler::Tick()
{
    int64 now = NowMs();
    // run all due tasks; simple O(n) scan with compaction
    Vector<Item> rest;
    for (int i = 0; i < q.GetCount(); ++i) {
        if (q[i].due_ms <= now) {
            if (q[i].fn) q[i].fn();
        } else {
            rest.Add(pick(q[i]));
        }
    }
    q = pick(rest);
}

void ManiaTaskScheduler::RunUntilEmpty()
{
    while (!q.IsEmpty())
        Tick();
}

void ManiaTaskScheduler::Clear()
{
    q.Clear();
}

END_UPP_NAMESPACE

