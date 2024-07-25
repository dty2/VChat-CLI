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
extern std::unique_ptr<Function> function;

namespace dashboard {

enum { DASHBOARD = 0, HELP, ABOUT };
enum { NONE = 0, LOG, SIGN };

class Dashboard {
private:
  struct Log {
    std::string id, password;
    int selected = 0; // choose which input box
    int but = 0; // choose which button
    Component content;
    int status; // logging or cancel
    Log(Dashboard*);
  }log; // log dialog
  struct Sign {
    std::string id, username, password;
    int selected = 0; // choose which input box
    int but = 0; // choose which button
    int status; // signing or cancel
    Component content;
    Sign(Dashboard*);
  }sign; // sign dialog
  Component help; // Help page
  Component about; // About page
  void init_help(); // init Help page
  void init_about(); // init about page
  int dialog = NONE; // choose log dialog or sign dialog
  int page = DASHBOARD; // choose Dashboard page or About page or Help page
  int selected = 0;

public:
  Component content; // Dashboard page
  Dashboard();
};

} // namespace dashboard

#endif // DASHBOARD_H
