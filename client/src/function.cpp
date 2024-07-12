#include "function.h"
#include "net.h"
#include "package.h"

namespace vchat {

Function::Function()
  : net(std::bind(&Function::handle, this, std::placeholders::_1, std::placeholders::_2)) {}

void Function::start() {
  net.start();
}

void Function::end() {
  this->net.close();
}

void Function::handle(int op, Json::Value value) {
  LOG(INFO) << "handle op and value";
  switch (op) {
  case method::signin_suc:
    postevent("signin_suc");
    break;
  case method::signin_err:
    postevent("signin_err");
    break;
  case method::login_suc:
    this->handle_login(value);
    postevent("login_suc");
    break;
  case method::login_err:
    postevent("login_err");
    break;
  case method::findfd_suc:
    postevent("findfd_suc");
    break;
  case method::findfd_err:
    postevent("findfd_err");
    break;
  case method::addfd:
    postevent("addfd");
    break;
  case method::accept_addfd:
    this->handle_addfd(value);
    postevent("accept_addfd");
    break;
  case method::refuse_addfd:
    postevent("refuse_addfd");
    break;
  case method::deletefd:
    postevent("deletefd");
    break;
  case method::sendmsg:
    this->handle_msg(value);
    postevent("sendmsg");
    break;
  }
}

void Function::handle_login(Json::Value &value) {
  Info::info->change([&] {
    Info::info->myself.id = value["persionalinfo"]["id"].asInt();
    Info::info->myself.password = value["persionalinfo"]["password"].asInt();
    Info::info->myself.username = value["persionalinfo"]["username"].asString();
    for (auto& v : value["friendlist"]) {
      int id = v["friendid"].asInt();
      std::string name = v["friendname"].asString();
      Info::info->friendinfo[id] = FriendInfo(id, name);
    }
    for (auto& v : value["messagelist"]) {
      int sender = v["sender"].asInt();
      int receiver = v["receiver"].asInt();
      std::string message = v["message"].asString();
      int64_t time = v["time"].asInt64();
      int id = sender != Info::info->myself.id ? sender : receiver;
      Info::info->messageinfo[id].emplace_back(
        MessageInfo(sender, receiver, message, time)
      );
    }
  });
  DLOG(INFO) << Info::info->myself.id;
  DLOG(INFO) << Info::info->myself.password;
  DLOG(INFO) << Info::info->myself.username;
  DLOG(INFO) << Info::info->friendinfo.size();
  DLOG(INFO) << Info::info->messageinfo.size();
  for(auto& v : Info::info->friendinfo)
    DLOG(INFO) << v.first << v.second.friendname;
  for(auto& v : Info::info->messageinfo) {
    DLOG(INFO) << v.first;
    for(auto& x : v.second)
      DLOG(INFO) << x.sender << x.receiver << x.msg << x.time;
  }
}

void Function::handle_find(Json::Value &value) {}

void Function::handle_msg(Json::Value &value) {
  Info::info->change([&]{
    MessageInfo temp;
    temp.sender = value["sender"].asInt();
    temp.receiver = value["receiver"].asInt();
    temp.msg = value["message"].asString();
    temp.time = value["time"].asInt64();
    int id = temp.sender == Info::info->myself.id ? temp.sender : temp.receiver;
    Info::info->messageinfo[id].emplace_back(temp);
  });
}

void Function::handle_addfd(Json::Value &value) {
  Info::info->change([&] {
    FriendInfo temp;
    temp.friendid = value["friendid"].asInt();
    Info::info->friendinfo[temp.friendid] = temp;
  });
}

bool Function::login(int id, int password) {
  DLOG(INFO) << "start login";
  Json::Value loginfo;
  loginfo["id"] = id;
  loginfo["password"] = password;
  int signal = 0;
  if (!(signal = net.write(method::login, loginfo))) {
    return 0;
  } else
    return signal;
  return 0;
}

bool Function::logout(int id) {
  DLOG(INFO) << "start login";
  Json::Value loginfo;
  loginfo["id"] = id;
  int signal = 0;
  if (!(signal = net.write(method::logout, loginfo))) {
    return 0;
  } else
    return signal;
  return 0;
}

bool Function::signin(int id, int password, std::string username) {
  DLOG(INFO) << "start signin";
  Json::Value signinfo;
  signinfo["id"] = id;
  signinfo["password"] = password;
  signinfo["username"] = username;
  int signal = 0;
  if (!(signal = net.write(method::signin, signinfo))) {
    return 0;
  } else {
    return signal;
  }
  return 0;
}

bool Function::signout(int id, int password) {
  DLOG(INFO) << "signout";
  Json::Value signinfo;
  signinfo["id"] = id;
  signinfo["password"] = password;
  int signal = 0;
  if (!(signal = net.write(method::signout, signinfo))) {
    return 0;
  } else {
    return signal;
  }
  return 0;
}

bool Function::sendmsg(int receiver, int sender, std::string message,
                       int64_t time) {
  Json::Value chatinfo;
  chatinfo["sender"] = sender;
  chatinfo["receiver"] = receiver;
  chatinfo["message"] = message;
  chatinfo["time"] = time;
  int signal = 0;
  if (!(signal = net.write(method::sendmsg, chatinfo))) {
    Info::info->change([&] {
      DLOG(INFO) << "chat info write to info";
      MessageInfo temp;
      temp.sender = sender;
      temp.receiver = receiver;
      temp.msg = message;
      temp.time = time;
      Info::info->messageinfo[receiver].emplace_back(temp);
    });
    return 0;
  } else
    return signal;
  return 0;
}

bool Function::find(int id) {
  Json::Value friendinfo;
  friendinfo["userid"] = Info::info->myself.id;
  friendinfo["friendid"] = id;
  int signal = 0;
  if (!(signal = net.write(method::findfd, friendinfo))) {
    return 0;
  } else
    return signal;
  return 0;
}

bool Function::addfriend(int id) {
  Json::Value friendinfo;
  friendinfo["userid"] = Info::info->myself.id;
  friendinfo["friendid"] = id;
  int signal = 0;
  if (!(signal = net.write(method::addfd, friendinfo))) {
    return 0;
  } else
    return signal;
  return 0;
}

bool Function::responseadd(int id, bool isagree) {
  Json::Value friendinfo;
  friendinfo["userid"] = Info::info->myself.id;
  friendinfo["friendid"] = id;
  int signal = 0;
  if (isagree) {
    if (!(signal = net.write(method::accept_addfd, friendinfo))) {
      return 0;
    } else return signal;
  } else {
    if (!(signal = net.write(method::refuse_addfd, friendinfo))) {
      return 0;
    } else return signal;
  }
  return 0;
}

bool Function::deletefriend(int id) {
  Json::Value friendinfo;
  friendinfo["userid"] = Info::info->myself.id;
  friendinfo["friendid"] = id;
  int signal = 0;
  if (!(signal = net.write(method::deletefd, friendinfo))) {
    return 0;
  } else
    return signal;
  return 0;
}

} // namespace vchat
