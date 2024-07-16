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

#include "dashboard.h"

namespace dashboard {

Dashboard::Dashboard(int& now_, Function& function_, ScreenInteractive& screen_)
  : now(now_), function(function_), screen(screen_) {
  option.password = true;
  auto cchoice = Container::Vertical({
    Button( "   登陆 ", [&] { dialog = dashboard::log; }, ButtonOption::Ascii()),
    Button( "   注册 ", [&] { dialog = dashboard::sign; }, ButtonOption::Ascii()),
    Button( " 󰞋  帮助 ", [&] { this->now = HELP; }, ButtonOption::Ascii()), // 2: help window
    Button( "   关于 ", [&] { this->now = ABOUT; }, ButtonOption::Ascii()), // 3: about window
    Button( " 󰩈  退出 ", [&] { this->screen.Exit(); }, ButtonOption::Ascii()),
  }, &choice);
  auto echoice = CatchEvent(cchoice, [&](Event event) {
    if(event == Event::CtrlP) { if(choice) choice --; return true; }
    else if(event == Event::CtrlN) { if(choice != 4) choice ++; return true; }
    else if (event == Event::Return) { return false; }
    else return true;
  });
  auto rchoice = Renderer(echoice, [=] {
    return vbox({ paragraph_imp(graph::LOGO), echoice->Render() | center }) | center;
  });
  // main
  auto cmain = Container::Tab({ rchoice, dialog_log(), dialog_sign() }, &dialog);
  auto rmain = Renderer(cmain, [=] {
    Element document = rchoice->Render();
    switch (dialog) {
    case dashboard::none: break;
    case dashboard::log:
      document = dbox( document,
        window(text(" 登陆 "), dialog_log()->Render())
        | color(Color::Yellow)
        | size(WIDTH, EQUAL, 36) | size(HEIGHT, EQUAL, 5)
        | clear_under | center
      );
      break;
      case dashboard::sign:
      document = dbox( document,
        window(text(" 注册 "), dialog_sign()->Render())
        | color(Color::Yellow)
        | size(WIDTH, EQUAL, 36) | size(HEIGHT, EQUAL, 6)
        | clear_under | center
      );
      break;
    }
    return document;
  });
  content = rmain;
}

Component Dashboard::dialog_log() {
  auto cid = Input(&id, "请输入账号") | size(WIDTH, EQUAL, 20);
  auto cpassword = Input(&password, "请输入密码", option) | size(WIDTH, EQUAL, 20);;
  auto cinput = Container::Vertical({
    Renderer(cid, [=]{ return hbox(text("[ 账号 ]: "), cid->Render()); }),
    Renderer(cpassword, [=]{ return hbox(text("[ 密码 ]: "), cpassword->Render()); }),
    Container::Horizontal({
      Button( " 确定 ", [&] {
        if(!id.empty() && !password.empty()) {
          function.login(std::stoi(id), std::stoi(password));
          id = ""; password = "";
        }
      }, ButtonOption::Ascii()),
      Button(" 取消 ", [&]{ id = ""; password = ""; dialog = dashboard::none; }, ButtonOption::Ascii())
    }, &log_but) | center
  }, &log_selected) | center | vcenter;
  auto einput = CatchEvent(cinput, [&](Event event){
    if(event == Event::CtrlP) { if(log_selected) log_selected --; return true; }
    else if(event == Event::CtrlN) { if(log_selected != 2) log_selected ++; return true; }
    else if(event == Event::CtrlB) { if(log_selected == 2) log_but = 0; return true; }
    else if(event == Event::CtrlF) { if(log_selected == 2) log_but = 1; return true; }
    else if(event == Event::Special("login_suc")) { this->now = CHAT; return true; }
    return false;
  });
  return einput;
}

Component Dashboard::dialog_sign() {
  auto cid = Input(&id, "请输入账号") | size(WIDTH, EQUAL, 20);
  auto cusername = Input(&username, "请输入用户名") | size(WIDTH, EQUAL, 20);
  auto cpassword = Input(&password, "请输入密码", option) | size(WIDTH, EQUAL, 20);
  auto cinput = Container::Vertical({
    Renderer(cid,       [=]{ return hbox(text("[ 账号 ]:   "), cid->Render()); }),
    Renderer(cusername, [=]{ return hbox(text("[ 用户名 ]: "), cusername->Render()); }),
    Renderer(cpassword, [=]{ return hbox(text("[ 密码 ]:   "), cpassword->Render()); }),
    Container::Horizontal({
      Button( " 确定 ", [&] {
        if(!id.empty() && !password.empty() && !username.empty())
          function.signin(std::stoi(id), std::stoi(password), username);
        id = ""; username = ""; password = ""; dialog = dashboard::none;
        }, ButtonOption::Ascii()),
      Button( " 取消 ", [&] {
        id = ""; username = ""; password = ""; dialog = dashboard::none;
      }, ButtonOption::Ascii()),
    }, &sign_but) | center
  }, &sign_selected) | center | vcenter;
  auto einput = CatchEvent(cinput, [=](Event event){
    if(event == Event::CtrlP) { if(sign_selected) sign_selected --; return true; }
    else if(event == Event::CtrlN) { if(sign_selected != 3) sign_selected ++; return true; }
    else if(event == Event::CtrlB) { if(sign_selected == 3) sign_but = 0; return true; }
    else if(event == Event::CtrlF) { if(sign_selected == 3) sign_but = 1; return true; }
    else if(event == Event::Special("signin_suc")) { dialog = dashboard::none; return true; }
    return false;
  });
  return einput;
}

} // namespace dashboard
