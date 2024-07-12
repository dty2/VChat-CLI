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
  // 查询
  findfd,
  findfd_suc,
  findfd_err,
  // 添加好友
  addfd,
  accept_addfd,
  refuse_addfd,
  // 删除好友
  deletefd,
  // 发消息
  sendmsg,
};

} // namespace method

namespace packer {
// enpack
std::string enpack(int, Json::Value);
std::string enpack(int);
// depack
std::pair<int, int> depackhead(char *); // return headmethod, bodysize
Json::Value depackbody(char *, int);
} // namespace packer

} // namespace vchat

#endif // PACKAGE_H
