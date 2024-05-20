#ifndef USERINFO_HPP
#define USERINFO_HPP

#include <string>
#include <list>
#include <boost/serialization/access.hpp>
#include <boost/serialization/list.hpp>

/*
 * struct for transmission message
 */
struct MessageInfo {
  int sender;
  int receiver;
  std::string msg;
  MessageInfo(int sender_, int receiver_, std::string msg_) :
    sender(sender_),
    receiver(receiver_),
    msg(msg_) {}
  // serialization
  friend class boost::serialization::access;
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int) {
      ar & sender;
      ar & receiver;
      ar & msg;
  }
};

/*
 * struct for transmission friendinfo 
 */
struct FriendInfo {
  int friendid;
  FriendInfo(int id_) : friendid(id_) {}
  // serialization
  friend class boost::serialization::access;
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int) {
    ar & friendid;
  }
};

/*
 * struct for transmission userinfo
 */

struct PersionalInfo {
  int id;
  int password;
  std::string username;
  PersionalInfo(int id_, int password_, std::string username_) :
    id(id_),
    password(password_),
    username(username_) {}
  // serialization
  friend class boost::serialization::access;
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int) {
    ar & id;
    ar & password;
    ar & username;
  }
};

struct UserInfo {
  PersionalInfo persionalinfo;
  std::list<FriendInfo> friendlist;
  std::list<MessageInfo> messagelist;
  // serialization
  friend class boost::serialization::access;
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int) {
    ar & persionalinfo;
    ar & friendlist;
    ar & messagelist;
  }
};

#endif // USERINFO_HPP
