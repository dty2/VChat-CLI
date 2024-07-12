#include "vchat.h"
#include "info.h"

namespace vchat {

std::unordered_map<int, Chat*> Chat::chats_map;
int Chat::chats_selected = 0;
std::unordered_map<int, Friend*> Friend::friends_map;
int Friend::friends_selected = 0;

Page::Page(int id_, Function& function_, ScreenInteractive* screen_)
  : id(id_), function(function_), screen(screen_) {}

Page::Page(int id_, Function& function_)
  : id(id_), function(function_) {}

Page::Page(Function& function_) : function(function_) {}

// 生成消息列表
void Chat::getmessagelist() {
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
  this->list = elist | border | flex;
}

// 生成输入区域
void Chat::getinputarea() {
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
        if(!function.sendmsg(id, Info::info->myself.id, input,
          static_cast<int64_t>(std::stoll(oss.str())))) {
          input.clear();
          screen->PostEvent(Event::Special("ssendmsg"));
          list->TakeFocus();
        }
      return true;
    } else return false;
  }) | size(HEIGHT, EQUAL, 1);
  this->inputarea = einput | border;
}

// 聊天页面
Chat::Chat(int id_, Function& function_, ScreenInteractive* screen_)
  : Page(id_, function_, screen_) {
  getmessagelist();
  getinputarea();
  this->show_list = true;
  if(Info::info->messageinfo[id].size()) lastmsg = Info::info->messageinfo[id].back().msg;
  auto cchatpage = Container::Vertical({list, inputarea}, &inputfocus);
  auto echatpage = CatchEvent(cchatpage, [&](Event event){
    if(event == Event::CtrlN) {
      list->TakeFocus();
      if(selected_msg != Info::info->messageinfo[id].size())
        selected_msg ++;
      return true;
    }
    else if(event == Event::CtrlP) {
      list->TakeFocus();
      if(selected_msg > 1) selected_msg --;
      return true;
    }
    else if(event == Event::Special("sendmsg")) {
      list->TakeFocus();
      lastmsg = Info::info->messageinfo[id].back().msg;
      return false;
    }
    else inputarea->TakeFocus();
    return false;
  });
  selected_msg = Info::info->messageinfo[id].size();
  this->content = echatpage | flex;
}

// 朋友页面
Friend::Friend(int id_, Function& function_, int* page_selected_)
  : Page(id_, function_), page_selected(page_selected_) {
  auto container = Container::Horizontal({
    Button(" 󰍡  发送消息 ", [&]{
      Chat::chats_selected = id;
      *page_selected = CHAT_PAGE;
    }, ButtonOption::Ascii()),
    Button(" 󰆴  删除好友 ", [&]{
      function.deletefriend(id);
    }, ButtonOption::Ascii()),
  }, &friend_op_selected);
  auto rcontainer = Renderer(container, [=]{
    std::string username = Info::info->friendinfo[id].friendname;
    return vbox({
      text("   账号  :" + std::to_string(id)) | center,
      text("  用户名 :" + username) | center,
      container->Render() | center
    });
  });
  auto econtainer = CatchEvent(rcontainer, [&](Event event){
    if(event == Event::CtrlB) {
      friend_op_selected = 0;
      return true;
    } else if(event == Event::CtrlF) {
      friend_op_selected = 1;
      return true;
    }
    return false;
  });
  content = econtainer;
}

// 个人页面
Myself::Myself(Function& function_) : Page(function_) {
  content = Renderer([=]{
    std::string id = std::to_string(Info::info->myself.id);
    std::string username = Info::info->myself.username;
    return vbox(
      text("    账号  :" + id),
      text("  用户名 :" + username)
    ) | center;
  });
}

// 综合所有页面
void Vchat::getpage() {
  DLOG(INFO) << "friendinfo.size:" << Info::info->friendinfo.size();
  for(auto& v : Info::info->friendinfo)
    Chat::chats_map[v.first] = new Chat(v.first, function, screen);
  DLOG(INFO) << "chat_map.size:" << Chat::chats_map.size();
  for(auto& v : Info::info->friendinfo)
    Friend::friends_map[v.first] = new Friend(v.first, function, &page_selected);
  this->myself = new Myself(function);
  // 初始化开始页面位置
  Chat::chats_selected = Friend::friends_selected = Info::info->friendinfo.begin()->first;
  // 整合所有页面组件
  auto container = Container::Tab({}, &page_selected);
  auto page = Renderer(container, [=]{
    container->DetachAllChildren();
    auto first = Chat::chats_map[Chat::chats_selected]->content;
    auto second = Friend::friends_map[Friend::friends_selected]->content;
    auto third = myself->content;
    container->Add(first);
    container->Add(second);
    container->Add(third);
    return container->Render();
  });
  this->pages = page;
}

// 生成侧边栏消息列表
void Vchat::getmessagelist() {
  auto clist = Container::Vertical({}, &messages_selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    for (auto& v : Chat::chats_map) {
      if (!v.second->show_list) continue;
      std::string name = Info::info->friendinfo[v.first].friendname;
      auto but = Button(" " + name, [&]{
        Chat::chats_selected = v.first;
        page_selected = CHAT_PAGE;
        DLOG(INFO) << "press button";
      }, ButtonOption::Ascii());
      auto per = Renderer(but, [=]{
        return vbox(
          but->Render(),
          text("> " + v.second->lastmsg)
        );
      });
      clist->Add(per);
    }
    return clist->Render();
  });
  messageslist = rlist;
}

