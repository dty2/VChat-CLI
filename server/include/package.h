#ifndef PACKAGE_H
#define PACKAGE_H

#include "common.h"

namespace vchat {

enum request {
  login = 100,
  signin = 200,
  chat = 300,
  addfriend = 400
};

enum response {
  login_success = 101,
  chat_success = 301,
  addfriend_success = 401,
};

typedef struct Head {
  bool type = 0;
  int size = 0;
  int method = 0;
}Head;

namespace packer {
int getheadsize();
// enpack
std::string enpack(int, Json::Value);
// depack
Head depackhead(char*);
Json::Value depackbody(char*, int);
} // namespace packer

} // namespace vchat

#endif // PACKAGE_H
