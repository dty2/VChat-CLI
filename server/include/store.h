#ifndef STORE_H
#define STORE_H

#include <memory>
#include <SQLiteCpp/SQLiteCpp.h>
#include "common.h"
#include "data.hpp"

class Store {
private:
  const std::string db_address = "/home/hunter/project/vchat/resource/sql/vchat.db";
  std::unique_ptr<SQLite::Database> db;
public:
  Store();
  void getPersional(PersionalInfo&, const int);
  void getFriend(std::list<FriendInfo>&, const int);
  void getMessage(std::list<MessageInfo>&, const int);
  void getUser(UserInfo&, const int);
};

#endif // STORE_H
