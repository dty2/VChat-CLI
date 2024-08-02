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
  for(int i = 1; i < 8; i ++) {
    Element show;
    switch (i) {
      case 1 : show = text("退出VChat"); break;
      case 2 : show = text("帮助页面");  break;
      case 3 : show = text("关于页面");  break;
      case 4 : show = text("群组列表");  break;
      case 5 : show = text("好友列表");  break;
      case 6 : show = text("消息列表");  break;
      case 7 : show = text("个人管理");  break;
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
      if(focused) return hbox(text("󱍢  群组"), filler(), text("  Ctrl + o "))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  群组"), filler(), text("  Ctrl + o "));
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
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󱅻 自己"), filler(), text("  Ctrl + y "))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󱅻 自己"), filler(), text("  Ctrl + o "));
    }),
  }, &selected);
  auto rlist = Renderer(clist, [=]{
    return window(text(" 󰨝  选项 ") | center, clist->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  auto elist = CatchEvent(rlist, [=](Event event) {
    if(event == Event::CtrlN) { if(selected != 7) selected ++; return true; }
    else if(event == Event::CtrlP) { if(selected != 1) selected --; return true; }
    else if(event == Event::Return) {
      switch (selected) {
        case 1 : screen->Exit(); break;
        case 2 : telescope->vchat->open_other(1); break;
        case 3 : telescope->vchat->open_other(0); break;
        case 4 :
          // TODO:
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
        case 7 :
          telescope->selected = INPUT;
          telescope->myself.function_selected = 5;
          telescope->myself.selected = 0;
          telescope->list_selected = MYSELF;
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
      if (Info::info->messageinfo[v.first].size()) {
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
    }
  } else {
    int count = 1;
    for(auto& v : Info::info->friendinfo) {
      std::string name = v.second.friendname;
      if (Info::info->messageinfo[v.first].size()) {
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
  }
  previews[0] =
    window(text(" 󰐩  预览 ") | center, paragraph_imp(graph::EMPTY) | center, LIGHT)
    | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
  selected = list.size();
}

Chats::Chats(Telescope* telescope_) : telescope(telescope_) {
  refresh();
  auto cmain = Container::Vertical({}, &selected);
  auto rmain = Renderer(cmain, [=]{
    cmain->DetachAllChildren();
    cmain->Add(Renderer([=](bool focused){ return filler(); }));
    for(auto& v : list) cmain->Add(v.second);
    return window(text(" 󰨝  消息 ") | center, cmain->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  auto emain = CatchEvent(rmain, [=](Event event){
    if (event == Event::CtrlN) {
      if(selected < list.size()) selected ++;
    } else if (event == Event::CtrlP) {
      if(selected > 1) selected --;
    } else if(event == Event::Return) {
      telescope->vchat->open_chat(list[selected - 1].first);
    }
    return false;
  });
  this->content = emain;
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
  int count = 1;
  if (!telescope->ss.empty()) {
    for(auto v : Info::info->friendinfo) {
      std::string name = v.second.friendname;
      std::regex e(telescope->ss);
      if (std::regex_search(name, e)) {
        list.emplace_back(addlist(v.second));
        previews[count] = window(
          text(" 󰐩  预览 ") | center,
          vbox(
            text("    ID    :" + std::to_string(v.first)),
            text(" Username :" + v.second.friendname)
          ) | flex,
          LIGHT
        ) | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
        count ++;
      }
    }
  } else {
    for(auto v : Info::info->friendinfo) {
      std::string name = v.second.friendname;
      list.emplace_back(addlist(v.second));
      previews[count] = window(
        text(" 󰐩  预览 ") | center,
        vbox(
          text("    ID    :" + std::to_string(v.first)),
          text(" Username :" + v.second.friendname)
        ) | flex,
        LIGHT
      ) | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
      count ++;
    }
  }
  for(auto v : Info::info->requestaddlist) {
    list.emplace_back(-v.first, Renderer([=](bool focused){
      Element show = text(" 账号" +
        v.second.first["username"].asString() + "请求添加为好友") | color(Color::Blue);
      if (focused) show |= bgcolor(Color::Blue) | color(Color::Yellow);
      return show;
    }));
    previews[count] =
      window(text(" 󰐩  预览 ") | center, text("好友申请") | center, LIGHT)
      | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
  }
  previews[0] =
    window(text(" 󰐩  预览 ") | center, paragraph_imp(graph::EMPTY) | center, LIGHT)
    | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
}

Friends::Friends(Telescope* telescope_) : telescope(telescope_) {
  selected = 0;
  function_selected = 1;
  function2_selected = 3;
  function1_selected = 4;
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
      if (friend_selected > 1) friend_selected --;
      return true;
    } else if(event == Event::CtrlN) {
      if (friend_selected < list.size()) friend_selected ++;
      return true;
    } else if(event == Event::Return) {
      selected = 1;
      if (list[friend_selected - 1].first < 0) {
        function_selected = 1;
        return false;
      }
      function_selected = 0;
      return false;
    }
    return false;
  });
  auto cfunction1 = Container::Vertical({
    Renderer([=](bool focused){ return filler(); }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰆴 删除好友"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰆴 删除好友"));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  修改备注"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  修改备注"));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰭹 发送消息"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰭹 发送消息"));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰌑 返回列表"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰌑 返回列表"));
    })
  }, &function1_selected);
  auto efunction1 = CatchEvent(cfunction1, [=](Event event){
    if(event == Event::CtrlP) {
      if (function1_selected != 1) function1_selected --;
      return true;
    } else if(event == Event::CtrlN) {
      if (function1_selected != 4) function1_selected ++;
      return true;
    } else if(event == Event::Return) {
      switch (function1_selected) {
        // TODO:
        case 1:
          // function->deletefriend(list[friend_selected - 1].first);
          break;
        case 2: break;
        case 3:
          telescope->vchat->open_chat(list[friend_selected - 1].first);
          telescope->toggle = 0;
          break;
        case 4: break;
      }
      selected = 0;
      function1_selected = 3;
      return true;
    }
    return false;
  });
  auto cfunction2 = Container::Vertical({
    Renderer([=](bool focused){ return filler(); }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  拒绝"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  拒绝"));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  添加"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  添加"));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󰌑 返回列表"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󰌑 返回列表"));
    })
  }, &function2_selected);
  auto efunction2 = CatchEvent(cfunction2, [=](Event event){
    if(event == Event::CtrlP) {
      if (function2_selected != 1) function2_selected --;
      return true;
    } else if(event == Event::CtrlN) {
      if (function2_selected != 3) function2_selected ++;
      return true;
    } else if(event == Event::Return) {
      switch (function2_selected) {
        case 1:
          function->responseadd(-list[friend_selected - 1].first, 0);
          telescope->toggle = 0;
          break;
        case 2:
          function->responseadd(-list[friend_selected - 1].first, 1);
          telescope->toggle = 0;
          break;
        case 3: function2_selected = 0; break;
      }
      selected = 0;
      function2_selected = 3;
      return true;
    }
    return false;
  });
  auto cfunction = Container::Tab({efunction1, efunction2}, &function_selected);
  auto cmain = Container::Tab({elist, cfunction}, &selected);
  auto rmain = Renderer(cmain, [=]{
    return window(text(" 󰨝  朋友 ") | center, cmain->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  this->content = rmain;
}

Myself::AddFriend::AddFriend(Myself* myself) {
  selected = 0;
  auto cmain = Container::Vertical({}, &selected);
  cmain->Add(Renderer([=](bool focused){ return filler(); }));
  auto rmain = Renderer(cmain, [=]{
    cmain->DetachAllChildren();
    cmain->Add(Renderer([=](bool focused){ return filler(); }));
    for(auto v : Info::info->findlist) {
      auto meta = Renderer([=](bool focused){
        Element show = text(v.second["name"].asString());
        if (focused) show |= bgcolor(Color::Blue) | color(Color::Yellow);
        return show;
      });
      cmain->Add(CatchEvent(meta, [=](Event event){
        if(event == Event::Return) {
          function->addfriend(v.first);
        }
        return true;
      }));
    }
    return window(text(" 󰨝  搜索结果 ") | center, cmain->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  auto emain = CatchEvent(rmain, [=](Event event){
    if(event == Event::CtrlP) {
      if (selected != 1) selected --;
      return true;
    } else if(event == Event::CtrlN) {
      if (selected != cmain->ChildCount() - 1) selected ++;
      return true;
    } else if(event == Event::Special("findfd_suc")) {
      return true;
    }
    return false;
  });
  this->content = emain;
}

Myself::Myself(Telescope* telescope_) : addfriend(this), telescope(telescope_) {
  function_selected = 5;
  selected = 0;
  for(int i = 1; i < 6; i ++) {
    Element show;
    switch (i) {
      case 1 : show = text("注销账号"); break;
      case 2 : show = text("退出登陆"); break;
      case 3 : show = text("修改信息"); break;
      case 4 : show = text("添加群组"); break; // auto create group when not find group
      case 5 : show = text("添加好友"); break;
    }
    previews[i] = window(text(" 󰐩  预览 ") | center, show, LIGHT)
    | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 15) | color(Color::Yellow);
  }
  auto clist = Container::Vertical({
    Renderer([=](bool focused){ return filler(); }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢 󱘑 注销账号"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text(" 󱘑 注销账号"), filler());
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  退出登陆"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  退出登陆"), filler());
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  修改信息"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  修改信息"), filler());
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  添加群组"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  添加群组"));
    }),
    Renderer([=](bool focused){
      if(focused) return hbox(text("󱍢  添加好友"))
      | bgcolor(Color::Blue) | color(Color::Yellow);
      else return hbox(text("  添加好友"));
    })
  }, &function_selected);
  auto rlist = Renderer(clist, [=]{
    return window(text(" 󰨝  选项 ") | center, clist->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14) | color(Color::Blue);
  });
  auto elist = CatchEvent(rlist, [=](Event event) {
    if(event == Event::CtrlN) { if(function_selected != 5) function_selected ++; return true; }
    else if(event == Event::CtrlP) { if(function_selected != 1) function_selected --; return true; }
    else if(event == Event::Return) {
      switch (function_selected) {
        case 1 : ; break; // TODO:
        case 2 : ; break; // TODO:
        case 3 : ; break; // TODO:
        case 4 : ; break; // TODO:
        case 5 : selected = 1; break;
      }
      return true;
    }
    return true;
  });
  auto cmain = Container::Tab({elist, addfriend.content}, &selected);
  this->content = cmain;
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
  auto emain = CatchEvent(rmain, [=](Event event){
    if(event == Event::Return && list_selected == MYSELF) {
      bool signal = 1;
      for(auto& v : ss) if(v >= 'a' && v <= 'z') signal = 0;
      if(signal) function->find(std::stoi(ss));
      else function->find(ss);
      ss.clear();
      selected = LIST;
      myself.content->TakeFocus();
      myself.selected = 1;
    }
    return false;
  });
  this->input = emain;
}

