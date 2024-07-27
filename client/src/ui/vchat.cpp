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

std::unordered_map<int, struct Chat*> Chat::chats;
std::unordered_map<int, struct Friend*> Friend::friends;

// chat list
Chat::List::List(Chat *chat) {
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    clist->Add(
      Renderer([=](bool focused){
        Element element = text(
          "You have added " + Info::info->friendinfo[chat->id].friendname +
          ", you can start chatting"
        ) | center;
        if(focused) element |= focus;
        return element;
      })
    );
    for(auto& value : Info::info->messageinfo[chat->id]) {
      clist->Add(
        Renderer([=](bool focused){
          Element element;
          if(focused) {
            if(value.sender == chat->id) {
              element = hbox(text(" > " + value.msg), filler()) | bgcolor(Color::Blue);
            } else if(value.receiver == chat->id){
              element = hbox(filler(), text(value.msg + " < ")) | bgcolor(Color::Blue);
            }
            element |= focus;
          } else {
            if(value.sender == chat->id) {
              element = hbox(text(" > "), text(value.msg), filler());
            } else if(value.receiver == chat->id){
              element = hbox(filler(), text(value.msg), text(" < "));
            }
          }
          return element;
        })
      );
    }
    return clist->Render() | flex | borderLight;
  }) | vscroll_indicator | frame;
  auto elist = CatchEvent(rlist, [&](Event event){
    if(event == Event::Special("sendmsg")) {}
    return false;
  });
  this->content = elist | flex;
}

// input
Chat::InputBox::InputBox(Chat *chat) {
  auto cinput = Input(&ss, "Press ctrl + y to send your message");
  auto rinput = Renderer(cinput, [=]{
   return hbox(text("  > "), cinput->Render()) | size(HEIGHT, EQUAL, 2) | borderLight;
  });
  auto einput = CatchEvent(rinput, [&](Event event){
    if(event == Event::CtrlY) {
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
        if(!function->sendmsg(chat->id, Info::info->myself.id, ss,
          static_cast<int64_t>(std::stoll(oss.str())))) {
          ss.clear();
          chat->list.content->TakeFocus();
        }
      return true;
    } else return false;
  });
  this->content = einput;
}

// chat
Chat::Chat(int id_)
  : list(this), input(this), id(id_) {
  auto cchat = Container::Vertical({ list.content, input.content }, &selected);
  auto echat = CatchEvent(cchat, [&](Event event){
    if(event == Event::CtrlN) {
      list.content->TakeFocus();
      if(selected != Info::info->messageinfo[id].size())
        selected ++;
      return true;
    }
    else if(event == Event::CtrlP) {
      list.content->TakeFocus();
      if(selected > 1) selected --;
      return true;
    }
    else if(event == Event::Special("sendmsg")) {
      list.content->TakeFocus();
      return false;
    }
    else input.content->TakeFocus();
    return false;
  });
  selected = Info::info->messageinfo[id].size();
  this->content = echat | flex;
}

// myself page
Friend::Friend(int id_) : id(id_) {
  auto container = Container::Horizontal({
    Button(" 󰚼  Edit", [&]{
    }, ButtonOption::Ascii()),
    Button("   Delete ", [&]{
    }, ButtonOption::Ascii()),
  }, &selected);
  auto rcontainer = Renderer(container, [=]{
    std::string username = Info::info->friendinfo[id].friendname;
    return hbox({
      vbox(
        text("[    ID    ]: " + std::to_string(id)),
        text("[ username ]: " + username)
      ),
      container->Render()
    });
  }) | flex;
  auto econtainer = CatchEvent(rcontainer, [&](Event event){
    if(event == Event::e) {
      selected = 0;
      return true;
    } else if(event == Event::d) {
      selected = 1;
      return true;
    }
    return false;
  });
  this->content = econtainer | flex | borderLight;
}

void Vchat::createdialog(Component content) {
  dialog->Add(content);
}

// main show
Vchat::Vchat() : telescope(this, &selected) {
  pages.emplace_back(Renderer([](bool focused){ return text(""); }));
  for(auto& v : Info::info->friendinfo)
    Chat::chats[v.first] = new Chat(v.first);
  for(auto& v : Info::info->friendinfo)
    Friend::friends[v.first] = new Friend(v.first);
  auto cpage = Container::Horizontal({}, &page_selected);
  auto rpage = Renderer(cpage, [=]{
    cpage->DetachAllChildren();
    for(auto& v : pages) cpage->Add(v);
    Element show;
    if (cpage->ChildCount() == 1)
      show = hbox(paragraph_imp(graph::LOGO) | color(Color::Blue)) | center;
    else show = dbox(paragraph_imp(graph::LOGO) | color(Color::Blue), cpage->Render() | clear_under | center);
    return show;
  });
  auto epage = CatchEvent(rpage, [&](Event event){
    if(event == Event::CtrlF) {
      if(page_selected) page_selected --;
    } else if(event == Event::CtrlB) {
      if(page_selected != pages.size()) page_selected ++;
    }
    return false;
  });
  auto cmain = Container::Tab({epage, telescope.content}, &selected);
  auto rmain = Renderer(cmain, [=]{
    Elements status;
    status.emplace_back(text("[   VChat >") | color(Color::Blue));
    if (selected) status.emplace_back(text("  望远镜 >") | color(Color::Blue));
    else status.emplace_back(text("  窗口 >") | color(Color::Blue));
    if(!pages.size()) status.emplace_back(text(" 背景 >") | color(Color::Blue));
    else status.emplace_back(text(" 小明 >") | color(Color::Blue));
    status.emplace_back(filler());
    status.emplace_back(text("< 提示 ]") | color(Color::Blue));
    if(selected) return vbox(
      dbox(
        epage->Render() | flex,
        telescope.content->Render() | clear_under | center
      ) | flex,
      text(" ") | underlined | color(Color::Blue),
      hbox(status)
    );
    return vbox(
      epage->Render() | flex,
      text(" ") | underlined | color(Color::Blue),
      hbox(status)
    );
  });
  auto emain = CatchEvent(rmain, [=](Event event) {
    if(event == Event::CtrlO || event == Event::CtrlJ
      || event == Event::CtrlK || event == Event::CtrlL) {
      selected = 1;
      return false;
    } else if(event == Event::Escape) {
      if (selected) { selected = 0; return true; }
    } else if(event == Event::Special("accept_addfd")) {
      for(auto& v : Info::info->requestaddlist)
        if(v.second.second) {
          Chat::chats[v.first] = new Chat(v.first);
          Friend::friends[v.first] = new Friend(v.first);
          break;
        }
    }
    return false;
  });
  this->content = emain;
}
