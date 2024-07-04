#include "chat.h"

namespace vchat {

Chat::Chat(std::function<void(int)> fun) : pfun(fun) {
  getpage();
  getcatalogue();
  // main
  auto cmain = Container::Tab({catalogue, page}, &catalogue_toggle);
  auto rmain = Renderer(cmain, [&] {
    Element show = page->Render();
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

void Chat::getmessagespage() {
  // chat input
  int chatid = friendlist.begin()->friendid;
  std::string input;
  int selected_msg = messagelist.size();
  auto c_chatlist = Container::Vertical({}, &selected_msg);
  auto c_input = Input(&input, "Here to input...");
  auto ce_input = CatchEvent(c_input, [&](Event event){
    if(event == Event::CtrlY) {
      std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm* now_tm = std::localtime(&now);
      std::ostringstream oss;
      oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_year - 100); // year
      oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_mon + 1); // month
      oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_mday); // date
      oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_hour); // hour
      oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_min); // minute
      oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_sec); // second
      if(!input.empty()) {
        if(Function::function->do_chat(userid, chatid, input,
          static_cast<int64_t>(std::stoll(oss.str())))) {
          screen.PostEvent(Event::Custom);
          input.clear();
        }
      }
      return true;
    } else return false;
  }) | size(HEIGHT, EQUAL, 6);

  // chat list
  auto cr_chatlist = Renderer(c_chatlist, [&]{
    c_chatlist->DetachAllChildren();
    std::vector<MessageInfo> chatlist;
    for(auto& value : messagelist)
      if(value.sender == chatid || value.receiver == chatid) {
        chatlist.push_back(value);
        DLOG(INFO) << value.msg;
      }
    std::sort(chatlist.begin(), chatlist.end(),
      [&](MessageInfo x, MessageInfo y){ return x.time < y.time;}
    );
    for(auto& value : chatlist) {
      c_chatlist->Add( Renderer([&](bool focused){
      Element element;
      if(value.sender == chatid) {
        element = hbox(text(value.msg), filler());
      } else if(value.receiver == chatid){
        element = hbox(filler(), text(value.msg));
      }
      if(focused) element |= focus;
      return element;
      }));
    }
    if(!c_chatlist->ChildCount()) {
      c_chatlist->Add( Renderer([&](bool focused){
        Element element = text("no message, try to send first...");
        if(focused) element |= focus;
        return element;
      }));
    }
    return c_chatlist->Render() | vscroll_indicator | frame | flex;
  });

  // chat all
  int chat_selected = 0;
  auto c_chat_page = Container::Vertical({}, &chat_selected);
  auto cr_chat_page = Renderer(c_chat_page, [&]{
    if(input_toggle) {
      c_chat_page->Add(ce_input);
      ce_input->TakeFocus();
    } else {
      c_chat_page->DetachAllChildren();
      c_chat_page->Add(cr_chatlist);
    }
    return c_chat_page->Render();
  });
  auto cre_chat_page = CatchEvent(cr_chat_page, [&](Event event){
    if(event == Event::Escape) {
      input_toggle = input_toggle ? false : true;
      if(input_toggle) chat_selected = 1;//cr_chat_page->ChildCount();
      return true;
    } else return false;
  });
  messagespage = emessagepage;
}

void Chat::getfriendspage() {}

void Chat::getmyselfpage() {}

void Chat::getpage() {
  getmessagespage();
  getfriendspage();
  getmyselfpage();
  auto page = Container::Tab({messagespage, friendspage, myselfpage}, &option_selected);
  this->page = page | border;
}

void Chat::getcatalogue() {
  // option
  auto option = Container::Horizontal(
      {Button(
           "   消息 ", [&] { page_selected = messages; },
           ButtonOption::Ascii()),
       Button(
           "   好友 ", [&] { page_selected = friends; },
           ButtonOption::Ascii()),
       Button(
           "   自己 ", [&] { page_selected = myself; },
           ButtonOption::Ascii())},
      &option_selected);
  // message list
  auto messageslist = Container::Vertical(
      {
          Button(
              " messagelist1", [&] {}, ButtonOption::Ascii()),
          Button(
              " messagelist2", [&] {}, ButtonOption::Ascii()),
          Button(
              " messagelist3", [&] {}, ButtonOption::Ascii()),
      },
      &messages_selected);
  // friend list
  auto friendslist = Container::Vertical(
      {
          Button(
              " friendlist1", [&] {}, ButtonOption::Ascii()),
          Button(
              " friendlist2", [&] {}, ButtonOption::Ascii()),
          Button(
              " friendlist3", [&] {}, ButtonOption::Ascii()),
      },
      &friends_selected);
  // myself list
  auto myselflist = Container::Vertical(
      {
          Button(
              " myselflist1", [&] {}, ButtonOption::Ascii()),
          Button(
              " myselflist2", [&] {}, ButtonOption::Ascii()),
          Button(
              " myselflist3", [&] {}, ButtonOption::Ascii()),
      },
      &myself_selected);
  // list
  auto list =
      Container::Tab({messageslist, friendslist, myselflist}, &option_selected);
  // main
  auto catalogue = Container::Vertical({option, list});
  auto ecatalogue = CatchEvent(catalogue, [=](Event event) {
    if (event == Event::CtrlP) {
      if (!option->Focused())
        switch (option_selected) {
        case 0:
          if (messages_selected)
            messages_selected--;
          break;
        case 1:
          if (friends_selected)
            friends_selected--;
          break;
        case 2:
          if (myself_selected)
            myself_selected--;
          break;
        }
      list->TakeFocus();
    } else if (event == Event::CtrlN) {
      if (!option->Focused())
        switch (option_selected) {
        case 0:
          if (messages_selected != 2)
            messages_selected++;
          break;
        case 1:
          if (friends_selected != 2)
            friends_selected++;
          break;
        case 2:
          if (myself_selected != 2)
            myself_selected++;
          break;
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

} // namespace vchat
