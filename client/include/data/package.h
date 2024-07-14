#ifndef PACKAGE_H
#define PACKAGE_H

#include "common.h"
#include "data.hpp"

namespace vchat {

namespace method {
enum method {
  // 注册
  signin = 100,
  signin_suc, // 101
  signin_err, // 102
  signout, // 103
  // 登陆
  login, // 104
  login_suc, // 105
  login_err, // 106
  logout, // 107
  // 查询
  findfd, // 108
  findfd_suc, // 109
  findfd_err, // 110
  // 添加好友
  addfd, // 111
  accept_addfd, // 112
  refuse_addfd, // 113
  // 删除好友
  deletefd, // 114
  // 发消息
  sendmsg, // 115
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
