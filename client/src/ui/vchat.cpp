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

#include "vchat.h"
#include "dashboard.h"
#include "telescope.h"
#include "ui.h"

// chat list
Chat::List::List(Chat *chat) {
  this->selected = Info::info->messageinfo[chat->id].size();
  cmain = Container::Vertical({}, &selected);
  auto rmain = Renderer(cmain, [=]{
    std::string name = Info::info->friendinfo[chat->id].friendname;
    cmain->DetachAllChildren();
    if (!chat->selected) {
      cmain->Add(
        Renderer([=](bool focused){
          Element element = text(
            "您已添加" + name + "，现在可以开始聊天了"
          ) | center;
          if(focused) element |= focus;
          return element;
        })
      );
      for(auto& value : Info::info->messageinfo[chat->id]) {
        cmain->Add(
          Renderer([=](bool focused){
            Element element;
            if(focused) {
              if(value.sender == chat->id) {
                element = hbox(text("  $ " + value.msg), filler())
                | color(Color::Yellow) | bgcolor(Color::Blue);
              } else if(value.receiver == chat->id){
                element = hbox(filler(), text(value.msg + " #     "))
                | color(Color::Yellow) | bgcolor(Color::Blue);
              }
              element |= focus;
            } else {
              if(value.sender == chat->id) {
                element = hbox(text("  $ "), text(value.msg), filler());
              } else if(value.receiver == chat->id){
                element = hbox(filler(), text(value.msg), text(" #     "));
              }
            }
            return element | size(WIDTH, EQUAL, 54) | size(HEIGHT, EQUAL, 1);
          })
        );
      }
    } else {
      for(auto it = Info::info->messageinfo[chat->id].end() - 21;
        it != Info::info->messageinfo[chat->id].end(); it ++) {
        cmain->Add(
          Renderer([=](bool focused){
            Element element;
            if(focused) {
              if(it->sender == chat->id) {
                element = hbox(text("  $ " + it->msg), filler())
                | color(Color::Yellow) | bgcolor(Color::Blue);
              } else if(it->receiver == chat->id){
                element = hbox(filler(), text(it->msg + " #     "))
                | color(Color::Yellow) | bgcolor(Color::Blue);
              }
              element |= focus;
            } else {
              if(it->sender == chat->id) {
                element = hbox(text("  $ "), text(it->msg), filler());
              } else if(it->receiver == chat->id){
                element = hbox(filler(), text(it->msg), text(" #     "));
              }
            }
            return element | size(WIDTH, EQUAL, 54) | size(HEIGHT, EQUAL, 1);
          })
        );
      }
    }
    return cmain->Render() | vscroll_indicator | frame | color(Color::Blue)
      | size(WIDTH, EQUAL, 54) | size(HEIGHT, EQUAL, 24);
  });
  auto emain = CatchEvent(rmain, [=](Event event){
    if(event == Event::CtrlN) {
      if(selected != Info::info->messageinfo[chat->id].size()) selected ++;
      return true;
    } else if(event == Event::CtrlP) {
      if(selected != 1) selected --;
      return true;
    }
    return false;
  });
  this->content = emain;
}

// chat input
Chat::Input::Input(Chat *chat) {
  auto cinput = myinput(&ss, "按CtrlY发送消息", false, "󱍢 ");
  auto rinput = Renderer(cinput, [=]{
   return hbox(text("   > "), cinput->Render())
    | size(WIDTH, EQUAL, 54) | size(HEIGHT, EQUAL, 3);
  });
  auto einput = CatchEvent(rinput, [=](Event event){
    if(event == Event::Return) {
      std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm* now_tm = std::localtime(&now);
      std::ostringstream oss;
      oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_year - 100) // year
          << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_mon + 1) // month
          << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_mday) // date
          << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_hour) // hour
          << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_min) // minute
          << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_sec); // second
      if(!ss.empty())
        function->sendmsg(chat->id, Info::info->myself.id, ss,
          static_cast<int64_t>(std::stoll(oss.str())));
        ss.clear();
        chat->list.content->TakeFocus();
        return true;
    } else return false;
  });
  this->content = einput;
}

// chat
Chat::Chat(int id_) : id(id_), list(this), input(this) {
  this->selected = 0;
  auto cchat = Container::Vertical({list.content, input.content }, &selected);
  auto rchat = Renderer(cchat, [=]{
    Element show = window(
      text(" 󱍢 " + Info::info->friendinfo[id].friendname + " "),
      vbox(list.content->Render(), separator(), input.content->Render())
      , LIGHT) | vcenter | center;
    if (cchat->Focused()) show |= color(Color::Yellow);
    return show | color(Color::Blue);
  });
  auto echat = CatchEvent(rchat, [=](Event event){
    if(event == Event::CtrlN || event == Event::CtrlP) {
      if (selected) {
        list.content->TakeFocus();
        selected = 0;
        return true;
      }
      return false;
    } else {
      selected = 1;
      input.content->TakeFocus();
    }
    return false;
  });
  this->content = echat | size(WIDTH, EQUAL, 54) | size(HEIGHT, EQUAL, 27);
}

