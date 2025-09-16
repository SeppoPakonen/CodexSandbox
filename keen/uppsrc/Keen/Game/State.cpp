#include "Game.h"

NAMESPACE_UPP

namespace Game {

void StateMachine::Step(Input::Device& in) {
    switch (mode) {
    case Mode::Title:
        if (in.WasPressed(Input::Device::Key::Jump)) mode = Mode::Playing;
        break;
    case Mode::Playing:
        if (in.WasPressed(Input::Device::Key::Up)) mode = Mode::Paused;
        break;
    case Mode::Paused:
        if (in.WasPressed(Input::Device::Key::Up)) mode = Mode::Playing;
        break;
    }
    // Toggle demo/record with Down/Right presses irrespective of mode
    if (in.WasPressed(Input::Device::Key::Down)) demo = !demo;
    if (in.WasPressed(Input::Device::Key::Right)) record = !record;
}

const char* StateMachine::Name() const {
    switch (mode) {
    case Mode::Title: return "TITLE";
    case Mode::Playing: return "PLAY";
    case Mode::Paused: return "PAUSE";
    }
    return "?";
}

void StateMachine::Update(int ticks) {
    if (mode == Mode::Playing)
        counter += ticks;
}

}

END_UPP_NAMESPACE
