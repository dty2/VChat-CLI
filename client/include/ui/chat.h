#ifndef CHAT_H
#define CHAT_H

#include "ui.h"

namespace vchat {

using namespace ftxui;

// page
enum {
  messages = 0,
  friends,
  myself,
};

class Chat {
private:
  std::function<void(int)> pfun;
  int catalogue_toggle = 1;
  int option_selected = 0;

  // page
  int page_selected = 0;
  Component messagespage;
  Component friendspage;
  Component myselfpage;
  Component page;
  void getpage();
  void getmessagespage();
  void getfriendspage();
  void getmyselfpage();

  // catalogue
  int friends_selected = 0;
  int messages_selected = 0;
  int myself_selected = 0;
  int list_selected = 0;
  Component catalogue;
  void getcatalogue();

public:
  Component content;
  Chat(std::function<void(int)>);
};

} // namespace vchat

#endif // CHAT_H
