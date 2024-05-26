#include "info.h"

namespace vchat {

Info* Info::info = nullptr;
Info::Info() {};

void Info::getinstance() {
  info = new Info();
}

void Info::info_write(std::function<void()> op) {
  lock.lock();
  op();
  lock.unlock();
}

void Info::info_read() {
}

} // namespace vchat
