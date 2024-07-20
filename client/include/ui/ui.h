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

enum page { DASHBOARD = 0, HELP, ABOUT, CHAT };

namespace graph {
static std::string SMALL_COW = "\
    这里一片空白...                \n\
                   .  ,__,         \n\
                    . (oo)____     \n\
                     .(__)    )\\  \n\
                         ||--||     ";

static std::string LOGO = "\
     ██╗   ██╗ ██████╗██╗  ██╗ █████╗ ████████╗ \n\
     ██║   ██║██╔════╝██║  ██║██╔══██╗╚══██╔══╝ \n\
     ██║   ██║██║     ███████║███████║   ██║    \n\
     ╚██╗ ██╔╝██║     ██╔══██║██╔══██║   ██║    \n\
      ╚████╔╝ ╚██████╗██║  ██║██║  ██║   ██║    \n\
       ╚═══╝   ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝    \n\
                                                \n\
                VChat 欢迎您...                 \n\
                                                \n";

static std::string HELPINFO = "\
                                                  \n\
                  食用说明书                      \n\
                                                  \n\
一· 操作                                          \n\
  上                                    Ctrl + p \n\
  下                                    Ctrl + n \n\
  左                                    Ctrl + b \n\
  右                                    Ctrl + f \n\
  确定                                  Enter    \n\
                                                  \n \n \n \n \n \n \n \n";

static std::string ABOUTINFO = "\
                                   关于                                          \n\
碎碎念:                                                                          \n\
在功能上，本项目完成了基本的登陆注册，查询，添加和删除好友，与好友聊天等基础功能 \n\
在测试上，测试覆盖率高达10%，基本没咋测试，仅仅是功能串起来好使罢了。            \n\
在安全上，没有安全。                                                             \n\
在性能上... 等等，还有性能？                                                     \n\
                                                                                 \n\
有关FTXUI：                                                                      \n\
在这里强推一波 FTXUI，该库确实不错，非常适合像我这种C++新手阅读                  \n\
而且尽管是TUI库，但很多效果都能实现，并且该库作者较为友好，提的issue都会认真回复 \n\
在这里感谢FTXUI库的作者，他回答了我几个issue，要不然也不会有这个项目，hhh        \n\
项目地址在这里：https://github.com/ArthurSonzogni/FTXUI，感兴趣的小伙伴可以去看看\n\
                                                                                 \n\
                                                                                 \n\n\n\n\n\
                                                                菜狗作者: 执着   \n";

} // namespace graph
extern Elements split(std::string);
extern Element paragraph_imp(std::string);

#endif // UI_H
