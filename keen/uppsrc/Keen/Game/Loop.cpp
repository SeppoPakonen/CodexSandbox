#include "Game.h"
#include "../Id/Id.h"

NAMESPACE_UPP

namespace Game {

void InjectDemo(Input::Device& in, StateMachine& sm, int& demo_accum, int ticks) {
    if (!sm.IsDemo()) return;
    demo_accum += ticks;
    const int pulse = 70; // approx 1s at 70Hz
    // Ensure mappings exist
    if (in.GetMapping(Input::Device::Key::Jump) == 0)
        in.MapKey(Input::Device::Key::Jump, 0x39); // space
    if (in.GetMapping(Input::Device::Key::Left) == 0)
        in.MapKey(Input::Device::Key::Left, 0x4B);
    if (in.GetMapping(Input::Device::Key::Right) == 0)
        in.MapKey(Input::Device::Key::Right, 0x4D);

    static bool dir_right = true;
    static int pause_accum = 0;
    pause_accum += ticks;
    if (demo_accum >= pulse) {
        // Jump pulse (edge)
        in.PushEvent(in.GetMapping(Input::Device::Key::Jump), true);
        // Toggle movement direction and update held keys via releases and new press
        // Release both first to avoid both being held
        in.PushEvent(in.GetMapping(Input::Device::Key::Left), false);
        in.PushEvent(in.GetMapping(Input::Device::Key::Right), false);
        if (dir_right)
            in.PushEvent(in.GetMapping(Input::Device::Key::Right), true);
        else
            in.PushEvent(in.GetMapping(Input::Device::Key::Left), true);
        dir_right = !dir_right;
        demo_accum = 0;
    }
    if (pause_accum >= (pulse * 4)) {
        if (in.GetMapping(Input::Device::Key::Up) == 0)
            in.MapKey(Input::Device::Key::Up, 0x48);
        in.PushEvent(in.GetMapping(Input::Device::Key::Up), true);
        pause_accum = 0;
    }
}

bool AutoDemoTimeout(int& idle_accum, int ticks, int thresholdTicks) {
    idle_accum += ticks;
    if (idle_accum >= thresholdTicks) { idle_accum = 0; return true; }
    return false;
}

int ComputeAutoDemoThresholdTicks(const AppTime::FrameRate& fr) {
    int ms = Config::Store::Instance().GetInt("auto_demo_ms", 5000);
    int base = fr.BaseMs();
    if (base <= 0) base = 14;
    int ticks = ms / base;
    if (ticks < 70) ticks = 70; // at least ~1s
    return ticks;
}

void RunDemoLoop(int frames) {
    Video::Screen s;
    s.SetSize(160, 120);
    s.Clear(0xff101010);
    s.SetNumBuffers(2);

    Input::Device& in = Input::Device::Default();
    in.Startup();

    Player player;
    player.Reset();
    StateMachine sm; sm.Reset();
    Hud hud; hud.Reset();
    TitleMenu menu; menu.Reset();

    AppTime::FrameRate fr(14, 2, 5, 3);
    uint32_t last = AppTime::Clock::NowMs();
    RF::State rf;
    rf.SetBuffers(2);
    int prevx = player.X();
    int prevy = player.Y();

    int demo_accum = 0;
    int title_idle = 0;
    int auto_demo_threshold = ComputeAutoDemoThresholdTicks(fr);
    for (int f = 0; f < frames; ++f) {
        uint32_t now = AppTime::Clock::NowMs();
        uint32_t elapsed = now - last;
        last = now;

        int ticks = fr.ComputeTicks(elapsed);
        InjectDemo(in, sm, demo_accum, ticks);
        in.Poll();
        if (sm.Get() == StateMachine::Mode::Title) {
            // Process title menu input
            auto act = menu.Step(in);
            if (act == TitleMenu::Action::Start) {
                sm.SetMode(StateMachine::Mode::Playing);
            } else if (act == TitleMenu::Action::StartDemo) {
                sm.SetDemo(true);
                sm.SetMode(StateMachine::Mode::Playing);
            } else {
                // Auto-start demo after timeout without input
                if (AutoDemoTimeout(title_idle, ticks, auto_demo_threshold)) {
                    sm.SetDemo(true);
                    sm.SetMode(StateMachine::Mode::Playing);
                }
            }
        } else {
            sm.Step(in);
            sm.Update(ticks);
            player.Step(in, ticks);
        }
        hud.Update(in, ticks);

        // Draw background and player as a simple 3x3 white box
        s.Clear(0xff102020);
        if (sm.Get() == StateMachine::Mode::Title) {
            menu.Draw(s);
            menu.Update(ticks);
        } else {
            int px = player.X() >> 4; // scale units to pixels
            int py = (player.Y() >> 4);
            s.DrawRect(80 + px, 60 - py, 3, 3, 0xffffffff);
        }
        // Mark dirty tiles around previous and current positions
        auto mark_dirty = [&](int ux, int uy){
            int sx = 80 + (ux >> 4);
            int sy = 60 - (uy >> 4);
            int tx = sx >> 4; int ty = sy >> 4;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx) {
                    int mx = tx + dx, my = ty + dy;
                    rf.MarkBlockDirty(mx, my, 1, rf.GetActiveBuffer());
                }
        };
        if (sm.Get() != StateMachine::Mode::Title) {
            mark_dirty(prevx, prevy);
            mark_dirty(player.X(), player.Y());
            prevx = player.X(); prevy = player.Y();
        }

        // Compute bounding rect between previous and current positions and request update
        int old_px = 80 + (prevx >> 4);
        int old_py = 60 - (prevy >> 4);
        int cur_px = 80 + (player.X() >> 4);
        int cur_py = 60 - (player.Y() >> 4);
        int rx = std::min(old_px, cur_px) - 4;
        int ry = std::min(old_py, cur_py) - 4;
        int rw = std::abs(cur_px - old_px) + 8;
        int rh = std::abs(cur_py - old_py) + 8;
        s.UpdateRect(rx, ry, rw, rh);

        char buf[96];
        // Append demo/record markers
        const char* demo = sm.IsDemo() ? " D" : "";
        const char* rec  = sm.IsRecording() ? " REC" : "";
        std::snprintf(buf, sizeof(buf), "%s%s%s B:%d T:%d X:%d Y:%d", sm.Name(), demo, rec, s.GetActiveBufferId(), ticks, player.X(), player.Y());
        s.DrawText5x7(2, 2, buf, 0xffffffff);
        // Pause overlay
        if (sm.Get() == StateMachine::Mode::Paused) {
            s.DrawRect(40, 40, 80, 40, 0xff000000);
            s.DrawText5x7(58, 58, "PAUSE", 0xffffffff);
        }
        // DEMO banner
        if (sm.IsDemo())
            s.DrawText5x7(120, 2, "DEMO", 0xffffff00);
        if (sm.Get() != StateMachine::Mode::Title)
            hud.Draw(s);
        s.SwapOnNextPresent();
        s.Present();
        rf.SwitchBuffer();

        AppTime::Clock::DelayMs(16);
    }
}

}

END_UPP_NAMESPACE
