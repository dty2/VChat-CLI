#ifndef TUI_H
#define TUI_H

#include "common.h"
#include "function.h"
#include "ui.h"
#include "dashboard.h"
#include "chat.h"
#include "help.h"
#include "about.h"

namespace vchat {

using namespace ftxui;

enum page { dashboard = 0, chat, help, about, };
enum dialog { option = 0, quit };

class Tui {
private:
  int now = 0; // 当前页面是dashboard,chat,help还是about
  ScreenInteractive screen;
  std::unique_ptr<Dashboard> window_dashboard = nullptr;
  std::unique_ptr<Chat> window_chat = nullptr;
  std::unique_ptr<Help> window_help = nullptr;
  std::unique_ptr<About> window_about = nullptr;
  void window_change(int);
  Component main;
  void postevent(std::string); // 接收消息，好友申请
public:
  int window_now();
  Function function;
  Tui();
};

} // namespace vchat

#endif // TUI_H
