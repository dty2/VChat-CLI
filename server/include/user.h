#ifndef USER_H
#define USER_H

#include <string>
#include <list>
#include "data.hpp"

class User
{
private:
  int id;
  int password;
  std::string username;
  std::list<FriendInfo> friendlist;
  std::list<MessageInfo> messagelist;

public:
  int getId();
  int getPassword();
  std::string getUsername();
  FriendInfo getFriendInfo(int);
  MessageInfo getMessageInfo(int);
};

#endif // USER_H

