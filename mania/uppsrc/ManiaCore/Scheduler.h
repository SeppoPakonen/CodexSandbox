#pragma once

// Simple time-based scheduler (engine-agnostic)

struct ManiaTaskScheduler {
    void   PostDelayed(double seconds, Function<void()> fn);
    void   Tick(); // runs due tasks; call periodically
    void   RunUntilEmpty();
    void   Clear();

private:
    struct Item { int64 due_ms; Function<void()> fn; };
    Vector<Item> q;
};

extern ManiaTaskScheduler Scheduler;

