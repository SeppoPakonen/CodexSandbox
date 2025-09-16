// Minimal HUD (resource-agnostic)
#pragma once

namespace Game {

class Hud {
public:
    void Reset() { score = 0; shots = 0; elapsed = 0; blink = false; }
    void Update(Input::Device& in, int ticks);
    void Draw(Video::Screen& s) const;
    void AddScore(int v) { score += v; }
    int Score() const { return score; }
    int Shots() const { return shots; }
private:
    int score = 0;
    int shots = 0;
    int elapsed = 0; // ticks
    bool blink = false;
};

}

