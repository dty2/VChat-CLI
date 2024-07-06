#ifndef SERVICE_H
#define SERVICE_H

#include "common.h"
#include "connection.h"
#include "package.h"
#include "store.h"

class Connection;

class Service {
private:
  Service();
  static Service* service;
  static struct Cache {
    std::map<int, Connection*> online;
    std::vector<std::pair<int, int>> addfriendapply;
  } cache;
  // serve
  void login(Json::Value, Connection*);
  //void logout(Json::Value, Connection*);
  void signin(Json::Value, Connection*);
  //void signout(Json::Value, Connection*);
  void sendmsg(Json::Value);
  //void find(Json::Value, Connection*);
  void addfriend(Json::Value, Connection*);
  void addfriend(int, Json::Value, Connection*);
  //void deletefriend(Json::Value, Connection*);

public:
  static void getinstance();
  Service(const Service&) = delete;
  Service& operator=(const Service&) = delete;
  static void serve(int, Json::Value, Connection*);

};

#endif // SERVICE_H

