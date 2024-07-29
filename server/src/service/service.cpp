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

#include "service.h"
#include "connection.h"
#include "package.h"

Service* Service::service = nullptr;
Service::Cache Service::cache;

Service::Service() {}

void Service::getinstance() { service = new Service(); }

void Service::serve(int method, Json::Value value, Connection *connection) {
  switch (method) {
    case method::signin: service->signin(value, connection); break;
    //case method::signout: service->signout(value, connection); break;
    case method::login: service->login(value, connection); break;
    //case method::logout: service->logout(value, connection); break;
    //case method::findfd: service->find(value, connection); break;
    case method::addfd: service->addfriend(value, connection); break;
    case method::accept_addfd: service->addfriend(method, value, connection); break;
    case method::refuse_addfd: service->addfriend(method, value, connection); break;
    //case method::cdeletefd: service->deletefriend(value, connection); break;
    case method::sendmsg: service->sendmsg(value); break;
  }
}

void Service::signin(Json::Value value, Connection* connection) {
  try {
    PersionalInfo persionalinfo;
    persionalinfo.id = value["id"].asInt();
    persionalinfo.password = value["password"].asInt();
    persionalinfo.username = value["username"].asString();
    if(Store::store->insertPersional(persionalinfo)) {
      connection->write(method::signin_suc, 0);
    } else {
      connection->write(method::signin_idexist, 0);
    }
  } catch (const std::exception& e) {
    connection->write(method::signin_fmerr, 0);
  }
}

void Service::login(Json::Value value, Connection* connection) {
  try {
    int id = value["id"].asInt();
    int password = value["password"].asInt();
    PersionalInfo persionalinfo;
    if(Store::store->getPersional(persionalinfo, id)) {
      LOG(INFO) << id << " log error" << " because no id";
      connection->write(method::login_noid, value);
      return;
    }
    if (persionalinfo.password == password) {
      UserInfo userinfo;
      Store::store->getUser(userinfo, id);
      Json::Value root, persionalinfo, friendlist, messagelist;
      persionalinfo["id"] = userinfo.persionalinfo.id;
      persionalinfo["password"] = userinfo.persionalinfo.password;
      persionalinfo["username"] = userinfo.persionalinfo.username;
      for (auto x : userinfo.friendlist) {
        Json::Value friendinfo;
        friendinfo["friendid"] = x.friendid;
        friendinfo["friendname"] = x.friendname;
        friendlist.append(friendinfo);
      }
      for (auto x : userinfo.messagelist) {
        Json::Value messageinfo;
        messageinfo["sender"] = x.sender;
        messageinfo["receiver"] = x.receiver;
        messageinfo["message"] = x.msg;
        messageinfo["time"] = x.time;
        messagelist.append(messageinfo);
      }
      root["persionalinfo"] = persionalinfo;
      root["friendlist"] = friendlist;
      root["messagelist"] = messagelist;
      Service::cache.online[id] = connection;
      LOG(INFO) << id << " log in";
      connection->write(method::login_suc, root);
      for (auto x : this->cache.addfriendapply[id])
        connection->write(method::addfd, x);
    } else {
      LOG(INFO) << id << " log error" << " because password error";
      connection->write(method::login_pwerr, value);
    }
  } catch (const std::exception& e) {
    connection->write(method::login_err, 0);
  }
}

void Service::sendmsg(Json::Value value) {
  try {
    MessageInfo messageinfo;
    messageinfo.sender = value["sender"].asInt();
    messageinfo.receiver = value["receiver"].asInt();
    messageinfo.msg = value["message"].asString();
    messageinfo.time = value["time"].asInt64();
    bool op = Store::store->insertMessage(messageinfo);
    if(Service::cache.online[messageinfo.receiver]) {
      Service::cache.online[messageinfo.receiver]->write(method::sendmsg, value);
    }
  } catch (const std::exception& e) {
  }
}

void Service::addfriend(Json::Value value, Connection* connection) {
  try {
    int friendid = value["friendid"].asInt();
    if(Service::cache.online[friendid])
      Service::cache.online[friendid]->write(method::addfd, value);
    else
      Service::cache.addfriendapply[friendid].emplace_back(value);
  } catch (const std::exception& e) {
  }
}

// 这里添加好友比较复杂，在接受好友申请方同意后
// 需要对数据库添加两个好友记录如 1 2 test1 和 2 1 test2
// 置于为什么要添加两次，因为当初在设计的时候考虑到取数据的时候方便写因此就这么设计了
// 但是由于这里的函数内部变量名字起得乱七八糟，因此这里压根看不出来谁是被加好友的，谁是主动加好友的
// 这里在之后的版本百分之百会进行修改，因为这里的变量名字起得真的是太糟糕了，在写的时候就给自己绕懵了
// 为什么不在写的时候就起一个好名字？好问题，因为想给日后的自己留个天坑
void Service::addfriend(int method, Json::Value value, Connection* connection) {
  try {
    FriendInfo friendinfo;
    int userid = value["friendid"].asInt(); // 添加好友的人
    friendinfo.friendid = value["userid"].asInt(); // 被添加的人
    friendinfo.friendname = value["username"].asString();
    if (method == method::accept_addfd) {
      bool op = Store::store->insertFriend(friendinfo, userid);
      userid = friendinfo.friendid;
      PersionalInfo temp;
      Store::store->getPersional(temp, value["friendid"].asInt());
      friendinfo.friendid = temp.id;
      friendinfo.friendname = temp.username;
      bool opp = Store::store->insertFriend(friendinfo, userid);
      if(Service::cache.online[friendinfo.friendid])
        Service::cache.online[friendinfo.friendid]->write(method::accept_addfd, value);
    } else {
      if(Service::cache.online[friendinfo.friendid])
        Service::cache.online[friendinfo.friendid]->write(method::refuse_addfd, value);
    }
  } catch (const std::exception& e) {
  }
}
