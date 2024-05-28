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
  // -------------------
  //      userinfo
  // -------------------
  int userid = Info::info->userinfo.persionalinfo.id;
  std::string username = Info::info->userinfo.persionalinfo.username;
  std::vector<FriendInfo> friendlist(
    Info::info->userinfo.friendlist.begin(),
    Info::info->userinfo.friendlist.end()
  );
  std::vector<MessageInfo> messagelist(
    Info::info->userinfo.messagelist.begin(),
    Info::info->userinfo.messagelist.end()
  );

  // -------------------
  //      user_page
  // -------------------
  auto c_user_page = Container::Vertical({

  });
 
  // -------------------
  //     friend_page
  // -------------------
  auto c_friend_page= Container::Vertical({

  });

  // -------------------
  //     chat_page
  // -------------------
  int chatid = friendlist[0].friendid;
  auto c_chatlist = Container::Vertical({});
  for(auto& value : messagelist) {
    if(value.sender == userid) c_chatlist->Add(Renderer([&]{ return text(value); }) | right);
    if(value.sender == chatid) c_chatlist->Add(Renderer([&]{ return text(value); }) | left);
  }


  // -------------------
  //     tool_page
  // -------------------
  auto c_tool_page = Container::Vertical({

  });

  // -------------------
  //      context
  // -------------------
  auto c_context = Container::Tab({
    //c_user_page,
    //c_friend_page,
    //c_chat_page,
    //c_tool_page
  }, &selected);
  auto cr_context = Renderer(c_context, [&]{
    Element show = NULL;
    switch (selected) {
      case user_page:            ; break;
      case friend_page:          ; break;
      case chat_page:            ; break;
      case tool_page:            ; break;
    }
    return vbox(show) | flex;
  });

  // -------------------
  //      status
  // -------------------
  std::atomic<bool> refresh_time = true;
  std::string logo_time;
  std::thread timeupdate([&]{
    while(refresh_time) {
      std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm* now_tm = std::localtime(&now);
      std::string time(std::asctime(now_tm));
      logo_time = "  " + time.substr(11, 8) + " ";
      std::this_thread::sleep_for(std::chrono::seconds(1));
      screen.PostEvent(Event::Custom);
    }
  });
  auto c_stauts = Container::Horizontal({
    Renderer([&]{
      return text((mode ? " NORAML " : " CHAT "))
      | (mode ? bgcolor(Color::Blue) : bgcolor(Color::Green));
    }),
    Renderer([]{ return text("") | flex | bgcolor(Color::RGB(30, 32, 48)); }),
    Renderer([&]{ return text(logo_time) | bgcolor(Color::Blue); })
  }); 

  // -------------------
  //        main
  // -------------------
  auto c_main = Container::Vertical({ c_context, c_stauts });
  auto ce_main = CatchEvent(c_main, [&](Event event){
    return false;
  });

  // -------------------
  //    start loop
  // -------------------
  screen.Loop(ce_main);
  refresh_time = false;
  timeupdate.join();
}

} // namespace vchat
