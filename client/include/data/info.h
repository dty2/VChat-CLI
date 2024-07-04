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
  UserInfo userinfo;
  static Info *info;
  static void getinstance();
  void change(std::function<void()>);
};

} // namespace vchat

#endif // INFO_H
