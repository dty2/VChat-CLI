#include "function.h"

namespace vchat {

Function* Function::function = nullptr;

Function::Function() {}
Function::~Function() {
  free(function);
}

void Function::getinstance() {
  function = new Function();
}

bool Function::do_login(int id, int password) {
  LOG(INFO) << "do_login" << '\n';
  Json::Value loginfo;
  loginfo["id"] = id;
  loginfo["password"] = password;
  packer::enpack(login, loginfo);
  Connection::connection->do_write(login, loginfo);
  std::unique_lock<std::mutex> lock(Info::info->mtx);
  Info::info->cv.wait(lock, [&]{ return Info::info->dataReady; });
  if(Info::info->userinfo.persionalinfo.username.empty())
    return false;
  else return true;
}

bool Function::do_signin(int id, int password, std::string username) {
  Json::Value signinfo;
  signinfo["id"] = id;
  signinfo["password"] = password;
  signinfo["username"] = password;
  packer::enpack(signin, signinfo);
  Connection::connection->do_write(signin, signinfo);
  std::unique_lock<std::mutex> lock(Info::info->mtx);
  Info::info->cv.wait(lock, [&]{ return Info::info->dataReady; });
  if(Info::info->userinfo.persionalinfo.username.empty())
    return false;
  else return true;
}

void Function::do_chat(int sender, int receiver, std::string message, int time) {
  Json::Value chatinfo;
  chatinfo["sender"] = sender;
  chatinfo["receiver"] = receiver;
  chatinfo["message"] = message;
  chatinfo["time"] = time;
  packer::enpack(chat, chatinfo);
  Connection::connection->do_write(chat, chatinfo);
}

void Function::do_addfriend(int id) {
  Json::Value friendinfo;
  friendinfo["friendid"] = id;
  packer::enpack(addfriend, friendinfo);
  Connection::connection->do_write(addfriend, friendinfo);
}

} // namespace vchat
