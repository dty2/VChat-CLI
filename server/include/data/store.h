#ifndef STORE_H
#define STORE_H

#include "data.hpp"
#include "common.h"

class Store {
private:
  const std::string db_address = "/home/hunter/project/vchat/resource/sql/vchat.db";
  std::unique_ptr<SQLite::Database> db;
  Store();
public:
  static Store* store;
  static Store* getInstance();
  Store(const Store&) = delete;
  Store& operator=(const Store&) = delete;

  bool getPersional(PersionalInfo&, const int);
  bool getFriend(std::list<FriendInfo>&, const int);
  bool getMessage(std::list<MessageInfo>&, const int);
  bool getUser(UserInfo&, const int);
  bool insertPersional(PersionalInfo&);
  bool insertFriend(FriendInfo&, int);
  bool insertMessage(MessageInfo&);
};

#endif // STORE_H