// 生成侧边栏朋友列表
void Vchat::getfriendlist() {
  auto clist = Container::Vertical({}, &friends_selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    for(auto& n : Info::info->friendinfo) {
      auto but = Button(n.second.friendname, [&]{
        Friend::friends_selected = n.first;
        page_selected = FRIENDS_PAGE;
      }, ButtonOption::Ascii());
      clist->Add(but);
    }
    return clist->Render();
  });
  auto elist = CatchEvent(rlist, [=](Event event) {
    if(event == Event::Special("accept_addfd")) { return true; }
    else if(event == Event::Special("refuse_addfd")) { return true; }
    return true;
  });
  friendslist = elist;
}

// 生成侧边栏个人功能列表
void Vchat::getmyselflist() {
  auto cinput = Input(&friend_input, "这里输入用户ID...");
  auto einput = CatchEvent(cinput, [&](Event event){
    if(event == Event::CtrlY) {
      if(!friend_input.empty())
        if(!function.addfriend(std::stoi(friend_input))) {
          friend_input.clear();
        }
      return true;
    } else return false;
    return false;
  }) | size(HEIGHT, EQUAL, 1) | border;
  auto rlist = Renderer([=]{
    Components addlist;
    addlist.emplace_back(einput);
    auto list = Container::Vertical(addlist, &myself_selected);
    return list->Render();
  });
  auto elist = CatchEvent(rlist, [=](Event event){
    if(event == Event::Special("addfd")) {
      for(auto& v : function.tempinfo.requestaddlist) {
        auto buts = Container::Horizontal({
          Button("添加", [&]{ function.responseadd(v, 1); }, ButtonOption::Ascii()),
          Button("拒绝", [&]{ function.responseadd(v, 0); }, ButtonOption::Ascii())
        });
        auto rbuts = Renderer(buts, [=]{
          Element show;
          show = vbox(
            text("账号" + std::to_string(v) + "请求添加为好友"),
            buts->Render()
          );
          return show;
        });
        content->Add(rbuts);
      }
      return true;
    }
    return false;
  });
  myselflist = elist;
}

// 综合所有侧边栏列表
void Vchat::getcatalogue() {
  getmessagelist();
  getfriendlist();
  getmyselflist();
  auto option = Container::Horizontal({
    Button( " 消息", [&] { page_selected = CHAT_PAGE; }, ButtonOption::Ascii()),
    Button( " 好友", [&] { page_selected = FRIENDS_PAGE; }, ButtonOption::Ascii()),
    Button( " 自己", [&] { page_selected = MYSELF_PAGE; }, ButtonOption::Ascii())},
  &option_selected);
  auto list = Container::Tab({messageslist, friendslist, myselflist}, &option_selected);
  auto catalogue = Container::Vertical({option, list});
  auto ecatalogue = CatchEvent(catalogue, [=](Event event) {
    if (event == Event::CtrlP) {
      if (!option->Focused())
        switch (option_selected) {
          case 0: if (messages_selected) messages_selected --; break;
          case 1: if (friends_selected) friends_selected --; break;
          case 2: if (myself_selected) myself_selected --; break;
        }
      list->TakeFocus();
    } else if (event == Event::CtrlN) {
      if (!option->Focused())
        switch (option_selected) {
        case 0: if (messages_selected != Chat::chats_map.size() - 1) messages_selected ++; break;
        case 1: if (friends_selected != Friend::friends_map.size() - 1) friends_selected ++; break;
        case 2: if (myself_selected != function.tempinfo.requestaddlist.size()) myself_selected ++; break;
        }
      list->TakeFocus();
    } else if (event == Event::CtrlB) {
      if (!list->Focused())
        if (!list_selected && option_selected)
          option_selected--;
      option->TakeFocus();
    } else if (event == Event::CtrlF) {
      if (!list->Focused())
        if (!list_selected && option_selected != 2)
          option_selected++;
      option->TakeFocus();
    }
    return false;
  });
  this->catalogue = ecatalogue | border;
}

// 整个聊天的主窗口，控制侧栏
Vchat::Vchat(int& now_, Function& function_, ScreenInteractive* screen_)
  : now(now_), function(function_), screen(screen_) {
  // 初始化页面与侧边栏目录
  this->getpage();
  this->getcatalogue();
  // 整合页面与侧边栏目录组件
  auto cmain = Container::Tab({catalogue, pages}, &catalogue_toggle);
  auto rmain = Renderer(cmain, [=] {
    Element show = pages->Render();
    if (!catalogue_toggle) {
      show = hbox(catalogue->Render(), show);
      catalogue->TakeFocus();
    }
    return show;
  });
  auto emain = CatchEvent(rmain, [&](Event event) {
    if (event == Event::CtrlL) {
      catalogue_toggle = catalogue_toggle ? 0 : 1;
      return true;
    }
    return false;
  });
  content = emain;
}

} // namespace vchat
