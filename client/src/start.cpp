#include "start.h"

namespace vchat {
// c: component
// e: event
// r: render
// cer: component + event + render
// example: cer_main
Start::Start() : screen(ScreenInteractive::FitComponent()) {
  int selected = start_page;
  InputOption option;
  option.password = true;
  // -------------------
  // log, sign info
  // -------------------
  std::string log_id;
  std::string log_password;
  auto c_logid = Input(&log_id, "id");
  auto c_logpassword = Input(&log_password, "password", option);
  // -------------------
  std::string sign_id;
  std::string sign_username;
  std::string sign_password;
  auto c_signid = Input(&sign_id, "id");
  auto c_signusername = Input(&sign_username, "username");
  auto c_signpassword = Input(&sign_password, "password", option);

  // -------------------
  //         log
  // -------------------
  int selected_log = 0;
  auto c_loginput = Container::Vertical({
    Renderer(c_logid, [&]{
      return hbox({ text("      [ID]: "), c_logid->Render() });
    }),
    Renderer(c_logpassword, [&]{
      return hbox({ text("[PASSWORD]: "), c_logpassword->Render() });
    }),
  }, &selected_log);
  auto ce_loginput = CatchEvent(c_loginput, [&](Event event){
    if(event == Event::CtrlK) {
      c_logid->TakeFocus();
      return true;
    } else if(event == Event::CtrlJ) {
      c_logpassword->TakeFocus();
      return true;
    } else if(event == Event::CtrlY) {
      if(!log_id.empty() && !log_password.empty()) {
        if(Function::function->do_login(std::stoi(log_id), std::stoi(log_password))) {
          screen.Exit();
          Chat::getinstance();
        }
      }
      return true;
    } else return false;
  });

  // -------------------
  //       sign
  // -------------------
  int selected_sign = 0;
  auto c_signinput = Container::Vertical({
    Renderer(c_signid, [&]{
      return hbox({ text("      [ID]: "), c_signid->Render() });
    }),
    Renderer(c_signusername, [&]{
      return hbox({ text("[USERNAME]: "), c_signusername->Render() });
    }),
    Renderer(c_signpassword, [&]{
      return hbox({ text("[PASSWORD]: "), c_signpassword->Render() });
    }),
  }, &selected_sign);
  auto ce_signinput = CatchEvent(c_signinput, [&](Event event){
    if(event == Event::CtrlK) {
      switch (selected_sign) {
        case 0: c_signid->TakeFocus();       break;
        case 1: c_signid->TakeFocus();       break;
        case 2: c_signusername->TakeFocus(); break;
      }
      return true;
    } else if(event == Event::CtrlJ) {
      switch (selected_sign) {
        case 0: c_signusername->TakeFocus();  break;
        case 1: c_signpassword ->TakeFocus(); break;
        case 2: c_signpassword->TakeFocus();  break;
      }
      return true;
    } else if(event == Event::CtrlY) {
      if(!log_id.empty() && !log_password.empty()) {
        Function::function->do_signin(std::stoi(sign_id), std::stoi(sign_password), sign_username);
      }
      return true;
    }
    return false;
  });

  // -------------------
  //        change
  // -------------------
  auto c_change = Container::Tab({
    Renderer([]{ return paragraph_imp(START); }),
    ce_loginput,
    ce_signinput,
    Renderer([]{ return paragraph_imp(START); }),
  }, &selected);

  // -------------------
  //         left
  // -------------------
  auto cr_left = Renderer(c_change, [&]{
    Element show = NULL;
    switch (selected) {
      case start_page:
        show = vbox(paragraph_imp(START), text("\n")); break;
      case log_page:
        show = vbox(c_change->Render(),text("\n")); break;
      case sign_page:
        show = c_change->Render(); break;
      case quit:
        show = vbox(paragraph_imp(QUIT), paragraph_imp("\n\n"));
        screen.Exit();
        break;
    }
    return vbox(show, paragraph_imp(COWSAY));
  }) | size(WIDTH, EQUAL, 36) | vcenter;

  // -------------------
  //        right
  // -------------------
  auto c_right = Container::Vertical({
    Button(" LOG   ", [&]{ selected = log_page;   cr_left->TakeFocus(); }, ButtonOption::Ascii()),
    Button(" SIGN  ", [&]{ selected = sign_page;  cr_left->TakeFocus(); }, ButtonOption::Ascii()),
    Button(" HELP  ", [&]{ selected = help_page;  cr_left->TakeFocus(); }, ButtonOption::Ascii()),
    Button(" ABOUT ", [&]{ selected = about_page; cr_left->TakeFocus(); }, ButtonOption::Ascii()),
    Button(" QUIT  ", [&]{ selected = quit;                             }, ButtonOption::Ascii())
  }) | size(WIDTH, EQUAL, 10) | vcenter;

  // -------------------
  //         main
  // -------------------
  auto c_main = Container::Horizontal({
    cr_left, Renderer([]{ return vbox(paragraph_imp(SPLIT)); }), c_right
  });
  auto ce_main = CatchEvent(c_main, [&](Event event){
    if(event == Event::CtrlH) { // this function not work... I don't know why...
      cr_left->TakeFocus();
      return true;
    } else if(event == Event::CtrlL) {
      c_right->TakeFocus();
      return true;
    } else return false;
  });
  auto cer_main = Renderer(ce_main, [&]{
    return window(text(" VChat ") | center, ce_main->Render()) | color(Color::Blue);
  });

  // -------------------
  //    start loop
  // -------------------
  c_right->TakeFocus();
  screen.Loop(cer_main);
}

} // namespace vchat
