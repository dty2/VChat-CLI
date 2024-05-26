#ifndef INFO_H
#define INFO_H

#include "common.h"
#include "data.hpp"

namespace vchat {

class Info {
public:
  static Info* info;
  UserInfo userinfo;
  std::unique_lock<std::mutex> lock;

  static void getinstance();
  Info(const Info&) = delete;
  Info& operator=(const Info&) = delete;
  void info_write(std::function<void()>);
  void info_read();

private:
  Info();

};

} // namespace vchat

#endif // INFO_H