void Vchat::open_chat(int id) {
  if (pages->ChildCount() == 2)
    pages->ChildAt(0)->Detach();
  selected = 0;
  telescope->selected = INPUT;
  telescope->list_selected = -1;
  page_selected = pages->ChildCount() - 1;
  pages->Add(chats[id]->content);
}

void Vchat::open_other(bool target) {
  if (pages->ChildCount() == 2)
    pages->ChildAt(0)->Detach();
  selected = 0;
  telescope->selected = INPUT;
  telescope->list_selected = -1;
  page_selected = pages->ChildCount() - 1;
  if (target) {
    pages->Add(about);
    return;
  }
  pages->Add(help);
}

// init all page
void Vchat::init_page() {
  page_selected = 0;
  for(auto& v : Info::info->friendinfo)
    chats[v.first] = new Chat(v.first);
  this->about = Renderer([](bool focused){
    Element show = paragraph_imp(graph::ABOUTINFO_CN) | color(Color::Blue)
    | center | vcenter | borderLight | size(WIDTH, EQUAL, 54) | size(HEIGHT, EQUAL, 27);
    if (focused) show |= color(Color::Yellow);
    return show | color(Color::Blue);
  });
  this->help = Renderer([](bool focused){
    Element show = paragraph_imp(graph::HELPINFO_CN) | color(Color::Blue)
    | center | vcenter | borderLight | size(WIDTH, EQUAL, 54) | size(HEIGHT, EQUAL, 27);
    if (focused) show |= color(Color::Yellow);
    return show | color(Color::Blue);
  });
  empty = Renderer([](bool focused){ return paragraph_imp(graph::EMPTY); });
  pages = Container::Horizontal({empty}, &page_selected);
  auto rmain = Renderer(pages, [=]{
    if(!pages->ChildCount()) pages->Add(empty);
    else if(pages->ChildCount() > 1) empty->Detach();
    return pages->Render() | vcenter | center;
  });
  auto emain= CatchEvent(rmain, [=](Event event){
    if(event == Event::CtrlB) {
      page_selected = 0;
    } else if(event == Event::CtrlF) {
      page_selected = 1;
    }
    return false;
  });
  this->page = emain;
}

bool Vchat::handleEvent(Event event, int listSelection) {
  if (telescope->list_selected == -1) {
    telescope->input->TakeFocus();
    selected = 1;
    return false;
  } else if(telescope->list_selected == listSelection) {
    selected = 0;
    telescope->list_selected = -1;
    return false;
  } else return false;
}

// vchat
Vchat::Vchat() {
  init_page();
  telescope = new Telescope(this, &selected);
  auto cmain = Container::Tab({page, telescope->content}, &selected);
  auto rmain = Renderer(cmain, [=]{
    Elements status;
    status.emplace_back(text("[ 󱍢 VChat >") | color(Color::Blue));
    if (selected) status.emplace_back(text("  望远镜 >") | color(Color::Blue));
    else status.emplace_back(text("  窗口 >") | color(Color::Blue));
    status.emplace_back(filler());
    status.emplace_back(text("< " + Info::info->myself.username + "]") | color(Color::Blue));
    Element show;
    if(selected) {
    show = vbox(
      dbox(
        page->Render(), telescope->content->Render() | clear_under | center) | flex,
        text(" ") | underlined | color(Color::Blue),
        hbox(status)
      );
    } else {
      show = vbox(
        page->Render() | flex,
        text(" ") | underlined | color(Color::Blue),
        hbox(status)
      );
    }
    return show;
  });
  auto emain = CatchEvent(rmain, [=](Event event) {
    if(event == Event::CtrlK) {
      return handleEvent(event, COMMON);
    } else if(event == Event::CtrlY) {
      return handleEvent(event, MYSELF);
    } else if(event == Event::CtrlU) {
      return handleEvent(event, CHAT);
    } else if(event == Event::CtrlI) {
      return handleEvent(event, FRIEND);
    } else if(event == Event::CtrlO){
      return handleEvent(event, GROUP);
    } else if(event == Event::Escape) {
      if (selected) {
        selected = 0;
        telescope->selected = INPUT;
        telescope->list_selected = -1;
        return true;
      } else {
        pages->ChildAt(page_selected)->Detach();
      }
    } else if(event == Event::Special("accept_addfd")) {
      for(auto& v : Info::info->requestaddlist)
        if(v.second) {
          chats[v.first] = new Chat(v.first);
          break;
        }
    }
    return false;
  });
  this->content = emain;
}

Vchat::~Vchat() {
  delete telescope;
}
