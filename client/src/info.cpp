#include "info.h"

namespace vchat {

Info* Info::info = nullptr;
Info::Info() {};

void Info::getinstance() {
  info = new Info();
}

void Info::opinfo(std::function<void()> op) {
  std::lock_guard<std::mutex> lock(mtx);
  LOG(INFO) << "start info write";
  op();
  LOG(INFO) << "finish info write";
  dataReady = true;
  cv.notify_one();
  LOG(INFO) << "ready new data";
}

} // namespace vchat
