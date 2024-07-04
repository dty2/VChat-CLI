#ifndef ABOUT_H
#define ABOUT_H

#include "ui.h"

namespace vchat {

using namespace ftxui;

class About {
private:
  int main_selected = 0;
  std::shared_ptr<std::function<void(int)>> pfun;
public:
  Component content;
  About(std::function<void(int)>);
};

} // namespace vchat

#endif // ABOUT_H


