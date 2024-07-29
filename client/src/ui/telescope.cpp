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
      if(focused) return hbox(text("󱍢  群组"), filler(), text("  Ctrl + l "))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  群组"), filler(), text("  Ctrl + l "));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  好友"), filler(), text("  Ctrl + k "))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  好友"), filler(), text("  Ctrl + k "));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰭹 消息"), filler(), text("  Ctrl + j "))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰭹 消息"), filler(), text("  Ctrl + j "));
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
          telescope->inform.selected = Info::info->requestaddlist.size();
          telescope->list_selected = GROUP;
          break;
        case 5 :
          telescope->selected = INPUT;
          telescope->friends.selected = telescope->friends.list.size();
          telescope->list_selected = FRIEND;
          break;
        case 6 :
          telescope->selected = INPUT;
          telescope->chats.selected = telescope->chats.list.size();
          telescope->list_selected = CHAT;
          break;
      }
      *(telescope->toggle) = 0;
      return true;
    }
    return true;
  });
  this->content = elist;
}

void Chats::refresh() {
  list.clear();
  previews.clear();
  for(auto& v : Info::info->friendinfo) {
    list.emplace_back(std::make_pair(v.first, Renderer([=](bool focused){
      int64_t time_int = Info::info->messageinfo[v.first].back().time;
      std::string time_ss = std::to_string(time_int);
      time_ss =
        time_ss.substr(2, 2) + "月" +
        time_ss.substr(4, 2) + "日" +
        time_ss.substr(6, 2) + "时" +
        time_ss.substr(8, 2) + "分";
      Element show;
      if(focused) {
        show = hbox(
          text("󱍢 " + v.second.friendname + " >"),
          text(Info::info->messageinfo[v.first].back().msg),
          filler(), text(time_ss)
        ) | color(Color::Yellow) | bgcolor(Color::Blue);
      } else {
        show = hbox(
          text(v.second.friendname + " >"),
          text(Info::info->messageinfo[v.first].back().msg),
          filler(), text(time_ss)
        ) | color(Color::Blue) | bgcolor(Color::Default);
      }
      return show;
    })));
    int count = 1;
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

Chats::Chats(Telescope* telescope) {
  refresh();
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    if(list.size() != clist->ChildCount()) {
      clist->DetachAllChildren();
      clist->Add(Renderer([=](bool focused){ return filler(); }));
      for(auto& v : list) clist->Add(v.second);
      if(clist->ChildCount() == 1) {
        clist->DetachAllChildren();
        clist->Add(Renderer(
          [=](bool focused){ return text("  没有消息...") | center; })
        );
      }
    }
    return window(text(" 󰨝  消息 ") | center, clist->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  auto elist = CatchEvent(rlist, [=](Event event){
    if (event == Event::CtrlN) {
      if(selected != Info::info->friendinfo.size()) selected ++;
    } else if (event == Event::CtrlP) {
      if(selected != 1) selected --;
    } else if(event == Event::Return) {
      telescope->vchat->open_chat(list[selected - 1].first);
    }
    return false;
  });
  this->content = elist;
}

void Friends::refresh() {
  list.clear();
  previews.clear();
  for(auto v : Info::info->friendinfo) {
    list.emplace_back(std::make_pair(v.first, Renderer([=](bool focused){
      Element show;
      if(focused) {
        show = hbox(
          text("󱍢 " + v.second.friendname),
          filler()
        ) | color(Color::Yellow) | bgcolor(Color::Blue);
      } else {
        show = hbox(
          text(v.second.friendname),
          filler()
        ) | color(Color::Blue) | bgcolor(Color::Default);
      }
      return show;
    })));
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

Friends::Friends(Telescope* telescope) {
  refresh();
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    if (clist->ChildCount() != list.size()) {
      clist->DetachAllChildren();
      clist->Add(Renderer([=](bool focused){ return filler(); }));
      for(auto& v : list) clist->Add(v.second);
      if(clist->ChildCount() == 1) {
        clist->DetachAllChildren();
        clist->Add(Renderer([=](bool focused){ return text("  没有朋友...") | center; }));
      }
  }
    return window(text(" 󰨝  朋友 ") | center, clist->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  auto elist = CatchEvent(rlist, [&](Event event){
    if(event == Event::CtrlP) {
      if (selected != 1) selected --;
    } else if(event == Event::CtrlN) {
      if (selected != Info::info->friendinfo.size()) selected ++;
    } else if(event == Event::Return) { return true; } // TODO:
    return false;
  });
  this->content = elist;
}

Inform::Inform(Telescope *telescope) {
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    for(auto& v : Info::info->requestaddlist) {
      previews[v.first] = window(text(" 󰐩  Preview "), vbox(
        text("[     ID    ] :" + std::to_string(v.first)),
        text("[  Username ] :" + v.second.first["username"].asString())
      ) | center, LIGHT);
      auto rbuts = Renderer([=](bool focused){
        Element show;
        if (focused)
          show = vbox(
            text(">username "
              + v.second.first["username"].asString()
              + "requests to add a friend "
            )
          ) | bgcolor(Color::Blue);
        else
          show = vbox(
            text("username "
              + v.second.first["username"].asString()
              + "requests to add a friend ")
          );
        return show;
      });
      clist->Add(rbuts);
      id[selected] = v.first;
    }
    if(!clist->ChildCount())
      clist->Add(Renderer([=](bool focused){ return text("📢 no inform..."); }));
    return clist->Render();
  });
  auto elist = CatchEvent(rlist, [&](Event event){
    if (event == Event::CtrlN) {
      if(selected != Info::info->requestaddlist.size() - 1) selected ++;
    } else if(event == Event::CtrlP) {
      if(selected) selected --;
    } else if(event == Event::Return) {
      return true;
    }
    return false;
  });
  this->list = elist;
}

Telescope::Input::Input() {
  auto cmain = myinput(&ss, "", false, "󱍢 ");
  auto rmain = Renderer(cmain, [=]{
    return window(
      text("  搜索 ") | center,
      hbox(text("   "), cmain->Render()),
      LIGHT
    ) | color(Color::Green);
  });
  auto emain = CatchEvent(rmain, [=](Event event){
    return false;
  });
  this->content = emain;
}

Telescope::Telescope(Vchat *vchat_, int *toggle_)
  : common(this), chats(this), friends(this), inform(this), toggle(toggle_) {
  Telescope::vchat = vchat_;
  list_selected = -1;
  auto clist = Container::Tab({common.content, chats.content, friends.content}, &list_selected);
  auto cmain = Container::Vertical({clist, input.content}, &selected);
  auto rmain = Renderer(cmain, [=]{
    if (list_selected == COMMON) {
      return hbox(cmain->Render(), common.previews[common.selected]);
    } else if (list_selected == CHAT) {
      return hbox(cmain->Render(), chats.previews[1]);
    } else if (list_selected == FRIEND) {
      return hbox(cmain->Render(), friends.previews[friends.selected]);
    } else if (list_selected == FRIEND) {
      return hbox(cmain->Render(), friends.previews[inform.selected]);
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
      friends.selected = friends.list.size();
      list_selected = FRIEND;
      return true;
    } else if (event == Event::CtrlO) {
      selected = INPUT;
      inform.selected = Info::info->requestaddlist.size();
      list_selected = GROUP;
      return true;
    } else if (event == Event::CtrlN || event == Event::CtrlP) {
      if(input.content->Focused()) { clist->TakeFocus(); return true; }
      clist->TakeFocus();
      return false;
    } else if (event == Event::Escape) {
      *toggle = *toggle ? 0 : 1;
    } else if (event == Event::Return) {
      return false;
    } else if ( event == Event::Special("sendmsg")
      || event == Event::Special("accept_addfd")) {
      chats.refresh();
      friends.refresh();
    } else input.content->TakeFocus();
    return false;
  });
  this->content = emain;
}

Telescope::~Telescope() {}
