#include "about.h"

namespace vchat {
About::About(int& now_, Function& function_, ScreenInteractive& screen_)
  : now(now_), function(function_), screen(screen_) {
  auto cmain = Container::Horizontal({
    Button(" 󰌑  返回 ", [&]{ this->now = DASHBOARD; }, ButtonOption::Ascii()), // 0: dashboard
    Button(" 󰩈  退出 ", [&]{ this->screen.Exit(); }, ButtonOption::Ascii()),
  }, &main_selected);
  auto rmain = Renderer(cmain, [=]{
    return vbox(
      paragraph_imp(graph::ABOUTINFO),
      cmain->Render() | center
    ) | center;
  });
  auto emain = CatchEvent(rmain, [&](Event event){
    if(event == Event::CtrlB) {
      main_selected = 0;
      return true;
    } else if(event == Event::CtrlF) {
      main_selected = 1;
      return true;
    } else return false;
  });
  content = emain;
}

} // namespace vchat
