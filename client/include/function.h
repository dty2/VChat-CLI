#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "info.h"
#include "net.h"
#include "package.h"

namespace vchat {

class Function {
public:
  Function();
  void start();
  void end();
  bool login(int, int);
  bool logout(int);
  bool signin(int, int, std::string);
  bool signout(int, int);
  bool sendmsg(int, int, std::string, int64_t);
  bool find(int);
  bool addfriend(int);
  bool deletefriend(int);
  std::function<void(std::string)> postevent;
  //TempInfo tempinfo;

private:
  bool mode = 0;
  void handle(int, Json::Value);
  void handle_login(Json::Value&);
  void handle_find(Json::Value&);
  void handle_saddfd(Json::Value&);
  void handle_ssendmsg(Json::Value&);
  Net net;
};

} // namespace vchat

#endif // FUNCTION_H
