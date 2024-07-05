#include "help.h"

namespace vchat {
Help::Help(int& now_, Function& function_, ScreenInteractive& screen_)
  : now(now_), function(function_), screen(screen_) {
  auto cmain = Container::Horizontal({
    Button(" 󰌑  Back ", [&]{ this->now = DASHBOARD; }, ButtonOption::Ascii()),
    Button(" 󰩈  Exit ", [&]{ this->screen.Exit(); }, ButtonOption::Ascii()),
  }, &main_selected);
  auto rmain = Renderer(cmain, [=]{
    return vbox(
      cmain->Render()
    );
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
