#ifndef ABOUT_H
#define ABOUT_H

#include "ui.h"

namespace vchat {

using namespace ftxui;

class About {
private:
  int &now;
  Function& function;
  ScreenInteractive& screen;
  int main_selected = 0;
public:
  Component content;
  About(int&, Function&, ScreenInteractive&);
};

} // namespace vchat

#endif // ABOUT_H
