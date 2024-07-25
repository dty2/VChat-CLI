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
#include "telescope.h"
#include <ftxui/component/component.hpp>

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
  for(auto& v : Info::info->friendinfo)
    Chat::chats[v.first] = new Chat(v.first);
  for(auto& v : Info::info->friendinfo)
    Friend::friends[v.first] = new Friend(v.first);
  auto cpage = Container::Horizontal(pages, &page_selected);
  auto rpage = Renderer(cpage, [=]{
    cpage->DetachAllChildren();
    for(auto& v : pages) cpage->Add(v);
    return cpage->Render();
  });
  auto epage = CatchEvent(rpage, [&](Event event){
    if(event == Event::CtrlF) {
      if(page_selected) page_selected --;
    } else if(event == Event::CtrlB) {
      if(page_selected != pages.size()) page_selected ++;
    }
    return false;
  });
  auto cdialog = Container::Tab({epage}, &dialog_selected);
  auto rdialog = Renderer(cdialog, [=]{
    if (dialog->ChildCount()) {
      cdialog->Add(dialog);
    }
    return cdialog->Render();
  });
  auto cmain = Container::Tab({}, &selected) | flex;
  auto rmain = Renderer(cmain, [=]{
    cmain->DetachAllChildren();
    Elements status;
    // colors is at the bottom in this file
    status.emplace_back(
      text("   VChat ") | color(Color::Blue) | bgcolor(Color::RGB(21, 101, 192)));
    status.emplace_back(
      text(" ") | color(Color::RGB(21, 101, 192)) | bgcolor(Color::RGB(30, 136, 229)));
    if (selected) {
      status.emplace_back(
        text("  望远镜 ") | color(Color::Blue) | bgcolor(Color::RGB(30, 136, 229)));
      status.emplace_back(
        text(" ") | color(Color::RGB(30, 136, 229)) | bgcolor(Color::RGB(66, 165, 245)));
      status.emplace_back(filler() | bgcolor(Color::RGB(66, 165, 245)));
      status.emplace_back(text(" ") | bgcolor(Color::RGB(66, 165, 245)));
    }
    else {
      status.emplace_back(
        text("  窗口 ") | color(Color::Blue) | bgcolor(Color::RGB(30, 136, 229)));
      status.emplace_back(
        text(" ") | color(Color::RGB(30, 136, 229)) | bgcolor(Color::RGB(66, 165, 245)));
      status.emplace_back(filler() | bgcolor(Color::RGB(66, 165, 245)));
      status.emplace_back(text(" ") | bgcolor(Color::RGB(66, 165, 245)));
    }
    cmain->Add(rdialog);
    Component show;
    if(selected) {
      show = telescope.gettelescope(telescope_selected);
      cmain->Add(show);
    }
    if(selected) return vbox(
      dbox(
        epage->Render(),
        show->Render() | clear_under | center
      ) | flex,
      hbox(status)
    );
    return vbox(epage->Render() | flex, hbox(status));
  });
  auto emain = CatchEvent(rmain, [=](Event event) {
    if(event == Event::Special("accept_addfd")) {
      for(auto& v : Info::info->requestaddlist)
        if(v.second.second) {
          Chat::chats[v.first] = new Chat(v.first);
          Friend::friends[v.first] = new Friend(v.first);
          break;
        }
    }
    return false;
  });
  //this->content = emain;
  this->content = Button("test", []{});
}

/*
  * blue
  * 0D47A1 -> RGB (13, 71, 161)
  * 1565C0 -> RGB (21, 101, 192)
  * 1976D2 -> RGB (25, 118, 210)
  * 1E88E5 -> RGB (30, 136, 229)
  * 2196F3 -> RGB (33, 150, 243)
  * 42A5F5 -> RGB (66, 165, 245)
  * 64B5F6 -> RGB (100, 181, 246)
  * 90CAF9 -> RGB (144, 202, 249)
  */
