#include "about.h"

namespace vchat {
About::About(std::function<void(int)> fun) {
  pfun = std::make_shared<std::function<void(int)>>(fun);
  // main
  auto cmain = Container::Vertical({
    Button("about test", [&]{ ;  }, ButtonOption::Ascii()),
    Button("about test2 ", [&]{ ; }, ButtonOption::Ascii()),
    Button("about change", [&]{ (*pfun)(0); }, ButtonOption::Ascii()),
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

