#include "chat.h"

namespace vchat {

Chat* Chat::chat = nullptr;

void Chat::getinstance() {
  chat = new Chat();
}

// c: component
// e: event
// r: render
// cer: component + event + render
// example: cer_main
Chat::Chat() : screen(ScreenInteractive::Fullscreen()) {
  input_toggle = false;
  selected_page = chat_page;
  search_toggle = 0;
  // -------------------
  //   prepare info
  // -------------------
  int &userid = Info::info->userinfo.persionalinfo.id;
  std::string &username = Info::info->userinfo.persionalinfo.username;
  std::list<FriendInfo>& friendlist = Info::info->userinfo.friendlist;
  std::list<MessageInfo>& messagelist = Info::info->userinfo.messagelist;

  // -------------------
  //        page
  // -------------------
  //    user page
  // -------------------
  auto c_user_page = Container::Vertical({
    Button("userpage", []{})
  });
  // -------------------
  //    friend page
  // -------------------
  auto c_friend_page = Container::Vertical({
    Button("friendpage", []{})
  });

  // -------------------
  //    chat page
  // -------------------
  //    chat list
  // -------------------
  int chatid = friendlist.begin()->friendid;
  int selected_msg = 0;
  auto c_chatlist = Container::Vertical({}, &selected_msg);
  auto cr_chatlist = Renderer(c_chatlist, [&]{
    c_chatlist->DetachAllChildren();
    std::vector<MessageInfo> chatlist;
    for(auto& value : messagelist)
      if(value.sender == chatid || value.receiver == chatid)
        chatlist.push_back(value);
    std::sort(chatlist.begin(), chatlist.end(),
      [&](MessageInfo x, MessageInfo y){ return x.time < y.time;}
    );
    for(auto& value : chatlist) {
      auto c_msg = Button(value.msg, []{});
      Component cr_msg;
      if(value.sender == chatid) {
        cr_msg = Renderer(c_msg, [&]{
          return hbox(text("") | flex, c_msg->Render());
        });
      } else if(value.receiver == chatid){
        cr_msg = Renderer(c_msg, [&]{
          return hbox(c_msg->Render(), text("") | flex);
        });
      }
      c_chatlist->Add(cr_msg);
    }
    selected_msg = chatlist.size();
    return c_chatlist->Render();
  }) | flex | frame | vscroll_indicator;

  // -------------------
  //    chat input
  // -------------------
  std::string input;
  auto c_input = Input(&input, "Here to input...");
  auto ce_input = CatchEvent(c_input, [&](Event event){
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* now_tm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_year - 100); // year
    oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_mon + 1); // month
    oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_mday); // date
    oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_hour); // hour
    oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_min); // minute
    oss << std::setfill('0') << std::setw(2) << std::to_string(now_tm->tm_sec); // second
    if(event == Event::Return) {
      Function::function->do_chat(userid, chatid, input, std::stoi(oss.str()));
      return true;
    } else return false;
  }) | size(HEIGHT, EQUAL, 6);


  // -------------------
  //     chat all
  // -------------------
  int chat_selected = 0;
  auto c_chat_page = Container::Vertical({}, &chat_selected);
  auto cr_chat_page = Renderer(c_chat_page, [&]{
    if(input_toggle) {
      c_chat_page->Add(ce_input);
    } else {
      c_chat_page->DetachAllChildren();
      c_chat_page->Add(cr_chatlist);
    }
    return c_chat_page->Render();
  });
  auto cre_chat_page = CatchEvent(cr_chat_page, [&](Event event){
    if(event == Event::Escape) {
      input_toggle = input_toggle ? false : true;
      if(input_toggle) {
        selected_msg = messagelist.size();
        chat_selected = cr_chat_page->ChildCount();
      }
      return true;
    } else return false;
  });

  // -------------------
  //    status time
  // -------------------
  std::atomic<bool> refresh_time = true;
  std::string logo_time;
  std::thread timeupdate([&]{
    while(refresh_time) {
      std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm* now_tm = std::localtime(&now);
      std::string time(std::asctime(now_tm));
      logo_time = "   " + time.substr(11, 8) + " ";
      std::this_thread::sleep_for(std::chrono::seconds(1));
      screen.PostEvent(Event::Custom);
    }
  });

  // -------------------
  //       page
  // -------------------
  auto c_page = Container::Tab({
    cre_chat_page,
    c_friend_page,
    c_user_page,
  }, &selected_page);
  auto cr_page = Renderer(c_page, [&]{
    Element show = vbox(
      c_page->Render() | flex,
      hbox(
        text(input_toggle ? " CHAT " : " NORAML ")
          | (input_toggle ? bgcolor(Color::Green) : bgcolor(Color::Blue)),
        text("") | flex | bgcolor(Color::RGB(30, 32, 48)),
        text(logo_time) | bgcolor(Color::Blue)
      )
    );
    return show;
  });
  auto cre_page = CatchEvent(cr_page, [&](Event event)->bool{
    if(event == Event::CtrlJ) {
      cre_chat_page->TakeFocus();
      selected_page = chat_page;
      return true;
    } else if(event == Event::CtrlK) {
      c_friend_page->TakeFocus();
      selected_page = friend_page;
      return true;
    } else if(event == Event::CtrlL) {
      c_user_page->TakeFocus();
      selected_page = user_page;
      return true;
    } else return false;
  });

  // -------------------
  //     search
  // -------------------
  auto c_search = Container::Vertical({});
  auto cr_search = Renderer(c_search, [&]{
    c_search->DetachAllChildren();
    for(auto& value : friendlist) {
      c_search->Add(Button(value.friendname, [&]{ chatid = value.friendid; }, ButtonOption::Ascii()));
    }
    return c_search->Render();
  });

  // -------------------
  //      main
  // -------------------
  auto c_main = Container::Tab({ cre_page, cr_search }, &search_toggle);
  auto cr_main = Renderer(c_main, [&]{
    Element show = cre_page->Render();
    if(search_toggle) {
      show = dbox( cre_page->Render(), cr_search->Render() | clear_under | center);
      c_main->SetActiveChild(cr_search);
    } else {
      c_main->SetActiveChild(cre_page);
    }
    return show;
  });
  auto cre_main = CatchEvent(cr_main, [&](Event event)->bool{
    if(event == Event::CtrlU) {
      search_toggle = search_toggle ? 0 : 1;
      return true;
    } else return false;
  });

  // -------------------
  //    start loop
  // -------------------
  screen.Loop(cre_main);
  refresh_time = false;
  timeupdate.join();
}

} // namespace vchat
