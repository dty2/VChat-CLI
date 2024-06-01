#ifndef CHAT_H
#define CHAT_H

#include "common.h"
#include "function.h"

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
  chat_page = 0,
  friend_page,
  user_page,
};

class Chat {
private:
  int input_toggle; // 0: NORMAL, 1: INSERT
  int search_toggle; // 0: close tools dialog 1: open tools dialog
  int selected_page; // context select
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
