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
static std::string EMPTY = "\
空空如也 ...              \n\
            .  ,__,       \n\
             . (oo)____   \n\
               (__)    )\\\n\
                  ||--||   ";

static std::string LOGO = "\
                                                   \n\n\n\n\
    ██╗   ██╗  ██████╗ ██╗  ██╗  █████╗  ████████╗ \n\
    ██║   ██║ ██╔════╝ ██║  ██║ ██╔══██╗ ╚══██╔══╝ \n\
    ██║   ██║ ██║      ███████║ ███████║    ██║    \n\
    ╚██╗ ██╔╝ ██║      ██╔══██║ ██╔══██║    ██║    \n\
     ╚████╔╝  ╚██████╗ ██║  ██║ ██║  ██║    ██║    \n\
      ╚═══╝    ╚═════╝ ╚═╝  ╚═╝ ╚═╝  ╚═╝    ╚═╝    \n\
                                                   \n\n\n";

static std::string HELPINFO_EN = "\
                      Cook Book               \n\
                                               \n\
1·   operation                                \n\
    Previous                         Ctrl + p\n\
    Next                             Ctrl + n\n\
    Back                             Ctrl + b\n\
    Front                            Ctrl + f\n\
    Enter                            Enter   \n\
  󰜺  Escape                           Esc     \n\
                                               \n\
2.   function                                 \n\
    option                           Ctrl + k\n\
  󰻞  messages                         Ctrl + u\n\
    friends                          Ctrl + i\n\
    groups                           Ctrl + o\n\
  󱈸  inform                           Ctrl + y\n\
                                               \n\n\
                   Move like Emacs             \n\
                               .  ,__,         \n\
                                . (oo)____     \n\
                                  (__)    )\\  \n\
                                     ||--||     ";

static std::string HELPINFO_CN = "\
                     食用说明书               \n\
                                               \n\
一·   操作                                    \n\
    上    Previous                   Ctrl + p\n\
    下    Next                       Ctrl + n\n\
    左    Back                       Ctrl + b\n\
    右    Front                      Ctrl + f\n\
    确定  Enter                      Enter   \n\
  󰜺  取消  Escape                     Esc     \n\
                                               \n\
二.   功能                                    \n\
    选项                             Ctrl + k\n\
  󰻞  消息                             Ctrl + u\n\
    朋友                             Ctrl + i\n\
    群组                             Ctrl + o\n\
  󱈸  通知                             Ctrl + y\n\
                                               \n\n\
                    Emacs用户大喜              \n\
                               .  ,__,         \n\
                                . (oo)____     \n\
                                  (__)    )\\  \n\
                                     ||--||     ";

static std::string ABOUTINFO_EN = "\
                                                 \n\
                 󱍢  About VChat                  \n\
               TUI  󰭹  Chat    MIT             \n\
                                                 \n\
VChat is a TUI (Text User Interface) chat program\n\
licensed under the MIT License.                  \n\
The current version is 0.8.0. You can get the    \n\
latest version from its repository:              \n\
https://github.com/dty2/VChat                    \n\
                                                 \n\
For the TUI, I used FTXUI, which can be found at \n\
https://github.com/ArthurSonzogni/FTXUI          \n\
                                                 \n\
Reference program:                               \n\
wechat, qq, vim, nvim, emacs,                    \n\
nerdtree(vim plugin), airline(vim plugin)        \n\
telescope(nvim plugin) dashboard(nvim plugin)    \n\
noise(nvim plugin), ranger, alsamixer, polybar   \n\
                                                 \n\
                                                 \n\n\n\n\n\
                            Author: 执着(Hunter) \n\
                                                 \n";

static std::string ABOUTINFO_CN = "\
                                              \n\
                󱍢  关于 VChat                 \n\
              TUI  󰭹  Chat    MIT           \n\
                                              \n\
VChat 是一个 TUI (文本用户界面) 聊天程序      \n\
使用的协议是 MIT. 当前的版本是0.8.0.          \n\
你可以从这个仓库获得它最新的版本              \n\
https://github.com/dty2/VChat                 \n\
                                              \n\
Vchat使用FTXUI库实现TUI的界面, 你可以在这里找 \n\
到它 https://github.com/ArthurSonzogni/FTXUI  \n\
                                              \n\
参考程序:                                     \n\
微信, QQ, vim, nvim, emacs,                   \n\
nerdtree(vim 插件), airline(vim 插件)         \n\
telescope(nvim 插件) dashboard(nvim 插件)     \n\
noise(nvim 插件), ranger, alsamixer, polybar  \n\
                                              \n\
                                              \n\n\n\n\n\
                            作者: 执着(Hunter)\n\
                                              \n";
} // namespace graph

extern Component myinput(std::string*, const std::string, bool, const std::string cursor = "│");
extern Elements split(std::string);
extern Element paragraph_imp(std::string);
extern Element getbackground();

#endif // UI_H
