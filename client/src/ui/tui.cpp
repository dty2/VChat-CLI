#include "tui.h"

namespace vchat {
Tui::Tui() : screen(ScreenInteractive::Fullscreen()) {
  std::thread t([&] { function.start(); });
  function.postevent = std::bind(&Tui::postevent, this, std::placeholders::_1);
  main = Container::Tab({}, &now);
  auto rmain = Renderer( main, [&]{ return main->Render() | center | color(Color::Blue); });
  window_change(dashboard);
  screen.Loop(rmain);
}

void Tui::postevent(std::string ss) { screen.PostEvent(Event::Special(ss)); }

void Tui::window_change(int window) {
  this->now = window;
  switch (window) {
  case dashboard:
    if (!window_dashboard) {
      window_dashboard = std::make_unique<Dashboard>(
          std::bind(&Tui::window_change, this, std::placeholders::_1));
      main->Add(window_dashboard->content);
    };
    break;
  case chat:
    if (!window_chat) {
      window_chat = std::make_unique<Chat>(
          std::bind(&Tui::window_change, this, std::placeholders::_1));
      main->Add(window_chat->content);
    };
    break;
  case help:
    if (!window_help) {
      window_help = std::make_unique<Help>(
          std::bind(&Tui::window_change, this, std::placeholders::_1));
      main->Add(window_help->content);
    };
    break;
  case about:
    if (!window_about) {
      window_about = std::make_unique<About>(
          std::bind(&Tui::window_change, this, std::placeholders::_1));
      main->Add(window_about->content);
    };
    break;
  }
}

int Tui::window_now() { return now; }

} // namespace vchat
