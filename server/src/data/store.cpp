/*
 * Copyright (c) 2024 Hunter 执着
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "store.h"
#include "server.h"

Store* Store::store = nullptr;

void Store::getinstance() { store = new Store(); }

void Store::free() { delete store; }

Store::Store() {
  try {
    this->db = std::make_unique<SQLite::Database>(
      db_address,
      SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
    );
  }
  catch (const std::exception& e) {
    LOG(INFO) << "database error";
    exit(0);
  }
}

bool Store::getPersional(PersionalInfo& persionalinfo, const int id) {
  try {
    SQLite::Statement query(*this->db, "SELECT * FROM userinfo WHERE id = ?");
    query.bind(1, id);
    while(query.executeStep()) {
      persionalinfo.id = query.getColumn(0).getInt();
      persionalinfo.password = query.getColumn(1).getInt();
      persionalinfo.username = query.getColumn(2).getString();
    }
    if(persionalinfo.id && persionalinfo.password && persionalinfo.username.empty()) return true;
    else return false;
  } catch (std::exception& e) {
    return false;
  }
}

bool Store::getFriend(std::list<FriendInfo>& friendlist, const int id) {
  try {
    SQLite::Statement query(*this->db, "SELECT * FROM friendinfo WHERE id = ?");
    query.bind(1, id);
    while (query.executeStep())
      friendlist.push_back(FriendInfo(query.getColumn(1).getInt(), query.getColumn(2).getString()));
    return friendlist.empty();
  } catch (std::exception& e) {
    return false;
  }
}

bool Store::getMessage(std::list<MessageInfo>& messagelist, const int id) {
  try {
    SQLite::Statement querysid(*this->db, "SELECT * FROM messageinfo WHERE sender = ? OR receiver = ?");
    querysid.bind(1, id);
    querysid.bind(2, id);
    while (querysid.executeStep()) {
      int sender = querysid.getColumn(0).getInt();
      int receiver = querysid.getColumn(1).getInt();
      std::string msg = querysid.getColumn(2).getString();
      int64_t time = querysid.getColumn(3).getInt64();
      messagelist.push_back(MessageInfo(sender, receiver, msg, time));
    }
    return messagelist.empty();
  } catch (std::exception& e) {
    return false;
  }
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
    return false;
  } 
}

bool Store::insertFriend(FriendInfo& friendinfo, int userid) {
  try {
    SQLite::Statement query(*this->db, "INSERT INTO friendinfo VALUES (?, ?, ?)");
    query.bind(1, userid);
    query.bind(2, friendinfo.friendid);
    query.bind(3, friendinfo.friendname);
    query.exec();
    return true;
  } catch (std::exception& e) {
    return false;
  }
}

bool Store::insertMessage(MessageInfo& messageinfo) {
  try {
    SQLite::Statement query(*this->db, "INSERT INTO messageinfo VALUES (?, ?, ?, ?)");
    query.bind(1, messageinfo.sender);
    query.bind(2, messageinfo.receiver);
    query.bind(3, messageinfo.msg);
    query.bind(4, messageinfo.time);
    query.exec();
    return true;
  } catch (std::exception& e) {
    return false;
  }
}
