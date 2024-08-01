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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "telescope.h"
#include "dashboard.h"
#include "data.hpp"
#include "ui.h"
#include "vchat.h"
 
Common::Common(Telescope* telescope) {
  for(int i = 1; i < 7; i ++) {
    Element show;
    switch (i) {
      case 1 : show = text("退出VChat"); break;
      case 2 : show = text("帮助页面");  break;
      case 3 : show = text("关于页面");  break;
      case 4 : show = text("群组列表");  break;
      case 5 : show = text("好友列表");  break;
      case 6 : show = text("消息列表");  break;
    }
    previews[i] = window(text(" 󰐩  预览 ") | center, show, LIGHT)
    | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
  }
  auto clist = Container::Vertical({
    Renderer([=](bool focused){ return filler(); }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰩈 退出"), filler())
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰩈 退出"), filler());
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  关于"), filler())
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  关于"), filler());
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰞋 帮助"), filler())
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰞋 帮助"), filler());
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  群组"), filler(), text("  Ctrl + o "))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  群组"), filler(), text("  Ctrl + o "));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  好友"), filler(), text("  Ctrl + i "))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  好友"), filler(), text("  Ctrl + i "));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰭹 消息"), filler(), text("  Ctrl + u "))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰭹 消息"), filler(), text("  Ctrl + u "));
    }),
  }, &selected);
  auto rlist = Renderer(clist, [=]{
    return window(text(" 󰨝  选项 ") | center, clist->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  auto elist = CatchEvent(rlist, [=](Event event) {
    if(event == Event::CtrlN) { if(selected != 6) selected ++; return true; }
    else if(event == Event::CtrlP) { if(selected != 1) selected --; return true; }
    else if(event == Event::Return) {
      switch (selected) {
        case 1 : screen->Exit(); break;
        case 2 : telescope->vchat->open_other(1); break;
        case 3 : telescope->vchat->open_other(0); break;
        case 4 :
          telescope->selected = INPUT;
          //telescope->inform.selected = Info::info->requestaddlist.size();
          telescope->list_selected = GROUP;
          break;
        case 5 :
          telescope->selected = INPUT;
          telescope->friends.friend_selected = telescope->friends.list.size();
          telescope->friends.function_selected = 3;
          telescope->list_selected = FRIEND;
          break;
        case 6 :
          telescope->selected = INPUT;
          telescope->chats.selected = telescope->chats.list.size();
          telescope->list_selected = CHAT;
          break;
      }
      return true;
    }
    return true;
  });
  this->content = elist;
}

std::pair<int, Component> Chats::addlist(FriendInfo v) {
  std::string msg = Info::info->messageinfo[v.friendid].back().msg;
  return std::make_pair(v.friendid,
    Renderer([=](bool focused){
      int64_t time_int = Info::info->messageinfo[v.friendid].back().time;
      std::string time_ss = std::to_string(time_int);
      time_ss =
        time_ss.substr(2, 2) + "月" +
        time_ss.substr(4, 2) + "日" +
        time_ss.substr(6, 2) + "时" +
        time_ss.substr(8, 2) + "分";
      Element show;
      if(focused) {
        show = hbox(
          text("󱍢 " + v.friendname+ " >"),
          text(msg),
          filler(), text(time_ss)
        ) | color(Color::Yellow) | bgcolor(Color::Blue);
      } else {
        show = hbox(
          text(v.friendname + " >"),
          text(msg),
          filler(), text(time_ss)
        ) | color(Color::Blue) | bgcolor(Color::Default);
      }
      return show;
    })
  );
}

void Chats::refresh() {
  list.clear();
  previews.clear();
  if (!telescope->ss.empty()) {
    std::regex e(telescope->ss);
    int count = 1;
    for(auto& v : Info::info->friendinfo) {
      std::string name = v.second.friendname;
      std::string msg = Info::info->messageinfo[v.first].back().msg;
      if (std::regex_search(name, e) || std::regex_search(msg, e)) {
        list.emplace_back(addlist(v.second));
        Elements show;
        for(auto vv : Info::info->messageinfo[v.first]) {
          if(vv.sender == v.first) {
            show.emplace_back(hbox(text(" > "), text(vv.msg), filler()));
          } else if(vv.receiver == v.first){
            show.emplace_back(hbox(filler(), text(vv.msg), text(" < ")));
          }
        }
        previews[count] =
          window(text(" 󰐩  预览 ") | center, vbox(show) | flex, LIGHT)
          | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
        count ++;
      }
    }
  } else {
    int count = 1;
    for(auto& v : Info::info->friendinfo) {
      std::string name = v.second.friendname;
      std::string msg = Info::info->messageinfo[v.first].back().msg;
      list.emplace_back(addlist(v.second));
      Elements show;
      for(auto vv : Info::info->messageinfo[v.first]) {
        if(vv.sender == v.first) {
          show.emplace_back(hbox(text(" > "), text(vv.msg), filler()));
        } else if(vv.receiver == v.first){
          show.emplace_back(hbox(filler(), text(vv.msg), text(" < ")));
        }
      }
      previews[count] =
        window(text(" 󰐩  预览 ") | center, vbox(show) | flex, LIGHT)
        | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
      count ++;
    }
  }
  previews[0] =
    window(text(" 󰐩  预览 ") | center, paragraph_imp(graph::EMPTY) | center, LIGHT)
    | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
  selected = list.size();
}

Chats::Chats(Telescope* telescope_) : telescope(telescope_) {
  refresh();
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    clist->Add(Renderer([=](bool focused){ return filler(); }));
    for(auto& v : list) clist->Add(v.second);
    return window(text(" 󰨝  消息 ") | center, clist->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  auto elist = CatchEvent(rlist, [=](Event event){
    if (event == Event::CtrlN) {
      if(selected < list.size()) selected ++;
    } else if (event == Event::CtrlP) {
      if(selected > 1) selected --;
    } else if(event == Event::Return) {
      telescope->vchat->open_chat(list[selected - 1].first);
    }
    return false;
  });
  this->content = elist;
}

std::pair<int, Component> Friends::addlist(FriendInfo v) {
  return std::make_pair(v.friendid,
    Renderer([=](bool focused){
      Element show;
      if(focused) {
        show = hbox(
          text("󱍢 " + v.friendname),
          filler()
        ) | color(Color::Yellow) | bgcolor(Color::Blue);
      } else {
        show = hbox(
          text(v.friendname),
          filler()
        ) | color(Color::Blue) | bgcolor(Color::Default);
      }
      return show;
    })
  );
}

void Friends::refresh() {
  list.clear();
  previews.clear();
  if (!telescope->ss.empty()) {
    for(auto v : Info::info->friendinfo) {
      std::string name = v.second.friendname;
      std::regex e(telescope->ss);
      if (std::regex_search(name, e)) {
        list.emplace_back(addlist(v.second));
        int count = 1;
        previews[count] = window(
          text(" 󰐩  预览 ") | center,
          vbox(
            text("    ID    :" + std::to_string(v.first)),
            text(" Username :" + v.second.friendname)
          ) | flex,
          LIGHT
        ) | size(WIDTH, EQUAL, 30)
          | size(HEIGHT, EQUAL, 15)
          | color(Color::Yellow);
        count ++;
      }
    }
  } else {
    for(auto v : Info::info->friendinfo) {
      std::string name = v.second.friendname;
      std::regex e(telescope->ss);
      list.emplace_back(addlist(v.second));
      int count = 1;
      previews[count] = window(
        text(" 󰐩  预览 ") | center,
        vbox(
          text("    ID    :" + std::to_string(v.first)),
          text(" Username :" + v.second.friendname)
        ) | flex,
        LIGHT
      ) | size(WIDTH, EQUAL, 30)
        | size(HEIGHT, EQUAL, 15)
        | color(Color::Yellow);
      count ++;
    }
  }
  previews[0] =
    window(text(" 󰐩  预览 ") | center, paragraph_imp(graph::EMPTY) | center, LIGHT)
    | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
  friend_selected = list.size();
}

Friends::Friends(Telescope* telescope_) : telescope(telescope_) {
  selected = 0;
  function_selected = 3;
  refresh();
  auto clist = Container::Vertical({}, &friend_selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    clist->Add(Renderer([=](bool focused){ return filler(); }));
    for(auto& v : list) clist->Add(v.second);
    return clist->Render();
  });
  auto elist = CatchEvent(rlist, [=](Event event){
    if(event == Event::CtrlP) {
      if (friend_selected != 1) friend_selected --;
      return true;
    } else if(event == Event::CtrlN) {
      if (friend_selected != Info::info->friendinfo.size()) friend_selected ++;
      return true;
    } else if(event == Event::Return) {
      selected = 1;
      return true;
    }
    return false;
  });
  auto cfunction = Container::Vertical({
    Renderer([=](bool focused){ return filler(); }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰆴 删除"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰆴 删除"));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰭹 发消息"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰭹 发消息"));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰌑 返回"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰌑 返回"));
    })
  }, &function_selected);
  auto efunction = CatchEvent(cfunction, [=](Event event){
    if(event == Event::CtrlP) {
      if (function_selected != 1) function_selected --;
      return true;
    } else if(event == Event::CtrlN) {
      if (function_selected != 3) function_selected ++;
      return true;
    } else if(event == Event::Return) {
      switch (function_selected) {
        case 1:
          // function->deletefriend(list[friend_selected - 1].first);
          break;
        case 2:
          telescope->vchat->open_chat(list[friend_selected - 1].first);
          telescope->toggle = 0;
          break;
        case 3: break;
      }
      selected = 0;
      function_selected = 3;
      return true;
    }
    return false;
  });
  auto cmain = Container::Tab({elist, efunction}, &selected);
  auto rmain = Renderer(cmain, [=]{
    return window(text(" 󰨝  朋友 ") | center, cmain->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  this->content = rmain;
}

void Telescope::getinput() {
  auto cmain = myinput(&ss, "", false, "󱍢 ");
  auto rmain = Renderer(cmain, [=]{
    return window(
      text("  搜索 ") | center,
      hbox(text("   "), cmain->Render()),
      LIGHT
    ) | color(Color::Green);
  });
  this->input = rmain;
}

Telescope::Telescope(Vchat *vchat_, int *toggle_)
  : common(this), chats(this), friends(this), toggle(toggle_) {
  getinput();
  Telescope::vchat = vchat_;
  list_selected = -1;
  auto clist = Container::Tab({common.content, chats.content, friends.content}, &list_selected);
  auto cmain = Container::Vertical({clist, input}, &selected);
  auto rmain = Renderer(cmain, [=]{
    if (list_selected == CHAT) chats.refresh();
    else if (list_selected == FRIEND) friends.refresh();
    if (list_selected == COMMON) {
      return hbox(cmain->Render(), common.previews[common.selected]);
    } else if (list_selected == CHAT) {
      return hbox(cmain->Render(), chats.previews[chats.selected]);
    } else if (list_selected == FRIEND) {
      return hbox(cmain->Render(), friends.previews[friends.friend_selected]);
    }
    return text("Error");
  });
  auto emain = CatchEvent(rmain, [=](Event event){
    if (event == Event::CtrlK) {
      selected = INPUT;
      common.selected = 6;
      list_selected = COMMON;
      return true;
    } else if (event == Event::CtrlU) {
      selected = INPUT;
      chats.selected = chats.list.size();
      list_selected = CHAT;
      return true;
    } else if (event == Event::CtrlI) {
      selected = INPUT;
      friends.friend_selected = friends.list.size();
      list_selected = FRIEND;
      return true;
    } else if (event == Event::CtrlO) {
      selected = INPUT;
      //group.selected = Info::info->requestaddlist.size();
      list_selected = GROUP;
      return true;
    } else if (event == Event::CtrlN || event == Event::CtrlP) {
      if(input->Focused()) { clist->TakeFocus(); return true; }
      clist->TakeFocus();
      return false;
    } else if (event == Event::Escape) {
      *toggle = *toggle ? 0 : 1;
    } else if (event == Event::Return) {
      return false;
    } else input->TakeFocus();
    return false;
  });
  this->content = emain;
}

Telescope::~Telescope() {}
