#ifndef TUI_H
#define TUI_H

#include "ui.h"
#include "dashboard.h"
#include "vchat.h"
#include "help.h"
#include "about.h"

namespace vchat {

using namespace ftxui;

enum dialog { option = 0, quit };

class Tui {
private:
  int now = 0; // 当前页面是dashboard,chat,help还是about
  ScreenInteractive screen;
  std::unique_ptr<Dashboard> window_dashboard = nullptr;
  std::unique_ptr<Vchat> window_chat = nullptr;
  std::unique_ptr<Help> window_help = nullptr;
  std::unique_ptr<About> window_about = nullptr;
  void changewindow(int);
  Component main;
  void postevent(std::string); // 接收消息，好友申请

public:
  Function function;
  Tui();
};

} // namespace vchat

#endif // TUI_H
