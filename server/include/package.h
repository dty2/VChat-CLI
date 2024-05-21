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
  success = 100,
  error = 400
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
