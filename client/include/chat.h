#ifndef CHAT_H
#define CHAT_H

#include "common.h"
#include "function.h"

#include <ctime>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/flexbox_config.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>

namespace vchat {

using namespace ftxui;

enum {
  user_page = 0,
  friend_page,
  chat_page,
  tool_page,
};

class Chat {
private:
  bool mode; // 1: NORMAL, 2: INSERT
  int selected = chat_page;
  ScreenInteractive screen;
  Chat();

public:
  static Chat* chat;
  static void getinstance();
  Chat(const Chat&) = delete;
  Chat& operator=(const Chat&) = delete;

};

Elements split(std::string);
Element paragraph_imp(std::string);

} // namespace vchat

#endif // CHAT_H
