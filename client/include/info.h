#ifndef INFO_H
#define INFO_H

#include "common.h"
#include "data.hpp"

namespace vchat {

class Info {
public:
  static Info* info;
  UserInfo userinfo;
  std::mutex mtx;
  std::condition_variable cv;
  bool dataReady = false;

  static void getinstance();
  Info(const Info&) = delete;
  Info& operator=(const Info&) = delete;
  void opinfo(std::function<void()>);

private:
  Info();

};

} // namespace vchat

#endif // INFO_H

