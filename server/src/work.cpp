#include "work.h"

namespace vchat {

WorkManager::WorkManager() {}

WorkManager* WorkManager::getInstance() {
  WorkManager* instance = new WorkManager();
  return instance;
}

void WorkManager::push_work(Head head, Json::Value value, std::function<void(int, Json::Value)> callback) {
  LOG(INFO) << head.size << ' ' << head.type << ' ' << head.method;
  LOG(INFO) << "push a work";
  if(!head.type) {
    switch (head.method) {
      case login        : Service::service->do_login(value, callback);        break;
      case signin       : Service::service->do_signin(value, callback);       break;
      case chat         : Service::service->do_chat(value, callback);         break;
      case addfriend    : Service::service->do_addfriend(value, callback);    break;
      default: LOG(ERROR) << "no request service error" << '\n';
    }
  }
}

} // namespace vchat
