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
  int id = value["id"].asInt();
  int password = value["password"].asInt();
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
      Json::Value e;
      e["friendid"] = x.friendid;
      friendlist.append(e);
    }
    for(auto x : userinfo.messagelist) {
      Json::Value e;
      e["sender"] = x.sender;
      e["receiver"] = x.receiver;
      e["message"] = x.msg;
      messagelist.append(e);
    }
    root.append(persionalinfo);
    root.append(friendlist);
    root.append(messagelist);
    callback(login_success, root);
  } else {
    Json::Value root;
    callback(login_error, root);
  }
  online.insert(id);
}

void Service::do_logout(Json::Value value, std::function<void(int, Json::Value)> callback) {
  int id = value["id"].asInt();
  if(online.find(19) != online.end()) {
    online.erase(19);
    Json::Value root;
    callback(logout_success, root);
  } else {
    Json::Value root;
    callback(logout_error, root);
  }
}

void Service::do_signin(Json::Value value, std::function<void(int, Json::Value)> callback) {
  PersionalInfo persionalinfo;
  persionalinfo.id = value["id"].asInt();
  persionalinfo.password = value["password"].asInt();
  persionalinfo.username = value["username"].asString();
  bool op = Store::store->insertPersional(persionalinfo);
  Json::Value root;
  if(op) { callback(signin_success, root); }
  else { callback(signin_error, root); }
}

void Service::do_signout(Json::Value value, std::function<void(int, Json::Value)> callback) {
  int id = value["id"].asInt();
  PersionalInfo persionalinfo;
  bool op = Store::store->getPersional(persionalinfo, id);
  Json::Value root;
  if(op) { callback(signout_success, root); }
  else { callback(signout_error, root); }
}

void Service::do_chat(Json::Value value, std::function<void(int, Json::Value)> callback) {
  MessageInfo messageinfo;
  messageinfo.sender = value["sender"].asInt();
  messageinfo.receiver = value["receiver"].asInt();
  messageinfo.msg = value["message"].asString();
  bool op = Store::store->insertMessage(messageinfo);
  if(op) { callback(chat_success, value); }
  else { callback(chat_error, value); }
}

void Service::do_addfriend(Json::Value value, std::function<void(int, Json::Value)> callback) {
}

void Service::do_deletefriend(Json::Value value, std::function<void(int, Json::Value)> callback) {
}

} // namespace vchat
