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

#ifndef TUI_H
#define TUI_H

#include "ui.h"
#include "dashboard.h"
#include "vchat.h"
#include "help.h"
#include "about.h"

using namespace ftxui;

enum dialog { option = 0, quit };

class Tui {
private:
  std::string language;
  int now_page;
  ScreenInteractive screen;
  Function function;
  std::unique_ptr<dashboard::Dashboard> window_dashboard = nullptr;
  std::unique_ptr<Vchat> window_chat = nullptr;
  std::unique_ptr<Help> window_help = nullptr;
  std::unique_ptr<About> window_about = nullptr;
  void changewindow(int);
  Component content;
  void postevent(std::string); // 接收消息，好友申请

public:
  Tui(std::string, std::string, std::string);
};

#endif // TUI_H
