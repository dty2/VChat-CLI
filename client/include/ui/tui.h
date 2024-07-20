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
#include "function.h"

using namespace ftxui;

class Tui {
private:
  std::string language;
  int now_page;
  std::unique_ptr<dashboard::Dashboard> dashboard;
  std::unique_ptr<vchat::Vchat> chat;
  std::unique_ptr<Help> help;
  std::unique_ptr<About> about;
  void changewindow(int);
  Component content;

public:
  void postevent(std::string); // post a new event
  Tui(std::string);
};

extern std::unique_ptr<Function> function;

#endif // TUI_H
