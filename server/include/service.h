#ifndef SERVICE_H
#define SERVICE_H

#include "common.h"
#include "package.h"
#include "store.h"
#include "data.hpp"

namespace vchat {

class Service {
private:
  // online user
  std::unordered_set<int> online;
public:
  static Service* service;
  static Service* getInstance();
  Service(const Service&) = delete;
  Service& operator=(const Service&) = delete;

  // request service
  bool do_login(Json::Value, std::function<void(int, Json::Value)>);
  bool do_logout(Json::Value, std::function<void(int, Json::Value)>);
  bool do_signin(Json::Value, std::function<void(int, Json::Value)>);
  bool do_signout(Json::Value, std::function<void(int, Json::Value)>);
  bool do_chat(Json::Value, std::function<void(int, Json::Value)>);
  bool do_addfriend(Json::Value, std::function<void(int, Json::Value)>);
  bool do_deletefriend(Json::Value, std::function<void(int, Json::Value)>);

};

} // namespace vchat

#endif // SERVICE_H
