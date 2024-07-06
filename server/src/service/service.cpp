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
    case method::caddfd: service->addfriend(method, value, connection); break;
    case method::caddfd_suc: service->addfriend(method, value, connection); break;
    case method::caddfd_err: service->addfriend(method, value, connection); break;
    //case method::cdeletefd: service->deletefriend(value, connection); break;
    case method::csendmsg: service->sendmsg(value); break;
  }
}

void Service::login(Json::Value value, Connection* connection) {
  int id = value["id"].asInt();
  int password = value["password"].asInt();
  PersionalInfo persionalinfo;
  Store::store->getPersional(persionalinfo, id);
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
    connection->write(method::login_suc, root);
  }
}

void Service::signin(Json::Value value, Connection* connection) {
  PersionalInfo persionalinfo;
  persionalinfo.id = value["id"].asInt();
  persionalinfo.password = value["password"].asInt();
  persionalinfo.username = value["username"].asString();
  bool op = Store::store->insertPersional(persionalinfo);
  connection->write(method::signin_suc, 0);
}

void Service::sendmsg(Json::Value value) {
  MessageInfo messageinfo;
  messageinfo.sender = value["sender"].asInt();
  messageinfo.receiver = value["receiver"].asInt();
  messageinfo.msg = value["message"].asString();
  messageinfo.time = value["time"].asInt64();
  bool op = Store::store->insertMessage(messageinfo);
  if(Service::cache.online[messageinfo.receiver]) {
    Service::cache.online[messageinfo.receiver]->write(method::ssendmsg, value);
  }
}

void Service::addfriend(Json::Value value, Connection* connection) {
  FriendInfo friendinfo;
  friendinfo.friendid = value["friendid"].asInt();
  if(Service::cache.online[friendinfo.friendid]) {
    Service::cache.online[friendinfo.friendid]->write(method::saddfd, value);
  }
}

void Service::addfriend(int method, Json::Value value, Connection* connection) {
  FriendInfo friendinfo;
  auto userid = value["userid"].asInt();
  friendinfo.friendid = value["friendid"].asInt();
  if (method == method::caddfd_suc) {
    bool op = Store::store->insertFriend(friendinfo, userid);
    if(Service::cache.online[userid])
      Service::cache.online[userid]->write(method::saddfd_suc, value);
  } else {
    if(Service::cache.online[userid])
      Service::cache.online[userid]->write(method::saddfd_err, value);
  }
}
