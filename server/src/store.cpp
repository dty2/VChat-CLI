#include "store.h"
#include <list>

Store::Store() {
  try {
    this->db = std::make_unique<SQLite::Database>(
      db_address,
      SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE
    );
  }
  catch (const std::exception& e) {
    LOG(INFO) << "Exception: " << e.what() << "\n";
  }
}

void Store::getPersional(PersionalInfo& persionalinfo, const int id) {
  try {
    SQLite::Statement query(*this->db, "SELECT * FROM userinfo where id == ?");
    query.bind(1, std::to_string(id));
    persionalinfo.id = query.getColumn(0);
    persionalinfo.password = query.getColumn(1);
    persionalinfo.username = std::string(query.getColumn(2));
  } catch (std::exception& e) {
    LOG(INFO) << "exception: " << e.what() << "\n";
  } 
}

void Store::getFriend(std::list<FriendInfo>& friendlist, const int id) {
  try {
    SQLite::Statement query(*this->db, "SELECT * FROM friendinfo WHERE id == ?");
    query.bind(1, std::to_string(id));
    while (query.executeStep())
      friendlist.push_back(FriendInfo(query.getColumn(0)));
  } catch (std::exception& e) {
    LOG(INFO) << "exception: " << e.what() << "\n";
  }
}

void Store::getMessage(std::list<MessageInfo>& messagelist, const int id) {
  try {
    SQLite::Statement query(*this->db, "SELECT * FROM friendinfo WHERE id == ?");
    query.bind(1, std::to_string(id));
    while (query.executeStep()) {
      int sender = query.getColumn(0);
      int receiver = query.getColumn(1);
      std::string msg = query.getColumn(2);
      messagelist.push_back(MessageInfo(sender, receiver, msg));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "exception: " << e.what() << "\n";
  }
}

void Store::getUser(UserInfo& userinfo, const int id) {
  getPersional(userinfo.persionalinfo, id);
  getFriend(userinfo.friendlist, id);
  getMessage(userinfo.messagelist, id);
}
