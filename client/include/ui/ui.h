#ifndef UI_H
#define UI_H

#include "common.h"
#include "function.h"
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/flexbox_config.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>

namespace vchat {

using namespace ftxui;

enum page { DASHBOARD = 0, CHAT, HELP, ABOUT };

namespace graph {
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
} // namespace graph
extern Elements split(std::string);
extern Element paragraph_imp(std::string);

} // namespace vchat

#endif // UI_H
