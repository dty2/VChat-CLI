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
#include "ui.h"

namespace dashboard {

Dashboard::Log::Log(Dashboard* dashboard) {
  auto cid = myinput(&id, "", false, "󱍢 ") | size(WIDTH, EQUAL, 20);
  auto cpassword = myinput(&password, "", true, "󱍢 ")
    | size(WIDTH, EQUAL, 20);
  auto cinput = Container::Vertical({
    Renderer(cid, [=]{
      return hbox(text("[ 账号 ]: "), cid->Render() | bgcolor(Color::Default)); }),
    Renderer(cpassword, [=]{
      return hbox(text("[ 密码 ]: "), cpassword->Render() | bgcolor(Color::Default)); }),
    Container::Horizontal({
      Button(" 登陆 ", [=]{
        if(!id.empty() && !password.empty()) {
          function->login(std::stoi(id), std::stoi(password));
          id = "";
          password = "";
          status = 1;
        }
      }, ButtonOption::Ascii()),
      Button(" 取消 ", [=]{
        id = ""; password = "";
        dashboard->dialog = NONE;
        status = 0;
      }, ButtonOption::Ascii())
    }, &but) | center
  }, &selected) | center | vcenter;
  auto clogging = Renderer([=](bool focused){ return text("登陆中...") | center; });
  auto cmain = Container::Tab({cinput, clogging}, &status);
  auto rmain = Renderer(cmain, [=]{
    Element show;
    if (!status) show = window(text(" 登陆 ") | center, cinput->Render());
    else show = window(text(" 登陆 ") | center, clogging->Render());
    return show | color(Color::Yellow)
      | size(WIDTH, EQUAL, 36) | size(HEIGHT, EQUAL, 5)
      | clear_under | center;
  });
  auto emain = CatchEvent(rmain, [&](Event event){
    if(event == Event::CtrlP) { if(selected) selected --; return true; }
    else if(event == Event::CtrlN) { if(selected != 2) selected ++; return true; }
    else if(event == Event::CtrlB) { if(selected == 2) but = 0; return true; }
    else if(event == Event::CtrlF) { if(selected == 2) but = 1; return true; }
    return false;
  });
  this->content = emain;
}

Dashboard::Sign::Sign(Dashboard* dashboard) {
  auto cid = myinput(&id, "", false, "󱍢 ")
  | size(WIDTH, EQUAL, 20) | bgcolor(Color::Default);
  auto cusername = myinput(&username, "", false, "󱍢 ")
  | size(WIDTH, EQUAL, 20) | bgcolor(Color::Default);
  auto cpassword = myinput(&password, "", true, "󱍢 ")
  | size(WIDTH, EQUAL, 20) | bgcolor(Color::Default);
  auto cinput = Container::Vertical({
    Renderer(cid, [=]{
      return hbox(text("[  账号  ]: "), cid->Render() | bgcolor(Color::Default)); }),
    Renderer(cusername, [=]{
      return hbox(text("[ 用户名 ]: "), cusername->Render() | bgcolor(Color::Default)); }),
    Renderer(cpassword, [=]{
      return hbox(text("[  密码  ]: "), cpassword->Render() | bgcolor(Color::Default)); }),
    Container::Horizontal({
      Button( " 注册 ", [=] {
        if(!id.empty() && !password.empty() && !username.empty())
          function->signin(std::stoi(id), std::stoi(password), username);
        id = ""; username = ""; password = "";
        }, ButtonOption::Ascii()),
      Button( " 取消 ", [=] {
        id = ""; username = ""; password = ""; dashboard->dialog = NONE;
      }, ButtonOption::Ascii()),
    }, &but) | center
  }, &selected) | center | vcenter;
  auto csigning = Renderer([=](bool focused){ return text("注册中...") | center; });
  auto cmain = Container::Tab({cinput, csigning}, &status);
  auto rmain = Renderer(cmain, [=]{
    Element show;
    if (!status) show = window(text(" 注册 ") | center, cinput->Render());
    else show = window(text(" 注册 ") | center, csigning->Render());
    return show | color(Color::Yellow)
      | size(WIDTH, EQUAL, 36) | size(HEIGHT, EQUAL, 6)
      | clear_under | center;
  });
  auto emain = CatchEvent(rmain, [&](Event event){
    if(event == Event::CtrlP) { if(selected) selected --; return true; }
    else if(event == Event::CtrlN) { if(selected != 3) selected ++; return true; }
    else if(event == Event::CtrlB) { if(selected == 3) but = 0; return true; }
    else if(event == Event::CtrlF) { if(selected == 3) but = 1; return true; }
    return false;
  });
  this->content = emain;
}

void Dashboard::init_help() {
  auto cmain = Button(" 󰌑  返回 ", [&]{ page = DASHBOARD; }, ButtonOption::Ascii());
  auto rmain = Renderer(cmain, [=]{
    return vbox( paragraph_imp(graph::HELPINFO_CN), cmain->Render() | center) | center;
  });
  auto emain = CatchEvent(rmain, [&](Event event){
    if(event == Event::Return) return false;
    return true;
  });
  this->help = emain | color(Color::Blue);
}

void Dashboard::init_about() {
  auto cmain = Button(" 󰌑  返回 ", [&]{ page = DASHBOARD; }, ButtonOption::Ascii());
  auto rmain = Renderer(cmain, [=]{
    return vbox( paragraph_imp(graph::ABOUTINFO_CN), cmain->Render() | center) | center;
  });
  auto emain = CatchEvent(rmain, [&](Event event){
    if(event == Event::Return) return false;
    return true;
  });
  this->about = emain | color(Color::Blue);
}

Dashboard::Dashboard() : log(this), sign(this) {
  init_about();
  init_help();
  auto cchoice = Container::Vertical({
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  󰌆  登陆   ")) | color(Color::Yellow);
      else return hbox(text("   󰌆  登陆   ") | color(Color::Blue));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  󰗛  注册   ")) | color(Color::Yellow);
      else return hbox(text("   󰗛  注册   ") | color(Color::Blue));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  󰞋  帮助   ")) | color(Color::Yellow);
      else return hbox(text("   󰞋  帮助   ") | color(Color::Blue));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢    关于   ")) | color(Color::Yellow);
      else return hbox(text("     关于   ") | color(Color::Blue));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  󰩈  退出   ")) | color(Color::Yellow);
      else return hbox(text("   󰩈  退出   ") | color(Color::Blue));
    }),
  }, &selected);
  auto rchoice = Renderer(cchoice, [=]{
    return vbox(
      paragraph_imp(graph::LOGO) | color(Color::Blue),
      cchoice->Render() | center,
      text("") | center, text("") | center,
      text("按 ctrl + n/p 上下切换选项...") | color(Color::Blue) | center
    ) | center;
  });
  auto echoice = CatchEvent(rchoice, [&](Event event) {
    if(event == Event::CtrlN) { if(selected != 4) selected ++; return true; }
    else if(event == Event::CtrlP) { if(selected) selected --; return true; }
    else if (event == Event::Return) {
      switch (selected) {
        case 0 : dialog = LOG; break;
        case 1 : dialog = SIGN; break;
        case 2 : page = HELP; break;
        case 3 : page = ABOUT; break;
        case 4 : screen->Exit(); break;
      }
      return true;
    }
    return true;
  });
  auto cdialog = Container::Tab({ echoice, log.content, sign.content }, &dialog);
  auto rdialog = Renderer(cdialog, [=]{
    Element document = rchoice->Render();
    switch (dialog) {
    case DASHBOARD: break;
    case LOG: document = dbox(document, log.content->Render()); break;
    case SIGN: document = dbox(document, sign.content->Render()); break;
    }
    return document;
  });
  auto cmain = Container::Tab({ rdialog, help, about }, &page);
  auto rmain = Renderer(cmain, [=]{
    return cmain->Render();
  });
  auto emain = CatchEvent(rmain, [&](Event event){
    return false;
  });
  this->content = emain;
}

} // namespace dashboard
