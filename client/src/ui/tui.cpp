#include "tui.h"

namespace vchat {
Tui::Tui() : screen(ScreenInteractive::Fullscreen()) {
  window_dashboard = std::make_unique<Dashboard>(now, function, screen, std::bind(&Tui::getvchat, this));
  window_help = std::make_unique<Help>(now, function, screen);
  window_about = std::make_unique<About>(now, function, screen);
  std::thread t([&] {
    function.postevent = std::bind(&Tui::postevent, this, std::placeholders::_1);
    function.start();
  });
  main = Container::Tab({
    window_dashboard->content,
    window_help->content,
    window_about->content
  }, &now);
  auto rmain = Renderer(
      main, [&] { return main->Render() | flex | color(Color::Blue); });
  now = DASHBOARD;
  screen.Loop(rmain);
  function.end();
  t.join();
}

// 通讯底层受到消息后通过该函数向上反馈事件
void Tui::postevent(std::string ss) {
  screen.PostEvent(Event::Special(ss));
}

// 初始化主聊天界面，作为函数对象传递给daskboard
void Tui::getvchat() {
  window_chat = std::make_unique<Vchat>(now, function, &screen);
  main->Add(window_chat->content);
}

} // namespace vchat
