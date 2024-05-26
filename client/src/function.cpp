#include "function.h"

namespace vchat {

Function* Function::function = nullptr;
Connection* Function::connect = nullptr;

Function::Function() {}
Function::~Function() {
  free(function);
  free(connect);
}

void Function::getinstance() {
  function = new Function();
  connect = Connection::getinstance();
}

void Function::do_login(int id, int password) {
  Json::Value loginfo;
  loginfo["id"] = id;
  loginfo["password"] = password;
  packer::enpack(login, loginfo);
  connect->do_write(login, loginfo);
}

void Function::do_signin(int id, int password, std::string username) {
  Json::Value signinfo;
  signinfo["id"] = id;
  signinfo["password"] = password;
  signinfo["username"] = password;
  packer::enpack(signin, signinfo);
  connect->do_write(signin, signinfo);
}

void Function::do_chat(int sender, int receiver, std::string message) {
  Json::Value chatinfo;
  chatinfo["sender"] = sender;
  chatinfo["receiver"] = receiver;
  chatinfo["message"] = message;
  packer::enpack(chat, chatinfo);
  connect->do_write(chat, chatinfo);
}

void Function::do_addfriend(int id) {
  Json::Value friendinfo;
  friendinfo["friendid"] = id;
  packer::enpack(addfriend, friendinfo);
  connect->do_write(addfriend, friendinfo);
}

} // namespace vchat
