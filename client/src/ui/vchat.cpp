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
#include "info.h"
#include "ui.h"

namespace vchat {

int Vchat::pages_selected;
int ChatPage::chats_selected;
int FriendPage::friends_selected;
int ChatPage::choose_list;
int FriendPage::choose_list;
int MyselfPage::choose_list;

// persional list
MyselfPage::List::List(MyselfPage* myself_) : myself(myself_){
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    for(auto& n : Info::info->requestaddlist) {
     auto buts = Container::Horizontal({
        Button("Y添加", [&]{ function->responseadd(n.first, 1); }, ButtonOption::Ascii()),
        Button("N拒绝", [&]{ function->responseadd(n.first, 0); }, ButtonOption::Ascii())
      }, &myself->choose_add);
      auto rbuts = Renderer(buts, [=]{
        Element show;
        show = vbox(
          text("账号" + n.second["username"].asString() + "请求添加为好友"),
          buts->Render()
        );
        return show;
      });
      auto ebuts = CatchEvent(rbuts, [&](Event event){
        if(event == Event::Y) { myself->choose_add = 0; }
        else if(event == Event::N) { myself->choose_add = 1; }
        return false;
      });
      clist->Add(ebuts);
    }
    if(!clist->ChildCount())
      clist->Add(Renderer([=](bool focused){ return text("当前没有好友申请"); }));
    return clist->Render();
  });
  auto elist = CatchEvent(rlist, [&](Event event){
    if (event == Event::CtrlN) {
      if(selected != Info::info->requestaddlist.size() - 1) selected ++;
    } else if (event == Event::CtrlP) {
      if(selected) selected --;
    }
    return false;
  });
  content = elist | border | size(WIDTH, EQUAL, 30);
}

// myself page
MyselfPage::MyselfPage() : list(this) {
  auto info = Container::Vertical({
    Renderer([&](bool focused){
    std::string id = std::to_string(Info::info->myself.id);
    std::string username = Info::info->myself.username;
      return vbox(
        text("  账号  :" + id),
        text(" 用户名 :" + username)
      );
    }),
    Button(" 编辑信息", [&]{}, ButtonOption::Ascii())
  }) | flex | border;
  auto cmyself = Container::Horizontal({}, &choose_list);
  auto rmyself = Renderer(cmyself, [=]{
    cmyself->DetachAllChildren();
    cmyself->Add(list.content);
    cmyself->Add(info);
    return cmyself->Render();
  });
  auto emyself = CatchEvent(rmyself, [&](Event event){
    if (event == Event::CtrlB) {
      if(!choose_list) choose_list ++;
    } else if (event == Event::CtrlF) {
      if(choose_list) choose_list --;
    }
    return false;
  });
  content = emyself;
}

// message
void ChatPage::Chat::getmessage() {
  auto clist = Container::Vertical({}, &selected_msg);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    clist->Add(
      Renderer([=](bool focused){
        Element element = text(
          "您已添加" + Info::info->friendinfo[id].friendname +
          ", 现在可以开始聊天啦"
        ) | center;
        if(focused) element |= focus;
        return element;
      })
    );
    for(auto& value : Info::info->messageinfo[id]) {
      clist->Add(
        Renderer([=](bool focused){
          Element element;
          if(focused) {
            if(value.sender == id) {
              element = hbox(text(" >[ " + value.msg + " ]"), filler());
            } else if(value.receiver == id){
              element = hbox(filler(), text("[ " + value.msg + " ]< "));
            }
            element |= focus;
          } else {
            if(value.sender == id) {
              element = hbox(text(" > "), text(value.msg), filler());
            } else if(value.receiver == id){
              element = hbox(filler(), text(value.msg), text(" < "));
            }
          }
          return element;
        })
      );
    }
    return clist->Render();
  }) | vscroll_indicator | frame;
  auto elist = CatchEvent(rlist, [&](Event event){
    if(event == Event::Special("sendmsg")) {}
    return false;
  });
  this->messagelist = elist | flex | border;
}

// inputarea
void ChatPage::Chat::getinput() {
  auto cinput = Input(&input, "这里输入消息... 按 Ctrl + Y 发送");
  auto einput = CatchEvent(cinput, [&](Event event){
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
      if(!input.empty())
        if(!function->sendmsg(id, Info::info->myself.id, input,
          static_cast<int64_t>(std::stoll(oss.str())))) {
          input.clear();
          messagelist->TakeFocus();
        }
      return true;
    } else return false;
  }) | size(HEIGHT, EQUAL, 1);
  this->inputarea = einput | border;
}

