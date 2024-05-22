#include "store.h"

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

bool Store::getPersional(PersionalInfo& persionalinfo, const int id) {
  try {
    SQLite::Statement query(*this->db, "SELECT * FROM userinfo where id == ?");
    query.bind(1, std::to_string(id));
    persionalinfo.id = query.getColumn(0);
    persionalinfo.password = query.getColumn(1);
    persionalinfo.username = std::string(query.getColumn(2));
  } catch (std::exception& e) {
    LOG(INFO) << "exception: " << e.what() << "\n";
  } 
  if(persionalinfo.id && persionalinfo.password && persionalinfo.username.empty()) return true;
  else return false;
}

bool Store::getFriend(std::list<FriendInfo>& friendlist, const int id) {
  try {
    SQLite::Statement query(*this->db, "SELECT * FROM friendinfo WHERE id == ?");
    query.bind(1, std::to_string(id));
    while (query.executeStep())
      friendlist.push_back(FriendInfo(query.getColumn(0)));
  } catch (std::exception& e) {
    LOG(INFO) << "exception: " << e.what() << "\n";
  }
  return friendlist.empty();
}

bool Store::getMessage(std::list<MessageInfo>& messagelist, const int id) {
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
  return messagelist.empty();
}

bool Store::getUser(UserInfo& userinfo, const int id) {
  bool get1 = getPersional(userinfo.persionalinfo, id);
  bool get2 = getFriend(userinfo.friendlist, id);
  bool get3 = getMessage(userinfo.messagelist, id);
  if(get1 && get2 && get3) return true;
  else return false;
}

bool Store::insertPersional(PersionalInfo& persionalinfo) {
  try {
    SQLite::Statement query(*this->db, "INSERT INTO userinfo VALUES (?, ?, ?)");
    query.bind(1, persionalinfo.id);
    query.bind(2, persionalinfo.password);
    query.bind(3, persionalinfo.username);
    query.exec();
    return true;
  } catch (std::exception& e) {
    LOG(INFO) << "exception: " << e.what() << "\n";
    return false;
  } 
}

bool Store::insertFriend(FriendInfo& friendinfo) {
  try {
    SQLite::Statement query(*this->db, "INSERT INTO friendlist VALUES (?)");
    query.bind(1, friendinfo.friendid);
    query.exec();
    return true;
  } catch (std::exception& e) {
    LOG(INFO) << "exception: " << e.what() << "\n";
    return false;
  }
}

bool Store::insertMessage(MessageInfo& messageinfo) {
  try {
    SQLite::Statement query(*this->db, "INSERT INTO friendlist VALUES (?, ?, ?)");
    query.bind(1, messageinfo.sender);
    query.bind(2, messageinfo.receiver);
    query.bind(3, messageinfo.msg);
    query.exec();
    return true;
  } catch (std::exception& e) {
    LOG(INFO) << "exception: " << e.what() << "\n";
    return false;
  }
}
