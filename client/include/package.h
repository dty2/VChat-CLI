#ifndef PACKAGE_H
#define PACKAGE_H

#include "common.h"
#include "data.hpp"

namespace vchat {

enum request {
  login = 1,
  signin,
  chat,
  addfriend
};

enum response {
  login_success = 101,
  signin_success = 103,
  chat_success = 105,
  addfriend_success = 106,
};

typedef struct Head {
  bool type = 0;
  int size = 0;
  int method = 0;
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
