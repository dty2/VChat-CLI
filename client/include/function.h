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

#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "info.h"
#include "net.h"
#include "package.h"

class Function {
private:
  bool mode = 0;
  void handle(int, Json::Value);
  void handle_login(Json::Value&);
  void handle_find(Json::Value&);
  void handle_addfd(Json::Value&);
  void handle_addfd(int, Json::Value&);
  void handle_msg(Json::Value&);
  Net net;

public:
  Function(std::string, std::string);
  std::function<void(std::string)> postevent;
  void start();
  void end();
  bool login(int, int);
  bool logout(int);
  bool signin(int, int, std::string);
  bool signout(int, int);
  bool sendmsg(int, int, std::string, int64_t);
  bool find(int);
  bool find(std::string);
  bool addfriend(int);
  bool responseadd(int, bool);
  bool deletefriend(int);

};

#endif // FUNCTION_H
