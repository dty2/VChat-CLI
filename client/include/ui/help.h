#ifndef Help_H
#define Help_H

#include "ui.h"

namespace vchat {

using namespace ftxui;

class Help {
private:
  int main_selected = 0;
  std::function<void(int)> pfun;
public:
  Component content;
  Help(std::function<void(int)>);
};

} // namespace vchat

#endif // Help_H

