// Minimal title menu (resource-agnostic)
#pragma once

namespace Game {

class TitleMenu {
public:
    enum class Action { None, Start, StartDemo, ShowScores };
    void Reset() { sel = 0; }
    void Update(int ticks);
    // Processes input edges; returns action when Jump is pressed
    Action Step(Input::Device& in);
    void Draw(Video::Screen& s) const;
    int Selection() const { return sel; }
private:
    int sel = 0; // 0 = START, 1 = DEMO, 2 = SCORES
    int blink_accum = 0;
    bool blink = false;
};

}
