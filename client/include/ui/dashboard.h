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

extern ScreenInteractive* screen;
extern std::unique_ptr<Function> function;

namespace dashboard {

enum { none = 0, log, sign };

class Dashboard {
private:
  int &now;

  int dialog = none;
  int choice = 0;
  InputOption option;
  int log_selected = 0;
  int sign_selected = 0;
  std::string id, username, password;

  // dialog
  Component dialog_log();
  int log_but = 0;
  Component dialog_sign();
  int sign_but = 0;

public:
  Component content;
  Dashboard(int&);
};

} // namespace dashboard

#endif // DASHBOARD_H
