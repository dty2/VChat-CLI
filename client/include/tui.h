#ifndef TUI_H
#define TUI_H

#include "common.h"
#include "picture.h"
#include "help.h"

#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/flexbox_config.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/captured_mouse.hpp>

namespace vchat {

typedef enum {start_option = 0, log_option, sign_option, help_option, exit_option} option;

using namespace ftxui;

class Tui {
private:
  int mode; // command mode: 1, insert mode: 0
  bool getmode(Event&);
  Elements split(std::string);
  Element paragraph_imp(std::string);
  Tui();

public:
  ScreenInteractive screen;
  static Tui* getinstance();
  Tui(const Tui&) = delete;
  Tui& operator=(const Tui&) = delete;
  void start();
};

class ContainerBase;
class VerticalContainer;

} // namespace vchat

# endif // TUI_H