// chat
ChatPage::Chat::Chat(int id_) : id(id_) {
  getmessage();
  getinput();
  this->showlist = true;
  if(Info::info->messageinfo[id].size()) lastmsg = Info::info->messageinfo[id].back().msg;
  auto cchat = Container::Vertical({messagelist, inputarea}, &inputfocus);
  auto echat = CatchEvent(cchat, [&](Event event){
    if(event == Event::CtrlN) {
      messagelist->TakeFocus();
      if(selected_msg != Info::info->messageinfo[id].size())
        selected_msg ++;
      return true;
    }
    else if(event == Event::CtrlP) {
      messagelist->TakeFocus();
      if(selected_msg > 1) selected_msg --;
      return true;
    }
    else if(event == Event::Special("sendmsg")) {
      messagelist->TakeFocus();
      lastmsg = Info::info->messageinfo[id].back().msg;
      return false;
    }
    else inputarea->TakeFocus();
    return false;
  });
  selected_msg = Info::info->messageinfo[id].size();
  this->content = echat | flex;
}

// chat list
ChatPage::List::List(ChatPage* chat_) : chat(chat_){
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    for (auto& v : chat->chats) {
      if (!v.second->showlist) continue;
      std::string name = Info::info->friendinfo[v.first].friendname;
      auto but = Button("  " + name + " ", [&]{
        chats_selected = v.first;
      }, ButtonOption::Ascii());
      auto per = Renderer(but, [=]{
        return vbox(
          but->Render(),
          text(" > " + v.second->lastmsg)
        );
      });
      clist->Add(per);
    }
    if(!clist->ChildCount())
      clist->Add(Renderer([=](bool focused){ return text("   一条消息都没有哎 "); }));
    return clist->Render();
  });
  auto elist = CatchEvent(rlist, [&](Event event){
    if (event == Event::CtrlN) {
      if(selected != chat_->chats.size() - 1) selected ++;
    } else if (event == Event::CtrlP) {
      if(selected) selected --;
    }
    return false;
  });
  this->content = elist | border | size(WIDTH, EQUAL, 24);
}

// chat page
ChatPage::ChatPage() : list(this) {
  for(auto& v : Info::info->friendinfo)
    chats[v.first] = new Chat(v.first);
  choose_list = 0;
  auto cchatpage = Container::Horizontal({}, &choose_list);
  auto rchatpage = Renderer(cchatpage, [=]{
    cchatpage->DetachAllChildren();
    cchatpage->Add(list.content);
    if (chats.size())
      cchatpage->Add(chats[chats_selected]->content);
    else
      cchatpage->Add(Renderer([&](bool focused){
        return paragraph_imp(graph::SMALL_COW) | center;
      }));
    return cchatpage->Render();
  });
  auto echatpage = CatchEvent(rchatpage, [&](Event event){
    if (event == Event::Special("accept_addfd")) {
      chats.clear();
      for(auto& v : Info::info->friendinfo)
        chats[v.first] = new Chat(v.first);
    } else if (event == Event::CtrlF) {
      if(!choose_list) choose_list ++;
    } else if (event == Event::CtrlB) {
      if(choose_list) choose_list --;
    }
    return false;
  });
  this->content = echatpage;
}

// friend list
FriendPage::List::List(FriendPage* friend__) : friend_(friend__){
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    for(auto& n : friend_->friends) {
      auto but = Button("   " + Info::info->friendinfo[n.first].friendname + ' ', [&]{
        friends_selected = n.first;
      }, ButtonOption::Ascii());
      clist->Add(but);
    }
    if(!clist->ChildCount())
      clist->Add(Renderer([=](bool focused){ return text("   一个好友都没有哎..."); }));
    return clist->Render() ;
  });
  auto elist = CatchEvent(rlist, [&](Event event){
    if(event == Event::CtrlP) {
      if (selected) {
        selected --;
      }
      return true;
    } else if(event == Event::CtrlN) {
      if (selected != Info::info->friendinfo.size() - 1) {
        selected ++;
      }
      return true;
    }
    return false;
  });
  this->content = rlist | border | size(WIDTH, EQUAL, 24);
}

// friend
FriendPage::Friend::Friend(int id_) : id(id_) {
  auto container = Container::Vertical({
    Button("󰍡 发送消息 ", [&]{
      ChatPage::chats_selected = id;
      Vchat::pages_selected = CHAT;
      ChatPage::choose_list = 1;
    }, ButtonOption::Ascii()),
    Button(" 编辑信息", [&]{
    }, ButtonOption::Ascii()),
  }, &friend_op_selected);
  auto rcontainer = Renderer(container, [=]{
    std::string username = Info::info->friendinfo[id].friendname;
    return hbox({
      vbox(
        text("  账号  :" + std::to_string(id)),
        text(" 用户名 :" + username)
      ),
      container->Render()
    });
  }) | border | flex;
  auto econtainer = CatchEvent(rcontainer, [&](Event event){
    if(event == Event::CtrlP) {
      friend_op_selected = 0;
      return true;
    } else if(event == Event::CtrlN) {
      friend_op_selected = 1;
      return true;
    }
    return false;
  });
  content = econtainer;
}

