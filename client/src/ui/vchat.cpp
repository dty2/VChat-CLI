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
  auto rlist = Renderer([=]{
    messagelist.clear();
    std::vector<MessageInfo> messages;
    for (auto& v : Info::info->userinfo.messagelist) {
      if (v.receiver == id || v.sender == id)
        messages.emplace_back(v);
    }
    for(auto& value : messages) {
      messagelist.emplace_back(
        Renderer([=](bool focused){
          Element element;
          if(value.sender == id) {
            element = hbox(text(" > "), text(value.msg), filler());
          } else if(value.receiver == id){
            element = hbox(filler(), text(value.msg), text(" <<  "));
          }
          if(focused) element |= focus;
          return element;
        })
      );
    }
    auto show = Container::Vertical(messagelist, &selected_msg) | vscroll_indicator | frame;
    return show->Render();
  });
  auto elist = CatchEvent(rlist, [&](Event event){
    if(event == Event::CtrlN) {
      if(selected_msg != list->ChildCount() - 1) selected_msg ++;
      return true;
    }
    else if(event == Event::CtrlP) {
      if(selected_msg) selected_msg --;
      return true;
    }
    else if(event == Event::Special("ssendmsg")) {
      LOG(INFO) << "update last msg";
      for(auto& v : Info::info->userinfo.messagelist)
        if(v.sender == id || v.receiver == id) lastmsg = v.msg;
      return true;
    }
    return false;
  });
  this->list = elist | border | flex;
}

// 生成输入区域
void Chat::getinputarea() {
  auto cinput = Input(&input, "Here to input...");
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
        if(!function.sendmsg(id, Info::info->userinfo.persionalinfo.id, input,
          static_cast<int64_t>(std::stoll(oss.str())))) {
          input.clear();
          selected_msg = messagelist.size() - 1;
          screen->PostEvent(Event::Special("ssendmsg"));
          list->TakeFocus();
        }
      return true;
    } else return false;
  }) | size(HEIGHT, EQUAL, 1);
  this->inputarea = einput | border;
}

// 聊天页面
Chat::Chat(int id_, Function& function_, ScreenInteractive* screen_) : Page(id_, function_, screen_) {
  if (id == -1) {
    this->content = Renderer([=](bool focused){
      Element element = text("没有消息，快去聊天吧...") | center;
      if(focused) element |= focus;
      return element;
    });
  } else {
    getmessagelist();
    getinputarea();
    for(auto& v : Info::info->userinfo.messagelist)
      if(v.sender == id || v.receiver == id) lastmsg = v.msg;
    auto cchatpage = Container::Vertical({list, inputarea}, &inputfocus);
    auto echatpage = CatchEvent(cchatpage, [&](Event event){
      if(event == Event::CtrlN) {
        list->TakeFocus();
        if(selected_msg != messagelist.size() - 1)
          selected_msg ++;
        return true;
      }
      else if(event == Event::CtrlP) {
        list->TakeFocus();
        if(selected_msg)
          selected_msg --;
        return true;
      }
      else if(event != Event::Special("ssendmsg")) {
        inputarea->TakeFocus();
      }
      return false;
    });
    selected_msg = messagelist.size() - 1;
    this->content = echatpage | flex;
  }
}

// 朋友页面
Friend::Friend(int id_, Function& function_) : Page(id_, function_) {
  content = Container::Horizontal({
    Button(" 󰍡  发消息 ", [&]{ }, ButtonOption::Ascii()),
    Button(" 󰆴 删除好友 ", [&]{ }, ButtonOption::Ascii()),
  });
}

// 个人页面
Myself::Myself(Function& function_) : Page(function_) {
  content = Renderer([=]{
    std::string id = std::to_string(Info::info->userinfo.persionalinfo.id);
    std::string username = Info::info->userinfo.persionalinfo.username;
    return vbox(
      text(" 账号  :" + id),
      text("用户名 :" + username)
    ) | center;
  });
}

// 综合所有页面
void Vchat::getpage() {
  for(auto& v : Info::info->userinfo.friendlist)
    Chat::chats_map[v.friendid] = new Chat(v.friendid, function, screen);
  DLOG(INFO) << "chats size: " << Chat::chats_map.size();
  if(!Chat::chats_map.size())
    Chat::chats_map[-1] = new Chat(-1, function, screen);
  for(auto& v : Info::info->userinfo.friendlist)
    Friend::friends_map[v.friendid] = new Friend(v.friendid, function);
  DLOG(INFO) << "friendlist size: " << Info::info->userinfo.friendlist.size();
  if(!Friend::friends_map.size())
    Friend::friends_map[-1] = new Friend(-1, function);
  Chat::chats_selected = Chat::chats_map.begin()->second->id;
  Friend::friends_selected = Friend::friends_map.begin()->second->id;
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
  auto container = Container::Vertical({}, &messages_selected);
  auto content = Renderer(container, [=]{
    container->DetachAllChildren();
    for (auto& v : Chat::chats_map) {
      Component but;
      for(auto& n : Info::info->userinfo.friendlist)
        if(n.friendid == v.first) {
          but = Button(n.friendname, [&]{
            Chat::chats_selected = n.friendid;
            LOG(INFO) << "change chats_selected";
          }, ButtonOption::Ascii());
          break;
        }
      auto per = Renderer(but, [=]{
        return vbox(
          but->Render(),
          text("> " + v.second->lastmsg)
        );
      });
      container->Add(per);
    }
    return container->Render();
  });
  messageslist = content;
}

// 生成侧边栏朋友列表
void Vchat::getfriendlist() {
  auto container = Container::Vertical({}, &friends_selected);
  auto content = Renderer(container, [=]{
    container->DetachAllChildren();
    for(auto& n : Info::info->userinfo.friendlist) {
      auto temp = Button(n.friendname, [&]{}, ButtonOption::Ascii());
      container->Add(temp);
    }
    return container->Render();
  });
  auto econtent = CatchEvent(content, [=](Event event) {
    if(event == Event::Special("caddfd_suc")) {
    } else if(event == Event::Special("saddfd_suc")) {
    }
    return false;
  });
  friendslist = econtent;
}

// 生成侧边栏个人功能列表
void Vchat::getmyselflist() {
  auto content = Container::Vertical( {
    Button("myselflist1", [&] {}, ButtonOption::Ascii()),
  }, &myself_selected);
  myselflist = content;
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
          case 0: if (messages_selected) messages_selected--; break;
          case 1: if (friends_selected) friends_selected--; break;
          case 2: if (myself_selected) myself_selected--; break;
        }
      list->TakeFocus();
    } else if (event == Event::CtrlN) {
      if (!option->Focused())
        switch (option_selected) {
        case 0: if (messages_selected != Chat::chats_map.size() - 1) messages_selected++; break;
        case 1: if (friends_selected != Friend::friends_map.size() - 1) friends_selected++; break;
        case 2: if (myself_selected != 2) myself_selected++; break;
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
    } else if (event == Event::Return) {
      return false;
    }
    return true;
  });
  this->catalogue = ecatalogue | border;
}

// 整个聊天的主窗口
Vchat::Vchat(int& now_, Function& function_, ScreenInteractive* screen_)
  : now(now_), function(function_), screen(screen_) {
  this->myself = new Myself(function);
  this->getpage();
  this->getcatalogue();
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
