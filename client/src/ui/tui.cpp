#include "tui.h"

namespace vchat {
Tui::Tui() : screen(ScreenInteractive::Fullscreen()) {
  window_dashboard = std::make_unique<Dashboard>(now, function, screen);
  window_chat = std::make_unique<Vchat>(now, function, screen);
  window_help = std::make_unique<Help>(now, function, screen);
  window_about = std::make_unique<About>(now, function, screen);
  std::thread t([&] {
    function.postevent = std::bind(&Tui::postevent, this, std::placeholders::_1);
    function.start();
  });
  main = Container::Tab({
    window_dashboard->content,
    window_chat->content,
    window_help->content,
    window_about->content
  }, &now);
  auto rmain = Renderer(
      main, [&] { return main->Render() | center | color(Color::Blue); });
  now = DASHBOARD;
  screen.Loop(rmain);
  function.end();
  t.join();
}

void Tui::postevent(std::string ss) { screen.PostEvent(Event::Special(ss)); }

} // namespace vchat
