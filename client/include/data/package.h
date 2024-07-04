#ifndef PACKAGE_H
#define PACKAGE_H

#include "common.h"
#include "data.hpp"

namespace vchat {

namespace method {
enum method {
  // 注册
  signin = 100,
  signin_suc,
  signin_err,
  signout,
  // 登陆
  login,
  login_suc,
  login_err,
  logout,
  // 好友
  // 查询
  findfd,
  findfd_suc,
  findfd_err,
  // 添加
  caddfd,
  saddfd,
  caddfd_suc,
  caddfd_err,
  saddfd_suc,
  saddfd_err,
  // 删除
  cdeletefd,
  sdeletefd,
  // 发消息
  csendmsg,
  ssendmsg,
};

} // namespace method

namespace packer {
// enpack
std::string enpack(int, Json::Value);
std::string enpack(int);
// depack
std::pair<int, int> depackhead(char *);
Json::Value depackbody(char *, int);
} // namespace packer

} // namespace vchat

#endif // PACKAGE_H
