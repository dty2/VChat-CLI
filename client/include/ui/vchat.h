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

#include "function.h"
#include "ui.h"
#include "data.hpp"

using namespace ftxui;

// page
enum { CHAT_PAGE = 0, FRIENDS_PAGE, MYSELF_PAGE };

// 页面父类，三种初始化方式分别对应不同的页面子类
class Page {
public:
  int id;
  Function &function;
  ScreenInteractive *screen;
  Page(int, Function&, ScreenInteractive*);
  Page(int, Function&);
  Page(Function&);
  Component content;
};

// 聊天页面
class Chat : public Page {
private:
  int selected_msg = 0;
  int inputfocus = 0; // 默认为0,焦点在消息框中
  std::string input;
  Component inputarea;
  Component list;
  void getinputarea();
  void getmessagelist();

public:
  static std::unordered_map<int, Chat*> chats_map;
  static int chats_selected;
  bool show_list;
  Chat(int, Function&, ScreenInteractive*);
  std::string lastmsg;
};

// 朋友页面
class Friend : public Page{
private:
  int* page_selected;
  int friend_op_selected = 0;
public:
  static std::unordered_map<int, Friend*> friends_map;
  static int friends_selected;
  Friend(int, Function&, int*);
  Component content;
};

// 个人页面
class Myself : public Page {
private:
  std::string friend_input;
public:
  Myself(Function&);
};

// 聊天主页面
class Vchat {
private:
  int catalogue_toggle = 1;
  int option_selected = 0;
  int &now;
  Function &function;
  ScreenInteractive *screen;
  // 页面信息
  int page_selected = 0;
  Myself *myself;
  Component pages;
  void getpage();
  // 侧边栏目录信息
  int list_selected = 0;
  Component catalogue;
  int messages_selected = 0;
  Component messageslist;
  int friends_selected = 0;
  Component friendslist;
  int myself_selected = 0;
  Component myselflist;
  int lr = 0;
  void getmessagelist();
  void getfriendlist();
  void getmyselflist();
  void getcatalogue();

public:
  Component content;
  Vchat(int&, Function&, ScreenInteractive*);
};

#endif // CHAT_H
