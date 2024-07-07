#ifndef CHAT_H
#define CHAT_H

#include "ui.h"
#include "data.hpp"

namespace vchat {

using namespace ftxui;

// page
enum { CHAT_PAGE = 0, FRIENDS_PAGE, MYSELF_PAGE };

class Chat {
private:
  int id;
  Function &function;
  ScreenInteractive& screen;
  int selected_msg = 0;
  int inputfocus = 0; // 默认为0,焦点在消息框中
  std::string input;
  Component inputarea;
  Components messagelist;
  Component list;
  void updatemessagelist();
  void getinputarea();
  void getmessagelist();
  std::vector<MessageInfo> getmessage(int);

public:
  Chat(int, Function&, ScreenInteractive&);
  Component content;
};

class Friend {
private:
  int id;
  Function &function;
public:
  Friend(int, Function&);
  Component content;
};

class Myself {
private:
  Function &function;
public:
  Myself(Function &);
  Component content;
};

class Vchat {
private:
  int catalogue_toggle = 1;
  int option_selected = 0;
  int &now;
  Function &function;
  ScreenInteractive& screen;

  // page
  int page_selected = 0;
  std::unordered_map<int, Chat*> chats;
  std::unordered_map<int, Friend*> friends;
  Myself *myself;
  // catalogue
  int friends_selected = 0;
  int messages_selected = 0;
  int myself_selected = 0;
  int list_selected = 0;
  Component catalogue;
  Component pages;
  void getcatalogue();
  void getpage();

public:
  Component content;
  Vchat(int&, Function&, ScreenInteractive&);
};

} // namespace vchat

#endif // CHAT_H
