// Copyright 2024 Arthur Hunter. All rights reserved.
// Use of this source code is governed by the GPL-3.0 license 
// that can be found in the LICENSE file.

#ifndef TUI_HPP
#define TUI_HPP

#include "common.hpp"
#include "logo.hpp"
#include "help.hpp"

#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/flexbox_config.hpp>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/captured_mouse.hpp>

namespace tui {

typedef enum {start_option = 0, log_option, sign_option, help_option, exit_option} option;

using namespace ftxui;

class Module
{
private:
  int mode; // command mode: 1, insert mode: 0
  bool getmode(Event&);
  Elements split(std::string);
  Element paragraph_imp(std::string);

public:
  Module();
  void start();
};

} // tui namespace end

class ContainerBase;
class VerticalContainer;

# endif // TUI_HPP END
