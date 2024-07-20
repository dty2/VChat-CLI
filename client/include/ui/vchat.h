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

#ifndef CHAT_H
#define CHAT_H

#include "ui.h"
#include "function.h"
#include "data.hpp"

extern ScreenInteractive* screen;
extern std::unique_ptr<Function> function;

using namespace ftxui;

namespace vchat{

enum { MYSELF = 0, CHAT, FRIEND, SEARCH };

// myself
class MyselfPage {
private:
  struct List {
    Component content;
    int selected;
    MyselfPage* myself;
    List(MyselfPage*);
  }list;
  int choose_notify; // choose notify or list

public:
  int choose_add;
  static int choose_list; // choose list or myself info
  MyselfPage();
  Component content;
};

// chat page
class ChatPage {
private:
  struct Chat {
    int id;
    int selected_msg = 0;
    int inputfocus = 0;
    std::string input;
    Component inputarea;
    Component messagelist;
    bool showlist;
    std::string lastmsg;
    void getinput();
    void getmessage();
    Chat(int);
    Component content;
  };
  struct List {
    Component content;
    int selected;
    ChatPage* chat;
    List(ChatPage*);
  }list;
  std::unordered_map<int, struct Chat*> chats;

public:
  static int chats_selected;
  static int choose_list; // choose list or chat info
  Component content;
  ChatPage();
};

// friend
class FriendPage {
private:
  struct Friend {
    int friend_op_selected = 0;
    int id;
    Component content;
    Friend(int);
  };
  struct List {
    Component content;
    int selected;
    FriendPage* friend_;
    List(FriendPage*);
  }list;
  std::unordered_map<int, struct Friend*> friends;

public:
  static int friends_selected;
  static int choose_list; // choose list or friendinfo
  Component content;
  FriendPage();
};

// search
class Search {
private:
  struct Show {
    Component content;
    Show();
  };
  int list_selected;

public:
  static int choose_list; // choose input or show
  Component content;
  Search();

};

// vchat
class Vchat {
private:
  int &now;
  int selected; // choose options, pages
  Component options;
  MyselfPage* myself;
  ChatPage* chat;
  FriendPage* friend_;
  void getoptions();
  int options_selected;

public:
  static int pages_selected;
  Component content;
  Vchat(int&);
};

} // namespace vchat

#endif // CHAT_H
