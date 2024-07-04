#include "dashboard.h"

namespace vchat {
Dashboard::Dashboard(std::function<void(int)> fun) : pfun(fun) {
  option.password = true;
  // choice
  auto cchoice = Container::Vertical({
    Button( "   登陆 ", [&] { dialog = log;  }, ButtonOption::Ascii()),
    Button( "   注册 ", [&] { dialog = sign; }, ButtonOption::Ascii()),
    Button( " 󰞋  帮助 ", [&] { this->pfun(2); }, ButtonOption::Ascii()),
    Button( "   关于 ", [&] { this->pfun(3); }, ButtonOption::Ascii()),
    Button( " 󰩈  Exit ", [&] { exit();        }, ButtonOption::Ascii()),
  }, &choice);
  auto echoice = CatchEvent(cchoice, [&](Event event) {
    if (event == Event::CtrlP) { choice = 0; return true; }
    else if (event == Event::CtrlN) { choice = 1; return true; }
    else if (event == Event::Return) { return false; }
    else return true;
  });
  auto rchoice = Renderer(echoice, [=] {
    return vbox({ ui::paragraph_imp(ui::LOGO), echoice->Render() | center });
  });
  // main
  auto cmain = Container::Tab({ rchoice, dialog_log(), dialog_sign() }, &dialog);
  auto rmain = Renderer(cmain, [=] {
    Element document = rchoice->Render();
    switch (dialog) {
    case none:  break;
    case log:
      document = dbox( document,
        window(text(" 登陆 "), dialog_log()->Render())
        | color(Color::Yellow)
        | size(WIDTH, EQUAL, 36) | size(HEIGHT, EQUAL, 5)
        | clear_under | center
      );
      break;
    case sign:
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
      Button( " 确定 ", [&] { this->pfun(1); }, ButtonOption::Ascii()), // 1 代表 chat window
      Button( " 取消 ", [&] { id = ""; password = ""; dialog = none; }, ButtonOption::Ascii())
    }, &log_but) | center
  }, &log_selected) | center | vcenter;
  auto einput = CatchEvent(cinput, [&](Event event){
    if(event == Event::CtrlP) { if(log_selected) log_selected --; return true; }
    else if(event == Event::CtrlN) { if(log_selected != 2) log_selected ++; return true; }
    else if(event == Event::CtrlB) { if(log_selected == 2) log_but = 0; return true; }
    else if(event == Event::CtrlF) { if(log_selected == 2) log_but = 1; return true; }
    else if(event == Event::Return) { return false; }
    else if(event == Event::CtrlY) {
      if(!id.empty() && !password.empty()) {}
      return true;
    } else {
      if (log_selected == 2) { return true; }
      else return false;
    }
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
      Button( " 确定 ", [&] { dialog = none; }, ButtonOption::Ascii()),
      Button( " 取消 ", [&] {
        id = ""; username = ""; password = ""; dialog = none;
      }, ButtonOption::Ascii()),
    }, &sign_but) | center
  }, &sign_selected) | center | vcenter;
  auto einput = CatchEvent(cinput, [=](Event event){
    if(event == Event::CtrlP) { if(sign_selected) sign_selected --; return true; }
    else if(event == Event::CtrlN) { if(sign_selected != 3) sign_selected ++; return true; }
    else if(event == Event::CtrlB) { if(sign_selected == 3) sign_but = 0; return true; }
    else if(event == Event::CtrlF) { if(sign_selected == 3) sign_but = 1; return true; }
    else if(event == Event::Return) { return false; }
    else if(event == Event::CtrlY) {
      if(!id.empty() && !password.empty() && !username.empty()) {}
      return true;
    } else {
      if (sign_selected == 3) { return true; }
      else return false;
    }
    return false;
  });
  return einput;
}

} // namespace vchat
