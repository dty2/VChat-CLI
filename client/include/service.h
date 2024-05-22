#ifndef SERVICE_H
#define SERVICE_H

#include "common.h"
#include "package.h"
#include "store.h"

namespace vchat {

class Service {
private:
  // online user
  std::unordered_set<int> online;
  static std::unordered_map<int, MessageInfo> messagelist;
  Service();
public:
  static Service* service;
  static Service* getInstance();
  Service(const Service&) = delete;
  Service& operator=(const Service&) = delete;

  // request service
  void do_login(Json::Value, std::function<void(int, Json::Value)>);
  void do_logout(Json::Value, std::function<void(int, Json::Value)>);
  void do_signin(Json::Value, std::function<void(int, Json::Value)>);
  void do_signout(Json::Value, std::function<void(int, Json::Value)>);
  void do_chat(Json::Value, std::function<void(int, Json::Value)>);
  void do_addfriend(Json::Value, std::function<void(int, Json::Value)>);
  void do_deletefriend(Json::Value, std::function<void(int, Json::Value)>);

};

} // namespace vchat

#endif // SERVICE_H
