#include "tui.h"

namespace vchat {

Tui::Tui() : screen(ScreenInteractive::Fullscreen()) {
  this->mode = 1;
  this->start();
}

Tui* Tui::getinstance() {
  Tui* instance = new vchat::Tui();
  return instance;
}

void Tui::start() {
  // start
  option selected = start_option;
  auto menu_container = Container::Horizontal({
    Button("Log", [&]{ selected = log_option; }, ButtonOption::Ascii()),
    Button("Sign",[&]{ selected = sign_option; },ButtonOption::Ascii()),
    Button("Help",[&]{ selected = help_option; },ButtonOption::Ascii()),
    Button("Exit",[&]{ selected = exit_option; },ButtonOption::Ascii()),
  });

  auto start_render = Renderer(menu_container, [&](){
    return vbox({
      paragraph_imp(LOGO) | hcenter,
      paragraph_imp(TALK_START) | hcenter,
      paragraph_imp(MASCOT) | hcenter,
      menu_container->Render() | center
    }) | color(Color::Blue);
  });

  // log
  std::string logusername;
  std::string logpassword;
  InputOption password_option;
  password_option.password = true;
  auto loginputusername = Input(&logusername, "Username");
  auto loginputpassword = Input(&logpassword, "Password", password_option);

  int selected_log = 0;
  auto loginput_container = Container::Vertical({
    Renderer(loginputusername, [&]{
      return hbox({
        text("Username: "),
        loginputusername->Render(),
      });
    }),
    Renderer([]{ return text("------------------------------"); }),
    Renderer(loginputpassword, [&]{
      return hbox({
        text("Password: "),
        loginputpassword->Render(),
      });
    }),
    Renderer([]{ return text("------------------------------"); }),
  }, &selected_log);

  auto log_render = Renderer(loginput_container, [&](){
    return vbox({
      paragraph_imp(LOGO) | center,
      paragraph_imp(TALK_LOG) | center,
      paragraph_imp(MASCOT) | center,
      loginput_container->Render() | center,
    }) | color(Color::Blue);
  });

  log_render = CatchEvent(log_render, [&](Event event){
    if(getmode(event))
    {
      if (event == Event::Character('q'))
        selected = start_option;
      if (event == Event::Character('k'))
        selected_log -= 2;
      if (event == Event::Character('j'))
        selected_log += 2;
      if (event == Event::Return)
      // TODO:
        selected = start_option;
      return true;
    }
    return false;
  });

  // sign
  std::string signusername;
  std::string signpassword;
  auto signinputusername = Input(&signusername, "Username");
  auto signinputpassword = Input(&signpassword, "Password");

  int selected_sign = 0;
  auto signinput_container = Container::Vertical({
    Renderer(signinputusername, [&]{
      return hbox({
        text("Username: "),
        signinputusername->Render(),
      });
    }),
    Renderer([]{ return text("------------------------------"); }),
    Renderer(signinputpassword, [&]{
      return hbox({
        text("Username: "),
        signinputpassword->Render(),
      });
    }),
    Renderer([]{ return text("------------------------------"); }),
  }, &selected_sign);

  auto sign_render = Renderer(signinput_container, [&](){
    return vbox({
      paragraph_imp(LOGO) | center,
      paragraph_imp(TALK_SIGN) | center,
      paragraph_imp(MASCOT) | center,
      signinput_container->Render() | center
    }) | color(Color::Blue);
  });

  sign_render = CatchEvent(sign_render, [&](Event event){
    if(getmode(event))
    {
      if (event == Event::Character('q'))
        selected = start_option;
      if (event == Event::Character('k'))
        selected_sign -= 2;
      if (event == Event::Character('j'))
        selected_sign += 2;
      if (event == Event::Return)
        selected = start_option;
      return true;
    }
    return false;
  });

  // Help
  // why "bool focuse"? see more for 
  // https://github.com/ArthurSonzogni/FTXUI/issues/623
  auto help_render = Renderer([&](bool focuse){ 
    return vbox({
      paragraph_imp(LOGO) | hcenter,
      paragraph_imp(HELP) | hcenter,
    }) | hcenter;
  }) | center | color(Color::Blue);
  
  help_render = CatchEvent(help_render, [&](Event event){
    if(event == Event::Character('q'))
    {
      selected = start_option;
      return true;
    }
    return false;
  });

  // main
  auto main_container = Container::Tab({
    start_render,
    log_render,
    sign_render,
    help_render,
  }, (int *)&selected);

  auto main_render = Renderer(main_container, [&](){
    Element show = NULL;
    switch (selected)
    {
      case start_option: 
        show = vbox({
          start_render->Render(),
        }) | center;
        break;
      case log_option: 
        show = vbox({
          log_render->Render() | center,
        }) | center;
        break;
      case sign_option: 
        show = vbox({
          sign_render->Render() | center,
        }) | center;
        break;
      case help_option:
        show = help_render->Render() | center;
        break;
      case exit_option:
        show = text("Exit");
        screen.Exit();
        break;
    }
    return show | borderLight | color(Color::Blue);
  });

  screen.Loop(main_render);
}

// offered by ftxui author see more for https://github.com/ArthurSonzogni/FTXUI/issues/336
Elements Tui::split(std::string the_text) {
  Elements output;
  std::stringstream ss(the_text);
  std::string word;
  while (std::getline(ss, word, '\n'))
    output.push_back(text(word));
  return output;
}

Element Tui::paragraph_imp(std::string the_text) {
  Elements lines;
  for(Element &line : split(std::move(the_text)))
    lines.push_back(line);
  return vbox(std::move(lines));
}

bool Tui::getmode(Event &event) {
  if(event == Event::Escape)
    return mode = mode ? 0 : 1;
  return mode;
}

} // namespace vchat
