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

#include "tui.h"

ScreenInteractive* screen;

Tui::Tui(std::string language_)
  : language(language_) {
  screen = new ScreenInteractive(ScreenInteractive::Fullscreen());
  this->now_page = DASHBOARD;
  dashboard = std::make_unique<dashboard::Dashboard>(now_page);
  help = std::make_unique<Help>(now_page);
  about = std::make_unique<About>(now_page);
  auto cpages = Container::Tab({
    dashboard->content,
    help->content,
    about->content
  }, &now_page);
  auto rpages = Renderer(cpages, [=]{ return cpages->Render() | flex | color(Color::Blue); });
  auto epages = CatchEvent(rpages, [&](Event event){
    if(event == Event::Special("login_suc")) {
      LOG(INFO) << "Catch a event login_suc";
      chat = std::make_unique<vchat::Vchat>(now_page);
      cpages->Add(chat->content);
    }
    return false;
  });
  this->content = epages;
  now_page = DASHBOARD;
  std::thread t([&]{
    function->postevent = std::bind(&Tui::postevent, this, std::placeholders::_1);
    function->start();
  });
  screen->Loop(content);
  function->end();
  t.join();
}

// function post event to tui to make it update
void Tui::postevent(std::string ss) {
  LOG(INFO) << "post a event: " << ss;
  screen->PostEvent(Event::Special(ss));
}
