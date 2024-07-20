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

#include "help.h"

Help::Help(int& now_) : now(now_) {
  auto cmain = Container::Horizontal({
    Button(" 󰌑  返回 ", [&]{ now = DASHBOARD; }, ButtonOption::Ascii()),
    Button(" 󰩈  退出 ", [&]{ screen->Exit(); }, ButtonOption::Ascii()),
  }, &main_selected);
  auto rmain = Renderer(cmain, [=]{
    return vbox(
      paragraph_imp(graph::HELPINFO),
      cmain->Render() | center
    ) | center;
  });
  auto emain = CatchEvent(rmain, [&](Event event){
    if(event == Event::CtrlB) main_selected = 0;
    else if(event == Event::CtrlF) main_selected = 1;
    else if(event == Event::Return) return false;
    return true;
  });
  content = emain;
}
