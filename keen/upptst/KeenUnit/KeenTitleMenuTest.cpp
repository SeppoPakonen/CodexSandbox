#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenTitleMenuTests() {
    Game::TitleMenu menu; menu.Reset();
    Input::Device& in = Input::Device::Default();
    in.MapKey(Input::Device::Key::Up, 90);
    in.MapKey(Input::Device::Key::Down, 91);
    in.MapKey(Input::Device::Key::Jump, 92);
    in.PushEvent(92, true); in.Poll();
    ASSERT(menu.Step(in) == Game::TitleMenu::Action::Start);
    in.PushEvent(91, true); in.Poll(); menu.Step(in);
    in.PushEvent(92, true); in.Poll();
    ASSERT(menu.Step(in) == Game::TitleMenu::Action::StartDemo);
}

END_UPP_NAMESPACE

