#ifndef INFO_H
#define INFO_H

#include "common.h"
#include "data.hpp"

namespace vchat {

class Info {
private:
  Info();
  Info(const Info &) = delete;
  Info &operator=(const Info &) = delete;
  std::mutex mtx;

public:
  PersionalInfo myself;
  std::map<int, FriendInfo> friendinfo;
  std::map<int, std::vector<MessageInfo>> messageinfo;
  std::map<int, Json::Value> requestaddlist; // 好友请求添加列表
  static Info *info;
  static void getinstance();
  void change(std::function<void()>);
};

} // namespace vchat

#endif // INFO_H