Telescope::Telescope(Vchat *vchat_, int *toggle_)
  : common(this), myself(this), chats(this), friends(this), toggle(toggle_) {
  getinput();
  Telescope::vchat = vchat_;
  list_selected = -1;
  auto clist = Container::Tab({common.content, myself.content, chats.content, friends.content}, &list_selected);
  auto cmain = Container::Vertical({clist, input}, &selected);
  auto rmain = Renderer(cmain, [=]{
    if (list_selected == CHAT) chats.refresh();
    else if (list_selected == FRIEND) friends.refresh();
    if (list_selected == COMMON) {
      return hbox(cmain->Render(), common.previews[common.selected]);
    } else if (list_selected == MYSELF) {
      return hbox(cmain->Render(), myself.previews[myself.function_selected]);
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
      common.selected = 7;
      list_selected = COMMON;
      return true;
    } else if (event == Event::CtrlY) {
      selected = INPUT;
      myself.function_selected = 5;
      list_selected = MYSELF;
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
      // TODO:
      list_selected = GROUP;
      return true;
    } else if (event == Event::CtrlN || event == Event::CtrlP) {
      if(input->Focused()) { clist->TakeFocus(); return true; }
      clist->TakeFocus();
      return false;
    } else if(event == Event::Special("findfd_suc")) {
      myself.content->TakeFocus();
      list_selected = MYSELF;
      selected = LIST;
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
