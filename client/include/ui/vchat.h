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

#ifndef VCHAT_H
#define VCHAT_H

#include "ui.h"
#include "function.h"
#include "data.hpp"

extern ScreenInteractive* screen;
extern std::unique_ptr<Function> function;

using namespace ftxui;

class Vchat;

// chat
class Chat {
private:
  int id;
  int selected;
  struct List {
    Component content;
    int selected;
    List(Chat*);
  }list;
  struct Input{
    std::string ss;
    Component content;
    Input(Chat*);
  }input;

public:
  Component content;
  Chat(int);
};

// friend
class Friend {
private:
  int id;
  int selected = 0;
public:
  Component content;
  Friend(int);
};


class Telescope;

// vchat
class Vchat {
private:
  int selected = 0;
  int page_selected = 0;
  int dialog_selected = 0;
  Telescope* telescope;
  std::unordered_map<int, Chat*> chats;
  std::unordered_map<int, Friend*> friends;
  Component about, help, pages, dialog, empty;
  void init_page(); // init all page
  bool handleEvent(Event, int);

public:
  Component page;
  Component content;
  void open_chat(int);
  void open_friend(int);
  void open_other(bool);
  Vchat();
  ~Vchat();
  Vchat(Vchat &&) = delete;
  Vchat(const Vchat &) = delete;
};

#endif // VCHAT_H
