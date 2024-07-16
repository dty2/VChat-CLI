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

#ifndef USERINFO_HPP
#define USERINFO_HPP

#include "common.h"

typedef struct MessageInfo {
  int sender;
  int receiver;
  std::string msg;
  int64_t time;
  MessageInfo(){};
  MessageInfo(int sender_, int receiver_, std::string msg_, std::time_t time_)
      : sender(sender_), receiver(receiver_), msg(msg_), time(time_) {}
} MessageInfo;

typedef struct FriendInfo {
  int friendid;
  std::string friendname;
  FriendInfo(){};
  FriendInfo(int id_, std::string friendname_)
      : friendid(id_), friendname(friendname_) {}
} FriendInfo;

typedef struct PersionalInfo {
  int id;
  int password;
  std::string username;
  PersionalInfo() {}
  PersionalInfo(int id_, int password_, std::string username_)
    : id(id_), password(password_), username(username_) {}
} PersionalInfo;

typedef struct UserInfo {
  PersionalInfo persionalinfo;
  std::map<int, std::vector<FriendInfo>> friendinfo;
  std::map<int, std::vector<MessageInfo>> messageinfo;
  UserInfo(){};
} UserInfo;

#endif // USERINFO_HPP
