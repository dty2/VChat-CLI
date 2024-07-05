#ifndef Help_H
#define Help_H

#include "ui.h"

namespace vchat {

using namespace ftxui;

class Help {
private:
  int &now;
  Function& function;
  ScreenInteractive& screen;
  int main_selected = 0;
public:
  Component content;
  Help(int&, Function&, ScreenInteractive&);
};

} // namespace vchat

#endif // Help_H

