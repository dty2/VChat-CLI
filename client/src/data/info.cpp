#include "info.h"

namespace vchat {

Info* Info::info = nullptr;
Info::Info() {};
void Info::getinstance() {
  Info::info = new Info();
};

void Info::change(std::function<void()> callback) {
  std::lock_guard<std::mutex> lock(mtx);
  callback();
}

} // namespace vchat
