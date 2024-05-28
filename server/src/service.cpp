#include "service.h"

namespace vchat {

Service* Service::service = nullptr;

Service::Service() {
  Store::store = Store::getInstance();
}

Service* Service::getInstance() {
  Service* instance = new Service();
  return instance;
}

void Service::do_login(Json::Value value, std::function<void(int, Json::Value)> callback) {
  LOG(INFO) << "someone login";
  int id = value["id"].asInt();
  int password = value["password"].asInt();
  LOG(INFO) << "id: " << id << "password: " << password;
  PersionalInfo persionalinfo;
  Store::store->getPersional(persionalinfo, id);
  if(persionalinfo.password == password) {
    UserInfo userinfo;
    Store::store->getUser(userinfo, id);
    Json::Value root, persionalinfo, friendlist, messagelist;
    persionalinfo["id"] = userinfo.persionalinfo.id;
    persionalinfo["password"] = userinfo.persionalinfo.password;
    persionalinfo["username"] = userinfo.persionalinfo.username;
    for(auto x : userinfo.friendlist) {
      Json::Value friendinfo;
      friendinfo["id"] = x.friendid;
      friendlist.append(friendinfo);
    }
    for(auto x : userinfo.messagelist) {
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
    online.insert(id);
    callback(login_success, root);
  }
}

void Service::do_signin(Json::Value value, std::function<void(int, Json::Value)> callback) {
  PersionalInfo persionalinfo;
  persionalinfo.id = value["id"].asInt();
  persionalinfo.password = value["password"].asInt();
  persionalinfo.username = value["username"].asString();
  bool op = Store::store->insertPersional(persionalinfo);
  if(op) { callback(signin_success, value); }
}

void Service::do_chat(Json::Value value, std::function<void(int, Json::Value)> callback) {
  MessageInfo messageinfo;
  messageinfo.sender = value["sender"].asInt();
  messageinfo.receiver = value["receiver"].asInt();
  messageinfo.msg = value["message"].asString();
  messageinfo.time = value["time"].asInt();
  bool op = Store::store->insertMessage(messageinfo);
  if(op) { callback(chat_success, value); }
}

void Service::do_addfriend(Json::Value value, std::function<void(int, Json::Value)> callback) {
}

void Service::do_deletefriend(Json::Value value, std::function<void(int, Json::Value)> callback) {
}

} // namespace vchat
