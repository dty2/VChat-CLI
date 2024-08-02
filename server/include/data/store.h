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

#ifndef STORE_H
#define STORE_H

#include "data.hpp"
#include "common.h"
#include <vector>

class Store {
private:
  const std::string db_address = "/home/hunter/source/project/vchat/resource/sql/vchat.db";
  std::unique_ptr<SQLite::Database> db;
  Store();
public:
  static Store* store;
  static void getinstance();
  static void free();
  Store(const Store&) = delete;
  Store& operator=(const Store&) = delete;

  bool getPersional(PersionalInfo&, const int);
  bool getPersional(std::vector<PersionalInfo>&, const std::string);
  bool getFriend(std::list<FriendInfo>&, const int);
  bool getMessage(std::list<MessageInfo>&, const int);
  bool getUser(UserInfo&, const int);
  bool insertPersional(PersionalInfo&);
  bool insertFriend(FriendInfo&, int);
  bool insertMessage(MessageInfo&);
};

#endif // STORE_H
