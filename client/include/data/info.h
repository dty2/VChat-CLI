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

#ifndef INFO_H
#define INFO_H

#include "common.h"
#include "data.hpp"

class Info {
private:
  Info();
  Info(const Info &) = delete;
  Info &operator=(const Info &) = delete;
  std::mutex mtx;

public:
  PersionalInfo myself;
  std::map<int, FriendInfo> friendinfo;
  std::map<int, std::vector<MessageInfo>> messageinfo;
  std::map<int, Json::Value> requestaddlist; // 好友请求添加列表
  std::map<int, Json::Value> findlist; // 搜索列表
  static Info *info;
  static void getinstance();
  void change(std::function<void()>);
};

#endif // INFO_H
