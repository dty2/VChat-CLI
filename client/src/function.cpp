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
    postevent(value["error"].asString());
    break;
  case method::login_suc:
    this->handle_login(value);
    LOG(INFO) << "receive log info";
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

    postevent("saddfd");
    break;
  case method::saddfd_suc:
    this->handle_saddfd(value);
    postevent("saddfd_suc");
    break;
  case method::saddfd_err:
    postevent(value["error"].asString());
    break;
  case method::sdeletefd:
    postevent("sdeletedfd");
    break;
  case method::ssendmsg:
    DLOG(INFO) << "get ssendmsg";
    this->handle_ssendmsg(value);
    postevent("ssendmsg");
    break;
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
    for (auto& v : value["friendlist"]) {
      Info::info->userinfo.friendlist.push_back(FriendInfo(v["friendid"].asInt(), v["friendname"].asString()));
    }
    for (auto& v : value["messagelist"])
      Info::info->userinfo.messagelist.push_back(
        MessageInfo(
          v["sender"].asInt(), v["receiver"].asInt(),
          v["message"].asString(), v["time"].asInt64()
        )
      );
  });
  //DLOG(INFO) << Info::info->userinfo.persionalinfo.id;
  //DLOG(INFO) << Info::info->userinfo.persionalinfo.password;
  //DLOG(INFO) << Info::info->userinfo.persionalinfo.username;
  //DLOG(INFO) << Info::info->userinfo.friendlist.size();
  //DLOG(INFO) << Info::info->userinfo.messagelist.size();
  //for(auto& v : Info::info->userinfo.friendlist) // 注意这里要使用&引用
  //  DLOG(INFO) << v.friendid << v.friendname;
  //for(auto& v : Info::info->userinfo.messagelist)
  //  DLOG(INFO) << v.sender << v.receiver << v.msg << v.time;
}

void Function::handle_find(Json::Value &value) {}

void Function::handle_ssendmsg(Json::Value &value) {
  Info::info->change([&]{
    MessageInfo temp;
    temp.sender = value["sender"].asInt();
    temp.receiver = value["receiver"].asInt();
    temp.msg = value["message"].asString();
    temp.time = value["time"].asInt64();
    Info::info->userinfo.messagelist.push_back(temp);
  });
}

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

bool Function::sendmsg(int receiver, int sender, std::string message,
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

bool Function::responseadd(int id, bool isagree) {
  Json::Value friendinfo;
  friendinfo["userid"] = Info::info->userinfo.persionalinfo.id;
  friendinfo["friendid"] = id;
  int signal = 0;
  if (isagree) {
    if (!(signal = net.write(method::caddfd_suc, friendinfo))) {
      return 0;
    } else return signal;
  } else {
    if (!(signal = net.write(method::caddfd_err, friendinfo))) {
      return 0;
    } else return signal;
  }
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
