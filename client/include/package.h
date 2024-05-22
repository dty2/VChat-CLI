#ifndef PACKAGE_H
#define PACKAGE_H

#include "common.h"

namespace vchat {

enum request {
  login = 1, logout,
  signin, signout,
  chat,
  addfriend, deletefriend
};

enum response {
  login_success = 101,
  login_error = 401,
  logout_success = 102,
  logout_error = 402,
  signin_success = 103,
  signin_error = 403,
  signout_success = 104,
  signout_error = 404,
  chat_success = 105,
  chat_error = 405,
  addfriend_success = 106,
  addfriend_error = 406,
  deletefriend_success = 107,
  deletefriend_error = 407,
};

typedef struct Head {
  bool type;
  int size;
  int method;
  // serialization
  friend class boost::serialization::access;
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int) {
    ar & type;
    ar & size;
    ar & method;
  }
}Head;

namespace packer {
int getheadsize();
// enpack
std::string enpack(int, Json::Value);
// depack
Head depackhead(std::shared_ptr<std::string>);
Json::Value depackbody(std::shared_ptr<std::string>);
} // namespace packer

} // namespace vchat

#endif // PACKAGE_H
