#include "help.h"

namespace vchat {
Help::Help(std::function<void(int)> fun) {
  auto cmain = Container::Vertical({
    Button(" 󰩈  Exit ", [&]{ pfun(0); }, ButtonOption::Ascii()),
    Button(" 󰌑  Back ", [&]{ pfun(0); }, ButtonOption::Ascii()),
  }, &main_selected);
  auto emain = CatchEvent(cmain, [&](Event event){
    if(event == Event::CtrlP) {
      main_selected = 0;
      return true;
    } else if(event == Event::CtrlN) {
      main_selected = 1;
      return true;
    } else return false;
  });
  content = emain;
}

} // namespace vchat
