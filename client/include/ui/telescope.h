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

#ifndef TELESCOPE_H
#define TELESCOPE_H

#include "function.h"
#include "ui.h"

using namespace ftxui;

extern ScreenInteractive* screen;
extern std::unique_ptr<Function> function;

enum { COMMON = 0, CHAT, FRIEND, GROUP };
enum { LIST = 0, INPUT };

class Vchat;
class Telescope;

class Common {
public:
  int selected;
  Common(Telescope*);
  Component list;
  std::unordered_map<int, Element> previews;
};

class Chats {
public:
  void refresh_preview();
  int selected;
  Chats(Telescope*);
  Component list;
  std::unordered_map<int, Element> previews;
};

class Friends {
public:
  void refresh_preview();
  int selected;
  Friends(Telescope*);
  Component list;
  std::unordered_map<int, Element> previews;
};

class Inform {
private:
  std::map<int, int> id;
public:
  int selected;
  Inform(Telescope*);
  Component list;
  std::unordered_map<int, Element> previews;
};

class Telescope {
private:
  Common common;
  Chats chats;
  Friends friends;
  Inform inform;
  std::string ss;
  int selected = INPUT;
  int list_selected = CHAT;

public:
  int *toggle;
  Vchat *vchat;
  Telescope(Vchat*, int*);
  Telescope(Telescope &&) = delete;
  Telescope(const Telescope &) = delete;
  ~Telescope();
  Component content;

};

#endif // TELESCOPE_H
