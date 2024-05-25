#ifndef START_H
#define START_H

#include "common.h"
#include "cowsay.h"
#include "function.h"

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

using namespace ftxui;

enum {
  start_page = 0,
  log_page,
  sign_page,
  help_page,
  about_page,
  quit
};

class Start {
private:
  bool mode = 1;
  int selected = start_page;
  ScreenInteractive screen;

public:
  Start();
};

Elements split(std::string);
Element paragraph_imp(std::string);

} // namespace vchat

# endif // START_H
