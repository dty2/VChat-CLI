#ifndef UI_H
#define UI_H

#include "common.h"
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

namespace ui {

Elements split(std::string);
Element paragraph_imp(std::string);

static std::string SPLIT = "│ \n│ \n│ \n│ \n│ \n│ \n│ \n│";

static std::string SMALL_COW = "\
                      ,__,        \n\
                      (oo)____    \n\
                      (__)    )\\  \n\
                         ||--||   ";

static std::string LOGO = "\
██╗   ██╗ ██████╗██╗  ██╗ █████╗ ████████╗ \n\
██║   ██║██╔════╝██║  ██║██╔══██╗╚══██╔══╝ \n\
██║   ██║██║     ███████║███████║   ██║    \n\
╚██╗ ██╔╝██║     ██╔══██║██╔══██║   ██║    \n\
 ╚████╔╝ ╚██████╗██║  ██║██║  ██║   ██║    \n\
  ╚═══╝   ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝    \n\
                                           \n\
                                           \n\
                                           \n\
                                           ";

static std::string START = " [ Welcome to VChat... ] ";
static std::string QUIT  = " [ Quit VChat ...] ";

}
} // namespace vchat

# endif // UI_H

