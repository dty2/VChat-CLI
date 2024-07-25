/*
 * Copyright (c) 2024 Hunter 执着
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef UI_H
#define UI_H

#include "common.h"
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/flexbox_config.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>

using namespace ftxui;

namespace graph {
static std::string SMALL_COW = "\
    这里一片空白...                \n\
                   .  ,__,         \n\
                    . (oo)____     \n\
                     .(__)    )\\  \n\
                         ||--||     ";

static std::string LOGO = "\
                                                    \n\n\n\n\
     ██╗   ██╗  ██████╗ ██╗  ██╗  █████╗  ████████╗ \n\
     ██║   ██║ ██╔════╝ ██║  ██║ ██╔══██╗ ╚══██╔══╝ \n\
     ██║   ██║ ██║      ███████║ ███████║    ██║    \n\
     ╚██╗ ██╔╝ ██║      ██╔══██║ ██╔══██║    ██║    \n\
      ╚████╔╝  ╚██████╗ ██║  ██║ ██║  ██║    ██║    \n\
       ╚═══╝    ╚═════╝ ╚═╝  ╚═╝ ╚═╝  ╚═╝    ╚═╝    \n\
                                                    \n\n\n";

static std::string HELPINFO = "\
                                              \n\
                 VChat Cook Book             \n\
                                              \n\
一·   window                                 \n\
  Previous                          Ctrl + p\n\
  Next                              Ctrl + n\n\
  Back                              Ctrl + b\n\
  Front                             Ctrl + f\n\
  Enter                             Enter   \n\
󱊷  Esc                               Cancel  \n\
                                              \n\
二.   telescope                              \n\
󰻞  messages                          Ctrl + j\n\
  friends                           Ctrl + k\n\
  groups                            Ctrl + l\n\
                                              \n\
                                              \n\
                                              \n";

static std::string ABOUTINFO = "\
                 󱍢  About VChat                  \n\
                                                 \n\
               TUI  󰭹  Chat    MIT             \n\
                                                 \n\
VChat is a TUI (Text User Interface) chat program\n\
licensed under the MIT License.                  \n\
The current version is 0.1.0. You can get the    \n\
latest version from its repository:              \n\
https://github.com/dty2/VChat                    \n\
                                                 \n\
I created VChat not only for my graduation       \n\
project but also out of a personal passion.      \n\
For the TUI, I used FTXUI, which can be found at \n\
https://github.com/ArthurSonzogni/FTXUI          \n\
                                                 \n\
Reference program:                               \n\
wechat, qq, vim, nvim, emacs,                    \n\
nerdtree(vim plugin), airline(vim plugin)        \n\
telescope(nvim plugin) dashboard(nvim plugin)    \n\
noise(nvim plugin), ranger, alsamixer, polybar   \n\
                                                 \n\
                            Author: 执着(Hunter) \n\
                                                 \n";

} // namespace graph
extern Component myinput(std::string*, const std::string, bool, const std::string cursor = "│");
extern Elements split(std::string);
extern Element paragraph_imp(std::string);

#endif // UI_H
