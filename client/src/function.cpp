#include "function.h"
#include "package.h"

namespace vchat {

void Function::start() {
  if (!net.connect()) {
    this->mode = true;
    this->handle();
  }
}

void Function::end() { this->mode = false; }

void Function::handle() {
  while (mode) {
    DLOG(INFO) << "start handle";
    int op, signal;
    Json::Value value;
    if (!(signal = net.read(op, value))) {
      switch (op) {
      case method::signin_suc:
        postevent("signin_suc");
        break;
      case method::signin_err:
        postevent(value["error"].asString());
        break;
      case method::login_suc:
        this->handle_login(value);
        postevent("login_suc");
        break;
      case method::login_err:
        postevent(value["error"].asString());
        break;
      case method::findfd_suc:
        postevent("resource_findfd_suc");
        break;
      case method::findfd_err:
        postevent(value["error"].asString());
        break;
      case method::saddfd:
        postevent("resource_saddfd");
        break;
      case method::saddfd_suc:
        this->handle_saddfd(value);
        postevent("saddfd_suc");
        break;
      case method::saddfd_err:
        postevent(value["error"].asString());
        break;
      case method::sdeletefd:
        postevent("resource_sdeletedfd");
        break;
      case method::ssendmsg:
        this->handle_ssendmsg(value);
        postevent("ssendmsg");
        break;
      }
    } else
      LOG(ERROR) << signal;
  }
}

void Function::handle_login(Json::Value &value) {
  Info::info->change([&] {
    Info::info->userinfo.persionalinfo.id =
        value["persionalinfo"]["id"].asInt();
    Info::info->userinfo.persionalinfo.password =
        value["persionalinfo"]["password"].asInt();
    Info::info->userinfo.persionalinfo.username =
        value["persionalinfo"]["username"].asString();
    for (auto v : value["friendlist"])
      Info::info->userinfo.friendlist.push_back(
          FriendInfo(v["friendid"].asInt(), v["friendname"].asString()));
    for (auto v : value["messagelist"])
      Info::info->userinfo.messagelist.push_back(
          MessageInfo(v["sender"].asInt(), v["receiver"].asInt(),
                      v["message"].asString(), v["time"].asInt64()));
  });
}

void Function::handle_find(Json::Value &value) {}
void Function::handle_ssendmsg(Json::Value &value) {}

void Function::handle_saddfd(Json::Value &value) {
  Info::info->change([&] {
    FriendInfo temp;
    temp.friendid = value["friendid"].asInt();
    Info::info->userinfo.friendlist.push_back(temp);
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

bool Function::sendmsg(int sender, int receiver, std::string message,
                       int64_t time) {
  DLOG(INFO) << "sendmsg";
  Json::Value chatinfo;
  chatinfo["sender"] = sender;
  chatinfo["receiver"] = receiver;
  chatinfo["message"] = message;
  chatinfo["time"] = time;
  int signal = 0;
  if (!(signal = net.write(method::csendmsg, chatinfo))) {
    Info::info->change([&] {
      DLOG(INFO) << "chat info write to info";
      MessageInfo temp;
      temp.sender = sender;
      temp.receiver = receiver;
      temp.msg = message;
      temp.time = time;
      Info::info->userinfo.messagelist.push_back(temp);
    });
    return 0;
  } else
    return signal;
  return 0;
}

bool Function::find(int id) {
  Json::Value friendinfo;
  friendinfo["userid"] = Info::info->userinfo.persionalinfo.id;
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
  friendinfo["userid"] = Info::info->userinfo.persionalinfo.id;
  friendinfo["friendid"] = id;
  int signal = 0;
  if (!(signal = net.write(method::caddfd, friendinfo))) {
    return 0;
  } else
    return signal;
  return 0;
}

bool Function::deletefriend(int id) {
  Json::Value friendinfo;
  friendinfo["userid"] = Info::info->userinfo.persionalinfo.id;
  friendinfo["friendid"] = id;
  int signal = 0;
  if (!(signal = net.write(method::cdeletefd, friendinfo))) {
    return 0;
  } else
    return signal;
  return 0;
}

} // namespace vchat
