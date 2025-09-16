// Minimal state machine (resource-agnostic)
#pragma once

namespace Game {

class StateMachine {
public:
    enum class Mode { Title, Playing, Paused, HighScores };
    void Reset() { mode = Mode::Title; }
    void Step(Input::Device& in);
    void Update(int ticks);
    Mode Get() const { return mode; }
    void SetMode(Mode m) { mode = m; }
    const char* Name() const;
    int Counter() const { return counter; }
    // Demo/Record flags (resource-agnostic toggles)
    bool IsDemo() const { return demo; }
    bool IsRecording() const { return record; }
    void SetDemo(bool v) { demo = v; }
    void SetRecording(bool v) { record = v; }
private:
    Mode mode = Mode::Title;
    int counter = 0; // simple gameplay counter (ticks) while playing
    bool demo = false;
    bool record = false;
};

}