// friend page
FriendPage::FriendPage() : list(this) {
  for(auto& v : Info::info->friendinfo)
    friends[v.first] = new Friend(v.first);
  choose_list = 0;
  auto cfriendpage = Container::Horizontal({}, &choose_list);
  auto rfriendpage = Renderer(cfriendpage, [=]{
    cfriendpage->DetachAllChildren();
    cfriendpage->Add(list.content);
    if (friends.size())
      cfriendpage->Add(friends[ChatPage::chats_selected]->content);
    else
      cfriendpage->Add(Renderer([&](bool focused){
        return paragraph_imp(graph::SMALL_COW) | center;
      }));
    return cfriendpage->Render();
  });
  auto echatpage = CatchEvent(rfriendpage, [&](Event event){
    if (event == Event::Special("accept_addfd")) {
      friends.clear();
      for(auto& v : Info::info->friendinfo)
        friends[v.first] = new Friend(v.first);
    } else if (event == Event::CtrlF) {
      if(!choose_list) choose_list ++;
    } else if (event == Event::CtrlB) {
      if(choose_list) choose_list --;
    }
    return false;
  });
  this->content = echatpage;
}

void Vchat::getoptions() {
  options_selected = 2;
  auto coptions = Container::Vertical({
    Renderer([&](bool focused){ return text(" "); }),
    Renderer([&](bool focused){ return text("──"); }),
    Renderer([&](bool focused){ if(focused) return text(" "); return text(" "); }),
    Renderer([&](bool focused){ if(focused) return text("󰭹 "); return text("󰻞 "); }),
    Renderer([&](bool focused){ if(focused) return text("󰅟 "); return text(" "); }),
    Renderer([&](bool focused){ return filler(); }),
    Renderer([&](bool focused){ if(focused) return text("󰋗 "); return text("󰘥 "); }),
    Renderer([&](bool focused){ if(focused) return text("󰋼 "); return text("󰋽 "); }),
    Renderer([&](bool focused){ if(focused) return text("󰅙 "); return text("󰅚 "); }),
  }, &options_selected) | border;
  auto eoptions = CatchEvent(coptions, [&](Event event){
    if(event == Event::Return) {
      switch (options_selected) {
        case 2: pages_selected = MYSELF; break;
        case 3: pages_selected = CHAT;   break;
        case 4: pages_selected = FRIEND; break;
        case 6: now = HELP;              break;
        case 7: now = ABOUT;             break;
        case 8: screen->Exit();          break;
        default: break;
      }
      return false;
    } else if(event == Event::CtrlN) {
      if(options_selected != 8 && options_selected != 4) options_selected ++;
      else if(options_selected != 8 && options_selected == 4) options_selected += 2;
      return true;
    } else if(event == Event::CtrlP) {
      if(options_selected != 2 && options_selected != 6) options_selected --;
      else if(options_selected != 2 && options_selected == 6) options_selected -= 2;
      return true;
    }
    return false;
  });
  this->options = eoptions;
}

// main show
Vchat::Vchat(int& now_) : now(now_) {
  chat = new ChatPage();
  friend_ = new FriendPage();
  myself = new MyselfPage();
  this->getoptions();
  selected = 0;
  pages_selected = 0;
  ChatPage::chats_selected
    = FriendPage::friends_selected
    = Info::info->friendinfo.begin()->first;
  auto page = Container::Tab({
    myself->content, chat->content, friend_->content
  }, &pages_selected) | flex;
  auto cmain = Container::Horizontal({options, page}, &selected);
  auto emain = CatchEvent(cmain, [=](Event event) {
    if(event == Event::CtrlB) {
      if(selected) {
        if(pages_selected == MYSELF && !MyselfPage::choose_list) selected --;
        else if(pages_selected == CHAT && !ChatPage::choose_list) selected --;
        else if(pages_selected == FRIEND && !FriendPage::choose_list) selected --;
      }
    }
    else if(event == Event::CtrlF) {
      if(!selected) {
        selected ++;
        return true;
      }
    }
    else if(event == Event::Special("accept_addfd")) {
      ChatPage::chats_selected = FriendPage::friends_selected = Info::info->friendinfo.begin()->first;
    }
    return false;
  });
  content = emain;
}

} // namespace vchat
