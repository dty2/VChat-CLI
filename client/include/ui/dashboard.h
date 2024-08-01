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

#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "function.h"
#include "ui.h"

using namespace ftxui;

extern ScreenInteractive* screen; // define at tui.cpp
extern std::unique_ptr<Function> function; // define at function.cpp

namespace dashboard {

enum { DASHBOARD = 0, HELP, ABOUT };
enum { NONE = 0, LOG, SIGN };
enum STATUS { INPUT = 0, ING, END };

class Dashboard {
private:
  struct Log {
    int errinfo;
    std::string id, password;
    int selected = 0;
    int but = 0;
    Component content;
    int status;
    Log(Dashboard*);
  }log;
  struct Sign {
    int errinfo;
    std::string id, username, password;
    int selected = 0;
    int but = 0;
    int status;
    Component content;
    Sign(Dashboard*);
  }sign;
  Component help;
  Component about;
  void init_help();
  void init_about();
  int dialog = NONE;
  int page = DASHBOARD;
  int selected = 0;

public:
  Component content; // Dashboard page
  Dashboard();
};

} // namespace dashboard

#endif // DASHBOARD_H
